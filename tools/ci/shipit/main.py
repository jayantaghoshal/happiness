import os
import re
import subprocess
import time
import argparse
import logging
import logging.config
import json
from shipit.serial_mapping import open_serials, PortMapping
from shipit import serial_mapping
from shipit import recording_serial
from shipit.process_tools import check_output_logged


logger = logging.getLogger(__name__)


def boot_mp_to_elk(vip_serial: recording_serial.RecordingSerial):
    logging.info("Boot into ELK")
    vip_serial.writeline("gpio 1.10 0 o")
    vip_serial.writeline("gpio 8.10 0 o")
    vip_serial.writeline("gpio 1.10 1 o")


def boot_mp_to_android(vip_serial: recording_serial.RecordingSerial):
    logging.info("Boot into android")
    vip_serial.writeline("gpio 1.10 0 o")
    vip_serial.writeline("gpio 8.10 1 o")
    vip_serial.writeline("gpio 1.10 1 o")


def expect_line(s: recording_serial.RecordingSerial, pattern: str, timeout_sec: int):
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        line = s.readline(timeout_sec)
        if line is not None and re.match(pattern, line):
            return True
    raise RuntimeError("Timeout %d sec when expecting match on pattern: \"%s\"" % (timeout_sec, pattern))


def wait_line(s: recording_serial.RecordingSerial, pattern: str, timeout_sec: int):
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        line = s.readline(timeout_sec)
        if line is not None and re.match(pattern, line):
            return


def wait_for_dmesg_timestamp(s: recording_serial.RecordingSerial, device_timestamp_to_wait_for: int, timeout_sec: int):
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        s.writeline("\n")
        line = s.readline(timeout_sec)
        if line:
            m = re.match(r"\[(.*)\]", line)
            if m:
                numstr = m.group(1)
                num = float(numstr.strip())
                if num > device_timestamp_to_wait_for:
                    return
        time.sleep(0.5)

    raise RuntimeError("Never reached time: %d" % device_timestamp_to_wait_for)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--aosp_root_dir")
    parser.add_argument("--aic_tools_dir")
    parser.add_argument("--vip_port")
    parser.add_argument("--mp_port")
    parser.add_argument("--device_serial_nr",
                        help="The device id to use when issuing fastboot/adb commands, "
                             "find this ID using 'adb devices' or 'fastboot devices'")
    parsed_args = parser.parse_args()

    with open("logging.json", "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    try:
        build_dir = parsed_args.aosp_root_dir
        build_out_dir = os.path.join(build_dir, "out")
        logging.info("Start build")
        build_image(build_dir, parsed_args.aic_tools_dir)
        logging.info("Build completed")
        logging.info("Start flash")

        # ihu_serials = serial_mapping.auto_detect_port_mapping()
        port_mapping = PortMapping(parsed_args.vip_port, parsed_args.mp_port)
        device_serial_nr = parsed_args.device_serial_nr
        flash_image(port_mapping, device_serial_nr, build_out_dir)
        logging.info("Flash completed")
    except subprocess.CalledProcessError as cpe:
        print(cpe.output)
        print(cpe.stderr)
        raise


def build_image(build_dir: str, aic_tools_dir: str):
    buildwithdocker_command = os.path.join(aic_tools_dir, "docker", "aic", "run.sh")

    commands = [["lunch", "ihu_abl_car-eng"],
                ["make", "systemimage", "vendorimage", "-j8"]]
    run_command = " && ".join(" ".join(c) for c in commands)
    check_output_logged([buildwithdocker_command, run_command], cwd=build_dir)


def flash_image(port_mapping: PortMapping,
                device_serial_nr: str,
                build_out_dir: str):
    adb_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "adb")
    fastboot_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "fastboot")

    ihu_serials = open_serials(port_mapping)

    try:
        adb_bootmode = check_output_logged([adb_executable,
                                                "-s", device_serial_nr,
                                                "get-state"],
                                               timeout_sec=60).decode().strip(" \n\r\t")
        logger.info("Bootmode before: %s", adb_bootmode)
    except BaseException as e:
        logger.info("Could not check bootmode, probably not booted, continue anyway. Reason: %r" % e)

    try:
        #wait_for_timestamp(ihu_serials.vip, 3, 15)
        ihu_serials.vip.writeline("sm alw 1")
        expect_line(ihu_serials.vip, ".*SysM- Always_On: 1.*", 5)
        logger.info("Set boot mode to ELK")
        boot_mp_to_elk(ihu_serials.vip)
        expect_line(ihu_serials.mp, "Android", 5)
        expect_line(ihu_serials.mp, "auto-boot ...", 5)
        expect_line(ihu_serials.mp, "SB: Disabled", 5)
        expect_line(ihu_serials.mp, ">>>.*", 5)
        logger.info("ELK confirmed, boot elk to fastboot")
        ihu_serials.mp.writeline("boot elk")
        expect_line(ihu_serials.mp, "==> jump to image.*", 5)
        expect_line(ihu_serials.mp, "USB for fastboot transport layer selected", 5)
        logger.info("Fastboot confirmed")

        scan_devices_stop_time = time.time() + 10
        devices_lines = []
        while time.time() < scan_devices_stop_time:
            devices_output = check_output_logged([fastboot_executable, "devices"]).decode().strip(" \n\r\t")
            devices_lines = devices_output.split(os.linesep)
            if any(device_serial_nr in line for line in devices_lines):
                break
            time.sleep(1)
        if not any(device_serial_nr in line for line in devices_lines):
            raise RuntimeError("Device %s not found in fastboot mode" % device_serial_nr)
        logger.info("Device found via fastboot over USB")

        # NOTE: fastboot will auto wait for device to be connected, maybe don't need check above?


        #gpt_path = os.path.join(BUILD_OUT_DIR, "target", "product", "ihu_abl_car", "gpt_ihu_6_0_HIGH.img")
        #output = check_output_logged([FASTBOOT_BINARY,
        #                                  "-s", device_serial_nr,
        #                                  "flash", "gpd", gpt_path]).decode().strip(" \n\r\t")

        for image_to_erase in ["data", "misc", "metadata", "persistent"]:
            output = check_output_logged([fastboot_executable,
                                          "-s", device_serial_nr,
                                          "erase", image_to_erase]).decode().strip(" \n\r\t")

        for image_to_format in ["config", "cache"]:
            output = check_output_logged([fastboot_executable,
                                          "-s", device_serial_nr,
                                          "format", image_to_format]).decode().strip(" \n\r\t")

        for image_to_flash in ["system", "vendor", "cache", "userdata"]:
            if image_to_flash == "userdata":
                continue    #TODO: We should probably use this also...
            img_path = os.path.join(build_out_dir, "target", "product", "ihu_abl_car", image_to_flash + ".img")
            output = check_output_logged([fastboot_executable,
                                          "-s", device_serial_nr,
                                          "flash", "system", img_path]).decode().strip(" \n\r\t")

        output = check_output_logged([fastboot_executable,
                                      "-s", device_serial_nr,
                                      "reboot"]).decode().strip(" \n\r\t")

        boot_mp_to_android(ihu_serials.vip)

        logging.info("Wait for device to enter device-mode via ADB")
        # TODO: Don't use adb command line, use https://github.com/android/platform_development/blob/master/testrunner/adb_interface.py
        #      Right now we are missing platform_development repo from intel AOSP delivery
        output = check_output_logged([adb_executable,
                                      "-s", device_serial_nr,
                                      "wait-for-device"],
                                     timeout_sec=60*5).decode().strip(" \n\r\t")
        logging.info("Device confirmed to be in device-mode via ADB")

        serial_mapping.verify_serial_is_mp_android(ihu_serials.mp)

    except Exception:
        time.sleep(2)   # Nice to have output of VIP+MP shortly after exception also, in case of bad timing
        raise
    finally:
        ihu_serials.vip.close()
        ihu_serials.mp.close()


if __name__ == "__main__":
    main()

#!/usr/bin/env python3

# TODO: Don't use adb command line, use https://github.com/android/platform_development/blob/master/testrunner/adb_interface.py
#       Right now we are missing platform_development repo from intel AOSP delivery


import os
import re
import subprocess
import time
import argparse
import logging
import logging.config
import json
import socket
from shipit.serial_mapping import open_serials, PortMapping
from shipit import serial_mapping
from shipit import recording_serial
from shipit.process_tools import check_output_logged

logger = logging.getLogger(__name__)


def mp_reset_low(vip_serial: recording_serial.RecordingSerial):
    vip_serial.writeline("gpio 1.10 0 o")


def mp_reset_high(vip_serial: recording_serial.RecordingSerial):
    vip_serial.writeline("gpio 1.10 1 o")


def mp_bootpin_abl(vip_serial: recording_serial.RecordingSerial):
    vip_serial.writeline("gpio 8.10 0 o")
    vip_serial.writeline("gpio 1.2 0 o")
    vip_serial.writeline("gpio 1.3 0 o")


def mp_bootpin_prod(vip_serial: recording_serial.RecordingSerial):
    vip_serial.writeline("gpio 8.10 1 o")
    vip_serial.writeline("gpio 1.2 0 o")
    vip_serial.writeline("gpio 1.3 0 o")


def boot_mp_to_abl_cmdline(vip_serial: recording_serial.RecordingSerial):
    logging.info("Boot into ABL CMD line")

    mp_reset_low(vip_serial)
    mp_bootpin_abl(vip_serial)
    time.sleep(1)
    mp_reset_high(vip_serial)


def boot_mp_to_android(vip_serial: recording_serial.RecordingSerial):
    logging.info("Boot into android")
    mp_reset_low(vip_serial)
    mp_bootpin_prod(vip_serial)
    time.sleep(1)
    mp_reset_high(vip_serial)


def expect_line(s: recording_serial.RecordingSerial, pattern: str, timeout_sec: int, hint: str = None):
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        line = s.readline(timeout_sec)
        if line is not None and re.match(pattern, line):
            return True
    message = "Timeout %d sec when expecting match on pattern: \"%s\." % (
        timeout_sec, pattern)
    if not hint:
        message += " %d." + hint
    raise RuntimeError(message)


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


def flash_image(port_mapping: PortMapping, product: str, build_out_dir: str, update_mp: bool, update_vip: bool):
    adb_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "adb")
    fastboot_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "fastboot")

    ihu_serials = open_serials(port_mapping)
    is_vip_app = serial_mapping.verify_serial_is_vip_app(ihu_serials.vip)
    is_vip_pbl = serial_mapping.verify_serial_is_vip_pbl(ihu_serials.vip)
    if not (is_vip_app or is_vip_pbl):
        raise RuntimeError(
            "VIP UART port couldn't be opened, or it is not connected to VIP UART on target?")

    try:
        adb_bootmode = check_output_logged([adb_executable,
                                            "get-state"],
                                           timeout_sec=60).decode().strip(" \n\r\t")
        logger.info("Bootmode before: %s", adb_bootmode)
    except Exception as e:
        logger.info(
            "Could not check bootmode, probably not booted, continue anyway. Reason: %r" % e)

    try:
        if is_vip_app:
            ihu_serials.vip.writeline("sm alw 1")
            expect_line(ihu_serials.vip, ".*SysM- Always_On: 1.*", 15)

        if is_vip_pbl:
            ihu_serials.vip.writeline("sm st_off")
            expect_line(ihu_serials.vip, ".*Disabling startup timer.*", 15)

        if update_mp:
            logger.info("Updating MP software (via Fastboot)")
            logger.info("Starting ABL command line")
            boot_mp_to_abl_cmdline(ihu_serials.vip)

            expect_line(ihu_serials.mp, ">>>.*", 30, "Is the MP UART connected? Or do you have the TTY open already?")
            logger.info("ABL command line confirmed, boot into fastboot")
            ihu_serials.mp.writeline("boot elk")
            expect_line(ihu_serials.mp, "==> jump to image.*", 30)
            expect_line(ihu_serials.mp,
                        "USB for fastboot transport layer selected", 30)
            logger.info("Fastboot confirmed on console")

            # Verify that the Fastboot protocol is working between host and target.
            fastboot_timeout_s = 10
            while fastboot_timeout_s > 0:
                fastboot_devices = check_output_logged([fastboot_executable,
                                                        "devices"]).decode().strip(" \n\r\t")
                if fastboot_devices:
                    logger.info("Fastboot device over cable: %s" %
                                fastboot_devices)
                    break
                fastboot_timeout_s -= 1
                time.sleep(1)

            if fastboot_timeout_s == 0:
                raise Exception(
                    "No Fastboot device found. Did you forget to connect host with target?")

            bsp_provided_flashfiles_path = os.path.join(build_out_dir,
                                                        "target",
                                                        "product",
                                                        product,
                                                        "fast_flashfiles")

            logger.info("Running fastboot.sh inside " + bsp_provided_flashfiles_path)

            # Ensure that after flashing completes the pin is in correct state
            # fastboot reboot boots into android anyway, but toggling reset afterwards should not cause
            # booting to abl
            mp_bootpin_prod(ihu_serials.vip)

            check_output_logged(['bash', 'fastboot.sh', '--abl'],
                                cwd=bsp_provided_flashfiles_path).decode().strip(" \n\r\t")

            wait_for_device_adb(adb_executable)
            wait_for_boot_completed(adb_executable)

        if update_vip:
            logger.info("Flashing VIP")

            try:
                ihu_serials.vip.writeline("swdl e")  # normal swdl e
                expect_line(ihu_serials.vip, ".*PBL Version.*", 15)
            except Exception:
                logger.warning("As usually swdl e failed due to power moding, go for emergency one")
                ihu_serials.vip.writeline("swdl er")  # brutal # swdl e

            serial_mapping.verify_serial_is_vip_pbl(ihu_serials.vip, timeout_sec=15)
            time.sleep(1)

            ihu_serials.vip.writeline("sm st_off")
            expect_line(ihu_serials.vip, ".*Disabling startup timer.*", 15)

            ensure_device_mode_for_vip_flashing(adb_executable, ihu_serials)

            try:  # ugly hack until SSBL gets proper partnum in header (so we can lazy load)
                check_output_logged([adb_executable,
                                     "shell",
                                     "vbf_flasher",
                                     "/vendor/vip-update/pbl/vip-ssbl.VBF"],
                                    timeout_sec=60 * 2).decode().strip(" \n\r\t")
            except Exception:
                # If it fails becouse it is already loaded we can ignore
                # If it fails from any other reason next step will fail anyway
                # and interrupt the process
                logger.warning("Failed to flash SSBL, will try PBL in case SSBL is already there")

            output = check_output_logged([adb_executable,
                                          "shell",
                                          "vbf_flasher",
                                          "/vendor/vip-update/pbl/vip-pbl.VBF"],
                                         timeout_sec=60 * 2).decode().strip(" \n\r\t")
            logger.info("VIP PBL update finished with result %s", output)

            logger.info("Rebooting VIP...")
            ihu_serials.vip.writeline("swdl e")
            expect_line(ihu_serials.vip, ".*PBL Version.*", 15)

            ihu_serials.vip.writeline("sm st_off")
            expect_line(ihu_serials.vip, ".*Disabling startup timer.*", 15)

            ensure_device_mode_for_vip_flashing(adb_executable, ihu_serials)

            output = check_output_logged([adb_executable,
                                          "shell",
                                          "vbf_flasher",
                                          "/vendor/vip-update/app/*"],
                                         timeout_sec=60 * 2).decode().strip(" \n\r\t")
            logger.info("VIP APP update finished with result %r", output)
            logger.info("Do power cycle to get full application experience (you are in undefined behavior mode)")


    except Exception:
        # Sleep some as it's nice to have output of VIP+MP shortly after exception also, in case of bad timing
        time.sleep(2)
        raise
    finally:
        ihu_serials.vip.close()
        ihu_serials.mp.close()


def ensure_device_mode_for_vip_flashing(adb_executable, ihu_serials):
    try:
        adb_bootmode = check_output_logged([adb_executable,
                                            "get-state"],
                                           timeout_sec=60).decode().strip(" \n\r\t")
    except Exception:
        adb_bootmode = 'unknown'

    if adb_bootmode != 'device':
        logger.info("In VIP PBL unit does not booted into device mode, forcing...")
        boot_mp_to_android(ihu_serials.vip)
        wait_for_device_adb(adb_executable)
        wait_for_boot_completed(adb_executable)


def wait_for_boot_completed(adb_executable):
    then = time.time()
    while True:
        try:
            output = check_output_logged([adb_executable,
                                          "shell", 'getprop', 'sys.boot_completed'],
                                         timeout_sec=7).decode().strip(" \n\r\t")
        except Exception:
            output = "0"  # Ignore if the command times out
        if output == "1":
            break
        if time.time() > then + 60 * 4:  # Wait four minutes after the ADB service becomes available.
            raise RuntimeError("Wait for boot timeout. property sys.boot_completed==1 not found.")
        time.sleep(4)


def wait_for_device_adb(adb_executable):
    logging.info("Wait for device to enter device-mode via ADB")
    check_output_logged([adb_executable,
                        "wait-for-device"],
                        timeout_sec=60 * 7).decode().strip(" \n\r\t")


def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected.')


def main():
    parser = argparse.ArgumentParser(
        description="Update an IHU using fastboot.")
    parser.add_argument("--product", default="ihu_vcc", help="Product")
    parser.add_argument("--hardware", default="", help="Hardware")
    parser.add_argument("--aosp_root_dir", required=True,
                        help="Repo root directory")
    parser.add_argument(
        "--vip_port", required=True, help="TTY device connected to VIP console UART")
    parser.add_argument(
        "--mp_port", required=True, help="TTY device connected to VIP console UART")

    parser.add_argument("--update-vip", default=False, type=str2bool, help="Also flash VIP with vbf-flasher")
    parser.add_argument("--update-mp", default=True, type=str2bool, help="Flash MP software via fastboot")
    parsed_args = parser.parse_args()

    if socket.gethostname() != "aic-docker":
        raise RuntimeError("Script seems to be running outside the docker container, exiting")

    with open(os.path.dirname(__file__) + "/logging.json", "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    build_dir = parsed_args.aosp_root_dir
    build_out_dir = os.path.join(build_dir, "out")
    logging.info("Start flash with args %r", parsed_args)

    port_mapping = PortMapping(parsed_args.vip_port, parsed_args.mp_port)
    product = parsed_args.product

    if parsed_args.hardware != "":
        logger.warning("Hardware is auto detected, stop passing it manually")

    flash_image(port_mapping,
                product,
                build_out_dir,
                update_mp=parsed_args.update_mp,
                update_vip=parsed_args.update_vip)
    logging.info("Flash completed")


if __name__ == "__main__":
    main()

#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

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
from shipit.serial_mapping import open_serials, PortMapping, VipSerial, MpSerial, IhuSerials
from shipit import serial_mapping
from shipit import recording_serial
from shipit.process_tools import check_output_logged

logger = logging.getLogger(__name__)


def run(cmd, **kwargs):
    return check_output_logged(cmd, **kwargs).decode().strip(" \t\n\r")


def mp_reset_low(vip_serial: VipSerial):
    vip_serial.writeline("gpio 1.10 0 o")


def mp_reset_high(vip_serial: VipSerial):
    vip_serial.writeline("gpio 1.10 1 o")


def mp_bootpin_abl(vip_serial: VipSerial):
    vip_serial.writeline("gpio 8.10 0 o")
    vip_serial.writeline("gpio 1.2 0 o")
    vip_serial.writeline("gpio 1.3 0 o")


def mp_bootpin_prod(vip_serial: VipSerial):
    vip_serial.writeline("gpio 8.10 1 o")
    vip_serial.writeline("gpio 1.2 0 o")
    vip_serial.writeline("gpio 1.3 0 o")


def boot_mp_to_abl_cmdline(vip_serial: VipSerial):
    logging.info("Boot into ABL CMD line")

    mp_reset_low(vip_serial)
    mp_bootpin_abl(vip_serial)
    time.sleep(1)
    mp_reset_high(vip_serial)


def boot_mp_to_android(vip_serial: VipSerial):
    logging.info("Boot into android")
    mp_reset_low(vip_serial)
    mp_bootpin_prod(vip_serial)
    time.sleep(1)
    mp_reset_high(vip_serial)


def flash_image(port_mapping: PortMapping, product: str, build_out_dir: str, update_mp: bool, update_vip: bool):
    adb_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "adb")
    fastboot_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "fastboot")
    bsp_provided_flashfiles_path = os.path.join(build_out_dir,
                                                "target",
                                                "product",
                                                product,
                                                "fast_flashfiles")

    try:
        ihu_serials = open_serials(port_mapping)
    except Exception as e:
        print("""Failed to open Serial ports")
          Troubleshooting:")
            1. USB-to-Serial cables connected?")
            2. Restart docker container. If you plug in serial cable after docker started it will not auto detect.")
                 Ensure all docker terminals are closed and verify container not running with 'docker ps' and 'docker kill'")
            3. Other program using tty-device? Picocom?")
            4. Maybe you have multiple or renamed /dev/ttyUSB-devices, use --mp_port and --vip_port to choose explicitly""")
        return

    VIP, MP = ihu_serials.vip, ihu_serials.mp
    is_vip_app = serial_mapping.verify_serial_is_vip_app(ihu_serials.vip)
    is_vip_pbl = serial_mapping.verify_serial_is_vip_pbl(ihu_serials.vip)
    if not (is_vip_app or is_vip_pbl):
        raise RuntimeError(
            """VIP not detected.
            Troubleshooting:
             1. Power on?
             2. Serial connected?
             3. Swap MP/VIP serial with --swap_tty argument?
             4. Power cycle VIP
             5. Power off VIP and unplug USB, wait 2 minutes to fully discharge internal capacitor then power on again.""")

    try:
        if is_vip_app:
            VIP.writeline("sm alw 1")
            VIP.expect_line(".*SysM- Always_On: 1.*", 15)

        if is_vip_pbl:
            VIP.writeline("sm st_off")
            VIP.expect_line(".*Disabling startup timer.*", 15)

        try:
            adb_bootmode = run([adb_executable, "get-state"], timeout_sec=60)
            logger.info("Bootmode before: %s", adb_bootmode)
        except Exception as e:
            logger.info("Could not check bootmode, probably not booted, continue anyway. Reason: %r" % e)

        if update_mp:
            logger.info("Updating MP software (via Fastboot)")
            logger.info("Starting ABL command line")
            boot_mp_to_abl_cmdline(ihu_serials.vip)

            logger.info("Waiting for ABL commandline, it is usually quick but"
                        "it might take longer in case ABL has some update/init work to do.")
            MP.expect_line(">>>.*", 3 * 60, "Is the MP UART connected? Or do you have the TTY open already?")
            logger.info("ABL command line confirmed, boot into fastboot")
            MP.writeline("boot elk")
            MP.expect_line("==> jump to image.*", 30)
            MP.expect_line("USB for fastboot transport layer selected", 30)
            logger.info("Fastboot confirmed on console")

            # Verify that the Fastboot protocol is working between host and target.
            fastboot_timeout_s = 10
            while fastboot_timeout_s > 0:
                fastboot_devices = run([fastboot_executable, "devices"])
                if fastboot_devices:
                    logger.info("Fastboot device over cable: %s" % fastboot_devices)
                    break
                fastboot_timeout_s -= 1
                time.sleep(1)

            if fastboot_timeout_s == 0:
                raise Exception("No Fastboot device found. Did you forget to connect host with target?")

            # Ensure that after flashing completes the pin is in correct state
            # fastboot reboot boots into android anyway, but toggling reset afterwards should not cause
            # booting to abl
            mp_bootpin_prod(ihu_serials.vip)

            logger.info("Running fastboot.sh inside " + bsp_provided_flashfiles_path)
            run(['bash', 'fastboot.sh', '--abl', '--disable-verity'],
                cwd=bsp_provided_flashfiles_path)
            try:
                # TODO decrease timeout after vip flashing stabilizes
                wait_for_device_adb(adb_executable, timeout_sec=15 * 60)
            except Exception:
                logger.error("Booting after fastboot failed, VIP can be a reason...")
                VIP.writeline("swdl er")  # brutal swdl e
                VIP.expect_line(".*PBL Version.*", 15)
                # This is only extra 15 minutes if VIP Power moding is broken again...
                wait_for_device_adb(adb_executable, timeout_sec=15 * 60)
                pass

            logger.info("ADB available, current properties:")
            dump_props(adb_executable)

            # TODO decrease timeout after vip flashing stabilizes
            wait_for_boot_and_flashing_completed(adb_executable, timeout_sec=15 * 60)
            logger.info("Boot and postboot operations completed, current properties:")
            dump_props(adb_executable)

            logger.info("----------------------------")
            logger.info("Flash and reboot complete")
            logger.info("ro.build.version.release: %s",
                        run([adb_executable, "shell", "getprop", "ro.build.version.release"]))
            logger.info("ro.build.date: %s", run([adb_executable, "shell", "getprop", "ro.build.date"]))
        else:
            logger.info("NOT Updating MP software")

        if update_vip:
            logger.info("Flashing VIP")

            try:
                VIP.writeline("swdl e")  # normal swdl e
                VIP.expect_line(".*PBL Version.*", 15)
            except Exception:
                logger.warning("As usually swdl e failed due to power moding, go for emergency one")
                VIP.writeline("swdl er")  # brutal swdl e

            serial_mapping.verify_serial_is_vip_pbl(ihu_serials.vip, timeout_sec=15)
            time.sleep(1)

            VIP.writeline("sm st_off")
            VIP.expect_line(".*Disabling startup timer.*", 15)

            ensure_device_mode_for_vip_flashing(adb_executable, ihu_serials)

            try:  # ugly hack until SSBL gets proper partnum in header (so we can lazy load)
                run([adb_executable,
                     "shell",
                     "vbf_flasher",
                     "/vendor/vip-update/pbl/vip-ssbl.VBF"],
                    timeout_sec=60 * 2)
            except Exception:
                # If it fails because it is already loaded we can ignore
                # If it fails from any other reason next step will fail anyway
                # and interrupt the process
                logger.warning("Failed to flash SSBL, will try PBL in case SSBL is already there")

            output = run([adb_executable,
                          "shell",
                          "vbf_flasher",
                          "/vendor/vip-update/pbl/vip-pbl.VBF"],
                         timeout_sec=60 * 2)
            logger.info("VIP PBL update finished with result %s", output)

            logger.info("Rebooting VIP...")
            VIP.writeline("swdl e")
            VIP.expect_line(".*PBL Version.*", 15)

            VIP.writeline("sm st_off")
            VIP.expect_line(".*Disabling startup timer.*", 15)

            ensure_device_mode_for_vip_flashing(adb_executable, ihu_serials)

            output = run([adb_executable,
                          "shell",
                          "vbf_flasher",
                          "/vendor/vip-update/app/*"],
                         timeout_sec=60 * 2)
            logger.info("VIP APP update finished with result %r", output)

            VIP.writeline("sm restart")  # new command, already in our PBL
            VIP.expect_line("Rebooting to normal mode", 30, hint="Reseting VIP app failed, try power cycling")

            logger.info("New VIP APP booted successfully")

            wait_for_device_adb(adb_executable)
            wait_for_boot_and_flashing_completed(adb_executable)
            logger.info("MP booted successfully with new VIP APP")

        else:
            logger.info("NOT Flashing VIP")

        logging.info("Flash completed")
    except Exception:
        # Sleep some as it's nice to have output of VIP+MP shortly after exception also, in case of bad timing
        time.sleep(2)
        raise
    finally:
        ihu_serials.vip.close()
        ihu_serials.mp.close()


def ensure_device_mode_for_vip_flashing(adb_executable: str, ihu_serials: IhuSerials):
    try:
        wait_for_device_adb(adb_executable)
        adb_bootmode = run([adb_executable,
                            "get-state"],
                           timeout_sec=60)
    except Exception:
        adb_bootmode = 'unknown'

    if adb_bootmode != 'device':
        logger.info("In VIP PBL unit does not booted into device mode, forcing...")
        boot_mp_to_android(ihu_serials.vip)
        wait_for_device_adb(adb_executable)
        wait_for_boot_and_flashing_completed(adb_executable)


def wait_for_boot_and_flashing_completed(adb_executable: str, timeout_sec=60 * 8):
    then = time.time()
    while True:
        try:
            boot_completed = run([adb_executable,
                                  "shell", 'getprop', 'sys.boot_completed'],
                                 timeout_sec=7)
            session = run([adb_executable,
                           "shell", 'getprop', 'ro.boot.swdl.session'],
                          timeout_sec=7)
        except Exception:
            boot_completed = "0"  # Ignore if the command times out
            session = ""

        if boot_completed == "1" and session == "default":
            vip_auto_flashing = run([adb_executable,
                                     "shell", 'getprop', 'persist.swdl.EnableAutoFlashing'],
                                    timeout_sec=1)
            if vip_auto_flashing == "1":
                vip_ok = run([adb_executable,
                              "shell", 'getprop', 'swdl.vip_version_ok'],
                             timeout_sec=1)
                if vip_ok == "1":
                    break
            else:
                break
        else:
            pass  # continue polling

        if time.time() > then + timeout_sec:
            logger.error("Waiting for booting and flashing failed, properties snapshot:")
            dump_props(adb_executable)
            raise RuntimeError("""Wait for boot and flashing timeout.
                                  Probably unit not booting up successfully or flashing services
                                  fail to ensure consistent software versions.""")
        time.sleep(4)


def wait_for_device_adb(adb_executable, timeout_sec=60 * 7):
    logging.info("Wait for device to enter device-mode via ADB")
    run([adb_executable,
         "wait-for-device"],
        timeout_sec=timeout_sec)


def dump_props(adb_executable, timeout_sec=15):
    run([adb_executable,
                          'shell', 'getprop'],
                         timeout_sec=timeout_sec)


def str2bool(v: str) -> bool:
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
    parser.add_argument("--aosp_root_dir", required=False,
                        help="Repo root directory",
                        default="$ANDROID_BUILD_TOP")
    parser.add_argument(
        "--vip_port", required=False, help="TTY device connected to VIP console UART", default="/dev/ttyUSB0")
    parser.add_argument(
        "--mp_port", required=False, help="TTY device connected to MP console UART", default="/dev/ttyUSB1")
    parser.add_argument(
        "--swap_tty", required=False, action="store_true", help="Swap VIP and MP serial port")

    parser.add_argument("--update-vip", default=False, type=str2bool, help="Also flash VIP with vbf-flasher")
    parser.add_argument("--update-mp", default=True, type=str2bool, help="Flash MP software via fastboot")
    parsed_args = parser.parse_args()

    if socket.gethostname() != "aic-docker":
        raise RuntimeError("Script seems to be running outside the docker container, exiting")

    with open(os.path.dirname(__file__) + "/logging.json", "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    build_dir = os.path.expandvars(parsed_args.aosp_root_dir)
    build_out_dir = os.path.join(build_dir, "out")
    logging.info("Start flash with args %r", parsed_args)

    if parsed_args.swap_tty:
        port_mapping = PortMapping(parsed_args.mp_port, parsed_args.vip_port)
    else:
        port_mapping = PortMapping(parsed_args.vip_port, parsed_args.mp_port)
    product = parsed_args.product

    if parsed_args.hardware != "":
        logger.warning("Hardware is auto detected, stop passing it manually")

    flash_image(port_mapping,
                product,
                build_out_dir,
                update_mp=parsed_args.update_mp,
                update_vip=parsed_args.update_vip)


if __name__ == "__main__":
    main()

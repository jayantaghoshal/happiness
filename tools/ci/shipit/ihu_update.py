#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO: Don't use adb command line, use https://github.com/android/platform_development/blob/master/testrunner/adb_interface.py
#       Right now we are missing platform_development repo from intel AOSP delivery

import os
import time
import argparse
import logging
import logging.config
import json
import shutil

from shipit.serial_mapping import open_serials, PortMapping, VipSerial, MpSerial, IhuSerials
from shipit import serial_mapping
from shipit.process_tools import check_output_logged

logger = logging.getLogger("ihu_update")


def run(cmd, **kwargs) -> str:
    return check_output_logged(cmd, **kwargs).decode().strip(" \t\n\r")


def mp_reset_low(vip_serial: VipSerial) -> None:
    vip_serial.writeline("gpio 1.10 0 o")


def mp_reset_high(vip_serial: VipSerial) -> None:
    vip_serial.writeline("gpio 1.10 1 o")


def mp_bootpin_abl(vip_serial: VipSerial) -> None:
    vip_serial.writeline("gpio 8.10 0 o")
    vip_serial.writeline("gpio 1.2 0 o")
    vip_serial.writeline("gpio 1.3 0 o")


def mp_bootpin_prod(vip_serial: VipSerial) -> None:
    vip_serial.writeline("gpio 8.10 1 o")
    vip_serial.writeline("gpio 1.2 0 o")
    vip_serial.writeline("gpio 1.3 0 o")


def boot_mp_to_abl_cmdline(vip_serial: VipSerial) -> None:
    logger.info("Boot into ABL CMD line")

    mp_reset_low(vip_serial)
    mp_bootpin_abl(vip_serial)
    time.sleep(1)
    mp_reset_high(vip_serial)


def boot_mp_to_android(vip_serial: VipSerial) -> None:
    logger.info("Boot into android")
    mp_reset_low(vip_serial)
    mp_bootpin_prod(vip_serial)
    time.sleep(1)
    mp_reset_high(vip_serial)


def flash_image(port_mapping: PortMapping,
                product: str, build_out_dir: str,
                update_mp: bool,
                update_vip: bool,
                disable_verity: bool) -> None:
    adb_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "adb")
    fastboot_executable = os.path.join(build_out_dir, "host", "linux-x86", "bin", "fastboot")
    bsp_provided_flashfiles_path = os.path.join(build_out_dir,
                                                "target",
                                                "product",
                                                product,
                                                "fast_flashfiles")

    try:
        ihu_serials = open_serials(port_mapping)
        vip, mp = ihu_serials.vip, ihu_serials.mp
    except Exception:
        print("""Failed to open Serial ports")
          Troubleshooting:")
            1. USB-to-Serial cables connected?")
            2. Restart docker container. If you plug in serial cable after docker started it will not auto detect.")
                 Ensure all docker terminals are closed and verify container not running with 'docker ps' and 'docker kill'")
            3. Other program using tty-device? Picocom?")
            4. Maybe you have multiple or renamed /dev/ttyUSB-devices, use --mp_port and --vip_port to choose explicitly""")
        return

    is_vip_app = serial_mapping.verify_serial_is_vip_app(vip)
    is_vip_pbl = serial_mapping.verify_serial_is_vip_pbl(vip)
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
            vip.writeline("sm alw 1 s")
            vip.expect_line(".*SysM- Always_On: 1.*", 15)

        if is_vip_pbl:
            vip.writeline("sm st_off")
            vip.expect_line(".*Disabling startup timer.*", 15)

        if update_mp:
            logger.info("Updating MP software (via Fastboot)")
            logger.info("Starting ABL command line")
            boot_mp_to_abl_cmdline(vip)
            confirm_mp_abl_on_serial(mp)
            start_fastboot_from_mp_abl_cmdline(mp)

            wait_for_host_target_fastboot_connection(fastboot_executable)

            # Ensure that after flashing completes the pin is in correct state
            # fastboot reboot boots into android anyway, but toggling reset afterwards should not cause
            # booting to abl
            mp_bootpin_prod(vip)

            fastboot_command = ['bash', 'fastboot.sh', '--abl']
            if disable_verity:
                fastboot_command += ['--disable-verity']

            logger.info("Running %r inside %r", fastboot_command, bsp_provided_flashfiles_path)

            run(fastboot_command,
                cwd=bsp_provided_flashfiles_path)

            logger.info("Waiting for confirmation that system rebooted...")

            try:
                mp.expect_line("Rebooting ...", timeout_sec=30)
            except Exception:
                logger.error("Rebooting failed, assuming that its only reboot and flashing was still ok", exc_info=True)
                boot_mp_to_android(vip)
                mp.expect_line("abl-APL:.*", timeout_sec=2 * 60)

            try:
                logger.info("Waiting for confirmation that previous ABL uses abl_update_1...")
                mp.expect_line("Searching for ABL update partition abl_update_1.*", timeout_sec=60)

                logger.info("Waiting for ABL update confirmation")
                mp.expect_line("Update time:.*", timeout_sec=120)
                logger.info("ABL update completed.")
            except Exception:
                logger.error("Failed for confirm ABL self update...", exc_info=True)
                logger.info("Trying manually...")
                boot_mp_to_abl_cmdline(vip)
                confirm_mp_abl_on_serial(mp)
                mp.writeline("update mmc1:@0")
                mp.expect_line("Update time:.*", timeout_sec=120)
                logger.info("ABL updated, rebooting")
                mp_bootpin_prod(vip)
                mp.writeline("reset")
                pass

            try:
                logger.info("Fist boot after fastboot  and ABL updates")
                # TODO decrease timeout after vip flashing stabilizes
                wait_for_device_adb(adb_executable, timeout_sec=15 * 60)
            except Exception:
                logger.error("Booting after fastboot failed, VIP power moding ignoring go to programming session"
                             "can be a reason...", exc_info=True)
                vip.writeline("swdl er")  # brutal swdl e
                vip.expect_line(".*PBL Version.*", 15)
                logger.info("Booted PBL, either vip auto flash starts or we will get timout.")
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
                vip.writeline("swdl e")  # normal swdl e
                vip.expect_line(".*PBL Version.*", 15)
            except Exception:
                logger.warning("As usually swdl e failed due to power moding, go for emergency one", exc_info=True)
                vip.writeline("swdl er")  # brutal swdl e

            serial_mapping.verify_serial_is_vip_pbl(vip, timeout_sec=15)
            time.sleep(1)

            vip.writeline("sm st_off")
            vip.expect_line(".*Disabling startup timer.*", 15)

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
            vip.writeline("swdl e")
            vip.expect_line(".*PBL Version.*", 15)

            vip.writeline("sm st_off")
            vip.expect_line(".*Disabling startup timer.*", 15)

            ensure_device_mode_for_vip_flashing(adb_executable, ihu_serials)

            output = run([adb_executable,
                          "shell",
                          "vbf_flasher",
                          "/vendor/vip-update/app/*"],
                         timeout_sec=60 * 2)
            logger.info("VIP APP update finished with result %r", output)

            vip.writeline("sm restart")  # new command, already in our PBL
            vip.expect_line("Rebooting to normal mode", 30, hint="Reseting VIP app failed, try power cycling")

            logger.info("New VIP APP booted successfully")

            wait_for_device_adb(adb_executable)
            wait_for_boot_and_flashing_completed(adb_executable)
            logger.info("MP booted successfully with new VIP APP")

        else:
            logger.info("NOT Flashing VIP")

        logger.info("Flash completed")
    except Exception:
        # Sleep some as it's nice to have output of VIP+MP shortly after exception also, in case of bad timing
        time.sleep(2)
        raise
    finally:
        vip.close()
        mp.close()


def wait_for_host_target_fastboot_connection(fastboot_executable: str) -> None:
    logger.info("Verifying that the fastboot protocol is working between host and target...")
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


def start_fastboot_from_mp_abl_cmdline(mp: MpSerial) -> None:
    logger.info("Booting fastboot from ABL command line...")
    mp.writeline("boot elk")
    mp.expect_line("==> jump to image.*", 30)
    mp.expect_line("USB for fastboot transport layer selected", 30)
    logger.info("Fastboot confirmed on console")


def confirm_mp_abl_on_serial(mp: MpSerial) -> None:
    logger.info("Waiting for ABL commandline, it is usually quick but"
                "it might take longer in case ABL has some update/init work to do.")

    mp.expect_line("abl-APL:.*", 30, "Is the MP UART connected? Or do you have the TTY open already?"
                                     "If it seems that ABL was executing some extra action - report bug!")

    mp.expect_line(">>>.*", 180, "ABL started but we did not received final prompt."
                                 "If it seems that ABL was executing some extra action - report bug!")

    logger.info("ABL command line confirmed")


def ensure_device_mode_for_vip_flashing(adb_executable: str, ihu_serials: IhuSerials) -> None:
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


def wait_for_boot_and_flashing_completed(adb_executable: str, timeout_sec=60 * 8) -> None:
    logger.info("Wait for device to complete boot/onboot actions with timeout %r", timeout_sec)

    started_at = time.time()
    finished_by_deadline = started_at + timeout_sec
    while True:
        try:
            boot_completed = run([adb_executable,
                                  "shell", 'getprop', 'sys.boot_completed'],
                                 timeout_sec=7)
            session = run([adb_executable,
                           "shell", 'getprop', 'ro.boot.swdl.session'],
                          timeout_sec=7)

            if boot_completed == "1" and session == "default":
                vip_auto_flashing = run([adb_executable,
                                         "shell", 'getprop', 'persist.swdl.EnableAutoFlashing'],
                                        timeout_sec=1)
                if vip_auto_flashing == "1":
                    vip_version_ok = run([adb_executable,
                                          "shell", 'getprop', 'swdl.vip_version_ok'],
                                         timeout_sec=1)
                    if vip_version_ok == "1":
                        break
                else:
                    break
            else:
                pass  # continue polling

        except Exception:
            pass

        if time.time() > finished_by_deadline:
            logger.error("Waiting for booting and flashing failed, properties snapshot:")

            try:
                dump_props(adb_executable)
            except Exception:
                # best effort to store properties failed.
                pass

            raise RuntimeError("""Wait for boot and flashing timeout.
                                  Probably unit not booting up successfully or flashing services
                                  fail to ensure consistent software versions.""")
        time.sleep(4)


def wait_for_device_adb(adb_executable, timeout_sec=60 * 7) -> None:
    logger.info("Wait for device to enter device-mode via ADB with timeout %r", timeout_sec)
    run([adb_executable,
         "wait-for-device"],
        timeout_sec=timeout_sec)
    logger.info("Unit entered ADB mode")


def dump_props(adb_executable, timeout_sec=15) -> None:
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


def main() -> None:
    parser = argparse.ArgumentParser(description="Bulletproof update an IHU using 2 serials and USB.")
    parser.add_argument("--product",
                        help="Product",
                        default="$TARGET_PRODUCT")

    parser.add_argument("--aosp-root-dir", '--aosp_root_dir',
                        required=False,
                        help="Repo root directory",
                        default="$ANDROID_BUILD_TOP")

    parser.add_argument("--aosp-out-dir",
                        required=False,
                        help="Build out/ directory, used only when --aosp-root-dir does not exists",
                        default="")

    parser.add_argument("--vip-port", '--vip_port',
                        required=False,
                        help="TTY device connected to VIP console UART",
                        default="/dev/ttyUSB0")

    parser.add_argument("--mp-port", '--mp_port',
                        required=False,
                        help="TTY device connected to MP console UART",
                        default="/dev/ttyUSB1")

    parser.add_argument("--swap-tty", '--swap_tty',
                        required=False,
                        action="store_true",
                        help="Swap VIP and MP serial port")

    parser.add_argument("--disable-verity",
                        required=False,
                        action="store_true",
                        help="Disable dm-verity")

    parser.add_argument("--update-vip", default=False, type=str2bool, help="Also flash VIP with vbf-flasher")
    parser.add_argument("--update-mp", default=True, type=str2bool, help="Flash MP software via fastboot")
    parsed_args = parser.parse_args()

    with open(os.path.dirname(__file__) + "/logging.json", "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    logger.info("Starting parsing for args:\r\n %r", parsed_args)

    aosp_root_dir = os.path.expanduser(os.path.expandvars(parsed_args.aosp_root_dir))
    aosp_out_dir = os.path.expanduser(os.path.expandvars(parsed_args.aosp_out_dir))

    if os.path.isdir(aosp_root_dir):
        if os.path.isdir(aosp_out_dir):
            build_out_dir = aosp_out_dir
        else:
            build_out_dir = os.path.join(aosp_root_dir, "out")
    else:
        if os.path.isdir(aosp_out_dir):
            build_out_dir = aosp_out_dir
        else:
            logger.info("--aosp-root-dir expanded to %r", aosp_root_dir)
            logger.info("--aosp-out-dir expanded to %r", aosp_out_dir)
            raise Exception("--aosp-root-dir does not exists and --aosp-out-dir was not "
                            "specified / is also missing")

    # prepend host tools from out directory to PATH so fastboot.sh pick them.
    os.environ['PATH'] = os.path.abspath(os.path.join(build_out_dir, 'host', 'linux-x86', 'bin')) + \
                         os.pathsep + \
                         os.environ['PATH']

    logger.info("When calling without full path using fastboot from %r", shutil.which('fastboot'))
    logger.info("When calling without full path using adb from %r", shutil.which('adb'))

    if parsed_args.swap_tty:
        port_mapping = PortMapping(parsed_args.mp_port, parsed_args.vip_port)
    else:
        port_mapping = PortMapping(parsed_args.vip_port, parsed_args.mp_port)

    products_dir = os.path.join(build_out_dir, 'target', 'product')
    product_path = os.environ['ANDROID_PRODUCT_OUT']

    if os.path.isdir(product_path):
        product = os.path.basename(product_path)
    else:
        raise Exception(
            "Provided product " + parsed_args.product +
            " after ENV expansion yielded not existing out path " +
            product_path, " see \"" + products_dir + "\" for already built products you have available")

    logger.info("Using product %r, files from %r", product, product_path)

    flash_image(port_mapping,
                product,
                build_out_dir,
                update_mp=parsed_args.update_mp,
                update_vip=parsed_args.update_vip,
                disable_verity=parsed_args.disable_verity)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import time
import argparse
import logging
import logging.config
import json
import shutil

from shipit.serial_mapping import open_serials, swap_serials, PortMapping, VipSerial, MpSerial, IhuSerials
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
                android_product_out: str,
                update_mp: bool,
                force_update_vip: bool,
                disable_verity: bool) -> None:
    bsp_provided_flashfiles_path = os.path.join(android_product_out, "fast_flashfiles")

    try:
        ihu_serials = open_serials(port_mapping)
        vip, mp = ihu_serials.vip, ihu_serials.mp
    except Exception as a:
        logger.error(
            """Failed to open Serial ports
   Troubleshooting:
     1. USB-to-Serial cables connected?
     2. Restart docker container. If you plug in serial cable after docker started it will not auto detect.
        Ensure all docker terminals are closed and verify container not running with 'docker ps' and 'docker kill'
     3. Other program using tty-device? Picocom? Screen?
     4. Maybe you have multiple or renamed /dev/ttyUSB-devices, use --mp_port and --vip_port to choose explicitly
""")
        logger.error("5. Maybe Exception message is more helpfull:", exc_info=True)

        return

    vip_type = vip.type()

    if vip_type is None:
        logger.info("VIP not detected. Trying to switch ttys")
        ihu_serials = swap_serials(port_mapping, ihu_serials)
        vip, mp = ihu_serials.vip, ihu_serials.mp
        vip_type = vip.type()
        if vip_type is None:
            raise RuntimeError(
                """VIP not detected.
                Troubleshooting:
                1. Power on?
                2. Serial connected?
                3. Power cycle VIP
                4. Power off VIP and unplug USB, wait 2 minutes to fully discharge internal capacitor then power on again.
                5. Make sure no gremlins are chewing on the serial cable.""")

    try:
        if vip_type == VipSerial.VIP_APP:
            vip.writeline("sm alw 1 s")
            vip.expect_line(".*SysM- Always_On: 1.*", 15)

        if vip_type == VipSerial.VIP_PBL:
            vip.writeline("sm st_off")
            vip.expect_line(".*Disabling startup timer.*", 15)

        if update_mp:
            logger.info("Updating MP software (via Fastboot)")
            logger.info("Starting ABL command line")
            boot_mp_to_abl_cmdline(vip)
            confirm_mp_abl_on_serial(mp)
            start_fastboot_from_mp_abl_cmdline(mp)

            wait_for_host_target_fastboot_connection()

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
                wait_for_device_adb(timeout_sec=15 * 60)
            except Exception:
                logger.error("Booting after fastboot failed, VIP power moding ignoring go to programming session"
                             "can be a reason...", exc_info=True)
                vip.writeline("swdl er")  # brutal swdl e
                vip.expect_line(".*PBL Version.*", 15)
                logger.info("Booted PBL, either vip auto flash starts or we will get timout.")
                # This is only extra 15 minutes if VIP Power moding is broken again...
                wait_for_device_adb(timeout_sec=15 * 60)
                pass

            logger.info("ADB available, current properties:")
            dump_props()

            # TODO decrease timeout after vip flashing stabilizes
            wait_for_boot_and_flashing_completed(timeout_sec=15 * 60)
            logger.info("Boot and postboot operations completed, current properties:")
            dump_props()

            logger.info("----------------------------")
            logger.info("Flash and reboot complete")
            logger.info("ro.build.version.release: %s", run(['adb', "shell", "getprop", "ro.build.version.release"]))
            logger.info("ro.build.date: %s", run(['adb', "shell", "getprop", "ro.build.date"]))
        else:
            logger.info("NOT Updating MP software")

        if force_update_vip:
            logger.info("Flashing VIP")

            try:
                vip.writeline("swdl e")  # normal swdl e
                vip.expect_line(".*PBL Version.*", 15)
            except Exception:
                logger.warning("As usually swdl e failed due to power moding, go for emergency one", exc_info=True)
                vip.writeline("swdl er")  # brutal swdl e

            if not vip._is_vip_pbl(timeout_sec=15):
                raise RuntimeError("No VIP PBL detected")
            time.sleep(1)

            vip.writeline("sm st_off")
            vip.expect_line(".*Disabling startup timer.*", 15)

            ensure_device_mode_for_vip_flashing(ihu_serials)

            try:  # ugly hack until SSBL gets proper partnum in header (so we can lazy load)
                run(['adb',
                     "shell",
                     "vbf_flasher",
                     "/vendor/vip-update/pbl/vip-ssbl.VBF"],
                    timeout_sec=60 * 2)
            except Exception:
                # If it fails because it is already loaded we can ignore
                # If it fails from any other reason next step will fail anyway
                # and interrupt the process
                logger.warning("Failed to flash SSBL, will try PBL in case SSBL is already there")

            output = run(['adb',
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

            ensure_device_mode_for_vip_flashing(ihu_serials)

            output = run(['adb',
                          "shell",
                          "vbf_flasher",
                          "/vendor/vip-update/app/*"],
                         timeout_sec=60 * 2)
            logger.info("VIP APP update finished with result %r", output)

            vip.writeline("sm restart")  # new command, already in our PBL
            vip.expect_line("Rebooting to normal mode", 30, hint="Reseting VIP app failed, try power cycling")

            logger.info("New VIP APP booted successfully")

            wait_for_device_adb()
            wait_for_boot_and_flashing_completed()
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


def wait_for_host_target_fastboot_connection() -> None:
    logger.info("Verifying that the fastboot protocol is working between host and target...")
    fastboot_timeout_s = 10
    while fastboot_timeout_s > 0:
        fastboot_devices = run(['fastboot', "devices"])
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

    mp.expect_line("abl-APL:.*", 30, "Is the MP UART connected? Or do you have the TTY open already?\r\n"
                                     "Is the unit so old it launched ABL without ab-APL printed?\r\n"
                                     "It will require manual fastboot.sh in that case.\r\n"
                                     "If it seems that ABL was executing some extra action - report bug!")

    mp.expect_line(">>>.*", 180, "ABL started but we did not received final prompt.\r\n"
                                 "If it seems that ABL was executing some extra action - report bug!")

    logger.info("ABL command line confirmed")


def ensure_device_mode_for_vip_flashing(ihu_serials: IhuSerials) -> None:
    try:
        wait_for_device_adb()
        adb_bootmode = run(['adb',
                            "get-state"],
                           timeout_sec=60)
    except Exception:
        adb_bootmode = 'unknown'

    if adb_bootmode != 'device':
        logger.info("In VIP PBL unit does not booted into device mode, forcing...")
        boot_mp_to_android(ihu_serials.vip)
        wait_for_device_adb()
        wait_for_boot_and_flashing_completed()


def wait_for_boot_and_flashing_completed(timeout_sec=60 * 8) -> None:
    logger.info("Wait for device to complete boot/onboot actions with timeout %r", timeout_sec)

    started_at = time.time()
    finished_by_deadline = started_at + timeout_sec
    while True:
        try:
            boot_completed = run(['adb', "shell", 'getprop', 'sys.boot_completed'],
                                 timeout_sec=7)
            session = run(['adb', "shell", 'getprop', 'ro.boot.swdl.session'],
                          timeout_sec=7)

            if boot_completed == "1" and session == "default":
                vip_auto_flashing = run(['adb', "shell", 'getprop', 'persist.swdl.EnableAutoFlashing'],
                                        timeout_sec=1)
                if vip_auto_flashing == "1":
                    vip_version_ok = run(['adb', "shell", 'getprop', 'swdl.vip_version_ok'],
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
                dump_props()
            except Exception:
                # best effort to store properties failed.
                pass

            raise RuntimeError("""Wait for boot and flashing timeout.
                                  Probably unit not booting up successfully or flashing services
                                  fail to ensure consistent software versions.""")
        time.sleep(4)


def wait_for_device_adb(timeout_sec=60 * 7) -> None:
    logger.info("Wait for device to enter device-mode via ADB with timeout %r", timeout_sec)
    run(['adb', "wait-for-device"],
        timeout_sec=timeout_sec)
    logger.info("Unit entered ADB mode")


def dump_props(timeout_sec=15) -> None:
    run(['adb', 'shell', 'getprop'],
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
    parser.add_argument("--android-host-out",
                        help="ANDROID_HOST_OUT directory, i.e. ./out/host/linux-x86",
                        default="$ANDROID_HOST_OUT")

    parser.add_argument("--android-product-out",
                        help="ANDROID_PRODUCT_OUT directory, i.e. ./out/target/product/ihu_abl_car",
                        default="$ANDROID_PRODUCT_OUT")

    parser.add_argument("--vip-port", '--vip_port',
                        required=False,
                        help="TTY device connected to VIP console UART",
                        default="/dev/ttyUSB0")

    parser.add_argument("--mp-port", '--mp_port',
                        required=False,
                        help="TTY device connected to MP console UART",
                        default="/dev/ttyUSB1")

    parser.add_argument("--disable-verity",
                        required=False,
                        action="store_true",
                        help="Disable dm-verity")

    parser.add_argument("--force-update-vip",
                        default=False,
                        type=str2bool,
                        help="Also flash VIP with vbf-flasher, used as recovery if autoflashing fails")

    parser.add_argument("--update-mp",
                        default=True,
                        type=str2bool,
                        help="Flash MP software via fastboot")
    parsed_args = parser.parse_args()

    with open(os.path.dirname(__file__) + "/logging.json", "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    logger.info("Starting parsing for args:\r\n %r", parsed_args)

    android_host_out = os.path.expanduser(os.path.expandvars(parsed_args.android_host_out))
    logger.info("--android-host-out expanded to %r", android_host_out)
    if not os.path.isdir(android_host_out):
        raise Exception("Provided --android-host-out expanded to not existing path: " + android_host_out)

    android_product_out = os.path.expanduser(os.path.expandvars(parsed_args.android_product_out))
    logger.info("--android-product-out expanded to %r", android_product_out)
    if not os.path.isdir(android_product_out):
        raise Exception("Provided --android-product-out expanded to not existing path: " + android_product_out)

    # prepend host tools from out directory to PATH so fastboot.sh pick them.
    os.environ['PATH'] = os.path.abspath(os.path.join(android_host_out, 'bin')) + \
                         os.pathsep + \
                         os.environ['PATH']

    logger.info("Using fastboot from %r", shutil.which('fastboot'))
    logger.info("Using adb from %r", shutil.which('adb'))

    port_mapping = PortMapping(parsed_args.mp_port, parsed_args.vip_port)

    flash_image(port_mapping=port_mapping,
                android_product_out=android_product_out,
                update_mp=parsed_args.update_mp,
                force_update_vip=parsed_args.force_update_vip,
                disable_verity=parsed_args.disable_verity)


if __name__ == "__main__":
    main()

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
import sys

from collections import namedtuple
from shipit.serial_mapping import open_serials, swap_serials, PortMapping, VipSerial, MpSerial, IhuSerials
from shipit import serial_mapping
from shipit.process_tools import check_output_logged

logger = logging.getLogger("ihu_update")


def run(cmd, **kwargs) -> str:
    return check_output_logged(cmd, **kwargs).decode().strip(" \t\n\r")


# region Bootmodes handling
def mp_reset_low(vip: VipSerial) -> None:
    vip.writeline("gpio 1.10 0 o")
    vip.expect_line("Pin:.*0")


def mp_reset_high(vip: VipSerial) -> None:
    vip.writeline("gpio 1.10 1 o")
    vip.expect_line("Pin:.*1")


def mp_bootpin_abl(vip: VipSerial) -> None:
    vip.writeline("gpio 8.10 0 o")
    vip.expect_line("Pin:.*0")

    vip.writeline("gpio 1.2 0 o")
    vip.expect_line("Pin:.*0")

    vip.writeline("gpio 1.3 0 o")
    vip.expect_line("Pin:.*0")


def mp_bootpin_prod(vip: VipSerial) -> None:
    vip.writeline("gpio 8.10 1 o")
    vip.expect_line("Pin:.*1")

    vip.writeline("gpio 1.2 0 o")
    vip.expect_line("Pin:.*0")

    vip.writeline("gpio 1.3 0 o")
    vip.expect_line("Pin:.*0")


def boot_mp_to_abl_cmdline_with_gpios(vip: VipSerial) -> None:
    logger.info("Boot into ABL CMD line")

    mp_reset_low(vip)
    mp_bootpin_abl(vip)
    time.sleep(1)
    mp_reset_high(vip)


def reboot_mp_to_android_default_with_gpios(vip: VipSerial) -> None:
    logger.info("Boot into android")
    mp_reset_low(vip)
    mp_bootpin_prod(vip)
    time.sleep(1)
    mp_reset_high(vip)


def expect_mp_abl_on_serial(mp: MpSerial) -> None:
    logger.info("Waiting for ABL commandline, it is usually quick but"
                "it might take longer in case ABL has some update/init work to do.")

    mp.expect_line("abl-APL:.*", 30, "Is the MP UART connected? Or do you have the TTY open already?\r\n"
                                     "Is the unit so old it launched ABL without ab-APL printed?\r\n"
                                     "It will require manual fastboot.sh in that case.\r\n"
                                     "If it seems that ABL was executing some extra action - report bug!")

    mp.expect_line(">>>.*", 180, "ABL started but we did not received final prompt.\r\n"
                                 "If it seems that ABL was executing some extra action - report bug!")

    logger.info("ABL command line confirmed")


def ensure_mp_is_in_abl_cmdline(vip: VipSerial, mp: MpSerial) -> None:
    try:
        # reboot method with low risk of damaging Apollo lake chip
        boot_mp_to_abl_cmdline_with_bootmodes(vip)
        expect_mp_abl_on_serial(mp)
    except serial_mapping.ExpectedResponseNotPresentError:
        logger.warning("Failed to boot to boot mode", exc_info=True)
        vip.writeline("version")  # print to know if this version was actually old or sth else happened

        # reboot with high success rate on old VIPs
        boot_mp_to_abl_cmdline_with_gpios(vip)
        expect_mp_abl_on_serial(mp)


def boot_mp_to_abl_cmdline_with_bootmodes(vip: VipSerial) -> None:
    vip.writeline("pwr bm 0")
    vip.expect_line(".*pwr bm 0")


def reboot_mp_into_android_default_mode_and_verify_abl_bootmode_confirmation(vip: VipSerial, mp: MpSerial) -> None:
    reboot_mp_into_android_default_mode_without_waiting(vip)
    expect_mp_android_default_session(mp)


def reboot_mp_into_android_default_mode_without_waiting(vip: VipSerial):
    try:
        reboot_mp_to_android_default_with_bootmodes(vip)
    except serial_mapping.ExpectedResponseNotPresentError:
        logger.warning("Failed to boot to boot mode")
        vip.writeline("version")  # print to know if this version was actually old or sth else happened


def reboot_mp_to_android_default_with_bootmodes(vip: VipSerial) -> None:
    vip.writeline("pwr bm 1")
    vip.expect_line(".*pwr bm 1")


def expect_mp_android_default_session(mp: MpSerial) -> None:
    mp.expect_line("abl-APL:.*", timeout_sec=2 * 60)
    mp.expect_line(".+androidboot.swdl.session=default")


# endregion

# region Profiles
DEVELOPER = "developer"
DEVELOPER_RIG = "developer-rig"
CI_MACHINERY = "ci-machinery"
AS_APTIV_FACTORY = "as-aptiv-factory"
AS_VCC_FACTORY = "as-vcc-factory"

ProfileFlags = namedtuple("ProfileFlags", ["sm_alw_1_s", "enable_verity"])

Profiles = {
    DEVELOPER: ProfileFlags(sm_alw_1_s=True, enable_verity=False),
    DEVELOPER_RIG: ProfileFlags(sm_alw_1_s=False, enable_verity=False),
    CI_MACHINERY: ProfileFlags(sm_alw_1_s=True, enable_verity=True),
    AS_APTIV_FACTORY: ProfileFlags(sm_alw_1_s=False, enable_verity=True),
    AS_VCC_FACTORY: ProfileFlags(sm_alw_1_s=False, enable_verity=True),
}


# endregion


def flash_image(port_mapping: PortMapping,
                android_product_out: str,
                update_mp: bool,
                force_update_abl: bool,
                force_update_vip: bool,
                profile_flags: ProfileFlags) -> None:
    try:
        ihu_serials = open_serials(port_mapping)
        vip, mp = ihu_serials.vip, ihu_serials.mp
    except Exception:
        logger.error(
            """Failed to open Serial ports
   Troubleshooting:
     1. USB-to-Serial cables connected?
     2. Restart docker container. If you plug in serial cable after docker started it will not auto detect.
        Ensure all docker terminals are closed and verify container not running with 'docker ps' and 'docker kill'
     3. Other program using tty-device? Picocom? Screen?
     4. Maybe you have multiple or renamed /dev/ttyUSB-devices, use --mp_port and --vip_port to choose explicitly
     5. Maybe exception message is more helpful
""", exc_info=True)

        return

    vip_type = vip.type()

    if vip_type is None:
        logger.info("VIP not detected. Trying to switch ttys")
        ihu_serials = swap_serials(port_mapping, ihu_serials)
        vip, mp = ihu_serials.vip, ihu_serials.mp
        vip_type = vip.type()
        if vip_type is None:
            raise RuntimeError("""
VIP not detected.
Troubleshooting:
1. Power on?
2. Serial connected?
3. Power cycle VIP
4. Power off VIP and unplug USB, wait 2 minutes to fully discharge internal capacitor then power on again.
5. Make sure no gremlins are chewing on the serial cable.
""")

    try:
        reboot_vip_into_pbl(vip)

        if update_mp:
            logger.info("Updating MP software (via Fastboot)")
            logger.info("Starting ABL command line")
            ensure_mp_is_in_abl_cmdline(vip, mp)
            start_fastboot_from_mp_abl_cmdline(mp)

            wait_for_host_target_fastboot_connection()

            fastboot_command = ['bash', 'fastboot.sh', '--abl']
            if not profile_flags.enable_verity:
                fastboot_command += ['--disable-verity']

            bsp_provided_flashfiles_path = os.path.join(android_product_out, "fast_flashfiles")
            logger.info("Running %r inside %r", fastboot_command, bsp_provided_flashfiles_path)

            run(fastboot_command,
                cwd=bsp_provided_flashfiles_path)

            logger.info("Waiting for confirmation that system rebooted...")

            try:
                mp.expect_line("Rebooting ...", timeout_sec=30)
            except serial_mapping.ExpectedResponseNotPresentError:
                reboot_mp_into_android_default_mode_without_waiting(vip)

            try:
                logger.info("Waiting for confirmation that previous ABL uses abl_update_1...")
                mp.expect_line("Searching for ABL update partition abl_update_1.*", timeout_sec=60)

                logger.info("Waiting for ABL update confirmation")
                mp.expect_line("Update time:.*", timeout_sec=120)
                logger.info("ABL update completed.")
            except serial_mapping.ExpectedResponseNotPresentError as ex:
                raise RuntimeError("Failed for confirm reboot or ABL self update,\r\n"
                                   "with exception message {}\r\n"
                                   "Consider reporting bug and ensuring you ABL is correct by running script"
                                   "with --update-mp=false and --force-abl-update true".format(ex))

            logger.info("Flashing was done in PBL, going to application for subsequent boot")
            vip.writeline("sm restart")  # just restart to app
            if vip.is_vip_app(timeout_sec=60):
                if profile_flags.sm_alw_1_s:
                    vip.writeline("sm alw 1 s")
                else:
                    vip.writeline("sm alw 1")
                vip.expect_line(".*SysM- Always_On: 1.*", 15)

            elif vip.is_vip_pbl():
                raise RuntimeError("VIP is stuck in PBL, please rerun with: "
                                   "--update-mp false --force-update-vip true")

            wait_for_boot_and_flashing_completed(timeout_sec=15 * 60)

            logger.info("Boot and postboot operations completed, current properties:")
            dump_props()

            logger.info("----------------------------")
            logger.info("Flash and reboot complete")
            logger.info("ro.build.version.release: %s", run(['adb', "shell", "getprop", "ro.build.version.release"]))
            logger.info("ro.build.date: %s", run(['adb', "shell", "getprop", "ro.build.date"]))
        else:
            logger.info("NOT Updating MP software")

        if force_update_abl:
            logger.info("Flashing ABL via ABL command line")
            ensure_mp_is_in_abl_cmdline(vip, mp)
            mp.writeline("update mmc1:@0")
            mp.expect_line("Update time:.*", timeout_sec=120)
            logger.info("ABL updated, rebooting")
            mp_bootpin_prod(vip)
            mp.writeline("reset")
        else:
            logger.info("NOT forcing updating of ABL manually")

        if force_update_vip:
            logger.info("Flashing VIP")

            reboot_vip_into_pbl(vip)
            ensure_mp_mode_for_force_vip_flashing(ihu_serials)

            output = run(['adb',
                          "shell",
                          "vbf_flasher",
                          "/vendor/vip-update/pbl/vip-ssbl.VBF",
                          "/vendor/vip-update/pbl/vip-pbl.VBF"],
                         timeout_sec=60 * 2)
            logger.info("VIP PBL update finished with result %s", output)

            reboot_vip_into_pbl(vip)
            ensure_mp_mode_for_force_vip_flashing(ihu_serials)

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
            logger.info("NOT forcing Flashing VIP manually")

        logger.info("Flash completed, ensuring post conditions")

        if vip.is_vip_pbl():
            vip.writeline("sm restart")
            vip.is_vip_app(timeout_sec=20)

        if profile_flags.sm_alw_1_s:
            vip.writeline("sm alw 1 s")
            vip.expect_line(".*SysM- Always_On: 1.*", 15)
        else:
            vip.writeline("sm alw 0 s")
            vip.expect_line(".*SysM- Always_On: 0.*", 15)

        # Sleep some as it's nice to have output of VIP+MP before final log
        time.sleep(2)

        logger.info("SUCCESS: Flash completed and post conditions verified")

    except Exception:
        # Sleep some as it's nice to have output of VIP+MP shortly after exception also, in case of bad timing
        time.sleep(2)
        raise
    finally:
        vip.close()
        mp.close()


def reboot_vip_into_pbl(vip):
    # issue both so it works no matter PBL or APP
    # app tends to fails swdl e, PBL would ignore er.
    vip.writeline("swdl er")  # brutal swdl e
    vip.writeline("swdl e")  # swdl e for PBL
    if not vip.is_vip_pbl(timeout_sec=15):
        raise RuntimeError("No VIP PBL detected")

    time.sleep(1)
    vip.writeline("sm st_off")
    vip.expect_line(".*Disabling startup timer.*", 15)


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


def ensure_mp_mode_for_force_vip_flashing(ihu_serials: IhuSerials) -> None:
    try:
        wait_for_device_adb()
        adb_bootmode = run(['adb', "get-state"],
                           timeout_sec=60)
    except Exception:
        adb_bootmode = 'unknown'

    if adb_bootmode != 'device':
        logger.info("In VIP PBL unit does not booted into device mode, forcing...")
        reboot_mp_into_android_default_mode_and_verify_abl_bootmode_confirmation(ihu_serials.vip, ihu_serials.mp)
        wait_for_boot_and_flashing_completed()


def wait_for_boot_and_flashing_completed(timeout_sec=60 * 8) -> None:
    logger.info("Wait for device to complete boot/onboot actions with timeout %r", timeout_sec)

    started_at = time.time()
    finished_by_deadline = started_at + timeout_sec

    # str as its direct readout of Android Property, and its 3 valued by default:
    # empty -> unknown, 0 -> know to be failed, 1 -> known to be succesful
    vip_version_ok = ""

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
                    elif vip_version_ok == "0":
                        logger.error("VIP auto update failed")
                        run(['adb', "logcat", '-s', 'vip_flashing_service'],
                            timeout_sec=7)
                        dump_props()
                        break

                else:
                    break
            elif boot_completed == "1" and session == "programming":
                break
            else:
                pass  # continue polling

        except Exception:
            # keep polling after sleep below
            pass

        if vip_version_ok == "0":
            raise RuntimeError("VIP auto update failed, after reporting a bug rerun script "
                               "with arguments --update-mp false and --force-vip-update true")

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

    parser.add_argument("--profile",
                        required=True,
                        choices=[DEVELOPER, DEVELOPER_RIG, AS_APTIV_FACTORY, AS_VCC_FACTORY, CI_MACHINERY],
                        help="""
Select one of profiles, so the unit can take 'non-SW' factors into account.

If on VCC docker you can use 'ihu_update_developer' as shortcut.

Possible profiles:
|-------------------------------------------------------------------------------------------|
| Option:    | developer | developer-rig | as-aptiv-factory | as-vcc-factory | ci-machinery |
|-------------------------------------------------------------------------------------------|
| sm alw 1 s | on        | off           | off              | off            | on           |
| dm verity  | disabled  | disabled      | enabled          | enabled        | enabled      |
|-------------------------------------------------------------------------------------------|

""")

    parser.add_argument("--force-update-vip",
                        default=False,
                        type=str2bool,
                        help="Also flash VIP with vbf-flasher, used as recovery if autoflashing fails")

    parser.add_argument("--force-update-abl",
                        default=False,
                        type=str2bool,
                        help="Also flash ABL from its command line")

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

    if parsed_args.update_mp:
        android_product_out = os.path.expanduser(os.path.expandvars(parsed_args.android_product_out))
        logger.info("--android-product-out expanded to %r", android_product_out)
        if not os.path.isdir(android_product_out):
            raise Exception("Provided --android-product-out expanded to not existing path: " + android_product_out)
    else:
        android_product_out = None

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
                force_update_abl=parsed_args.force_update_abl,
                profile_flags=Profiles[parsed_args.profile])


if __name__ == "__main__":
    try:
        main()
        sys.exit(0)
    except Exception as ex:
        logger.error("Update failed exception", exc_info=True)
        logger.error("\r\n\r\n\r\nUpdate failed with error:{}".format(ex))
        sys.exit(1)

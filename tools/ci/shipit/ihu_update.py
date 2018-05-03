#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import time
import argparse
import logging
import logging.config
import json
import re
import shutil
import sys

from collections import namedtuple
from shipit.serial_mapping import open_serials, swap_serials, PortMapping, VipSerial, MpSerial, IhuSerials
from shipit import serial_mapping
from shipit.process_tools import check_output_logged
from distutils.version import LooseVersion

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
    logger.info("Waiting for ABL commandline, it is usually quick but "
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

        old_pbl = (LooseVersion(vip.pbl_version) <= LooseVersion("6.6.0"))
        logger.info("PBL version is {}".format(vip.pbl_version))

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
            except serial_mapping.ExpectedResponseNotPresentError as exp:
                raise RuntimeError("Failed for confirm reboot or ABL self update,\r\n"
                                   "with exception message {}\r\n"
                                   "Consider reporting bug and ensuring you ABL is correct by running script"
                                   "with --update-mp=false and --force-abl-update true".format(exp))

            if old_pbl:
                logger.info("VIP had old PBL ({}) so first action after fastboot is to fix that".format(vip.pbl_version))
                # Go into APP to make the MP go into device mode since it needs to complete IFWI update. When
                # IFWI update is done, the MP will reboot. To make sure that it goes into device mode, we need the
                # VIP to be in APP mode.
                if not vip.is_vip_app():
                    reboot_vip_into_app(profile_flags, vip)
                    if not vip.is_vip_app():
                        raise RuntimeError("VIP needs to be in APP")
                vip.writeline("sm alw 1") # Keep system up
                wait_for_device_adb()
                # IFWI update should be done here. Boot back into PBL.
                reboot_vip_into_pbl(vip)
                vip.writeline("sm alw 1")
                # Give fastboot mode a few retries, just in case IFWI update runs at startup
                wait_for_fastboot(retries=10, time_between_retries=10)
                run(["fastboot", "continue"])
                wait_for_device_adb()
                # Flash APP
                run(["adb", "shell", "vbf_flasher", "/vendor/vip-update/app/*"])
                # Reboot pbl to pbl
                reboot_vip_into_pbl(vip)
                vip.writeline("sm alw 1") # Keep alive for flashing
                wait_for_fastboot(retries=10, time_between_retries=10)
                run(["fastboot", "continue"])
                wait_for_device_adb()
                # Flash PBL
                run(["adb", "shell", "vbf_flasher", "/vendor/vip-update/pbl/*"])
                # Reboot into APP
                run(["adb", "shell", "vbf_flasher", "-a"])
                # Wait for APP to come up
                vip.expect_line(r"\[0\.\d+\].*Sys_Man_Init.*", timeout_sec=3 * 60)
                time.sleep(2) # Lots of traffic on this port during boot, wait until it dies down a bit.
                if not vip.is_vip_app():
                    raise RuntimeError("VIP did not return to APP mode")
                ensure_no_powermoding_or_resets(profile_flags, vip)
                wait_for_device_adb()
            else:
                try:
                    reboot_mp_to_android_default_with_bootmodes(vip) # VIP command to reboot MP to normal mode
                    logger.info("Waiting for 30 seconds for sth looking like MP bootup")
                    mp.expect_line("Loader: Launch VMM", timeout_sec=30)
                except serial_mapping.ExpectedResponseNotPresentError:
                    logger.error("Failed to boot after initial startup, "
                                 "sth around fastboot reboot command is broken\r\n"
                                 "will try to restart via serials...")

                    reboot_vip_into_app(profile_flags, vip)

            wait_for_boot_and_flashing_completed(profile_flags, ihu_serials, timeout_sec=15 * 60)

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

            force_flash_vip_pbl_and_app(profile_flags, ihu_serials)

            logger.info("Freshly flashed VIP APP started successfully")

            wait_for_boot_and_flashing_completed(profile_flags, ihu_serials, timeout_sec=5 * 60)
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


def force_flash_vip_pbl_and_app(profile_flags, ihu_serials):
    def ensure_mp_mode_for_force_vip_flashing() -> None:
        def ensure_mp_with_with_adb_even_if_fastboot():
            try:
                wait_for_device_adb()
            except Exception:
                logger.warning("Unit not booted into device, maybe its fastboot...")
                wait_for_host_target_fastboot_connection()
                run(['fastboot', "continue"])
                wait_for_device_adb()

        try:
            ensure_mp_with_with_adb_even_if_fastboot()
        except Exception:
            logger.info("In VIP PBL unit does not booted into device mode, forcing...")
            reboot_mp_into_android_default_mode_without_waiting(ihu_serials.vip)
            ensure_mp_with_with_adb_even_if_fastboot()

    if ihu_serials.vip.type() != VipSerial.VIP_APP:
        logger.info("VIP not in APP mode. Ensuring that it is.")
        reboot_vip_into_app(profile_flags, ihu_serials.vip)
    ensure_mp_mode_for_force_vip_flashing()
    output = run(['adb',
                  "shell",
                  "vbf_flasher",
                  "/vendor/vip-update/pbl/vip-ssbl.VBF",
                  "/vendor/vip-update/pbl/vip-pbl.VBF"],
                 timeout_sec=60 * 2)
    logger.info("VIP PBL update finished with result %s", output)
    if ihu_serials.vip.type() != VipSerial.VIP_PBL:
        logger.info("VIP not in PBL mode. Ensuring that it is.")
        reboot_vip_into_pbl(ihu_serials.vip)
    ensure_mp_mode_for_force_vip_flashing()
    output = run(['adb',
                  "shell",
                  "vbf_flasher",
                  "/vendor/vip-update/app/*"],
                 timeout_sec=60 * 2)
    logger.info("VIP APP update finished with result %r", output)
    reboot_vip_into_app(profile_flags, ihu_serials.vip)


def reboot_vip_into_app(profile_flags: ProfileFlags, vip: VipSerial):
    vip_type = vip.type()

    def reboot_pbl_to_app():
        vip.writeline("sm restart")  # just restart pbl to app
        if vip.try_expect_line(r"Missing parameter - command: sm restart", timeout_sec=1):
            logger.info("Apparently we're dealing with a VIP from the dark ages...")
            # Ok, so this appears to be an old VIP that doesn't support "sm restart".
            # Let's set sm alw 0 and restart to PBL. That, and the startup timer timing out should
            # take us back to APP
            vip.writeline("swdl e") # Restart to PBL
            if vip.try_expect_line(r"\[0\.\d+\].*Sys_Man_Init.*", timeout_sec=30):
                if not vip.is_vip_pbl(timeout_sec=30):
                    raise RuntimeError("Unable to boot into PBL")
            else:
                raise RuntimeError("No sign of life from VIP")
            logger.info("Booted into VIP PBL, awaiting boot timeout for transition to APP mode")
            if vip.try_expect_line(r"\[0\.\d+\].*Sys_Man_Init.*", timeout_sec=3 * 60):
                time.sleep(2) # Wait for console traffic to die down a bit before issuing commands
                if not vip.is_vip_app(timeout_sec=30):
                    raise RuntimeError("VIP not in APP after timeout and reboot")
            else:
                raise RuntimeError("VIP not responding")
        else:
            # matching first second log timestamps is more robust in case of lost traces
            vip.expect_line(r"\[0\.\d+\]", timeout_sec=30, hint="VIP has not booted (no traces indicating booting up)")

    if vip_type == vip.VIP_PBL:
        reboot_pbl_to_app()
    elif vip_type == vip.VIP_APP:
        vip.writeline("sm restart 3")  # just restart APP to APP with "neutral" restart reason diagnostics
        try:
            vip.expect_line("r\[0\.\d+\]", timeout_sec=60, hint="VIP has not booted (no traces indicating booting up)")
        except serial_mapping.ExpectedResponseNotPresentError:
            logger.error("VIP failed to reboot, trying to go through PBL")
            reboot_vip_into_pbl(vip)
            reboot_pbl_to_app()

    ensure_no_powermoding_or_resets(profile_flags, vip)
    logger.info("VIP ready in APP mode")


def reboot_vip_into_pbl(vip):
    logger.info("Rebooting VIP into PBL mode")
    vip_type = vip.type()

    if vip_type == vip.VIP_APP:
        logger.info("VIP APP detected, using swdl er")
        vip.writeline("swdl er")  # brutal swdl e
    elif vip_type == vip.VIP_PBL:
        logger.info("VIP PBL detected, using swdl e")
        vip.writeline("swdl e")  # swdl e for PBL

    logger.info("Waiting for VIP reboot")
    vip.expect_line(r"\[0\.\d+\].*Sys_Man_Init",
                    timeout_sec=30,
                    hint="VIP has not booted (no traces indicating booting up)")

    time.sleep(1)  # a lot console traffic in this second
    logger.info("Confirming VIP in PBL mode...")

    if not vip.is_vip_pbl():
        raise RuntimeError("Failed to confirm VIP PBL")

    vip.writeline("sm st_off")
    vip.expect_line(".*Disabling startup timer.*", 15)

    logger.info("VIP ready in PBL mode")


def ensure_no_powermoding_or_resets(profile_flags: ProfileFlags, vip: VipSerial):
    logger.info("Ensuring VIP will not reboot by accident")
    if vip.is_vip_app():
        if profile_flags.sm_alw_1_s:
            vip.writeline("sm alw 1 s")
        else:
            vip.writeline("sm alw 1")
        vip.expect_line(".*SysM- Always_On: 1.*", 15)

    elif vip.is_vip_pbl():
        vip.writeline("sm st_off")
        vip.expect_line(".*Disabling startup timer.*", 15)

    logger.info("VIP Power Moding/startup timers disabled")


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


def is_mp_in_fastboot() -> bool:
    output = run(["fastboot", "devices"])
    if output:
        return True
    return False


def wait_for_fastboot(retries: int = 10, time_between_retries: int = 10) -> None:
    check_fastboot_retries = retries
    while check_fastboot_retries > 0:
        if is_mp_in_fastboot():
            break
        time.sleep(time_between_retries)
        check_fastboot_retries -= 1
    if not is_mp_in_fastboot():
        raise RuntimeError("For some reason MP did not go into fastboot")


def wait_for_boot_and_flashing_completed(profile_flags: ProfileFlags,
                                         ihu_serials: IhuSerials,
                                         timeout_sec: int) -> None:
    logger.info("Wait for device to complete boot/onboot actions with timeout %r", timeout_sec)

    vip_recovery_retries_left = 3

    started_at = time.time()
    finished_by_deadline = started_at + timeout_sec

    # str as its direct readout of Android Property, and its 3 valued by default:
    # empty -> unknown, 0 -> know to be failed, 1 -> known to be succesful
    vip_auto_flashing = ""
    vip_version_ok = ""

    while True:
        try:
            wait_for_device_adb(10)
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
                        logger.info("Everything went fine, all properties seem like healthy system")
                        break
                    elif vip_version_ok == "0":
                        logger.error("VIP auto update failed, retries left %r, getting logs...",
                                     vip_recovery_retries_left)
                        try:
                            run(['adb', "logcat", '-s', '-d', 'vip_flashing_service'],
                                timeout_sec=7)
                            logger.info("Logged adb logs form vip_flashing service.")
                        except Exception:
                            logger.warning("Failed to get logs from ADB")

                        if vip_recovery_retries_left > 0:
                            vip_recovery_retries_left -= 1

                            if vip_recovery_retries_left > 0:
                                logger.info("Rebooting VIP into APP as an attempt to help vip_flashing_service suceeed")
                                reboot_vip_into_app(profile_flags, ihu_serials.vip)
                                vip_version_ok = ""
                                time.sleep(30)
                            else:
                                logger.info("Flashing VIP via command line as last resort")
                                force_flash_vip_pbl_and_app(profile_flags, ihu_serials)

                        else:
                            logger.error("No more VIP auto update retries left")

                else:
                    logger.info("System booted and VIP auto flashing is disabled")
                    break
            elif boot_completed == "1" and session == "programming":
                logger.info("System booted in programming session, waiting for booting back to default.")
            else:
                pass  # continue polling

            logger.info("Waiting for booting and flashing on startup to complete, status:\r\n" +
                        "sys.boot_completed = {}\r\n".format(boot_completed) +
                        "ro.boot.swdl.session = {}\r\n".format(session) +
                        "sys.persist.swdl.EnableAutoFlashing = {}\r\n".format(vip_auto_flashing) +
                        "swdl.vip_version_ok = {}\r\n".format(vip_version_ok))

        except Exception as waiting_ex:
            logger.info("Waiting for system to read all properties, current error {}".format(waiting_ex))
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
    try:
        run(['adb', "wait-for-device"],
            timeout_sec=timeout_sec)
        logger.info("Unit entered ADB mode")
    except Exception:
        raise RuntimeError("Waiting {} seconds for ADB failed".format(timeout_sec))


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

    with open(os.path.join(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
        # Add a file handler to log to ihu_update.log as well
        log_config['handlers']['update_ihu_file_handler'] = {
            'class': 'logging.handlers.RotatingFileHandler',
            'level': 'DEBUG',
            'formatter': 'simple',
            'filename': 'ihu_update.log',
            'maxBytes': 0,
            'backupCount': 5,
            'encoding': 'utf8'
        }
        log_config['root']['handlers'].append("update_ihu_file_handler")
    logging.config.dictConfig(log_config)

    script_version = run(["git", "-C", os.path.dirname(sys.argv[0]), "rev-parse", "HEAD"])
    logger.info("{} ({})".format(sys.argv[0], script_version))

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
        logger.error("\r\n\r\n\r\nUpdate failed with error: {}".format(ex))
        sys.exit(1)

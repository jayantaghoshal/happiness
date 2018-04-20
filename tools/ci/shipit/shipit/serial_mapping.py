# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import re
import time
from typing import NamedTuple, Any
from .recording_serial import RecordingSerial


class ExpectedResponseNotPresentError(RuntimeError):
    pass


class Serial(RecordingSerial):
    def __init__(self, *args, **kwargs):
        super(Serial, self).__init__(*args, **kwargs)

    def try_expect_line(self, pattern: str, timeout_sec: int = 5, hint: str = None):
        try:
            self.expect_line(pattern, timeout_sec, hint)
            return True
        except Exception:
            return False

    def expect_line(self, pattern: str, timeout_sec: int = 5, hint: str = None):
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            line = self.readline(timeout_sec)
            if line is not None and re.match(pattern, line):
                return True
        message = "Timeout %d sec when expecting match on pattern: \"%s\." % (
            timeout_sec, pattern)
        if hint is not None:
            message += hint
        raise ExpectedResponseNotPresentError(message)

    def wait_line(self, pattern: str, timeout_sec: int):
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            line = self.readline(timeout_sec)
            if line is not None and re.match(pattern, line):
                return

    def wait_for_dmesg_timestamp(self, device_timestamp_to_wait_for: int, timeout_sec: int):
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            self.writeline("\n")
            line = self.readline(timeout_sec)
            if line:
                m = re.match(r"\[(.*)\]", line)
                if m:
                    numstr = m.group(1)
                    num = float(numstr.strip())
                    if num > device_timestamp_to_wait_for:
                        return
            time.sleep(0.5)

        raise RuntimeError("Never reached time: %d" % device_timestamp_to_wait_for)


class VipSerial(Serial):
    VIP_APP = 0
    VIP_PBL = 1

    def __init__(self, *args, **kwargs):
        super(VipSerial, self).__init__(*args, **kwargs)

    def type(self) -> Any:
        if self.is_vip_app():
            return VipSerial.VIP_APP
        if self.is_vip_pbl():
            return VipSerial.VIP_PBL
        return None

    def is_vip_app(self, timeout_sec=5) -> bool:
        self.writeline("version")
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            line = self.readline(timeout_sec)
            if not line:
                continue
            if re.match(r"\s*Project_ID.*VCC_IHU.*VIP", line) is not None:
                return True
            if re.match(r".*/sh: version: not found", line) is not None:
                return False
        return False

    def is_vip_pbl(self, timeout_sec=5) -> bool:
        self.writeline("version")
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            line = self.readline(timeout_sec)
            if not line:
                continue
            pbl_version_match = re.match(r".*PBL Version: PBL/v(.*)$", line)
            if pbl_version_match is not None:
                # Tuck away PBL version since we've already found it
                self.pbl_version = pbl_version_match.group(1)
                return True
            if re.match(r".*/sh: version: not found", line) is not None:
                return False
        return False


class MpSerial(Serial):
    pass


PortMapping = NamedTuple("PortMapping", [("vip_tty_device", str), ("mp_tty_device", str)])
IhuSerials = NamedTuple("IhuSerials", [("vip", VipSerial), ("mp", MpSerial)])


def open_serials(ports: PortMapping) -> IhuSerials:
    vip = VipSerial(ports.vip_tty_device, 115200, timeout_sec=1, log_context_name="VIP")
    mp = MpSerial(ports.mp_tty_device, 115200, timeout_sec=1, log_context_name="_MP")
    return IhuSerials(vip, mp)


def swap_serials(ports: PortMapping, serials: IhuSerials) -> IhuSerials:
    serials.vip.close()
    serials.mp.close()
    return open_serials(PortMapping(ports.mp_tty_device, ports.vip_tty_device))

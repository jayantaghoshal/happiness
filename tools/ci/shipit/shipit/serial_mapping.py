# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import re
import time
from typing import NamedTuple, Any
from .recording_serial import RecordingSerial


class Serial(RecordingSerial):
    def expect_line(self, pattern: str, timeout_sec: int, hint: str = None):
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            line = self.readline(timeout_sec)
            if line is not None and re.match(pattern, line):
                return True
        message = "Timeout %d sec when expecting match on pattern: \"%s\." % (
            timeout_sec, pattern)
        if hint is not None:
            message += hint
        raise RuntimeError(message)

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

    def type(self) -> Any:
        if self._is_vip_app():
            return VipSerial.VIP_APP
        if self._is_vip_pbl():
            return VipSerial.VIP_PBL
        return None

    def _is_vip_app(self, timeout_sec=5) -> bool:
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

    def _is_vip_pbl(self, timeout_sec=5) -> bool:
        self.writeline("version")
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            line = self.readline(timeout_sec)
            if not line:
                continue
            if re.match(r".*PBL Version: PBL/.*", line) is not None:
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

import re
import time
from collections import namedtuple
import serial
import serial.tools.list_ports
from .recording_serial import RecordingSerial


PortMapping = namedtuple("PortMapping", ["vip_tty_device", "mp_tty_device"])
IhuSerials = namedtuple("IhuSerials", ["vip", "mp"])


def verify_serial_is_vip(s, timeout_sec=5):
    s.writeline("version")
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        line = s.readline(timeout_sec)
        if not line:
            continue
        if re.match(r"\s*Project_ID.*VCC_IHU.*VIP", line) is not None:
            return True
        if re.match(r".*/sh: version: not found", line) is not None:
            return False
    return False


def verify_serial_is_mp_android(s, timeout_sec=5):
    s.writeline("")
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        line = s.readline(timeout_sec)
        if re.match(r".*|ihu_abl_car:", line) is not None:
            return True
    return False


def verify_serial_is_mp_android_elk(s, timeout_sec=5):
    s.writeline("")
    stop_time = time.time() + timeout_sec
    while time.time() < stop_time:
        line = s.readline()
        if re.match(r">>>.*", line) is not None:
            return True
    return False


def open_serials(ports : PortMapping) -> IhuSerials:
    vip = RecordingSerial(ports.vip_tty_device, 115200, timeout_sec=1, log_context_name="VIP")
    mp = RecordingSerial(ports.mp_tty_device, 115200, timeout_sec=1, log_context_name="_MP")
    return IhuSerials(vip, mp)


def auto_detect_port_mapping():
    #TODO: This is very unreliable, some times the MP is not even started so we have to start it from VIP
    #      Before we can find it
    ports = serial.tools.list_ports.comports()
    vip = None
    mp = None

    for p in ports:
        s = RecordingSerial(p.device, 115200, timeout_sec=1)
        if vip is None:
            if verify_serial_is_vip(s):
                vip = s
                continue  # Don't close
        if mp is None:
            if verify_serial_is_mp_android(s) or verify_serial_is_mp_android_elk(s):
                mp = s
                continue # Don't close
        s.close()
    if vip is None:
        raise RuntimeError("Failed to find VIP TTY")
    if mp is None:
        raise RuntimeError("Failed to find MP TTY")
    return IhuSerials(vip, mp)
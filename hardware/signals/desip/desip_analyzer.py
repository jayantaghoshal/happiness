import re
from collections import namedtuple
import typing

import sys

LogcatEntry = namedtuple("LogcatEntry", "date time pid tid level tag msg")



DIR_RX = 0
DIR_TX =1

DESIP_SOF = 0x1C
DESIP_ESCAPE = 0x1b

def get_logcat_entries(lines : typing.Iterable[str]):
    logcatRegex = (
        # Date
        r"(\S+)\s+" +
        # Time
        r"(\S+)\s+" +
        # PID
        r"(\d+)\s+" +
        # ThreadId
        r"(\d+)\s+" +
        # Verbosity
        r"(\S)\s+" +
        # Log tag
        r"(\S+)\s+" +
        # Message
        r":\s*(.*)")

    for l in lines:
        m = re.match(logcatRegex, l)
        if m != None:
            (date, time, pid, tid, level, tag, msg) = m.groups()
            yield LogcatEntry(date, time, pid, tid, level, tag, msg)


def get_desip_traffic(logcat_entries: typing.Iterable[LogcatEntry]):
    rxRegex = re.compile(r"Serial terminal port 1 received\s*\d+\s*bytes: (.*)")
    txRegex = re.compile(r"VIBPutMsg\(\): Sending data: (.*)")
    for l in logcat_entries:
        if l.tag == "DESIPD":
            rx_match = rxRegex.match(l.msg)
            if rx_match is not None:
                yield (l.time, DIR_RX, rx_match.group(1))
                continue
            tx_match = txRegex.match(l.msg)
            if tx_match is not None:
                yield (l.time, DIR_TX, tx_match.group(1))


def desip_statemachine(traffic: typing.Iterable[typing.Tuple[str, int, str]]):
    rx = {"msg":[], "in_escape": False, "in_error": False}
    tx = {"msg":[], "in_escape": False, "in_error": False}

    for (timestamp, dir, data) in traffic:
        bytes = data.strip(":").split(":")
        statemachine = rx if dir == DIR_RX else tx
        msg = statemachine["msg"]
        for b in (int(b, 16) for b in bytes):
            in_escape = statemachine["in_escape"]

            if statemachine["in_error"]:
                if b == DESIP_SOF:
                    statemachine["in_escape"] = False
                    statemachine["in_error"] = False
                    statemachine["msg"].clear()
                continue

            if in_escape:
                if b == 0x1d:
                    msg.append(0x1b)
                elif b == 0x1e:
                    msg.append(0x1c)
                else:
                    statemachine["in_error"] = True
                    print("Error: Unallowed escape sequence: %02X, from data: %s" % (b, data))
                statemachine["in_escape"] = False

            if b == DESIP_SOF:
                if len(msg) != 0:
                    yield (timestamp, dir, msg)
                    msg.clear()
            elif b == DESIP_ESCAPE:
                statemachine["in_escape"] = True
            else:
                msg.append(b)

def desip_parser(traffic: typing.Iterable[typing.Tuple[str, int, typing.List[int]]]):
    def arrtohex(a):
        return ":".join(("%02X" % v for v in a))
    for (timestamp, dir, packet) in traffic:
        [b0, b1] = packet[0:2]
        cntl  = (b0 & 0b10000000) != 0
        force = (b0 & 0b01000000) != 0
        nack  = (b0 & 0b00100000) != 0
        res   = (b0 & 0b00010000) != 0
        RxNP = b1 & 0xF0 >> 8
        RxID = b1 & 0x0F
        if cntl:
            RxLen = b0 & 0x0F
            yield (timestamp, "VIP" if dir == DIR_RX else " MP", "Cntl", "Force" if force else "-----", "NACK" if nack else "----", "RxNP=0x%02X RxID=0x%02X" % (RxNP, RxID),"RxLen=0x%02X" % RxLen, "----------",   arrtohex(packet))
        else:
            txid = b0 & 0x0F
            yield (timestamp, "VIP" if dir == DIR_RX else " MP", "Data", "Force" if force else "-----", "NACK" if nack else "----", "RxNP=0x%02X RxID=0x%02X" % (RxNP, RxID), "-----------", "TxId=0x%02X"% txid,arrtohex(packet))


def stream_stdin():
    for line in sys.stdin:
        yield  line

def main():
    if len(sys.argv) == 2:
        with open(sys.argv[1], "r", encoding="UTF-8") as f:
            lines = f.readlines()
    else:
        lines = stream_stdin()

    logcat_entries = get_logcat_entries(lines)
    traffic = get_desip_traffic(logcat_entries)
    packets = desip_statemachine(traffic)
    print(("Time", "Sender", "Type", "Force", "Nack", "RxNP / RxID", "-", "Original"))
    for d in desip_parser(packets):
        print(d)

if __name__ == "__main__":
    main()
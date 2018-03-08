# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import struct
import socket
import threading
import logging
import time
import select
logger = logging.getLogger(__name__)
logger_lowlevel_rx = logging.getLogger(__name__ + ".rx")
logger_lowlevel_tx = logging.getLogger(__name__ + ".tx")
import typing


class kCommandCode:
    start=0x0001
    stop=0x0002
    key=0x0003
    status=0x0004
    dataExchange=0x005
    dataRequest=0x0006
    dataError=0x0007
    freeRunningRequest=0x0008
    freeRunningCancel=0x0009
    statusRequest=0x000A
    sequenceNrError=0x000B


class kMeasurmentState:
    NotRunning = 1
    PreStart = 2
    Running = 3
    Stop = 4


class kDataError:
    MeasurementNotRunning = 1
    GroupIdInvalid = 2
    DataSizeTooLarge = 3


class kFreeRunningFlag:
    transmitAtPreStart = 1  # Transmits data group at pre start phase of the CANoe measurement
    transmitAtStop = 2      # Transmits data group at stop of measurement
    transmitCyclic = 4      # Transmits the data group cyclically while the measurement is running.
                            # The transmit cycle is defined by the fields cycleTime and firstDuration
    transmitAtTrigger = 8   # Transmits the data group if the CAPL function FDXTriggerDataGroup is called


def reverse_dict(enum_class, value):
    for key, v in enum_class.__dict__.items():
        if v == value:
            return key
    return "Unknown value: %d" % value


fdxSignature = 0x584446656F4E4143
fdxMajorVersion = 0x1
fdxMinorVersion = 0x1
size_size = 2


def increase_seq_nr(nr):
    nr = nr + 1
    if nr > 0x7FFF:
        nr = 1
    return nr


class FdxCommand:
    def __init__(self, size, command):
        self.size = size
        self.command = command


class FdxCommandParser:
    def __init__(self):
        self.results = []  #type: typing.List[FdxCommand]
        self.done = False
        self.prev_seq_nr = 0

    def feed(self):
        if self.done:
            raise Exception("Parser already finished, create a new one to parse more commands")

        expected_signature = struct.pack('=Q', fdxSignature)
        for index in range(len(expected_signature)):
            b = yield
            if b == ord(expected_signature[index]):
                index += 1
            else:
                raise Exception("Expected byte %d at index %d, got %d" % (ord(expected_signature[index]), index, b))

        received_major_version = yield
        if received_major_version != fdxMajorVersion:
            raise Exception("Received FDX major version %d, only supporting version: %d" % (received_major_version, fdxMajorVersion))

        received_minor_version = yield
        if received_minor_version != fdxMinorVersion:
            pass # This is ok. minor_version change = backwards compatible

        number_of_commands1 = yield
        number_of_commands2 = yield
        (nrOfCommands, ) = struct.unpack('=h', bytearray([number_of_commands1, number_of_commands2]))

        seq_nr1 = yield
        seq_nr2 = yield
        (seq_nr, ) = struct.unpack('=h', bytearray([seq_nr1, seq_nr2]))
        next_seq_nr = increase_seq_nr(self.prev_seq_nr)
        if self.prev_seq_nr != 0 and next_seq_nr != seq_nr:
            logger.error("Sequence nr error, got %d, expected %d", seq_nr, next_seq_nr)
        self.prev_seq_nr = seq_nr

        reserved1 = yield  # NOQA
        reserved2 = yield  # NOQA

        for n in range(nrOfCommands):
            command = []
            len1 = yield
            len2 = yield
            command.append(len1)
            command.append(len2)

            (command_size, ) = struct.unpack('=h', bytearray([len1, len2]))

            for i in range(command_size - size_size):
                b = yield
                command.append(b)
            self.results.append(FdxCommand(command_size, command))
        self.done = True
        yield
        return


class FDXConnection:
    def __init__(self,
                 data_exchange_callback,    # type: typing.Any
                 ip,                        # type: str
                 port=2809                  # type: int
                 ):
        # type: (...) -> None
        self.ip = ip
        self.port = port
        self.next_seq_nr_to_send = 0
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.close_pipe = os.pipe()
        self.close_requested = False
        self.recv_thread = threading.Thread(target=self.receiver)
        self.recv_thread.daemon = True
        self.data_exchange_callback = data_exchange_callback
        self.last_received_measurement_status = kMeasurmentState.NotRunning
        self.recv_thread.start()

    def close(self):
        logger.debug("Closing receiver")
        os.write(self.close_pipe[1], b"x")
        self.recv_thread.join(20)
        os.close(self.close_pipe[0])
        os.close(self.close_pipe[1])
        self.sock.close()

    def receiver(self):
        parser = FdxCommandParser()
        logger.info("Starting receiver")
        if os.name == "nt":
            # TODO: Fix close wakeup for windows
            watch_list = [self.sock]
        else:
            watch_list = [self.sock, self.close_pipe[0]]
        while True:
            try:
                buffer_size = 1024 * 8

                (ready_to_read, _, _) = select.select(watch_list, [], [], None)
                if self.close_pipe[0] in ready_to_read:
                    os.read(self.close_pipe[0], 100)
                    break
                if self.sock in ready_to_read:
                    (data, (host, port)) = self.sock.recvfrom(buffer_size)

                    if host != self.ip and port != self.port:
                        logger.error("Received data from different host than sending to, host: %s, port: %d", host, port)
                        continue
                    logger_lowlevel_rx.debug("Received message: %r", data)
                    gen = parser.feed()
                    gen.send(None)
                    for d in data:
                        gen.send(ord(d))
                        if parser.done:
                            # TODO: I think we might be able to extract more accurate time stamps here if we combine multiple
                            # messages from the datagram to parse.
                            # See section 2.2.10 of CANoe FDX protocol
                            # "When CANoe transmits data. [...] via DataExchange command [...], the status message is
                            #  added to the datagram"
                            for c in parser.results:
                                self.parse(c)
                            parser = FdxCommandParser()
            except Exception as e:
                logger.exception("Parse error %r", e)
                parser = FdxCommandParser()
        logger.info("Receiver closed")

    def send_command(self, command):
        # type: (bytes) -> None
        reserved = 0
        nr_of_commands = 1
        header = struct.pack('=Qbbhhh',
                             fdxSignature,
                             fdxMajorVersion,
                             fdxMinorVersion,
                             nr_of_commands,
                             self.next_seq_nr_to_send,
                             reserved
                             )
        self.next_seq_nr_to_send = increase_seq_nr(self.next_seq_nr_to_send)

        datagram = header + struct.pack('=h', len(command) + size_size) + command
        logger_lowlevel_tx.debug("Sending data: %r", datagram)
        self.sock.sendto(datagram, (self.ip, self.port))

    def send_start(self):
        start_command = struct.pack('=h', kCommandCode.start)
        self.send_command(start_command)

    def send_stop(self):
        start_command = struct.pack('=h', kCommandCode.stop)
        self.send_command(start_command)

    def status_req(self):
        self.send_command(struct.pack('=h', kCommandCode.statusRequest))

    def send_data_exchange(self, groupId, dataSize, dataBytes):
        # type: (int, int, bytes) -> None
        assert(dataSize == len(dataBytes))
        data = struct.pack('=hhh', kCommandCode.dataExchange, groupId, dataSize)
        data += bytes(dataBytes)
        self.send_command(data)

    def send_data_request(self,
                          groupId):
        # type: (int) -> None
        self.send_command(struct.pack('=hh', kCommandCode.dataRequest, groupId))

    def send_free_running_request(self,
                                  group_id,             # type: int
                                  freeRunningFlags,     # type: int
                                  cycleTime_ns,         # type: int
                                  firstDuration_ns,     # type: int
                                  ):
        self.send_command(struct.pack('=hhhLL',
                                      kCommandCode.freeRunningRequest,
                                      group_id,
                                      freeRunningFlags,
                                      cycleTime_ns,
                                      firstDuration_ns))

    def send_free_running_cancel(self, group_id):
        self.send_command(struct.pack('=hh', kCommandCode.freeRunningCancel, group_id))

    def parse(self, command):
        # type: (FdxCommand) -> None
        (commandCode, ) = struct.unpack('=h', bytearray(command.command[2:4]))

        if commandCode == kCommandCode.status:
            (measurement_state, unused, timestamps) = struct.unpack('=b3sq', bytearray(command.command[4:]))
            self.last_received_measurement_status = measurement_state
        elif commandCode == kCommandCode.dataError:
            (groupId, dataErrorCode) = struct.unpack('=hh', bytearray(command.command[4:]))
            logger.error("DataError, groupid: %d, dataErrorCode: %s" % (groupId, reverse_dict(kDataError, dataErrorCode)))
        elif commandCode == kCommandCode.dataExchange:
            (groupId, dataSize) = struct.unpack('=hh', bytearray(command.command[4:8]))
            data = command.command[8:]
            if len(data) != dataSize:
                raise Exception("Data exchange length does not match, dataSize=%d, len(data)=%d, groupId=%d" %
                                (dataSize, len(data), groupId))

            self.data_exchange_callback(groupId, data)
        elif commandCode == kCommandCode.sequenceNrError:
            (received_seqnr, expected_seqnr) = struct.unpack('=hh', bytearray(command.command[4:8]))
            logger.error("Remote side report invalid seqnr received, expected %d, got %d" % (expected_seqnr, received_seqnr))
        else:
            logger.error("Received unhandled command: %s, data=%r",
                          reverse_dict(kCommandCode, commandCode),
                          command.command)

    def confirmed_start(self, timeout_sec=30):
        # type: (int) -> None
        deadline = time.time() + timeout_sec
        while time.time() < deadline:
            if self.last_received_measurement_status == kMeasurmentState.Running:
                return
            self.send_start()
            self.status_req()
            time.sleep(1)
        raise Exception(("Timeout waiting for measurement to start, current status: %s." + os.linesep + "Make sure you are connected to the Vector hardware") %
                        reverse_dict(kMeasurmentState, self.last_received_measurement_status))

    def confirmed_stop(self, timeout_sec=30):
        # type: (int) -> None
        deadline = time.time() + timeout_sec
        while time.time() < deadline:
            if self.last_received_measurement_status == kMeasurmentState.NotRunning:
                return
            self.send_stop()
            self.status_req()
            time.sleep(1)
        raise Exception("Timeout waiting for measurement to stop, current status: %s" %
                        reverse_dict(kMeasurmentState, self.last_received_measurement_status))

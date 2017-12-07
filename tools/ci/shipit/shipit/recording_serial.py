# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import threading
import time

import serial #type: ignore
import logging
from typing import List, Tuple

class RecordingSerial:
    def __init__(self,
                 port: str,
                 baudrate: int,
                 timeout_sec: int,
                 encoding="utf-8",
                 log_context_name: str = None) -> None:
        if log_context_name is None:
            self.log_context_name = __name__  + "." + port
        else:
            self.log_context_name = log_context_name
        self._s = serial.Serial(port, baudrate, timeout=timeout_sec)
        self._encoding = encoding
        self._output = []   # type: List[Tuple[float, str]]
        self._input = []    # type: List[Tuple[float, str]]
        self._read_index = 0
        self._close_event = threading.Event()
        self._output_lock = threading.Lock()
        self._read_thread = threading.Thread(target=self._read_worker, daemon=True, name="TTY Reader %s" % port)
        self._read_thread.start()

    def _read_worker(self):
        logger = logging.getLogger(self.log_context_name + "._IN")
        while not self._close_event.is_set():
            try:
                line = self._s.readline().decode(self._encoding)
                if len(line) > 0:   #TODO: Don't think this is correct
                    line = line.strip("\r\n")
                    t = time.time()
                    with self._output_lock:
                        self._output.append((t, line))
                    logger.debug("%s", line)
            except UnicodeDecodeError as e:
                logger.error("FAILED TO DECODE, %s", e.reason)

    def readline(self, timeout_sec: int):
        stop_time = time.time() + timeout_sec
        while time.time() < stop_time:
            with self._output_lock:
                if len(self._output) > self._read_index:
                    entry = self._output[self._read_index]
                    line = entry[1]
                    self._read_index += 1
                    return line
            time.sleep(0.1) #TODO: Use some threading signal instead
        return None

    def writeline(self, line: str):
        logger = logging.getLogger(self.log_context_name + ".OUT")
        logger.debug("%s", line)
        self._input.append((time.time(), line))
        self._s.write(line.encode(self._encoding))
        self._s.write("\n".encode(self._encoding))
        self._s.flush()

    def close(self):
        self._close_event.set()
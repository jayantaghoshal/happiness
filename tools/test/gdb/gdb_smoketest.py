# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import subprocess
import re
import threading
import sys
from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from collections import deque
import time
import logging

sys.path.append('/usr/local/lib/python2.7/dist-packages')
import typing

class GdbServerClientAndSymbolsTest(base_test.BaseTestClass):

    def wait_for_server_to_create_process(self, gdbserver_stdout_queue):
        # type: (typing.Deque[str]) -> str
        start_server_deadline = time.time() + 20
        while True:
            asserts.assertLess(time.time(), start_server_deadline,
                               "Timeout. Did not find PID of launched process through gdbserver64. See test host_log for more details.")
            try:
                line = gdbserver_stdout_queue.popleft()
            except IndexError:
                time.sleep(0.05)
                continue

            if "Can't bind address" in line or "Exiting" in line:
                raise Exception(line)

            pidmatch = re.search(r"Process.*created;\s*pid\s*=\s*(\d+)", line)
            if pidmatch:
                pid = pidmatch.group(1)
                logging.info("Pid: %r", pid)
                return pid


    def verify_gdbclient_can_set_breakpoints(self, gdbclient_stdout, gdbclient_stdin):
        # type: (typing.Deque[str], typing.BinaryIO) -> bool
        state = 0
        expected_lines_found = [
            {"line": "Breakpoint 1, main", "found": False},
            {"line": "vendor/volvocars/hardware/signals/dataelements/tools/signaltrace/signaltrace.cpp",
             "found": False}
        ]

        excpect_break_at_set_breakpoint_deadline = time.time() + 20
        output_line_candidate_to_process = ""
        while True:
            asserts.assertLess(time.time(), excpect_break_at_set_breakpoint_deadline,
                               "Timeout. Sequence of [set breakpoint/continue/expect breakpoint hit], was not seen. Current state is %d. See test host_log for more details." % state)
            try:
                output_line_candidate_to_process += gdbclient_stdout.popleft()
            except IndexError:
                time.sleep(0.001)
                continue

            split = re.split(r"(\n)|(\(gdb\) )", output_line_candidate_to_process, maxsplit=1)
            if len(split) <= 1:
                time.sleep(0.001)
                continue
            (line, split_newline, split_gdb, split_remains) = split
            if split_gdb is not None:
                line += split_gdb
            logging.info(line)
            output_line_candidate_to_process = split_remains

            if state == 0:
                if "(gdb)" in line:
                    state = 1
                    time.sleep(1)
                    gdbclient_stdin.write("break signaltrace.cpp:161\n".encode())
                    gdbclient_stdin.flush()
                    logging.info("write ok")
            elif state == 1:
                if "Breakpoint 1 at" in line:
                    logging.info("Go to state 2")
                    state = 2
            elif state == 2:
                if "(gdb)" in line:
                    state = 3
                    gdbclient_stdin.write("continue\n".encode())
                    gdbclient_stdin.flush()
            elif state == 3:
                for expected_line in expected_lines_found:
                    if expected_line["line"] in line:
                        expected_line["found"] = True
                if all((e["found"] for e in expected_lines_found)):
                    logging.info(
                        "Success! GDB can set breakpoints on line numbers and they will be hit and printed correctly later!")
                    return True

    def testGdb(self):
        # Arbitary choice of process, just any one that is preinstalled on target and doesn't change very often.
        server = subprocess.Popen(["adb", "shell", "gdbserver64 tcp:5039 /vendor/bin/signaltrace"],
                                  stderr=subprocess.PIPE,
                                  bufsize=1)
        try:
            # Keep reading the server stderr in background to avoid blocking the process when stdout buffer is full
            server_output_queue = deque()  # type: typing.Deque[str]
            def server_reader_worker():
                while True:
                    serverline = server.stderr.readline().decode()
                    if len(serverline) > 0:
                        logging.info("SERVER: ", serverline)
                    server_output_queue.append(serverline)
            server_reader_thread = threading.Thread(target=server_reader_worker)
            server_reader_thread.daemon = True
            server_reader_thread.start()

            target_pid = self.wait_for_server_to_create_process(server_output_queue)


            client = subprocess.Popen(["gdbclient.py",
                                       "-p", target_pid],
                                      stdout=subprocess.PIPE,
                                      stderr=subprocess.STDOUT,
                                      stdin=subprocess.PIPE,
                                      bufsize=1)
            try:
                # Read the output in a separate thread to avoid blocking and deadlocking the subprocess
                client_stdout = deque()  # type: typing.Deque[str]

                def output_reader_func():
                    while True:
                        client_stdout.append(client.stdout.read(1).decode())

                output_reader_thread = threading.Thread(target=output_reader_func)
                output_reader_thread.daemon = True
                output_reader_thread.start()

                success = self.verify_gdbclient_can_set_breakpoints(client_stdout, client.stdin)
                asserts.assertTrue(success, "Gdbclient failed to attach to server and set breakpoints")
            finally:
                client.kill()
        finally:
            server.kill()


if __name__ == "__main__":
    test_runner.main()
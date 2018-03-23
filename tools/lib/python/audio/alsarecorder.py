#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project
from contextlib import contextmanager
import threading
import time

# This starts a recording of the reference channel. Usage
#
# ar = AlsaRecorder(dut)  # dut is vts device-under-test object
# with ar.alsa_recorder (10):  # record for 10s
#     # Do stuff to trigger audio here
#
# # audio recording will be in file with name ar.host_fn
# # e.g.:
# assertTrue(verify_audio(ar.host_fn), "Audio does not match expected")


class AlsaRecorder(object):

    def __init__(self, dut):
        self.dut = dut

    @contextmanager
    def alsa_recorder(self,
                      duration_sec,     # type: int
                      file_name = None  # type: str
                      ):
        if file_name is None:
            file_name = 'recording_%s.wav' % (time.strftime('%Y%m%d_%H%M%S'))
        self.host_fn = '/tmp/' + file_name
        self.target_fn = '/storage/' + file_name


        def record(terminal,
                   file_name,  # type: str
                   duration    # type: int
                   ):
            rec_cmd = "alsa_arecord -D MicRef -c 6 -d %d -r 48000 -f S16_LE %s" % (duration_sec, file_name)
            terminal.Execute(rec_cmd)

        self.dut.shell.InvokeTerminal('alsa_record')
        self.dut.shell.alsa_record.Execute('rm -f %s' % self.target_fn)
        # Need a thread so that recording can go on in background
        thread = threading.Thread(target=record, args=(self.dut.shell.alsa_record, self.target_fn, duration_sec))
        thread.daemon = True
        thread.start()
        yield
        thread.join(duration_sec + 10)  # If it is not finished in duration + 10s then something have gone wrong
        self.dut.adb.pull(self.target_fn, self.host_fn)


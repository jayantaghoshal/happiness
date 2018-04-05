#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from contextlib import contextmanager
import subprocess
import threading
import time
import os
#import pkg_resources


# This starts a recording of the reference channel. Usage
#
# ar = AlsaRecorder()
# with ar.alsa_recorder (10):  # record for 10s
#     # Do stuff to trigger audio here
#
# # audio recording will be in file with name ar.host_fn
# # e.g.:
# assertTrue(verify_audio(ar.host_fn), "Audio does not match expected")


class AlsaRecorder(object):

    def __init__(self, vts_dut=None):
        self.vts_dut = vts_dut  # If vts_dut is none, then adb will be run via subprocess, with no dependency to VTS
        self.host_fn = 'Not Set'
        self.target_fn = 'Not Set'
        # Work-around for file being gone in P (until it is returned)
        self.binary = self._get_alsa_record_binary_name()

    def _adb_shell_cmd(self, cmd):
        if self.vts_dut is None:
            cmd_list = ['adb', 'shell'] + cmd.split(' ')
            res = subprocess.check_output(cmd_list, universal_newlines=True)
        else:  # use vts dut interface
            res = self.vts_dut.adb.shell(cmd)
        return res

    def _adb_pull_file(self, target_fn, host_fn):
        if self.vts_dut is None:
            subprocess.check_output(['adb', 'pull', self.target_fn, self.host_fn],
                                    universal_newlines=True)
        else:
            self.vts_dut.adb.pull(target_fn, host_fn)
        if not os.path.exists(host_fn):
            raise Exception("File %s does not exists on target" % host_fn)

    def _check_file_on_target(self, fn):
        ls_res = ""
        try:
            ls_res = self._adb_shell_cmd("ls %s" % fn).strip('\n')
        except subprocess.CalledProcessError:
            pass  # Don't care about the return code - we do analysis of returned string below.
        # if output from ls matches ls, then file is on target, if it it doesn't then it isn't
        return False if ls_res != fn else True

    def _get_alsa_record_binary_name(self):
        org_binary = '/vendor/bin/alsa_arecord'
        tmp_binary = '/vendor/bin/alsa_arecord_omr1'
        if self._check_file_on_target(org_binary):
            return org_binary
        elif self._check_file_on_target(tmp_binary):
            return tmp_binary
        else:
            raise Exception("No alsa_arecord / alsa_arecord_omr1 found on target")

    @contextmanager
    def alsa_recorder(self,
                      duration_sec,     # type: int
                      file_name=None,   # type: str
                      host_folder=None  # type: str
                      ):
        if file_name is None:
            file_name = 'recording_%s.wav' % (time.strftime('%Y%m%d_%H%M%S'))
        if host_folder is None:
            host_folder = '/tmp/'
        self.host_fn = os.path.join(host_folder, file_name)
        self.target_fn = '/storage/' + file_name

        def record(file_path,  # type: str
                   duration    # type: int
                   ):
            print("Duration:", duration)
            rec_cmd = "%s -D MicRef -c 6 -d %d -r 48000 -f S16_LE %s" % (self.binary, duration, file_path)
            self._adb_shell_cmd(rec_cmd)

        self._adb_shell_cmd('rm -f %s' % self.target_fn)
        # Need a thread so that recording can go on in background
        thread = threading.Thread(target=record, args=(self.target_fn, duration_sec))
        thread.daemon = True
        thread.start()
        yield
        thread.join(duration_sec + 10)  # If it is not finished in duration + 10s then something have gone wrong
        self._adb_pull_file(self.target_fn, self.host_fn)

#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.runners.host import const
from vts.utils.python.controllers import android_device
import os
import sys
import re
import logging
import thread
import threading
from subprocess import Popen, PIPE
from time import sleep
print(sys.path)
import audio.alsarecorder
import audio.audioutils
import audio.match

sys.path.append('/usr/local/lib/python2.7/dist-packages')

from fdx import fdx_client
from fdx import fdx_description_file_parser
from generated.pyDataElements import \
    FrSignalInterface, \
    FltIndcrTurnLeRe, \
    FltIndcrTurnLeFrnt, \
    FltIndcrTurnRiFrnt, \
    FltIndcrTurnRiRe, \
    IndcrDisp1WdSts, \
    EpbLampReq, \
    VehSpdLgtSafe, \
    BltRmnSound1, \
    BltRmnSound2, \
    BltRmnSound3, \
    BltRmnSound4


class VtsSoundNotificationIT(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        self.flexray = FrSignalInterface()
        self.host_folder = '/tmp'

    def getNormalizedSoundlist(self, filename, normalize_level):
        # Extract ref-channel to separate filecdhost_folder
        ch5_file = audio.audioutils.extract_channel(filename, 5)

        # Normalize file to normalize_level - makes it more similar to input.
        norm_file = audio.audioutils.normalize(ch5_file, normalize_level)

        # Get list of sounds (seprated by quiet)
        sounds = audio.audioutils.get_sounds(norm_file)
        return sounds

    # ----------------------------------------------------------------------------------------------------------
    # Test Park Brake sound.
    # ----------------------------------------------------------------------------------------------------------
    def testParkBrake(self):
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder= self.host_folder):
            self.flexray.EpbLampReq.send(EpbLampReq.map.On)
            self.flexray.VehSpdLgtSafe.send(
                5)
            pass
        self.flexray.EpbLampReq.send(EpbLampReq.map.Off)
        self.flexray.VehSpdLgtSafe.send(1)
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.5)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            1.23, 0.04, 0.25, 0.06),], reorder=False)
        asserts.assertEqual(res, True, "Park Brake sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Left Turn Indicator Functionality.
    # ----------------------------------------------------------------------------------------------------------
    def testLeftIndicator(self):
        _s = 0.5
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            self.flexray.FltIndcrTurnLeRe.send_repetitive(
                FltIndcrTurnLeRe.map.NoFlt)
            self.flexray.FltIndcrTurnLeFrnt.send_repetitive(
                FltIndcrTurnLeFrnt.map.NoFlt)

            for _ in range(5):
                self.flexray.IndcrDisp1WdSts.send(IndcrDisp1WdSts.map.LeOn)
                time.sleep(_s)
                self.flexray.IndcrDisp1WdSts.send(IndcrDisp1WdSts.map.Off)
                time.sleep(_s)

            self.flexray.FltIndcrTurnLeFrnt.stop_send()
            self.flexray.FltIndcrTurnLeRe.stop_send()
            pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            0.218, 0.05, 0.176, 0.05), audio.match.Match(0.0587, 0.05, 0.554, 0.05)], reorder=False)
        asserts.assertEqual(
            res, True, "TurnIndicator Left on sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Right Turn Indicator Functionality.
    # ----------------------------------------------------------------------------------------------------------
    def testRightIndicator(self):
        _s = 0.5
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            self.flexray.FltIndcrTurnRiRe.send_repetitive(
                FltIndcrTurnRiRe.map.NoFlt)
            self.flexray.FltIndcrTurnRiFrnt.send_repetitive(
                FltIndcrTurnRiFrnt.map.NoFlt)

            for _ in range(5):
                self.flexray.IndcrDisp1WdSts.send(IndcrDisp1WdSts.map.RiOn)
                time.sleep(_s)
                self.flexray.IndcrDisp1WdSts.send(IndcrDisp1WdSts.map.Off)
                time.sleep(_s)

            self.flexray.FltIndcrTurnRiFrnt.stop_send()
            self.flexray.FltIndcrTurnRiRe.stop_send()
            pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            0.218, 0.05, 0.176, 0.05), audio.match.Match(0.0587, 0.05, 0.554, 0.05)], reorder=False)
        asserts.assertEqual(
            res, True, "TurnIndicator Right on sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test TurnIndicator Left on and Right on
    # ----------------------------------------------------------------------------------------------------------
    def testBothIndicator(self):
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        self.flexray.FltIndcrTurnLeRe.send_repetitive(
            FltIndcrTurnLeRe.map.NoFlt)
        self.flexray.FltIndcrTurnLeFrnt.send_repetitive(
            FltIndcrTurnLeFrnt.map.NoFlt)
        self.flexray.FltIndcrTurnRiFrnt.send_repetitive(
            FltIndcrTurnRiFrnt.map.NoFlt)
        self.flexray.FltIndcrTurnRiRe.send_repetitive(
            FltIndcrTurnRiRe.map.NoFlt)

        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):

            for _ in range(5):
                self.flexray.IndcrDisp1WdSts.send(
                    IndcrDisp1WdSts.map.LeAndRiOn)
                time.sleep(_s)
                self.flexray.IndcrDisp1WdSts.send(IndcrDisp1WdSts.map.Off)
                time.sleep(_s)
            self.flexray.FltIndcrTurnRiRe.stop_send()
            self.flexray.FltIndcrTurnRiFrnt.stop_send()
            self.flexray.FltIndcrTurnLeFrnt.stop_send()
            self.flexray.FltIndcrTurnLeRe.stop_send()
            pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            0.218, 0.02, 0.0693, 0.05), audio.match.Match(0.0587, 0.02, 0.6773, 0.05)], reorder=False)
        asserts.assertEqual(
            res, True, "TurnIndicator Left ON and Right ON sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound1 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound1(self):
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.BltRmnSound1.send(BltRmnSound1.map.Trig)
                time.sleep(_s)
                self.flexray.BltRmnSound1.send(BltRmnSound1.map.NoTrig)
                time.sleep(_s)
        pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            1.386, 0.02, 0.6133, 0.05)],reorder=False)
        asserts.assertEqual(res, True, "Belt Reminder sound1 NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound2 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound2(self):
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.BltRmnSound2.send(BltRmnSound2.map.Trig)
                time.sleep(_s)
                self.flexray.BltRmnSound2.send(BltRmnSound2.map.NoTrig)
                time.sleep(_s)
        pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            1.386, 0.02, 0.6133, 0.05)],reorder=False)
        asserts.assertEqual(res, True, "Belt Reminder sound2 NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound3 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound3(self):
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.BltRmnSound3.send(BltRmnSound3.map.Trig)
                time.sleep(_s)
                self.flexray.BltRmnSound3.send(BltRmnSound3.map.NoTrig)
                time.sleep(_s)
        pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            1.386, 0.02, 0.6133, 0.05)],reorder=False)
        asserts.assertEqual(res, True, "Belt Reminder sound3 NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound4 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound4(self):
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.BltRmnSound4.send(BltRmnSound4.map.Trig)
                time.sleep(_s)
                self.flexray.BltRmnSound4.send(BltRmnSound4.map.NoTrig)
                time.sleep(_s)
        pass
        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        sounds = self.getNormalizedSoundlist(fn, -0.9)

        # Match that list of sounds towards a list of Match objects
        res = audio.match.match_sound_silence(sounds, [audio.match.Match(
            1.986, 0.02, 0.0426, 0.005)],reorder=False)
        asserts.assertEqual(res, True, "Belt Reminder sound4 NOT MATCHED - FAILED")

if __name__ == "__main__":
    logging.basicConfig(
        level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()

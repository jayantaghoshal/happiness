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
import logging
import logging.config
sys.path.append('/usr/local/lib/python2.7/dist-packages')

import audio.alsarecorder
import audio.audioutils
import audio.match

import vehiclehalcommon
from generated import datatypes as de_types

class VtsSoundNotificationIT(base_test.BaseTestClass):
    def setUpClass(self):
        self.logger = logging.getLogger('Test')
        self.logger.setLevel(logging.DEBUG)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        self.logger.addHandler(ch)
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        self.flexray = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        self.host_folder = '/tmp'
        self.log_list = list()  #type: list

    def setUp(self):
        #clear the log used for sound matching
        self.log_list = list()

    def _log_list_as_str(self):
        return '\n'.join(self.log_list)

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
        # This is (?) Park Brake warning sound, SPA-UI-S018.wav
        self.logger.info("=== Test testParkBrake Start ===")
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        epb_lamp_req = de_types.EpbLampReqRec()
        epb_lamp_req.EpbLampReq = de_types.EpbLampReqType1.On
        epb_lamp_req.EpbLampReqChks = 0
        epb_lamp_req.EpbLampReqCntr = 0
        veh_spd_lgt = de_types.VehSpdLgtSafe1()
        veh_spd_lgt.VehSpdLgt = 5
        veh_spd_lgt.VehSpdLgtQf = de_types.GenQf1.AccurData
        veh_spd_lgt.VehSpdLgtCntr = 0
        veh_spd_lgt.VehSpdLgtChks = 0

        with ar.alsa_recorder(10, host_folder=self.host_folder):
            self.flexray.send_EpbLampReq(epb_lamp_req)
            self.flexray.send_VehSpdLgtSafe(veh_spd_lgt)

        epb_lamp_req.EpbLampReq = de_types.EpbLampReqType1.Off
        self.flexray.send_EpbLampReq(epb_lamp_req)
        veh_spd_lgt.VehSpdLgt = 1
        self.flexray.send_VehSpdLgtSafe(veh_spd_lgt)

        try:
            fn = ar.host_fn
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.5)
        except Exception as e:
            self.logger.warning("Exception during testParkBrake sounds retrieval", e)
        try:
            # Match that list of sounds towards a list of Match objects
            # S018.wav is currently 1.493s, so we expect sound to repeated every 1.5s
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                1.23, 0.04, 0.25, 0.06)], reorder=False, log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())

        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testParkBrake result %s ===" % res)
        asserts.assertEqual(res, True, "Park Brake sound NOT MATCHED - FAILED")



    # ----------------------------------------------------------------------------------------------------------
    # Test Left Turn Indicator Functionality.
    # ----------------------------------------------------------------------------------------------------------
    def testLeftIndicator(self):
        # This is (?) Left indicator sound, SPA-UI-S026.wav
        self.logger.info("=== Test testLeftIndicator Start ===")
        _s = 0.5
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            self.flexray.send_FltIndcrTurnLeRe(de_types.DevErrSts2.NoFlt)
            self.flexray.send_FltIndcrTurnLeFrnt(de_types.DevErrSts2.NoFlt)

            for _ in range(5):
                self.flexray.send_IndcrDisp1WdSts(de_types.IndcrSts1.LeOn)
                time.sleep(_s)
                self.flexray.send_IndcrDisp1WdSts(de_types.IndcrSts1.Off)
                time.sleep(_s)

        fn = ar.host_fn
        try:
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testLeftIndicator sounds retrieval", e)
        try:
            # Match that list of sounds towards a list of Match objects
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                0.218, 0.05, 0.176, 0.05), audio.match.Match(0.0587, 0.05, 0.554, 0.05)],
                                                  reorder=False, log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testLeftIndicator result %s ===" % res)
        asserts.assertEqual(
            res, True, "TurnIndicator Left on sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Right Turn Indicator Functionality.
    # ----------------------------------------------------------------------------------------------------------
    def testRightIndicator(self):
        # This is (?) Right indicator sound, SPA-UI-S026.wav
        self.logger.info("=== Test testRightIndicator Start ===")
        _s = 0.5
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            self.flexray.send_FltIndcrTurnRiRe(de_types.DevErrSts2.NoFlt)
            self.flexray.send_FltIndcrTurnRiFrnt(de_types.DevErrSts2.NoFlt)

            for _ in range(5):
                self.flexray.send_IndcrDisp1WdSts(de_types.IndcrSts1.RiOn)
                time.sleep(_s)
                self.flexray.send_IndcrDisp1WdSts(de_types.IndcrSts1.Off)
                time.sleep(_s)

        fn = ar.host_fn
        try:
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testRightIndicator sounds retrieval", e)
        try:
            # Match that list of sounds towards a list of Match objects
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                0.218, 0.05, 0.176, 0.05), audio.match.Match(0.0587, 0.05, 0.554, 0.05)],
                                                  reorder=False, log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False
        self.logger.info("=== testRightIndicator result %s ===" % res)

        asserts.assertEqual(res, True, "TurnIndicator Right on sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test TurnIndicator Left on and Right on
    # ----------------------------------------------------------------------------------------------------------
    def testBothIndicator(self):
        # This is (?) Hazard indicator sound, SPA-UI-S028.wav
        self.logger.info("=== Test testBothIndicator Start ===")
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        self.flexray.send_FltIndcrTurnLeRe(de_types.DevErrSts2.NoFlt)
        self.flexray.send_FltIndcrTurnLeFrnt(de_types.DevErrSts2.NoFlt)
        self.flexray.send_FltIndcrTurnRiFrnt(de_types.DevErrSts2.NoFlt)
        self.flexray.send_FltIndcrTurnRiRe(de_types.DevErrSts2.NoFlt)

        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.send_IndcrDisp1WdSts(de_types.IndcrSts1.LeAndRiOn)
                time.sleep(_s)
                self.flexray.send_IndcrDisp1WdSts(de_types.IndcrSts1.Off)
                time.sleep(_s)

        fn = ar.host_fn
        # Get list of normalized sounds (seprated by quiet)
        try:
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testBothIndicator sounds retrieval", e)
        try:
            # Match that list of sounds towards a list of Match objects
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                0.218, 0.02, 0.0693, 0.05), audio.match.Match(0.0587, 0.02, 0.6773, 0.05)],
                                                  reorder=False, log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testBothIndicator result %s ===" % res)
        asserts.assertEqual(
            res, True, "TurnIndicator Left ON and Right ON sound NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound1 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound1(self):
        # This is (?) the US Belt sound, SPA-UI-S055.wav

        self.logger.info("=== Test testBeltReminderSound1 Start ===")
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.send_BltRmnSound1(de_types.Trig1.Trig)
                time.sleep(_s)
                self.flexray.send_BltRmnSound1(de_types.Trig1.NoTrig)
                time.sleep(_s)

        fn = ar.host_fn
        try:
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testBeltReminderSound1 sounds retrieval", e)
        try:
            # Match that list of sounds towards a list of Match objects
            # S055.wav is 1.400s, so we expect sound + silence to be 2.0s
            # The level is low the last 0.2s of file, but probably enough for detection, so expect 1.4 + 0.6
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                1.386, 0.02, 0.6133, 0.05)], log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testBeltReminderSound1 result %s ===" % res)
        asserts.assertEqual(res, True, "Belt Reminder sound1 NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound2 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound2(self):
        # This is (?) the ROW initial Belt sound, SPA-UI-S023.wav
        self.logger.info("=== Test testBeltReminderSound2 Start ===")
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()

        #ar._adb_shell_cmd('log -t BRS2_Test "START_TEST"')
        #ar._adb_shell_cmd('log -t BRS2_Test "Send_AudMsgReq-1"')
        #self.flexray.send_AudMsgReq(1)
        #time.sleep(_s)
        #ar._adb_shell_cmd('log -t BRS2_Test "Send_AudMsgReq-0"')
        #self.flexray.send_AudMsgReq(0)
        #time.sleep(_s)

        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for i in range(5):
                #ar._adb_shell_cmd('log -t BRS2_Test "Send_BltRmnSnd-Trig"')
                self.flexray.send_BltRmnSound2(de_types.Trig1.Trig)
                time.sleep(_s)
                #ar._adb_shell_cmd('log -t BRS2_Test "Send_BltRmnSnd-Trig"')
                self.flexray.send_BltRmnSound2(de_types.Trig1.NoTrig)
                time.sleep(_s)

        #ar._adb_shell_cmd('log -t BRS2_Test "Send_AudMsgReq-1_and_BltRmnSnd-Trig"')
        #self.flexray.send_AudMsgReq(1)
        #self.flexray.send_BltRmnSound2(de_types.Trig1.Trig)
        #time.sleep(_s)
        #ar._adb_shell_cmd('log -t BRS2_Test "Send_AudMsgReq-0_and_BltRmnSnd-NoTrig"')
        #self.flexray.send_BltRmnSound2(de_types.Trig1.NoTrig)
        #self.flexray.send_AudMsgReq(0)
        #time.sleep(_s)


        fn = ar.host_fn
        try:
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testBeltReminderSound2 sounds retrieval", e)
            raise e
        try:
            # Match that list of sounds towards a list of Match objects
            # S023.wav is 1.997s, so we expect sound + silence to be 2.0s
            # This sound is a bit 'iffy' so may be difficult to match with current logic?
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                1.997, 0.02, 0.05, 0.04)], log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testBeltReminderSound2 result %s ===" % res)
        asserts.assertEqual(res, True, "Belt Reminder sound2 NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound3 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound3(self):
        # This is (?) the ROW final Belt sound, SPA-UI-S024.wav
        self.logger.info("=== Test testBeltReminderSound3 Start ===")
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.send_BltRmnSound3(de_types.Trig1.Trig)
                time.sleep(_s)
                self.flexray.send_BltRmnSound3(de_types.Trig1.NoTrig)
                time.sleep(_s)

        fn = ar.host_fn
        try:
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testBeltReminderSound3 sounds retrieval", e)
            raise e
        try:
            # Match that list of sounds towards a list of Match objects
            # S024.wav is 1.400s long and goes all the way, so we expect 1.4 + 0.6
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                1.386, 0.02, 0.6133, 0.05)], reorder=False, log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testBeltReminderSound3 result %s ===" % res)

        asserts.assertEqual(res, True, "Belt Reminder sound3 NOT MATCHED - FAILED")

    # ----------------------------------------------------------------------------------------------------------
    # Test Belt Reminder Sound4 On
    # ----------------------------------------------------------------------------------------------------------
    def testBeltReminderSound4(self):
        # This is (?) the Rear Belt sound, SPA-UI-S054.wav
        self.logger.info("=== Test testBeltReminderSound4 Start ===")
        _s = 1
        ar = audio.alsarecorder.AlsaRecorder()
        # start recording for 10 seconds.
        with ar.alsa_recorder(10, host_folder=self.host_folder):
            for _ in range(5):
                self.flexray.send_BltRmnSound4(de_types.Trig1.Trig)
                time.sleep(_s)
                self.flexray.send_BltRmnSound4(de_types.Trig1.NoTrig)
                time.sleep(_s)

        fn = ar.host_fn
        try:
            # Get list of normalized sounds (seprated by quiet)
            sounds = self.getNormalizedSoundlist(fn, -0.9)
        except Exception as e:
            self.logger.warning("Exception during testBeltReminderSound4 sounds retrieval", e)
            raise e
        try:
            # Match that list of sounds towards a list of Match objects
            # S054.wav is 2.000s, with 1.997s sound, so we expect silence to be short!
            res = audio.match.match_sound_silence(sounds, [audio.match.Match(
                1.986, 0.02, 0.0426, 0.08)], reorder=False, log_buffer_list=self.log_list)
            if not res:
                self.logger.info("Sound matching returned:\n" + self._log_list_as_str())
        except audio.match.NoMatch:
            self.logger.warning("Too few sound recorded to match!")
            res = False

        self.logger.info("=== testBeltReminderSound4 result %s ===" % res)
        asserts.assertEqual(res, True, "Belt Reminder sound4 NOT MATCHED - FAILED")

if __name__ == "__main__":
    logging.basicConfig(
        level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()

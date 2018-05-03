#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import sys

from vts.runners.host import asserts
from vts.runners.host import test_runner

sys.path.append('/usr/local/lib/python2.7/dist-packages')
from vf_common import vf_base_test
from vf_common import uiautomator_common
from vf_common.signals_common import wait_for_fr_signal, assert_fr_signal_equals
from generated import datatypes as DE

# Button ids
dai_off_id = uiautomator_common.AppId.app_context_vehiclefunctions + "dai_state_1_no_info_button"
dai_visual_id = uiautomator_common.AppId.app_context_vehiclefunctions + "dai_state_2_visual_only_button"
dai_visual_sound_id  = uiautomator_common.AppId.app_context_vehiclefunctions + "dai_state_3_visual_and_audio_button"
wait_after_button_click = 0.5
DAI_SETTING = 557842437 # Hard coded VCC-HAL property ID for DAI_SETTING_VALUE


class VtsDAISettingsComponentTest(vf_base_test.VFBaseTest):

    def testDriveAwayInfoCCDisabled_NoButtons_SendDefaultValues(self):
        self.change_carconfig_and_reboot_if_needed([(315, 1)])
        self.setUpVehicleFunction()

        self.ui.scrollAndAssertViewsAreNotFound([
            dai_off_id,
            dai_visual_id,
            dai_visual_sound_id])

        wait_for_fr_signal(self.fr.get_DriveAwayInfoActvReq, 0)
        wait_for_fr_signal(self.fr.get_DriveAwayInfoSoundWarnActvReq, 0)


    def testDriveAwayInfoCCEnabled(self):
        self.change_carconfig_and_reboot_if_needed([(315, 3)])
        self.setUpVehicleFunction()
        fr = self.fr
        vhal = self.vhal

        self.ui.waitUntilViewAvailable(dai_off_id, timeout_seconds=5)
        dai_off_button = self.ui.scrollDownAndFindViewById(dai_off_id)
        dai_visual_button = self.ui.scrollDownAndFindViewById(dai_visual_id)
        dai_visual_sound_button = self.ui.scrollDownAndFindViewById(dai_visual_sound_id)

        # Set UsgModSts to ACTIVE, setting should change in ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        # Set setting to OFF
        dai_off_button.click()
        vhal.wait_for_Int32_equals(0, DAI_SETTING)
        wait_for_fr_signal(fr.get_DriveAwayInfoActvReq, 0)
        wait_for_fr_signal(fr.get_DriveAwayInfoSoundWarnActvReq, 0)

        # Set setting to VISUAL
        dai_visual_button.click()
        vhal.wait_for_Int32_equals(1, DAI_SETTING)
        wait_for_fr_signal(fr.get_DriveAwayInfoActvReq, 1)
        wait_for_fr_signal(fr.get_DriveAwayInfoSoundWarnActvReq, 0)

        # Set setting to VISUAL AND SOUND
        dai_visual_sound_button.click()
        vhal.wait_for_Int32_equals(2, DAI_SETTING)
        wait_for_fr_signal(fr.get_DriveAwayInfoActvReq, 1)
        wait_for_fr_signal(fr.get_DriveAwayInfoSoundWarnActvReq, 1)



        # Set UsgModSts to NOT_ACTIVE, setting should not change in NOT_ACTIVE
        current_dai_settings_value = vhal.getVhalInt32(DAI_SETTING)
        current_dai_settings_signal_visual = fr.get_DriveAwayInfoActvReq()
        current_dai_settings_signal_visual_sound = fr.get_DriveAwayInfoSoundWarnActvReq()

        self.signals.set_usage_mode(DE.UsgModSts1.UsgModInActv)

        dai_off_button.click()
        vhal.assertVhalInt32Equal(current_dai_settings_value, DAI_SETTING)
        assert_fr_signal_equals(fr.get_DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        assert_fr_signal_equals(fr.get_DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        dai_visual_button.click()
        vhal.assertVhalInt32Equal(current_dai_settings_value, DAI_SETTING)
        assert_fr_signal_equals(fr.get_DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        assert_fr_signal_equals(fr.get_DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        dai_visual_sound_button.click()
        vhal.assertVhalInt32Equal(current_dai_settings_value, DAI_SETTING)
        assert_fr_signal_equals(fr.get_DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        assert_fr_signal_equals(fr.get_DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)



if __name__ == "__main__":
    test_runner.main()

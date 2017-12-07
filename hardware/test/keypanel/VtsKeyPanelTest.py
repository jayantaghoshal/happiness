# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#!/usr/bin/env python

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.runners.host import const

import logging
import json
from time import sleep,time
import sys
import os
from subprocess import check_output
sys.path.append('/usr/local/lib/python2.7/dist-packages')

from generated.pyDataElements import FrSignalInterface

LOGCAT_LOGTAG = "Test_KeyPanelEventSniffer: "
LOGCAT_GREP_COMMAND = 'logcat -d -s "Test_KeyPanelEventSniffer" | tail -n 2'
LOGCAT_GREP_COMMAND4 = 'logcat -d -s "Test_KeyPanelEventSniffer" | tail -n 4'
KEYUP_EVENT = "keyup"
KEYDOWN_EVENT = "keydown"
BUTTON_PRESS_SLEEP = 0.25
EXPECT_LOG_SLEEP = 1.0

def dolsusb():
    output = check_output(['lsusb'])
    return output

# Poll host usb devices and check if IHU is connected or not
def is_ihuconnected():
    lsusb = dolsusb()
    # 8087:09ef is the vendor:product usb code for the IHU (Intel:xyz)
    return lsusb.find('8087:09ef') != -1

class ComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        # Starting CANoe simulation might trigger a reboot,
        # Set up required signals for unit to boot and ensure it is booted
        #
        # The environment variables VECTOR_FDX_PORT and VECTOR_FDX_IP must be set
        # if a real connection to the flexray bus is what you want
        # (which is needed to run these tests for real)
        self.logger=logging.getLogger('KeyPanelTests')
        self.logger.info("setUpClass()")
        self.flexray = FrSignalInterface()
        usgModSts = self.flexray.UsgModSts
        fRNetworkStatus = self.flexray.FRNetworkStatus
        vehBattUSysU = self.flexray.VehBattUSysU
        usgModSts.send(usgModSts.map.UsgModDrvg)
        fRNetworkStatus.send(1)
        vehBattUSysU.send(12)
        self.waitForIhuUp(30)

        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        while True :
            self.dut.shell.one.Execute('am start -S com.volvocars.keypaneleventsniffer/com.volvocars.keypaneleventsniffer.MainActivity')
            sleep(2)
            logData = self.dut.shell.one.Execute('logcat -d -s "Test_KeyPanelEventSniffer" | tail -n 1')[const.STDOUT][0]
            if logData.find('InForeground') > -1:
                break
            else:
                self.dut.shell.one.Execute('am force-stop com.volvocars.keypaneleventsniffer')

    def waitForIhuUp(self, maxwaittime):
        # Wait for IHU(MP) to come up
        startt = time()
        self.logger.info('Wait for IHU to go up (%d)' % maxwaittime)
        while not is_ihuconnected() and (time()-startt < maxwaittime):
            sleep(1)
        self.logger.info('IHU %s after %f secs, should be up' % (('up' if is_ihuconnected() else 'down' ), time()-startt))
        return is_ihuconnected()

    def waitForIhuDown(self, maxwaittime):
        # Wait for IHU(MP) to power off
        startt = time()
        self.logger.info('Wait for IHU to go down (%d)' % maxwaittime)
        while is_ihuconnected() and (time()-startt < maxwaittime):
            sleep(1)
        self.logger.info('IHU %s after %f secs, should be down' % (('up' if is_ihuconnected() else 'down' ), time()-startt))
        return not is_ihuconnected()

    def tearDownClass(self):
        self.dut.shell.one.Execute('am force-stop com.volvocars.keypaneleventsniffer')
        self.dut.shell.one.Execute('pm uninstall -k com.volvocars.keypaneleventsniffer')


    def extractData(self,consoleout):
        eventdata = json.loads(consoleout)
        return eventdata["event"], eventdata["keycode"]

    def verifyLog(self,logData,expectedKeyCode):
        if logData.find(LOGCAT_LOGTAG) > -1:
            eventdata = logData.split('\n')[0].split(LOGCAT_LOGTAG)[1]
            eventdata2 = logData.split('\n')[1].split(LOGCAT_LOGTAG)[1]
            event,keycode = self.extractData(eventdata)
            asserts.assertEqual(keycode, expectedKeyCode)
            asserts.assertEqual(event, KEYDOWN_EVENT)
            event,keycode = self.extractData(eventdata2)
            asserts.assertEqual(keycode, expectedKeyCode)
            asserts.assertEqual(event, KEYUP_EVENT)
        else:
            asserts.assertEqual(False, "Test failed " + expectedKeyCode)

    """
    Description TestCase
    Test all KeyPanelButtons and that they are mapped as expected keyevents in android world.
    Test launches a android Application that then logs onKeyDown and onKeyUp to logcat.
    Python script send flexray signal for each button under test.
    Waits 1.5 sec then execute logcat and takes 2 latest entries with the LOGTAG Test_KeyPanelEventSniffer
    KeyDown and KeyUp is expected.
    For mappings see DesipKeyHandlerService (device/delphi/vcc_ihu/private/bb_android_desip_key_handler/libdesipkeyhandler/src/DesipKeyHandlerService.cpp)
    {Btn4ForUsrSwtPanFrntReq.Psd && Btn4ForUsrSwtPanFrntReq.NotPsd           VIP_KEY_LIN19_DEFROST_FRONT,                   }
    {Btn3ForUsrSwtPanFrntReq.Psd && Btn3ForUsrSwtPanFrntReq.NotPsd           VIP_KEY_LIN19_NEXT,                            }
    {Btn1ForUsrSwtPanFrntReq.Psd && Btn1ForUsrSwtPanFrntReq.NotPsd           VIP_KEY_LIN19_PLAY_PAUSE,                      }
    {Btn5ForUsrSwtPanFrntReq.Psd && Btn5ForUsrSwtPanFrntReq.NotPsd           VIP_KEY_LIN19_DEFROST_REAR,                    }
    {Btn2ForUsrSwtPanFrntReq.Psd && Btn2ForUsrSwtPanFrntReq.NotPsd           VIP_KEY_LIN19_PREVIOUS,                        }
    {                                                                        VIP_KEY_LIN19_CCSM_BTN6,                       }
    {                                                                        VIP_KEY_LIN19_DMSM_ACCEPT,                     }
    {BtnMmedLeRiSts.BtnRiPsd && BtnMmedLeRiSts.BtnLePsd                      VIP_KEY_FLRAY_SWC_LEFT_RIGHT,                  }
    {                                                                        VIP_KEY_FLRAY_SWC_SET,                         }
    {BtnMmedModSts.Psd && BtnMmedModSts.NotPsd                               VIP_KEY_FLRAY_SWC_MODE,                        }
    {BtnAudFbSts.Psd && BtnAudFbSts.NotPSd                                   VIP_KEY_FLRAY_SWC_PUSH_TO_TALK,                }
    {BtnAudVolSts.BtnVolUpPsd& && BtnAudVolSts.BtnVolDownPsd                 VIP_KEY_FLRAY_SWC_VOLUME,                      }
    {RotyDirReq1 && RotyPosReq1                                              VIP_KEY_VOLUME_KNOB,                           }
    {                                                                        VIP_KEY_LIN19_CLIMA_HILO_SEC_ROW_LEFT,         }
    {                                                                        VIP_KEY_LIN19_TEMPR_SETPOINT_ROW_SEC_RIGHT,    }
    {                                                                        VIP_KEY_LIN19_CLIMA_HILO_SEC_ROW_RIGHT,        }
    {                                                                        VIP_KEY_LIN19_UPDTEMPR_SETPOINT_ROW_SEC_RIGHT, }
    {                                                                        VIP_KEY_LIN19_UPDTEMPR_SETPOINT_ROW_SEC_LEFT,  }
    {                                                                        VIP_KEY_LIN19_UPDSEAT_HEAT_LVL_ROW_SEC_RIGHT,  }
    {                                                                        VIP_KEY_LIN19_UPDSEAT_HEAT_LVL_ROW_SEC_LEFT,   }
    {                                                                        VIP_KEY_LIN19_UPDFAN_LVL_ROW_SEC,              }
    {                                                                        VIP_KEY_LIN19_SEAT_HEAT_LVL_ROW_SEC_LEFT,      }
    {                                                                        VIP_KEY_LIN19_TEMPR_SETPOINT_ROW_SEC_LEFT,     }
    {                                                                        VIP_KEY_LIN19_SEAT_HEAT_LVL_ROW_SEC_RIGHT,     }
    {                                                                        VIP_KEY_LIN19_SEAT_HEAT_LVL_LEFT,              }
    {                                                                        VIP_KEY_LIN19_SEAT_HEAT_LVL_RIGHT,             }
    {                                                                        VIP_KEY_LIN19_UPDSEAT_VENT_LVL_ROW_SEC_LEFT,   }
    {                                                                        VIP_KEY_LIN19_SEAT_VENT_LVL_ROW_SEC_LEFT,      }
    {                                                                        VIP_KEY_LIN19_SEAT_VENT_LVL_ROW_SEC_RIGHT,     }
    {                                                                        VIP_KEY_LIN19_UPDSEAT_VENT_LVL_ROW_SEC_RIGHT,  }
    {                                                                        VIP_KEY_LIN19_FAN_LVL_ROW_SEC,                 }
    {                                                                        VIP_KEY_FLRAY_SEAT_PASSENGER_AB,               }
    {                                                                        VIP_KEY_FLRAY_SEAT_PASSENGER_HORIZ,            }
    {                                                                        VIP_KEY_FLRAY_SEAT_PASSENGER_VERTI,            }
    {                                                                        VIP_KEY_FLRAY_SEAT_DRIVER_AB,                  }                                                                            VIP_KEY_FLRAY_SEAT_DRIVER_HORIZ,               }
    {                                                                        VIP_KEY_FLRAY_SEAT_DRIVER_VERTI,               }
    """
    def testKeyPanelPlayPause(self):
        #PlayPause Button 1
        Btn1ForUsrSwtPanFrntReq = self.flexray.Btn1ForUsrSwtPanFrntReq
        Btn1ForUsrSwtPanFrntReq.send(Btn1ForUsrSwtPanFrntReq.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        Btn1ForUsrSwtPanFrntReq.send(Btn1ForUsrSwtPanFrntReq.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_MEDIA_PLAY_PAUSE")

    def testKeyPanelLeftButton2(self):
        #Left Button 2
        Btn2ForUsrSwtPanFrntReq = self.flexray.Btn2ForUsrSwtPanFrntReq
        Btn2ForUsrSwtPanFrntReq.send(Btn2ForUsrSwtPanFrntReq.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        Btn2ForUsrSwtPanFrntReq.send(Btn2ForUsrSwtPanFrntReq.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_MEDIA_PREVIOUS")

    def testKeyPanelRightButton2(self):
        # Right Button 3
        Btn3ForUsrSwtPanFrntReq = self.flexray.Btn3ForUsrSwtPanFrntReq
        Btn3ForUsrSwtPanFrntReq.send(Btn3ForUsrSwtPanFrntReq.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        Btn3ForUsrSwtPanFrntReq.send(Btn3ForUsrSwtPanFrntReq.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_MEDIA_NEXT")

    def testKeyPanelDefrostFrontButton4(self):
        # Defrost Front Button 4
        Btn4ForUsrSwtPanFrntReq = self.flexray.Btn4ForUsrSwtPanFrntReq
        Btn4ForUsrSwtPanFrntReq.send(Btn4ForUsrSwtPanFrntReq.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        Btn4ForUsrSwtPanFrntReq.send(Btn4ForUsrSwtPanFrntReq.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_4")

    def testKeyPanelDefrostRearButton5(self):
        # Defrost Rear Button 5
        Btn5ForUsrSwtPanFrntReq = self.flexray.Btn5ForUsrSwtPanFrntReq
        Btn5ForUsrSwtPanFrntReq.send(Btn5ForUsrSwtPanFrntReq.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        Btn5ForUsrSwtPanFrntReq.send(Btn5ForUsrSwtPanFrntReq.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_7")


    def testKeyPanelVolumeRotationButton(self):
        #Volume Rotation Button ClockWise
        RotyDirReq1 = self.flexray.RotyDirReq1
        RotyPosReq1 = self.flexray.RotyPosReq1
        RotyDirReq1.send(RotyDirReq1.map.CW)
        RotyPosReq1.send(1)
        sleep(BUTTON_PRESS_SLEEP)
        RotyDirReq1.send(RotyDirReq1.map.Idle)
        RotyPosReq1.send(0)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_VOLUME_UP")

        #Volume Rotation Button CounterClockWise
        RotyDirReq1.send(RotyDirReq1.map.CCW)
        RotyPosReq1.send(1)
        sleep(BUTTON_PRESS_SLEEP)
        RotyDirReq1.send(RotyDirReq1.map.Idle)
        RotyPosReq1.send(0)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_VOLUME_DOWN")

    def testKeyPanelPushToTalk(self):
        #PushToTalk
        BtnAudFbSts = self.flexray.BtnAudFbSts
        BtnAudFbSts.send(BtnAudFbSts.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        BtnAudFbSts.send(BtnAudFbSts.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_VOICE_ASSIST")

    def testKeyPanelModeSelect(self):
        #Mode Select
        BtnMmedModSts = self.flexray.BtnMmedModSts
        BtnMmedModSts.send(BtnMmedModSts.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        BtnMmedModSts.send(BtnMmedModSts.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND4)[const.STDOUT][0]
        #SPECIALCASE
        if logData.find(LOGCAT_LOGTAG) > -1:
            alleventdata = logData.split('\n')
            event,keycode = self.extractData(alleventdata[0].split(LOGCAT_LOGTAG)[1])
            asserts.assertEqual(keycode, "KEYCODE_BUTTON_MODE")
            asserts.assertEqual(event, KEYDOWN_EVENT)
            event,keycode = self.extractData(alleventdata[1].split(LOGCAT_LOGTAG)[1])
            asserts.assertEqual(keycode, "KEYCODE_MENU")
            asserts.assertEqual(event, KEYDOWN_EVENT)
            event,keycode = self.extractData(alleventdata[2].split(LOGCAT_LOGTAG)[1])
            asserts.assertEqual(keycode, "KEYCODE_BUTTON_MODE")
            asserts.assertEqual(event, KEYUP_EVENT)
            event,keycode = self.extractData(alleventdata[3].split(LOGCAT_LOGTAG)[1])
            asserts.assertEqual(keycode, "KEYCODE_MENU")
            asserts.assertEqual(event, KEYUP_EVENT)
        else:
            asserts.assertEqual(False, "BtnMmedModSts Button Failed")

    def testKeyButtonLeft(self):
        #Left
        BtnMmedLeRiSts = self.flexray.BtnMmedLeRiSts
        BtnMmedLeRiSts.send(BtnMmedLeRiSts.map.BtnLePsd)
        sleep(BUTTON_PRESS_SLEEP)
        BtnMmedLeRiSts.send(BtnMmedLeRiSts.map.NoBtnPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_MEDIA_PREVIOUS")

        #Right
        BtnMmedLeRiSts.send(BtnMmedLeRiSts.map.BtnRiPsd)
        sleep(BUTTON_PRESS_SLEEP)
        BtnMmedLeRiSts.send(BtnMmedLeRiSts.map.NoBtnPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_MEDIA_NEXT")

    def testKeyUp(self):
        #Up
        BtnAudVolSts = self.flexray.BtnAudVolSts
        BtnAudVolSts.send(BtnAudVolSts.map.BtnVolUpPsd)
        sleep(BUTTON_PRESS_SLEEP)
        BtnAudVolSts.send(BtnAudVolSts.map.NoBtnPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_VOLUME_UP")

        #Down
        BtnAudVolSts.send(BtnAudVolSts.map.BtnVolDwnPsd)
        sleep(BUTTON_PRESS_SLEEP)
        BtnAudVolSts.send(BtnAudVolSts.map.NoBtnPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"KEYCODE_VOLUME_DOWN")

        #Not yet implemented buttons.
        """#DriveMode Button
        SwtAcptReq = self.flexray.SwtAcptReq
        SwtAcptReq.send(SwtAcptReq.map.Psd)
        sleep(BUTTON_PRESS_SLEEP)
        SwtAcptReq.send(SwtAcptReq.map.NotPsd)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"DriveMode Button")

        #Drivemode UP
        RotyDirReq2 = self.flexray.RotyDirReq2
        RotyPosReq2 = self.flexray.RotyPosReq2
        RotyDirReq2.send(RotyDirReq2.map.CCW)
        RotyPosReq2.send(31)
        sleep(BUTTON_PRESS_SLEEP)
        RotyPosReq2.send(0)
        sleep(BUTTON_PRESS_SLEEP)
        RotyPosReq2.send(1)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"DriveMode Button")
        #Drivemode Down
        RotyDirReq2.send(RotyDirReq2.map.CW)
        RotyPosReq2.send(0)
        sleep(BUTTON_PRESS_SLEEP)
        RotyPosReq2.send(31)
        sleep(BUTTON_PRESS_SLEEP)
        RotyPosReq2.send(30)
        sleep(EXPECT_LOG_SLEEP)
        logData = self.dut.shell.one.Execute(LOGCAT_GREP_COMMAND)[const.STDOUT][0]
        self.verifyLog(logData,"DriveMode Button")
"""

if __name__ == "__main__":
    #logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()

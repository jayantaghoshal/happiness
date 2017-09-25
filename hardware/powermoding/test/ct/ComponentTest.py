#!/usr/bin/env python

import logging
import os
from subprocess import Popen, PIPE
from time import sleep
import datetime
from generated.pyDataElements import FrSignalInterface


# execute and fetch the output from the 'lsusb' command
def dolsusb():
    p = Popen(['lsusb'], stdout=PIPE)
    output, _ = p.communicate()
    return output


# Poll host usb devices and check if IHU is connected or not
def is_ihuconnected():
    lsusb = dolsusb()
    # 8087:09ef is the vendor:product usb code for the IHU (Intel:xyz)
    return lsusb.find('8087:09ef') != -1


class ComponentTest(object):
    def __init__(self):
        self.logger=logging.getLogger('powermoding')

        # The environment variables VECTOR_FDX_PORT and VECTOR_FDX_IP must be set
        # if a real connection to the flexray bus is what you want
        # (which is needed to run these tests for real)
        self.flexray = FrSignalInterface()


    def power_control_ihu(self,on):
        # TODO
        # on=True -> power on IHU
        # on=False -> power off IHU
        # this function shall control the physical power to the whole IHU
        # currently it deos nothing since the API to do this is not clear and the required CI hardware is not in place
        # If it fails to power on/off then log and do exit(1)
        self.logger.info('Power ' + ('on' if on else 'off'))
        pass

    def sleep(self,duration):
        self.logger.info('sleep %d seconds',duration)
        sleep(duration)


    def assertEqual(self, first, second, msg=None):
        """Asserts that first == second"""
        if second == first:
            return
        fullMsg = "%s is not equal to %s" % (str(first), str(second))
        if msg is not None:
            fullMsg = "%s (%s)" % (msg,fullMsg)
        self.logger.error(fullMsg)
        exit(1)


    def assertNotEqual(self, first, second, msg=None):
        """Asserts that first != second"""
        if second != first:
            return
        fullMsg = "%s is equal to %s" % (str(first), str(second))
        if msg is not None:
            fullMsg = "%s (%s)" % (msg,fullMsg)
        self.logger.error(fullMsg)
        exit(1)


    def startTest(self):
        '''
        The following signals are used by the test cases:
        ihu-out
            MmedMaiPwrMod (Sleep,ON)
            MmedHmiModStd (OFF,ON)

        ihu-in
            UsgModSts (all modes)
            CarModSts1 (Normal)
            DoorDrvrSts (Closed, Open)
        '''
        self.test1()
        self.test2()


    '''test1: basic start up
    pre-req
        Power Off, CarMode=normal, UsgMode=Abandoned and Close the door.
    
    part 1:  
        Power on, wait 20 secs and verify that the IHU is NOT
        running and that we have MmedMaiPwrMod==Sleep , MmedHmiModStd==OFF
    
    part 2:
        Open & close the door and enter UsgModActive. Verify after 40 secs that the IHU is alive. 
    '''
    def test1(self):
        usgModSts     = self.flexray.UsgModSts
        carModSts1    = self.flexray.CarModSts1
        doorDrvrSts   = self.flexray.DoorDrvrSts
        mmedMaiPwrMod = self.flexray.MmedMaiPwrMod
        mmedHmiModStd = self.flexray.MmedHmiModStd
        self.logger=logging.getLogger('powermoding.test1')
        self.logger.info('starting')

        # pre-req
        self.power_control_ihu(False)
        usgModSts.send(usgModSts.map.UsgModAbdnd)
        carModSts1.send(carModSts1.map.CarModNorm)
        doorDrvrSts.send(doorDrvrSts.map.Clsd)
        self.sleep(1.0)

        # part 1
        self.power_control_ihu(True)
        self.sleep(20.0) # the IHU is up after about 11 secs when it starts up (which it should not do here!!)
        # Verify that we don't have any IHU USB connection to host
        # and that we have MmedMaiPwrMod==Sleep and MmedHmiModStd==OFF
        self.assertEqual(is_ihuconnected(), False, 'IHU USB shall NOT be connected')
        self.assertEqual(mmedMaiPwrMod.receive(), mmedMaiPwrMod.map.IHUStateSleep, 'mmedMaiPwrMod shall be == Sleep')
        self.assertEqual(mmedHmiModStd.receive(), mmedHmiModStd.map.InfModeOff, 'mmedHmiModStd shall be == Off')

        # part 2
        # change to InActive
        usgModSts.send(usgModSts.map.UsgModInActv)
        self.sleep(1.0)
        # Open and close the driver door
        doorDrvrSts.send(doorDrvrSts.map.Opend)
        self.sleep(1.0)
        doorDrvrSts.send(doorDrvrSts.map.Clsd)
        # change to convenience and then to active
        usgModSts.send(usgModSts.map.UsgModCnvinc)
        self.sleep(1.0)
        usgModSts.send(usgModSts.map.UsgModActv)

        # Now wait 20+20 secs verify that we have a IHU USB connection to host
        # and that we have received MmedMaiPwrMod==ON and MmedHmiModStd==ON
        self.sleep(20.0)
        self.assertEqual(is_ihuconnected(), True, 'IHU USB shall be connected')
        self.sleep(20.0)
        self.assertEqual(is_ihuconnected(), True, 'IHU USB shall be connected')
        self.assertEqual(mmedMaiPwrMod.receive(), mmedMaiPwrMod.map.IHUStateOn, 'mmedMaiPwrMod shall be == On')
        self.assertEqual(mmedHmiModStd.receive(), mmedHmiModStd.map.InfModeOn, 'mmedHmiModStd shall be == On')


    '''test2: basic shutdown
    pre-req
        Power On, Set CarMode=normal, UsgMode=Active
        wait 40 secs and verify that the IHU is up

    part 1:
    UsgMod=InActive and open+close the door. Wait 30 secs and verify the IHU is down
    '''
    def test2(self):
        usgModSts = self.flexray.UsgModSts
        carModSts1 = self.flexray.CarModSts1
        doorDrvrSts = self.flexray.DoorDrvrSts
        mmedMaiPwrMod = self.flexray.MmedMaiPwrMod
        mmedHmiModStd = self.flexray.MmedHmiModStd

        logger = logging.getLogger('powermoding.test2')
        logger.info('starting')

        # pre-req
        usgModSts.send(usgModSts.map.UsgModActv)
        carModSts1.send(carModSts1.map.CarModNorm)
        doorDrvrSts.send(doorDrvrSts.map.Clsd)
        self.sleep(0.5)
        self.power_control_ihu(True) # this should lead to a full start of the IHU
        self.sleep(40.0)
        self.assertEqual(is_ihuconnected(), True, 'IHU USB shall be connected')
        self.assertEqual(mmedMaiPwrMod.receive(), mmedMaiPwrMod.map.IHUStateOn, 'mmedMaiPwrMod shall be == On')
        self.assertEqual(mmedHmiModStd.receive(), mmedHmiModStd.map.InfModeOn, 'mmedHmiModStd shall be == On')

        # part 1
        # change to InActive
        usgModSts.send(usgModSts.map.UsgModInActv)
        self.sleep(1.0)
        # Open and close the driver door
        doorDrvrSts.send(doorDrvrSts.map.Opend)
        self.sleep(1.0)
        doorDrvrSts.send(doorDrvrSts.map.Clsd)
        # Now wait 30 secs and after that verify that we don't have any IHU USB connection to host
        # and that we have received MmedMaiPwrMod==Sleep and MmedHmiModStd==Off
        self.sleep(30.0)
        self.assertEqual(is_ihuconnected(), False, 'IHU USB shall NOT be connected')
        self.assertEqual(mmedMaiPwrMod.receive(), mmedMaiPwrMod.map.IHUStateSleep, 'mmedMaiPwrMod shall be == Sleep')
        self.assertEqual(mmedHmiModStd.receive(), mmedHmiModStd.map.InfModeOff, 'mmedHmiModStd shall be == Off')

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')

    ct = ComponentTest()
    ct.startTest()
    exit(0)

#!/usr/bin/env python

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

import logging
from subprocess import check_output
from time import sleep,time
from generated.pyDataElements import FrSignalInterface


# execute and fetch the output from the 'lsusb' command
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
        pass
#        self.dut = self.registerController(android_device)[0]
#        self.dut.shell.InvokeTerminal("one")
#        self.shell = self.dut.shell.one
#        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode


    def setUp(self):
        self.logger.info("setUp()")
        # Starting CANoe simulation might trigger a reboot,
        # Set up required signals for unit to boot and ensure it is booted
        #
        # The environment variables VECTOR_FDX_PORT and VECTOR_FDX_IP must be set
        # if a real connection to the flexray bus is what you want
        # (which is needed to run these tests for real)
        self.flexray = FrSignalInterface()
        usgModSts = self.flexray.UsgModSts
        fRNetworkStatus = self.flexray.FRNetworkStatus
        vehBattUSysU = self.flexray.VehBattUSysU

        usgModSts.send(usgModSts.map.UsgModDrvg)
        fRNetworkStatus.send(1)
        vehBattUSysU.send(12)

        self.waitForIhuUp(30)


    def tearDown(self):
        self.logger.info('tearDown()')
        self.flexray.close()


    def __init__(self):
        self.logger=logging.getLogger('powermoding_basic')


    def power_control_ihu(self,on):
        # TODO
        # on=True -> power on IHU
        # on=False -> power off IHU
        # this function shall control the physical power to the whole IHU
        # currently it does nothing since the API to do this is not clear and the required CI hardware is not in place
        # If it fails to power on/off then log and do exit(1)
        self.logger.info('Power ' + ('on' if on else 'off'))
        pass


    def waitForIhuUp(self, maxwaittime):
        # Wait for IHU(MP) to come up
        startt = time()
        self.logger.info('Wait for IHU to go up (%d)', maxwaittime)
        while not is_ihuconnected() and (time()-startt < maxwaittime):
            sleep(1)
        self.logger.info('IHU %s after %d secs, should be up', ('up' if is_ihuconnected() else 'down' ), time()-startt)
        return is_ihuconnected()


    def waitForIhuDown(self, maxwaittime):
        # Wait for IHU(MP) to power off
        startt = time()
        self.logger.info('Wait for IHU to go down (%d)', maxwaittime)
        while is_ihuconnected() and (time()-startt < maxwaittime):
            sleep(1)
        self.logger.info('IHU %s after %d secs, should be down', ('up' if is_ihuconnected() else 'down' ), time()-startt)
        return not is_ihuconnected()


    def sleep(self,duration):
        self.logger.info('sleep %d seconds',duration)
        sleep(duration)


    '''test1: basic start up
    pre-req
        Power Off, CarMode=normal, UsgMode=Abandoned and Close the door.
    
    part 1:  
        Power on, wait 20 secs and verify that the IHU is NOT
        running and that we have MmedMaiPwrMod==Sleep , MmedHmiModStd==OFF
    
    part 2:
        Open & close the door and enter UsgModActive. Verify after 40 secs that the IHU is alive. 
    '''
    '''
    # TODO waiting for power switch control
    def testUp(self):
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
        asserts.assertFalse(is_ihuconnected(), 'IHU USB shall NOT be connected')
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateSleep, 'mmedMaiPwrMod shall be == Sleep')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOff, 'mmedHmiModStd shall be == Off')

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
        asserts.assertTrue(is_ihuconnected(), 'IHU USB shall be connected')
        self.sleep(20.0)
        asserts.assertTrue(is_ihuconnected(), 'IHU USB shall be connected')
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateOn, 'mmedMaiPwrMod shall be == On')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOn, 'mmedHmiModStd shall be == On')
    '''

    '''test2: basic shutdown
    pre-req
        Power On, Set CarMode=normal, UsgMode=Active
        wait 40 secs and verify that the IHU is up

    part 1:
        UsgMod=InActive and open+close the door. Wait 30 secs and verify the IHU is down
    '''
    '''
    # TODO waiting for power switch control
    def testDown(self):
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
        asserts.assertTrue(is_ihuconnected(), 'IHU USB shall be connected')
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateOn, 'mmedMaiPwrMod shall be == On')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOn, 'mmedHmiModStd shall be == On')

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
        asserts.assertFalse(is_ihuconnected(), 'IHU USB shall NOT be connected')
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateSleep, 'mmedMaiPwrMod shall be == Sleep')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOff, 'mmedHmiModStd shall be == Off')
    '''

    '''test3: basic shutdown and then startup without power switch control capability
    pre-req
        Power is On, Set CarMode=normal, UsgMode=Active, door closed
        wait 40 secs and verify that the IHU is up

    part 1:
        UsgMod=InActive and open+close the door. Wait 30 secs and verify the IHU is down

    part 2:  
        Power on, wait 20 secs and verify that the IHU is NOT
        running and that we have MmedMaiPwrMod==Sleep , MmedHmiModStd==OFF
    '''
    def testDownUp(self):
        usgModSts = self.flexray.UsgModSts
        carModSts1 = self.flexray.CarModSts1
        doorDrvrSts = self.flexray.DoorDrvrSts
        mmedMaiPwrMod = self.flexray.MmedMaiPwrMod
        mmedHmiModStd = self.flexray.MmedHmiModStd

        logger = logging.getLogger('powermoding.test3')
        logger.info('starting')

        # pre-req
        logger.info('pre-req...')
        usgModSts.send(usgModSts.map.UsgModActv)
        carModSts1.send(carModSts1.map.CarModNorm)
        doorDrvrSts.send(doorDrvrSts.map.Clsd)
        up = self.waitForIhuUp(40)
        asserts.assertTrue(up, 'IHU shall be up after 40 secs but is not')
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateOn, 'mmedMaiPwrMod shall be == On')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOn,  'mmedHmiModStd shall be == On')

        # part 1
        # change to InActive
        logger.info('part 1...')
        usgModSts.send(usgModSts.map.UsgModInActv)
        self.sleep(1.0)
        # Open and close the driver door
        doorDrvrSts.send(doorDrvrSts.map.Opend)
        self.sleep(1.0)
        doorDrvrSts.send(doorDrvrSts.map.Clsd)
        # Now wait 30 secs and after that verify that we don't have any IHU USB connection to host
        # and that we have received MmedMaiPwrMod==Sleep and MmedHmiModStd==Off
        down = self.waitForIhuDown(30)
        asserts.assertTrue(down, 'IHU shall be down after 30 secs but is not')
        self.sleep(1.0)
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateSleep, 'mmedMaiPwrMod shall be == Sleep')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOff, 'mmedHmiModStd shall be == Off')

        # part 2
        logger.info('part 2...')
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
        up = self.waitForIhuUp(30) # wait max 30 secs to have it up again
        asserts.assertTrue(up, 'IHU shall be up after 30 secs but is not')
        # Wait another 20 secs to verify it is still up
        # and that we have received MmedMaiPwrMod==ON and MmedHmiModStd==ON
        self.sleep(20.0)
        asserts.assertTrue(is_ihuconnected(), 'IHU USB shall still be up but is not')
        asserts.assertEqual(mmedMaiPwrMod.get(), mmedMaiPwrMod.map.IHUStateOn, 'mmedMaiPwrMod shall be == On')
        asserts.assertEqual(mmedHmiModStd.get(), mmedHmiModStd.map.InfModeOn, 'mmedHmiModStd shall be == On')


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
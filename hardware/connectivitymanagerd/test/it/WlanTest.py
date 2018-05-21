# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import threading

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

from volvocars.hardware.connectivitymanagerd.test.tcamipcbsimulator import messagehandler
from volvocars.hardware.connectivitymanagerd.test.tcamipcbsimulator import wlanservice

sys.path.append('/tmp/ihu/.local/lib/python2.7/site-packages')
from uiautomator import device as d


class WlanTest(base_test.BaseTestClass):

    """Running tests"""

    def setUpClass(self):
        self._relay_package_name = 'com.volvocars.connectivitymanager.relay'
        self._relay_activity_name = 'com.volvocars.connectivitymanager.relay/.MainActivity'

        self._wlan_station_mode_switch_id = 'com.volvocars.connectivitymanager.relay:id/wifiStationSwitch'

        self._logger = logging.getLogger('WLAN Test')
        print 'wat'
        self.dut = self.registerController(android_device)[0]
        print 'wat2'
        self._msg_handler = messagehandler.MessageHandler(
            self.default_message_callback)
        self.wlan = wlanservice.WlanService()

        self._msg_handler.start()

    def tearDownClass(self):
        self._msg_handler.stop()
        self.dut.cleanUp()
        # Force stopping remote uiautomator helper app. Not Closing  it
        # results in sockets not closing properly on host
        self.dut.adb.shell('am force-stop com.github.uiautomator')

    def setUp(self):
        try:
            # Using am force-stop seems to have unsufficient rights to stop a
            # system app. So use killall.
            self.dut.adb.shell(
                'killall ' + self._relay_package_name)
        except:
            pass

    def testWlanModeNotificationAndSetRequest(self):

        def cb(msg, caller=self):
            asserts.assertEqual(msg['operation_type'], self._msg_handler._MESSAGE_TYPES[
                                'REQUEST'], "Wrong type of message received")
            payload = caller.wlan.encode_wlan_mode_notification_message(
                self.wlan._WLAN_MODES['OFF'])
            pdu = caller._msg_handler.construct_response(msg['service_id'], msg['operation_id'], msg[
                'sender_handle_id'], self._msg_handler._MESSAGE_TYPES['RESPONSE'], payload)
            caller._msg_handler.send_message(pdu)
            caller._msg_handler.register_callback(
                caller.default_message_callback)

        self._msg_handler.register_callback(cb)

        self.dut.adb.shell('am start ' + self._relay_activity_name)
        asserts.assertTrue(
            d(packageName=self._relay_package_name).exists, "Package is up")

        self._msg_handler.register_callback(self.default_message_callback)

        payload = self.wlan.encode_wlan_mode_notification_message(
            self.wlan._WLAN_MODES['STATION'])
        pdu = self._msg_handler.construct_pdu(self.wlan.service_id, self.wlan._OPERATION_IDS[
                                              'WLANMode'], self._msg_handler._MESSAGE_TYPES['NOTIFICATION'], payload)

        self._msg_handler.send_message(pdu)

        switchButton = d(
            resourceId=self._wlan_station_mode_switch_id, clickable='true', checked='true')
        asserts.assertTrue(switchButton.exists, "Switch is clickable")
        switchButton.click()
        switchButton = d(
            resourceId=self._wlan_station_mode_switch_id, checked='false')
        asserts.assertTrue(switchButton.exists, "Switch is on, expected off")

    def testRelaySendingWlanModeGetRequestAtStartup(self):
        sem = threading.Semaphore()
        sem.acquire()

        def cb(msg, caller=self, sem=sem):
            asserts.assertEqual(msg['operation_type'], self._msg_handler._MESSAGE_TYPES[
                                'REQUEST'], "Wrong type of message received")
            payload = caller.wlan.encode_wlan_mode_notification_message(
                self.wlan._WLAN_MODES['OFF'])
            pdu = caller._msg_handler.construct_response(msg['service_id'], msg['operation_id'], msg[
                'sender_handle_id'], self._msg_handler._MESSAGE_TYPES['RESPONSE'], payload)
            caller._msg_handler.send_message(pdu)
            caller._msg_handler.register_callback(
                caller.default_message_callback)
            sem.release()

        self._msg_handler.register_callback(cb)

        self.dut.adb.shell('am start ' + self._relay_activity_name)
        asserts.assertTrue(
            d(packageName=self._relay_package_name).exists, "Expected: Package is up")

        asserts.assertTrue(
            self._wait_lock(sem, 4), "Expected: Callback called at least once")

        switchButton = d(
            resourceId=self._wlan_station_mode_switch_id, clickable='true', checked='false')
        asserts.assertTrue(
            switchButton.exists, "Expected: Switch is clickable and unchecked")

    def testStartUpDontRespond(self):
        sem = threading.Semaphore()
        sem.acquire()

        def cb(msg, caller=self, sem=sem):
            if not msg['operation_type'] == self._msg_handler._MESSAGE_TYPES['REQUEST']:
                asserts.assertTrue(False, "Unexpected call")

            # Dont reply
            sem.release()
            caller._msg_handler.register_callback(
                caller.non_responsive_message_callback)

        self._msg_handler.register_callback(cb)

        self.dut.adb.shell('am start ' + self._relay_activity_name)
        asserts.assertTrue(
            d(packageName=self._relay_package_name).exists, "Package is up")

        asserts.assertTrue(
            self._wait_lock(sem, 4), "Expected: Callback called at least once")

        switchButton = d(
            resourceId=self._wlan_station_mode_switch_id, clickable='false')
        asserts.assertTrue(
            switchButton.exists, "Expected: switch is available but not clickable")

    def non_responsive_message_callback(self, msg):
        self._logger.info("Ignoring message: %s:%s:%s (%s)[%s]", msg['service_id'], msg[
            'operation_id'], msg['operation_type'], msg['sender_handle_id'], msg['payload'])

    def default_message_callback(self, msg):
        self._logger.info("Processing message: %s:%s:%s (%s)[%s]", msg['service_id'], msg[
            'operation_id'], msg['operation_type'], msg['sender_handle_id'], msg['payload'])
        if msg['operation_type'] == self._msg_handler._MESSAGE_TYPES['REQUEST']:
            payload = self.wlan.encode_wlan_mode_notification_message(
                self.wlan._WLAN_MODES['OFF'])
            pdu = self._msg_handler.construct_response(msg['service_id'], msg['operation_id'], msg[
                'sender_handle_id'], self._msg_handler._MESSAGE_TYPES['RESPONSE'], payload)
            self._logger.info("Sending Get-Response")
            self._msg_handler.send_message(pdu)
        elif msg['operation_type'] == self._msg_handler._MESSAGE_TYPES['SET_REQUEST']:
            pdu = self._msg_handler.construct_response(msg['service_id'], msg['operation_id'], msg[
                'sender_handle_id'], self._msg_handler._MESSAGE_TYPES['RESPONSE'], msg['payload'].decode('hex'))
            self._logger.info("Sending Set-Response")
            self._msg_handler.send_message(pdu)
        else:
            self._logger.warning('UNHANDLED REQUEST')

    def _wait_lock(self, lock, timeout):
        current_time = start_time = time.time()
        while current_time < start_time + timeout:
            if lock.acquire(False):
                return True
            else:
                time.sleep(min(1, (start_time + timeout - current_time)))
                current_time = time.time()
        return False

if __name__ == "__main__":
    logging.basicConfig(stream=sys.stdout,
                        format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.DEBUG)
    test_runner.main()

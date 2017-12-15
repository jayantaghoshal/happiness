# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import requests
import sys
import json
import time
import string

''' Class to control the IP Power Management (IPM) device
    IPM docs: http://www.planet.com.tw/en/product/product.php?id=48844#dl'''
class Client(object):

    def __init__(self, ip):
        self._ip = ip

    def get_status(self):
        return self._status()

    def get_power_state(self, port):
        return (True if self._status()['OutSwitch'][int(port)-1]['OutStat'] else False)

    def on(self,port):
        return (True if self._ctrl(1, port) == 1 else False)

    def off(self,port):
        return (True if self._ctrl(2, port) == 0 else False)

    def reboot(self,port,sleep_time=10):
        self.off(port)
        time.sleep(sleep_time)
        self.on(port)

    def _status(self):
        url = "http://%s/dev_status.csp" % (self._ip)
        headers = {
            'User-Agent': 'ipm.py'
        }
        auth = {'auth_user': 'dark', 'auth_passwd': 'lab123'}
        response = requests.post(url, data=auth, headers=headers)
        return response.json()  # returns json dictionary

    def _ctrl(self,state,port):
        url = "http://%s/out_ctrl.csp?port=%s&ctrl_kind=%s" % (self._ip, port, state)
        headers = {
            'User-Agent': 'ipm.py'
        }
        auth = {'auth_user': 'dark', 'auth_passwd': 'lab123'}
        response = requests.get(url, auth=('dark', 'lab123'), headers=headers)
        response.raise_for_status()
        return response.json()['OutCtrl'][0]['Ret'] #returns status code


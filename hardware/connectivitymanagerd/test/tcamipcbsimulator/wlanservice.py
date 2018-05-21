# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from . import asncodec


class WlanService(object):

    _OPERATION_IDS = {
        'WLANMode': '0400'
    }

    _WLAN_MODES = {
        'OFF': 'off',
        'STATION': 'sta',
        'ACCESS_POINT': 'ap',
        'WORKSHOP': 'staWS'
    }

    def __init__(self):

        self._asnobj = asncodec.AsnHelper()
        if self._asnobj is None:
            raise ValueError('Invalid ASN Object !!!')

        self._asnhelper = self._asnobj.asnlib

        self.service_id = '00A4'

    def encode_wlan_mode_notification_message(self, wlan_mode):
        ipcb_wlan_mode = {'wLANMode': wlan_mode}
        return self._asnhelper.encode('OpWLANMode-Notification', ipcb_wlan_mode)

    def encode_wlan_mode_response_message(self, wlan_mode):
        ipcb_wlan_mode = {'wLANMode': wlan_mode}
        return self._asnhelper.encode('OpWLANMode-Response', ipcb_wlan_mode)

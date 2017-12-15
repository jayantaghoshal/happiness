# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import tkinter.ttk

def init(app):
    lcma = LCMAApp(app)


class LCMAApp:
    def __init__(self, app):
        self.lkaCounter = 0
        self.lcmaCounter = 0
        self.app = app

        app.add_sender_element("RsdsSysStsLe")
        app.add_sender_element("RsdsSysStsRi")
        app.add_sender_element("LcmaOn1")
        app.register_message_handler("LaneKeepAidActv", self.LaneKeepAidActvSts)
        app.register_message_handler("BlisSwOnOff", self.lcmaOn1)

    def LaneKeepAidActvSts(self, msg_raw, msg_data):
        self.lkaCounter += 1

        lkaValue = msg_data['Sts']

        print(("LaneKeepAidActv value ", lkaValue))

        # Calculate the correct status value depending on the received value
        lkaStatusValue = 0

        if self.lkaCounter % 5 != 0:
            if lkaValue == 0:
                lkaStatusValue = 1  # Off
            elif lkaValue == 1:
                lkaStatusValue = 6  # On
        else:
            lkaStatusValue = 5  # Set service required every 5th time

        self.app.external_send("/i/LaneKeepAidSts", lkaStatusValue, "FctSts2")


    def lcmaOn1(self, msg_data):
        self.lcmaCounter += 1

        lcmaStatusRequest = msg_data['value']

        print(("LaneChangeMergeAid status request value " + str(lcmaStatusRequest)))

        # Calculate the correct status value depending on the received value
        lcmaStatusValue = 0

        if self.lcmaCounter % 3 != 0:
            if lcmaStatusRequest == 0:
                lcmaStatusValue = 2  # Off
            elif lcmaStatusRequest == 1:
                lcmaStatusValue = 1  # On
        else:
            if self.lcmaCounter % 6 == 0:
                lcmaStatusValue = 3  # Set TrlrOff every 6th time to produce an error symbol in the HMI
            else:
                lcmaStatusValue = 0  # Set StrtUpOn, this value shall be ignored by the API.

        self.app.external_send('/i/LcmaOn1', lcmaStatusValue, 'LcmaCtraOn')


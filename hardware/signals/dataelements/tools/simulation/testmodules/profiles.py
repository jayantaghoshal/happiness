# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import tkinter.ttk
from generated import datatypes
from Sym import carsimconnection

def init(app):
    profile = ProfileApp(app)

class ProfileApp:
    def __init__(self, app):
        self.app = app

        # Enabled Profile settings
        bProfAct = tkinter.ttk.Button(app.master, text="Send 5 enabled profiles", command=self.Send_ProfAct)
        app.add_external_button_row(bProfAct, None)

        # KeyReadResponse
        bKeyReadSts = tkinter.ttk.Button(app.master, text="Key Read Sts", command=self.Send_KeyReadResponse)
        app.add_external_button_row(bKeyReadSts, None)

        app.register_message_handler("ProfChg", self.ProfileResponse)


        app.add_sender_element("ProfPenSts1")
        app.add_sender_element("NrOfKeyAvl")
        app.add_sender_element("KeyReadStsToProfCtrl")

    def ProfileResponse(self, msg_data: carsimconnection.CarsimDataElementValue):
        profId = msg_data.value
        print("Prof req value ", profId)
        if profId > 0: #Simulate back a profile change if incoming request is more than 0(ProfUnknown) as that is used as a "invalid" value
            self.app.external_send2('ProfPenSts1', profId)

    def Send_ProfAct(self):
        to_send = datatypes.ProfAct1()
        to_send.Prof1 = True
        to_send.Prof1 = True
        to_send.Prof2 = True
        to_send.Prof3 = True
        to_send.Prof4 = True
        to_send.Prof5 = True
        to_send.Prof6 = False
        to_send.Prof7 = False
        to_send.Prof8 = False
        to_send.Prof9 = False
        to_send.Prof10 = False
        to_send.Prof11 = False
        to_send.Prof12 = False
        self.app.external_send2("ProfAct", to_send)

    def Send_KeyReadResponse(self):
        to_send = datatypes.KeyReadStsToProfCtrl()
        to_send.KeyId = datatypes.KeyId1.Key1
        to_send.Boolean = True
        self.app.external_send2("KeyReadStsToProfCtrl", to_send)


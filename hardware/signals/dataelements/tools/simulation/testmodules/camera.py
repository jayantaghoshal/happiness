# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sched
import threading
import time
import tkinter
import tkinter.ttk

def init(app):
    fu = FastUpdates(app)

    app.add_sender_element("VehModMngtGlbSafe1")
    app.add_sender_element("VehSpdLgtSafe")
    app.add_sender_element("VehMtnStSafe")
    app.add_sender_element("DrvrDesDir")

    app.add_sender_element("PrkgAssiManvActvSts")
    app.add_sender_element("PrkgCamSysAvlSts")
    app.add_sender_element("TrSts")
    app.add_sender_element("TrlrPrsntSts1WdSts")
    app.add_sender_element("DrvrDesDir")

    app.add_sender_element("PinionSteerAg1")
    app.add_sender_element("NetTelmActvt")

    app.add_sender_element("DoorDrvrSts")
    app.add_sender_element("DoorDrvrReSts")
    app.add_sender_element("DoorPassReSts")
    app.add_sender_element("DoorPassSts")

    app.add_sender_element("MirrFoldStsAtDrvr")
    app.add_sender_element("MirrFoldStsAtPass")

    app.add_sender_element("DoorDrvrSts")
    app.add_sender_element("DoorDrvrReSts")
    app.add_sender_element("DoorPassReSts")
    app.add_sender_element("DoorPassSts")

    app.add_sender_element("IndcnOfParkAssiSts")
    app.add_sender_element("PrkgAssiSts")
    app.add_sender_element("SnsrParkAssiFrnt")
    app.add_sender_element("SnsrParkAssiRe")

    app.add_sender_element("CarTiGlb")


class FastUpdates:
    def __init__(self, app):
        self.app = app
        self.spam_enabled = False
        self.sch = sched.scheduler(time.time, time.sleep, )
        self.sch.enter(0.6, 1, self.timer_action, ( ))
        self.current_pinion_angle = 0
        self.pinion_sweep_direction = 0.3

        self.infoBindVar = tkinter.StringVar()
        self.infoBindVar.set("off")

        bKeyReadSts = tkinter.ttk.Button(app.master, text="Enable spam", command=self.toggle)
        eTurnIndicStatus = tkinter.ttk.Label(app.master, textvariable=self.infoBindVar)
        app.add_external_button_row(bKeyReadSts, eTurnIndicStatus)

        t = threading.Thread(target=self.sch.run)
        t.daemon = True
        t.start()

    def toggle(self):
        self.spam_enabled = not self.spam_enabled
        self.infoBindVar.set("SPAM ON" if self.spam_enabled else "Spam off")

    def timer_action(self):
        if self.spam_enabled:
            if self.current_pinion_angle > 14.5:
                self.pinion_sweep_direction = -0.1
            if self.current_pinion_angle < -14.5:
                self.pinion_sweep_direction = 0.1
            self.current_pinion_angle += self.pinion_sweep_direction
            value = {"PinionSteerAg1": self.current_pinion_angle, "PinionSteerAg1Qf": 3}
            self.app.external_send("/i/PinionSteerAg1", value, "PinionSteerAg1Rec")
            self.app.external_send("/i/CarTiGlb", 0.0, "double")

        self.sch.enter(0.005, 1, self.timer_action, ( ))


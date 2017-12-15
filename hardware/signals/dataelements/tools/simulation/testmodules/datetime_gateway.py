# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
Takes the local time of the host computer and sends it to the IHU using TiAndDateIndcn signal.
"""

import sched
import threading
import time
import tkinter.ttk
import tkinter
import datetime


def init(app):
    app.add_sender_element("TiAndDateIndcn")
    app.add_sender_element("SetTiAndDate")
    x = DateTimeGateway(app)

class Mode:
    Off = 0
    SetTime = 1
    TiIndcn = 2
    Both = 3

    modestrs = {Off: "Off", SetTime: "SetTime", TiIndcn: "TiIndcn", Both: "SetTi and TiIndcn"}


class DateTimeGateway:
    def __init__(self, app):
        self.app = app
        self.mode = Mode.SetTime

        self.infoBindVar = tkinter.StringVar()
        self.infoBindVar.set(Mode.modestrs[self.mode])
        toggleButton = tkinter.ttk.Button(app.master, text="Toggle datetime gateway", command=self.toggle)
        toggleStatusLabel = tkinter.ttk.Label(app.master, textvariable=self.infoBindVar)
        app.add_external_button_row(toggleButton, toggleStatusLabel)

        self.sch = sched.scheduler(time.time, time.sleep, )
        self.sch.enter(1, 1, self.timer_action, ( ))

        thread = threading.Thread(target=self.sch.run)
        thread.daemon = True
        thread.start()

    def toggle(self):
        self.mode = (self.mode + 1) % 4
        self.infoBindVar.set(Mode.modestrs[self.mode])

    def send_datetime(self):
        now = datetime.datetime.utcnow()
        now.utcoffset()

        ti_data = {
            "Yr1": (now.year % 2000),
            "Mth1": now.month,
            "Day": now.day,
            "Hr1": now.hour,
            "Mins1": now.minute,
            "Sec1": now.second,
            "DataValid": 1
        }
        set_ti_data = {
            "DateOrTi_": 3,
            "Year": (now.year % 2000),
            "Month": now.month,
            "Day": now.day,
            "Hour": now.hour,
            "Minute": now.minute,
            "Second": now.second
        }

        if self.mode in [Mode.TiIndcn, Mode.Both]:
            self.app.external_send("/i/TiAndDateIndcn", ti_data, "DateTi30")
        if self.mode in [Mode.SetTime, Mode.Both]:
            self.app.external_send("/o/SetTiAndDate", set_ti_data, "DateTi1ForSet")
        else:
            set_ti_data["DateOrTi_"] = 0
            self.app.external_send("/o/SetTiAndDate", set_ti_data, "DateTi1ForSet")

    def timer_action(self):
        self.send_datetime()
        self.sch.enter(1, 1, self.timer_action, ( ))

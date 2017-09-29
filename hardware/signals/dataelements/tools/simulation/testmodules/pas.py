import sched
import threading
import time
import tkinter
import tkinter.ttk
from .util import toggle

def init(app):
    _ = PasActivationAutoReply(app)
    app.add_sender_element("VehModMngtGlbSafe1")

    # PAS
    app.add_sender_element("IndcnOfParkAssiSts")
    app.add_sender_element("PrkgAssiFailr")
    app.add_sender_element("PrkgAssiSts")
    app.add_sender_element("SnsrParkAssiFrnt")
    app.add_sender_element("SnsrParkAssiRe")
    app.add_sender_element("PrkgAssiManvActvSts")
    app.add_sender_element("VehSpdLgtSafe")
    app.add_sender_element("VehMtnStSafe")
    app.add_sender_element("DrvrDesDir")
    app.add_sender_element("PrkgCamSysAvlSts")
    app.add_sender_element("DoorDrvrSts")
    app.add_sender_element("DoorDrvrReSts")
    app.add_sender_element("DoorPassSts")
    app.add_sender_element("DoorPassReSts")
    app.add_sender_element("MirrFoldStsAtPass")
    app.add_sender_element("MirrFoldStsAtDrvr")
    app.add_sender_element("TrSts")
    app.add_sender_element("TrlrPrsntSts1WdSts")

    #CTA
    app.add_sender_element("RsdsSysStsLe")
    app.add_sender_element("RsdsSysStsRi")
    app.add_sender_element("CtraIndcnLe")
    app.add_sender_element("CtraIndcnRi")
    app.add_sender_element("CtraOn1")


class PasActivationAutoReply():
    def __init__(self, app):
        self.app = app
        self.toggle = toggle.ToggleButton(app, "PAS Activation auto reply")
        app.register_message_handler("ActvnOfPrkgAssi", self.on_actvn_message)

    def on_actvn_message(self, msg_data):
        if not self.toggle.enabled:
            return
        on = msg_data['value']
        self.app.external_send("/i/IndcnOfParkAssiSts", on, "OnOff1")

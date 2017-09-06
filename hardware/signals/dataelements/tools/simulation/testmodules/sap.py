import ttk
import Tkinter
import random
import sched
import time
import threading


def init(app):
    sap = SAPSimulator(app)

class P3:
    SAPNotActive=0
    SAPPassiveScan=1
    OverspeedSlowDown=2
    ScanLeft=3
    ScanRight=4
    SAPPSlotFoundLeftGoForward=5
    SAPPSlotFoundRightGoForward=6
    PPASlotFoundLeftGoForward=7
    PPASlotFoundRightGoForward=8
    SAPStopToParkLeft=9
    SAPStopToParkRight=10
    PPAStopToParkLeft=11
    PPAStopToParkRight=12
    SAPPStopAndReverseGearLeft=13
    SAPPStopAndReverseGearRight=14
    PPAStopAndReverseGearLeft=15
    PPAStopAndReverseGearRight=16
    SAPPLookAroundLeft=17
    SAPPLookAroundRight=18
    PPALookAroundLeft=19
    PPALookAroundRight=20
    SAPPFirstMoveLeft=21
    SAPPFirstMoveRight=22
    PPAFirstMoveLeft=23
    PPAFirstMoveRight=24
    SAPPChangeToForwardLeft=25
    SAPPChangeToForwardRight=26
    PPAChangeToForwardLeft=27
    PPAChangeToForwardRight=28
    SAPPMoveForwardLeft=29
    SAPPMoveForwardRight=30
    PPAMoveForwardLeft=31
    PPAMoveForwardRight=32
    SAPPChangeToReverseLeft=33
    SAPPChangeToReverseRight=34
    PPAChangeToReverseLeft=35
    PPAChangeToReverseRight=36
    SAPPMoveRearwardLeft=37
    SAPPMoveRearwardRight=38
    PPAMoveRearwardLeft=39
    PPAMoveRearwardRight=40
    Finished=41
    ParkingFail=42
    Off=43
    TemporaryNotAvailable=44
    CancelHighSpeed=45
    CancelSlipperyRoad=46
    CancelTrailerConnect=47
    CancelEngineStall=48
    CancelSteringIntervention=49
    CancelDriverDeactivation=50
    CancelManyAttempts=51
    CancelWrongGear=52
    Resume=53
    SelectParkOutSide=54
    POAScannig=55
    POAPathNotFound=56
    POAChanageToForwardGearLeft=57
    POAChanageToForwardGearRight=58
    POAChanageToReverseGearLeft=59
    POAChanageToReverseGearRight=60
    POAStopAndChangeToForwardGearLeft=61
    POAStopAndChangeToForwardGearRight=62
    POAStopAndChangeToReverseGearLeft=63
    POAStopAndChangeToReverseGearRight=64
    POARearwardMoveLeft=65
    POARearwardMoveRight=66
    POAForwardMoveLeft=67
    POAForwardMoveRight=68
    POAFinishedGoLeft=69
    POAFinishedGoRight=70
    POALookAroundLeft=71
    POALookAroundRight=72
    POAFailr=73
    EmgyBrkDsbld=74
    EmgyBrkActv=75
    EmgyBrkUsrActn=76

deactivation_with_lingering = {
    P3.Finished,
    P3.ParkingFail,
    P3.TemporaryNotAvailable,
    P3.CancelHighSpeed,
    P3.CancelSlipperyRoad,
    P3.CancelTrailerConnect,
    P3.CancelManyAttempts,
    P3.POAFinishedGoLeft,
    P3.POAFinishedGoRight,
    P3.POAFailr,
    P3.POAPathNotFound
}
deactivation_without_lingering= {
    P3.Off,
    P3.SAPNotActive
}

resume_values = {
    P3.CancelEngineStall,
    P3.CancelWrongGear,
    P3.CancelSteringIntervention,
    P3.EmgyBrkActv,
    P3.EmgyBrkDsbld,
    P3.CancelDriverDeactivation
}

instruction_values = {
    P3.OverspeedSlowDown,
    P3.ScanLeft,
    P3.ScanRight,
    P3.SAPPSlotFoundLeftGoForward,
    P3.SAPPSlotFoundRightGoForward,
    P3.PPASlotFoundLeftGoForward,
    P3.PPASlotFoundRightGoForward,
    P3.SAPStopToParkLeft,
    P3.SAPStopToParkRight,
    P3.PPAStopToParkLeft,
    P3.PPAStopToParkRight,
    P3.SAPPLookAroundLeft,
    P3.SAPPLookAroundRight,
    P3.PPALookAroundLeft,
    P3.PPALookAroundRight,
    P3.POALookAroundLeft,
    P3.POALookAroundRight,
    P3.ParkingFail,
    P3.Finished,
    P3.TemporaryNotAvailable,
    P3.Resume,
    P3.SelectParkOutSide,
    P3.POAPathNotFound
}
userinteraction_values = {
    P3.SAPPStopAndReverseGearLeft,
    P3.SAPPStopAndReverseGearRight,
    P3.PPAStopAndReverseGearLeft,
    P3.PPAStopAndReverseGearRight,
    P3.POAChanageToForwardGearLeft,
    P3.POAChanageToForwardGearRight,
    P3.POAChanageToReverseGearLeft,
    P3.POAChanageToReverseGearRight
}

manouver_values = {
    P3.SAPPFirstMoveLeft,
    P3.SAPPFirstMoveRight,
    P3.PPAFirstMoveLeft,
    P3.PPAFirstMoveRight,
    P3.SAPPMoveForwardLeft,
    P3.SAPPMoveForwardRight,
    P3.PPAMoveForwardLeft,
    P3.PPAMoveForwardRight,
    P3.SAPPMoveRearwardLeft,
    P3.SAPPMoveRearwardLeft,
    P3.PPAMoveRearwardLeft,
    P3.PPAMoveRearwardRight,
    P3.POARearwardMoveLeft,
    P3.POARearwardMoveRight,
    P3.POAForwardMoveLeft,
    P3.POAForwardMoveRight
}


class PrkgTypVld1:
    NoPrkgDetd=0
    ParaPrkgDetd=1
    PerpPrkgDetd=2
    ParaAndPerpPrkgDetd=3

class ActvnOfPrkgAut:
    Off = 0
    PrkgTypAutSeln = 1
    PrkgTypPara = 2
    PrkgTypPerp = 3
    PrkgManvCncl = 4
    ParkOutManv = 5
    Resd1 = 6
    Resd2 = 7

class IndOfPrkgAutSts:
    Off = 0
    PrkgTypAutSeln = 1
    PrkgTypPara = 2
    PrkgTypPerp = 3
    PrkgOutManv = 4


class SAPSimulator:
    def __init__(self, app):
        self.app = app
        app.add_sender_element("VehModMngtGlbSafe1")
        app.add_sender_element("TrlrPrsntSts1WdSts")
        app.add_sender_element("IndcnOfPrkgAutSts")
        app.add_sender_element("PrkgTypVld")
        app.add_sender_element("PrkgAssiManvProgs")
        app.add_sender_element("PrkgAutSts")
        app.add_sender_element("ActvnOfPrkgAut") #output

        app.register_message_handler("ActvnOfPrkgAut", self.handleActvn)
        app.register_message_handler("PrkgOutCfm", self.handleCfm)
        app.set_filter("VehModMng TrlrPrsnt IndcnOf PrkgTypv Prkgassimanv prkgautsts ActvnOfPrkgAut PrkgOutCfm")

        self.autoReplyEnabled = False
        self.infoBindVar = Tkinter.StringVar()
        self.infoBindVar.set("off")
        bEnableAutoReply = ttk.Button(app.master, text="Enable auto reply", command=self.toggle)
        lEnableAutoReplyStatus = ttk.Label(app.master, textvariable=self.infoBindVar)
        app.add_external_button_row(bEnableAutoReply, lEnableAutoReplyStatus)

        responseTimeLabel = ttk.Label(app.master, text="Response time (ms)")
        responseTimeInfoStruct = ttk.Frame(app.master)
        self.responseTimeBindVar = Tkinter.IntVar(app.master, 1000)
        self.responseTimescale = ttk.Scale(responseTimeInfoStruct, from_=0, to=5000, orient=Tkinter.HORIZONTAL, variable=self.responseTimeBindVar)
        responseTimeValueLabel= ttk.Label(responseTimeInfoStruct, textvariable=self.responseTimeBindVar)
        self.responseTimescale.pack(side=Tkinter.LEFT)
        responseTimeValueLabel.pack(side=Tkinter.LEFT)
        app.add_external_button_row(responseTimeLabel, responseTimeInfoStruct)

        self.cancelDueToDriverDeactivation = False

        structframe = ttk.Frame(app.master)

        bDeactivWithLinger = ttk.Button(structframe, text="Deact + Linger", command=lambda: self.sendPrkgAutSts(random.choice(tuple(deactivation_with_lingering))))
        bDeactivWithLinger.pack(side=Tkinter.LEFT)

        bDeactivWithoutLinger = ttk.Button(structframe, text="Deact", command=lambda: self.sendPrkgAutSts(random.choice(tuple(deactivation_without_lingering))))
        bDeactivWithoutLinger.pack(side=Tkinter.LEFT)

        bUserInteraction = ttk.Button(structframe, text="Userinteraction", command=lambda: self.sendPrkgAutSts(random.choice(tuple(userinteraction_values))))
        bUserInteraction.pack(side=Tkinter.LEFT)

        instruction_not_deactivation = instruction_values - deactivation_with_lingering
        bInstruction = ttk.Button(structframe, text="Instruction", command=lambda: self.sendPrkgAutSts(random.choice(tuple(instruction_not_deactivation))))
        bInstruction.pack(side=Tkinter.LEFT)

        bResumable = ttk.Button(structframe, text="Resumable", command=lambda: self.sendPrkgAutSts(random.choice(tuple(resume_values))))
        bResumable.pack(side=Tkinter.LEFT)

        bManouver = ttk.Button(structframe, text="Manouver", command=lambda: self.sendPrkgAutSts(random.choice(tuple(manouver_values))))
        bManouver.pack(side=Tkinter.LEFT)

        app.add_external_button_row(None, structframe)


        self.autoStatusBindVar = Tkinter.StringVar()
        self.autoStatusBindVar.set("off")
        autoStatusLabel = ttk.Label(app.master, textvariable=self.autoStatusBindVar)
        app.add_external_button_row(None, autoStatusLabel)

    def toggle(self):
        self.autoReplyEnabled = not self.autoReplyEnabled
        self.infoBindVar.set("on " if self.autoReplyEnabled else "off")

    def handleActvn(self, msg_data):
        if not self.autoReplyEnabled:
            return

        actvn_value = msg_data['value']

        time_sec = self.responseTimeBindVar.get()/1000.0
        print("handleactvn: %r" % time_sec)

        t = threading.Timer(time_sec, self.handleActvnDelayed, (actvn_value, ))
        t.start()

    def handleActvnDelayed(self, actvn_value):
        print("handleActvnDelayed: %r" % actvn_value)

        if actvn_value == ActvnOfPrkgAut.PrkgTypAutSeln:
            self.app.external_send('/i/IndcnOfPrkgAutSts', IndOfPrkgAutSts.PrkgTypAutSeln, 'IndOfPrkgAutSts')
            self.app.external_send('/i/PrkgTypVld', PrkgTypVld1.ParaAndPerpPrkgDetd, 'PrkgTypVld1')
            if self.cancelDueToDriverDeactivation:
                self.cancelDueToDriverDeactivation = False
                self.infoBindVar.set("on")
                self.sendPrkgAutSts(P3.CancelDriverDeactivation)
            else:
                self.parking_sequence()
        elif actvn_value == ActvnOfPrkgAut.PrkgTypPara:
            self.app.external_send('/i/IndcnOfPrkgAutSts', IndOfPrkgAutSts.PrkgTypPara, 'IndOfPrkgAutSts')
            self.app.external_send('/i/PrkgTypVld', PrkgTypVld1.ParaAndPerpPrkgDetd, 'PrkgTypVld1')
            self.parking_sequence()
        elif actvn_value == ActvnOfPrkgAut.PrkgTypPerp:
            self.app.external_send('/i/IndcnOfPrkgAutSts', IndOfPrkgAutSts.PrkgTypPerp, 'IndOfPrkgAutSts')
            self.app.external_send('/i/PrkgTypVld', PrkgTypVld1.ParaAndPerpPrkgDetd, 'PrkgTypVld1')
            self.parking_sequence()
        elif actvn_value == ActvnOfPrkgAut.ParkOutManv:
            self.app.external_send('/i/IndcnOfPrkgAutSts', IndOfPrkgAutSts.PrkgOutManv, 'IndOfPrkgAutSts')
            self.app.external_send('/i/PrkgTypVld', PrkgTypVld1.ParaAndPerpPrkgDetd, 'PrkgTypVld1')
            self.parking_sequence()
        else:
            if actvn_value ==ActvnOfPrkgAut.PrkgManvCncl:
                if self.cancelDueToDriverDeactivation:
                    # Restart
                    self.app.external_send('/i/IndcnOfPrkgAutSts', IndOfPrkgAutSts.PrkgTypAutSeln, 'IndOfPrkgAutSts')
                    self.app.external_send('/i/PrkgTypVld', PrkgTypVld1.ParaAndPerpPrkgDetd, 'PrkgTypVld1')
                    self.parking_sequence()
                else:
                    self.cancelDueToDriverDeactivation = True
                    self.infoBindVar.set("on - usercancel")
                    self.sendPrkgAutSts(P3.CancelDriverDeactivation)
            self.app.external_send('/i/IndcnOfPrkgAutSts', IndOfPrkgAutSts.Off, 'IndOfPrkgAutSts')
            self.app.external_send('/i/PrkgTypVld', PrkgTypVld1.ParaAndPerpPrkgDetd, 'PrkgTypVld1')


    def handleCfm(self, msg_data):
        if not self.autoReplyEnabled:
            return

        confirmation_on = (msg_data['value'] == 1)
        t = threading.Timer(self.responseTimeBindVar.get()/1000.0, self.handleCfm_delayed, (confirmation_on, ))
        t.start()

    def handleCfm_delayed(self, confirmation_on):
        if confirmation_on:
            self.parking_sequence()


    def parking_sequence(self):
        prkgAutSts=random.choice(tuple(instruction_values-deactivation_without_lingering))
        print("Sending prkgautsts: %d" % prkgAutSts)
        self.sendPrkgAutSts(prkgAutSts)

    def sendPrkgAutSts(self, value):
        self.autoStatusBindVar.set("Sent PrkgAutSts=%d" % value)
        self.app.external_send('/i/PrkgAutSts', value, 'PrkgAutSts3')
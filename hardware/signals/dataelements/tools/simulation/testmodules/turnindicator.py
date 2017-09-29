import sched
import time
import tkinter.ttk
import tkinter
import threading


def init(app):
    ignored = TurnIndicator(app)

MODE_OFF = 0
MODE_LEFT = 1
MODE_RIGHT = 2
MODE_HAZARD = 3

def get_mode_str(mode):
    return ("Off" if mode == MODE_OFF else
            "Left" if mode == MODE_LEFT else
            "Right" if mode == MODE_RIGHT else
            "Hazard")

class TurnIndicator:
    def __init__(self, app):
        self.app = app
        self.mode = 0
        self.previous_on = False
        self.sch = sched.scheduler(time.time, time.sleep, )
        self.sch.enter(0.6, 1, self.timer_action, ( ))

        t = threading.Thread(target=self.sch.run)
        t.daemon = True
        t.start()

        self.infoBindVar = tkinter.StringVar()
        self.infoBindVar.set("off")

        bKeyReadSts = tkinter.ttk.Button(app.master, text="Toggle turn indicator", command=self.toggle)
        eTurnIndicStatus = tkinter.ttk.Label(app.master, textvariable=self.infoBindVar)
        app.add_external_button_row(bKeyReadSts, eTurnIndicStatus)

        self.intervalBindVar = tkinter.StringVar()
        self.intervalBindVar.set("0.5")
        lTurnIndicatorFrequency= tkinter.ttk.Label(app.master, text="Turn indicator interval")
        eTurnIndicatorFrequency = tkinter.ttk.Entry(app.master, textvariable=self.intervalBindVar)
        app.add_external_button_row(lTurnIndicatorFrequency, eTurnIndicatorFrequency)

        app.add_sender_element("IndcrDisp1WdSts")
        app.add_sender_element("FltIndcrTurnLeFrnt")
        app.add_sender_element("FltIndcrTurnLeRe")
        app.add_sender_element("FltIndcrTurnRiFrnt")
        app.add_sender_element("FltIndcrTurnRiRe")

    def toggle(self):
        self.mode = (self.mode + 1) % 4
        if self.mode == MODE_OFF:
            self.app.external_send("/i/IndcrDisp1WdSts", MODE_OFF, "IndcrSts1")

        self.infoBindVar.set(get_mode_str(self.mode))


    def timer_action(self):
        if self.previous_on:
            value = 0
        else:
            value = self.mode

        if self.mode == MODE_OFF:
            pass
        else:
            self.infoBindVar.set(get_mode_str(self.mode) + " " + str(not self.previous_on))
            self.app.external_send("/i/IndcrDisp1WdSts", value, "IndcrSts1")
            self.previous_on = not self.previous_on

        interval = 0.6
        try:
            interval = float(self.intervalBindVar.get())
        except:
            pass
        self.sch.enter(interval, 1, self.timer_action, ( ))
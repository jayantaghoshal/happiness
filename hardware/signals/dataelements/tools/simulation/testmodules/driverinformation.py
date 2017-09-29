import sched
import threading
import time
import tkinter
import tkinter.ttk
from tkinter import *

def init(app):
    def CreateLabel(str):
      label= tkinter.ttk.Label(app.master, text= str)
      app.add_external_button_row(label, None)
      return
    def Grouper(str=""):
      if str:
        str = ' ' + str + ' '
        formatlen = 80 - len(str)
      else: formatlen = 80
      strGrouper = '*'* (formatlen/2) + str + '*'* (formatlen/2)
      CreateLabel(strGrouper)
      return
    Grouper("SIMULATE CAR DRIVE")
    drive = DriveAround(app)
    Grouper()
    Grouper("SIMULATE CEM")
    cem = CEM(app)
    Grouper()
    Grouper("SIMULATE A GPS DRIVE")
    gps = Gps(app)
    Grouper()
    Grouper("VEHICLEMODE")
    app.add_sender_element("VehModMngtGlbSafe1")
    Grouper()
    Grouper("CLOCK")
    app.add_sender_element("TiAndDateIndcn")
    app.add_sender_element("PosnFromSatlt")
    Grouper()
    Grouper("UNIT SETTINGS")
    app.add_sender_element("IndcnUnit")
    app.add_sender_element("ProfPenSts1")
    Grouper()
    Grouper("TRIP STATISTICS")
    app.add_sender_element("CarTiGlb")
    app.add_sender_element("NewTripCdn")
    app.add_sender_element("DstTrvldHiResl")
    Grouper("FUEL CONSUMPTION")
    app.add_sender_element("EngFuCnsFild")
    app.add_sender_element("EngFuCns")
    app.add_sender_element("FuHeatrFuCns1")
    app.add_sender_element("FuHeatrFuCnsDurgCyc1")
    Grouper()
    Grouper("ENERGY CONSUMPTION")
    app.add_sender_element("DispBattEgyIn")
    app.add_sender_element("DispBattEgyOut")
    Grouper()
    Grouper()
    Grouper("POWERFLOW")
    app.add_sender_element("DispOfPrpsnMod")
    Grouper()
class UsageMode:
    Abandoned   = 0
    InActive    = 1
    Convenience = 2
    Active      = 11
    Driving     = 13

vehicle_mode = {
    "UsgModSts": 0,
    "EgyLvlElecMai": 0,
    "CarModSubtypWdCarModSubtyp": 0,
    "EgyLvlElecSubtyp": 0,
    "FltEgyCnsWdSts": 0,
    "CarModSts1_": 0,
    "PwrLvlElecSubtyp": 0,
    "Cntr": 0,
    "PwrLvlElecMai": 0,
    "Chks": 0
}

class DriveAround:
    def __init__(self, app):
        self.app = app
        self.drive_enabled = False
        self.sch = sched.scheduler(time.time, time.sleep, )
        self.sch.enter(0.6, 1, self.timer_action, ( ))

        self.current_distance_travelled = 0
        self.current_fuel_consumption   = 0
        self.current_energy_consumption   = 0

        self.info_bind_var = tkinter.StringVar()
        self.info_bind_var.set("Not driving")
        self.hybrid_enabled = tkinter.BooleanVar()
        self.hybrid_enabled.set(False)
        self.energy_consumption_entry = {}

        hybrid_enable_button = tkinter.ttk.Checkbutton(app.master, text="Hybrid", variable = self.hybrid_enabled , command=self.updatehybrid)
        app.add_external_button_row(hybrid_enable_button, None)
        drive_toggle_button = tkinter.ttk.Button(app.master, text="Go for a drive", command=self.toggle)
        drive_label = tkinter.ttk.Label(app.master, textvariable=self.info_bind_var)
        app.add_external_button_row(drive_toggle_button, drive_label)

        self.add_speed_entry()
        self.add_fuel_consumption_entry()
        self.add_energy_consumption_entry()

        thread = threading.Thread(target=self.sch.run)
        thread.daemon = True
        thread.start()

    def add_speed_entry(self):
        self.speed_bind_var = tkinter.StringVar()
        self.speed_bind_var.set("100")
        speed_label = tkinter.ttk.Label(self.app.master, text="Speed (kph)")
        speed_entry = tkinter.ttk.Entry(self.app.master, textvariable=self.speed_bind_var)
        self.app.add_external_button_row(speed_label, speed_entry)

    def add_fuel_consumption_entry(self):
        self.fuel_consumption_bind_var = tkinter.StringVar()
        self.fuel_consumption_bind_var.set("5.0")
        fuel_consumption_label = tkinter.ttk.Label(self.app.master, text="litres/100km")
        fuel_consumption_entry = tkinter.ttk.Entry(self.app.master, textvariable=self.fuel_consumption_bind_var)
        self.app.add_external_button_row(fuel_consumption_label, fuel_consumption_entry)

    def add_energy_consumption_entry(self):
        self.energy_consumption_bind_var = tkinter.StringVar()
        self.energy_consumption_bind_var.set("5.0")
        energy_consumption_label = tkinter.ttk.Label(self.app.master, text="kWh/km")
        self.energy_consumption_entry = tkinter.ttk.Entry(self.app.master, textvariable=self.energy_consumption_bind_var , state='disabled')
        self.app.add_external_button_row(energy_consumption_label, self.energy_consumption_entry)

    def updatehybrid(self):
        if self.hybrid_enabled:
           self.energy_consumption_entry.config(state='normal')
        else:
           self.energy_consumption_entry.config(state='disabled')
        self.hybrid_enabled = not self.hybrid_enabled

    def toggle(self):
        self.drive_enabled = not self.drive_enabled
        self.info_bind_var.set("Driving" if self.drive_enabled else "Not driving")

        vehicle_mode["UsgModSts"] = UsageMode.Driving if self.drive_enabled else UsageMode.Convenience
        self.app.external_send("/i/VehModMngtGlbSafe1", vehicle_mode, "VehModMngtGlbSafe1")

    def update_distance(self):
        speed = 10
        try:
            speed_kph = int(self.speed_bind_var.get())
            speed = int((speed_kph * 1000) / 3600)
        except:
            pass
        previous_distance = self.current_distance_travelled
        self.current_distance_travelled = self.current_distance_travelled + speed
        self.app.external_send("/i/DstTrvldHiResl", self.current_distance_travelled % 256, "uint8_t")
        return self.current_distance_travelled - previous_distance

    def update_fuel_consumption(self, distance_delta):
        fuel_consumption = 1000
        try:
            fuel_consumption_ul = float(self.fuel_consumption_bind_var.get()) * 1000000
            fuel_consumption = int(fuel_consumption_ul / 100000) * distance_delta
        except:
            pass
        self.current_fuel_consumption = self.current_fuel_consumption + fuel_consumption
        self.app.external_send("/i/EngFuCnsFild", self.current_fuel_consumption % 51000, "double")

    def update_energy_consumption(self, distance_delta):
        energy_consumption = 1000
        try:
            energy_consumption_Wh = float(self.energy_consumption_bind_var.get()) * 1000000
            energy_consumption = int(energy_consumption_Wh) * distance_delta
        except:
            pass
        self.current_energy_consumption = self.current_energy_consumption + energy_consumption
        self.app.external_send("/i/DispBattEgyOut", self.current_energy_consumption % 127.5, "double")

    def timer_action(self):
        if self.drive_enabled:
            delta = self.update_distance()
            self.update_fuel_consumption(distance_delta=delta)
            if self.hybrid_enabled:
                self.update_energy_consumption(distance_delta=delta)

        self.sch.enter(1, 1, self.timer_action, ( ))

date_ti_30 = {
    "Mth1": 1,
    "Mins1": 0,
    "Hr1": 0,
    "Sec1": 0,
    "DataValid": 0,
    "Yr1": 0,
    "Day": 1
}

date_ti_1_for_set = {
    "Hour": 0,
    "DateOrTi_": 0,
    "Month": 1,
    "Second": 0,
    "Year": 0,
    "Day": 1,
    "Minute": 0
}

class CEM:
    def __init__(self, app):
        self.app = app
        self.cem_enabled = False
        app.register_message_handler("SetTiAndDate", self.set_time_reply)
        print("CEM callback: ")
        print((self.set_time_reply))

        self.info_bind_var = tkinter.StringVar()
        self.info_bind_var.set("Not active")

        cem_toggle_button = tkinter.ttk.Button(app.master, text="Toggle CEM", command=self.toggle)
        cem_label = tkinter.ttk.Label(app.master, textvariable=self.info_bind_var)
        self.app.add_external_button_row(cem_toggle_button, cem_label)

    def set_time_reply(self, client, msg_raw, msg_data):
        date_ti_30["Yr1"] = msg_data["Year"]
        date_ti_30["Mth1"] = msg_data["Month"]
        date_ti_30["Day"] = msg_data["Day"]
        date_ti_30["Hr1"] = msg_data["Hour"]
        date_ti_30["Mins1"] = msg_data["Minute"]
        date_ti_30["Sec1"] = msg_data["Second"]
        date_ti_30["DataValid"] = 1

        print("<=======set_time_reply=====>")
        self.app.external_send("/i/TiAndDateIndcn", date_ti_30, "DateTi30")

    def toggle(self):
        self.cem_enabled = not self.cem_enabled
        self.info_bind_var.set("Active" if self.cem_enabled else "Not active")

    def active(self):
        return self.cem_enabled == True



posn_from_satlt  = {
    "PosnLat": 0,
    "PosnLgt": 0,
    "PosnAlti": 0,
    "PosnSpd": 0,
    "PosnVHozl": 0,
    "PosnVVert": 0,
    "PosnDir": 0,
    "TiForYr": 0,
    "TiForMth": 6,
    "TiForDay": 1,
    "TiForHr": 0,
    "TiForMins": 0,
    "TiForSec": 0,
    "SatltSysNo1InUse": 0,
    "SatltSysNo2InUse": 0,
    "SatltSysNo3InUse": 0,
    "SatltSysNo4InUse": 0,
    "SatltSysNo5InUse": 0,
    "SatltSysNo6InUse": 0,
    "SatltPosnStsPrm1": 0,
    "SatltPosnStsPrm2": 0,
    "SatltPosnStsPrm3": 0,
    "NoOfSatltForSysNo1": 0,
    "NoOfSatltForSysNo2": 0,
    "NoOfSatltForSysNo3": 0,
    "NoOfSatltForSysNo4": 0,
    "NoOfSatltForSysNo5": 0,
    "NoOfSatltForSysNo6": 0,
    "PrePosnDil": 0,
    "PreHozlDil": 0,
    "PreVertDil": 0,
    "PreTiDil": 0,
}


class Gps:
    def __init__(self, app):
        self.app = app
        self.gps_enabled = False
        self.sch = sched.scheduler(time.time, time.sleep, )
        self.sch.enter(0.6, 1, self.timer_action, ( ))

        self.current_seconds = 0
        self.current_minutes = 0

        self.info_bind_var = tkinter.StringVar()
        self.info_bind_var.set("Gps Off")

        gps_toggle_button = tkinter.ttk.Button(app.master, text="Toggle Gps Reception", command=self.dataToggle)
        gps_label = tkinter.ttk.Label(app.master, textvariable=self.info_bind_var)
        app.add_external_button_row(gps_toggle_button, gps_label)

        thread = threading.Thread(target=self.sch.run)
        thread.daemon = True
        thread.start()

    def dataToggle(self):
        self.gps_enabled = not self.gps_enabled
        self.info_bind_var.set("Gps on" if self.gps_enabled else "Gps Off")

        vehicle_mode["UsgModSts"] = UsageMode.Driving if self.gps_enabled else UsageMode.Convenience
        self.app.external_send("/i/VehModMngtGlbSafe1", vehicle_mode, "VehModMngtGlbSafe1")

    def update_gps(self):
        self.current_seconds = self.current_seconds + 1 if ((self.current_seconds + 1) < 60) else 0
        self.current_minutes = self.current_minutes + 1 if (self.current_seconds < 1) else self.current_minutes
        posn_from_satlt["TiForYr"] = 0
        posn_from_satlt["TiForMth"] = 6
        posn_from_satlt["TiForDay"] = 1
        posn_from_satlt["TiForHr"] = 0
        posn_from_satlt["TiForMins"] = self.current_minutes
        posn_from_satlt["TiForSec"] = self.current_seconds
        posn_from_satlt["SatltPosnStsPrm1"] = 2

    def send_gps(self):
        print("<=======gps_input_sent=====>")
        self.app.external_send("/i/PosnFromSatlt", posn_from_satlt, "PosnFromSatlt")

    def timer_action(self):
        self.update_gps()
        if self.gps_enabled:
            self.send_gps()

        self.sch.enter(1, 1, self.timer_action, ( ))


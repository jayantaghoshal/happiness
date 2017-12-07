# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import tkinter
import tkinter.ttk


class ToggleButton:
    def __init__(self, app, text):
        self.app = app
        self.infoBindVar = tkinter.StringVar()
        self.infoBindVar.set("Off")

        toggle_button = tkinter.ttk.Button(app.master, text=text, command=self.toggle)
        info_label = tkinter.ttk.Label(app.master, textvariable=self.infoBindVar)
        app.add_external_button_row(toggle_button, info_label)
        self.enabled = False

    def toggle(self):
        self.enabled = not self.enabled
        self.infoBindVar.set("On" if self.enabled else "Off")

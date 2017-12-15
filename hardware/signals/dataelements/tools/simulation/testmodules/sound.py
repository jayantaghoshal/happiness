# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

def init(app):
    app.add_sender_element("AudMsgReq")


    app.add_sender_element("HoodSts")
    app.add_sender_element("VehModMngtGlbSafe1")
    app.add_sender_element("VehSpdLgtSafe")


    app.add_sender_element("BltRmnSound1")
    app.add_sender_element("BltRmnSound2")
    app.add_sender_element("BltRmnSound3")
    app.add_sender_element("BltRmnSound4")
# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

def init(app):
	 #Collision warnings
    app.add_sender_element("CllsnWarnReq")
    app.add_sender_element("CllsnFwdWarnReq")
    app.add_sender_element("EpbLampReq")
    app.add_sender_element("VehSpdLgtSafe")
    app.add_sender_element("BrkRelsWarnReq")

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

def init(app):
    app.add_sender_element("iTPMSTirePMSts")
    app.add_sender_element("iTPMSCalSts")
    app.add_sender_element("iTPMSCalPsbl")
    app.add_sender_element("VehModMngtGlbSafe1")

    app.set_filter("iTPMSCalReq iTPMSTirePMSts iTPMSCalSts iTPMSCalPsbl VehModMngtGlbSafe1 CarTiGlb")

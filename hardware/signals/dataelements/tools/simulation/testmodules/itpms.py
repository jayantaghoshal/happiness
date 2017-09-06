def init(app):
    app.add_sender_element("iTPMSTirePMSts")
    app.add_sender_element("iTPMSCalSts")
    app.add_sender_element("iTPMSCalPsbl")
    app.add_sender_element("VehModMngtGlbSafe1")

    app.set_filter("iTPMSCalReq iTPMSTirePMSts iTPMSCalSts iTPMSCalPsbl VehModMngtGlbSafe1 CarTiGlb")

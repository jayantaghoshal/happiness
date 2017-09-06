import ttk

def init(app):
    profile = ProfileApp(app)

class ProfileApp:
    def __init__(self, app):
        self.app = app

        # Enabled Profile settings
        bProfAct = ttk.Button(app.master, text="Send 5 enabled profiles", command=self.Send_ProfAct)
        app.add_external_button_row(bProfAct, None)

        # KeyReadResponse
        bKeyReadSts = ttk.Button(app.master, text="Key Read Sts", command=self.Send_KeyReadResponse)
        app.add_external_button_row(bKeyReadSts, None)

        app.register_message_handler("ProfChg", self.ProfileResponse)


        app.add_sender_element("ProfPenSts1")
        app.add_sender_element("NrOfKeyAvl")
        app.add_sender_element("KeyReadStsToProfCtrl")

    def ProfileResponse(self, msg_data):
        profId = msg_data['value']
        print("Prof req value ", profId)
        if profId > 0: #Simulate back a profile change if incoming request is more than 0(ProfUnknown) as that is used as a "invalid" value
              self.app.external_send('/i/ProfPenSts1', profId, "IdPen")

    def Send_ProfAct(self):
        value = {
            "Prof1": True,
            "Prof2": True,
            "Prof3": True,
            "Prof4": True,
            "Prof5": True,
            "Prof6": False,
            "Prof7": False,
            "Prof8": False,
            "Prof9": False,
            "Prof10": False,
            "Prof11": False,
            "Prof12": False}

        self.app.external_send("/i/ProfAct", value, "ProfAct1")

    def Send_KeyReadResponse(self):
        value = {
            "KeyId": 1,
            "Boolean": True
        }
        self.app.external_send("/i/KeyReadStsToProfCtrl", value, "KeyReadStsToProfCtrl")


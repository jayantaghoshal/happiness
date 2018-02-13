# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# List of Flexray/LIN signals that are controlled exclusively from VIP
# Reason for rx exclusion is usually too high frequency and signal not of interest for MP
# Reason for tx exclusion is usually signal controlled by VIP component such as power moding
#
# NOTE: Changing this list does not change how VIP behaves
#       If you removing something here, the VIP will still block the signal.
#       This filter is just to avoid accidental misuse that will not work from DataElements and for testing purposes.
#
# Talk to michal.korman@aptiv.com for details about this list
SIGNALS_EXCLUDED_FROM_MP = {
    # Excluded from MpRouter Send to MP
    "BackCntrForMissCom",
    "MstCfgIDBackboneFR",
    "IniValSigCfgIDBackboneFR",

    # Controlled by VIP Dimming Module:
    "ActvnOfSwtIllmnCen",
    "ActvnOfSwtIllmnClima",
    "ActvnOfSwtIllmnDrvMod",
    "ActvnOfSwtIllmnForSeatHeatrRe",
    "IntrBriStsForSeatHeatrRe",
    "TwliBriStsForSeatHeatrRe",

    # Controlled by VIP Actv1 module:
    "ActvnOfSwtIllmnForSeatHeatrRe",

    # Controlled by VIP KeyPanel module:
    "DispAndHomeBtnSts",
    "SwtAtCenSts",

    # Controlled by VIP PwrMod Module:
    "MmedHmiModStd",
    "MmedMaiPwrMod",

    # To be controlled by VIP TVMMgr module (???)
    "MmedTvmPwerMod",
    "MmedTvmPwerMod",
}
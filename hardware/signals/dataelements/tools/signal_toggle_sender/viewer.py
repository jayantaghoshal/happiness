# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import sys
from generated.pyDataElements import FrSignalInterface

def main():
    fr = FrSignalInterface()
    fr.FRNetworkStatus.send(1)
    assert(fr.connected)
    while True:
        print("FR.ClimaTmrStsRqrd=%d, LIN.ActvnOfSwtIllmnCen=???" %
              (fr.ClimaTmrStsRqrd.get()))
        # TODO: fr.ActvnOfSwtIllmnCen not in fdx_client, need to update FDXDescriptionFile.xml generator
        # As workaround, if you have rig you can check if center stack buttons are blinking
        time.sleep(0.5)

if __name__ == "__main__":
    main()
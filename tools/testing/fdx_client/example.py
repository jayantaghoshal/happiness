# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import os
import typing
import logging
from generated import datatypes as DE
from generated import dataelements_carsim
from generated import dataelements_fdx
from generated import dataelements_abc
from Sym import adb_connection
from Sym import carsimconnection



# duplicate from vehiclehalcommon, to avoid dependencies to VTS while developing
def get_dataelements_connection(adb, mode=None):
    # type: (typing.Optional[typing.Any], str) -> dataelements_abc.DataElements
    if mode is None:
        mode = os.environ.get("DATAELEMENTS_MODE", "adb")
    if mode == "adb":
        adbconnection = adb_connection.AdbCarsimConnection(None if adb is None else adb.shell, None if adb is None else adb.forward)
        fr = dataelements_carsim.CarsimSerializer(adbconnection, None)
        adbconnection.connect()
        return fr
    elif mode == "ethernet":
        connection = carsimconnection.CarsimConnection()
        fr = dataelements_carsim.CarsimSerializer(connection, None)
        connection.connect("localhost", 8080)
        return fr
    elif mode == "canoe":
        return dataelements_fdx.FrSignalInterface()
    else:
        raise RuntimeError("Invalid DATAELEMENTS_MODE %r" % mode)



def main():
    logging.basicConfig(level=logging.DEBUG)

    #conn = get_dataelements_connection(None, "adb")
    conn = get_dataelements_connection(None, "ethernet")
    conn = get_dataelements_connection(None, "canoe")
    try:
        run(conn)
    finally:
        if conn:
            conn.close()

def run(conn):
    # type: (dataelements_abc.DataElements) -> None
    time.sleep(2)   #Sleep for Signals to be received

    vehmod = DE.VehModMngtGlbSafe1()
    vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
    vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
    vehmod.Chks = 0
    vehmod.Cntr = 0
    vehmod.CarModSubtypWdCarModSubtyp = 0
    vehmod.EgyLvlElecMai = 0
    vehmod.EgyLvlElecSubtyp = 0
    vehmod.PwrLvlElecMai = 0
    vehmod.PwrLvlElecSubtyp = 0
    vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt
    conn.send_ClimaActv(DE.OnOff1.On)
    conn.send_VehModMngtGlbSafe1(vehmod)

    #while True:
    #    print(conn.get_HmiHvacFanLvlFrnt())
    #    time.sleep(0.3)


    conn.send_AccSts(DE.AccSts1.StopTmp)
    time.sleep(3)
    conn.send_AccSts(DE.AccSts1.Stop)
    time.sleep(3)
    conn.stop_AccSts()

    time.sleep(6)


    time.sleep(1)
    hmiseat = conn.get_HmiSeatClima()
    print(hmiseat)
    print(hmiseat.HmiSeatHeatgForRowFirstLe)



if __name__ == "__main__":
    main()



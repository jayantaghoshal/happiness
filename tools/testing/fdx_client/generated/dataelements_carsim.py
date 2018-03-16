#coding: utf-8

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#AUTO GENERATED FILE, do not modify by hand.
# Generated with args: generate_signal_scaling.py --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml --fdxdescriptionfile=../CANoe/SPA2610/FDXDescriptionFile.xml --out=generated/pyDataElements.py
SWC_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml"
COM_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml"
from . import datatypes
from . import dataelements_abc
from Sym import carsimconnection
import json
import time
import threading
import typing
from enum import Enum
import sys
if sys.version_info[0] == 3:
    from queue import Queue
else:
    from Queue import Queue
import logging
import sys
current_module = sys.modules[__name__]



def _get_if_not_None(x):
    if x is None:
        raise AttributeError("Trying to send signal with empty data")
    return x

def _extract_enum_value(x):
    if isinstance(x, Enum):
        return x.value
    return x







class CarsimSerializer(dataelements_abc.DataElements):
    def __init__(self, connection, dataelement_rec_queue ):
        # type: (carsimconnection.CarsimConnection, typing.Optional[Queue] ) -> None
        self.connection = connection
        self.datamap = {}   # type: typing.Dict[str, typing.Any]
        self.dataelement_rec_queue = dataelement_rec_queue
        self.raw_rec_queue = self.connection.receive_queue
        self.process_thread = threading.Thread(target=self.process_messages)
        self.process_thread.daemon=True
        self.process_thread.start()

    def process_messages(self):
        while True:
            if self.raw_rec_queue.empty():
                time.sleep(0.05)
                continue

            msg = self.raw_rec_queue.get()  # type: carsimconnection.RawCarsimMessage
            try:
                self.handle_message(msg)
            except Exception as e:
                logging.exception("Exception %s in handle_signal %s" % (e, msg))
            self.raw_rec_queue.task_done()

    def handle_message(self, msg):
        # type: (carsimconnection.RawCarsimMessage) -> None
        if msg.name == "ActivateVfc":
            return

        if msg.state != 0:
            self.datamap[msg.name] = "error" #TODO
            return

        parse_func = getattr(self, "_parse_" + msg.name)
        parsed_dataelement = parse_func(msg.data)
        self.datamap[msg.name] = parsed_dataelement

        if self.dataelement_rec_queue is not None:
            # Used by carsim to re-render the UI, not needed when using the get-functions
            self.dataelement_rec_queue.put(carsimconnection.CarsimDataElementValue(msg.name, msg.state,parsed_dataelement))

    def close(self):
        self.connection.close()






    def send_ADataRawSafe(self, data):
        # type: (datatypes.ADataRawSafe1) -> None
        s = toJson_ADataRawSafe1(data)
        name = "ADataRawSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ADataRawSafe(self):
        name = "ADataRawSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ADataRawSafe(self, datastring):
        # type: (str) -> datatypes.ADataRawSafe1
        return fromJson_ADataRawSafe1(datastring)
    def get_ADataRawSafe(self):
        # type: () -> (datatypes.ADataRawSafe1)
        return self.datamap["ADataRawSafe"]


    def send_AccAdprSpdLimActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "AccAdprSpdLimActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AccAdprSpdLimActv(self):
        name = "AccAdprSpdLimActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AccAdprSpdLimActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_AccAdprSpdLimActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["AccAdprSpdLimActv"]


    def send_AccAdprTurnSpdActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "AccAdprTurnSpdActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AccAdprTurnSpdActv(self):
        name = "AccAdprTurnSpdActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AccAdprTurnSpdActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_AccAdprTurnSpdActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["AccAdprTurnSpdActv"]


    def send_AccAutResuWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AccAutResuWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AccAutResuWarnReq(self):
        name = "AccAutResuWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AccAutResuWarnReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_AccAutResuWarnReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["AccAutResuWarnReq"]


    def send_AccSts(self, data):
        # type: (datatypes.AccSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AccSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AccSts(self):
        name = "AccSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AccSts(self, datastring):
        # type: (str) -> datatypes.AccSts1
        return json.loads(datastring)
    def get_AccSts(self):
        # type: () -> datatypes.AccSts1
        return self.datamap["AccSts"]


    def send_ActrPosnFrnReqForOsaAndRecirc(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ActrPosnFrnReqForOsaAndRecirc"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActrPosnFrnReqForOsaAndRecirc(self):
        name = "ActrPosnFrnReqForOsaAndRecirc"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActrPosnFrnReqForOsaAndRecirc(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_ActrPosnFrnReqForOsaAndRecirc(self):
        # type: () -> (float)
        return self.datamap["ActrPosnFrnReqForOsaAndRecirc"]


    def send_ActvNoiseCtrlReq(self, data):
        # type: (datatypes.NormSptPen) -> None
        s = toJson_NormSptPen(data)
        name = "ActvNoiseCtrlReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvNoiseCtrlReq(self):
        name = "ActvNoiseCtrlReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvNoiseCtrlReq(self, datastring):
        # type: (str) -> datatypes.NormSptPen
        return fromJson_NormSptPen(datastring)
    def get_ActvNoiseCtrlReq(self):
        # type: () -> (datatypes.NormSptPen)
        return self.datamap["ActvNoiseCtrlReq"]


    def send_ActvOfHorn(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ActvOfHorn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvOfHorn(self):
        name = "ActvOfHorn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvOfHorn(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ActvOfHorn(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ActvOfHorn"]


    def send_ActvnOfCrsEco(self, data):
        # type: (datatypes.ActvnOfCrsEco) -> None
        s = toJson_ActvnOfCrsEco(data)
        name = "ActvnOfCrsEco"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfCrsEco(self):
        name = "ActvnOfCrsEco"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfCrsEco(self, datastring):
        # type: (str) -> datatypes.ActvnOfCrsEco
        return fromJson_ActvnOfCrsEco(datastring)
    def get_ActvnOfCrsEco(self):
        # type: () -> (datatypes.ActvnOfCrsEco)
        return self.datamap["ActvnOfCrsEco"]


    def send_ActvnOfPrkgAssi(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ActvnOfPrkgAssi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfPrkgAssi(self):
        name = "ActvnOfPrkgAssi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfPrkgAssi(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ActvnOfPrkgAssi(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ActvnOfPrkgAssi"]


    def send_ActvnOfPrkgAut(self, data):
        # type: (datatypes.ActvnOfPrkgAut) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ActvnOfPrkgAut"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfPrkgAut(self):
        name = "ActvnOfPrkgAut"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfPrkgAut(self, datastring):
        # type: (str) -> datatypes.ActvnOfPrkgAut
        return json.loads(datastring)
    def get_ActvnOfPrkgAut(self):
        # type: () -> datatypes.ActvnOfPrkgAut
        return self.datamap["ActvnOfPrkgAut"]


    def send_ActvnOfSwtIllmnCen(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ActvnOfSwtIllmnCen"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfSwtIllmnCen(self):
        name = "ActvnOfSwtIllmnCen"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfSwtIllmnCen(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_ActvnOfSwtIllmnCen(self):
        # type: () -> (bool)
        return self.datamap["ActvnOfSwtIllmnCen"]


    def send_ActvnOfSwtIllmnClima(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ActvnOfSwtIllmnClima"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfSwtIllmnClima(self):
        name = "ActvnOfSwtIllmnClima"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfSwtIllmnClima(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_ActvnOfSwtIllmnClima(self):
        # type: () -> (bool)
        return self.datamap["ActvnOfSwtIllmnClima"]


    def send_ActvnOfSwtIllmnDrvMod(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ActvnOfSwtIllmnDrvMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfSwtIllmnDrvMod(self):
        name = "ActvnOfSwtIllmnDrvMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfSwtIllmnDrvMod(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_ActvnOfSwtIllmnDrvMod(self):
        # type: () -> (bool)
        return self.datamap["ActvnOfSwtIllmnDrvMod"]


    def send_ActvnOfSwtIllmnForSeatHeatrRe(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ActvnOfSwtIllmnForSeatHeatrRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvnOfSwtIllmnForSeatHeatrRe(self):
        name = "ActvnOfSwtIllmnForSeatHeatrRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvnOfSwtIllmnForSeatHeatrRe(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_ActvnOfSwtIllmnForSeatHeatrRe(self):
        # type: () -> (bool)
        return self.datamap["ActvnOfSwtIllmnForSeatHeatrRe"]


    def send_ActvtAutnmsPrkgCtrl(self, data):
        # type: (datatypes.ActvtAutnmsPrkgCtrl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ActvtAutnmsPrkgCtrl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ActvtAutnmsPrkgCtrl(self):
        name = "ActvtAutnmsPrkgCtrl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ActvtAutnmsPrkgCtrl(self, datastring):
        # type: (str) -> datatypes.ActvtAutnmsPrkgCtrl
        return json.loads(datastring)
    def get_ActvtAutnmsPrkgCtrl(self):
        # type: () -> datatypes.ActvtAutnmsPrkgCtrl
        return self.datamap["ActvtAutnmsPrkgCtrl"]


    def send_AdjSpdLimnSts(self, data):
        # type: (datatypes.AdjSpdLimnSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AdjSpdLimnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AdjSpdLimnSts(self):
        name = "AdjSpdLimnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AdjSpdLimnSts(self, datastring):
        # type: (str) -> datatypes.AdjSpdLimnSts2
        return json.loads(datastring)
    def get_AdjSpdLimnSts(self):
        # type: () -> datatypes.AdjSpdLimnSts2
        return self.datamap["AdjSpdLimnSts"]


    def send_AdjSpdLimnWarn(self, data):
        # type: (datatypes.AdjSpdLimnWarnCoding) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AdjSpdLimnWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AdjSpdLimnWarn(self):
        name = "AdjSpdLimnWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AdjSpdLimnWarn(self, datastring):
        # type: (str) -> datatypes.AdjSpdLimnWarnCoding
        return json.loads(datastring)
    def get_AdjSpdLimnWarn(self):
        # type: () -> datatypes.AdjSpdLimnWarnCoding
        return self.datamap["AdjSpdLimnWarn"]


    def send_AdprTurnSpdMod(self, data):
        # type: (datatypes.AdprTurnSpdModPen1) -> None
        s = toJson_AdprTurnSpdModPen1(data)
        name = "AdprTurnSpdMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AdprTurnSpdMod(self):
        name = "AdprTurnSpdMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AdprTurnSpdMod(self, datastring):
        # type: (str) -> datatypes.AdprTurnSpdModPen1
        return fromJson_AdprTurnSpdModPen1(datastring)
    def get_AdprTurnSpdMod(self):
        # type: () -> (datatypes.AdprTurnSpdModPen1)
        return self.datamap["AdprTurnSpdMod"]


    def send_AgDataRawSafe(self, data):
        # type: (datatypes.AgDataRawSafe1) -> None
        s = toJson_AgDataRawSafe1(data)
        name = "AgDataRawSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AgDataRawSafe(self):
        name = "AgDataRawSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AgDataRawSafe(self, datastring):
        # type: (str) -> datatypes.AgDataRawSafe1
        return fromJson_AgDataRawSafe1(datastring)
    def get_AgDataRawSafe(self):
        # type: () -> (datatypes.AgDataRawSafe1)
        return self.datamap["AgDataRawSafe"]


    def send_LiExtReq1WdReq5(self, data):
        # type: (datatypes.SlowNormFastPen) -> None
        s = toJson_SlowNormFastPen(data)
        name = "LiExtReq1WdReq5"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq1WdReq5(self):
        name = "LiExtReq1WdReq5"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq1WdReq5(self, datastring):
        # type: (str) -> datatypes.SlowNormFastPen
        return fromJson_SlowNormFastPen(datastring)
    def get_LiExtReq1WdReq5(self):
        # type: () -> (datatypes.SlowNormFastPen)
        return self.datamap["LiExtReq1WdReq5"]


    def send_AirClngReq(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AirClngReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AirClngReq(self):
        name = "AirClngReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AirClngReq(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_AirClngReq(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["AirClngReq"]


    def send_AmbTIndcdWithUnit(self, data):
        # type: (datatypes.AmbTIndcdWithUnit) -> None
        s = toJson_AmbTIndcdWithUnit(data)
        name = "AmbTIndcdWithUnit"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AmbTIndcdWithUnit(self):
        name = "AmbTIndcdWithUnit"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AmbTIndcdWithUnit(self, datastring):
        # type: (str) -> datatypes.AmbTIndcdWithUnit
        return fromJson_AmbTIndcdWithUnit(datastring)
    def get_AmbTIndcdWithUnit(self):
        # type: () -> (datatypes.AmbTIndcdWithUnit)
        return self.datamap["AmbTIndcdWithUnit"]


    def send_AmbTRaw(self, data):
        # type: (datatypes.AmbTWithQly) -> None
        s = toJson_AmbTWithQly(data)
        name = "AmbTRaw"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AmbTRaw(self):
        name = "AmbTRaw"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AmbTRaw(self, datastring):
        # type: (str) -> datatypes.AmbTWithQly
        return fromJson_AmbTWithQly(datastring)
    def get_AmbTRaw(self):
        # type: () -> (datatypes.AmbTWithQly)
        return self.datamap["AmbTRaw"]


    def send_AntithftRednReq(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "AntithftRednReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AntithftRednReq(self):
        name = "AntithftRednReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AntithftRednReq(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_AntithftRednReq(self):
        # type: () -> (int)
        return self.datamap["AntithftRednReq"]


    def send_AsyFctDeactvnWarn(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "AsyFctDeactvnWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AsyFctDeactvnWarn(self):
        name = "AsyFctDeactvnWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AsyFctDeactvnWarn(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_AsyFctDeactvnWarn(self):
        # type: () -> (int)
        return self.datamap["AsyFctDeactvnWarn"]


    def send_AsyLaneChg(self, data):
        # type: (datatypes.AsyLaneChg1) -> None
        s = toJson_AsyLaneChg1(data)
        name = "AsyLaneChg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AsyLaneChg(self):
        name = "AsyLaneChg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AsyLaneChg(self, datastring):
        # type: (str) -> datatypes.AsyLaneChg1
        return fromJson_AsyLaneChg1(datastring)
    def get_AsyLaneChg(self):
        # type: () -> (datatypes.AsyLaneChg1)
        return self.datamap["AsyLaneChg"]


    def send_AsySteerApplyRqrd(self, data):
        # type: (datatypes.LvlWarn1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AsySteerApplyRqrd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AsySteerApplyRqrd(self):
        name = "AsySteerApplyRqrd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AsySteerApplyRqrd(self, datastring):
        # type: (str) -> datatypes.LvlWarn1
        return json.loads(datastring)
    def get_AsySteerApplyRqrd(self):
        # type: () -> datatypes.LvlWarn1
        return self.datamap["AsySteerApplyRqrd"]


    def send_AsySteerFctDeactvdWarn(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AsySteerFctDeactvdWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AsySteerFctDeactvdWarn(self):
        name = "AsySteerFctDeactvdWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AsySteerFctDeactvdWarn(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_AsySteerFctDeactvdWarn(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["AsySteerFctDeactvdWarn"]


    def send_AudMsgReq(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "AudMsgReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AudMsgReq(self):
        name = "AudMsgReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AudMsgReq(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_AudMsgReq(self):
        # type: () -> (int)
        return self.datamap["AudMsgReq"]


    def send_AudWarnActv(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AudWarnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AudWarnActv(self):
        name = "AudWarnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AudWarnActv(self, datastring):
        # type: (str) -> datatypes.NoYesCrit1
        return json.loads(datastring)
    def get_AudWarnActv(self):
        # type: () -> datatypes.NoYesCrit1
        return self.datamap["AudWarnActv"]


    def send_AutnmsPrkgActvScenario(self, data):
        # type: (datatypes.AutnmsPrkgActvScenario) -> None
        s = toJson_AutnmsPrkgActvScenario(data)
        name = "AutnmsPrkgActvScenario"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AutnmsPrkgActvScenario(self):
        name = "AutnmsPrkgActvScenario"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AutnmsPrkgActvScenario(self, datastring):
        # type: (str) -> datatypes.AutnmsPrkgActvScenario
        return fromJson_AutnmsPrkgActvScenario(datastring)
    def get_AutnmsPrkgActvScenario(self):
        # type: () -> (datatypes.AutnmsPrkgActvScenario)
        return self.datamap["AutnmsPrkgActvScenario"]


    def send_AutnmsPrkgActvn(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AutnmsPrkgActvn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AutnmsPrkgActvn(self):
        name = "AutnmsPrkgActvn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AutnmsPrkgActvn(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_AutnmsPrkgActvn(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["AutnmsPrkgActvn"]


    def send_AutnmsPrkgAvlScenario(self, data):
        # type: (datatypes.AutnmsPrkgAvlScenario) -> None
        s = toJson_AutnmsPrkgAvlScenario(data)
        name = "AutnmsPrkgAvlScenario"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AutnmsPrkgAvlScenario(self):
        name = "AutnmsPrkgAvlScenario"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AutnmsPrkgAvlScenario(self, datastring):
        # type: (str) -> datatypes.AutnmsPrkgAvlScenario
        return fromJson_AutnmsPrkgAvlScenario(datastring)
    def get_AutnmsPrkgAvlScenario(self):
        # type: () -> (datatypes.AutnmsPrkgAvlScenario)
        return self.datamap["AutnmsPrkgAvlScenario"]


    def send_AutnmsPrkgEnaResu(self, data):
        # type: (datatypes.EnaResu) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AutnmsPrkgEnaResu"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AutnmsPrkgEnaResu(self):
        name = "AutnmsPrkgEnaResu"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AutnmsPrkgEnaResu(self, datastring):
        # type: (str) -> datatypes.EnaResu
        return json.loads(datastring)
    def get_AutnmsPrkgEnaResu(self):
        # type: () -> datatypes.EnaResu
        return self.datamap["AutnmsPrkgEnaResu"]


    def send_AutnmsPrkgProgs(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "AutnmsPrkgProgs"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AutnmsPrkgProgs(self):
        name = "AutnmsPrkgProgs"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AutnmsPrkgProgs(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_AutnmsPrkgProgs(self):
        # type: () -> (int)
        return self.datamap["AutnmsPrkgProgs"]


    def send_AutnmsPrkgSeldScenario(self, data):
        # type: (datatypes.AutnmsPrkgSeldScenario) -> None
        s = toJson_AutnmsPrkgSeldScenario(data)
        name = "AutnmsPrkgSeldScenario"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AutnmsPrkgSeldScenario(self):
        name = "AutnmsPrkgSeldScenario"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AutnmsPrkgSeldScenario(self, datastring):
        # type: (str) -> datatypes.AutnmsPrkgSeldScenario
        return fromJson_AutnmsPrkgSeldScenario(datastring)
    def get_AutnmsPrkgSeldScenario(self):
        # type: () -> (datatypes.AutnmsPrkgSeldScenario)
        return self.datamap["AutnmsPrkgSeldScenario"]


    def send_AvlStsForLatAutDrv(self, data):
        # type: (datatypes.AvlStsForLatAutDrv5) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "AvlStsForLatAutDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AvlStsForLatAutDrv(self):
        name = "AvlStsForLatAutDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AvlStsForLatAutDrv(self, datastring):
        # type: (str) -> datatypes.AvlStsForLatAutDrv5
        return json.loads(datastring)
    def get_AvlStsForLatAutDrv(self):
        # type: () -> datatypes.AvlStsForLatAutDrv5
        return self.datamap["AvlStsForLatAutDrv"]


    def send_BackCntrForMissCom(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "BackCntrForMissCom"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BackCntrForMissCom(self):
        name = "BackCntrForMissCom"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BackCntrForMissCom(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_BackCntrForMissCom(self):
        # type: () -> (int)
        return self.datamap["BackCntrForMissCom"]


    def send_BlisSwOnOff(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BlisSwOnOff"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BlisSwOnOff(self):
        name = "BlisSwOnOff"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BlisSwOnOff(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_BlisSwOnOff(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["BlisSwOnOff"]


    def send_BltLockStAtDrvr(self, data):
        # type: (datatypes.BltLockStFrnt) -> None
        s = toJson_BltLockStFrnt(data)
        name = "BltLockStAtDrvr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtDrvr(self):
        name = "BltLockStAtDrvr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtDrvr(self, datastring):
        # type: (str) -> datatypes.BltLockStFrnt
        return fromJson_BltLockStFrnt(datastring)
    def get_BltLockStAtDrvr(self):
        # type: () -> (datatypes.BltLockStFrnt)
        return self.datamap["BltLockStAtDrvr"]


    def send_BltLockStAtPass(self, data):
        # type: (datatypes.BltLockStFrnt) -> None
        s = toJson_BltLockStFrnt(data)
        name = "BltLockStAtPass"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtPass(self):
        name = "BltLockStAtPass"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtPass(self, datastring):
        # type: (str) -> datatypes.BltLockStFrnt
        return fromJson_BltLockStFrnt(datastring)
    def get_BltLockStAtPass(self):
        # type: () -> (datatypes.BltLockStFrnt)
        return self.datamap["BltLockStAtPass"]


    def send_BltLockStAtRowSecLe(self, data):
        # type: (datatypes.BltLockStRe) -> None
        s = toJson_BltLockStRe(data)
        name = "BltLockStAtRowSecLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtRowSecLe(self):
        name = "BltLockStAtRowSecLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtRowSecLe(self, datastring):
        # type: (str) -> datatypes.BltLockStRe
        return fromJson_BltLockStRe(datastring)
    def get_BltLockStAtRowSecLe(self):
        # type: () -> (datatypes.BltLockStRe)
        return self.datamap["BltLockStAtRowSecLe"]


    def send_BltLockStAtRowSecMid(self, data):
        # type: (datatypes.BltLockStRe) -> None
        s = toJson_BltLockStRe(data)
        name = "BltLockStAtRowSecMid"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtRowSecMid(self):
        name = "BltLockStAtRowSecMid"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtRowSecMid(self, datastring):
        # type: (str) -> datatypes.BltLockStRe
        return fromJson_BltLockStRe(datastring)
    def get_BltLockStAtRowSecMid(self):
        # type: () -> (datatypes.BltLockStRe)
        return self.datamap["BltLockStAtRowSecMid"]


    def send_BltLockStAtRowSecRi(self, data):
        # type: (datatypes.BltLockStRe) -> None
        s = toJson_BltLockStRe(data)
        name = "BltLockStAtRowSecRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtRowSecRi(self):
        name = "BltLockStAtRowSecRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtRowSecRi(self, datastring):
        # type: (str) -> datatypes.BltLockStRe
        return fromJson_BltLockStRe(datastring)
    def get_BltLockStAtRowSecRi(self):
        # type: () -> (datatypes.BltLockStRe)
        return self.datamap["BltLockStAtRowSecRi"]


    def send_BltLockStAtRowThrdLe(self, data):
        # type: (datatypes.BltLockStRe) -> None
        s = toJson_BltLockStRe(data)
        name = "BltLockStAtRowThrdLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtRowThrdLe(self):
        name = "BltLockStAtRowThrdLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtRowThrdLe(self, datastring):
        # type: (str) -> datatypes.BltLockStRe
        return fromJson_BltLockStRe(datastring)
    def get_BltLockStAtRowThrdLe(self):
        # type: () -> (datatypes.BltLockStRe)
        return self.datamap["BltLockStAtRowThrdLe"]


    def send_BltLockStAtRowThrdRi(self, data):
        # type: (datatypes.BltLockStRe) -> None
        s = toJson_BltLockStRe(data)
        name = "BltLockStAtRowThrdRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltLockStAtRowThrdRi(self):
        name = "BltLockStAtRowThrdRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltLockStAtRowThrdRi(self, datastring):
        # type: (str) -> datatypes.BltLockStRe
        return fromJson_BltLockStRe(datastring)
    def get_BltLockStAtRowThrdRi(self):
        # type: () -> (datatypes.BltLockStRe)
        return self.datamap["BltLockStAtRowThrdRi"]


    def send_BltRmnSound1(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BltRmnSound1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltRmnSound1(self):
        name = "BltRmnSound1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltRmnSound1(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_BltRmnSound1(self):
        # type: () -> datatypes.Trig1
        return self.datamap["BltRmnSound1"]


    def send_BltRmnSound2(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BltRmnSound2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltRmnSound2(self):
        name = "BltRmnSound2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltRmnSound2(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_BltRmnSound2(self):
        # type: () -> datatypes.Trig1
        return self.datamap["BltRmnSound2"]


    def send_BltRmnSound3(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BltRmnSound3"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltRmnSound3(self):
        name = "BltRmnSound3"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltRmnSound3(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_BltRmnSound3(self):
        # type: () -> datatypes.Trig1
        return self.datamap["BltRmnSound3"]


    def send_BltRmnSound4(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BltRmnSound4"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BltRmnSound4(self):
        name = "BltRmnSound4"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BltRmnSound4(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_BltRmnSound4(self):
        # type: () -> datatypes.Trig1
        return self.datamap["BltRmnSound4"]


    def send_BrkAndAbsWarnIndcnReq(self, data):
        # type: (datatypes.BrkAndAbsWarnIndcnReqRec1) -> None
        s = toJson_BrkAndAbsWarnIndcnReqRec1(data)
        name = "BrkAndAbsWarnIndcnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BrkAndAbsWarnIndcnReq(self):
        name = "BrkAndAbsWarnIndcnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BrkAndAbsWarnIndcnReq(self, datastring):
        # type: (str) -> datatypes.BrkAndAbsWarnIndcnReqRec1
        return fromJson_BrkAndAbsWarnIndcnReqRec1(datastring)
    def get_BrkAndAbsWarnIndcnReq(self):
        # type: () -> (datatypes.BrkAndAbsWarnIndcnReqRec1)
        return self.datamap["BrkAndAbsWarnIndcnReq"]


    def send_BrkFldLvl(self, data):
        # type: (datatypes.FldLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BrkFldLvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BrkFldLvl(self):
        name = "BrkFldLvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BrkFldLvl(self, datastring):
        # type: (str) -> datatypes.FldLvl
        return json.loads(datastring)
    def get_BrkFldLvl(self):
        # type: () -> datatypes.FldLvl
        return self.datamap["BrkFldLvl"]


    def send_BrkFricTqAtWhlAct(self, data):
        # type: (datatypes.BrkFricTqAtWhlActRec) -> None
        s = toJson_BrkFricTqAtWhlActRec(data)
        name = "BrkFricTqAtWhlAct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BrkFricTqAtWhlAct(self):
        name = "BrkFricTqAtWhlAct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BrkFricTqAtWhlAct(self, datastring):
        # type: (str) -> datatypes.BrkFricTqAtWhlActRec
        return fromJson_BrkFricTqAtWhlActRec(datastring)
    def get_BrkFricTqAtWhlAct(self):
        # type: () -> (datatypes.BrkFricTqAtWhlActRec)
        return self.datamap["BrkFricTqAtWhlAct"]


    def send_BrkRelsWarnReq(self, data):
        # type: (datatypes.NoYes1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BrkRelsWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BrkRelsWarnReq(self):
        name = "BrkRelsWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BrkRelsWarnReq(self, datastring):
        # type: (str) -> datatypes.NoYes1
        return json.loads(datastring)
    def get_BrkRelsWarnReq(self):
        # type: () -> datatypes.NoYes1
        return self.datamap["BrkRelsWarnReq"]


    def send_Btn1ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "Btn1ForUsrSwtPanFrntReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_Btn1ForUsrSwtPanFrntReq(self):
        name = "Btn1ForUsrSwtPanFrntReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_Btn1ForUsrSwtPanFrntReq(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_Btn1ForUsrSwtPanFrntReq(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["Btn1ForUsrSwtPanFrntReq"]


    def send_Btn2ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "Btn2ForUsrSwtPanFrntReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_Btn2ForUsrSwtPanFrntReq(self):
        name = "Btn2ForUsrSwtPanFrntReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_Btn2ForUsrSwtPanFrntReq(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_Btn2ForUsrSwtPanFrntReq(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["Btn2ForUsrSwtPanFrntReq"]


    def send_Btn3ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "Btn3ForUsrSwtPanFrntReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_Btn3ForUsrSwtPanFrntReq(self):
        name = "Btn3ForUsrSwtPanFrntReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_Btn3ForUsrSwtPanFrntReq(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_Btn3ForUsrSwtPanFrntReq(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["Btn3ForUsrSwtPanFrntReq"]


    def send_Btn4ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "Btn4ForUsrSwtPanFrntReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_Btn4ForUsrSwtPanFrntReq(self):
        name = "Btn4ForUsrSwtPanFrntReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_Btn4ForUsrSwtPanFrntReq(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_Btn4ForUsrSwtPanFrntReq(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["Btn4ForUsrSwtPanFrntReq"]


    def send_Btn5ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "Btn5ForUsrSwtPanFrntReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_Btn5ForUsrSwtPanFrntReq(self):
        name = "Btn5ForUsrSwtPanFrntReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_Btn5ForUsrSwtPanFrntReq(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_Btn5ForUsrSwtPanFrntReq(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["Btn5ForUsrSwtPanFrntReq"]


    def send_BtnAudFbSts(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BtnAudFbSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BtnAudFbSts(self):
        name = "BtnAudFbSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BtnAudFbSts(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_BtnAudFbSts(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["BtnAudFbSts"]


    def send_BtnAudVolSts(self, data):
        # type: (datatypes.BtnAudVolSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BtnAudVolSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BtnAudVolSts(self):
        name = "BtnAudVolSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BtnAudVolSts(self, datastring):
        # type: (str) -> datatypes.BtnAudVolSts
        return json.loads(datastring)
    def get_BtnAudVolSts(self):
        # type: () -> datatypes.BtnAudVolSts
        return self.datamap["BtnAudVolSts"]


    def send_BtnIllmnForWinDefrstFrnt(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BtnIllmnForWinDefrstFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BtnIllmnForWinDefrstFrnt(self):
        name = "BtnIllmnForWinDefrstFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BtnIllmnForWinDefrstFrnt(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_BtnIllmnForWinDefrstFrnt(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["BtnIllmnForWinDefrstFrnt"]


    def send_BtnMmedLeRiSts(self, data):
        # type: (datatypes.BtnMmedLeRiSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BtnMmedLeRiSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BtnMmedLeRiSts(self):
        name = "BtnMmedLeRiSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BtnMmedLeRiSts(self, datastring):
        # type: (str) -> datatypes.BtnMmedLeRiSts
        return json.loads(datastring)
    def get_BtnMmedLeRiSts(self):
        # type: () -> datatypes.BtnMmedLeRiSts
        return self.datamap["BtnMmedLeRiSts"]


    def send_BtnMmedModSts(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BtnMmedModSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BtnMmedModSts(self):
        name = "BtnMmedModSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BtnMmedModSts(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_BtnMmedModSts(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["BtnMmedModSts"]


    def send_BtnMmedSetSts(self, data):
        # type: (datatypes.BtnMmedSetSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "BtnMmedSetSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BtnMmedSetSts(self):
        name = "BtnMmedSetSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BtnMmedSetSts(self, datastring):
        # type: (str) -> datatypes.BtnMmedSetSts
        return json.loads(datastring)
    def get_BtnMmedSetSts(self):
        # type: () -> datatypes.BtnMmedSetSts
        return self.datamap["BtnMmedSetSts"]


    def send_CCSMBtn6(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CCSMBtn6"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CCSMBtn6(self):
        name = "CCSMBtn6"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CCSMBtn6(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_CCSMBtn6(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["CCSMBtn6"]


    def send_CallSts(self, data):
        # type: (datatypes.CallSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CallSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CallSts(self):
        name = "CallSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CallSts(self, datastring):
        # type: (str) -> datatypes.CallSts
        return json.loads(datastring)
    def get_CallSts(self):
        # type: () -> datatypes.CallSts
        return self.datamap["CallSts"]


    def send_CarTiGlb(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "CarTiGlb"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CarTiGlb(self):
        name = "CarTiGlb"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CarTiGlb(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_CarTiGlb(self):
        # type: () -> (float)
        return self.datamap["CarTiGlb"]


    def send_CchForFuEco(self, data):
        # type: (datatypes.CchForFuEco1) -> None
        s = toJson_CchForFuEco1(data)
        name = "CchForFuEco"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CchForFuEco(self):
        name = "CchForFuEco"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CchForFuEco(self, datastring):
        # type: (str) -> datatypes.CchForFuEco1
        return fromJson_CchForFuEco1(datastring)
    def get_CchForFuEco(self):
        # type: () -> (datatypes.CchForFuEco1)
        return self.datamap["CchForFuEco"]


    def send_ChdLockgProtnStsToHmi(self, data):
        # type: (datatypes.OnOffSafe1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ChdLockgProtnStsToHmi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ChdLockgProtnStsToHmi(self):
        name = "ChdLockgProtnStsToHmi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ChdLockgProtnStsToHmi(self, datastring):
        # type: (str) -> datatypes.OnOffSafe1
        return json.loads(datastring)
    def get_ChdLockgProtnStsToHmi(self):
        # type: () -> datatypes.OnOffSafe1
        return self.datamap["ChdLockgProtnStsToHmi"]


    def send_ChdWinProtnStsAtDrvrRe(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ChdWinProtnStsAtDrvrRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ChdWinProtnStsAtDrvrRe(self):
        name = "ChdWinProtnStsAtDrvrRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ChdWinProtnStsAtDrvrRe(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ChdWinProtnStsAtDrvrRe(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ChdWinProtnStsAtDrvrRe"]


    def send_ChdWinProtnStsAtPassRe(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ChdWinProtnStsAtPassRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ChdWinProtnStsAtPassRe(self):
        name = "ChdWinProtnStsAtPassRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ChdWinProtnStsAtPassRe(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ChdWinProtnStsAtPassRe(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ChdWinProtnStsAtPassRe"]


    def send_ChrgrHwAprvdWirelsAdr(self, data):
        # type: (datatypes.HwAprvdWirelsAdr1) -> None
        s = toJson_HwAprvdWirelsAdr1(data)
        name = "ChrgrHwAprvdWirelsAdr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ChrgrHwAprvdWirelsAdr(self):
        name = "ChrgrHwAprvdWirelsAdr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ChrgrHwAprvdWirelsAdr(self, datastring):
        # type: (str) -> datatypes.HwAprvdWirelsAdr1
        return fromJson_HwAprvdWirelsAdr1(datastring)
    def get_ChrgrHwAprvdWirelsAdr(self):
        # type: () -> (datatypes.HwAprvdWirelsAdr1)
        return self.datamap["ChrgrHwAprvdWirelsAdr"]


    def send_ChrgrWirelsSts(self, data):
        # type: (datatypes.ChrgrWirelsSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ChrgrWirelsSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ChrgrWirelsSts(self):
        name = "ChrgrWirelsSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ChrgrWirelsSts(self, datastring):
        # type: (str) -> datatypes.ChrgrWirelsSts
        return json.loads(datastring)
    def get_ChrgrWirelsSts(self):
        # type: () -> datatypes.ChrgrWirelsSts
        return self.datamap["ChrgrWirelsSts"]


    def send_ClimaActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClimaActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaActv(self):
        name = "ClimaActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ClimaActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ClimaActv"]


    def send_ClimaPwrCns(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ClimaPwrCns"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaPwrCns(self):
        name = "ClimaPwrCns"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaPwrCns(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_ClimaPwrCns(self):
        # type: () -> (int)
        return self.datamap["ClimaPwrCns"]


    def send_ClimaRqrdFromHmi(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClimaRqrdFromHmi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaRqrdFromHmi(self):
        name = "ClimaRqrdFromHmi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaRqrdFromHmi(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_ClimaRqrdFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["ClimaRqrdFromHmi"]


    def send_ClimaTmr(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ClimaTmr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaTmr(self):
        name = "ClimaTmr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaTmr(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_ClimaTmr(self):
        # type: () -> (float)
        return self.datamap["ClimaTmr"]


    def send_ClimaTmrSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClimaTmrSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaTmrSts(self):
        name = "ClimaTmrSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaTmrSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ClimaTmrSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ClimaTmrSts"]


    def send_ClimaTmrStsRqrd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClimaTmrStsRqrd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaTmrStsRqrd(self):
        name = "ClimaTmrStsRqrd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaTmrStsRqrd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ClimaTmrStsRqrd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ClimaTmrStsRqrd"]


    def send_ClimaWarn(self, data):
        # type: (datatypes.ClimaWarn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClimaWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClimaWarn(self):
        name = "ClimaWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClimaWarn(self, datastring):
        # type: (str) -> datatypes.ClimaWarn
        return json.loads(datastring)
    def get_ClimaWarn(self):
        # type: () -> datatypes.ClimaWarn
        return self.datamap["ClimaWarn"]


    def send_CllsnAidSnvtySeld(self, data):
        # type: (datatypes.SnvtyPen1) -> None
        s = toJson_SnvtyPen1(data)
        name = "CllsnAidSnvtySeld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CllsnAidSnvtySeld(self):
        name = "CllsnAidSnvtySeld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CllsnAidSnvtySeld(self, datastring):
        # type: (str) -> datatypes.SnvtyPen1
        return fromJson_SnvtyPen1(datastring)
    def get_CllsnAidSnvtySeld(self):
        # type: () -> (datatypes.SnvtyPen1)
        return self.datamap["CllsnAidSnvtySeld"]


    def send_CllsnFwdWarnActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "CllsnFwdWarnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CllsnFwdWarnActv(self):
        name = "CllsnFwdWarnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CllsnFwdWarnActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_CllsnFwdWarnActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["CllsnFwdWarnActv"]


    def send_CllsnFwdWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CllsnFwdWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CllsnFwdWarnReq(self):
        name = "CllsnFwdWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CllsnFwdWarnReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_CllsnFwdWarnReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["CllsnFwdWarnReq"]


    def send_CllsnWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CllsnWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CllsnWarnReq(self):
        name = "CllsnWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CllsnWarnReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_CllsnWarnReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["CllsnWarnReq"]


    def send_CllsnWarnSide(self, data):
        # type: (datatypes.CllsnWarnSide1) -> None
        s = toJson_CllsnWarnSide1(data)
        name = "CllsnWarnSide"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CllsnWarnSide(self):
        name = "CllsnWarnSide"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CllsnWarnSide(self, datastring):
        # type: (str) -> datatypes.CllsnWarnSide1
        return fromJson_CllsnWarnSide1(datastring)
    def get_CllsnWarnSide(self):
        # type: () -> (datatypes.CllsnWarnSide1)
        return self.datamap["CllsnWarnSide"]


    def send_ClngActv(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClngActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClngActv(self):
        name = "ClngActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClngActv(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_ClngActv(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["ClngActv"]


    def send_ClngRqrdFromHmi(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClngRqrdFromHmi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClngRqrdFromHmi(self):
        name = "ClngRqrdFromHmi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClngRqrdFromHmi(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_ClngRqrdFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["ClngRqrdFromHmi"]


    def send_ClsAutEna(self, data):
        # type: (datatypes.EnableDisableCoding) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ClsAutEna"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ClsAutEna(self):
        name = "ClsAutEna"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ClsAutEna(self, datastring):
        # type: (str) -> datatypes.EnableDisableCoding
        return json.loads(datastring)
    def get_ClsAutEna(self):
        # type: () -> datatypes.EnableDisableCoding
        return self.datamap["ClsAutEna"]


    def send_CmftFctActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CmftFctActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CmftFctActv(self):
        name = "CmftFctActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CmftFctActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_CmftFctActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["CmftFctActv"]


    def send_CmftFctSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CmftFctSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CmftFctSts(self):
        name = "CmftFctSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CmftFctSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_CmftFctSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["CmftFctSts"]


    def send_CmptmtAirTEstimdExtd(self, data):
        # type: (datatypes.CmptmtAirTEstimdExtd) -> None
        s = toJson_CmptmtAirTEstimdExtd(data)
        name = "CmptmtAirTEstimdExtd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CmptmtAirTEstimdExtd(self):
        name = "CmptmtAirTEstimdExtd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CmptmtAirTEstimdExtd(self, datastring):
        # type: (str) -> datatypes.CmptmtAirTEstimdExtd
        return fromJson_CmptmtAirTEstimdExtd(datastring)
    def get_CmptmtAirTEstimdExtd(self):
        # type: () -> (datatypes.CmptmtAirTEstimdExtd)
        return self.datamap["CmptmtAirTEstimdExtd"]


    def send_CmptmtTFrnt(self, data):
        # type: (datatypes.CmptmtTFrnt) -> None
        s = toJson_CmptmtTFrnt(data)
        name = "CmptmtTFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CmptmtTFrnt(self):
        name = "CmptmtTFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CmptmtTFrnt(self, datastring):
        # type: (str) -> datatypes.CmptmtTFrnt
        return fromJson_CmptmtTFrnt(datastring)
    def get_CmptmtTFrnt(self):
        # type: () -> (datatypes.CmptmtTFrnt)
        return self.datamap["CmptmtTFrnt"]


    def send_CnclWarnForCrsCtrl(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CnclWarnForCrsCtrl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CnclWarnForCrsCtrl(self):
        name = "CnclWarnForCrsCtrl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CnclWarnForCrsCtrl(self, datastring):
        # type: (str) -> datatypes.NoYesCrit1
        return json.loads(datastring)
    def get_CnclWarnForCrsCtrl(self):
        # type: () -> datatypes.NoYesCrit1
        return self.datamap["CnclWarnForCrsCtrl"]


    def send_CnclWarnLgtForAutDrv(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CnclWarnLgtForAutDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CnclWarnLgtForAutDrv(self):
        name = "CnclWarnLgtForAutDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CnclWarnLgtForAutDrv(self, datastring):
        # type: (str) -> datatypes.NoYesCrit1
        return json.loads(datastring)
    def get_CnclWarnLgtForAutDrv(self):
        # type: () -> datatypes.NoYesCrit1
        return self.datamap["CnclWarnLgtForAutDrv"]


    def send_CnsPrm(self, data):
        # type: (datatypes.CnsPrmRec1) -> None
        s = toJson_CnsPrmRec1(data)
        name = "CnsPrm"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CnsPrm(self):
        name = "CnsPrm"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CnsPrm(self, datastring):
        # type: (str) -> datatypes.CnsPrmRec1
        return fromJson_CnsPrmRec1(datastring)
    def get_CnsPrm(self):
        # type: () -> (datatypes.CnsPrmRec1)
        return self.datamap["CnsPrm"]


    def send_ComLostExtrSound(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ComLostExtrSound"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ComLostExtrSound(self):
        name = "ComLostExtrSound"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ComLostExtrSound(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_ComLostExtrSound(self):
        # type: () -> (bool)
        return self.datamap["ComLostExtrSound"]


    def send_ConSftyDataSharingAllwd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ConSftyDataSharingAllwd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ConSftyDataSharingAllwd(self):
        name = "ConSftyDataSharingAllwd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ConSftyDataSharingAllwd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ConSftyDataSharingAllwd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ConSftyDataSharingAllwd"]


    def send_ConSftyDataSharingSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ConSftyDataSharingSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ConSftyDataSharingSts(self):
        name = "ConSftyDataSharingSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ConSftyDataSharingSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_ConSftyDataSharingSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["ConSftyDataSharingSts"]


    def send_ConSftyWarn(self, data):
        # type: (datatypes.ConSftyWarn1) -> None
        s = toJson_ConSftyWarn1(data)
        name = "ConSftyWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ConSftyWarn(self):
        name = "ConSftyWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ConSftyWarn(self, datastring):
        # type: (str) -> datatypes.ConSftyWarn1
        return fromJson_ConSftyWarn1(datastring)
    def get_ConSftyWarn(self):
        # type: () -> (datatypes.ConSftyWarn1)
        return self.datamap["ConSftyWarn"]


    def send_CoolgReqForDispCen(self, data):
        # type: (datatypes.CoolgReqForDisp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CoolgReqForDispCen"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CoolgReqForDispCen(self):
        name = "CoolgReqForDispCen"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CoolgReqForDispCen(self, datastring):
        # type: (str) -> datatypes.CoolgReqForDisp
        return json.loads(datastring)
    def get_CoolgReqForDispCen(self):
        # type: () -> datatypes.CoolgReqForDisp
        return self.datamap["CoolgReqForDispCen"]


    def send_CoolgStsForDisp(self, data):
        # type: (datatypes.CoolgStsForDisp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CoolgStsForDisp"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CoolgStsForDisp(self):
        name = "CoolgStsForDisp"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CoolgStsForDisp(self, datastring):
        # type: (str) -> datatypes.CoolgStsForDisp
        return json.loads(datastring)
    def get_CoolgStsForDisp(self):
        # type: () -> datatypes.CoolgStsForDisp
        return self.datamap["CoolgStsForDisp"]


    def send_CrsCtrlrSts(self, data):
        # type: (datatypes.CrsCtrlrSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CrsCtrlrSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CrsCtrlrSts(self):
        name = "CrsCtrlrSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CrsCtrlrSts(self, datastring):
        # type: (str) -> datatypes.CrsCtrlrSts1
        return json.loads(datastring)
    def get_CrsCtrlrSts(self):
        # type: () -> datatypes.CrsCtrlrSts1
        return self.datamap["CrsCtrlrSts"]


    def send_CrvtSpdAdpvSts(self, data):
        # type: (datatypes.OffOnNotAvlSrvRqrd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CrvtSpdAdpvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CrvtSpdAdpvSts(self):
        name = "CrvtSpdAdpvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CrvtSpdAdpvSts(self, datastring):
        # type: (str) -> datatypes.OffOnNotAvlSrvRqrd
        return json.loads(datastring)
    def get_CrvtSpdAdpvSts(self):
        # type: () -> datatypes.OffOnNotAvlSrvRqrd
        return self.datamap["CrvtSpdAdpvSts"]


    def send_CrvtSpdEstimrSnvtySeld(self, data):
        # type: (datatypes.SnvtyPen3) -> None
        s = toJson_SnvtyPen3(data)
        name = "CrvtSpdEstimrSnvtySeld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CrvtSpdEstimrSnvtySeld(self):
        name = "CrvtSpdEstimrSnvtySeld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CrvtSpdEstimrSnvtySeld(self, datastring):
        # type: (str) -> datatypes.SnvtyPen3
        return fromJson_SnvtyPen3(datastring)
    def get_CrvtSpdEstimrSnvtySeld(self):
        # type: () -> (datatypes.SnvtyPen3)
        return self.datamap["CrvtSpdEstimrSnvtySeld"]


    def send_CrvtSpdWarnActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "CrvtSpdWarnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CrvtSpdWarnActv(self):
        name = "CrvtSpdWarnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CrvtSpdWarnActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_CrvtSpdWarnActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["CrvtSpdWarnActv"]


    def send_CrvtSpdWarnReq(self, data):
        # type: (datatypes.TurnSpdWarnReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CrvtSpdWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CrvtSpdWarnReq(self):
        name = "CrvtSpdWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CrvtSpdWarnReq(self, datastring):
        # type: (str) -> datatypes.TurnSpdWarnReq1
        return json.loads(datastring)
    def get_CrvtSpdWarnReq(self):
        # type: () -> datatypes.TurnSpdWarnReq1
        return self.datamap["CrvtSpdWarnReq"]


    def send_CrvtSpdWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CrvtSpdWarnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CrvtSpdWarnSts(self):
        name = "CrvtSpdWarnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CrvtSpdWarnSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_CrvtSpdWarnSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["CrvtSpdWarnSts"]


    def send_CtraIndcnLe(self, data):
        # type: (datatypes.CtraIndcn1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CtraIndcnLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CtraIndcnLe(self):
        name = "CtraIndcnLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CtraIndcnLe(self, datastring):
        # type: (str) -> datatypes.CtraIndcn1
        return json.loads(datastring)
    def get_CtraIndcnLe(self):
        # type: () -> datatypes.CtraIndcn1
        return self.datamap["CtraIndcnLe"]


    def send_CtraIndcnRi(self, data):
        # type: (datatypes.CtraIndcn1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CtraIndcnRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CtraIndcnRi(self):
        name = "CtraIndcnRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CtraIndcnRi(self, datastring):
        # type: (str) -> datatypes.CtraIndcn1
        return json.loads(datastring)
    def get_CtraIndcnRi(self):
        # type: () -> datatypes.CtraIndcn1
        return self.datamap["CtraIndcnRi"]


    def send_CtraOn1(self, data):
        # type: (datatypes.LcmaCtraOn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CtraOn1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CtraOn1(self):
        name = "CtraOn1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CtraOn1(self, datastring):
        # type: (str) -> datatypes.LcmaCtraOn
        return json.loads(datastring)
    def get_CtraOn1(self):
        # type: () -> datatypes.LcmaCtraOn
        return self.datamap["CtraOn1"]


    def send_CtraSwOnOff(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CtraSwOnOff"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CtraSwOnOff(self):
        name = "CtraSwOnOff"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CtraSwOnOff(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_CtraSwOnOff(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["CtraSwOnOff"]


    def send_CurtActvnReReq(self, data):
        # type: (datatypes.CurtActvnTyp1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "CurtActvnReReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_CurtActvnReReq(self):
        name = "CurtActvnReReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_CurtActvnReReq(self, datastring):
        # type: (str) -> datatypes.CurtActvnTyp1
        return json.loads(datastring)
    def get_CurtActvnReReq(self):
        # type: () -> datatypes.CurtActvnTyp1
        return self.datamap["CurtActvnReReq"]


    def send_DataDistbnAllwdForEgyPred(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DataDistbnAllwdForEgyPred"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DataDistbnAllwdForEgyPred(self):
        name = "DataDistbnAllwdForEgyPred"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DataDistbnAllwdForEgyPred(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_DataDistbnAllwdForEgyPred(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["DataDistbnAllwdForEgyPred"]


    def send_DataDistbnReqForEgyPred(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DataDistbnReqForEgyPred"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DataDistbnReqForEgyPred(self):
        name = "DataDistbnReqForEgyPred"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DataDistbnReqForEgyPred(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_DataDistbnReqForEgyPred(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["DataDistbnReqForEgyPred"]


    def send_DataSpclDMSM(self, data):
        # type: (datatypes.DataSpclTyp) -> None
        s = toJson_DataSpclTyp(data)
        name = "DataSpclDMSM"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DataSpclDMSM(self):
        name = "DataSpclDMSM"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DataSpclDMSM(self, datastring):
        # type: (str) -> datatypes.DataSpclTyp
        return fromJson_DataSpclTyp(datastring)
    def get_DataSpclDMSM(self):
        # type: () -> (datatypes.DataSpclTyp)
        return self.datamap["DataSpclDMSM"]


    def send_DayToSrv(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DayToSrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DayToSrv(self):
        name = "DayToSrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DayToSrv(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DayToSrv(self):
        # type: () -> (int)
        return self.datamap["DayToSrv"]


    def send_DeactvtLvlgCtrl(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DeactvtLvlgCtrl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DeactvtLvlgCtrl(self):
        name = "DeactvtLvlgCtrl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DeactvtLvlgCtrl(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_DeactvtLvlgCtrl(self):
        # type: () -> (bool)
        return self.datamap["DeactvtLvlgCtrl"]


    def send_DefrstDurgClimaEnad(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DefrstDurgClimaEnad"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DefrstDurgClimaEnad(self):
        name = "DefrstDurgClimaEnad"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DefrstDurgClimaEnad(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_DefrstDurgClimaEnad(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["DefrstDurgClimaEnad"]


    def send_DestSrcAndCfmRqrd(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DestSrcAndCfmRqrd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DestSrcAndCfmRqrd(self):
        name = "DestSrcAndCfmRqrd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DestSrcAndCfmRqrd(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DestSrcAndCfmRqrd(self):
        # type: () -> (int)
        return self.datamap["DestSrcAndCfmRqrd"]


    def send_DiagcCCSM(self, data):
        # type: (datatypes.DiagcForPanCenCtrl2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DiagcCCSM"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DiagcCCSM(self):
        name = "DiagcCCSM"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DiagcCCSM(self, datastring):
        # type: (str) -> datatypes.DiagcForPanCenCtrl2
        return json.loads(datastring)
    def get_DiagcCCSM(self):
        # type: () -> datatypes.DiagcForPanCenCtrl2
        return self.datamap["DiagcCCSM"]


    def send_DiagcRCSM(self, data):
        # type: (datatypes.DiagcForRCSM2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DiagcRCSM"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DiagcRCSM(self):
        name = "DiagcRCSM"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DiagcRCSM(self, datastring):
        # type: (str) -> datatypes.DiagcForRCSM2
        return json.loads(datastring)
    def get_DiagcRCSM(self):
        # type: () -> datatypes.DiagcForRCSM2
        return self.datamap["DiagcRCSM"]


    def send_DiagcRSHC(self, data):
        # type: (datatypes.DiagcForRCSM2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DiagcRSHC"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DiagcRSHC(self):
        name = "DiagcRSHC"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DiagcRSHC(self, datastring):
        # type: (str) -> datatypes.DiagcForRCSM2
        return json.loads(datastring)
    def get_DiagcRSHC(self):
        # type: () -> datatypes.DiagcForRCSM2
        return self.datamap["DiagcRSHC"]


    def send_DiagcStsDMSM(self, data):
        # type: (datatypes.DiagcStsTypDMSM) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DiagcStsDMSM"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DiagcStsDMSM(self):
        name = "DiagcStsDMSM"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DiagcStsDMSM(self, datastring):
        # type: (str) -> datatypes.DiagcStsTypDMSM
        return json.loads(datastring)
    def get_DiagcStsDMSM(self):
        # type: () -> datatypes.DiagcStsTypDMSM
        return self.datamap["DiagcStsDMSM"]


    def send_DispAndHomeBtnSts(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DispAndHomeBtnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DispAndHomeBtnSts(self):
        name = "DispAndHomeBtnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DispAndHomeBtnSts(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_DispAndHomeBtnSts(self):
        # type: () -> datatypes.Trig1
        return self.datamap["DispAndHomeBtnSts"]


    def send_DispBattEgyIn(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DispBattEgyIn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DispBattEgyIn(self):
        name = "DispBattEgyIn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DispBattEgyIn(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_DispBattEgyIn(self):
        # type: () -> (float)
        return self.datamap["DispBattEgyIn"]


    def send_DispBattEgyOut(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DispBattEgyOut"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DispBattEgyOut(self):
        name = "DispBattEgyOut"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DispBattEgyOut(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_DispBattEgyOut(self):
        # type: () -> (float)
        return self.datamap["DispBattEgyOut"]


    def send_DispOfPrpsnMod(self, data):
        # type: (datatypes.DispOfPrpsnMod3) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DispOfPrpsnMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DispOfPrpsnMod(self):
        name = "DispOfPrpsnMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DispOfPrpsnMod(self, datastring):
        # type: (str) -> datatypes.DispOfPrpsnMod3
        return json.loads(datastring)
    def get_DispOfPrpsnMod(self):
        # type: () -> datatypes.DispOfPrpsnMod3
        return self.datamap["DispOfPrpsnMod"]


    def send_DispStsCen(self, data):
        # type: (datatypes.FltCfmd1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DispStsCen"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DispStsCen(self):
        name = "DispStsCen"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DispStsCen(self, datastring):
        # type: (str) -> datatypes.FltCfmd1
        return json.loads(datastring)
    def get_DispStsCen(self):
        # type: () -> datatypes.FltCfmd1
        return self.datamap["DispStsCen"]


    def send_DoorDrvrLockReSts(self, data):
        # type: (datatypes.LockSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorDrvrLockReSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorDrvrLockReSts(self):
        name = "DoorDrvrLockReSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorDrvrLockReSts(self, datastring):
        # type: (str) -> datatypes.LockSts2
        return json.loads(datastring)
    def get_DoorDrvrLockReSts(self):
        # type: () -> datatypes.LockSts2
        return self.datamap["DoorDrvrLockReSts"]


    def send_DoorDrvrLockSts(self, data):
        # type: (datatypes.LockSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorDrvrLockSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorDrvrLockSts(self):
        name = "DoorDrvrLockSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorDrvrLockSts(self, datastring):
        # type: (str) -> datatypes.LockSts2
        return json.loads(datastring)
    def get_DoorDrvrLockSts(self):
        # type: () -> datatypes.LockSts2
        return self.datamap["DoorDrvrLockSts"]


    def send_DoorDrvrReSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorDrvrReSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorDrvrReSts(self):
        name = "DoorDrvrReSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorDrvrReSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_DoorDrvrReSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["DoorDrvrReSts"]


    def send_DoorDrvrSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorDrvrSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorDrvrSts(self):
        name = "DoorDrvrSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorDrvrSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_DoorDrvrSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["DoorDrvrSts"]


    def send_DoorPassLockReSts(self, data):
        # type: (datatypes.LockSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorPassLockReSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorPassLockReSts(self):
        name = "DoorPassLockReSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorPassLockReSts(self, datastring):
        # type: (str) -> datatypes.LockSts2
        return json.loads(datastring)
    def get_DoorPassLockReSts(self):
        # type: () -> datatypes.LockSts2
        return self.datamap["DoorPassLockReSts"]


    def send_DoorPassLockSts(self, data):
        # type: (datatypes.LockSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorPassLockSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorPassLockSts(self):
        name = "DoorPassLockSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorPassLockSts(self, datastring):
        # type: (str) -> datatypes.LockSts2
        return json.loads(datastring)
    def get_DoorPassLockSts(self):
        # type: () -> datatypes.LockSts2
        return self.datamap["DoorPassLockSts"]


    def send_DoorPassReSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorPassReSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorPassReSts(self):
        name = "DoorPassReSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorPassReSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_DoorPassReSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["DoorPassReSts"]


    def send_DoorPassSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DoorPassSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DoorPassSts(self):
        name = "DoorPassSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DoorPassSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_DoorPassSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["DoorPassSts"]


    def send_DriveAwayInfoActvReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DriveAwayInfoActvReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DriveAwayInfoActvReq(self):
        name = "DriveAwayInfoActvReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DriveAwayInfoActvReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_DriveAwayInfoActvReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["DriveAwayInfoActvReq"]


    def send_DriveAwayInfoActvSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DriveAwayInfoActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DriveAwayInfoActvSts(self):
        name = "DriveAwayInfoActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DriveAwayInfoActvSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_DriveAwayInfoActvSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["DriveAwayInfoActvSts"]


    def send_DriveAwayInfoSoundWarnActvReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DriveAwayInfoSoundWarnActvReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DriveAwayInfoSoundWarnActvReq(self):
        name = "DriveAwayInfoSoundWarnActvReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DriveAwayInfoSoundWarnActvReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_DriveAwayInfoSoundWarnActvReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["DriveAwayInfoSoundWarnActvReq"]


    def send_DriveAwayInfoWarnReq(self, data):
        # type: (datatypes.DriveAwayInfoWarnReq1) -> None
        s = toJson_DriveAwayInfoWarnReq1(data)
        name = "DriveAwayInfoWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DriveAwayInfoWarnReq(self):
        name = "DriveAwayInfoWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DriveAwayInfoWarnReq(self, datastring):
        # type: (str) -> datatypes.DriveAwayInfoWarnReq1
        return fromJson_DriveAwayInfoWarnReq1(datastring)
    def get_DriveAwayInfoWarnReq(self):
        # type: () -> (datatypes.DriveAwayInfoWarnReq1)
        return self.datamap["DriveAwayInfoWarnReq"]


    def send_DrvModDispd(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DrvModDispd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvModDispd(self):
        name = "DrvModDispd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvModDispd(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_DrvModDispd(self):
        # type: () -> (bool)
        return self.datamap["DrvModDispd"]


    def send_DrvModReq(self, data):
        # type: (datatypes.DrvModReqType1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvModReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvModReq(self):
        name = "DrvModReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvModReq(self, datastring):
        # type: (str) -> datatypes.DrvModReqType1
        return json.loads(datastring)
    def get_DrvModReq(self):
        # type: () -> datatypes.DrvModReqType1
        return self.datamap["DrvModReq"]


    def send_DrvgInWrgDirOfTrfc(self, data):
        # type: (datatypes.NoYes1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvgInWrgDirOfTrfc"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvgInWrgDirOfTrfc(self):
        name = "DrvgInWrgDirOfTrfc"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvgInWrgDirOfTrfc(self, datastring):
        # type: (str) -> datatypes.NoYes1
        return json.loads(datastring)
    def get_DrvgInWrgDirOfTrfc(self):
        # type: () -> datatypes.NoYes1
        return self.datamap["DrvgInWrgDirOfTrfc"]


    def send_DrvrCtrlOfPassSeatFrntReqd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrCtrlOfPassSeatFrntReqd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrCtrlOfPassSeatFrntReqd(self):
        name = "DrvrCtrlOfPassSeatFrntReqd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrCtrlOfPassSeatFrntReqd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_DrvrCtrlOfPassSeatFrntReqd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["DrvrCtrlOfPassSeatFrntReqd"]


    def send_DrvrCtrlOfPassSeatFrntSts(self, data):
        # type: (datatypes.ActvnAvl1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrCtrlOfPassSeatFrntSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrCtrlOfPassSeatFrntSts(self):
        name = "DrvrCtrlOfPassSeatFrntSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrCtrlOfPassSeatFrntSts(self, datastring):
        # type: (str) -> datatypes.ActvnAvl1
        return json.loads(datastring)
    def get_DrvrCtrlOfPassSeatFrntSts(self):
        # type: () -> datatypes.ActvnAvl1
        return self.datamap["DrvrCtrlOfPassSeatFrntSts"]


    def send_DrvrDesDir(self, data):
        # type: (datatypes.DrvrDesDir1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrDesDir"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrDesDir(self):
        name = "DrvrDesDir"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrDesDir(self, datastring):
        # type: (str) -> datatypes.DrvrDesDir1
        return json.loads(datastring)
    def get_DrvrDesDir(self):
        # type: () -> datatypes.DrvrDesDir1
        return self.datamap["DrvrDesDir"]


    def send_DrvrDispSetg(self, data):
        # type: (datatypes.DrvrDispSetgReq) -> None
        s = toJson_DrvrDispSetgReq(data)
        name = "DrvrDispSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrDispSetg(self):
        name = "DrvrDispSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrDispSetg(self, datastring):
        # type: (str) -> datatypes.DrvrDispSetgReq
        return fromJson_DrvrDispSetgReq(datastring)
    def get_DrvrDispSetg(self):
        # type: () -> (datatypes.DrvrDispSetgReq)
        return self.datamap["DrvrDispSetg"]


    def send_DrvrEntryLoReq(self, data):
        # type: (datatypes.NoYes1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrEntryLoReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrEntryLoReq(self):
        name = "DrvrEntryLoReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrEntryLoReq(self, datastring):
        # type: (str) -> datatypes.NoYes1
        return json.loads(datastring)
    def get_DrvrEntryLoReq(self):
        # type: () -> datatypes.NoYes1
        return self.datamap["DrvrEntryLoReq"]


    def send_DrvrHmiBackGndForHmiCen(self, data):
        # type: (datatypes.DrvrHmiForHmiCen) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DrvrHmiBackGndForHmiCen"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiBackGndForHmiCen(self):
        name = "DrvrHmiBackGndForHmiCen"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiBackGndForHmiCen(self, datastring):
        # type: (str) -> datatypes.DrvrHmiForHmiCen
        return json.loads(datastring)
    def get_DrvrHmiBackGndForHmiCen(self):
        # type: () -> datatypes.DrvrHmiForHmiCen
        return self.datamap["DrvrHmiBackGndForHmiCen"]


    def send_DrvrHmiBackGndInfoSetg(self, data):
        # type: (datatypes.DrvrHmiBackGndInfoSetg) -> None
        s = toJson_DrvrHmiBackGndInfoSetg(data)
        name = "DrvrHmiBackGndInfoSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiBackGndInfoSetg(self):
        name = "DrvrHmiBackGndInfoSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiBackGndInfoSetg(self, datastring):
        # type: (str) -> datatypes.DrvrHmiBackGndInfoSetg
        return fromJson_DrvrHmiBackGndInfoSetg(datastring)
    def get_DrvrHmiBackGndInfoSetg(self):
        # type: () -> (datatypes.DrvrHmiBackGndInfoSetg)
        return self.datamap["DrvrHmiBackGndInfoSetg"]


    def send_DrvrHmiDispdMod(self, data):
        # type: (datatypes.DrvrHmiDispdModPen) -> None
        s = toJson_DrvrHmiDispdModPen(data)
        name = "DrvrHmiDispdMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiDispdMod(self):
        name = "DrvrHmiDispdMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiDispdMod(self, datastring):
        # type: (str) -> datatypes.DrvrHmiDispdModPen
        return fromJson_DrvrHmiDispdModPen(datastring)
    def get_DrvrHmiDispdMod(self):
        # type: () -> (datatypes.DrvrHmiDispdModPen)
        return self.datamap["DrvrHmiDispdMod"]


    def send_DrvrHmiForHmiCen(self, data):
        # type: (datatypes.DrvrHmiForHmiCen) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DrvrHmiForHmiCen"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiForHmiCen(self):
        name = "DrvrHmiForHmiCen"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiForHmiCen(self, datastring):
        # type: (str) -> datatypes.DrvrHmiForHmiCen
        return json.loads(datastring)
    def get_DrvrHmiForHmiCen(self):
        # type: () -> datatypes.DrvrHmiForHmiCen
        return self.datamap["DrvrHmiForHmiCen"]


    def send_DrvrHmiSpdLimAdpnSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrHmiSpdLimAdpnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiSpdLimAdpnSts(self):
        name = "DrvrHmiSpdLimAdpnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiSpdLimAdpnSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_DrvrHmiSpdLimAdpnSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["DrvrHmiSpdLimAdpnSts"]


    def send_DrvrHmiTmrForHmiCen(self, data):
        # type: (datatypes.Array8ByteU8) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DrvrHmiTmrForHmiCen"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiTmrForHmiCen(self):
        name = "DrvrHmiTmrForHmiCen"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiTmrForHmiCen(self, datastring):
        # type: (str) -> datatypes.Array8ByteU8
        return json.loads(datastring)
    def get_DrvrHmiTmrForHmiCen(self):
        # type: () -> datatypes.Array8ByteU8
        return self.datamap["DrvrHmiTmrForHmiCen"]


    def send_DrvrHmiUsrIfSetg(self, data):
        # type: (datatypes.DrvrHmiUserIfSetgReq) -> None
        s = toJson_DrvrHmiUserIfSetgReq(data)
        name = "DrvrHmiUsrIfSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrHmiUsrIfSetg(self):
        name = "DrvrHmiUsrIfSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrHmiUsrIfSetg(self, datastring):
        # type: (str) -> datatypes.DrvrHmiUserIfSetgReq
        return fromJson_DrvrHmiUserIfSetgReq(datastring)
    def get_DrvrHmiUsrIfSetg(self):
        # type: () -> (datatypes.DrvrHmiUserIfSetgReq)
        return self.datamap["DrvrHmiUsrIfSetg"]


    def send_DrvrMassgRunng(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrMassgRunng"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrMassgRunng(self):
        name = "DrvrMassgRunng"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrMassgRunng(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_DrvrMassgRunng(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["DrvrMassgRunng"]


    def send_DrvrPfmncLvl(self, data):
        # type: (datatypes.DrvrPfmncLvl2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrPfmncLvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrPfmncLvl(self):
        name = "DrvrPfmncLvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrPfmncLvl(self, datastring):
        # type: (str) -> datatypes.DrvrPfmncLvl2
        return json.loads(datastring)
    def get_DrvrPfmncLvl(self):
        # type: () -> datatypes.DrvrPfmncLvl2
        return self.datamap["DrvrPfmncLvl"]


    def send_DrvrPfmncMonActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "DrvrPfmncMonActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrPfmncMonActv(self):
        name = "DrvrPfmncMonActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrPfmncMonActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_DrvrPfmncMonActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["DrvrPfmncMonActv"]


    def send_DrvrPfmncSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrPfmncSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrPfmncSts(self):
        name = "DrvrPfmncSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrPfmncSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_DrvrPfmncSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["DrvrPfmncSts"]


    def send_DrvrPfmncWarnReq(self, data):
        # type: (datatypes.DrvrPfmncWarnReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrPfmncWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrPfmncWarnReq(self):
        name = "DrvrPfmncWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrPfmncWarnReq(self, datastring):
        # type: (str) -> datatypes.DrvrPfmncWarnReq1
        return json.loads(datastring)
    def get_DrvrPfmncWarnReq(self):
        # type: () -> datatypes.DrvrPfmncWarnReq1
        return self.datamap["DrvrPfmncWarnReq"]


    def send_DrvrPfmncWarnReqToNav(self, data):
        # type: (datatypes.DrvrPfmncWarnReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrPfmncWarnReqToNav"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrPfmncWarnReqToNav(self):
        name = "DrvrPfmncWarnReqToNav"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrPfmncWarnReqToNav(self, datastring):
        # type: (str) -> datatypes.DrvrPfmncWarnReq1
        return json.loads(datastring)
    def get_DrvrPfmncWarnReqToNav(self):
        # type: () -> datatypes.DrvrPfmncWarnReq1
        return self.datamap["DrvrPfmncWarnReqToNav"]


    def send_DrvrSeatActvSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrSeatActvSpplFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrSeatActvSpplFct(self):
        name = "DrvrSeatActvSpplFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrSeatActvSpplFct(self, datastring):
        # type: (str) -> datatypes.SeatActvSpplFct1
        return json.loads(datastring)
    def get_DrvrSeatActvSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        return self.datamap["DrvrSeatActvSpplFct"]


    def send_DrvrSeatDispMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        s = toJson_SeatMassgFct(data)
        name = "DrvrSeatDispMassgFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrSeatDispMassgFct(self):
        name = "DrvrSeatDispMassgFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrSeatDispMassgFct(self, datastring):
        # type: (str) -> datatypes.SeatMassgFct
        return fromJson_SeatMassgFct(datastring)
    def get_DrvrSeatDispMassgFct(self):
        # type: () -> (datatypes.SeatMassgFct)
        return self.datamap["DrvrSeatDispMassgFct"]


    def send_DrvrSeatDispSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrSeatDispSpplFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrSeatDispSpplFct(self):
        name = "DrvrSeatDispSpplFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrSeatDispSpplFct(self, datastring):
        # type: (str) -> datatypes.SeatActvSpplFct1
        return json.loads(datastring)
    def get_DrvrSeatDispSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        return self.datamap["DrvrSeatDispSpplFct"]


    def send_DrvrSeatMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        s = toJson_SeatMassgFct(data)
        name = "DrvrSeatMassgFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrSeatMassgFct(self):
        name = "DrvrSeatMassgFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrSeatMassgFct(self, datastring):
        # type: (str) -> datatypes.SeatMassgFct
        return fromJson_SeatMassgFct(datastring)
    def get_DrvrSeatMassgFct(self):
        # type: () -> (datatypes.SeatMassgFct)
        return self.datamap["DrvrSeatMassgFct"]


    def send_DrvrSeatSwtSts(self, data):
        # type: (datatypes.DrvrSeatSwtSts1) -> None
        s = toJson_DrvrSeatSwtSts1(data)
        name = "DrvrSeatSwtSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrSeatSwtSts(self):
        name = "DrvrSeatSwtSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrSeatSwtSts(self, datastring):
        # type: (str) -> datatypes.DrvrSeatSwtSts1
        return fromJson_DrvrSeatSwtSts1(datastring)
    def get_DrvrSeatSwtSts(self):
        # type: () -> (datatypes.DrvrSeatSwtSts1)
        return self.datamap["DrvrSeatSwtSts"]


    def send_DrvrWLoadLvl(self, data):
        # type: (datatypes.Lvl30) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DrvrWLoadLvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DrvrWLoadLvl(self):
        name = "DrvrWLoadLvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DrvrWLoadLvl(self, datastring):
        # type: (str) -> datatypes.Lvl30
        return json.loads(datastring)
    def get_DrvrWLoadLvl(self):
        # type: () -> datatypes.Lvl30
        return self.datamap["DrvrWLoadLvl"]


    def send_DstEstimdToEmptyForDrvgElec(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstEstimdToEmptyForDrvgElec"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstEstimdToEmptyForDrvgElec(self):
        name = "DstEstimdToEmptyForDrvgElec"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstEstimdToEmptyForDrvgElec(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DstEstimdToEmptyForDrvgElec(self):
        # type: () -> (int)
        return self.datamap["DstEstimdToEmptyForDrvgElec"]


    def send_DstEstimdToEmptyForDrvgElecEco(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstEstimdToEmptyForDrvgElecEco"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstEstimdToEmptyForDrvgElecEco(self):
        name = "DstEstimdToEmptyForDrvgElecEco"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstEstimdToEmptyForDrvgElecEco(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DstEstimdToEmptyForDrvgElecEco(self):
        # type: () -> (int)
        return self.datamap["DstEstimdToEmptyForDrvgElecEco"]


    def send_DstNotifActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "DstNotifActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstNotifActv(self):
        name = "DstNotifActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstNotifActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_DstNotifActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["DstNotifActv"]


    def send_DstNotifSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "DstNotifSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstNotifSts(self):
        name = "DstNotifSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstNotifSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_DstNotifSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["DstNotifSts"]


    def send_DstToEmptyIndcd(self, data):
        # type: (datatypes.DstToEmptyWithUnit) -> None
        s = toJson_DstToEmptyWithUnit(data)
        name = "DstToEmptyIndcd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstToEmptyIndcd(self):
        name = "DstToEmptyIndcd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstToEmptyIndcd(self, datastring):
        # type: (str) -> datatypes.DstToEmptyWithUnit
        return fromJson_DstToEmptyWithUnit(datastring)
    def get_DstToEmptyIndcd(self):
        # type: () -> (datatypes.DstToEmptyWithUnit)
        return self.datamap["DstToEmptyIndcd"]


    def send_DstToManvLocn(self, data):
        # type: (datatypes.DstToManvLocnByNav) -> None
        s = toJson_DstToManvLocnByNav(data)
        name = "DstToManvLocn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstToManvLocn(self):
        name = "DstToManvLocn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstToManvLocn(self, datastring):
        # type: (str) -> datatypes.DstToManvLocnByNav
        return fromJson_DstToManvLocnByNav(datastring)
    def get_DstToManvLocn(self):
        # type: () -> (datatypes.DstToManvLocnByNav)
        return self.datamap["DstToManvLocn"]


    def send_DstToSrv(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstToSrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstToSrv(self):
        name = "DstToSrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstToSrv(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DstToSrv(self):
        # type: () -> (int)
        return self.datamap["DstToSrv"]


    def send_DstTrvld1(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstTrvld1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstTrvld1(self):
        name = "DstTrvld1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstTrvld1(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_DstTrvld1(self):
        # type: () -> (float)
        return self.datamap["DstTrvld1"]


    def send_DstTrvld2(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstTrvld2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstTrvld2(self):
        name = "DstTrvld2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstTrvld2(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_DstTrvld2(self):
        # type: () -> (float)
        return self.datamap["DstTrvld2"]


    def send_DstTrvldHiResl(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstTrvldHiResl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstTrvldHiResl(self):
        name = "DstTrvldHiResl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstTrvldHiResl(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DstTrvldHiResl(self):
        # type: () -> (int)
        return self.datamap["DstTrvldHiResl"]


    def send_BkpOfDstTrvld(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "BkpOfDstTrvld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_BkpOfDstTrvld(self):
        name = "BkpOfDstTrvld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_BkpOfDstTrvld(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_BkpOfDstTrvld(self):
        # type: () -> (int)
        return self.datamap["BkpOfDstTrvld"]


    def send_DstTrvldMst2(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "DstTrvldMst2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_DstTrvldMst2(self):
        name = "DstTrvldMst2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_DstTrvldMst2(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_DstTrvldMst2(self):
        # type: () -> (int)
        return self.datamap["DstTrvldMst2"]


    def send_EasyInOutDrvrSeatAdjmt(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "EasyInOutDrvrSeatAdjmt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EasyInOutDrvrSeatAdjmt(self):
        name = "EasyInOutDrvrSeatAdjmt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EasyInOutDrvrSeatAdjmt(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_EasyInOutDrvrSeatAdjmt(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["EasyInOutDrvrSeatAdjmt"]


    def send_EgyCostForRoute(self, data):
        # type: (datatypes.EgyCostForRouteRec1) -> None
        s = toJson_EgyCostForRouteRec1(data)
        name = "EgyCostForRoute"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EgyCostForRoute(self):
        name = "EgyCostForRoute"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EgyCostForRoute(self, datastring):
        # type: (str) -> datatypes.EgyCostForRouteRec1
        return fromJson_EgyCostForRouteRec1(datastring)
    def get_EgyCostForRoute(self):
        # type: () -> (datatypes.EgyCostForRouteRec1)
        return self.datamap["EgyCostForRoute"]


    def send_EgySave(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EgySave"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EgySave(self):
        name = "EgySave"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EgySave(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_EgySave(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["EgySave"]


    def send_EmgyAsscSts(self, data):
        # type: (datatypes.AsscSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EmgyAsscSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EmgyAsscSts(self):
        name = "EmgyAsscSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EmgyAsscSts(self, datastring):
        # type: (str) -> datatypes.AsscSts
        return json.loads(datastring)
    def get_EmgyAsscSts(self):
        # type: () -> datatypes.AsscSts
        return self.datamap["EmgyAsscSts"]


    def send_EmgyAsscStsConnGen2(self, data):
        # type: (datatypes.AsscSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EmgyAsscStsConnGen2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EmgyAsscStsConnGen2(self):
        name = "EmgyAsscStsConnGen2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EmgyAsscStsConnGen2(self, datastring):
        # type: (str) -> datatypes.AsscSts
        return json.loads(datastring)
    def get_EmgyAsscStsConnGen2(self):
        # type: () -> datatypes.AsscSts
        return self.datamap["EmgyAsscStsConnGen2"]


    def send_EmgyVehWarnActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EmgyVehWarnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EmgyVehWarnActv(self):
        name = "EmgyVehWarnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EmgyVehWarnActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_EmgyVehWarnActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["EmgyVehWarnActv"]


    def send_EmgyVehWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EmgyVehWarnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EmgyVehWarnSts(self):
        name = "EmgyVehWarnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EmgyVehWarnSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_EmgyVehWarnSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["EmgyVehWarnSts"]


    def send_EngCooltIndcnReq(self, data):
        # type: (datatypes.EngCooltIndcn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EngCooltIndcnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngCooltIndcnReq(self):
        name = "EngCooltIndcnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngCooltIndcnReq(self, datastring):
        # type: (str) -> datatypes.EngCooltIndcn
        return json.loads(datastring)
    def get_EngCooltIndcnReq(self):
        # type: () -> datatypes.EngCooltIndcn
        return self.datamap["EngCooltIndcnReq"]


    def send_EngCooltLvl(self, data):
        # type: (datatypes.FldLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EngCooltLvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngCooltLvl(self):
        name = "EngCooltLvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngCooltLvl(self, datastring):
        # type: (str) -> datatypes.FldLvl
        return json.loads(datastring)
    def get_EngCooltLvl(self):
        # type: () -> datatypes.FldLvl
        return self.datamap["EngCooltLvl"]


    def send_EngFuCns(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "EngFuCns"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngFuCns(self):
        name = "EngFuCns"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngFuCns(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_EngFuCns(self):
        # type: () -> (float)
        return self.datamap["EngFuCns"]


    def send_EngFuCnsFild(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "EngFuCnsFild"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngFuCnsFild(self):
        name = "EngFuCnsFild"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngFuCnsFild(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_EngFuCnsFild(self):
        # type: () -> (float)
        return self.datamap["EngFuCnsFild"]


    def send_EngHrToSrv(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "EngHrToSrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngHrToSrv(self):
        name = "EngHrToSrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngHrToSrv(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_EngHrToSrv(self):
        # type: () -> (int)
        return self.datamap["EngHrToSrv"]


    def send_EngIdleEco1(self, data):
        # type: (datatypes.EngIdleEco1) -> None
        s = toJson_EngIdleEco1(data)
        name = "EngIdleEco1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngIdleEco1(self):
        name = "EngIdleEco1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngIdleEco1(self, datastring):
        # type: (str) -> datatypes.EngIdleEco1
        return fromJson_EngIdleEco1(datastring)
    def get_EngIdleEco1(self):
        # type: () -> (datatypes.EngIdleEco1)
        return self.datamap["EngIdleEco1"]


    def send_EngNSafe(self, data):
        # type: (datatypes.EngNSafe) -> None
        s = toJson_EngNSafe(data)
        name = "EngNSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngNSafe(self):
        name = "EngNSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngNSafe(self, datastring):
        # type: (str) -> datatypes.EngNSafe
        return fromJson_EngNSafe(datastring)
    def get_EngNSafe(self):
        # type: () -> (datatypes.EngNSafe)
        return self.datamap["EngNSafe"]


    def send_EngOilLvl(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "EngOilLvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngOilLvl(self):
        name = "EngOilLvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngOilLvl(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_EngOilLvl(self):
        # type: () -> (float)
        return self.datamap["EngOilLvl"]


    def send_EngOilLvlSts(self, data):
        # type: (datatypes.EngOilLvlSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EngOilLvlSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngOilLvlSts(self):
        name = "EngOilLvlSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngOilLvlSts(self, datastring):
        # type: (str) -> datatypes.EngOilLvlSts1
        return json.loads(datastring)
    def get_EngOilLvlSts(self):
        # type: () -> datatypes.EngOilLvlSts1
        return self.datamap["EngOilLvlSts"]


    def send_EngOilPWarn(self, data):
        # type: (datatypes.EngOilPWarn1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EngOilPWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngOilPWarn(self):
        name = "EngOilPWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngOilPWarn(self, datastring):
        # type: (str) -> datatypes.EngOilPWarn1
        return json.loads(datastring)
    def get_EngOilPWarn(self):
        # type: () -> datatypes.EngOilPWarn1
        return self.datamap["EngOilPWarn"]


    def send_EngSpdDispd(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "EngSpdDispd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngSpdDispd(self):
        name = "EngSpdDispd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngSpdDispd(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_EngSpdDispd(self):
        # type: () -> (float)
        return self.datamap["EngSpdDispd"]


    def send_EngStrtStopDrvModIndSetg(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "EngStrtStopDrvModIndSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngStrtStopDrvModIndSetg(self):
        name = "EngStrtStopDrvModIndSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngStrtStopDrvModIndSetg(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_EngStrtStopDrvModIndSetg(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["EngStrtStopDrvModIndSetg"]


    def send_EngStrtStopSetg(self, data):
        # type: (datatypes.OnOff2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "EngStrtStopSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EngStrtStopSetg(self):
        name = "EngStrtStopSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EngStrtStopSetg(self, datastring):
        # type: (str) -> datatypes.OnOff2
        return json.loads(datastring)
    def get_EngStrtStopSetg(self):
        # type: () -> datatypes.OnOff2
        return self.datamap["EngStrtStopSetg"]


    def send_EpbApplyAutSetg(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "EpbApplyAutSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EpbApplyAutSetg(self):
        name = "EpbApplyAutSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EpbApplyAutSetg(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_EpbApplyAutSetg(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["EpbApplyAutSetg"]


    def send_EpbEngStallApplySetg(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "EpbEngStallApplySetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EpbEngStallApplySetg(self):
        name = "EpbEngStallApplySetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EpbEngStallApplySetg(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_EpbEngStallApplySetg(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["EpbEngStallApplySetg"]


    def send_EpbLampReq(self, data):
        # type: (datatypes.EpbLampReqRec) -> None
        s = toJson_EpbLampReqRec(data)
        name = "EpbLampReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EpbLampReq(self):
        name = "EpbLampReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EpbLampReq(self, datastring):
        # type: (str) -> datatypes.EpbLampReqRec
        return fromJson_EpbLampReqRec(datastring)
    def get_EpbLampReq(self):
        # type: () -> (datatypes.EpbLampReqRec)
        return self.datamap["EpbLampReq"]


    def send_EscSptModReqdByDrvr(self, data):
        # type: (datatypes.EscSptModReqdByDrvrRec1) -> None
        s = toJson_EscSptModReqdByDrvrRec1(data)
        name = "EscSptModReqdByDrvr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EscSptModReqdByDrvr(self):
        name = "EscSptModReqdByDrvr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EscSptModReqdByDrvr(self, datastring):
        # type: (str) -> datatypes.EscSptModReqdByDrvrRec1
        return fromJson_EscSptModReqdByDrvrRec1(datastring)
    def get_EscSptModReqdByDrvr(self):
        # type: () -> (datatypes.EscSptModReqdByDrvrRec1)
        return self.datamap["EscSptModReqdByDrvr"]


    def send_EscSt(self, data):
        # type: (datatypes.EscStSafe1) -> None
        s = toJson_EscStSafe1(data)
        name = "EscSt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EscSt(self):
        name = "EscSt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EscSt(self, datastring):
        # type: (str) -> datatypes.EscStSafe1
        return fromJson_EscStSafe1(datastring)
    def get_EscSt(self):
        # type: () -> (datatypes.EscStSafe1)
        return self.datamap["EscSt"]


    def send_EscWarnIndcnReq(self, data):
        # type: (datatypes.EscWarnIndcnReqRec1) -> None
        s = toJson_EscWarnIndcnReqRec1(data)
        name = "EscWarnIndcnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_EscWarnIndcnReq(self):
        name = "EscWarnIndcnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_EscWarnIndcnReq(self, datastring):
        # type: (str) -> datatypes.EscWarnIndcnReqRec1
        return fromJson_EscWarnIndcnReqRec1(datastring)
    def get_EscWarnIndcnReq(self):
        # type: () -> (datatypes.EscWarnIndcnReqRec1)
        return self.datamap["EscWarnIndcnReq"]


    def send_ExtrMirrFoldSetg(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "ExtrMirrFoldSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ExtrMirrFoldSetg(self):
        name = "ExtrMirrFoldSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ExtrMirrFoldSetg(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_ExtrMirrFoldSetg(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["ExtrMirrFoldSetg"]


    def send_ExtrMirrTiltSetg2(self, data):
        # type: (datatypes.ExtrMirrTiltSetg) -> None
        s = toJson_ExtrMirrTiltSetg(data)
        name = "ExtrMirrTiltSetg2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ExtrMirrTiltSetg2(self):
        name = "ExtrMirrTiltSetg2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ExtrMirrTiltSetg2(self, datastring):
        # type: (str) -> datatypes.ExtrMirrTiltSetg
        return fromJson_ExtrMirrTiltSetg(datastring)
    def get_ExtrMirrTiltSetg2(self):
        # type: () -> (datatypes.ExtrMirrTiltSetg)
        return self.datamap["ExtrMirrTiltSetg2"]


    def send_ExtrMirrTintgSetg(self, data):
        # type: (datatypes.MirrDimPen) -> None
        s = toJson_MirrDimPen(data)
        name = "ExtrMirrTintgSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ExtrMirrTintgSetg(self):
        name = "ExtrMirrTintgSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ExtrMirrTintgSetg(self, datastring):
        # type: (str) -> datatypes.MirrDimPen
        return fromJson_MirrDimPen(datastring)
    def get_ExtrMirrTintgSetg(self):
        # type: () -> (datatypes.MirrDimPen)
        return self.datamap["ExtrMirrTintgSetg"]


    def send_FRNetworkStatus(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "FRNetworkStatus"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FRNetworkStatus(self):
        name = "FRNetworkStatus"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FRNetworkStatus(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_FRNetworkStatus(self):
        # type: () -> (int)
        return self.datamap["FRNetworkStatus"]


    def send_FltIndcrTurnLeFrnt(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FltIndcrTurnLeFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FltIndcrTurnLeFrnt(self):
        name = "FltIndcrTurnLeFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FltIndcrTurnLeFrnt(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_FltIndcrTurnLeFrnt(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["FltIndcrTurnLeFrnt"]


    def send_FltIndcrTurnLeRe(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FltIndcrTurnLeRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FltIndcrTurnLeRe(self):
        name = "FltIndcrTurnLeRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FltIndcrTurnLeRe(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_FltIndcrTurnLeRe(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["FltIndcrTurnLeRe"]


    def send_FltIndcrTurnRiFrnt(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FltIndcrTurnRiFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FltIndcrTurnRiFrnt(self):
        name = "FltIndcrTurnRiFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FltIndcrTurnRiFrnt(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_FltIndcrTurnRiFrnt(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["FltIndcrTurnRiFrnt"]


    def send_FltIndcrTurnRiRe(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FltIndcrTurnRiRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FltIndcrTurnRiRe(self):
        name = "FltIndcrTurnRiRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FltIndcrTurnRiRe(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_FltIndcrTurnRiRe(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["FltIndcrTurnRiRe"]


    def send_FltOfLiDaytiRunngRi(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FltOfLiDaytiRunngRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FltOfLiDaytiRunngRi(self):
        name = "FltOfLiDaytiRunngRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FltOfLiDaytiRunngRi(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_FltOfLiDaytiRunngRi(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["FltOfLiDaytiRunngRi"]


    def send_FrntAxleWarn(self, data):
        # type: (datatypes.AxleWarn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FrntAxleWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FrntAxleWarn(self):
        name = "FrntAxleWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FrntAxleWarn(self, datastring):
        # type: (str) -> datatypes.AxleWarn
        return json.loads(datastring)
    def get_FrntAxleWarn(self):
        # type: () -> datatypes.AxleWarn
        return self.datamap["FrntAxleWarn"]


    def send_FrntWiprLvrReq2(self, data):
        # type: (datatypes.FrntWiprLvrReq2) -> None
        s = toJson_FrntWiprLvrReq2(data)
        name = "FrntWiprLvrReq2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FrntWiprLvrReq2(self):
        name = "FrntWiprLvrReq2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FrntWiprLvrReq2(self, datastring):
        # type: (str) -> datatypes.FrntWiprLvrReq2
        return fromJson_FrntWiprLvrReq2(datastring)
    def get_FrntWiprLvrReq2(self):
        # type: () -> (datatypes.FrntWiprLvrReq2)
        return self.datamap["FrntWiprLvrReq2"]


    def send_FuHeatrActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FuHeatrActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuHeatrActv(self):
        name = "FuHeatrActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuHeatrActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_FuHeatrActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["FuHeatrActv"]


    def send_FuHeatrFuCns1(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "FuHeatrFuCns1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuHeatrFuCns1(self):
        name = "FuHeatrFuCns1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuHeatrFuCns1(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_FuHeatrFuCns1(self):
        # type: () -> (float)
        return self.datamap["FuHeatrFuCns1"]


    def send_FuHeatrFuCnsDurgCyc1(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "FuHeatrFuCnsDurgCyc1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuHeatrFuCnsDurgCyc1(self):
        name = "FuHeatrFuCnsDurgCyc1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuHeatrFuCnsDurgCyc1(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_FuHeatrFuCnsDurgCyc1(self):
        # type: () -> (float)
        return self.datamap["FuHeatrFuCnsDurgCyc1"]


    def send_FuLvlIndcd(self, data):
        # type: (datatypes.FuLvlValWithQly) -> None
        s = toJson_FuLvlValWithQly(data)
        name = "FuLvlIndcd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuLvlIndcd(self):
        name = "FuLvlIndcd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuLvlIndcd(self, datastring):
        # type: (str) -> datatypes.FuLvlValWithQly
        return fromJson_FuLvlValWithQly(datastring)
    def get_FuLvlIndcd(self):
        # type: () -> (datatypes.FuLvlValWithQly)
        return self.datamap["FuLvlIndcd"]


    def send_FuLvlLoIndcn(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FuLvlLoIndcn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuLvlLoIndcn(self):
        name = "FuLvlLoIndcn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuLvlLoIndcn(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_FuLvlLoIndcn(self):
        # type: () -> datatypes.Trig1
        return self.datamap["FuLvlLoIndcn"]


    def send_FuLvlLoIndcnToNav(self, data):
        # type: (datatypes.FuLvlLoIndcnToNav1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FuLvlLoIndcnToNav"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuLvlLoIndcnToNav(self):
        name = "FuLvlLoIndcnToNav"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuLvlLoIndcnToNav(self, datastring):
        # type: (str) -> datatypes.FuLvlLoIndcnToNav1
        return json.loads(datastring)
    def get_FuLvlLoIndcnToNav(self):
        # type: () -> datatypes.FuLvlLoIndcnToNav1
        return self.datamap["FuLvlLoIndcnToNav"]


    def send_FuLvlLoWarn(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "FuLvlLoWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_FuLvlLoWarn(self):
        name = "FuLvlLoWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_FuLvlLoWarn(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_FuLvlLoWarn(self):
        # type: () -> datatypes.Trig1
        return self.datamap["FuLvlLoWarn"]


    def send_GearIndcnRec(self, data):
        # type: (datatypes.GearIndcnRec2) -> None
        s = toJson_GearIndcnRec2(data)
        name = "GearIndcnRec"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_GearIndcnRec(self):
        name = "GearIndcnRec"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_GearIndcnRec(self, datastring):
        # type: (str) -> datatypes.GearIndcnRec2
        return fromJson_GearIndcnRec2(datastring)
    def get_GearIndcnRec(self):
        # type: () -> (datatypes.GearIndcnRec2)
        return self.datamap["GearIndcnRec"]


    def send_GlbRstForSetgAndData(self, data):
        # type: (datatypes.GlbRstForSetgAndData) -> None
        s = toJson_GlbRstForSetgAndData(data)
        name = "GlbRstForSetgAndData"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_GlbRstForSetgAndData(self):
        name = "GlbRstForSetgAndData"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_GlbRstForSetgAndData(self, datastring):
        # type: (str) -> datatypes.GlbRstForSetgAndData
        return fromJson_GlbRstForSetgAndData(datastring)
    def get_GlbRstForSetgAndData(self):
        # type: () -> (datatypes.GlbRstForSetgAndData)
        return self.datamap["GlbRstForSetgAndData"]


    def send_HdHwAprvdWirelsAdr(self, data):
        # type: (datatypes.HwAprvdWirelsAdr1) -> None
        s = toJson_HwAprvdWirelsAdr1(data)
        name = "HdHwAprvdWirelsAdr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HdHwAprvdWirelsAdr(self):
        name = "HdHwAprvdWirelsAdr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HdHwAprvdWirelsAdr(self, datastring):
        # type: (str) -> datatypes.HwAprvdWirelsAdr1
        return fromJson_HwAprvdWirelsAdr1(datastring)
    def get_HdHwAprvdWirelsAdr(self):
        # type: () -> (datatypes.HwAprvdWirelsAdr1)
        return self.datamap["HdHwAprvdWirelsAdr"]


    def send_HdrestFoldReq2(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "HdrestFoldReq2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HdrestFoldReq2(self):
        name = "HdrestFoldReq2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HdrestFoldReq2(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_HdrestFoldReq2(self):
        # type: () -> (bool)
        return self.datamap["HdrestFoldReq2"]


    def send_HeatrDurgDrvgReqd(self, data):
        # type: (datatypes.OffOnAut1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HeatrDurgDrvgReqd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HeatrDurgDrvgReqd(self):
        name = "HeatrDurgDrvgReqd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HeatrDurgDrvgReqd(self, datastring):
        # type: (str) -> datatypes.OffOnAut1
        return json.loads(datastring)
    def get_HeatrDurgDrvgReqd(self):
        # type: () -> datatypes.OffOnAut1
        return self.datamap["HeatrDurgDrvgReqd"]


    def send_HeatrPreCdngTyp(self, data):
        # type: (datatypes.HeatrPreCdngTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HeatrPreCdngTyp"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HeatrPreCdngTyp(self):
        name = "HeatrPreCdngTyp"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HeatrPreCdngTyp(self, datastring):
        # type: (str) -> datatypes.HeatrPreCdngTyp
        return json.loads(datastring)
    def get_HeatrPreCdngTyp(self):
        # type: () -> datatypes.HeatrPreCdngTyp
        return self.datamap["HeatrPreCdngTyp"]


    def send_HmiAudSts(self, data):
        # type: (datatypes.FltCfmd1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiAudSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiAudSts(self):
        name = "HmiAudSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiAudSts(self, datastring):
        # type: (str) -> datatypes.FltCfmd1
        return json.loads(datastring)
    def get_HmiAudSts(self):
        # type: () -> datatypes.FltCfmd1
        return self.datamap["HmiAudSts"]


    def send_HmiCenForDrvrHmi(self, data):
        # type: (datatypes.HmiCenForDrvrHmi) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "HmiCenForDrvrHmi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiCenForDrvrHmi(self):
        name = "HmiCenForDrvrHmi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiCenForDrvrHmi(self, datastring):
        # type: (str) -> datatypes.HmiCenForDrvrHmi
        return json.loads(datastring)
    def get_HmiCenForDrvrHmi(self):
        # type: () -> datatypes.HmiCenForDrvrHmi
        return self.datamap["HmiCenForDrvrHmi"]


    def send_HmiCmptmtAirDistbnFrnt(self, data):
        # type: (datatypes.HmiCmptmtAirDistbnFrnt) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiCmptmtAirDistbnFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiCmptmtAirDistbnFrnt(self):
        name = "HmiCmptmtAirDistbnFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiCmptmtAirDistbnFrnt(self, datastring):
        # type: (str) -> datatypes.HmiCmptmtAirDistbnFrnt
        return json.loads(datastring)
    def get_HmiCmptmtAirDistbnFrnt(self):
        # type: () -> datatypes.HmiCmptmtAirDistbnFrnt
        return self.datamap["HmiCmptmtAirDistbnFrnt"]


    def send_HmiCmptmtCoolgReq(self, data):
        # type: (datatypes.HmiCmptmtCoolgReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiCmptmtCoolgReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiCmptmtCoolgReq(self):
        name = "HmiCmptmtCoolgReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiCmptmtCoolgReq(self, datastring):
        # type: (str) -> datatypes.HmiCmptmtCoolgReq
        return json.loads(datastring)
    def get_HmiCmptmtCoolgReq(self):
        # type: () -> datatypes.HmiCmptmtCoolgReq
        return self.datamap["HmiCmptmtCoolgReq"]


    def send_HmiCmptmtTSp(self, data):
        # type: (datatypes.HmiCmptmtTSp) -> None
        s = toJson_HmiCmptmtTSp(data)
        name = "HmiCmptmtTSp"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiCmptmtTSp(self):
        name = "HmiCmptmtTSp"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiCmptmtTSp(self, datastring):
        # type: (str) -> datatypes.HmiCmptmtTSp
        return fromJson_HmiCmptmtTSp(datastring)
    def get_HmiCmptmtTSp(self):
        # type: () -> (datatypes.HmiCmptmtTSp)
        return self.datamap["HmiCmptmtTSp"]


    def send_HmiDefrstElecReq(self, data):
        # type: (datatypes.HmiDefrstElecReq) -> None
        s = toJson_HmiDefrstElecReq(data)
        name = "HmiDefrstElecReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiDefrstElecReq(self):
        name = "HmiDefrstElecReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiDefrstElecReq(self, datastring):
        # type: (str) -> datatypes.HmiDefrstElecReq
        return fromJson_HmiDefrstElecReq(datastring)
    def get_HmiDefrstElecReq(self):
        # type: () -> (datatypes.HmiDefrstElecReq)
        return self.datamap["HmiDefrstElecReq"]


    def send_HmiDefrstElecSts(self, data):
        # type: (datatypes.HmiDefrstElecSts) -> None
        s = toJson_HmiDefrstElecSts(data)
        name = "HmiDefrstElecSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiDefrstElecSts(self):
        name = "HmiDefrstElecSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiDefrstElecSts(self, datastring):
        # type: (str) -> datatypes.HmiDefrstElecSts
        return fromJson_HmiDefrstElecSts(datastring)
    def get_HmiDefrstElecSts(self):
        # type: () -> (datatypes.HmiDefrstElecSts)
        return self.datamap["HmiDefrstElecSts"]


    def send_HmiDefrstMaxReq(self, data):
        # type: (datatypes.ActrReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiDefrstMaxReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiDefrstMaxReq(self):
        name = "HmiDefrstMaxReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiDefrstMaxReq(self, datastring):
        # type: (str) -> datatypes.ActrReq
        return json.loads(datastring)
    def get_HmiDefrstMaxReq(self):
        # type: () -> datatypes.ActrReq
        return self.datamap["HmiDefrstMaxReq"]


    def send_HmiHvacFanLvlFrnt(self, data):
        # type: (datatypes.HmiHvacFanLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiHvacFanLvlFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiHvacFanLvlFrnt(self):
        name = "HmiHvacFanLvlFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiHvacFanLvlFrnt(self, datastring):
        # type: (str) -> datatypes.HmiHvacFanLvl
        return json.loads(datastring)
    def get_HmiHvacFanLvlFrnt(self):
        # type: () -> datatypes.HmiHvacFanLvl
        return self.datamap["HmiHvacFanLvlFrnt"]


    def send_HmiHvacFanLvlRe(self, data):
        # type: (datatypes.HmiHvacFanLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiHvacFanLvlRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiHvacFanLvlRe(self):
        name = "HmiHvacFanLvlRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiHvacFanLvlRe(self, datastring):
        # type: (str) -> datatypes.HmiHvacFanLvl
        return json.loads(datastring)
    def get_HmiHvacFanLvlRe(self):
        # type: () -> datatypes.HmiHvacFanLvl
        return self.datamap["HmiHvacFanLvlRe"]


    def send_HmiHvacReCtrl(self, data):
        # type: (datatypes.HmiHvacReCtrl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiHvacReCtrl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiHvacReCtrl(self):
        name = "HmiHvacReCtrl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiHvacReCtrl(self, datastring):
        # type: (str) -> datatypes.HmiHvacReCtrl
        return json.loads(datastring)
    def get_HmiHvacReCtrl(self):
        # type: () -> datatypes.HmiHvacReCtrl
        return self.datamap["HmiHvacReCtrl"]


    def send_HmiHvacRecircCmd(self, data):
        # type: (datatypes.HmiHvacRecircCmd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HmiHvacRecircCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiHvacRecircCmd(self):
        name = "HmiHvacRecircCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiHvacRecircCmd(self, datastring):
        # type: (str) -> datatypes.HmiHvacRecircCmd
        return json.loads(datastring)
    def get_HmiHvacRecircCmd(self):
        # type: () -> datatypes.HmiHvacRecircCmd
        return self.datamap["HmiHvacRecircCmd"]


    def send_HmiSeatClima(self, data):
        # type: (datatypes.HmiSeatClima) -> None
        s = toJson_HmiSeatClima(data)
        name = "HmiSeatClima"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiSeatClima(self):
        name = "HmiSeatClima"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiSeatClima(self, datastring):
        # type: (str) -> datatypes.HmiSeatClima
        return fromJson_HmiSeatClima(datastring)
    def get_HmiSeatClima(self):
        # type: () -> (datatypes.HmiSeatClima)
        return self.datamap["HmiSeatClima"]


    def send_HmiSeatClimaExtd(self, data):
        # type: (datatypes.HmiSeatClimaExtd) -> None
        s = toJson_HmiSeatClimaExtd(data)
        name = "HmiSeatClimaExtd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HmiSeatClimaExtd(self):
        name = "HmiSeatClimaExtd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HmiSeatClimaExtd(self, datastring):
        # type: (str) -> datatypes.HmiSeatClimaExtd
        return fromJson_HmiSeatClimaExtd(datastring)
    def get_HmiSeatClimaExtd(self):
        # type: () -> (datatypes.HmiSeatClimaExtd)
        return self.datamap["HmiSeatClimaExtd"]


    def send_HoodSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HoodSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HoodSts(self):
        name = "HoodSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HoodSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_HoodSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["HoodSts"]


    def send_HptcWarnSeldForCllsnFwdWarn(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "HptcWarnSeldForCllsnFwdWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HptcWarnSeldForCllsnFwdWarn(self):
        name = "HptcWarnSeldForCllsnFwdWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HptcWarnSeldForCllsnFwdWarn(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_HptcWarnSeldForCllsnFwdWarn(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["HptcWarnSeldForCllsnFwdWarn"]


    def send_HudActvReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "HudActvReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudActvReq(self):
        name = "HudActvReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudActvReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_HudActvReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["HudActvReq"]


    def send_HudActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HudActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudActvSts(self):
        name = "HudActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_HudActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["HudActvSts"]


    def send_HudAdjmtReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HudAdjmtReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudAdjmtReq(self):
        name = "HudAdjmtReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudAdjmtReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_HudAdjmtReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["HudAdjmtReq"]


    def send_HudDiagc(self, data):
        # type: (datatypes.HudDiagc) -> None
        s = toJson_HudDiagc(data)
        name = "HudDiagc"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudDiagc(self):
        name = "HudDiagc"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudDiagc(self, datastring):
        # type: (str) -> datatypes.HudDiagc
        return fromJson_HudDiagc(datastring)
    def get_HudDiagc(self):
        # type: () -> (datatypes.HudDiagc)
        return self.datamap["HudDiagc"]


    def send_HudErgoSetgReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HudErgoSetgReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudErgoSetgReq(self):
        name = "HudErgoSetgReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudErgoSetgReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_HudErgoSetgReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["HudErgoSetgReq"]


    def send_HudSts(self, data):
        # type: (datatypes.HudStsForHmi) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HudSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudSts(self):
        name = "HudSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudSts(self, datastring):
        # type: (str) -> datatypes.HudStsForHmi
        return json.loads(datastring)
    def get_HudSts(self):
        # type: () -> datatypes.HudStsForHmi
        return self.datamap["HudSts"]


    def send_HudVisFctSetg(self, data):
        # type: (datatypes.HudVisFctSetgReq) -> None
        s = toJson_HudVisFctSetgReq(data)
        name = "HudVisFctSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HudVisFctSetg(self):
        name = "HudVisFctSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HudVisFctSetg(self, datastring):
        # type: (str) -> datatypes.HudVisFctSetgReq
        return fromJson_HudVisFctSetgReq(datastring)
    def get_HudVisFctSetg(self):
        # type: () -> (datatypes.HudVisFctSetgReq)
        return self.datamap["HudVisFctSetg"]


    def send_HvBattEgyAvlDcha3(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "HvBattEgyAvlDcha3"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HvBattEgyAvlDcha3(self):
        name = "HvBattEgyAvlDcha3"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HvBattEgyAvlDcha3(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_HvBattEgyAvlDcha3(self):
        # type: () -> (float)
        return self.datamap["HvBattEgyAvlDcha3"]


    def send_HvBattSmtActv(self, data):
        # type: (datatypes.Flg1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HvBattSmtActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HvBattSmtActv(self):
        name = "HvBattSmtActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HvBattSmtActv(self, datastring):
        # type: (str) -> datatypes.Flg1
        return json.loads(datastring)
    def get_HvBattSmtActv(self):
        # type: () -> datatypes.Flg1
        return self.datamap["HvBattSmtActv"]


    def send_HvBattSmtSeld(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HvBattSmtSeld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HvBattSmtSeld(self):
        name = "HvBattSmtSeld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HvBattSmtSeld(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_HvBattSmtSeld(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["HvBattSmtSeld"]


    def send_HvacAirMFlowEstimd(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "HvacAirMFlowEstimd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HvacAirMFlowEstimd(self):
        name = "HvacAirMFlowEstimd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HvacAirMFlowEstimd(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_HvacAirMFlowEstimd(self):
        # type: () -> (int)
        return self.datamap["HvacAirMFlowEstimd"]


    def send_HznData(self, data):
        # type: (datatypes.HznDataGroup2) -> None
        s = toJson_HznDataGroup2(data)
        name = "HznData"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznData(self):
        name = "HznData"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznData(self, datastring):
        # type: (str) -> datatypes.HznDataGroup2
        return fromJson_HznDataGroup2(datastring)
    def get_HznData(self):
        # type: () -> (datatypes.HznDataGroup2)
        return self.datamap["HznData"]


    def send_HznEdge(self, data):
        # type: (datatypes.HznEdgeGroup2) -> None
        s = toJson_HznEdgeGroup2(data)
        name = "HznEdge"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznEdge(self):
        name = "HznEdge"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznEdge(self, datastring):
        # type: (str) -> datatypes.HznEdgeGroup2
        return fromJson_HznEdgeGroup2(datastring)
    def get_HznEdge(self):
        # type: () -> (datatypes.HznEdgeGroup2)
        return self.datamap["HznEdge"]


    def send_HznPosn(self, data):
        # type: (datatypes.HznPosnGroup3) -> None
        s = toJson_HznPosnGroup3(data)
        name = "HznPosn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznPosn(self):
        name = "HznPosn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznPosn(self, datastring):
        # type: (str) -> datatypes.HznPosnGroup3
        return fromJson_HznPosnGroup3(datastring)
    def get_HznPosn(self):
        # type: () -> (datatypes.HznPosnGroup3)
        return self.datamap["HznPosn"]


    def send_HznPosnExtd(self, data):
        # type: (datatypes.HznPosnExtdGroup1) -> None
        s = toJson_HznPosnExtdGroup1(data)
        name = "HznPosnExtd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznPosnExtd(self):
        name = "HznPosnExtd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznPosnExtd(self, datastring):
        # type: (str) -> datatypes.HznPosnExtdGroup1
        return fromJson_HznPosnExtdGroup1(datastring)
    def get_HznPosnExtd(self):
        # type: () -> (datatypes.HznPosnExtdGroup1)
        return self.datamap["HznPosnExtd"]


    def send_HznPosnExtdOffs(self, data):
        # type: (datatypes.HznPosnExtdOffs) -> None
        s = toJson_HznPosnExtdOffs(data)
        name = "HznPosnExtdOffs"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznPosnExtdOffs(self):
        name = "HznPosnExtdOffs"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznPosnExtdOffs(self, datastring):
        # type: (str) -> datatypes.HznPosnExtdOffs
        return fromJson_HznPosnExtdOffs(datastring)
    def get_HznPosnExtdOffs(self):
        # type: () -> (datatypes.HznPosnExtdOffs)
        return self.datamap["HznPosnExtdOffs"]


    def send_HznProfLong(self, data):
        # type: (datatypes.HznProfLongGroup3) -> None
        s = toJson_HznProfLongGroup3(data)
        name = "HznProfLong"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznProfLong(self):
        name = "HznProfLong"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznProfLong(self, datastring):
        # type: (str) -> datatypes.HznProfLongGroup3
        return fromJson_HznProfLongGroup3(datastring)
    def get_HznProfLong(self):
        # type: () -> (datatypes.HznProfLongGroup3)
        return self.datamap["HznProfLong"]


    def send_HznProfLongExtd(self, data):
        # type: (datatypes.HznProfLongExtdGroup1) -> None
        s = toJson_HznProfLongExtdGroup1(data)
        name = "HznProfLongExtd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznProfLongExtd(self):
        name = "HznProfLongExtd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznProfLongExtd(self, datastring):
        # type: (str) -> datatypes.HznProfLongExtdGroup1
        return fromJson_HznProfLongExtdGroup1(datastring)
    def get_HznProfLongExtd(self):
        # type: () -> (datatypes.HznProfLongExtdGroup1)
        return self.datamap["HznProfLongExtd"]


    def send_HznProfSho(self, data):
        # type: (datatypes.HznProfSho2) -> None
        s = toJson_HznProfSho2(data)
        name = "HznProfSho"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznProfSho(self):
        name = "HznProfSho"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznProfSho(self, datastring):
        # type: (str) -> datatypes.HznProfSho2
        return fromJson_HznProfSho2(datastring)
    def get_HznProfSho(self):
        # type: () -> (datatypes.HznProfSho2)
        return self.datamap["HznProfSho"]


    def send_HznRstExtd(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "HznRstExtd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznRstExtd(self):
        name = "HznRstExtd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznRstExtd(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_HznRstExtd(self):
        # type: () -> (bool)
        return self.datamap["HznRstExtd"]


    def send_HznSeg(self, data):
        # type: (datatypes.HznSegGroup2) -> None
        s = toJson_HznSegGroup2(data)
        name = "HznSeg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznSeg(self):
        name = "HznSeg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznSeg(self, datastring):
        # type: (str) -> datatypes.HznSegGroup2
        return fromJson_HznSegGroup2(datastring)
    def get_HznSeg(self):
        # type: () -> (datatypes.HznSegGroup2)
        return self.datamap["HznSeg"]


    def send_HznSplyElectcSts(self, data):
        # type: (datatypes.HznSplyElectcSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HznSplyElectcSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HznSplyElectcSts(self):
        name = "HznSplyElectcSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HznSplyElectcSts(self, datastring):
        # type: (str) -> datatypes.HznSplyElectcSts1
        return json.loads(datastring)
    def get_HznSplyElectcSts(self):
        # type: () -> datatypes.HznSplyElectcSts1
        return self.datamap["HznSplyElectcSts"]


    def send_HzrdLiWarnActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HzrdLiWarnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HzrdLiWarnActv(self):
        name = "HzrdLiWarnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HzrdLiWarnActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_HzrdLiWarnActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["HzrdLiWarnActv"]


    def send_HzrdLiWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "HzrdLiWarnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_HzrdLiWarnSts(self):
        name = "HzrdLiWarnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_HzrdLiWarnSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_HzrdLiWarnSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["HzrdLiWarnSts"]


    def send_IndcnOfParkAssiSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "IndcnOfParkAssiSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IndcnOfParkAssiSts(self):
        name = "IndcnOfParkAssiSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IndcnOfParkAssiSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_IndcnOfParkAssiSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["IndcnOfParkAssiSts"]


    def send_IndcnOfPrkgAutSts(self, data):
        # type: (datatypes.IndOfPrkgAutSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "IndcnOfPrkgAutSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IndcnOfPrkgAutSts(self):
        name = "IndcnOfPrkgAutSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IndcnOfPrkgAutSts(self, datastring):
        # type: (str) -> datatypes.IndOfPrkgAutSts
        return json.loads(datastring)
    def get_IndcnOfPrkgAutSts(self):
        # type: () -> datatypes.IndOfPrkgAutSts
        return self.datamap["IndcnOfPrkgAutSts"]


    def send_IndcnUnit(self, data):
        # type: (datatypes.IndcnUnit) -> None
        s = toJson_IndcnUnit(data)
        name = "IndcnUnit"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IndcnUnit(self):
        name = "IndcnUnit"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IndcnUnit(self, datastring):
        # type: (str) -> datatypes.IndcnUnit
        return fromJson_IndcnUnit(datastring)
    def get_IndcnUnit(self):
        # type: () -> (datatypes.IndcnUnit)
        return self.datamap["IndcnUnit"]


    def send_IndcrDisp1WdSts(self, data):
        # type: (datatypes.IndcrSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "IndcrDisp1WdSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IndcrDisp1WdSts(self):
        name = "IndcrDisp1WdSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IndcrDisp1WdSts(self, datastring):
        # type: (str) -> datatypes.IndcrSts1
        return json.loads(datastring)
    def get_IndcrDisp1WdSts(self):
        # type: () -> datatypes.IndcrSts1
        return self.datamap["IndcrDisp1WdSts"]


    def send_IndcrTurnSts1WdSts(self, data):
        # type: (datatypes.IndcrSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "IndcrTurnSts1WdSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IndcrTurnSts1WdSts(self):
        name = "IndcrTurnSts1WdSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IndcrTurnSts1WdSts(self, datastring):
        # type: (str) -> datatypes.IndcrSts1
        return json.loads(datastring)
    def get_IndcrTurnSts1WdSts(self):
        # type: () -> datatypes.IndcrSts1
        return self.datamap["IndcrTurnSts1WdSts"]


    def send_IniValSigCfgIDBackboneFR(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "IniValSigCfgIDBackboneFR"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IniValSigCfgIDBackboneFR(self):
        name = "IniValSigCfgIDBackboneFR"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IniValSigCfgIDBackboneFR(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_IniValSigCfgIDBackboneFR(self):
        # type: () -> (int)
        return self.datamap["IniValSigCfgIDBackboneFR"]


    def send_InsdCarNoiseMeasd(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "InsdCarNoiseMeasd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_InsdCarNoiseMeasd(self):
        name = "InsdCarNoiseMeasd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_InsdCarNoiseMeasd(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_InsdCarNoiseMeasd(self):
        # type: () -> (float)
        return self.datamap["InsdCarNoiseMeasd"]


    def send_IntAudCnclWarn(self, data):
        # type: (datatypes.YesNo2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "IntAudCnclWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntAudCnclWarn(self):
        name = "IntAudCnclWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntAudCnclWarn(self, datastring):
        # type: (str) -> datatypes.YesNo2
        return json.loads(datastring)
    def get_IntAudCnclWarn(self):
        # type: () -> datatypes.YesNo2
        return self.datamap["IntAudCnclWarn"]


    def send_IntrBriSts(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "IntrBriSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntrBriSts(self):
        name = "IntrBriSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntrBriSts(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_IntrBriSts(self):
        # type: () -> (int)
        return self.datamap["IntrBriSts"]


    def send_IntrBriStsForSeatHeatrRe(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "IntrBriStsForSeatHeatrRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntrBriStsForSeatHeatrRe(self):
        name = "IntrBriStsForSeatHeatrRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntrBriStsForSeatHeatrRe(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_IntrBriStsForSeatHeatrRe(self):
        # type: () -> (int)
        return self.datamap["IntrBriStsForSeatHeatrRe"]


    def send_IntrLiAmbLiSetg(self, data):
        # type: (datatypes.IntrLiAmbLiSetg) -> None
        s = toJson_IntrLiAmbLiSetg(data)
        name = "IntrLiAmbLiSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntrLiAmbLiSetg(self):
        name = "IntrLiAmbLiSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntrLiAmbLiSetg(self, datastring):
        # type: (str) -> datatypes.IntrLiAmbLiSetg
        return fromJson_IntrLiAmbLiSetg(datastring)
    def get_IntrLiAmbLiSetg(self):
        # type: () -> (datatypes.IntrLiAmbLiSetg)
        return self.datamap["IntrLiAmbLiSetg"]


    def send_IntrLiSurrndgsLiSetgLi(self, data):
        # type: (datatypes.IntrLiSurrndgsLiSetg) -> None
        s = toJson_IntrLiSurrndgsLiSetg(data)
        name = "IntrLiSurrndgsLiSetgLi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntrLiSurrndgsLiSetgLi(self):
        name = "IntrLiSurrndgsLiSetgLi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntrLiSurrndgsLiSetgLi(self, datastring):
        # type: (str) -> datatypes.IntrLiSurrndgsLiSetg
        return fromJson_IntrLiSurrndgsLiSetg(datastring)
    def get_IntrLiSurrndgsLiSetgLi(self):
        # type: () -> (datatypes.IntrLiSurrndgsLiSetg)
        return self.datamap["IntrLiSurrndgsLiSetgLi"]


    def send_IntrMirrTintgSetg(self, data):
        # type: (datatypes.MirrDimPen) -> None
        s = toJson_MirrDimPen(data)
        name = "IntrMirrTintgSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntrMirrTintgSetg(self):
        name = "IntrMirrTintgSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntrMirrTintgSetg(self, datastring):
        # type: (str) -> datatypes.MirrDimPen
        return fromJson_MirrDimPen(datastring)
    def get_IntrMirrTintgSetg(self):
        # type: () -> (datatypes.MirrDimPen)
        return self.datamap["IntrMirrTintgSetg"]


    def send_IntvAndWarnModForLaneKeepAid(self, data):
        # type: (datatypes.WarnAndIntvPen1) -> None
        s = toJson_WarnAndIntvPen1(data)
        name = "IntvAndWarnModForLaneKeepAid"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_IntvAndWarnModForLaneKeepAid(self):
        name = "IntvAndWarnModForLaneKeepAid"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_IntvAndWarnModForLaneKeepAid(self, datastring):
        # type: (str) -> datatypes.WarnAndIntvPen1
        return fromJson_WarnAndIntvPen1(datastring)
    def get_IntvAndWarnModForLaneKeepAid(self):
        # type: () -> (datatypes.WarnAndIntvPen1)
        return self.datamap["IntvAndWarnModForLaneKeepAid"]


    def send_KeyLostWarnIndcn(self, data):
        # type: (datatypes.KeyLostWarnIndcn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "KeyLostWarnIndcn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_KeyLostWarnIndcn(self):
        name = "KeyLostWarnIndcn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_KeyLostWarnIndcn(self, datastring):
        # type: (str) -> datatypes.KeyLostWarnIndcn
        return json.loads(datastring)
    def get_KeyLostWarnIndcn(self):
        # type: () -> datatypes.KeyLostWarnIndcn
        return self.datamap["KeyLostWarnIndcn"]


    def send_KeyProfMpgUpd(self, data):
        # type: (datatypes.KeyProfMpgUpd1) -> None
        s = toJson_KeyProfMpgUpd1(data)
        name = "KeyProfMpgUpd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_KeyProfMpgUpd(self):
        name = "KeyProfMpgUpd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_KeyProfMpgUpd(self, datastring):
        # type: (str) -> datatypes.KeyProfMpgUpd1
        return fromJson_KeyProfMpgUpd1(datastring)
    def get_KeyProfMpgUpd(self):
        # type: () -> (datatypes.KeyProfMpgUpd1)
        return self.datamap["KeyProfMpgUpd"]


    def send_KeyReadReqFromSetgMgr(self, data):
        # type: (datatypes.KeyLocn1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "KeyReadReqFromSetgMgr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_KeyReadReqFromSetgMgr(self):
        name = "KeyReadReqFromSetgMgr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_KeyReadReqFromSetgMgr(self, datastring):
        # type: (str) -> datatypes.KeyLocn1
        return json.loads(datastring)
    def get_KeyReadReqFromSetgMgr(self):
        # type: () -> datatypes.KeyLocn1
        return self.datamap["KeyReadReqFromSetgMgr"]


    def send_KeyReadStsToProfCtrl(self, data):
        # type: (datatypes.KeyReadStsToProfCtrl) -> None
        s = toJson_KeyReadStsToProfCtrl(data)
        name = "KeyReadStsToProfCtrl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_KeyReadStsToProfCtrl(self):
        name = "KeyReadStsToProfCtrl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_KeyReadStsToProfCtrl(self, datastring):
        # type: (str) -> datatypes.KeyReadStsToProfCtrl
        return fromJson_KeyReadStsToProfCtrl(datastring)
    def get_KeyReadStsToProfCtrl(self):
        # type: () -> (datatypes.KeyReadStsToProfCtrl)
        return self.datamap["KeyReadStsToProfCtrl"]


    def send_KeyRmnIndcn(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "KeyRmnIndcn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_KeyRmnIndcn(self):
        name = "KeyRmnIndcn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_KeyRmnIndcn(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_KeyRmnIndcn(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["KeyRmnIndcn"]


    def send_KeySpdWarn(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "KeySpdWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_KeySpdWarn(self):
        name = "KeySpdWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_KeySpdWarn(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_KeySpdWarn(self):
        # type: () -> datatypes.Trig1
        return self.datamap["KeySpdWarn"]


    def send_LampSuppSrv(self, data):
        # type: (datatypes.SrvSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LampSuppSrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LampSuppSrv(self):
        name = "LampSuppSrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LampSuppSrv(self, datastring):
        # type: (str) -> datatypes.SrvSts
        return json.loads(datastring)
    def get_LampSuppSrv(self):
        # type: () -> datatypes.SrvSts
        return self.datamap["LampSuppSrv"]


    def send_LaneChgWarnActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LaneChgWarnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneChgWarnActv(self):
        name = "LaneChgWarnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneChgWarnActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LaneChgWarnActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LaneChgWarnActv"]


    def send_LaneChgWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LaneChgWarnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneChgWarnSts(self):
        name = "LaneChgWarnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneChgWarnSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_LaneChgWarnSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["LaneChgWarnSts"]


    def send_LaneDetnStsForAutDrv(self, data):
        # type: (datatypes.LaneDetnSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LaneDetnStsForAutDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneDetnStsForAutDrv(self):
        name = "LaneDetnStsForAutDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneDetnStsForAutDrv(self, datastring):
        # type: (str) -> datatypes.LaneDetnSts1
        return json.loads(datastring)
    def get_LaneDetnStsForAutDrv(self):
        # type: () -> datatypes.LaneDetnSts1
        return self.datamap["LaneDetnStsForAutDrv"]


    def send_LaneDetnStsForLaneKeepAid(self, data):
        # type: (datatypes.LaneDetnSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LaneDetnStsForLaneKeepAid"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneDetnStsForLaneKeepAid(self):
        name = "LaneDetnStsForLaneKeepAid"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneDetnStsForLaneKeepAid(self, datastring):
        # type: (str) -> datatypes.LaneDetnSts1
        return json.loads(datastring)
    def get_LaneDetnStsForLaneKeepAid(self):
        # type: () -> datatypes.LaneDetnSts1
        return self.datamap["LaneDetnStsForLaneKeepAid"]


    def send_LaneKeepAidActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LaneKeepAidActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneKeepAidActv(self):
        name = "LaneKeepAidActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneKeepAidActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LaneKeepAidActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LaneKeepAidActv"]


    def send_LaneKeepAidRoadEdgeActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LaneKeepAidRoadEdgeActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneKeepAidRoadEdgeActv(self):
        name = "LaneKeepAidRoadEdgeActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneKeepAidRoadEdgeActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LaneKeepAidRoadEdgeActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LaneKeepAidRoadEdgeActv"]


    def send_LaneKeepAidSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LaneKeepAidSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LaneKeepAidSts(self):
        name = "LaneKeepAidSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LaneKeepAidSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_LaneKeepAidSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["LaneKeepAidSts"]


    def send_LatMovmtWarn(self, data):
        # type: (datatypes.WarnLeRi1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LatMovmtWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LatMovmtWarn(self):
        name = "LatMovmtWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LatMovmtWarn(self, datastring):
        # type: (str) -> datatypes.WarnLeRi1
        return json.loads(datastring)
    def get_LatMovmtWarn(self):
        # type: () -> datatypes.WarnLeRi1
        return self.datamap["LatMovmtWarn"]


    def send_LcmaOn1(self, data):
        # type: (datatypes.LcmaCtraOn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LcmaOn1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LcmaOn1(self):
        name = "LcmaOn1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LcmaOn1(self, datastring):
        # type: (str) -> datatypes.LcmaCtraOn
        return json.loads(datastring)
    def get_LcmaOn1(self):
        # type: () -> datatypes.LcmaCtraOn
        return self.datamap["LcmaOn1"]


    def send_LiExtReq1WdReq1(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiExtReq1WdReq1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq1WdReq1(self):
        name = "LiExtReq1WdReq1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq1WdReq1(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiExtReq1WdReq1(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiExtReq1WdReq1"]


    def send_LiExtReq2WdReq1(self, data):
        # type: (datatypes.LiAutTranPen1) -> None
        s = toJson_LiAutTranPen1(data)
        name = "LiExtReq2WdReq1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq2WdReq1(self):
        name = "LiExtReq2WdReq1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq2WdReq1(self, datastring):
        # type: (str) -> datatypes.LiAutTranPen1
        return fromJson_LiAutTranPen1(datastring)
    def get_LiExtReq2WdReq1(self):
        # type: () -> (datatypes.LiAutTranPen1)
        return self.datamap["LiExtReq2WdReq1"]


    def send_LiExtReq1WdReq4(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiExtReq1WdReq4"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq1WdReq4(self):
        name = "LiExtReq1WdReq4"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq1WdReq4(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiExtReq1WdReq4(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiExtReq1WdReq4"]


    def send_LiExtReq1WdReq6(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiExtReq1WdReq6"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq1WdReq6(self):
        name = "LiExtReq1WdReq6"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq1WdReq6(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiExtReq1WdReq6(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiExtReq1WdReq6"]


    def send_LiExtReq1WdReq2(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiExtReq1WdReq2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq1WdReq2(self):
        name = "LiExtReq1WdReq2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq1WdReq2(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiExtReq1WdReq2(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiExtReq1WdReq2"]


    def send_LiDrvrFltIndcrTurn(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LiDrvrFltIndcrTurn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiDrvrFltIndcrTurn(self):
        name = "LiDrvrFltIndcrTurn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiDrvrFltIndcrTurn(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_LiDrvrFltIndcrTurn(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["LiDrvrFltIndcrTurn"]


    def send_LiForBtn4ForUsrSwtPanFrntCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LiForBtn4ForUsrSwtPanFrntCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiForBtn4ForUsrSwtPanFrntCmd(self):
        name = "LiForBtn4ForUsrSwtPanFrntCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiForBtn4ForUsrSwtPanFrntCmd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_LiForBtn4ForUsrSwtPanFrntCmd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["LiForBtn4ForUsrSwtPanFrntCmd"]


    def send_LiForBtn5ForUsrSwtPanFrntCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LiForBtn5ForUsrSwtPanFrntCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiForBtn5ForUsrSwtPanFrntCmd(self):
        name = "LiForBtn5ForUsrSwtPanFrntCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiForBtn5ForUsrSwtPanFrntCmd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_LiForBtn5ForUsrSwtPanFrntCmd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["LiForBtn5ForUsrSwtPanFrntCmd"]


    def send_LiExtSafe1WdReq2(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiExtSafe1WdReq2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtSafe1WdReq2(self):
        name = "LiExtSafe1WdReq2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtSafe1WdReq2(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiExtSafe1WdReq2(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiExtSafe1WdReq2"]


    def send_LiExtSafe1WdReq1(self, data):
        # type: (datatypes.LiTiPen2) -> None
        s = toJson_LiTiPen2(data)
        name = "LiExtSafe1WdReq1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtSafe1WdReq1(self):
        name = "LiExtSafe1WdReq1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtSafe1WdReq1(self, datastring):
        # type: (str) -> datatypes.LiTiPen2
        return fromJson_LiTiPen2(datastring)
    def get_LiExtSafe1WdReq1(self):
        # type: () -> (datatypes.LiTiPen2)
        return self.datamap["LiExtSafe1WdReq1"]


    def send_LiLvrSwt1Req(self, data):
        # type: (datatypes.SwtPush) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LiLvrSwt1Req"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiLvrSwt1Req(self):
        name = "LiLvrSwt1Req"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiLvrSwt1Req(self, datastring):
        # type: (str) -> datatypes.SwtPush
        return json.loads(datastring)
    def get_LiLvrSwt1Req(self):
        # type: () -> datatypes.SwtPush
        return self.datamap["LiLvrSwt1Req"]


    def send_LiPassFltIndcrTurn(self, data):
        # type: (datatypes.DevErrSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LiPassFltIndcrTurn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiPassFltIndcrTurn(self):
        name = "LiPassFltIndcrTurn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiPassFltIndcrTurn(self, datastring):
        # type: (str) -> datatypes.DevErrSts2
        return json.loads(datastring)
    def get_LiPassFltIndcrTurn(self):
        # type: () -> datatypes.DevErrSts2
        return self.datamap["LiPassFltIndcrTurn"]


    def send_LiSeldForDrvrPfmncMon(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiSeldForDrvrPfmncMon"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiSeldForDrvrPfmncMon(self):
        name = "LiSeldForDrvrPfmncMon"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiSeldForDrvrPfmncMon(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiSeldForDrvrPfmncMon(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiSeldForDrvrPfmncMon"]


    def send_LiTrfcSide1WdReq1(self, data):
        # type: (datatypes.LiTrfcSide1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LiTrfcSide1WdReq1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiTrfcSide1WdReq1(self):
        name = "LiTrfcSide1WdReq1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiTrfcSide1WdReq1(self, datastring):
        # type: (str) -> datatypes.LiTrfcSide1
        return json.loads(datastring)
    def get_LiTrfcSide1WdReq1(self):
        # type: () -> datatypes.LiTrfcSide1
        return self.datamap["LiTrfcSide1WdReq1"]


    def send_ListOfNodAv(self, data):
        # type: (datatypes.ListOfNodAv) -> None
        s = toJson_ListOfNodAv(data)
        name = "ListOfNodAv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ListOfNodAv(self):
        name = "ListOfNodAv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ListOfNodAv(self, datastring):
        # type: (str) -> datatypes.ListOfNodAv
        return fromJson_ListOfNodAv(datastring)
    def get_ListOfNodAv(self):
        # type: () -> (datatypes.ListOfNodAv)
        return self.datamap["ListOfNodAv"]


    def send_LockSpdReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LockSpdReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockSpdReq(self):
        name = "LockSpdReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockSpdReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LockSpdReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LockSpdReq"]


    def send_LockgCenSts(self, data):
        # type: (datatypes.LockgCenSts3) -> None
        s = toJson_LockgCenSts3(data)
        name = "LockgCenSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockgCenSts(self):
        name = "LockgCenSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockgCenSts(self, datastring):
        # type: (str) -> datatypes.LockgCenSts3
        return fromJson_LockgCenSts3(datastring)
    def get_LockgCenSts(self):
        # type: () -> (datatypes.LockgCenSts3)
        return self.datamap["LockgCenSts"]


    def send_LockgCenStsForUsrFb(self, data):
        # type: (datatypes.LockSt2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LockgCenStsForUsrFb"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockgCenStsForUsrFb(self):
        name = "LockgCenStsForUsrFb"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockgCenStsForUsrFb(self, datastring):
        # type: (str) -> datatypes.LockSt2
        return json.loads(datastring)
    def get_LockgCenStsForUsrFb(self):
        # type: () -> datatypes.LockSt2
        return self.datamap["LockgCenStsForUsrFb"]


    def send_LockgFbSoundReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LockgFbSoundReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockgFbSoundReq(self):
        name = "LockgFbSoundReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockgFbSoundReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LockgFbSoundReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LockgFbSoundReq"]


    def send_LockgFbVisReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LockgFbVisReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockgFbVisReq(self):
        name = "LockgFbVisReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockgFbVisReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LockgFbVisReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LockgFbVisReq"]


    def send_LockgPrsnlReqFromHmi(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LockgPrsnlReqFromHmi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockgPrsnlReqFromHmi(self):
        name = "LockgPrsnlReqFromHmi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockgPrsnlReqFromHmi(self, datastring):
        # type: (str) -> datatypes.OnOffNoReq
        return json.loads(datastring)
    def get_LockgPrsnlReqFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        return self.datamap["LockgPrsnlReqFromHmi"]


    def send_LockgPrsnlSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LockgPrsnlSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LockgPrsnlSts(self):
        name = "LockgPrsnlSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LockgPrsnlSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_LockgPrsnlSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["LockgPrsnlSts"]


    def send_LvlOfClimaCmft(self, data):
        # type: (datatypes.LvlOfClimaCmft) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "LvlOfClimaCmft"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LvlOfClimaCmft(self):
        name = "LvlOfClimaCmft"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LvlOfClimaCmft(self, datastring):
        # type: (str) -> datatypes.LvlOfClimaCmft
        return json.loads(datastring)
    def get_LvlOfClimaCmft(self):
        # type: () -> datatypes.LvlOfClimaCmft
        return self.datamap["LvlOfClimaCmft"]


    def send_LvlSeldForSpdLimAdpv(self, data):
        # type: (datatypes.LvlSeldForSpdLimAdpvPen1) -> None
        s = toJson_LvlSeldForSpdLimAdpvPen1(data)
        name = "LvlSeldForSpdLimAdpv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LvlSeldForSpdLimAdpv(self):
        name = "LvlSeldForSpdLimAdpv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LvlSeldForSpdLimAdpv(self, datastring):
        # type: (str) -> datatypes.LvlSeldForSpdLimAdpvPen1
        return fromJson_LvlSeldForSpdLimAdpvPen1(datastring)
    def get_LvlSeldForSpdLimAdpv(self):
        # type: () -> (datatypes.LvlSeldForSpdLimAdpvPen1)
        return self.datamap["LvlSeldForSpdLimAdpv"]


    def send_MassgFctActv(self, data):
        # type: (datatypes.MassgFctActv) -> None
        s = toJson_MassgFctActv(data)
        name = "MassgFctActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MassgFctActv(self):
        name = "MassgFctActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MassgFctActv(self, datastring):
        # type: (str) -> datatypes.MassgFctActv
        return fromJson_MassgFctActv(datastring)
    def get_MassgFctActv(self):
        # type: () -> (datatypes.MassgFctActv)
        return self.datamap["MassgFctActv"]


    def send_MemBtnSound(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "MemBtnSound"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MemBtnSound(self):
        name = "MemBtnSound"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MemBtnSound(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_MemBtnSound(self):
        # type: () -> (bool)
        return self.datamap["MemBtnSound"]


    def send_MirrDwnStsAtDrvr(self, data):
        # type: (datatypes.MirrDwnStsTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MirrDwnStsAtDrvr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MirrDwnStsAtDrvr(self):
        name = "MirrDwnStsAtDrvr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MirrDwnStsAtDrvr(self, datastring):
        # type: (str) -> datatypes.MirrDwnStsTyp
        return json.loads(datastring)
    def get_MirrDwnStsAtDrvr(self):
        # type: () -> datatypes.MirrDwnStsTyp
        return self.datamap["MirrDwnStsAtDrvr"]


    def send_MirrDwnStsAtPass(self, data):
        # type: (datatypes.MirrDwnStsTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MirrDwnStsAtPass"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MirrDwnStsAtPass(self):
        name = "MirrDwnStsAtPass"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MirrDwnStsAtPass(self, datastring):
        # type: (str) -> datatypes.MirrDwnStsTyp
        return json.loads(datastring)
    def get_MirrDwnStsAtPass(self):
        # type: () -> datatypes.MirrDwnStsTyp
        return self.datamap["MirrDwnStsAtPass"]


    def send_MirrFoldStsAtDrvr(self, data):
        # type: (datatypes.MirrFoldStsTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MirrFoldStsAtDrvr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MirrFoldStsAtDrvr(self):
        name = "MirrFoldStsAtDrvr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MirrFoldStsAtDrvr(self, datastring):
        # type: (str) -> datatypes.MirrFoldStsTyp
        return json.loads(datastring)
    def get_MirrFoldStsAtDrvr(self):
        # type: () -> datatypes.MirrFoldStsTyp
        return self.datamap["MirrFoldStsAtDrvr"]


    def send_MirrFoldStsAtPass(self, data):
        # type: (datatypes.MirrFoldStsTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MirrFoldStsAtPass"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MirrFoldStsAtPass(self):
        name = "MirrFoldStsAtPass"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MirrFoldStsAtPass(self, datastring):
        # type: (str) -> datatypes.MirrFoldStsTyp
        return json.loads(datastring)
    def get_MirrFoldStsAtPass(self):
        # type: () -> datatypes.MirrFoldStsTyp
        return self.datamap["MirrFoldStsAtPass"]


    def send_MmedHdPwrMod(self, data):
        # type: (datatypes.MmedMaiPwrMod) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MmedHdPwrMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MmedHdPwrMod(self):
        name = "MmedHdPwrMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MmedHdPwrMod(self, datastring):
        # type: (str) -> datatypes.MmedMaiPwrMod
        return json.loads(datastring)
    def get_MmedHdPwrMod(self):
        # type: () -> datatypes.MmedMaiPwrMod
        return self.datamap["MmedHdPwrMod"]


    def send_MmedHmiModStd(self, data):
        # type: (datatypes.MmedHmiModStd2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MmedHmiModStd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MmedHmiModStd(self):
        name = "MmedHmiModStd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MmedHmiModStd(self, datastring):
        # type: (str) -> datatypes.MmedHmiModStd2
        return json.loads(datastring)
    def get_MmedHmiModStd(self):
        # type: () -> datatypes.MmedHmiModStd2
        return self.datamap["MmedHmiModStd"]


    def send_MmedTvmPwerMod(self, data):
        # type: (datatypes.MmedTvmPwerMod) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "MmedTvmPwerMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MmedTvmPwerMod(self):
        name = "MmedTvmPwerMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MmedTvmPwerMod(self, datastring):
        # type: (str) -> datatypes.MmedTvmPwerMod
        return json.loads(datastring)
    def get_MmedTvmPwerMod(self):
        # type: () -> datatypes.MmedTvmPwerMod
        return self.datamap["MmedTvmPwerMod"]


    def send_MstCfgIDBackboneFR(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "MstCfgIDBackboneFR"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MstCfgIDBackboneFR(self):
        name = "MstCfgIDBackboneFR"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MstCfgIDBackboneFR(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_MstCfgIDBackboneFR(self):
        # type: () -> (int)
        return self.datamap["MstCfgIDBackboneFR"]


    def send_MtrlSnsrT(self, data):
        # type: (datatypes.MtrlSnsrT) -> None
        s = toJson_MtrlSnsrT(data)
        name = "MtrlSnsrT"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_MtrlSnsrT(self):
        name = "MtrlSnsrT"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_MtrlSnsrT(self, datastring):
        # type: (str) -> datatypes.MtrlSnsrT
        return fromJson_MtrlSnsrT(datastring)
    def get_MtrlSnsrT(self):
        # type: () -> (datatypes.MtrlSnsrT)
        return self.datamap["MtrlSnsrT"]


    def send_NFSDataFront(self, data):
        # type: (datatypes.SnsrPrkgAssi3) -> None
        s = toJson_SnsrPrkgAssi3(data)
        name = "NFSDataFront"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NFSDataFront(self):
        name = "NFSDataFront"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NFSDataFront(self, datastring):
        # type: (str) -> datatypes.SnsrPrkgAssi3
        return fromJson_SnsrPrkgAssi3(datastring)
    def get_NFSDataFront(self):
        # type: () -> (datatypes.SnsrPrkgAssi3)
        return self.datamap["NFSDataFront"]


    def send_NFSDataRear(self, data):
        # type: (datatypes.SnsrPrkgAssi3) -> None
        s = toJson_SnsrPrkgAssi3(data)
        name = "NFSDataRear"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NFSDataRear(self):
        name = "NFSDataRear"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NFSDataRear(self, datastring):
        # type: (str) -> datatypes.SnsrPrkgAssi3
        return fromJson_SnsrPrkgAssi3(datastring)
    def get_NFSDataRear(self):
        # type: () -> (datatypes.SnsrPrkgAssi3)
        return self.datamap["NFSDataRear"]


    def send_NetCtrlrActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        s = toJson_NetActvtRec1(data)
        name = "NetCtrlrActvt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NetCtrlrActvt(self):
        name = "NetCtrlrActvt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NetCtrlrActvt(self, datastring):
        # type: (str) -> datatypes.NetActvtRec1
        return fromJson_NetActvtRec1(datastring)
    def get_NetCtrlrActvt(self):
        # type: () -> (datatypes.NetActvtRec1)
        return self.datamap["NetCtrlrActvt"]


    def send_NetHdActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        s = toJson_NetActvtRec1(data)
        name = "NetHdActvt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NetHdActvt(self):
        name = "NetHdActvt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NetHdActvt(self, datastring):
        # type: (str) -> datatypes.NetActvtRec1
        return fromJson_NetActvtRec1(datastring)
    def get_NetHdActvt(self):
        # type: () -> (datatypes.NetActvtRec1)
        return self.datamap["NetHdActvt"]


    def send_NetTelmActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        s = toJson_NetActvtRec1(data)
        name = "NetTelmActvt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NetTelmActvt(self):
        name = "NetTelmActvt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NetTelmActvt(self, datastring):
        # type: (str) -> datatypes.NetActvtRec1
        return fromJson_NetActvtRec1(datastring)
    def get_NetTelmActvt(self):
        # type: () -> (datatypes.NetActvtRec1)
        return self.datamap["NetTelmActvt"]


    def send_NewTripCdn(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "NewTripCdn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NewTripCdn(self):
        name = "NewTripCdn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NewTripCdn(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_NewTripCdn(self):
        # type: () -> datatypes.Trig1
        return self.datamap["NewTripCdn"]


    def send_NoEntryWarnReq(self, data):
        # type: (datatypes.Warn2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "NoEntryWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NoEntryWarnReq(self):
        name = "NoEntryWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NoEntryWarnReq(self, datastring):
        # type: (str) -> datatypes.Warn2
        return json.loads(datastring)
    def get_NoEntryWarnReq(self):
        # type: () -> datatypes.Warn2
        return self.datamap["NoEntryWarnReq"]


    def send_NoSoundSys(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "NoSoundSys"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NoSoundSys(self):
        name = "NoSoundSys"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NoSoundSys(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_NoSoundSys(self):
        # type: () -> (bool)
        return self.datamap["NoSoundSys"]


    def send_NotifChkDistbn(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "NotifChkDistbn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NotifChkDistbn(self):
        name = "NotifChkDistbn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NotifChkDistbn(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_NotifChkDistbn(self):
        # type: () -> (int)
        return self.datamap["NotifChkDistbn"]


    def send_NrOfBltAppld(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "NrOfBltAppld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NrOfBltAppld(self):
        name = "NrOfBltAppld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NrOfBltAppld(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_NrOfBltAppld(self):
        # type: () -> (int)
        return self.datamap["NrOfBltAppld"]


    def send_NrOfKeyAvl(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "NrOfKeyAvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NrOfKeyAvl(self):
        name = "NrOfKeyAvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NrOfKeyAvl(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_NrOfKeyAvl(self):
        # type: () -> (bool)
        return self.datamap["NrOfKeyAvl"]


    def send_NrSerlDMSM(self, data):
        # type: (datatypes.NrSerlNodLIN) -> None
        s = toJson_NrSerlNodLIN(data)
        name = "NrSerlDMSM"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_NrSerlDMSM(self):
        name = "NrSerlDMSM"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_NrSerlDMSM(self, datastring):
        # type: (str) -> datatypes.NrSerlNodLIN
        return fromJson_NrSerlNodLIN(datastring)
    def get_NrSerlDMSM(self):
        # type: () -> (datatypes.NrSerlNodLIN)
        return self.datamap["NrSerlDMSM"]


    def send_OffsForDrvrSpprtFctActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "OffsForDrvrSpprtFctActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_OffsForDrvrSpprtFctActvSts(self):
        name = "OffsForDrvrSpprtFctActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_OffsForDrvrSpprtFctActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_OffsForDrvrSpprtFctActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["OffsForDrvrSpprtFctActvSts"]


    def send_OffsForSpdWarnSetg(self, data):
        # type: (datatypes.OffsForSpdWarnSetgPen) -> None
        s = toJson_OffsForSpdWarnSetgPen(data)
        name = "OffsForSpdWarnSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_OffsForSpdWarnSetg(self):
        name = "OffsForSpdWarnSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_OffsForSpdWarnSetg(self, datastring):
        # type: (str) -> datatypes.OffsForSpdWarnSetgPen
        return fromJson_OffsForSpdWarnSetgPen(datastring)
    def get_OffsForSpdWarnSetg(self):
        # type: () -> (datatypes.OffsForSpdWarnSetgPen)
        return self.datamap["OffsForSpdWarnSetg"]


    def send_PartNrDMSM(self, data):
        # type: (datatypes.PartNrNodLIN) -> None
        s = toJson_PartNrNodLIN(data)
        name = "PartNrDMSM"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PartNrDMSM(self):
        name = "PartNrDMSM"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PartNrDMSM(self, datastring):
        # type: (str) -> datatypes.PartNrNodLIN
        return fromJson_PartNrNodLIN(datastring)
    def get_PartNrDMSM(self):
        # type: () -> (datatypes.PartNrNodLIN)
        return self.datamap["PartNrDMSM"]


    def send_PasAlrmDeactvnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PasAlrmDeactvnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PasAlrmDeactvnReq(self):
        name = "PasAlrmDeactvnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PasAlrmDeactvnReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PasAlrmDeactvnReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PasAlrmDeactvnReq"]


    def send_PasAlrmSts(self, data):
        # type: (datatypes.NoReqOffReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PasAlrmSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PasAlrmSts(self):
        name = "PasAlrmSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PasAlrmSts(self, datastring):
        # type: (str) -> datatypes.NoReqOffReq
        return json.loads(datastring)
    def get_PasAlrmSts(self):
        # type: () -> datatypes.NoReqOffReq
        return self.datamap["PasAlrmSts"]


    def send_PassMassgRunng(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PassMassgRunng"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassMassgRunng(self):
        name = "PassMassgRunng"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassMassgRunng(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PassMassgRunng(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PassMassgRunng"]


    def send_PassSeatActvSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PassSeatActvSpplFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassSeatActvSpplFct(self):
        name = "PassSeatActvSpplFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassSeatActvSpplFct(self, datastring):
        # type: (str) -> datatypes.SeatActvSpplFct1
        return json.loads(datastring)
    def get_PassSeatActvSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        return self.datamap["PassSeatActvSpplFct"]


    def send_PassSeatDispMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        s = toJson_SeatMassgFct(data)
        name = "PassSeatDispMassgFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassSeatDispMassgFct(self):
        name = "PassSeatDispMassgFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassSeatDispMassgFct(self, datastring):
        # type: (str) -> datatypes.SeatMassgFct
        return fromJson_SeatMassgFct(datastring)
    def get_PassSeatDispMassgFct(self):
        # type: () -> (datatypes.SeatMassgFct)
        return self.datamap["PassSeatDispMassgFct"]


    def send_PassSeatDispSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PassSeatDispSpplFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassSeatDispSpplFct(self):
        name = "PassSeatDispSpplFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassSeatDispSpplFct(self, datastring):
        # type: (str) -> datatypes.SeatActvSpplFct1
        return json.loads(datastring)
    def get_PassSeatDispSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        return self.datamap["PassSeatDispSpplFct"]


    def send_PassSeatMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        s = toJson_SeatMassgFct(data)
        name = "PassSeatMassgFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassSeatMassgFct(self):
        name = "PassSeatMassgFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassSeatMassgFct(self, datastring):
        # type: (str) -> datatypes.SeatMassgFct
        return fromJson_SeatMassgFct(datastring)
    def get_PassSeatMassgFct(self):
        # type: () -> (datatypes.SeatMassgFct)
        return self.datamap["PassSeatMassgFct"]


    def send_PassSeatSts(self, data):
        # type: (datatypes.PassSeatSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PassSeatSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassSeatSts(self):
        name = "PassSeatSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassSeatSts(self, datastring):
        # type: (str) -> datatypes.PassSeatSts1
        return json.loads(datastring)
    def get_PassSeatSts(self):
        # type: () -> datatypes.PassSeatSts1
        return self.datamap["PassSeatSts"]


    def send_PassSeatSwtSts2(self, data):
        # type: (datatypes.PassSeatSwtSts2) -> None
        s = toJson_PassSeatSwtSts2(data)
        name = "PassSeatSwtSts2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PassSeatSwtSts2(self):
        name = "PassSeatSwtSts2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PassSeatSwtSts2(self, datastring):
        # type: (str) -> datatypes.PassSeatSwtSts2
        return fromJson_PassSeatSwtSts2(datastring)
    def get_PassSeatSwtSts2(self):
        # type: () -> (datatypes.PassSeatSwtSts2)
        return self.datamap["PassSeatSwtSts2"]


    def send_PinionSteerAg1(self, data):
        # type: (datatypes.PinionSteerAg1Rec) -> None
        s = toJson_PinionSteerAg1Rec(data)
        name = "PinionSteerAg1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PinionSteerAg1(self):
        name = "PinionSteerAg1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PinionSteerAg1(self, datastring):
        # type: (str) -> datatypes.PinionSteerAg1Rec
        return fromJson_PinionSteerAg1Rec(datastring)
    def get_PinionSteerAg1(self):
        # type: () -> (datatypes.PinionSteerAg1Rec)
        return self.datamap["PinionSteerAg1"]


    def send_PosnFromNav(self, data):
        # type: (datatypes.PosnFromNav) -> None
        s = toJson_PosnFromNav(data)
        name = "PosnFromNav"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PosnFromNav(self):
        name = "PosnFromNav"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PosnFromNav(self, datastring):
        # type: (str) -> datatypes.PosnFromNav
        return fromJson_PosnFromNav(datastring)
    def get_PosnFromNav(self):
        # type: () -> (datatypes.PosnFromNav)
        return self.datamap["PosnFromNav"]


    def send_PosnFromSatlt(self, data):
        # type: (datatypes.PosnFromSatlt) -> None
        s = toJson_PosnFromSatlt(data)
        name = "PosnFromSatlt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PosnFromSatlt(self):
        name = "PosnFromSatlt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PosnFromSatlt(self, datastring):
        # type: (str) -> datatypes.PosnFromSatlt
        return fromJson_PosnFromSatlt(datastring)
    def get_PosnFromSatlt(self):
        # type: () -> (datatypes.PosnFromSatlt)
        return self.datamap["PosnFromSatlt"]


    def send_PostDrvgClimaAvl(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PostDrvgClimaAvl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PostDrvgClimaAvl(self):
        name = "PostDrvgClimaAvl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PostDrvgClimaAvl(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PostDrvgClimaAvl(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PostDrvgClimaAvl"]


    def send_PostDrvgClimaReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PostDrvgClimaReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PostDrvgClimaReq(self):
        name = "PostDrvgClimaReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PostDrvgClimaReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PostDrvgClimaReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PostDrvgClimaReq"]


    def send_PreClngNotif(self, data):
        # type: (datatypes.PreClngNotif) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PreClngNotif"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PreClngNotif(self):
        name = "PreClngNotif"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PreClngNotif(self, datastring):
        # type: (str) -> datatypes.PreClngNotif
        return json.loads(datastring)
    def get_PreClngNotif(self):
        # type: () -> datatypes.PreClngNotif
        return self.datamap["PreClngNotif"]


    def send_PrkgAssiActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgAssiActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiActvSts(self):
        name = "PrkgAssiActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PrkgAssiActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PrkgAssiActvSts"]


    def send_PrkgAssiAudWarnFront(self, data):
        # type: (datatypes.PrkgAssiAudWarn) -> None
        s = toJson_PrkgAssiAudWarn(data)
        name = "PrkgAssiAudWarnFront"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiAudWarnFront(self):
        name = "PrkgAssiAudWarnFront"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiAudWarnFront(self, datastring):
        # type: (str) -> datatypes.PrkgAssiAudWarn
        return fromJson_PrkgAssiAudWarn(datastring)
    def get_PrkgAssiAudWarnFront(self):
        # type: () -> (datatypes.PrkgAssiAudWarn)
        return self.datamap["PrkgAssiAudWarnFront"]


    def send_PrkgAssiAudWarnRear(self, data):
        # type: (datatypes.PrkgAssiAudWarn) -> None
        s = toJson_PrkgAssiAudWarn(data)
        name = "PrkgAssiAudWarnRear"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiAudWarnRear(self):
        name = "PrkgAssiAudWarnRear"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiAudWarnRear(self, datastring):
        # type: (str) -> datatypes.PrkgAssiAudWarn
        return fromJson_PrkgAssiAudWarn(datastring)
    def get_PrkgAssiAudWarnRear(self):
        # type: () -> (datatypes.PrkgAssiAudWarn)
        return self.datamap["PrkgAssiAudWarnRear"]


    def send_PrkgAssiFailr(self, data):
        # type: (datatypes.PrkgAssiFailr2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgAssiFailr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiFailr(self):
        name = "PrkgAssiFailr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiFailr(self, datastring):
        # type: (str) -> datatypes.PrkgAssiFailr2
        return json.loads(datastring)
    def get_PrkgAssiFailr(self):
        # type: () -> datatypes.PrkgAssiFailr2
        return self.datamap["PrkgAssiFailr"]


    def send_PrkgAssiManvActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgAssiManvActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiManvActvSts(self):
        name = "PrkgAssiManvActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiManvActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PrkgAssiManvActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PrkgAssiManvActvSts"]


    def send_PrkgAssiManvProgs(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PrkgAssiManvProgs"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiManvProgs(self):
        name = "PrkgAssiManvProgs"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiManvProgs(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_PrkgAssiManvProgs(self):
        # type: () -> (int)
        return self.datamap["PrkgAssiManvProgs"]


    def send_PrkgAssiSts(self, data):
        # type: (datatypes.PrkgAssiSts3) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgAssiSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAssiSts(self):
        name = "PrkgAssiSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAssiSts(self, datastring):
        # type: (str) -> datatypes.PrkgAssiSts3
        return json.loads(datastring)
    def get_PrkgAssiSts(self):
        # type: () -> datatypes.PrkgAssiSts3
        return self.datamap["PrkgAssiSts"]


    def send_PrkgAutSts(self, data):
        # type: (datatypes.PrkgAutSts3) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgAutSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgAutSts(self):
        name = "PrkgAutSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgAutSts(self, datastring):
        # type: (str) -> datatypes.PrkgAutSts3
        return json.loads(datastring)
    def get_PrkgAutSts(self):
        # type: () -> datatypes.PrkgAutSts3
        return self.datamap["PrkgAutSts"]


    def send_PrkgCamSysAvlSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgCamSysAvlSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgCamSysAvlSts(self):
        name = "PrkgCamSysAvlSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgCamSysAvlSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_PrkgCamSysAvlSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["PrkgCamSysAvlSts"]


    def send_PrkgOutCfm(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgOutCfm"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgOutCfm(self):
        name = "PrkgOutCfm"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgOutCfm(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_PrkgOutCfm(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["PrkgOutCfm"]


    def send_PrkgTypVld(self, data):
        # type: (datatypes.PrkgTypVld1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrkgTypVld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrkgTypVld(self):
        name = "PrkgTypVld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrkgTypVld(self, datastring):
        # type: (str) -> datatypes.PrkgTypVld1
        return json.loads(datastring)
    def get_PrkgTypVld(self):
        # type: () -> datatypes.PrkgTypVld1
        return self.datamap["PrkgTypVld"]


    def send_ProfAct(self, data):
        # type: (datatypes.ProfAct1) -> None
        s = toJson_ProfAct1(data)
        name = "ProfAct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ProfAct(self):
        name = "ProfAct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ProfAct(self, datastring):
        # type: (str) -> datatypes.ProfAct1
        return fromJson_ProfAct1(datastring)
    def get_ProfAct(self):
        # type: () -> (datatypes.ProfAct1)
        return self.datamap["ProfAct"]


    def send_ProfChg(self, data):
        # type: (datatypes.IdPen) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ProfChg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ProfChg(self):
        name = "ProfChg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ProfChg(self, datastring):
        # type: (str) -> datatypes.IdPen
        return json.loads(datastring)
    def get_ProfChg(self):
        # type: () -> datatypes.IdPen
        return self.datamap["ProfChg"]


    def send_ProfLimd(self, data):
        # type: (datatypes.ProfLimd1) -> None
        s = toJson_ProfLimd1(data)
        name = "ProfLimd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ProfLimd(self):
        name = "ProfLimd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ProfLimd(self, datastring):
        # type: (str) -> datatypes.ProfLimd1
        return fromJson_ProfLimd1(datastring)
    def get_ProfLimd(self):
        # type: () -> (datatypes.ProfLimd1)
        return self.datamap["ProfLimd"]


    def send_ProfPenSts1(self, data):
        # type: (datatypes.IdPen) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ProfPenSts1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ProfPenSts1(self):
        name = "ProfPenSts1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ProfPenSts1(self, datastring):
        # type: (str) -> datatypes.IdPen
        return json.loads(datastring)
    def get_ProfPenSts1(self):
        # type: () -> datatypes.IdPen
        return self.datamap["ProfPenSts1"]


    def send_PrpsnDrvMod(self, data):
        # type: (datatypes.PrpsnDrvMod) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnDrvMod"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnDrvMod(self):
        name = "PrpsnDrvMod"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnDrvMod(self, datastring):
        # type: (str) -> datatypes.PrpsnDrvMod
        return json.loads(datastring)
    def get_PrpsnDrvMod(self):
        # type: () -> datatypes.PrpsnDrvMod
        return self.datamap["PrpsnDrvMod"]


    def send_PrpsnHvBattUsgModAct(self, data):
        # type: (datatypes.HvBattUsgType) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnHvBattUsgModAct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgModAct(self):
        name = "PrpsnHvBattUsgModAct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgModAct(self, datastring):
        # type: (str) -> datatypes.HvBattUsgType
        return json.loads(datastring)
    def get_PrpsnHvBattUsgModAct(self):
        # type: () -> datatypes.HvBattUsgType
        return self.datamap["PrpsnHvBattUsgModAct"]


    def send_PrpsnHvBattUsgModReq(self, data):
        # type: (datatypes.HvBattUsgType) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnHvBattUsgModReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgModReq(self):
        name = "PrpsnHvBattUsgModReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgModReq(self, datastring):
        # type: (str) -> datatypes.HvBattUsgType
        return json.loads(datastring)
    def get_PrpsnHvBattUsgModReq(self):
        # type: () -> datatypes.HvBattUsgType
        return self.datamap["PrpsnHvBattUsgModReq"]


    def send_PrpsnHvBattUsgOfChrgBlkd(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PrpsnHvBattUsgOfChrgBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfChrgBlkd(self):
        name = "PrpsnHvBattUsgOfChrgBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfChrgBlkd(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfChrgBlkd(self):
        # type: () -> (bool)
        return self.datamap["PrpsnHvBattUsgOfChrgBlkd"]


    def send_PrpsnHvBattUsgOfChrgBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnHvBattUsgOfChrgBlkd2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfChrgBlkd2(self):
        name = "PrpsnHvBattUsgOfChrgBlkd2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfChrgBlkd2(self, datastring):
        # type: (str) -> datatypes.HvBattUsgBlkdTyp
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfChrgBlkd2(self):
        # type: () -> datatypes.HvBattUsgBlkdTyp
        return self.datamap["PrpsnHvBattUsgOfChrgBlkd2"]


    def send_PrpsnHvBattUsgOfHldBlkd(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PrpsnHvBattUsgOfHldBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfHldBlkd(self):
        name = "PrpsnHvBattUsgOfHldBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfHldBlkd(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfHldBlkd(self):
        # type: () -> (bool)
        return self.datamap["PrpsnHvBattUsgOfHldBlkd"]


    def send_PrpsnHvBattUsgOfHldBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnHvBattUsgOfHldBlkd2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfHldBlkd2(self):
        name = "PrpsnHvBattUsgOfHldBlkd2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfHldBlkd2(self, datastring):
        # type: (str) -> datatypes.HvBattUsgBlkdTyp
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfHldBlkd2(self):
        # type: () -> datatypes.HvBattUsgBlkdTyp
        return self.datamap["PrpsnHvBattUsgOfHldBlkd2"]


    def send_PrpsnHvBattUsgOfHldSmtBlkd(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PrpsnHvBattUsgOfHldSmtBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfHldSmtBlkd(self):
        name = "PrpsnHvBattUsgOfHldSmtBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfHldSmtBlkd(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfHldSmtBlkd(self):
        # type: () -> (bool)
        return self.datamap["PrpsnHvBattUsgOfHldSmtBlkd"]


    def send_PrpsnHvBattUsgOfHldSmtBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnHvBattUsgOfHldSmtBlkd2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfHldSmtBlkd2(self):
        name = "PrpsnHvBattUsgOfHldSmtBlkd2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfHldSmtBlkd2(self, datastring):
        # type: (str) -> datatypes.HvBattUsgBlkdTyp
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfHldSmtBlkd2(self):
        # type: () -> datatypes.HvBattUsgBlkdTyp
        return self.datamap["PrpsnHvBattUsgOfHldSmtBlkd2"]


    def send_PrpsnHvBattUsgOfHldSpd(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PrpsnHvBattUsgOfHldSpd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgOfHldSpd(self):
        name = "PrpsnHvBattUsgOfHldSpd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgOfHldSpd(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_PrpsnHvBattUsgOfHldSpd(self):
        # type: () -> (int)
        return self.datamap["PrpsnHvBattUsgOfHldSpd"]


    def send_PrpsnHvBattUsgStsDispd(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PrpsnHvBattUsgStsDispd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnHvBattUsgStsDispd(self):
        name = "PrpsnHvBattUsgStsDispd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnHvBattUsgStsDispd(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_PrpsnHvBattUsgStsDispd(self):
        # type: () -> (bool)
        return self.datamap["PrpsnHvBattUsgStsDispd"]


    def send_PrpsnModElecDrvBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnModElecDrvBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnModElecDrvBlkd(self):
        name = "PrpsnModElecDrvBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnModElecDrvBlkd(self, datastring):
        # type: (str) -> datatypes.Typ1
        return json.loads(datastring)
    def get_PrpsnModElecDrvBlkd(self):
        # type: () -> datatypes.Typ1
        return self.datamap["PrpsnModElecDrvBlkd"]


    def send_PrpsnModOfSaveBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnModOfSaveBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnModOfSaveBlkd(self):
        name = "PrpsnModOfSaveBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnModOfSaveBlkd(self, datastring):
        # type: (str) -> datatypes.Typ1
        return json.loads(datastring)
    def get_PrpsnModOfSaveBlkd(self):
        # type: () -> datatypes.Typ1
        return self.datamap["PrpsnModOfSaveBlkd"]


    def send_PrpsnModOfTracBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnModOfTracBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnModOfTracBlkd(self):
        name = "PrpsnModOfTracBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnModOfTracBlkd(self, datastring):
        # type: (str) -> datatypes.Typ1
        return json.loads(datastring)
    def get_PrpsnModOfTracBlkd(self):
        # type: () -> datatypes.Typ1
        return self.datamap["PrpsnModOfTracBlkd"]


    def send_PrpsnModOffroadBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnModOffroadBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnModOffroadBlkd(self):
        name = "PrpsnModOffroadBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnModOffroadBlkd(self, datastring):
        # type: (str) -> datatypes.Typ1
        return json.loads(datastring)
    def get_PrpsnModOffroadBlkd(self):
        # type: () -> datatypes.Typ1
        return self.datamap["PrpsnModOffroadBlkd"]


    def send_PrpsnModSptBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PrpsnModSptBlkd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PrpsnModSptBlkd(self):
        name = "PrpsnModSptBlkd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PrpsnModSptBlkd(self, datastring):
        # type: (str) -> datatypes.Typ1
        return json.loads(datastring)
    def get_PrpsnModSptBlkd(self):
        # type: () -> datatypes.Typ1
        return self.datamap["PrpsnModSptBlkd"]


    def send_PtCluTq(self, data):
        # type: (datatypes.PtCluTq1) -> None
        s = toJson_PtCluTq1(data)
        name = "PtCluTq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PtCluTq(self):
        name = "PtCluTq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PtCluTq(self, datastring):
        # type: (str) -> datatypes.PtCluTq1
        return fromJson_PtCluTq1(datastring)
    def get_PtCluTq(self):
        # type: () -> (datatypes.PtCluTq1)
        return self.datamap["PtCluTq"]


    def send_PtDrvrSetg(self, data):
        # type: (datatypes.PtDrvrSetg2) -> None
        s = toJson_PtDrvrSetg2(data)
        name = "PtDrvrSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PtDrvrSetg(self):
        name = "PtDrvrSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PtDrvrSetg(self, datastring):
        # type: (str) -> datatypes.PtDrvrSetg2
        return fromJson_PtDrvrSetg2(datastring)
    def get_PtDrvrSetg(self):
        # type: () -> (datatypes.PtDrvrSetg2)
        return self.datamap["PtDrvrSetg"]


    def send_PtGearTar(self, data):
        # type: (datatypes.PtGearAct1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PtGearTar"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PtGearTar(self):
        name = "PtGearTar"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PtGearTar(self, datastring):
        # type: (str) -> datatypes.PtGearAct1
        return json.loads(datastring)
    def get_PtGearTar(self):
        # type: () -> datatypes.PtGearAct1
        return self.datamap["PtGearTar"]


    def send_PtTqAtWhlFrntAct(self, data):
        # type: (datatypes.PtTqAtWhlFrntActRec1) -> None
        s = toJson_PtTqAtWhlFrntActRec1(data)
        name = "PtTqAtWhlFrntAct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PtTqAtWhlFrntAct(self):
        name = "PtTqAtWhlFrntAct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PtTqAtWhlFrntAct(self, datastring):
        # type: (str) -> datatypes.PtTqAtWhlFrntActRec1
        return fromJson_PtTqAtWhlFrntActRec1(datastring)
    def get_PtTqAtWhlFrntAct(self):
        # type: () -> (datatypes.PtTqAtWhlFrntActRec1)
        return self.datamap["PtTqAtWhlFrntAct"]


    def send_PwrChrgDetdForPrkgHeatrElec(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "PwrChrgDetdForPrkgHeatrElec"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PwrChrgDetdForPrkgHeatrElec(self):
        name = "PwrChrgDetdForPrkgHeatrElec"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PwrChrgDetdForPrkgHeatrElec(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_PwrChrgDetdForPrkgHeatrElec(self):
        # type: () -> (bool)
        return self.datamap["PwrChrgDetdForPrkgHeatrElec"]


    def send_PwrSplyErrSts(self, data):
        # type: (datatypes.PwrSplyErrSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "PwrSplyErrSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_PwrSplyErrSts(self):
        name = "PwrSplyErrSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_PwrSplyErrSts(self, datastring):
        # type: (str) -> datatypes.PwrSplyErrSts1
        return json.loads(datastring)
    def get_PwrSplyErrSts(self):
        # type: () -> datatypes.PwrSplyErrSts1
        return self.datamap["PwrSplyErrSts"]


    def send_RadioFrqAn(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "RadioFrqAn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RadioFrqAn(self):
        name = "RadioFrqAn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RadioFrqAn(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_RadioFrqAn(self):
        # type: () -> (int)
        return self.datamap["RadioFrqAn"]


    def send_RainSenMemdReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "RainSenMemdReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RainSenMemdReq(self):
        name = "RainSenMemdReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RainSenMemdReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_RainSenMemdReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["RainSenMemdReq"]


    def send_ReAxleWarn(self, data):
        # type: (datatypes.AxleWarn) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "ReAxleWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ReAxleWarn(self):
        name = "ReAxleWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ReAxleWarn(self, datastring):
        # type: (str) -> datatypes.AxleWarn
        return json.loads(datastring)
    def get_ReAxleWarn(self):
        # type: () -> datatypes.AxleWarn
        return self.datamap["ReAxleWarn"]


    def send_RlyPwrDistbnCmd1WdBattSaveCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RlyPwrDistbnCmd1WdBattSaveCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RlyPwrDistbnCmd1WdBattSaveCmd(self):
        name = "RlyPwrDistbnCmd1WdBattSaveCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RlyPwrDistbnCmd1WdBattSaveCmd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RlyPwrDistbnCmd1WdBattSaveCmd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RlyPwrDistbnCmd1WdBattSaveCmd"]


    def send_RlyPwrDistbnCmd1WdIgnRlyCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RlyPwrDistbnCmd1WdIgnRlyCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RlyPwrDistbnCmd1WdIgnRlyCmd(self):
        name = "RlyPwrDistbnCmd1WdIgnRlyCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RlyPwrDistbnCmd1WdIgnRlyCmd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RlyPwrDistbnCmd1WdIgnRlyCmd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RlyPwrDistbnCmd1WdIgnRlyCmd"]


    def send_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RlyPwrDistbnCmd1WdIgnRlyExtCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self):
        name = "RlyPwrDistbnCmd1WdIgnRlyExtCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RlyPwrDistbnCmd1WdIgnRlyExtCmd"]


    def send_RlyPwrDistbnCmd1WdPreBattSaveCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RlyPwrDistbnCmd1WdPreBattSaveCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RlyPwrDistbnCmd1WdPreBattSaveCmd(self):
        name = "RlyPwrDistbnCmd1WdPreBattSaveCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RlyPwrDistbnCmd1WdPreBattSaveCmd(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RlyPwrDistbnCmd1WdPreBattSaveCmd(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RlyPwrDistbnCmd1WdPreBattSaveCmd"]


    def send_RmnLockgPrsnlReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RmnLockgPrsnlReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RmnLockgPrsnlReq(self):
        name = "RmnLockgPrsnlReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RmnLockgPrsnlReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RmnLockgPrsnlReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RmnLockgPrsnlReq"]


    def send_RngbdIllmnCmd(self, data):
        # type: (datatypes.RngBdIllmnCmdPen1) -> None
        s = toJson_RngBdIllmnCmdPen1(data)
        name = "RngbdIllmnCmd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RngbdIllmnCmd(self):
        name = "RngbdIllmnCmd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RngbdIllmnCmd(self, datastring):
        # type: (str) -> datatypes.RngBdIllmnCmdPen1
        return fromJson_RngBdIllmnCmdPen1(datastring)
    def get_RngbdIllmnCmd(self):
        # type: () -> (datatypes.RngBdIllmnCmdPen1)
        return self.datamap["RngbdIllmnCmd"]


    def send_RoadFricIndcnActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "RoadFricIndcnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadFricIndcnActv(self):
        name = "RoadFricIndcnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadFricIndcnActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_RoadFricIndcnActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["RoadFricIndcnActv"]


    def send_RoadFricIndcnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RoadFricIndcnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadFricIndcnSts(self):
        name = "RoadFricIndcnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadFricIndcnSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_RoadFricIndcnSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["RoadFricIndcnSts"]


    def send_RoadFricWarnReq(self, data):
        # type: (datatypes.RoadFricWarnReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RoadFricWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadFricWarnReq(self):
        name = "RoadFricWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadFricWarnReq(self, datastring):
        # type: (str) -> datatypes.RoadFricWarnReq1
        return json.loads(datastring)
    def get_RoadFricWarnReq(self):
        # type: () -> datatypes.RoadFricWarnReq1
        return self.datamap["RoadFricWarnReq"]


    def send_RoadSgnInfoActv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "RoadSgnInfoActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadSgnInfoActv(self):
        name = "RoadSgnInfoActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadSgnInfoActv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_RoadSgnInfoActv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["RoadSgnInfoActv"]


    def send_RoadSgnInfoSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RoadSgnInfoSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadSgnInfoSts(self):
        name = "RoadSgnInfoSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadSgnInfoSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_RoadSgnInfoSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["RoadSgnInfoSts"]


    def send_RoadSpdLimActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RoadSpdLimActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadSpdLimActvSts(self):
        name = "RoadSpdLimActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadSpdLimActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RoadSpdLimActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RoadSpdLimActvSts"]


    def send_RoadUsrProtnActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RoadUsrProtnActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadUsrProtnActv(self):
        name = "RoadUsrProtnActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadUsrProtnActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_RoadUsrProtnActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["RoadUsrProtnActv"]


    def send_RoadUsrProtnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RoadUsrProtnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RoadUsrProtnSts(self):
        name = "RoadUsrProtnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RoadUsrProtnSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_RoadUsrProtnSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["RoadUsrProtnSts"]


    def send_RotyDirReq2(self, data):
        # type: (datatypes.RotyDirUI) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RotyDirReq2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RotyDirReq2(self):
        name = "RotyDirReq2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RotyDirReq2(self, datastring):
        # type: (str) -> datatypes.RotyDirUI
        return json.loads(datastring)
    def get_RotyDirReq2(self):
        # type: () -> datatypes.RotyDirUI
        return self.datamap["RotyDirReq2"]


    def send_RotyDirReq1(self, data):
        # type: (datatypes.RotyDirUI1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "RotyDirReq1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RotyDirReq1(self):
        name = "RotyDirReq1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RotyDirReq1(self, datastring):
        # type: (str) -> datatypes.RotyDirUI1
        return json.loads(datastring)
    def get_RotyDirReq1(self):
        # type: () -> datatypes.RotyDirUI1
        return self.datamap["RotyDirReq1"]


    def send_RotyPosReq2(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "RotyPosReq2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RotyPosReq2(self):
        name = "RotyPosReq2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RotyPosReq2(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_RotyPosReq2(self):
        # type: () -> (int)
        return self.datamap["RotyPosReq2"]


    def send_RotyPosReq1(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "RotyPosReq1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RotyPosReq1(self):
        name = "RotyPosReq1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RotyPosReq1(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_RotyPosReq1(self):
        # type: () -> (int)
        return self.datamap["RotyPosReq1"]


    def send_RouteInfo(self, data):
        # type: (datatypes.RouteInfoRec1) -> None
        s = toJson_RouteInfoRec1(data)
        name = "RouteInfo"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RouteInfo(self):
        name = "RouteInfo"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RouteInfo(self, datastring):
        # type: (str) -> datatypes.RouteInfoRec1
        return fromJson_RouteInfoRec1(datastring)
    def get_RouteInfo(self):
        # type: () -> (datatypes.RouteInfoRec1)
        return self.datamap["RouteInfo"]


    def send_RsdsSysStsLe(self, data):
        # type: (datatypes.RsdsSysSts) -> None
        s = toJson_RsdsSysSts(data)
        name = "RsdsSysStsLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RsdsSysStsLe(self):
        name = "RsdsSysStsLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RsdsSysStsLe(self, datastring):
        # type: (str) -> datatypes.RsdsSysSts
        return fromJson_RsdsSysSts(datastring)
    def get_RsdsSysStsLe(self):
        # type: () -> (datatypes.RsdsSysSts)
        return self.datamap["RsdsSysStsLe"]


    def send_RsdsSysStsRi(self, data):
        # type: (datatypes.RsdsSysSts) -> None
        s = toJson_RsdsSysSts(data)
        name = "RsdsSysStsRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_RsdsSysStsRi(self):
        name = "RsdsSysStsRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_RsdsSysStsRi(self, datastring):
        # type: (str) -> datatypes.RsdsSysSts
        return fromJson_RsdsSysSts(datastring)
    def get_RsdsSysStsRi(self):
        # type: () -> (datatypes.RsdsSysSts)
        return self.datamap["RsdsSysStsRi"]


    def send_SaveSetgToMemPrmnt(self, data):
        # type: (datatypes.OffOnAut1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SaveSetgToMemPrmnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SaveSetgToMemPrmnt(self):
        name = "SaveSetgToMemPrmnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SaveSetgToMemPrmnt(self, datastring):
        # type: (str) -> datatypes.OffOnAut1
        return json.loads(datastring)
    def get_SaveSetgToMemPrmnt(self):
        # type: () -> datatypes.OffOnAut1
        return self.datamap["SaveSetgToMemPrmnt"]


    def send_ScrBarVolIndcn(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ScrBarVolIndcn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ScrBarVolIndcn(self):
        name = "ScrBarVolIndcn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ScrBarVolIndcn(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_ScrBarVolIndcn(self):
        # type: () -> (float)
        return self.datamap["ScrBarVolIndcn"]


    def send_ScrMaxFillgVol(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ScrMaxFillgVol"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ScrMaxFillgVol(self):
        name = "ScrMaxFillgVol"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ScrMaxFillgVol(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_ScrMaxFillgVol(self):
        # type: () -> (int)
        return self.datamap["ScrMaxFillgVol"]


    def send_ScrReagentTankVol(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "ScrReagentTankVol"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_ScrReagentTankVol(self):
        name = "ScrReagentTankVol"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_ScrReagentTankVol(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_ScrReagentTankVol(self):
        # type: () -> (int)
        return self.datamap["ScrReagentTankVol"]


    def send_SeatBackUnlckdThrd(self, data):
        # type: (datatypes.SeatBackUnlckd) -> None
        s = toJson_SeatBackUnlckd(data)
        name = "SeatBackUnlckdThrd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatBackUnlckdThrd(self):
        name = "SeatBackUnlckdThrd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatBackUnlckdThrd(self, datastring):
        # type: (str) -> datatypes.SeatBackUnlckd
        return fromJson_SeatBackUnlckd(datastring)
    def get_SeatBackUnlckdThrd(self):
        # type: () -> (datatypes.SeatBackUnlckd)
        return self.datamap["SeatBackUnlckdThrd"]


    def send_SeatDispBtnPsd(self, data):
        # type: (datatypes.SeatDispBtnPsd) -> None
        s = toJson_SeatDispBtnPsd(data)
        name = "SeatDispBtnPsd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatDispBtnPsd(self):
        name = "SeatDispBtnPsd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatDispBtnPsd(self, datastring):
        # type: (str) -> datatypes.SeatDispBtnPsd
        return fromJson_SeatDispBtnPsd(datastring)
    def get_SeatDispBtnPsd(self):
        # type: () -> (datatypes.SeatDispBtnPsd)
        return self.datamap["SeatDispBtnPsd"]


    def send_SeatHeatDurgClimaEnad(self, data):
        # type: (datatypes.SeatHeatDurgClimaEnad) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatHeatDurgClimaEnad"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatHeatDurgClimaEnad(self):
        name = "SeatHeatDurgClimaEnad"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatHeatDurgClimaEnad(self, datastring):
        # type: (str) -> datatypes.SeatHeatDurgClimaEnad
        return json.loads(datastring)
    def get_SeatHeatDurgClimaEnad(self):
        # type: () -> datatypes.SeatHeatDurgClimaEnad
        return self.datamap["SeatHeatDurgClimaEnad"]


    def send_SeatHeatgAutCdn(self, data):
        # type: (datatypes.Flg1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatHeatgAutCdn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatHeatgAutCdn(self):
        name = "SeatHeatgAutCdn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatHeatgAutCdn(self, datastring):
        # type: (str) -> datatypes.Flg1
        return json.loads(datastring)
    def get_SeatHeatgAutCdn(self):
        # type: () -> datatypes.Flg1
        return self.datamap["SeatHeatgAutCdn"]


    def send_SeatSwtLeSigThrd(self, data):
        # type: (datatypes.SwtVertSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatSwtLeSigThrd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatSwtLeSigThrd(self):
        name = "SeatSwtLeSigThrd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatSwtLeSigThrd(self, datastring):
        # type: (str) -> datatypes.SwtVertSts1
        return json.loads(datastring)
    def get_SeatSwtLeSigThrd(self):
        # type: () -> datatypes.SwtVertSts1
        return self.datamap["SeatSwtLeSigThrd"]


    def send_SeatSwtRiSigThrd(self, data):
        # type: (datatypes.SwtVertSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatSwtRiSigThrd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatSwtRiSigThrd(self):
        name = "SeatSwtRiSigThrd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatSwtRiSigThrd(self, datastring):
        # type: (str) -> datatypes.SwtVertSts1
        return json.loads(datastring)
    def get_SeatSwtRiSigThrd(self):
        # type: () -> datatypes.SwtVertSts1
        return self.datamap["SeatSwtRiSigThrd"]


    def send_SeatVentnAutCdn(self, data):
        # type: (datatypes.Flg1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatVentnAutCdn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatVentnAutCdn(self):
        name = "SeatVentnAutCdn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatVentnAutCdn(self, datastring):
        # type: (str) -> datatypes.Flg1
        return json.loads(datastring)
    def get_SeatVentnAutCdn(self):
        # type: () -> datatypes.Flg1
        return self.datamap["SeatVentnAutCdn"]


    def send_SetOfLang(self, data):
        # type: (datatypes.SetOfLang) -> None
        s = toJson_SetOfLang(data)
        name = "SetOfLang"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SetOfLang(self):
        name = "SetOfLang"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SetOfLang(self, datastring):
        # type: (str) -> datatypes.SetOfLang
        return fromJson_SetOfLang(datastring)
    def get_SetOfLang(self):
        # type: () -> (datatypes.SetOfLang)
        return self.datamap["SetOfLang"]


    def send_SetOfLangAck(self, data):
        # type: (datatypes.FailrNoFailr1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SetOfLangAck"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SetOfLangAck(self):
        name = "SetOfLangAck"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SetOfLangAck(self, datastring):
        # type: (str) -> datatypes.FailrNoFailr1
        return json.loads(datastring)
    def get_SetOfLangAck(self):
        # type: () -> datatypes.FailrNoFailr1
        return self.datamap["SetOfLangAck"]


    def send_SetTiAndDate(self, data):
        # type: (datatypes.DateTi1ForSet) -> None
        s = toJson_DateTi1ForSet(data)
        name = "SetTiAndDate"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SetTiAndDate(self):
        name = "SetTiAndDate"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SetTiAndDate(self, datastring):
        # type: (str) -> datatypes.DateTi1ForSet
        return fromJson_DateTi1ForSet(datastring)
    def get_SetTiAndDate(self):
        # type: () -> (datatypes.DateTi1ForSet)
        return self.datamap["SetTiAndDate"]


    def send_SetgAndRstOfTripForDrvr(self, data):
        # type: (datatypes.SetgAndRstOfTripForDrvr) -> None
        s = toJson_SetgAndRstOfTripForDrvr(data)
        name = "SetgAndRstOfTripForDrvr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SetgAndRstOfTripForDrvr(self):
        name = "SetgAndRstOfTripForDrvr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SetgAndRstOfTripForDrvr(self, datastring):
        # type: (str) -> datatypes.SetgAndRstOfTripForDrvr
        return fromJson_SetgAndRstOfTripForDrvr(datastring)
    def get_SetgAndRstOfTripForDrvr(self):
        # type: () -> (datatypes.SetgAndRstOfTripForDrvr)
        return self.datamap["SetgAndRstOfTripForDrvr"]


    def send_SftyCchAccSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        s = toJson_SftyCchActvnSts2(data)
        name = "SftyCchAccSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchAccSts(self):
        name = "SftyCchAccSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchAccSts(self, datastring):
        # type: (str) -> datatypes.SftyCchActvnSts2
        return fromJson_SftyCchActvnSts2(datastring)
    def get_SftyCchAccSts(self):
        # type: () -> (datatypes.SftyCchActvnSts2)
        return self.datamap["SftyCchAccSts"]


    def send_SftyCchDrvgCycIdx(self, data):
        # type: (datatypes.SftyCchIdx2) -> None
        s = toJson_SftyCchIdx2(data)
        name = "SftyCchDrvgCycIdx"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchDrvgCycIdx(self):
        name = "SftyCchDrvgCycIdx"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchDrvgCycIdx(self, datastring):
        # type: (str) -> datatypes.SftyCchIdx2
        return fromJson_SftyCchIdx2(datastring)
    def get_SftyCchDrvgCycIdx(self):
        # type: () -> (datatypes.SftyCchIdx2)
        return self.datamap["SftyCchDrvgCycIdx"]


    def send_SftyCchLaneKeepAidSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        s = toJson_SftyCchActvnSts2(data)
        name = "SftyCchLaneKeepAidSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchLaneKeepAidSts(self):
        name = "SftyCchLaneKeepAidSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchLaneKeepAidSts(self, datastring):
        # type: (str) -> datatypes.SftyCchActvnSts2
        return fromJson_SftyCchActvnSts2(datastring)
    def get_SftyCchLaneKeepAidSts(self):
        # type: () -> (datatypes.SftyCchActvnSts2)
        return self.datamap["SftyCchLaneKeepAidSts"]


    def send_SftyCchLongTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        s = toJson_SftyCchIdx1(data)
        name = "SftyCchLongTermIdx"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchLongTermIdx(self):
        name = "SftyCchLongTermIdx"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchLongTermIdx(self, datastring):
        # type: (str) -> datatypes.SftyCchIdx1
        return fromJson_SftyCchIdx1(datastring)
    def get_SftyCchLongTermIdx(self):
        # type: () -> (datatypes.SftyCchIdx1)
        return self.datamap["SftyCchLongTermIdx"]


    def send_SftyCchLongTermIdxRst(self, data):
        # type: (datatypes.NoYes1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchLongTermIdxRst"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchLongTermIdxRst(self):
        name = "SftyCchLongTermIdxRst"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchLongTermIdxRst(self, datastring):
        # type: (str) -> datatypes.NoYes1
        return json.loads(datastring)
    def get_SftyCchLongTermIdxRst(self):
        # type: () -> datatypes.NoYes1
        return self.datamap["SftyCchLongTermIdxRst"]


    def send_SftyCchMidTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        s = toJson_SftyCchIdx1(data)
        name = "SftyCchMidTermIdx"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchMidTermIdx(self):
        name = "SftyCchMidTermIdx"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchMidTermIdx(self, datastring):
        # type: (str) -> datatypes.SftyCchIdx1
        return fromJson_SftyCchIdx1(datastring)
    def get_SftyCchMidTermIdx(self):
        # type: () -> (datatypes.SftyCchIdx1)
        return self.datamap["SftyCchMidTermIdx"]


    def send_SftyCchPilotAssiSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        s = toJson_SftyCchActvnSts2(data)
        name = "SftyCchPilotAssiSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchPilotAssiSts(self):
        name = "SftyCchPilotAssiSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchPilotAssiSts(self, datastring):
        # type: (str) -> datatypes.SftyCchActvnSts2
        return fromJson_SftyCchActvnSts2(datastring)
    def get_SftyCchPilotAssiSts(self):
        # type: () -> (datatypes.SftyCchActvnSts2)
        return self.datamap["SftyCchPilotAssiSts"]


    def send_SftyCchPostTripSts(self, data):
        # type: (datatypes.SftyCchPostTripSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchPostTripSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchPostTripSts(self):
        name = "SftyCchPostTripSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchPostTripSts(self, datastring):
        # type: (str) -> datatypes.SftyCchPostTripSts1
        return json.loads(datastring)
    def get_SftyCchPostTripSts(self):
        # type: () -> datatypes.SftyCchPostTripSts1
        return self.datamap["SftyCchPostTripSts"]


    def send_SftyCchPrimFbAftDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchPrimFbAftDrvg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchPrimFbAftDrvg(self):
        name = "SftyCchPrimFbAftDrvg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchPrimFbAftDrvg(self, datastring):
        # type: (str) -> datatypes.SftyCchFb1
        return json.loads(datastring)
    def get_SftyCchPrimFbAftDrvg(self):
        # type: () -> datatypes.SftyCchFb1
        return self.datamap["SftyCchPrimFbAftDrvg"]


    def send_SftyCchPrimFbDurgDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchPrimFbDurgDrvg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchPrimFbDurgDrvg(self):
        name = "SftyCchPrimFbDurgDrvg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchPrimFbDurgDrvg(self, datastring):
        # type: (str) -> datatypes.SftyCchFb1
        return json.loads(datastring)
    def get_SftyCchPrimFbDurgDrvg(self):
        # type: () -> datatypes.SftyCchFb1
        return self.datamap["SftyCchPrimFbDurgDrvg"]


    def send_SftyCchSecFbAftDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchSecFbAftDrvg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchSecFbAftDrvg(self):
        name = "SftyCchSecFbAftDrvg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchSecFbAftDrvg(self, datastring):
        # type: (str) -> datatypes.SftyCchFb1
        return json.loads(datastring)
    def get_SftyCchSecFbAftDrvg(self):
        # type: () -> datatypes.SftyCchFb1
        return self.datamap["SftyCchSecFbAftDrvg"]


    def send_SftyCchSecFbDurgDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchSecFbDurgDrvg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchSecFbDurgDrvg(self):
        name = "SftyCchSecFbDurgDrvg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchSecFbDurgDrvg(self, datastring):
        # type: (str) -> datatypes.SftyCchFb1
        return json.loads(datastring)
    def get_SftyCchSecFbDurgDrvg(self):
        # type: () -> datatypes.SftyCchFb1
        return self.datamap["SftyCchSecFbDurgDrvg"]


    def send_SftyCchShortTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        s = toJson_SftyCchIdx1(data)
        name = "SftyCchShortTermIdx"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchShortTermIdx(self):
        name = "SftyCchShortTermIdx"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchShortTermIdx(self, datastring):
        # type: (str) -> datatypes.SftyCchIdx1
        return fromJson_SftyCchIdx1(datastring)
    def get_SftyCchShortTermIdx(self):
        # type: () -> (datatypes.SftyCchIdx1)
        return self.datamap["SftyCchShortTermIdx"]


    def send_SftyCchSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SftyCchSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyCchSts(self):
        name = "SftyCchSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyCchSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_SftyCchSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["SftyCchSts"]


    def send_SftySigGroupFromAudSafe(self, data):
        # type: (datatypes.SftySigGroupFromAudSafe1) -> None
        s = toJson_SftySigGroupFromAudSafe1(data)
        name = "SftySigGroupFromAudSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftySigGroupFromAudSafe(self):
        name = "SftySigGroupFromAudSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftySigGroupFromAudSafe(self, datastring):
        # type: (str) -> datatypes.SftySigGroupFromAudSafe1
        return fromJson_SftySigGroupFromAudSafe1(datastring)
    def get_SftySigGroupFromAudSafe(self):
        # type: () -> (datatypes.SftySigGroupFromAudSafe1)
        return self.datamap["SftySigGroupFromAudSafe"]


    def send_SftyWarnGroupFromAsySafe(self, data):
        # type: (datatypes.SftyWarnGroupFromAsySafe1) -> None
        s = toJson_SftyWarnGroupFromAsySafe1(data)
        name = "SftyWarnGroupFromAsySafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SftyWarnGroupFromAsySafe(self):
        name = "SftyWarnGroupFromAsySafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SftyWarnGroupFromAsySafe(self, datastring):
        # type: (str) -> datatypes.SftyWarnGroupFromAsySafe1
        return fromJson_SftyWarnGroupFromAsySafe1(datastring)
    def get_SftyWarnGroupFromAsySafe(self):
        # type: () -> (datatypes.SftyWarnGroupFromAsySafe1)
        return self.datamap["SftyWarnGroupFromAsySafe"]


    def send_SnsrParkAssiFrnt(self, data):
        # type: (datatypes.SnsrParkAssi1) -> None
        s = toJson_SnsrParkAssi1(data)
        name = "SnsrParkAssiFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SnsrParkAssiFrnt(self):
        name = "SnsrParkAssiFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SnsrParkAssiFrnt(self, datastring):
        # type: (str) -> datatypes.SnsrParkAssi1
        return fromJson_SnsrParkAssi1(datastring)
    def get_SnsrParkAssiFrnt(self):
        # type: () -> (datatypes.SnsrParkAssi1)
        return self.datamap["SnsrParkAssiFrnt"]


    def send_SnsrParkAssiRe(self, data):
        # type: (datatypes.SnsrParkAssi1) -> None
        s = toJson_SnsrParkAssi1(data)
        name = "SnsrParkAssiRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SnsrParkAssiRe(self):
        name = "SnsrParkAssiRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SnsrParkAssiRe(self, datastring):
        # type: (str) -> datatypes.SnsrParkAssi1
        return fromJson_SnsrParkAssi1(datastring)
    def get_SnsrParkAssiRe(self):
        # type: () -> (datatypes.SnsrParkAssi1)
        return self.datamap["SnsrParkAssiRe"]


    def send_SnsrPrkgAssiFrnt(self, data):
        # type: (datatypes.SnsrPrkgAssi2) -> None
        s = toJson_SnsrPrkgAssi2(data)
        name = "SnsrPrkgAssiFrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SnsrPrkgAssiFrnt(self):
        name = "SnsrPrkgAssiFrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SnsrPrkgAssiFrnt(self, datastring):
        # type: (str) -> datatypes.SnsrPrkgAssi2
        return fromJson_SnsrPrkgAssi2(datastring)
    def get_SnsrPrkgAssiFrnt(self):
        # type: () -> (datatypes.SnsrPrkgAssi2)
        return self.datamap["SnsrPrkgAssiFrnt"]


    def send_SnsrPrkgAssiRe(self, data):
        # type: (datatypes.SnsrPrkgAssi2) -> None
        s = toJson_SnsrPrkgAssi2(data)
        name = "SnsrPrkgAssiRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SnsrPrkgAssiRe(self):
        name = "SnsrPrkgAssiRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SnsrPrkgAssiRe(self, datastring):
        # type: (str) -> datatypes.SnsrPrkgAssi2
        return fromJson_SnsrPrkgAssi2(datastring)
    def get_SnsrPrkgAssiRe(self):
        # type: () -> (datatypes.SnsrPrkgAssi2)
        return self.datamap["SnsrPrkgAssiRe"]


    def send_SoundExtActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SoundExtActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SoundExtActvSts(self):
        name = "SoundExtActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SoundExtActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_SoundExtActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["SoundExtActvSts"]


    def send_WhlMotSysSpdAct(self, data):
        # type: (float) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "WhlMotSysSpdAct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WhlMotSysSpdAct(self):
        name = "WhlMotSysSpdAct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WhlMotSysSpdAct(self, datastring):
        # type: (str) -> float
        return json.loads(datastring)
    def get_WhlMotSysSpdAct(self):
        # type: () -> (float)
        return self.datamap["WhlMotSysSpdAct"]


    def send_SpdAlrmActvForRoadSgnInfo(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "SpdAlrmActvForRoadSgnInfo"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SpdAlrmActvForRoadSgnInfo(self):
        name = "SpdAlrmActvForRoadSgnInfo"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SpdAlrmActvForRoadSgnInfo(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_SpdAlrmActvForRoadSgnInfo(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["SpdAlrmActvForRoadSgnInfo"]


    def send_SpdCameraInfoSeldForRoadSgnInfo(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "SpdCameraInfoSeldForRoadSgnInfo"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SpdCameraInfoSeldForRoadSgnInfo(self):
        name = "SpdCameraInfoSeldForRoadSgnInfo"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SpdCameraInfoSeldForRoadSgnInfo(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_SpdCameraInfoSeldForRoadSgnInfo(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["SpdCameraInfoSeldForRoadSgnInfo"]


    def send_SpdLimAdpvSts(self, data):
        # type: (datatypes.OffOnNotAvlSrvRqrd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SpdLimAdpvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SpdLimAdpvSts(self):
        name = "SpdLimAdpvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SpdLimAdpvSts(self, datastring):
        # type: (str) -> datatypes.OffOnNotAvlSrvRqrd
        return json.loads(datastring)
    def get_SpdLimAdpvSts(self):
        # type: () -> datatypes.OffOnNotAvlSrvRqrd
        return self.datamap["SpdLimAdpvSts"]


    def send_SpdLimWarnReq(self, data):
        # type: (datatypes.SpdLimWarnReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SpdLimWarnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SpdLimWarnReq(self):
        name = "SpdLimWarnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SpdLimWarnReq(self, datastring):
        # type: (str) -> datatypes.SpdLimWarnReq1
        return json.loads(datastring)
    def get_SpdLimWarnReq(self):
        # type: () -> datatypes.SpdLimWarnReq1
        return self.datamap["SpdLimWarnReq"]


    def send_SpdWarn(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SpdWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SpdWarn(self):
        name = "SpdWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SpdWarn(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_SpdWarn(self):
        # type: () -> datatypes.Trig1
        return self.datamap["SpdWarn"]


    def send_SrvRst(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SrvRst"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SrvRst(self):
        name = "SrvRst"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SrvRst(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_SrvRst(self):
        # type: () -> datatypes.Trig1
        return self.datamap["SrvRst"]


    def send_SrvTrig(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "SrvTrig"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SrvTrig(self):
        name = "SrvTrig"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SrvTrig(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_SrvTrig(self):
        # type: () -> (int)
        return self.datamap["SrvTrig"]


    def send_SteerAssiActvForAutDrv(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "SteerAssiActvForAutDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerAssiActvForAutDrv(self):
        name = "SteerAssiActvForAutDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerAssiActvForAutDrv(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_SteerAssiActvForAutDrv(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["SteerAssiActvForAutDrv"]


    def send_SteerAssiLvlForAutDrv(self, data):
        # type: (datatypes.SteerAssiLvlForAutDrvPen1) -> None
        s = toJson_SteerAssiLvlForAutDrvPen1(data)
        name = "SteerAssiLvlForAutDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerAssiLvlForAutDrv(self):
        name = "SteerAssiLvlForAutDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerAssiLvlForAutDrv(self, datastring):
        # type: (str) -> datatypes.SteerAssiLvlForAutDrvPen1
        return fromJson_SteerAssiLvlForAutDrvPen1(datastring)
    def get_SteerAssiLvlForAutDrv(self):
        # type: () -> (datatypes.SteerAssiLvlForAutDrvPen1)
        return self.datamap["SteerAssiLvlForAutDrv"]


    def send_SteerSetg(self, data):
        # type: (datatypes.SteerSetg) -> None
        s = toJson_SteerSetg(data)
        name = "SteerSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerSetg(self):
        name = "SteerSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerSetg(self, datastring):
        # type: (str) -> datatypes.SteerSetg
        return fromJson_SteerSetg(datastring)
    def get_SteerSetg(self):
        # type: () -> (datatypes.SteerSetg)
        return self.datamap["SteerSetg"]


    def send_SteerWarnReqForAutDrv(self, data):
        # type: (datatypes.SteerWarnReqForAutDrv1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SteerWarnReqForAutDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWarnReqForAutDrv(self):
        name = "SteerWarnReqForAutDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWarnReqForAutDrv(self, datastring):
        # type: (str) -> datatypes.SteerWarnReqForAutDrv1
        return json.loads(datastring)
    def get_SteerWarnReqForAutDrv(self):
        # type: () -> datatypes.SteerWarnReqForAutDrv1
        return self.datamap["SteerWarnReqForAutDrv"]


    def send_SteerWarnReqForLaneKeepAid(self, data):
        # type: (datatypes.WarnLeRi) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SteerWarnReqForLaneKeepAid"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWarnReqForLaneKeepAid(self):
        name = "SteerWarnReqForLaneKeepAid"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWarnReqForLaneKeepAid(self, datastring):
        # type: (str) -> datatypes.WarnLeRi
        return json.loads(datastring)
    def get_SteerWarnReqForLaneKeepAid(self):
        # type: () -> datatypes.WarnLeRi
        return self.datamap["SteerWarnReqForLaneKeepAid"]


    def send_SteerWhlHeatgDurgClimaEnad(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SteerWhlHeatgDurgClimaEnad"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWhlHeatgDurgClimaEnad(self):
        name = "SteerWhlHeatgDurgClimaEnad"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWhlHeatgDurgClimaEnad(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_SteerWhlHeatgDurgClimaEnad(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["SteerWhlHeatgDurgClimaEnad"]


    def send_SteerWhlHeatgOnReq(self, data):
        # type: (datatypes.SteerWhlHeatgOnCmdTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SteerWhlHeatgOnReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWhlHeatgOnReq(self):
        name = "SteerWhlHeatgOnReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWhlHeatgOnReq(self, datastring):
        # type: (str) -> datatypes.SteerWhlHeatgOnCmdTyp
        return json.loads(datastring)
    def get_SteerWhlHeatgOnReq(self):
        # type: () -> datatypes.SteerWhlHeatgOnCmdTyp
        return self.datamap["SteerWhlHeatgOnReq"]


    def send_SteerWhlHeatgStrtAutCdnOk(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SteerWhlHeatgStrtAutCdnOk"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWhlHeatgStrtAutCdnOk(self):
        name = "SteerWhlHeatgStrtAutCdnOk"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWhlHeatgStrtAutCdnOk(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_SteerWhlHeatgStrtAutCdnOk(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["SteerWhlHeatgStrtAutCdnOk"]


    def send_SteerWhlSnsrSafe(self, data):
        # type: (datatypes.SteerWhlSnsrSafe1) -> None
        s = toJson_SteerWhlSnsrSafe1(data)
        name = "SteerWhlSnsrSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWhlSnsrSafe(self):
        name = "SteerWhlSnsrSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWhlSnsrSafe(self, datastring):
        # type: (str) -> datatypes.SteerWhlSnsrSafe1
        return fromJson_SteerWhlSnsrSafe1(datastring)
    def get_SteerWhlSnsrSafe(self):
        # type: () -> (datatypes.SteerWhlSnsrSafe1)
        return self.datamap["SteerWhlSnsrSafe"]


    def send_SteerWhlSwtPwr(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SteerWhlSwtPwr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SteerWhlSwtPwr(self):
        name = "SteerWhlSwtPwr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SteerWhlSwtPwr(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_SteerWhlSwtPwr(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["SteerWhlSwtPwr"]


    def send_StopStrtInhb(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "StopStrtInhb"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_StopStrtInhb(self):
        name = "StopStrtInhb"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_StopStrtInhb(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_StopStrtInhb(self):
        # type: () -> (int)
        return self.datamap["StopStrtInhb"]


    def send_StrtInProgs(self, data):
        # type: (datatypes.StrtInProgs1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "StrtInProgs"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_StrtInProgs(self):
        name = "StrtInProgs"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_StrtInProgs(self, datastring):
        # type: (str) -> datatypes.StrtInProgs1
        return json.loads(datastring)
    def get_StrtInProgs(self):
        # type: () -> datatypes.StrtInProgs1
        return self.datamap["StrtInProgs"]


    def send_SunRoofPosnSts(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SunRoofPosnSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SunRoofPosnSts(self):
        name = "SunRoofPosnSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SunRoofPosnSts(self, datastring):
        # type: (str) -> datatypes.WinAndRoofAndCurtPosnTyp
        return json.loads(datastring)
    def get_SunRoofPosnSts(self):
        # type: () -> datatypes.WinAndRoofAndCurtPosnTyp
        return self.datamap["SunRoofPosnSts"]


    def send_SuspSetg(self, data):
        # type: (datatypes.SuspSetgRec) -> None
        s = toJson_SuspSetgRec(data)
        name = "SuspSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SuspSetg(self):
        name = "SuspSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SuspSetg(self, datastring):
        # type: (str) -> datatypes.SuspSetgRec
        return fromJson_SuspSetgRec(datastring)
    def get_SuspSetg(self):
        # type: () -> (datatypes.SuspSetgRec)
        return self.datamap["SuspSetg"]


    def send_SwtAcptReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SwtAcptReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SwtAcptReq(self):
        name = "SwtAcptReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SwtAcptReq(self, datastring):
        # type: (str) -> datatypes.PsdNotPsd
        return json.loads(datastring)
    def get_SwtAcptReq(self):
        # type: () -> datatypes.PsdNotPsd
        return self.datamap["SwtAcptReq"]


    def send_SwtAtCenSts(self, data):
        # type: (datatypes.Trig1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SwtAtCenSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SwtAtCenSts(self):
        name = "SwtAtCenSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SwtAtCenSts(self, datastring):
        # type: (str) -> datatypes.Trig1
        return json.loads(datastring)
    def get_SwtAtCenSts(self):
        # type: () -> datatypes.Trig1
        return self.datamap["SwtAtCenSts"]


    def send_SwtForPassAirbCutOffSt(self, data):
        # type: (datatypes.OnOff2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SwtForPassAirbCutOffSt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SwtForPassAirbCutOffSt(self):
        name = "SwtForPassAirbCutOffSt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SwtForPassAirbCutOffSt(self, datastring):
        # type: (str) -> datatypes.OnOff2
        return json.loads(datastring)
    def get_SwtForPassAirbCutOffSt(self):
        # type: () -> datatypes.OnOff2
        return self.datamap["SwtForPassAirbCutOffSt"]


    def send_TankFlapSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TankFlapSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TankFlapSts(self):
        name = "TankFlapSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TankFlapSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_TankFlapSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["TankFlapSts"]


    def send_TelmSts(self, data):
        # type: (datatypes.TelmSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TelmSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TelmSts(self):
        name = "TelmSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TelmSts(self, datastring):
        # type: (str) -> datatypes.TelmSts
        return json.loads(datastring)
    def get_TelmSts(self):
        # type: () -> datatypes.TelmSts
        return self.datamap["TelmSts"]


    def send_TelmStsConnGen2(self, data):
        # type: (datatypes.TelmSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TelmStsConnGen2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TelmStsConnGen2(self):
        name = "TelmStsConnGen2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TelmStsConnGen2(self, datastring):
        # type: (str) -> datatypes.TelmSts
        return json.loads(datastring)
    def get_TelmStsConnGen2(self):
        # type: () -> datatypes.TelmSts
        return self.datamap["TelmStsConnGen2"]


    def send_TiAndDateIndcn(self, data):
        # type: (datatypes.DateTi30) -> None
        s = toJson_DateTi30(data)
        name = "TiAndDateIndcn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TiAndDateIndcn(self):
        name = "TiAndDateIndcn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TiAndDateIndcn(self, datastring):
        # type: (str) -> datatypes.DateTi30
        return fromJson_DateTi30(datastring)
    def get_TiAndDateIndcn(self):
        # type: () -> (datatypes.DateTi30)
        return self.datamap["TiAndDateIndcn"]


    def send_TiGapAdpvSeld(self, data):
        # type: (datatypes.TiGapAdpvSeldPen1) -> None
        s = toJson_TiGapAdpvSeldPen1(data)
        name = "TiGapAdpvSeld"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TiGapAdpvSeld(self):
        name = "TiGapAdpvSeld"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TiGapAdpvSeld(self, datastring):
        # type: (str) -> datatypes.TiGapAdpvSeldPen1
        return fromJson_TiGapAdpvSeldPen1(datastring)
    def get_TiGapAdpvSeld(self):
        # type: () -> (datatypes.TiGapAdpvSeldPen1)
        return self.datamap["TiGapAdpvSeld"]


    def send_TiGapLimd(self, data):
        # type: (datatypes.TiGapPen1) -> None
        s = toJson_TiGapPen1(data)
        name = "TiGapLimd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TiGapLimd(self):
        name = "TiGapLimd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TiGapLimd(self, datastring):
        # type: (str) -> datatypes.TiGapPen1
        return fromJson_TiGapPen1(datastring)
    def get_TiGapLimd(self):
        # type: () -> (datatypes.TiGapPen1)
        return self.datamap["TiGapLimd"]


    def send_TireCircumCalByNav(self, data):
        # type: (datatypes.TireCircumCalByNav) -> None
        s = toJson_TireCircumCalByNav(data)
        name = "TireCircumCalByNav"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TireCircumCalByNav(self):
        name = "TireCircumCalByNav"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TireCircumCalByNav(self, datastring):
        # type: (str) -> datatypes.TireCircumCalByNav
        return fromJson_TireCircumCalByNav(datastring)
    def get_TireCircumCalByNav(self):
        # type: () -> (datatypes.TireCircumCalByNav)
        return self.datamap["TireCircumCalByNav"]


    def send_TireMonCalReq(self, data):
        # type: (datatypes.StrtStopReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TireMonCalReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TireMonCalReq(self):
        name = "TireMonCalReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TireMonCalReq(self, datastring):
        # type: (str) -> datatypes.StrtStopReq1
        return json.loads(datastring)
    def get_TireMonCalReq(self):
        # type: () -> datatypes.StrtStopReq1
        return self.datamap["TireMonCalReq"]


    def send_TireMonrDispReq(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TireMonrDispReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TireMonrDispReq(self):
        name = "TireMonrDispReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TireMonrDispReq(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_TireMonrDispReq(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["TireMonrDispReq"]


    def send_TirePAbsltValFrntRi(self, data):
        # type: (datatypes.TirePAbsltValFrntRi1) -> None
        s = toJson_TirePAbsltValFrntRi1(data)
        name = "TirePAbsltValFrntRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TirePAbsltValFrntRi(self):
        name = "TirePAbsltValFrntRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TirePAbsltValFrntRi(self, datastring):
        # type: (str) -> datatypes.TirePAbsltValFrntRi1
        return fromJson_TirePAbsltValFrntRi1(datastring)
    def get_TirePAbsltValFrntRi(self):
        # type: () -> (datatypes.TirePAbsltValFrntRi1)
        return self.datamap["TirePAbsltValFrntRi"]


    def send_TirePAbsltValFrntLe(self, data):
        # type: (datatypes.TirePAbsltValFrntLe3) -> None
        s = toJson_TirePAbsltValFrntLe3(data)
        name = "TirePAbsltValFrntLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TirePAbsltValFrntLe(self):
        name = "TirePAbsltValFrntLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TirePAbsltValFrntLe(self, datastring):
        # type: (str) -> datatypes.TirePAbsltValFrntLe3
        return fromJson_TirePAbsltValFrntLe3(datastring)
    def get_TirePAbsltValFrntLe(self):
        # type: () -> (datatypes.TirePAbsltValFrntLe3)
        return self.datamap["TirePAbsltValFrntLe"]


    def send_TirePAbsltValReLe(self, data):
        # type: (datatypes.TirePAbsltValReLe1) -> None
        s = toJson_TirePAbsltValReLe1(data)
        name = "TirePAbsltValReLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TirePAbsltValReLe(self):
        name = "TirePAbsltValReLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TirePAbsltValReLe(self, datastring):
        # type: (str) -> datatypes.TirePAbsltValReLe1
        return fromJson_TirePAbsltValReLe1(datastring)
    def get_TirePAbsltValReLe(self):
        # type: () -> (datatypes.TirePAbsltValReLe1)
        return self.datamap["TirePAbsltValReLe"]


    def send_TirePAbsltValReRi(self, data):
        # type: (datatypes.TirePAbsltValReRi1) -> None
        s = toJson_TirePAbsltValReRi1(data)
        name = "TirePAbsltValReRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TirePAbsltValReRi(self):
        name = "TirePAbsltValReRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TirePAbsltValReRi(self, datastring):
        # type: (str) -> datatypes.TirePAbsltValReRi1
        return fromJson_TirePAbsltValReRi1(datastring)
    def get_TirePAbsltValReRi(self):
        # type: () -> (datatypes.TirePAbsltValReRi1)
        return self.datamap["TirePAbsltValReRi"]


    def send_TirePCalSts(self, data):
        # type: (datatypes.TirePCalSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TirePCalSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TirePCalSts(self):
        name = "TirePCalSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TirePCalSts(self, datastring):
        # type: (str) -> datatypes.TirePCalSts2
        return json.loads(datastring)
    def get_TirePCalSts(self):
        # type: () -> datatypes.TirePCalSts2
        return self.datamap["TirePCalSts"]


    def send_TirePMonData(self, data):
        # type: (datatypes.TirePMonData1) -> None
        s = toJson_TirePMonData1(data)
        name = "TirePMonData"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TirePMonData(self):
        name = "TirePMonData"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TirePMonData(self, datastring):
        # type: (str) -> datatypes.TirePMonData1
        return fromJson_TirePMonData1(datastring)
    def get_TirePMonData(self):
        # type: () -> (datatypes.TirePMonData1)
        return self.datamap["TirePMonData"]


    def send_TnlEntryOrEnd(self, data):
        # type: (datatypes.NoYes1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TnlEntryOrEnd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TnlEntryOrEnd(self):
        name = "TnlEntryOrEnd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TnlEntryOrEnd(self, datastring):
        # type: (str) -> datatypes.NoYes1
        return json.loads(datastring)
    def get_TnlEntryOrEnd(self):
        # type: () -> datatypes.NoYes1
        return self.datamap["TnlEntryOrEnd"]


    def send_TrSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrSts(self):
        name = "TrSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrSts(self, datastring):
        # type: (str) -> datatypes.DoorSts2
        return json.loads(datastring)
    def get_TrSts(self):
        # type: () -> datatypes.DoorSts2
        return self.datamap["TrSts"]


    def send_TrfcLiSpprtActv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrfcLiSpprtActv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrfcLiSpprtActv(self):
        name = "TrfcLiSpprtActv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrfcLiSpprtActv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_TrfcLiSpprtActv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["TrfcLiSpprtActv"]


    def send_TrfcLiSpprtSts(self, data):
        # type: (datatypes.FctSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrfcLiSpprtSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrfcLiSpprtSts(self):
        name = "TrfcLiSpprtSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrfcLiSpprtSts(self, datastring):
        # type: (str) -> datatypes.FctSts2
        return json.loads(datastring)
    def get_TrfcLiSpprtSts(self):
        # type: () -> datatypes.FctSts2
        return self.datamap["TrfcLiSpprtSts"]


    def send_TrfcRglnSeldForRoadSgnInfo(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "TrfcRglnSeldForRoadSgnInfo"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrfcRglnSeldForRoadSgnInfo(self):
        name = "TrfcRglnSeldForRoadSgnInfo"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrfcRglnSeldForRoadSgnInfo(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_TrfcRglnSeldForRoadSgnInfo(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["TrfcRglnSeldForRoadSgnInfo"]


    def send_TripModSeln(self, data):
        # type: (datatypes.TripModSeln) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TripModSeln"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TripModSeln(self):
        name = "TripModSeln"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TripModSeln(self, datastring):
        # type: (str) -> datatypes.TripModSeln
        return json.loads(datastring)
    def get_TripModSeln(self):
        # type: () -> datatypes.TripModSeln
        return self.datamap["TripModSeln"]


    def send_TrlrLampChkSts1WdSts1(self, data):
        # type: (datatypes.TrlrLampChkSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrlrLampChkSts1WdSts1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrlrLampChkSts1WdSts1(self):
        name = "TrlrLampChkSts1WdSts1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrlrLampChkSts1WdSts1(self, datastring):
        # type: (str) -> datatypes.TrlrLampChkSts1
        return json.loads(datastring)
    def get_TrlrLampChkSts1WdSts1(self):
        # type: () -> datatypes.TrlrLampChkSts1
        return self.datamap["TrlrLampChkSts1WdSts1"]


    def send_TrlrLampChkAutReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "TrlrLampChkAutReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrlrLampChkAutReq(self):
        name = "TrlrLampChkAutReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrlrLampChkAutReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_TrlrLampChkAutReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["TrlrLampChkAutReq"]


    def send_TrlrLampChkRemReq(self, data):
        # type: (datatypes.StrtStopReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrlrLampChkRemReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrlrLampChkRemReq(self):
        name = "TrlrLampChkRemReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrlrLampChkRemReq(self, datastring):
        # type: (str) -> datatypes.StrtStopReq1
        return json.loads(datastring)
    def get_TrlrLampChkRemReq(self):
        # type: () -> datatypes.StrtStopReq1
        return self.datamap["TrlrLampChkRemReq"]


    def send_TrlrLampChkSts1WdSts2(self, data):
        # type: (datatypes.TrlrLampChkSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrlrLampChkSts1WdSts2"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrlrLampChkSts1WdSts2(self):
        name = "TrlrLampChkSts1WdSts2"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrlrLampChkSts1WdSts2(self, datastring):
        # type: (str) -> datatypes.TrlrLampChkSts2
        return json.loads(datastring)
    def get_TrlrLampChkSts1WdSts2(self):
        # type: () -> datatypes.TrlrLampChkSts2
        return self.datamap["TrlrLampChkSts1WdSts2"]


    def send_TrlrPrsntSts1WdSts(self, data):
        # type: (datatypes.TrlrPrsntSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TrlrPrsntSts1WdSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TrlrPrsntSts1WdSts(self):
        name = "TrlrPrsntSts1WdSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TrlrPrsntSts1WdSts(self, datastring):
        # type: (str) -> datatypes.TrlrPrsntSts
        return json.loads(datastring)
    def get_TrlrPrsntSts1WdSts(self):
        # type: () -> datatypes.TrlrPrsntSts
        return self.datamap["TrlrPrsntSts1WdSts"]


    def send_LiExtReq1WdReq3(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "LiExtReq1WdReq3"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_LiExtReq1WdReq3(self):
        name = "LiExtReq1WdReq3"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_LiExtReq1WdReq3(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_LiExtReq1WdReq3(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["LiExtReq1WdReq3"]


    def send_TwliBriRaw(self, data):
        # type: (datatypes.TwliBriRaw) -> None
        s = toJson_TwliBriRaw(data)
        name = "TwliBriRaw"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TwliBriRaw(self):
        name = "TwliBriRaw"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TwliBriRaw(self, datastring):
        # type: (str) -> datatypes.TwliBriRaw
        return fromJson_TwliBriRaw(datastring)
    def get_TwliBriRaw(self):
        # type: () -> (datatypes.TwliBriRaw)
        return self.datamap["TwliBriRaw"]


    def send_TwliBriSts(self, data):
        # type: (datatypes.TwliBriSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TwliBriSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TwliBriSts(self):
        name = "TwliBriSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TwliBriSts(self, datastring):
        # type: (str) -> datatypes.TwliBriSts1
        return json.loads(datastring)
    def get_TwliBriSts(self):
        # type: () -> datatypes.TwliBriSts1
        return self.datamap["TwliBriSts"]


    def send_TwliBriStsForSeatHeatrRe(self, data):
        # type: (datatypes.TwliBriSts1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "TwliBriStsForSeatHeatrRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_TwliBriStsForSeatHeatrRe(self):
        name = "TwliBriStsForSeatHeatrRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_TwliBriStsForSeatHeatrRe(self, datastring):
        # type: (str) -> datatypes.TwliBriSts1
        return json.loads(datastring)
    def get_TwliBriStsForSeatHeatrRe(self):
        # type: () -> datatypes.TwliBriSts1
        return self.datamap["TwliBriStsForSeatHeatrRe"]


    def send_UkwnCptReqToInfoMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        s = toJson_UInt64Rec(data)
        name = "UkwnCptReqToInfoMgr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UkwnCptReqToInfoMgr(self):
        name = "UkwnCptReqToInfoMgr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UkwnCptReqToInfoMgr(self, datastring):
        # type: (str) -> datatypes.UInt64Rec
        return fromJson_UInt64Rec(datastring)
    def get_UkwnCptReqToInfoMgr(self):
        # type: () -> (datatypes.UInt64Rec)
        return self.datamap["UkwnCptReqToInfoMgr"]


    def send_UkwnCptReqToSoundMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        s = toJson_UInt64Rec(data)
        name = "UkwnCptReqToSoundMgr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UkwnCptReqToSoundMgr(self):
        name = "UkwnCptReqToSoundMgr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UkwnCptReqToSoundMgr(self, datastring):
        # type: (str) -> datatypes.UInt64Rec
        return fromJson_UInt64Rec(datastring)
    def get_UkwnCptReqToSoundMgr(self):
        # type: () -> (datatypes.UInt64Rec)
        return self.datamap["UkwnCptReqToSoundMgr"]


    def send_UkwnCptRespFromInfoMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        s = toJson_UInt64Rec(data)
        name = "UkwnCptRespFromInfoMgr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UkwnCptRespFromInfoMgr(self):
        name = "UkwnCptRespFromInfoMgr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UkwnCptRespFromInfoMgr(self, datastring):
        # type: (str) -> datatypes.UInt64Rec
        return fromJson_UInt64Rec(datastring)
    def get_UkwnCptRespFromInfoMgr(self):
        # type: () -> (datatypes.UInt64Rec)
        return self.datamap["UkwnCptRespFromInfoMgr"]


    def send_UkwnCptRespFromSoundMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        s = toJson_UInt64Rec(data)
        name = "UkwnCptRespFromSoundMgr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UkwnCptRespFromSoundMgr(self):
        name = "UkwnCptRespFromSoundMgr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UkwnCptRespFromSoundMgr(self, datastring):
        # type: (str) -> datatypes.UInt64Rec
        return fromJson_UInt64Rec(datastring)
    def get_UkwnCptRespFromSoundMgr(self):
        # type: () -> (datatypes.UInt64Rec)
        return self.datamap["UkwnCptRespFromSoundMgr"]


    def send_UnlckFbVisReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "UnlckFbVisReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UnlckFbVisReq(self):
        name = "UnlckFbVisReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UnlckFbVisReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_UnlckFbVisReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["UnlckFbVisReq"]


    def send_UnlckKeylsReq(self, data):
        # type: (datatypes.UnlckKeylsCfgPen2) -> None
        s = toJson_UnlckKeylsCfgPen2(data)
        name = "UnlckKeylsReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UnlckKeylsReq(self):
        name = "UnlckKeylsReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UnlckKeylsReq(self, datastring):
        # type: (str) -> datatypes.UnlckKeylsCfgPen2
        return fromJson_UnlckKeylsCfgPen2(datastring)
    def get_UnlckKeylsReq(self):
        # type: () -> (datatypes.UnlckKeylsCfgPen2)
        return self.datamap["UnlckKeylsReq"]


    def send_UnlckRemReq(self, data):
        # type: (datatypes.UnlckRemCfgPen1) -> None
        s = toJson_UnlckRemCfgPen1(data)
        name = "UnlckRemReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UnlckRemReq(self):
        name = "UnlckRemReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UnlckRemReq(self, datastring):
        # type: (str) -> datatypes.UnlckRemCfgPen1
        return fromJson_UnlckRemCfgPen1(datastring)
    def get_UnlckRemReq(self):
        # type: () -> (datatypes.UnlckRemCfgPen1)
        return self.datamap["UnlckRemReq"]


    def send_UsrSeldDrvrSpprtFct(self, data):
        # type: (datatypes.DrvrSpprtFct) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "UsrSeldDrvrSpprtFct"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSeldDrvrSpprtFct(self):
        name = "UsrSeldDrvrSpprtFct"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSeldDrvrSpprtFct(self, datastring):
        # type: (str) -> datatypes.DrvrSpprtFct
        return json.loads(datastring)
    def get_UsrSeldDrvrSpprtFct(self):
        # type: () -> datatypes.DrvrSpprtFct
        return self.datamap["UsrSeldDrvrSpprtFct"]


    def send_UsrSetSpdForKeySpdLimn(self, data):
        # type: (datatypes.UsrSetSpdForKeySpdLimn) -> None
        s = toJson_UsrSetSpdForKeySpdLimn(data)
        name = "UsrSetSpdForKeySpdLimn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSetSpdForKeySpdLimn(self):
        name = "UsrSetSpdForKeySpdLimn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSetSpdForKeySpdLimn(self, datastring):
        # type: (str) -> datatypes.UsrSetSpdForKeySpdLimn
        return fromJson_UsrSetSpdForKeySpdLimn(datastring)
    def get_UsrSetSpdForKeySpdLimn(self):
        # type: () -> (datatypes.UsrSetSpdForKeySpdLimn)
        return self.datamap["UsrSetSpdForKeySpdLimn"]


    def send_UsrSetSpdForKeySpdWarn(self, data):
        # type: (datatypes.UsrSetSpdForKeySpdWarn) -> None
        s = toJson_UsrSetSpdForKeySpdWarn(data)
        name = "UsrSetSpdForKeySpdWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSetSpdForKeySpdWarn(self):
        name = "UsrSetSpdForKeySpdWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSetSpdForKeySpdWarn(self, datastring):
        # type: (str) -> datatypes.UsrSetSpdForKeySpdWarn
        return fromJson_UsrSetSpdForKeySpdWarn(datastring)
    def get_UsrSetSpdForKeySpdWarn(self):
        # type: () -> (datatypes.UsrSetSpdForKeySpdWarn)
        return self.datamap["UsrSetSpdForKeySpdWarn"]


    def send_UsrSetVolMaxForKeyVolLimn(self, data):
        # type: (datatypes.UsrSetVolMaxForKeyVolLimn) -> None
        s = toJson_UsrSetVolMaxForKeyVolLimn(data)
        name = "UsrSetVolMaxForKeyVolLimn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSetVolMaxForKeyVolLimn(self):
        name = "UsrSetVolMaxForKeyVolLimn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSetVolMaxForKeyVolLimn(self, datastring):
        # type: (str) -> datatypes.UsrSetVolMaxForKeyVolLimn
        return fromJson_UsrSetVolMaxForKeyVolLimn(datastring)
    def get_UsrSetVolMaxForKeyVolLimn(self):
        # type: () -> (datatypes.UsrSetVolMaxForKeyVolLimn)
        return self.datamap["UsrSetVolMaxForKeyVolLimn"]


    def send_UsrSwtDispClimaReq(self, data):
        # type: (datatypes.OnOffIdle1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "UsrSwtDispClimaReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSwtDispClimaReq(self):
        name = "UsrSwtDispClimaReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSwtDispClimaReq(self, datastring):
        # type: (str) -> datatypes.OnOffIdle1
        return json.loads(datastring)
    def get_UsrSwtDispClimaReq(self):
        # type: () -> datatypes.OnOffIdle1
        return self.datamap["UsrSwtDispClimaReq"]


    def send_UsrSwtDispClimaReqForRowSec(self, data):
        # type: (datatypes.UsrSwtDispClimaReqForRowSec) -> None
        s = toJson_UsrSwtDispClimaReqForRowSec(data)
        name = "UsrSwtDispClimaReqForRowSec"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSwtDispClimaReqForRowSec(self):
        name = "UsrSwtDispClimaReqForRowSec"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSwtDispClimaReqForRowSec(self, datastring):
        # type: (str) -> datatypes.UsrSwtDispClimaReqForRowSec
        return fromJson_UsrSwtDispClimaReqForRowSec(datastring)
    def get_UsrSwtDispClimaReqForRowSec(self):
        # type: () -> (datatypes.UsrSwtDispClimaReqForRowSec)
        return self.datamap["UsrSwtDispClimaReqForRowSec"]


    def send_SeatHeatReLeSts(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatHeatReLeSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatHeatReLeSts(self):
        name = "SeatHeatReLeSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatHeatReLeSts(self, datastring):
        # type: (str) -> datatypes.SeatClimaLvl
        return json.loads(datastring)
    def get_SeatHeatReLeSts(self):
        # type: () -> datatypes.SeatClimaLvl
        return self.datamap["SeatHeatReLeSts"]


    def send_SeatHeatReRiSts(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatHeatReRiSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatHeatReRiSts(self):
        name = "SeatHeatReRiSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatHeatReRiSts(self, datastring):
        # type: (str) -> datatypes.SeatClimaLvl
        return json.loads(datastring)
    def get_SeatHeatReRiSts(self):
        # type: () -> datatypes.SeatClimaLvl
        return self.datamap["SeatHeatReRiSts"]


    def send_UsrSwtDispClimaSts(self, data):
        # type: (datatypes.UsrSwtDispClimaSts) -> None
        s = toJson_UsrSwtDispClimaSts(data)
        name = "UsrSwtDispClimaSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSwtDispClimaSts(self):
        name = "UsrSwtDispClimaSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSwtDispClimaSts(self, datastring):
        # type: (str) -> datatypes.UsrSwtDispClimaSts
        return fromJson_UsrSwtDispClimaSts(datastring)
    def get_UsrSwtDispClimaSts(self):
        # type: () -> (datatypes.UsrSwtDispClimaSts)
        return self.datamap["UsrSwtDispClimaSts"]


    def send_UsrSwtDispForSecRowSeatVentn(self, data):
        # type: (datatypes.UsrSwtDispForSecRowSeatVentn) -> None
        s = toJson_UsrSwtDispForSecRowSeatVentn(data)
        name = "UsrSwtDispForSecRowSeatVentn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSwtDispForSecRowSeatVentn(self):
        name = "UsrSwtDispForSecRowSeatVentn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSwtDispForSecRowSeatVentn(self, datastring):
        # type: (str) -> datatypes.UsrSwtDispForSecRowSeatVentn
        return fromJson_UsrSwtDispForSecRowSeatVentn(datastring)
    def get_UsrSwtDispForSecRowSeatVentn(self):
        # type: () -> (datatypes.UsrSwtDispForSecRowSeatVentn)
        return self.datamap["UsrSwtDispForSecRowSeatVentn"]


    def send_UsrSwtDispReqForSecRowSeatVentn(self, data):
        # type: (datatypes.UsrSwtDispReqForSecRowSeatVentn) -> None
        s = toJson_UsrSwtDispReqForSecRowSeatVentn(data)
        name = "UsrSwtDispReqForSecRowSeatVentn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSwtDispReqForSecRowSeatVentn(self):
        name = "UsrSwtDispReqForSecRowSeatVentn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSwtDispReqForSecRowSeatVentn(self, datastring):
        # type: (str) -> datatypes.UsrSwtDispReqForSecRowSeatVentn
        return fromJson_UsrSwtDispReqForSecRowSeatVentn(datastring)
    def get_UsrSwtDispReqForSecRowSeatVentn(self):
        # type: () -> (datatypes.UsrSwtDispReqForSecRowSeatVentn)
        return self.datamap["UsrSwtDispReqForSecRowSeatVentn"]


    def send_UsrSwtDispReqVrnt(self, data):
        # type: (datatypes.UsrSwtDispReqVrnt) -> None
        s = toJson_UsrSwtDispReqVrnt(data)
        name = "UsrSwtDispReqVrnt"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_UsrSwtDispReqVrnt(self):
        name = "UsrSwtDispReqVrnt"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_UsrSwtDispReqVrnt(self, datastring):
        # type: (str) -> datatypes.UsrSwtDispReqVrnt
        return fromJson_UsrSwtDispReqVrnt(datastring)
    def get_UsrSwtDispReqVrnt(self):
        # type: () -> (datatypes.UsrSwtDispReqVrnt)
        return self.datamap["UsrSwtDispReqVrnt"]


    def send_SeatHeatLvlReqLe(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatHeatLvlReqLe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatHeatLvlReqLe(self):
        name = "SeatHeatLvlReqLe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatHeatLvlReqLe(self, datastring):
        # type: (str) -> datatypes.SeatClimaLvl
        return json.loads(datastring)
    def get_SeatHeatLvlReqLe(self):
        # type: () -> datatypes.SeatClimaLvl
        return self.datamap["SeatHeatLvlReqLe"]


    def send_SeatHeatLvlReqRi(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "SeatHeatLvlReqRi"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_SeatHeatLvlReqRi(self):
        name = "SeatHeatLvlReqRi"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_SeatHeatLvlReqRi(self, datastring):
        # type: (str) -> datatypes.SeatClimaLvl
        return json.loads(datastring)
    def get_SeatHeatLvlReqRi(self):
        # type: () -> datatypes.SeatClimaLvl
        return self.datamap["SeatHeatLvlReqRi"]


    def send_VFCVectorIHU(self, data):
        # type: (datatypes.VFCGrp) -> None
        s = toJson_VFCGrp(data)
        name = "VFCVectorIHU"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VFCVectorIHU(self):
        name = "VFCVectorIHU"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VFCVectorIHU(self, datastring):
        # type: (str) -> datatypes.VFCGrp
        return fromJson_VFCGrp(datastring)
    def get_VFCVectorIHU(self):
        # type: () -> (datatypes.VFCGrp)
        return self.datamap["VFCVectorIHU"]


    def send_VehActvMsgToDrvr(self, data):
        # type: (datatypes.VehActv1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "VehActvMsgToDrvr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehActvMsgToDrvr(self):
        name = "VehActvMsgToDrvr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehActvMsgToDrvr(self, datastring):
        # type: (str) -> datatypes.VehActv1
        return json.loads(datastring)
    def get_VehActvMsgToDrvr(self):
        # type: () -> datatypes.VehActv1
        return self.datamap["VehActvMsgToDrvr"]


    def send_VehBattU(self, data):
        # type: (datatypes.SysU) -> None
        s = toJson_SysU(data)
        name = "VehBattU"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehBattU(self):
        name = "VehBattU"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehBattU(self, datastring):
        # type: (str) -> datatypes.SysU
        return fromJson_SysU(datastring)
    def get_VehBattU(self):
        # type: () -> (datatypes.SysU)
        return self.datamap["VehBattU"]


    def send_VehCfgPrm(self, data):
        # type: (datatypes.VehCfgPrm) -> None
        s = toJson_VehCfgPrm(data)
        name = "VehCfgPrm"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehCfgPrm(self):
        name = "VehCfgPrm"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehCfgPrm(self, datastring):
        # type: (str) -> datatypes.VehCfgPrm
        return fromJson_VehCfgPrm(datastring)
    def get_VehCfgPrm(self):
        # type: () -> (datatypes.VehCfgPrm)
        return self.datamap["VehCfgPrm"]


    def send_VehDecelCtrlSetg(self, data):
        # type: (datatypes.NormSptPen) -> None
        s = toJson_NormSptPen(data)
        name = "VehDecelCtrlSetg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehDecelCtrlSetg(self):
        name = "VehDecelCtrlSetg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehDecelCtrlSetg(self, datastring):
        # type: (str) -> datatypes.NormSptPen
        return fromJson_NormSptPen(datastring)
    def get_VehDecelCtrlSetg(self):
        # type: () -> (datatypes.NormSptPen)
        return self.datamap["VehDecelCtrlSetg"]


    def send_VehMNom(self, data):
        # type: (datatypes.VehMNomRec1) -> None
        s = toJson_VehMNomRec1(data)
        name = "VehMNom"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehMNom(self):
        name = "VehMNom"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehMNom(self, datastring):
        # type: (str) -> datatypes.VehMNomRec1
        return fromJson_VehMNomRec1(datastring)
    def get_VehMNom(self):
        # type: () -> (datatypes.VehMNomRec1)
        return self.datamap["VehMNom"]


    def send_VehModMngtGlbSafe1(self, data):
        # type: (datatypes.VehModMngtGlbSafe1) -> None
        s = toJson_VehModMngtGlbSafe1(data)
        name = "VehModMngtGlbSafe1"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehModMngtGlbSafe1(self):
        name = "VehModMngtGlbSafe1"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehModMngtGlbSafe1(self, datastring):
        # type: (str) -> datatypes.VehModMngtGlbSafe1
        return fromJson_VehModMngtGlbSafe1(datastring)
    def get_VehModMngtGlbSafe1(self):
        # type: () -> (datatypes.VehModMngtGlbSafe1)
        return self.datamap["VehModMngtGlbSafe1"]


    def send_VehMtnStSafe(self, data):
        # type: (datatypes.VehMtnStSafe1) -> None
        s = toJson_VehMtnStSafe1(data)
        name = "VehMtnStSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehMtnStSafe(self):
        name = "VehMtnStSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehMtnStSafe(self, datastring):
        # type: (str) -> datatypes.VehMtnStSafe1
        return fromJson_VehMtnStSafe1(datastring)
    def get_VehMtnStSafe(self):
        # type: () -> (datatypes.VehMtnStSafe1)
        return self.datamap["VehMtnStSafe"]


    def send_AudWarn(self, data):
        # type: (bool) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "AudWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_AudWarn(self):
        name = "AudWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_AudWarn(self, datastring):
        # type: (str) -> bool
        return json.loads(datastring)
    def get_AudWarn(self):
        # type: () -> (bool)
        return self.datamap["AudWarn"]


    def send_VehSpdAvgIndcd(self, data):
        # type: (datatypes.VehSpdIndcd1) -> None
        s = toJson_VehSpdIndcd1(data)
        name = "VehSpdAvgIndcd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehSpdAvgIndcd(self):
        name = "VehSpdAvgIndcd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehSpdAvgIndcd(self, datastring):
        # type: (str) -> datatypes.VehSpdIndcd1
        return fromJson_VehSpdIndcd1(datastring)
    def get_VehSpdAvgIndcd(self):
        # type: () -> (datatypes.VehSpdIndcd1)
        return self.datamap["VehSpdAvgIndcd"]


    def send_VehSpdCtrlActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "VehSpdCtrlActvSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehSpdCtrlActvSts(self):
        name = "VehSpdCtrlActvSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehSpdCtrlActvSts(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_VehSpdCtrlActvSts(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["VehSpdCtrlActvSts"]


    def send_VehSpdIndcd(self, data):
        # type: (datatypes.VehSpdIndcd1) -> None
        s = toJson_VehSpdIndcd1(data)
        name = "VehSpdIndcd"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehSpdIndcd(self):
        name = "VehSpdIndcd"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehSpdIndcd(self, datastring):
        # type: (str) -> datatypes.VehSpdIndcd1
        return fromJson_VehSpdIndcd1(datastring)
    def get_VehSpdIndcd(self):
        # type: () -> (datatypes.VehSpdIndcd1)
        return self.datamap["VehSpdIndcd"]


    def send_VehSpdLgtSafe(self, data):
        # type: (datatypes.VehSpdLgtSafe1) -> None
        s = toJson_VehSpdLgtSafe1(data)
        name = "VehSpdLgtSafe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VehSpdLgtSafe(self):
        name = "VehSpdLgtSafe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VehSpdLgtSafe(self, datastring):
        # type: (str) -> datatypes.VehSpdLgtSafe1
        return fromJson_VehSpdLgtSafe1(datastring)
    def get_VehSpdLgtSafe(self):
        # type: () -> (datatypes.VehSpdLgtSafe1)
        return self.datamap["VehSpdLgtSafe"]


    def send_Vin(self, data):
        # type: (datatypes.Vin1) -> None
        s = toJson_Vin1(data)
        name = "Vin"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_Vin(self):
        name = "Vin"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_Vin(self, datastring):
        # type: (str) -> datatypes.Vin1
        return fromJson_Vin1(datastring)
    def get_Vin(self):
        # type: () -> (datatypes.Vin1)
        return self.datamap["Vin"]


    def send_VisnAgWideCfg(self, data):
        # type: (datatypes.VisnAgWideCfg1) -> None
        s = toJson_VisnAgWideCfg1(data)
        name = "VisnAgWideCfg"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VisnAgWideCfg(self):
        name = "VisnAgWideCfg"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VisnAgWideCfg(self, datastring):
        # type: (str) -> datatypes.VisnAgWideCfg1
        return fromJson_VisnAgWideCfg1(datastring)
    def get_VisnAgWideCfg(self):
        # type: () -> (datatypes.VisnAgWideCfg1)
        return self.datamap["VisnAgWideCfg"]


    def send_VisnAgWideSts(self, data):
        # type: (datatypes.AvlSts2) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "VisnAgWideSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VisnAgWideSts(self):
        name = "VisnAgWideSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VisnAgWideSts(self, datastring):
        # type: (str) -> datatypes.AvlSts2
        return json.loads(datastring)
    def get_VisnAgWideSts(self):
        # type: () -> datatypes.AvlSts2
        return self.datamap["VisnAgWideSts"]


    def send_VisnImgAgWideInUse(self, data):
        # type: (datatypes.VisnImgAgWideInUse) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "VisnImgAgWideInUse"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VisnImgAgWideInUse(self):
        name = "VisnImgAgWideInUse"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VisnImgAgWideInUse(self, datastring):
        # type: (str) -> datatypes.VisnImgAgWideInUse
        return json.loads(datastring)
    def get_VisnImgAgWideInUse(self):
        # type: () -> datatypes.VisnImgAgWideInUse
        return self.datamap["VisnImgAgWideInUse"]


    def send_VisnImgAgWideReq(self, data):
        # type: (datatypes.VisnImgAgWideInUse) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "VisnImgAgWideReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VisnImgAgWideReq(self):
        name = "VisnImgAgWideReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VisnImgAgWideReq(self, datastring):
        # type: (str) -> datatypes.VisnImgAgWideInUse
        return json.loads(datastring)
    def get_VisnImgAgWideReq(self):
        # type: () -> datatypes.VisnImgAgWideInUse
        return self.datamap["VisnImgAgWideReq"]


    def send_VisnImgCamReq(self, data):
        # type: (datatypes.CamIndReq) -> None
        s = toJson_CamIndReq(data)
        name = "VisnImgCamReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_VisnImgCamReq(self):
        name = "VisnImgCamReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_VisnImgCamReq(self, datastring):
        # type: (str) -> datatypes.CamIndReq
        return fromJson_CamIndReq(datastring)
    def get_VisnImgCamReq(self):
        # type: () -> (datatypes.CamIndReq)
        return self.datamap["VisnImgCamReq"]


    def send_WarnTypForLaneChgWarn(self, data):
        # type: (datatypes.WarnTypForLaneChgWarnPen1) -> None
        s = toJson_WarnTypForLaneChgWarnPen1(data)
        name = "WarnTypForLaneChgWarn"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WarnTypForLaneChgWarn(self):
        name = "WarnTypForLaneChgWarn"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WarnTypForLaneChgWarn(self, datastring):
        # type: (str) -> datatypes.WarnTypForLaneChgWarnPen1
        return fromJson_WarnTypForLaneChgWarnPen1(datastring)
    def get_WarnTypForLaneChgWarn(self):
        # type: () -> (datatypes.WarnTypForLaneChgWarnPen1)
        return self.datamap["WarnTypForLaneChgWarn"]


    def send_WarnTypForLaneKeepAid(self, data):
        # type: (datatypes.WarnTypForLaneChgWarnPen1) -> None
        s = toJson_WarnTypForLaneChgWarnPen1(data)
        name = "WarnTypForLaneKeepAid"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WarnTypForLaneKeepAid(self):
        name = "WarnTypForLaneKeepAid"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WarnTypForLaneKeepAid(self, datastring):
        # type: (str) -> datatypes.WarnTypForLaneChgWarnPen1
        return fromJson_WarnTypForLaneChgWarnPen1(datastring)
    def get_WarnTypForLaneKeepAid(self):
        # type: () -> (datatypes.WarnTypForLaneChgWarnPen1)
        return self.datamap["WarnTypForLaneKeepAid"]


    def send_WhlCircum(self, data):
        # type: (int) -> None
        s = json.dumps(_get_if_not_None(data))
        name = "WhlCircum"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WhlCircum(self):
        name = "WhlCircum"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WhlCircum(self, datastring):
        # type: (str) -> int
        return json.loads(datastring)
    def get_WhlCircum(self):
        # type: () -> (int)
        return self.datamap["WhlCircum"]


    def send_WhlMotSysTqEst(self, data):
        # type: (datatypes.TqSafe2) -> None
        s = toJson_TqSafe2(data)
        name = "WhlMotSysTqEst"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WhlMotSysTqEst(self):
        name = "WhlMotSysTqEst"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WhlMotSysTqEst(self, datastring):
        # type: (str) -> datatypes.TqSafe2
        return fromJson_TqSafe2(datastring)
    def get_WhlMotSysTqEst(self):
        # type: () -> (datatypes.TqSafe2)
        return self.datamap["WhlMotSysTqEst"]


    def send_WhlRotToothCntr(self, data):
        # type: (datatypes.WhlRotToothCntrRec) -> None
        s = toJson_WhlRotToothCntrRec(data)
        name = "WhlRotToothCntr"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WhlRotToothCntr(self):
        name = "WhlRotToothCntr"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WhlRotToothCntr(self, datastring):
        # type: (str) -> datatypes.WhlRotToothCntrRec
        return fromJson_WhlRotToothCntrRec(datastring)
    def get_WhlRotToothCntr(self):
        # type: () -> (datatypes.WhlRotToothCntrRec)
        return self.datamap["WhlRotToothCntr"]


    def send_WinPosnStsAtDrv(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WinPosnStsAtDrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WinPosnStsAtDrv(self):
        name = "WinPosnStsAtDrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WinPosnStsAtDrv(self, datastring):
        # type: (str) -> datatypes.WinAndRoofAndCurtPosnTyp
        return json.loads(datastring)
    def get_WinPosnStsAtDrv(self):
        # type: () -> datatypes.WinAndRoofAndCurtPosnTyp
        return self.datamap["WinPosnStsAtDrv"]


    def send_WinPosnStsAtDrvrRe(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WinPosnStsAtDrvrRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WinPosnStsAtDrvrRe(self):
        name = "WinPosnStsAtDrvrRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WinPosnStsAtDrvrRe(self, datastring):
        # type: (str) -> datatypes.WinAndRoofAndCurtPosnTyp
        return json.loads(datastring)
    def get_WinPosnStsAtDrvrRe(self):
        # type: () -> datatypes.WinAndRoofAndCurtPosnTyp
        return self.datamap["WinPosnStsAtDrvrRe"]


    def send_WinPosnStsAtPass(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WinPosnStsAtPass"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WinPosnStsAtPass(self):
        name = "WinPosnStsAtPass"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WinPosnStsAtPass(self, datastring):
        # type: (str) -> datatypes.WinAndRoofAndCurtPosnTyp
        return json.loads(datastring)
    def get_WinPosnStsAtPass(self):
        # type: () -> datatypes.WinAndRoofAndCurtPosnTyp
        return self.datamap["WinPosnStsAtPass"]


    def send_WinPosnStsAtPassRe(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WinPosnStsAtPassRe"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WinPosnStsAtPassRe(self):
        name = "WinPosnStsAtPassRe"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WinPosnStsAtPassRe(self, datastring):
        # type: (str) -> datatypes.WinAndRoofAndCurtPosnTyp
        return json.loads(datastring)
    def get_WinPosnStsAtPassRe(self):
        # type: () -> datatypes.WinAndRoofAndCurtPosnTyp
        return self.datamap["WinPosnStsAtPassRe"]


    def send_WipgInfo(self, data):
        # type: (datatypes.WipgInfo) -> None
        s = toJson_WipgInfo(data)
        name = "WipgInfo"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WipgInfo(self):
        name = "WipgInfo"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WipgInfo(self, datastring):
        # type: (str) -> datatypes.WipgInfo
        return fromJson_WipgInfo(datastring)
    def get_WipgInfo(self):
        # type: () -> (datatypes.WipgInfo)
        return self.datamap["WipgInfo"]


    def send_WiprFrntSrvModReq(self, data):
        # type: (datatypes.WiprFrntSrvModReq) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WiprFrntSrvModReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WiprFrntSrvModReq(self):
        name = "WiprFrntSrvModReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WiprFrntSrvModReq(self, datastring):
        # type: (str) -> datatypes.WiprFrntSrvModReq
        return json.loads(datastring)
    def get_WiprFrntSrvModReq(self):
        # type: () -> datatypes.WiprFrntSrvModReq
        return self.datamap["WiprFrntSrvModReq"]


    def send_WiprInPosnForSrv(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WiprInPosnForSrv"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WiprInPosnForSrv(self):
        name = "WiprInPosnForSrv"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WiprInPosnForSrv(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_WiprInPosnForSrv(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["WiprInPosnForSrv"]


    def send_WiprReAutReq(self, data):
        # type: (datatypes.OnOffPen) -> None
        s = toJson_OnOffPen(data)
        name = "WiprReAutReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WiprReAutReq(self):
        name = "WiprReAutReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WiprReAutReq(self, datastring):
        # type: (str) -> datatypes.OnOffPen
        return fromJson_OnOffPen(datastring)
    def get_WiprReAutReq(self):
        # type: () -> (datatypes.OnOffPen)
        return self.datamap["WiprReAutReq"]


    def send_WshrFldSts1WdElmHMI(self, data):
        # type: (datatypes.OnOff1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "WshrFldSts1WdElmHMI"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_WshrFldSts1WdElmHMI(self):
        name = "WshrFldSts1WdElmHMI"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_WshrFldSts1WdElmHMI(self, datastring):
        # type: (str) -> datatypes.OnOff1
        return json.loads(datastring)
    def get_WshrFldSts1WdElmHMI(self):
        # type: () -> datatypes.OnOff1
        return self.datamap["WshrFldSts1WdElmHMI"]


    def send_iTPMSCalPsbl(self, data):
        # type: (datatypes.CalPsbl) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "iTPMSCalPsbl"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_iTPMSCalPsbl(self):
        name = "iTPMSCalPsbl"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_iTPMSCalPsbl(self, datastring):
        # type: (str) -> datatypes.CalPsbl
        return json.loads(datastring)
    def get_iTPMSCalPsbl(self):
        # type: () -> datatypes.CalPsbl
        return self.datamap["iTPMSCalPsbl"]


    def send_iTPMSCalReq(self, data):
        # type: (datatypes.StrtStopReq1) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "iTPMSCalReq"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_iTPMSCalReq(self):
        name = "iTPMSCalReq"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_iTPMSCalReq(self, datastring):
        # type: (str) -> datatypes.StrtStopReq1
        return json.loads(datastring)
    def get_iTPMSCalReq(self):
        # type: () -> datatypes.StrtStopReq1
        return self.datamap["iTPMSCalReq"]


    def send_iTPMSCalSts(self, data):
        # type: (datatypes.iTPMSTirePCalSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "iTPMSCalSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_iTPMSCalSts(self):
        name = "iTPMSCalSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_iTPMSCalSts(self, datastring):
        # type: (str) -> datatypes.iTPMSTirePCalSts
        return json.loads(datastring)
    def get_iTPMSCalSts(self):
        # type: () -> datatypes.iTPMSTirePCalSts
        return self.datamap["iTPMSCalSts"]


    def send_iTPMSTirePMSts(self, data):
        # type: (datatypes.iTPMSTirePMSts) -> None
        s = json.dumps(int(_extract_enum_value(data)))
        name = "iTPMSTirePMSts"
        self.connection.send_serialized_dataelement(name, type_map[name], is_insignal_map[name], s)

    def stop_iTPMSTirePMSts(self):
        name = "iTPMSTirePMSts"
        self.connection.send_error(name, is_insignal_map[name], type_map[name])

    def _parse_iTPMSTirePMSts(self, datastring):
        # type: (str) -> datatypes.iTPMSTirePMSts
        return json.loads(datastring)
    def get_iTPMSTirePMSts(self):
        # type: () -> datatypes.iTPMSTirePMSts
        return self.datamap["iTPMSTirePMSts"]



def fromJson_ADataRawSafe1(data):
    # type: (str) -> datatypes.ADataRawSafe1
    j = json.loads(data)
    v = datatypes.ADataRawSafe1()
    v.ALat = j["ALat"]
    v.ALat1Qf = j["ALat1Qf"]
    v.ALgt = j["ALgt"]
    v.ALgt1Qf = j["ALgt1Qf"]
    v.AVert = j["AVert"]
    v.AVertQf = j["AVertQf"]
    v.Chks = j["Chks"]
    v.Cntr = j["Cntr"]
    return v


def toJson_ADataRawSafe1(v):
    # type: (datatypes.ADataRawSafe1) -> str
    return json.dumps({
        "ALat" : _get_if_not_None(v.ALat),
        "ALat1Qf" : _extract_enum_value(_get_if_not_None(v.ALat1Qf)),

        "ALgt" : _get_if_not_None(v.ALgt),
        "ALgt1Qf" : _extract_enum_value(_get_if_not_None(v.ALgt1Qf)),

        "AVert" : _get_if_not_None(v.AVert),
        "AVertQf" : _extract_enum_value(_get_if_not_None(v.AVertQf)),

        "Chks" : _get_if_not_None(v.Chks),
        "Cntr" : _get_if_not_None(v.Cntr),

    })
def fromJson_ActvnOfCrsEco(data):
    # type: (str) -> datatypes.ActvnOfCrsEco
    j = json.loads(data)
    v = datatypes.ActvnOfCrsEco()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_ActvnOfCrsEco(v):
    # type: (datatypes.ActvnOfCrsEco) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_AdprTurnSpdModPen1(data):
    # type: (str) -> datatypes.AdprTurnSpdModPen1
    j = json.loads(data)
    v = datatypes.AdprTurnSpdModPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_AdprTurnSpdModPen1(v):
    # type: (datatypes.AdprTurnSpdModPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_AgDataRawSafe1(data):
    # type: (str) -> datatypes.AgDataRawSafe1
    j = json.loads(data)
    v = datatypes.AgDataRawSafe1()
    v.RollRate = j["RollRate"]
    v.RollRateQf = j["RollRateQf"]
    v.YawRate = j["YawRate"]
    v.YawRateQf = j["YawRateQf"]
    v.Chks = j["Chks"]
    v.Cntr = j["Cntr"]
    return v


def toJson_AgDataRawSafe1(v):
    # type: (datatypes.AgDataRawSafe1) -> str
    return json.dumps({
        "RollRate" : _get_if_not_None(v.RollRate),
        "RollRateQf" : _extract_enum_value(_get_if_not_None(v.RollRateQf)),

        "YawRate" : _get_if_not_None(v.YawRate),
        "YawRateQf" : _extract_enum_value(_get_if_not_None(v.YawRateQf)),

        "Chks" : _get_if_not_None(v.Chks),
        "Cntr" : _get_if_not_None(v.Cntr),

    })
def fromJson_AmbTIndcdWithUnit(data):
    # type: (str) -> datatypes.AmbTIndcdWithUnit
    j = json.loads(data)
    v = datatypes.AmbTIndcdWithUnit()
    v.AmbTIndcd = j["AmbTIndcd"]
    v.AmbTIndcdUnit = j["AmbTIndcdUnit_"]
    v.QF = j["QF"]
    return v


def toJson_AmbTIndcdWithUnit(v):
    # type: (datatypes.AmbTIndcdWithUnit) -> str
    return json.dumps({
        "AmbTIndcd" : _get_if_not_None(v.AmbTIndcd),
        "AmbTIndcdUnit_" : _extract_enum_value(_get_if_not_None(v.AmbTIndcdUnit)),

        "QF" : _extract_enum_value(_get_if_not_None(v.QF)),


    })
def fromJson_AmbTWithQly(data):
    # type: (str) -> datatypes.AmbTWithQly
    j = json.loads(data)
    v = datatypes.AmbTWithQly()
    v.AmbTVal = j["AmbTVal"]
    v.Qly = j["Qly"]
    return v


def toJson_AmbTWithQly(v):
    # type: (datatypes.AmbTWithQly) -> str
    return json.dumps({
        "AmbTVal" : _get_if_not_None(v.AmbTVal),
        "Qly" : _extract_enum_value(_get_if_not_None(v.Qly)),


    })
def fromJson_AsyLaneChg1(data):
    # type: (str) -> datatypes.AsyLaneChg1
    j = json.loads(data)
    v = datatypes.AsyLaneChg1()
    v.Typ = j["Typ"]
    v.Sts = j["Sts"]
    v.Psbl = j["Psbl"]
    return v


def toJson_AsyLaneChg1(v):
    # type: (datatypes.AsyLaneChg1) -> str
    return json.dumps({
        "Typ" : _extract_enum_value(_get_if_not_None(v.Typ)),

        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Psbl" : _extract_enum_value(_get_if_not_None(v.Psbl)),


    })
def fromJson_AutnmsPrkgActvScenario(data):
    # type: (str) -> datatypes.AutnmsPrkgActvScenario
    j = json.loads(data)
    v = datatypes.AutnmsPrkgActvScenario()
    v.APActiveState = j["APActiveState"]
    v.APActiveParkingSide = j["APActiveParkingSide"]
    v.APActiveSlotType = j["APActiveSlotType"]
    v.APActiveParkingScenario = j["APActiveParkingScenario"]
    v.APActiveParkingDirection = j["APActiveParkingDirection"]
    v.APActiveRemote = j["APActiveRemote"]
    v.APPauseStatus = j["APPauseStatus"]
    v.APCancelStatus = j["APCancelStatus"]
    v.APActiveDirectParking = j["APActiveDirectParking"]
    return v


def toJson_AutnmsPrkgActvScenario(v):
    # type: (datatypes.AutnmsPrkgActvScenario) -> str
    return json.dumps({
        "APActiveState" : _extract_enum_value(_get_if_not_None(v.APActiveState)),

        "APActiveParkingSide" : _extract_enum_value(_get_if_not_None(v.APActiveParkingSide)),

        "APActiveSlotType" : _extract_enum_value(_get_if_not_None(v.APActiveSlotType)),

        "APActiveParkingScenario" : _extract_enum_value(_get_if_not_None(v.APActiveParkingScenario)),

        "APActiveParkingDirection" : _extract_enum_value(_get_if_not_None(v.APActiveParkingDirection)),

        "APActiveRemote" : _extract_enum_value(_get_if_not_None(v.APActiveRemote)),

        "APPauseStatus" : _extract_enum_value(_get_if_not_None(v.APPauseStatus)),

        "APCancelStatus" : _extract_enum_value(_get_if_not_None(v.APCancelStatus)),

        "APActiveDirectParking" : _extract_enum_value(_get_if_not_None(v.APActiveDirectParking)),


    })
def fromJson_AutnmsPrkgAvlScenario(data):
    # type: (str) -> datatypes.AutnmsPrkgAvlScenario
    j = json.loads(data)
    v = datatypes.AutnmsPrkgAvlScenario()
    v.APAvailableParkingSide = j["APAvailableParkingSide"]
    v.APAvailableParkingScenario = j["APAvailableParkingScenario"]
    v.APAvailableSlotType = j["APAvailableSlotType"]
    v.APAvailableParkingDirection = j["APAvailableParkingDirection"]
    v.APAvailableRemote = j["APAvailableRemote"]
    v.APRecommendedParkingSide = j["APRecommendedParkingSide"]
    v.APRecommendedSlotType = j["APRecommendedSlotType"]
    v.APRecommendedParkingDirection = j["APRecommendedParkingDirection"]
    v.APRecommendedRemote = j["APRecommendedRemote"]
    v.APRecommendedParkingScenario = j["APRecommendedParkingScenario"]
    v.APAvailableDirectParking = j["APAvailableDirectParking"]
    return v


def toJson_AutnmsPrkgAvlScenario(v):
    # type: (datatypes.AutnmsPrkgAvlScenario) -> str
    return json.dumps({
        "APAvailableParkingSide" : _extract_enum_value(_get_if_not_None(v.APAvailableParkingSide)),

        "APAvailableParkingScenario" : _extract_enum_value(_get_if_not_None(v.APAvailableParkingScenario)),

        "APAvailableSlotType" : _extract_enum_value(_get_if_not_None(v.APAvailableSlotType)),

        "APAvailableParkingDirection" : _extract_enum_value(_get_if_not_None(v.APAvailableParkingDirection)),

        "APAvailableRemote" : _extract_enum_value(_get_if_not_None(v.APAvailableRemote)),

        "APRecommendedParkingSide" : _extract_enum_value(_get_if_not_None(v.APRecommendedParkingSide)),

        "APRecommendedSlotType" : _extract_enum_value(_get_if_not_None(v.APRecommendedSlotType)),

        "APRecommendedParkingDirection" : _extract_enum_value(_get_if_not_None(v.APRecommendedParkingDirection)),

        "APRecommendedRemote" : _extract_enum_value(_get_if_not_None(v.APRecommendedRemote)),

        "APRecommendedParkingScenario" : _extract_enum_value(_get_if_not_None(v.APRecommendedParkingScenario)),

        "APAvailableDirectParking" : _extract_enum_value(_get_if_not_None(v.APAvailableDirectParking)),


    })
def fromJson_AutnmsPrkgSeldScenario(data):
    # type: (str) -> datatypes.AutnmsPrkgSeldScenario
    j = json.loads(data)
    v = datatypes.AutnmsPrkgSeldScenario()
    v.AutnmsPrkgSeldSlotTyp = j["AutnmsPrkgSeldSlotTyp_"]
    v.AutnmsPrkgSeldPrkgScenario = j["AutnmsPrkgSeldPrkgScenario_"]
    v.AutnmsPrkgSeldPrkgDir = j["AutnmsPrkgSeldPrkgDir_"]
    v.AutnmsPrkgSeldPrkgSide = j["AutnmsPrkgSeldPrkgSide_"]
    v.AutnmsPrkgSeldRem = j["AutnmsPrkgSeldRem_"]
    v.AutnmsPrkgSeldDirectPrkg = j["AutnmsPrkgSeldDirectPrkg_"]
    return v


def toJson_AutnmsPrkgSeldScenario(v):
    # type: (datatypes.AutnmsPrkgSeldScenario) -> str
    return json.dumps({
        "AutnmsPrkgSeldSlotTyp_" : _extract_enum_value(_get_if_not_None(v.AutnmsPrkgSeldSlotTyp)),

        "AutnmsPrkgSeldPrkgScenario_" : _extract_enum_value(_get_if_not_None(v.AutnmsPrkgSeldPrkgScenario)),

        "AutnmsPrkgSeldPrkgDir_" : _extract_enum_value(_get_if_not_None(v.AutnmsPrkgSeldPrkgDir)),

        "AutnmsPrkgSeldPrkgSide_" : _extract_enum_value(_get_if_not_None(v.AutnmsPrkgSeldPrkgSide)),

        "AutnmsPrkgSeldRem_" : _extract_enum_value(_get_if_not_None(v.AutnmsPrkgSeldRem)),

        "AutnmsPrkgSeldDirectPrkg_" : _extract_enum_value(_get_if_not_None(v.AutnmsPrkgSeldDirectPrkg)),


    })
def fromJson_BltLockStFrnt(data):
    # type: (str) -> datatypes.BltLockStFrnt
    j = json.loads(data)
    v = datatypes.BltLockStFrnt()
    v.BltLockSt1 = j["BltLockSt1_"]
    v.BltLockSts = j["BltLockSts"]
    return v


def toJson_BltLockStFrnt(v):
    # type: (datatypes.BltLockStFrnt) -> str
    return json.dumps({
        "BltLockSt1_" : _extract_enum_value(_get_if_not_None(v.BltLockSt1)),

        "BltLockSts" : _extract_enum_value(_get_if_not_None(v.BltLockSts)),


    })
def fromJson_BltLockStRe(data):
    # type: (str) -> datatypes.BltLockStRe
    j = json.loads(data)
    v = datatypes.BltLockStRe()
    v.BltLockSt1 = j["BltLockSt1_"]
    v.BltLockSts = j["BltLockSts"]
    v.BltLockEquid = j["BltLockEquid"]
    return v


def toJson_BltLockStRe(v):
    # type: (datatypes.BltLockStRe) -> str
    return json.dumps({
        "BltLockSt1_" : _extract_enum_value(_get_if_not_None(v.BltLockSt1)),

        "BltLockSts" : _extract_enum_value(_get_if_not_None(v.BltLockSts)),

        "BltLockEquid" : _extract_enum_value(_get_if_not_None(v.BltLockEquid)),


    })
def fromJson_BrkAndAbsWarnIndcnReqRec1(data):
    # type: (str) -> datatypes.BrkAndAbsWarnIndcnReqRec1
    j = json.loads(data)
    v = datatypes.BrkAndAbsWarnIndcnReqRec1()
    v.BrkWarnIndcnReq = j["BrkWarnIndcnReq"]
    v.AbsWarnIndcnReq = j["AbsWarnIndcnReq_"]
    v.BrkAndAbsWarnIndcnReqChks = j["BrkAndAbsWarnIndcnReqChks"]
    v.BrkAndAbsWarnIndcnReqCntr = j["BrkAndAbsWarnIndcnReqCntr"]
    return v


def toJson_BrkAndAbsWarnIndcnReqRec1(v):
    # type: (datatypes.BrkAndAbsWarnIndcnReqRec1) -> str
    return json.dumps({
        "BrkWarnIndcnReq" : _extract_enum_value(_get_if_not_None(v.BrkWarnIndcnReq)),

        "AbsWarnIndcnReq_" : _extract_enum_value(_get_if_not_None(v.AbsWarnIndcnReq)),

        "BrkAndAbsWarnIndcnReqChks" : _get_if_not_None(v.BrkAndAbsWarnIndcnReqChks),
        "BrkAndAbsWarnIndcnReqCntr" : _get_if_not_None(v.BrkAndAbsWarnIndcnReqCntr),

    })
def fromJson_BrkFricTqAtWhlActRec(data):
    # type: (str) -> datatypes.BrkFricTqAtWhlActRec
    j = json.loads(data)
    v = datatypes.BrkFricTqAtWhlActRec()
    v.BrkFricTqAtWhlFrntLeAct = j["BrkFricTqAtWhlFrntLeAct"]
    v.BrkFricTqAtWhlFrntRiAct = j["BrkFricTqAtWhlFrntRiAct"]
    v.BrkFricTqAtWhlReLeAct = j["BrkFricTqAtWhlReLeAct"]
    v.BrkFricTqAtWhlReRiAct = j["BrkFricTqAtWhlReRiAct"]
    return v


def toJson_BrkFricTqAtWhlActRec(v):
    # type: (datatypes.BrkFricTqAtWhlActRec) -> str
    return json.dumps({
        "BrkFricTqAtWhlFrntLeAct" : _get_if_not_None(v.BrkFricTqAtWhlFrntLeAct),
        "BrkFricTqAtWhlFrntRiAct" : _get_if_not_None(v.BrkFricTqAtWhlFrntRiAct),
        "BrkFricTqAtWhlReLeAct" : _get_if_not_None(v.BrkFricTqAtWhlReLeAct),
        "BrkFricTqAtWhlReRiAct" : _get_if_not_None(v.BrkFricTqAtWhlReRiAct),

    })
def fromJson_CamIndReq(data):
    # type: (str) -> datatypes.CamIndReq
    j = json.loads(data)
    v = datatypes.CamIndReq()
    v.CamFrnt = j["CamFrnt"]
    v.CamRe = j["CamRe"]
    v.CamLe = j["CamLe"]
    v.CamRi = j["CamRi"]
    return v


def toJson_CamIndReq(v):
    # type: (datatypes.CamIndReq) -> str
    return json.dumps({
        "CamFrnt" : _extract_enum_value(_get_if_not_None(v.CamFrnt)),

        "CamRe" : _extract_enum_value(_get_if_not_None(v.CamRe)),

        "CamLe" : _extract_enum_value(_get_if_not_None(v.CamLe)),

        "CamRi" : _extract_enum_value(_get_if_not_None(v.CamRi)),


    })
def fromJson_CchForFuEco1(data):
    # type: (str) -> datatypes.CchForFuEco1
    j = json.loads(data)
    v = datatypes.CchForFuEco1()
    v.BarForFuEco = j["BarForFuEco"]
    v.GrdForFuEco = j["GrdForFuEco"]
    return v


def toJson_CchForFuEco1(v):
    # type: (datatypes.CchForFuEco1) -> str
    return json.dumps({
        "BarForFuEco" : _get_if_not_None(v.BarForFuEco),
        "GrdForFuEco" : _get_if_not_None(v.GrdForFuEco),

    })
def fromJson_CllsnWarnSide1(data):
    # type: (str) -> datatypes.CllsnWarnSide1
    j = json.loads(data)
    v = datatypes.CllsnWarnSide1()
    v.Le = j["Le"]
    v.Ri = j["Ri"]
    return v


def toJson_CllsnWarnSide1(v):
    # type: (datatypes.CllsnWarnSide1) -> str
    return json.dumps({
        "Le" : _extract_enum_value(_get_if_not_None(v.Le)),

        "Ri" : _extract_enum_value(_get_if_not_None(v.Ri)),


    })
def fromJson_CmptmtAirTEstimdExtd(data):
    # type: (str) -> datatypes.CmptmtAirTEstimdExtd
    j = json.loads(data)
    v = datatypes.CmptmtAirTEstimdExtd()
    v.ComptmtT = j["ComptmtT"]
    v.QlyFlg = j["QlyFlg"]
    return v


def toJson_CmptmtAirTEstimdExtd(v):
    # type: (datatypes.CmptmtAirTEstimdExtd) -> str
    return json.dumps({
        "ComptmtT" : _get_if_not_None(v.ComptmtT),
        "QlyFlg" : _extract_enum_value(_get_if_not_None(v.QlyFlg)),


    })
def fromJson_CmptmtTFrnt(data):
    # type: (str) -> datatypes.CmptmtTFrnt
    j = json.loads(data)
    v = datatypes.CmptmtTFrnt()
    v.CmptmtTFrnt = j["CmptmtTFrnt"]
    v.CmptmtTFrntQf = j["CmptmtTFrntQf_"]
    v.FanForCmptmtTRunng = j["FanForCmptmtTRunng"]
    return v


def toJson_CmptmtTFrnt(v):
    # type: (datatypes.CmptmtTFrnt) -> str
    return json.dumps({
        "CmptmtTFrnt" : _get_if_not_None(v.CmptmtTFrnt),
        "CmptmtTFrntQf_" : _extract_enum_value(_get_if_not_None(v.CmptmtTFrntQf)),

        "FanForCmptmtTRunng" : _extract_enum_value(_get_if_not_None(v.FanForCmptmtTRunng)),


    })
def fromJson_CnsPrmRec1(data):
    # type: (str) -> datatypes.CnsPrmRec1
    j = json.loads(data)
    v = datatypes.CnsPrmRec1()
    v.SpdVal = j["SpdVal"]
    v.SpdIdx = j["SpdIdx"]
    v.AuxPwrPrsnt = j["AuxPwrPrsnt"]
    v.AuxPwrLvlInct = j["AuxPwrLvlInct"]
    v.AuxTiPrsnt = j["AuxTiPrsnt"]
    v.AuxTiTranPha = j["AuxTiTranPha"]
    v.AVal = j["AVal"]
    return v


def toJson_CnsPrmRec1(v):
    # type: (datatypes.CnsPrmRec1) -> str
    return json.dumps({
        "SpdVal" : _get_if_not_None(v.SpdVal),
        "SpdIdx" : _get_if_not_None(v.SpdIdx),
        "AuxPwrPrsnt" : _get_if_not_None(v.AuxPwrPrsnt),
        "AuxPwrLvlInct" : _get_if_not_None(v.AuxPwrLvlInct),
        "AuxTiPrsnt" : _get_if_not_None(v.AuxTiPrsnt),
        "AuxTiTranPha" : _get_if_not_None(v.AuxTiTranPha),
        "AVal" : _get_if_not_None(v.AVal),

    })
def fromJson_ConSftyWarn1(data):
    # type: (str) -> datatypes.ConSftyWarn1
    j = json.loads(data)
    v = datatypes.ConSftyWarn1()
    v.ConSftyWarnId = j["ConSftyWarnId"]
    v.ConSftyWarnSnd = j["ConSftyWarnSnd"]
    v.ConSftyWarnTyp = j["ConSftyWarnTyp"]
    v.ConSftyWarnLvl = j["ConSftyWarnLvl"]
    v.DistanceToWarning = j["DistanceToWarning"]
    return v


def toJson_ConSftyWarn1(v):
    # type: (datatypes.ConSftyWarn1) -> str
    return json.dumps({
        "ConSftyWarnId" : _get_if_not_None(v.ConSftyWarnId),
        "ConSftyWarnSnd" : _extract_enum_value(_get_if_not_None(v.ConSftyWarnSnd)),

        "ConSftyWarnTyp" : _get_if_not_None(v.ConSftyWarnTyp),
        "ConSftyWarnLvl" : _extract_enum_value(_get_if_not_None(v.ConSftyWarnLvl)),

        "DistanceToWarning" : _get_if_not_None(v.DistanceToWarning),

    })
def fromJson_DataSpclTyp(data):
    # type: (str) -> datatypes.DataSpclTyp
    j = json.loads(data)
    v = datatypes.DataSpclTyp()
    v.DataIdn = j["DataIdn"]
    v.DataNrSpcl1 = j["DataNrSpcl1"]
    v.DataNrSpcl2 = j["DataNrSpcl2"]
    v.DataNrSpcl3 = j["DataNrSpcl3"]
    return v


def toJson_DataSpclTyp(v):
    # type: (datatypes.DataSpclTyp) -> str
    return json.dumps({
        "DataIdn" : _get_if_not_None(v.DataIdn),
        "DataNrSpcl1" : _get_if_not_None(v.DataNrSpcl1),
        "DataNrSpcl2" : _get_if_not_None(v.DataNrSpcl2),
        "DataNrSpcl3" : _get_if_not_None(v.DataNrSpcl3),

    })
def fromJson_DateTi1ForSet(data):
    # type: (str) -> datatypes.DateTi1ForSet
    j = json.loads(data)
    v = datatypes.DateTi1ForSet()
    v.DateOrTi = j["DateOrTi_"]
    v.Year = j["Year"]
    v.Month = j["Month"]
    v.Day = j["Day"]
    v.Hour = j["Hour"]
    v.Minute = j["Minute"]
    v.Second = j["Second"]
    return v


def toJson_DateTi1ForSet(v):
    # type: (datatypes.DateTi1ForSet) -> str
    return json.dumps({
        "DateOrTi_" : _extract_enum_value(_get_if_not_None(v.DateOrTi)),

        "Year" : _get_if_not_None(v.Year),
        "Month" : _get_if_not_None(v.Month),
        "Day" : _get_if_not_None(v.Day),
        "Hour" : _get_if_not_None(v.Hour),
        "Minute" : _get_if_not_None(v.Minute),
        "Second" : _get_if_not_None(v.Second),

    })
def fromJson_DateTi30(data):
    # type: (str) -> datatypes.DateTi30
    j = json.loads(data)
    v = datatypes.DateTi30()
    v.Yr1 = j["Yr1"]
    v.Mth1 = j["Mth1"]
    v.Day = j["Day"]
    v.Hr1 = j["Hr1"]
    v.Mins1 = j["Mins1"]
    v.Sec1 = j["Sec1"]
    v.DataValid = j["DataValid"]
    return v


def toJson_DateTi30(v):
    # type: (datatypes.DateTi30) -> str
    return json.dumps({
        "Yr1" : _get_if_not_None(v.Yr1),
        "Mth1" : _get_if_not_None(v.Mth1),
        "Day" : _get_if_not_None(v.Day),
        "Hr1" : _get_if_not_None(v.Hr1),
        "Mins1" : _get_if_not_None(v.Mins1),
        "Sec1" : _get_if_not_None(v.Sec1),
        "DataValid" : _extract_enum_value(_get_if_not_None(v.DataValid)),


    })
def fromJson_DriveAwayInfoWarnReq1(data):
    # type: (str) -> datatypes.DriveAwayInfoWarnReq1
    j = json.loads(data)
    v = datatypes.DriveAwayInfoWarnReq1()
    v.ReqSrc = j["ReqSrc"]
    v.TiToDrvCntDwnTi = j["TiToDrvCntDwnTi"]
    v.CtrlDirOfTrfcLi = j["CtrlDirOfTrfcLi"]
    v.VisWarnReq = j["VisWarnReq"]
    v.AudWarnReq = j["AudWarnReq"]
    v.InterruptionSrc = j["InterruptionSrc"]
    return v


def toJson_DriveAwayInfoWarnReq1(v):
    # type: (datatypes.DriveAwayInfoWarnReq1) -> str
    return json.dumps({
        "ReqSrc" : _extract_enum_value(_get_if_not_None(v.ReqSrc)),

        "TiToDrvCntDwnTi" : _get_if_not_None(v.TiToDrvCntDwnTi),
        "CtrlDirOfTrfcLi" : _extract_enum_value(_get_if_not_None(v.CtrlDirOfTrfcLi)),

        "VisWarnReq" : _extract_enum_value(_get_if_not_None(v.VisWarnReq)),

        "AudWarnReq" : _extract_enum_value(_get_if_not_None(v.AudWarnReq)),

        "InterruptionSrc" : _extract_enum_value(_get_if_not_None(v.InterruptionSrc)),


    })
def fromJson_DrvrDispSetgReq(data):
    # type: (str) -> datatypes.DrvrDispSetgReq
    j = json.loads(data)
    v = datatypes.DrvrDispSetgReq()
    v.Pen = j["Pen"]
    v.Sts = j["Sts"]
    return v


def toJson_DrvrDispSetgReq(v):
    # type: (datatypes.DrvrDispSetgReq) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),


    })
def fromJson_DrvrHmiBackGndInfoSetg(data):
    # type: (str) -> datatypes.DrvrHmiBackGndInfoSetg
    j = json.loads(data)
    v = datatypes.DrvrHmiBackGndInfoSetg()
    v.Pen = j["Pen"]
    v.Setg = j["Setg"]
    return v


def toJson_DrvrHmiBackGndInfoSetg(v):
    # type: (datatypes.DrvrHmiBackGndInfoSetg) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "Setg" : _extract_enum_value(_get_if_not_None(v.Setg)),


    })
def fromJson_DrvrHmiDispdModPen(data):
    # type: (str) -> datatypes.DrvrHmiDispdModPen
    j = json.loads(data)
    v = datatypes.DrvrHmiDispdModPen()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_DrvrHmiDispdModPen(v):
    # type: (datatypes.DrvrHmiDispdModPen) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_DrvrHmiUserIfSetgReq(data):
    # type: (str) -> datatypes.DrvrHmiUserIfSetgReq
    j = json.loads(data)
    v = datatypes.DrvrHmiUserIfSetgReq()
    v.Pen = j["Pen"]
    v.Setg = j["Setg"]
    return v


def toJson_DrvrHmiUserIfSetgReq(v):
    # type: (datatypes.DrvrHmiUserIfSetgReq) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "Setg" : _extract_enum_value(_get_if_not_None(v.Setg)),


    })
def fromJson_DrvrSeatSwtSts1(data):
    # type: (str) -> datatypes.DrvrSeatSwtSts1
    j = json.loads(data)
    v = datatypes.DrvrSeatSwtSts1()
    v.DrvrSeatSwtSldSts = j["DrvrSeatSwtSldSts"]
    v.DrvrSeatSwtHeiSts = j["DrvrSeatSwtHeiSts"]
    v.DrvrSeatSwtHeiFrntSts = j["DrvrSeatSwtHeiFrntSts"]
    v.DrvrSeatSwtInclSts = j["DrvrSeatSwtInclSts"]
    v.DrvrSeatSwtAdjmtOfSpplFctHozlSts = j["DrvrSeatSwtAdjmtOfSpplFctHozlSts"]
    v.DrvrSeatSwtAdjmtOfSpplFctVertSts = j["DrvrSeatSwtAdjmtOfSpplFctVertSts"]
    v.DrvrSeatSwtSelnOfSpplFctSts = j["DrvrSeatSwtSelnOfSpplFctSts"]
    return v


def toJson_DrvrSeatSwtSts1(v):
    # type: (datatypes.DrvrSeatSwtSts1) -> str
    return json.dumps({
        "DrvrSeatSwtSldSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtSldSts)),

        "DrvrSeatSwtHeiSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtHeiSts)),

        "DrvrSeatSwtHeiFrntSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtHeiFrntSts)),

        "DrvrSeatSwtInclSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtInclSts)),

        "DrvrSeatSwtAdjmtOfSpplFctHozlSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtAdjmtOfSpplFctHozlSts)),

        "DrvrSeatSwtAdjmtOfSpplFctVertSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtAdjmtOfSpplFctVertSts)),

        "DrvrSeatSwtSelnOfSpplFctSts" : _extract_enum_value(_get_if_not_None(v.DrvrSeatSwtSelnOfSpplFctSts)),


    })
def fromJson_DstToEmptyWithUnit(data):
    # type: (str) -> datatypes.DstToEmptyWithUnit
    j = json.loads(data)
    v = datatypes.DstToEmptyWithUnit()
    v.DstToEmpty = j["DstToEmpty"]
    v.DstUnit = j["DstUnit_"]
    return v


def toJson_DstToEmptyWithUnit(v):
    # type: (datatypes.DstToEmptyWithUnit) -> str
    return json.dumps({
        "DstToEmpty" : _get_if_not_None(v.DstToEmpty),
        "DstUnit_" : _extract_enum_value(_get_if_not_None(v.DstUnit)),


    })
def fromJson_DstToManvLocnByNav(data):
    # type: (str) -> datatypes.DstToManvLocnByNav
    j = json.loads(data)
    v = datatypes.DstToManvLocnByNav()
    v.DstToManv = j["DstToManv"]
    v.CntDwnToManvStrt = j["CntDwnToManvStrt"]
    v.PosnQly = j["PosnQly"]
    v.SpprtForFct = j["SpprtForFct"]
    return v


def toJson_DstToManvLocnByNav(v):
    # type: (datatypes.DstToManvLocnByNav) -> str
    return json.dumps({
        "DstToManv" : _get_if_not_None(v.DstToManv),
        "CntDwnToManvStrt" : _extract_enum_value(_get_if_not_None(v.CntDwnToManvStrt)),

        "PosnQly" : _extract_enum_value(_get_if_not_None(v.PosnQly)),

        "SpprtForFct" : _extract_enum_value(_get_if_not_None(v.SpprtForFct)),


    })
def fromJson_EgyCostForRouteRec1(data):
    # type: (str) -> datatypes.EgyCostForRouteRec1
    j = json.loads(data)
    v = datatypes.EgyCostForRouteRec1()
    v.Egy = j["Egy"]
    v.Idx = j["Idx"]
    return v


def toJson_EgyCostForRouteRec1(v):
    # type: (datatypes.EgyCostForRouteRec1) -> str
    return json.dumps({
        "Egy" : _get_if_not_None(v.Egy),
        "Idx" : _get_if_not_None(v.Idx),

    })
def fromJson_EngIdleEco1(data):
    # type: (str) -> datatypes.EngIdleEco1
    j = json.loads(data)
    v = datatypes.EngIdleEco1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_EngIdleEco1(v):
    # type: (datatypes.EngIdleEco1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_EngNSafe(data):
    # type: (str) -> datatypes.EngNSafe
    j = json.loads(data)
    v = datatypes.EngNSafe()
    v.EngN = j["EngN"]
    v.EngNGrdt = j["EngNGrdt"]
    v.EngNChks = j["EngNChks"]
    v.EngNCntr = j["EngNCntr"]
    return v


def toJson_EngNSafe(v):
    # type: (datatypes.EngNSafe) -> str
    return json.dumps({
        "EngN" : _get_if_not_None(v.EngN),
        "EngNGrdt" : _get_if_not_None(v.EngNGrdt),
        "EngNChks" : _get_if_not_None(v.EngNChks),
        "EngNCntr" : _get_if_not_None(v.EngNCntr),

    })
def fromJson_EpbLampReqRec(data):
    # type: (str) -> datatypes.EpbLampReqRec
    j = json.loads(data)
    v = datatypes.EpbLampReqRec()
    v.EpbLampReq = j["EpbLampReq"]
    v.EpbLampReqChks = j["EpbLampReqChks"]
    v.EpbLampReqCntr = j["EpbLampReqCntr"]
    return v


def toJson_EpbLampReqRec(v):
    # type: (datatypes.EpbLampReqRec) -> str
    return json.dumps({
        "EpbLampReq" : _extract_enum_value(_get_if_not_None(v.EpbLampReq)),

        "EpbLampReqChks" : _get_if_not_None(v.EpbLampReqChks),
        "EpbLampReqCntr" : _get_if_not_None(v.EpbLampReqCntr),

    })
def fromJson_EscSptModReqdByDrvrRec1(data):
    # type: (str) -> datatypes.EscSptModReqdByDrvrRec1
    j = json.loads(data)
    v = datatypes.EscSptModReqdByDrvrRec1()
    v.EscSptModReqdByDrvr = j["EscSptModReqdByDrvr"]
    v.Pen = j["Pen"]
    return v


def toJson_EscSptModReqdByDrvrRec1(v):
    # type: (datatypes.EscSptModReqdByDrvrRec1) -> str
    return json.dumps({
        "EscSptModReqdByDrvr" : _extract_enum_value(_get_if_not_None(v.EscSptModReqdByDrvr)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_EscStSafe1(data):
    # type: (str) -> datatypes.EscStSafe1
    j = json.loads(data)
    v = datatypes.EscStSafe1()
    v.EscSt = j["EscSt"]
    v.EscStChks = j["EscStChks"]
    v.EscStCntr = j["EscStCntr"]
    return v


def toJson_EscStSafe1(v):
    # type: (datatypes.EscStSafe1) -> str
    return json.dumps({
        "EscSt" : _extract_enum_value(_get_if_not_None(v.EscSt)),

        "EscStChks" : _get_if_not_None(v.EscStChks),
        "EscStCntr" : _get_if_not_None(v.EscStCntr),

    })
def fromJson_EscWarnIndcnReqRec1(data):
    # type: (str) -> datatypes.EscWarnIndcnReqRec1
    j = json.loads(data)
    v = datatypes.EscWarnIndcnReqRec1()
    v.EscWarnIndcnReq = j["EscWarnIndcnReq_"]
    v.EscWarnIndcnReqChks = j["EscWarnIndcnReqChks"]
    v.EscWarnIndcnReqCntr = j["EscWarnIndcnReqCntr"]
    return v


def toJson_EscWarnIndcnReqRec1(v):
    # type: (datatypes.EscWarnIndcnReqRec1) -> str
    return json.dumps({
        "EscWarnIndcnReq_" : _extract_enum_value(_get_if_not_None(v.EscWarnIndcnReq)),

        "EscWarnIndcnReqChks" : _get_if_not_None(v.EscWarnIndcnReqChks),
        "EscWarnIndcnReqCntr" : _get_if_not_None(v.EscWarnIndcnReqCntr),

    })
def fromJson_ExtrMirrTiltSetg(data):
    # type: (str) -> datatypes.ExtrMirrTiltSetg
    j = json.loads(data)
    v = datatypes.ExtrMirrTiltSetg()
    v.MirrDrvr = j["MirrDrvr"]
    v.MirrPass = j["MirrPass"]
    v.IdPen = j["IdPen_"]
    return v


def toJson_ExtrMirrTiltSetg(v):
    # type: (datatypes.ExtrMirrTiltSetg) -> str
    return json.dumps({
        "MirrDrvr" : _extract_enum_value(_get_if_not_None(v.MirrDrvr)),

        "MirrPass" : _extract_enum_value(_get_if_not_None(v.MirrPass)),

        "IdPen_" : _extract_enum_value(_get_if_not_None(v.IdPen)),


    })
def fromJson_FrntWiprLvrReq2(data):
    # type: (str) -> datatypes.FrntWiprLvrReq2
    j = json.loads(data)
    v = datatypes.FrntWiprLvrReq2()
    v.FrntWiprLvrCmd1 = j["FrntWiprLvrCmd1_"]
    v.FrntWiprLvrQf = j["FrntWiprLvrQf"]
    v.FrntWiprLvrCrc = j["FrntWiprLvrCrc"]
    v.FrntWiprLvrCntr = j["FrntWiprLvrCntr"]
    return v


def toJson_FrntWiprLvrReq2(v):
    # type: (datatypes.FrntWiprLvrReq2) -> str
    return json.dumps({
        "FrntWiprLvrCmd1_" : _extract_enum_value(_get_if_not_None(v.FrntWiprLvrCmd1)),

        "FrntWiprLvrQf" : _extract_enum_value(_get_if_not_None(v.FrntWiprLvrQf)),

        "FrntWiprLvrCrc" : _get_if_not_None(v.FrntWiprLvrCrc),
        "FrntWiprLvrCntr" : _get_if_not_None(v.FrntWiprLvrCntr),

    })
def fromJson_FuLvlValWithQly(data):
    # type: (str) -> datatypes.FuLvlValWithQly
    j = json.loads(data)
    v = datatypes.FuLvlValWithQly()
    v.FuLvlValFromFuTbl = j["FuLvlValFromFuTbl"]
    v.GenQF = j["GenQF"]
    return v


def toJson_FuLvlValWithQly(v):
    # type: (datatypes.FuLvlValWithQly) -> str
    return json.dumps({
        "FuLvlValFromFuTbl" : _get_if_not_None(v.FuLvlValFromFuTbl),
        "GenQF" : _extract_enum_value(_get_if_not_None(v.GenQF)),


    })
def fromJson_GearIndcnRec2(data):
    # type: (str) -> datatypes.GearIndcnRec2
    j = json.loads(data)
    v = datatypes.GearIndcnRec2()
    v.GearIndcn = j["GearIndcn"]
    v.GearShiftIndcn = j["GearShiftIndcn_"]
    v.GearTarIndcn = j["GearTarIndcn"]
    return v


def toJson_GearIndcnRec2(v):
    # type: (datatypes.GearIndcnRec2) -> str
    return json.dumps({
        "GearIndcn" : _extract_enum_value(_get_if_not_None(v.GearIndcn)),

        "GearShiftIndcn_" : _extract_enum_value(_get_if_not_None(v.GearShiftIndcn)),

        "GearTarIndcn" : _extract_enum_value(_get_if_not_None(v.GearTarIndcn)),


    })
def fromJson_GlbRstForSetgAndData(data):
    # type: (str) -> datatypes.GlbRstForSetgAndData
    j = json.loads(data)
    v = datatypes.GlbRstForSetgAndData()
    v.GlbRst = j["GlbRst_"]
    v.Pen = j["Pen"]
    return v


def toJson_GlbRstForSetgAndData(v):
    # type: (datatypes.GlbRstForSetgAndData) -> str
    return json.dumps({
        "GlbRst_" : _extract_enum_value(_get_if_not_None(v.GlbRst)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_HmiCmptmtTSp(data):
    # type: (str) -> datatypes.HmiCmptmtTSp
    j = json.loads(data)
    v = datatypes.HmiCmptmtTSp()
    v.HmiCmptmtTSpForRowFirstLe = j["HmiCmptmtTSpForRowFirstLe"]
    v.HmiCmptmtTSpSpclForRowFirstLe = j["HmiCmptmtTSpSpclForRowFirstLe"]
    v.HmiCmptmtTSpForRowFirstRi = j["HmiCmptmtTSpForRowFirstRi"]
    v.HmiCmptmtTSpSpclForRowFirstRi = j["HmiCmptmtTSpSpclForRowFirstRi"]
    v.HmiCmptmtTSpForRowSecLe = j["HmiCmptmtTSpForRowSecLe"]
    v.HmiCmptmtTSpSpclForRowSecLe = j["HmiCmptmtTSpSpclForRowSecLe"]
    v.HmiCmptmtTSpForRowSecRi = j["HmiCmptmtTSpForRowSecRi"]
    v.HmiCmptmtTSpSpclForRowSecRi = j["HmiCmptmtTSpSpclForRowSecRi"]
    return v


def toJson_HmiCmptmtTSp(v):
    # type: (datatypes.HmiCmptmtTSp) -> str
    return json.dumps({
        "HmiCmptmtTSpForRowFirstLe" : _get_if_not_None(v.HmiCmptmtTSpForRowFirstLe),
        "HmiCmptmtTSpSpclForRowFirstLe" : _extract_enum_value(_get_if_not_None(v.HmiCmptmtTSpSpclForRowFirstLe)),

        "HmiCmptmtTSpForRowFirstRi" : _get_if_not_None(v.HmiCmptmtTSpForRowFirstRi),
        "HmiCmptmtTSpSpclForRowFirstRi" : _extract_enum_value(_get_if_not_None(v.HmiCmptmtTSpSpclForRowFirstRi)),

        "HmiCmptmtTSpForRowSecLe" : _get_if_not_None(v.HmiCmptmtTSpForRowSecLe),
        "HmiCmptmtTSpSpclForRowSecLe" : _extract_enum_value(_get_if_not_None(v.HmiCmptmtTSpSpclForRowSecLe)),

        "HmiCmptmtTSpForRowSecRi" : _get_if_not_None(v.HmiCmptmtTSpForRowSecRi),
        "HmiCmptmtTSpSpclForRowSecRi" : _extract_enum_value(_get_if_not_None(v.HmiCmptmtTSpSpclForRowSecRi)),


    })
def fromJson_HmiDefrstElecReq(data):
    # type: (str) -> datatypes.HmiDefrstElecReq
    j = json.loads(data)
    v = datatypes.HmiDefrstElecReq()
    v.FrntElecReq = j["FrntElecReq"]
    v.ReElecReq = j["ReElecReq"]
    v.MirrElecReq = j["MirrElecReq"]
    return v


def toJson_HmiDefrstElecReq(v):
    # type: (datatypes.HmiDefrstElecReq) -> str
    return json.dumps({
        "FrntElecReq" : _extract_enum_value(_get_if_not_None(v.FrntElecReq)),

        "ReElecReq" : _extract_enum_value(_get_if_not_None(v.ReElecReq)),

        "MirrElecReq" : _extract_enum_value(_get_if_not_None(v.MirrElecReq)),


    })
def fromJson_HmiDefrstElecSts(data):
    # type: (str) -> datatypes.HmiDefrstElecSts
    j = json.loads(data)
    v = datatypes.HmiDefrstElecSts()
    v.Frnt = j["Frnt"]
    v.Re = j["Re"]
    v.Mirrr = j["Mirrr"]
    return v


def toJson_HmiDefrstElecSts(v):
    # type: (datatypes.HmiDefrstElecSts) -> str
    return json.dumps({
        "Frnt" : _extract_enum_value(_get_if_not_None(v.Frnt)),

        "Re" : _extract_enum_value(_get_if_not_None(v.Re)),

        "Mirrr" : _extract_enum_value(_get_if_not_None(v.Mirrr)),


    })
def fromJson_HmiSeatClima(data):
    # type: (str) -> datatypes.HmiSeatClima
    j = json.loads(data)
    v = datatypes.HmiSeatClima()
    v.HmiSeatHeatgForRowFirstLe = j["HmiSeatHeatgForRowFirstLe"]
    v.HmiSeatHeatgForRowFirstRi = j["HmiSeatHeatgForRowFirstRi"]
    v.HmiSeatHeatgForRowSecLe = j["HmiSeatHeatgForRowSecLe"]
    v.HmiSeatHeatgForRowSecRi = j["HmiSeatHeatgForRowSecRi"]
    v.HmiSeatVentnForRowFirstLe = j["HmiSeatVentnForRowFirstLe"]
    v.HmiSeatVentnForRowFirstRi = j["HmiSeatVentnForRowFirstRi"]
    return v


def toJson_HmiSeatClima(v):
    # type: (datatypes.HmiSeatClima) -> str
    return json.dumps({
        "HmiSeatHeatgForRowFirstLe" : _extract_enum_value(_get_if_not_None(v.HmiSeatHeatgForRowFirstLe)),

        "HmiSeatHeatgForRowFirstRi" : _extract_enum_value(_get_if_not_None(v.HmiSeatHeatgForRowFirstRi)),

        "HmiSeatHeatgForRowSecLe" : _extract_enum_value(_get_if_not_None(v.HmiSeatHeatgForRowSecLe)),

        "HmiSeatHeatgForRowSecRi" : _extract_enum_value(_get_if_not_None(v.HmiSeatHeatgForRowSecRi)),

        "HmiSeatVentnForRowFirstLe" : _extract_enum_value(_get_if_not_None(v.HmiSeatVentnForRowFirstLe)),

        "HmiSeatVentnForRowFirstRi" : _extract_enum_value(_get_if_not_None(v.HmiSeatVentnForRowFirstRi)),


    })
def fromJson_HmiSeatClimaExtd(data):
    # type: (str) -> datatypes.HmiSeatClimaExtd
    j = json.loads(data)
    v = datatypes.HmiSeatClimaExtd()
    v.HmiSeatVentnForRowSecLe = j["HmiSeatVentnForRowSecLe"]
    v.HmiSeatVentnForRowSecRi = j["HmiSeatVentnForRowSecRi"]
    return v


def toJson_HmiSeatClimaExtd(v):
    # type: (datatypes.HmiSeatClimaExtd) -> str
    return json.dumps({
        "HmiSeatVentnForRowSecLe" : _extract_enum_value(_get_if_not_None(v.HmiSeatVentnForRowSecLe)),

        "HmiSeatVentnForRowSecRi" : _extract_enum_value(_get_if_not_None(v.HmiSeatVentnForRowSecRi)),


    })
def fromJson_HudDiagc(data):
    # type: (str) -> datatypes.HudDiagc
    j = json.loads(data)
    v = datatypes.HudDiagc()
    v.HudTSts = j["HudTSts"]
    v.HudCircShoSts = j["HudCircShoSts"]
    v.HudCricOpenSts = j["HudCricOpenSts"]
    v.ImgHudTmpNotAvlSts = j["ImgHudTmpNotAvlSts"]
    v.ImgHudErrSts = j["ImgHudErrSts"]
    return v


def toJson_HudDiagc(v):
    # type: (datatypes.HudDiagc) -> str
    return json.dumps({
        "HudTSts" : _extract_enum_value(_get_if_not_None(v.HudTSts)),

        "HudCircShoSts" : _extract_enum_value(_get_if_not_None(v.HudCircShoSts)),

        "HudCricOpenSts" : _extract_enum_value(_get_if_not_None(v.HudCricOpenSts)),

        "ImgHudTmpNotAvlSts" : _extract_enum_value(_get_if_not_None(v.ImgHudTmpNotAvlSts)),

        "ImgHudErrSts" : _extract_enum_value(_get_if_not_None(v.ImgHudErrSts)),


    })
def fromJson_HudVisFctSetgReq(data):
    # type: (str) -> datatypes.HudVisFctSetgReq
    j = json.loads(data)
    v = datatypes.HudVisFctSetgReq()
    v.Pen = j["Pen"]
    v.HudFct00 = j["HudFct00"]
    v.HudFct01 = j["HudFct01"]
    v.HudFct02 = j["HudFct02"]
    v.HudFct03 = j["HudFct03"]
    v.HudFct04 = j["HudFct04"]
    v.HudFct05 = j["HudFct05"]
    v.HudFct06 = j["HudFct06"]
    v.HudFct07 = j["HudFct07"]
    v.HudFct08 = j["HudFct08"]
    v.HudFct09 = j["HudFct09"]
    v.HudFct10 = j["HudFct10"]
    v.HudFct11 = j["HudFct11"]
    v.HudFct12 = j["HudFct12"]
    v.HudFct13 = j["HudFct13"]
    v.HudFct14 = j["HudFct14"]
    v.HudFct15 = j["HudFct15"]
    v.HudFct16 = j["HudFct16"]
    v.HudFct17 = j["HudFct17"]
    v.HudFct18 = j["HudFct18"]
    v.HudFct19 = j["HudFct19"]
    return v


def toJson_HudVisFctSetgReq(v):
    # type: (datatypes.HudVisFctSetgReq) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "HudFct00" : _extract_enum_value(_get_if_not_None(v.HudFct00)),

        "HudFct01" : _extract_enum_value(_get_if_not_None(v.HudFct01)),

        "HudFct02" : _extract_enum_value(_get_if_not_None(v.HudFct02)),

        "HudFct03" : _extract_enum_value(_get_if_not_None(v.HudFct03)),

        "HudFct04" : _extract_enum_value(_get_if_not_None(v.HudFct04)),

        "HudFct05" : _extract_enum_value(_get_if_not_None(v.HudFct05)),

        "HudFct06" : _extract_enum_value(_get_if_not_None(v.HudFct06)),

        "HudFct07" : _extract_enum_value(_get_if_not_None(v.HudFct07)),

        "HudFct08" : _extract_enum_value(_get_if_not_None(v.HudFct08)),

        "HudFct09" : _extract_enum_value(_get_if_not_None(v.HudFct09)),

        "HudFct10" : _extract_enum_value(_get_if_not_None(v.HudFct10)),

        "HudFct11" : _extract_enum_value(_get_if_not_None(v.HudFct11)),

        "HudFct12" : _extract_enum_value(_get_if_not_None(v.HudFct12)),

        "HudFct13" : _extract_enum_value(_get_if_not_None(v.HudFct13)),

        "HudFct14" : _extract_enum_value(_get_if_not_None(v.HudFct14)),

        "HudFct15" : _extract_enum_value(_get_if_not_None(v.HudFct15)),

        "HudFct16" : _extract_enum_value(_get_if_not_None(v.HudFct16)),

        "HudFct17" : _extract_enum_value(_get_if_not_None(v.HudFct17)),

        "HudFct18" : _extract_enum_value(_get_if_not_None(v.HudFct18)),

        "HudFct19" : _extract_enum_value(_get_if_not_None(v.HudFct19)),


    })
def fromJson_HwAprvdWirelsAdr1(data):
    # type: (str) -> datatypes.HwAprvdWirelsAdr1
    j = json.loads(data)
    v = datatypes.HwAprvdWirelsAdr1()
    v.HwOffsAdr1 = j["HwOffsAdr1"]
    v.HwOffsAdr2 = j["HwOffsAdr2"]
    v.HwOffsAdr3 = j["HwOffsAdr3"]
    v.HwOffsAdr4 = j["HwOffsAdr4"]
    v.HwOffsAdr5 = j["HwOffsAdr5"]
    v.HwOffsAdr6 = j["HwOffsAdr6"]
    v.AprvdSts = j["AprvdSts_"]
    return v


def toJson_HwAprvdWirelsAdr1(v):
    # type: (datatypes.HwAprvdWirelsAdr1) -> str
    return json.dumps({
        "HwOffsAdr1" : _get_if_not_None(v.HwOffsAdr1),
        "HwOffsAdr2" : _get_if_not_None(v.HwOffsAdr2),
        "HwOffsAdr3" : _get_if_not_None(v.HwOffsAdr3),
        "HwOffsAdr4" : _get_if_not_None(v.HwOffsAdr4),
        "HwOffsAdr5" : _get_if_not_None(v.HwOffsAdr5),
        "HwOffsAdr6" : _get_if_not_None(v.HwOffsAdr6),
        "AprvdSts_" : _extract_enum_value(_get_if_not_None(v.AprvdSts)),


    })
def fromJson_HznDataGroup2(data):
    # type: (str) -> datatypes.HznDataGroup2
    j = json.loads(data)
    v = datatypes.HznDataGroup2()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.CtryCod = j["CtryCod"]
    v.RegnCod = j["RegnCod"]
    v.DrvgSide = j["DrvgSide"]
    v.SpdUnit = j["SpdUnit"]
    v.HdTxVers = j["HdTxVers"]
    v.TxVers = j["TxVers"]
    v.ChdTxVers = j["ChdTxVers"]
    v.HwVers = j["HwVers"]
    v.MapSrc = j["MapSrc"]
    v.YrVersOfMap = j["YrVersOfMap"]
    v.PartOfYrVersOfMap = j["PartOfYrVersOfMap"]
    return v


def toJson_HznDataGroup2(v):
    # type: (datatypes.HznDataGroup2) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "CtryCod" : _get_if_not_None(v.CtryCod),
        "RegnCod" : _get_if_not_None(v.RegnCod),
        "DrvgSide" : _extract_enum_value(_get_if_not_None(v.DrvgSide)),

        "SpdUnit" : _extract_enum_value(_get_if_not_None(v.SpdUnit)),

        "HdTxVers" : _get_if_not_None(v.HdTxVers),
        "TxVers" : _get_if_not_None(v.TxVers),
        "ChdTxVers" : _get_if_not_None(v.ChdTxVers),
        "HwVers" : _get_if_not_None(v.HwVers),
        "MapSrc" : _extract_enum_value(_get_if_not_None(v.MapSrc)),

        "YrVersOfMap" : _get_if_not_None(v.YrVersOfMap),
        "PartOfYrVersOfMap" : _get_if_not_None(v.PartOfYrVersOfMap),

    })
def fromJson_HznEdgeGroup2(data):
    # type: (str) -> datatypes.HznEdgeGroup2
    j = json.loads(data)
    v = datatypes.HznEdgeGroup2()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.TxPrev = j["TxPrev"]
    v.PahIdx = j["PahIdx"]
    v.Offs = j["Offs"]
    v.Upd = j["Upd"]
    v.PahIdxNew = j["PahIdxNew"]
    v.TurnAg = j["TurnAg"]
    v.RelProblty = j["RelProblty"]
    v.TypOfWay = j["TypOfWay"]
    v.NrOfLaneInDrvgDir = j["NrOfLaneInDrvgDir"]
    v.NrOfLaneInSecDir = j["NrOfLaneInSecDir"]
    v.IntscnCmplx = j["IntscnCmplx"]
    v.RiOfWay = j["RiOfWay"]
    v.RoadClass = j["RoadClass"]
    v.PartOfCourseCalcd = j["PartOfCourseCalcd"]
    v.LstEdgeAtOffs = j["LstEdgeAtOffs"]
    return v


def toJson_HznEdgeGroup2(v):
    # type: (datatypes.HznEdgeGroup2) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "TxPrev" : _extract_enum_value(_get_if_not_None(v.TxPrev)),

        "PahIdx" : _get_if_not_None(v.PahIdx),
        "Offs" : _get_if_not_None(v.Offs),
        "Upd" : _extract_enum_value(_get_if_not_None(v.Upd)),

        "PahIdxNew" : _get_if_not_None(v.PahIdxNew),
        "TurnAg" : _get_if_not_None(v.TurnAg),
        "RelProblty" : _get_if_not_None(v.RelProblty),
        "TypOfWay" : _extract_enum_value(_get_if_not_None(v.TypOfWay)),

        "NrOfLaneInDrvgDir" : _extract_enum_value(_get_if_not_None(v.NrOfLaneInDrvgDir)),

        "NrOfLaneInSecDir" : _extract_enum_value(_get_if_not_None(v.NrOfLaneInSecDir)),

        "IntscnCmplx" : _extract_enum_value(_get_if_not_None(v.IntscnCmplx)),

        "RiOfWay" : _extract_enum_value(_get_if_not_None(v.RiOfWay)),

        "RoadClass" : _get_if_not_None(v.RoadClass),
        "PartOfCourseCalcd" : _extract_enum_value(_get_if_not_None(v.PartOfCourseCalcd)),

        "LstEdgeAtOffs" : _extract_enum_value(_get_if_not_None(v.LstEdgeAtOffs)),


    })
def fromJson_HznPosnExtdGroup1(data):
    # type: (str) -> datatypes.HznPosnExtdGroup1
    j = json.loads(data)
    v = datatypes.HznPosnExtdGroup1()
    v.MsgTyp = j["MsgTyp"]
    v.PahIdx = j["PahIdx"]
    v.PosnIdx = j["PosnIdx"]
    v.PosnTiDif = j["PosnTiDif"]
    v.Spd = j["Spd"]
    v.RelDir = j["RelDir"]
    v.PosnProblty = j["PosnProblty"]
    v.PosnQly = j["PosnQly"]
    v.LanePrsnt = j["LanePrsnt"]
    return v


def toJson_HznPosnExtdGroup1(v):
    # type: (datatypes.HznPosnExtdGroup1) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "PahIdx" : _get_if_not_None(v.PahIdx),
        "PosnIdx" : _get_if_not_None(v.PosnIdx),
        "PosnTiDif" : _get_if_not_None(v.PosnTiDif),
        "Spd" : _get_if_not_None(v.Spd),
        "RelDir" : _get_if_not_None(v.RelDir),
        "PosnProblty" : _get_if_not_None(v.PosnProblty),
        "PosnQly" : _get_if_not_None(v.PosnQly),
        "LanePrsnt" : _extract_enum_value(_get_if_not_None(v.LanePrsnt)),


    })
def fromJson_HznPosnExtdOffs(data):
    # type: (str) -> datatypes.HznPosnExtdOffs
    j = json.loads(data)
    v = datatypes.HznPosnExtdOffs()
    v.Offs = j["Offs"]
    v.CycCntr = j["CycCntr"]
    return v


def toJson_HznPosnExtdOffs(v):
    # type: (datatypes.HznPosnExtdOffs) -> str
    return json.dumps({
        "Offs" : _get_if_not_None(v.Offs),
        "CycCntr" : _get_if_not_None(v.CycCntr),

    })
def fromJson_HznPosnGroup3(data):
    # type: (str) -> datatypes.HznPosnGroup3
    j = json.loads(data)
    v = datatypes.HznPosnGroup3()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.PahIdx = j["PahIdx"]
    v.Offs = j["Offs"]
    v.PosnIdx = j["PosnIdx"]
    v.PosnTiDif = j["PosnTiDif"]
    v.Spd = j["Spd"]
    v.RelDir = j["RelDir"]
    v.PosnProblty = j["PosnProblty"]
    v.PosnQly = j["PosnQly"]
    v.LanePrsnt = j["LanePrsnt"]
    return v


def toJson_HznPosnGroup3(v):
    # type: (datatypes.HznPosnGroup3) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "PahIdx" : _get_if_not_None(v.PahIdx),
        "Offs" : _get_if_not_None(v.Offs),
        "PosnIdx" : _get_if_not_None(v.PosnIdx),
        "PosnTiDif" : _get_if_not_None(v.PosnTiDif),
        "Spd" : _get_if_not_None(v.Spd),
        "RelDir" : _get_if_not_None(v.RelDir),
        "PosnProblty" : _get_if_not_None(v.PosnProblty),
        "PosnQly" : _get_if_not_None(v.PosnQly),
        "LanePrsnt" : _extract_enum_value(_get_if_not_None(v.LanePrsnt)),


    })
def fromJson_HznProfLongExtdGroup1(data):
    # type: (str) -> datatypes.HznProfLongExtdGroup1
    j = json.loads(data)
    v = datatypes.HznProfLongExtdGroup1()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.TxPrev = j["TxPrev"]
    v.PahIdx = j["PahIdx"]
    v.Offs = j["Offs"]
    v.Upd = j["Upd"]
    v.ProfTyp = j["ProfTyp"]
    v.NodCtrl = j["NodCtrl"]
    v.Val = j["Val"]
    return v


def toJson_HznProfLongExtdGroup1(v):
    # type: (datatypes.HznProfLongExtdGroup1) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "TxPrev" : _extract_enum_value(_get_if_not_None(v.TxPrev)),

        "PahIdx" : _get_if_not_None(v.PahIdx),
        "Offs" : _get_if_not_None(v.Offs),
        "Upd" : _extract_enum_value(_get_if_not_None(v.Upd)),

        "ProfTyp" : _extract_enum_value(_get_if_not_None(v.ProfTyp)),

        "NodCtrl" : _extract_enum_value(_get_if_not_None(v.NodCtrl)),

        "Val" : _get_if_not_None(v.Val),

    })
def fromJson_HznProfLongGroup3(data):
    # type: (str) -> datatypes.HznProfLongGroup3
    j = json.loads(data)
    v = datatypes.HznProfLongGroup3()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.TxPrev = j["TxPrev"]
    v.PahIdx = j["PahIdx"]
    v.Offs = j["Offs"]
    v.Upd = j["Upd"]
    v.ProfTyp = j["ProfTyp"]
    v.NodCtrl = j["NodCtrl"]
    v.Val = j["Val"]
    return v


def toJson_HznProfLongGroup3(v):
    # type: (datatypes.HznProfLongGroup3) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "TxPrev" : _extract_enum_value(_get_if_not_None(v.TxPrev)),

        "PahIdx" : _get_if_not_None(v.PahIdx),
        "Offs" : _get_if_not_None(v.Offs),
        "Upd" : _extract_enum_value(_get_if_not_None(v.Upd)),

        "ProfTyp" : _extract_enum_value(_get_if_not_None(v.ProfTyp)),

        "NodCtrl" : _extract_enum_value(_get_if_not_None(v.NodCtrl)),

        "Val" : _get_if_not_None(v.Val),

    })
def fromJson_HznProfSho2(data):
    # type: (str) -> datatypes.HznProfSho2
    j = json.loads(data)
    v = datatypes.HznProfSho2()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.TxPrev = j["TxPrev"]
    v.PahIdx = j["PahIdx"]
    v.Offs = j["Offs"]
    v.Upd = j["Upd"]
    v.ProfTyp = j["ProfTyp"]
    v.NodCtrl = j["NodCtrl"]
    v.Val0 = j["Val0"]
    v.Dst1 = j["Dst1"]
    v.Val1 = j["Val1"]
    v.ProfTypQly = j["ProfTypQly"]
    return v


def toJson_HznProfSho2(v):
    # type: (datatypes.HznProfSho2) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "TxPrev" : _extract_enum_value(_get_if_not_None(v.TxPrev)),

        "PahIdx" : _get_if_not_None(v.PahIdx),
        "Offs" : _get_if_not_None(v.Offs),
        "Upd" : _extract_enum_value(_get_if_not_None(v.Upd)),

        "ProfTyp" : _extract_enum_value(_get_if_not_None(v.ProfTyp)),

        "NodCtrl" : _extract_enum_value(_get_if_not_None(v.NodCtrl)),

        "Val0" : _get_if_not_None(v.Val0),
        "Dst1" : _get_if_not_None(v.Dst1),
        "Val1" : _get_if_not_None(v.Val1),
        "ProfTypQly" : _extract_enum_value(_get_if_not_None(v.ProfTypQly)),


    })
def fromJson_HznSegGroup2(data):
    # type: (str) -> datatypes.HznSegGroup2
    j = json.loads(data)
    v = datatypes.HznSegGroup2()
    v.MsgTyp = j["MsgTyp"]
    v.CycCntr = j["CycCntr"]
    v.TxPrev = j["TxPrev"]
    v.PahIdx = j["PahIdx"]
    v.Offs = j["Offs"]
    v.Upd = j["Upd"]
    v.RoadClass = j["RoadClass"]
    v.TypOfWay = j["TypOfWay"]
    v.SpdLimEfc = j["SpdLimEfc"]
    v.SpdLimTypEfc = j["SpdLimTypEfc"]
    v.NrOfLaneInDrvgDir = j["NrOfLaneInDrvgDir"]
    v.NrOfLaneInSecDir = j["NrOfLaneInSecDir"]
    v.Tnl = j["Tnl"]
    v.Brdg = j["Brdg"]
    v.RoadMpl = j["RoadMpl"]
    v.RoadLiAr = j["RoadLiAr"]
    v.IntscnCmplx = j["IntscnCmplx"]
    v.RelProblty = j["RelProblty"]
    v.PartOfCourseCalcd = j["PartOfCourseCalcd"]
    return v


def toJson_HznSegGroup2(v):
    # type: (datatypes.HznSegGroup2) -> str
    return json.dumps({
        "MsgTyp" : _extract_enum_value(_get_if_not_None(v.MsgTyp)),

        "CycCntr" : _get_if_not_None(v.CycCntr),
        "TxPrev" : _extract_enum_value(_get_if_not_None(v.TxPrev)),

        "PahIdx" : _get_if_not_None(v.PahIdx),
        "Offs" : _get_if_not_None(v.Offs),
        "Upd" : _extract_enum_value(_get_if_not_None(v.Upd)),

        "RoadClass" : _get_if_not_None(v.RoadClass),
        "TypOfWay" : _extract_enum_value(_get_if_not_None(v.TypOfWay)),

        "SpdLimEfc" : _extract_enum_value(_get_if_not_None(v.SpdLimEfc)),

        "SpdLimTypEfc" : _extract_enum_value(_get_if_not_None(v.SpdLimTypEfc)),

        "NrOfLaneInDrvgDir" : _extract_enum_value(_get_if_not_None(v.NrOfLaneInDrvgDir)),

        "NrOfLaneInSecDir" : _extract_enum_value(_get_if_not_None(v.NrOfLaneInSecDir)),

        "Tnl" : _extract_enum_value(_get_if_not_None(v.Tnl)),

        "Brdg" : _extract_enum_value(_get_if_not_None(v.Brdg)),

        "RoadMpl" : _extract_enum_value(_get_if_not_None(v.RoadMpl)),

        "RoadLiAr" : _extract_enum_value(_get_if_not_None(v.RoadLiAr)),

        "IntscnCmplx" : _extract_enum_value(_get_if_not_None(v.IntscnCmplx)),

        "RelProblty" : _get_if_not_None(v.RelProblty),
        "PartOfCourseCalcd" : _extract_enum_value(_get_if_not_None(v.PartOfCourseCalcd)),


    })
def fromJson_IndcnUnit(data):
    # type: (str) -> datatypes.IndcnUnit
    j = json.loads(data)
    v = datatypes.IndcnUnit()
    v.TUnit = j["TUnit"]
    v.SpdUnit = j["SpdUnit"]
    v.DstLong = j["DstLong_"]
    v.DstSho = j["DstSho_"]
    v.FuCnsUnit = j["FuCnsUnit"]
    v.VolUnit = j["VolUnit_"]
    v.IdPenForUnit = j["IdPenForUnit"]
    v.ClkFmt = j["ClkFmt_"]
    v.PUnit = j["PUnit_"]
    v.DateFmt = j["DateFmt_"]
    return v


def toJson_IndcnUnit(v):
    # type: (datatypes.IndcnUnit) -> str
    return json.dumps({
        "TUnit" : _extract_enum_value(_get_if_not_None(v.TUnit)),

        "SpdUnit" : _extract_enum_value(_get_if_not_None(v.SpdUnit)),

        "DstLong_" : _extract_enum_value(_get_if_not_None(v.DstLong)),

        "DstSho_" : _extract_enum_value(_get_if_not_None(v.DstSho)),

        "FuCnsUnit" : _extract_enum_value(_get_if_not_None(v.FuCnsUnit)),

        "VolUnit_" : _extract_enum_value(_get_if_not_None(v.VolUnit)),

        "IdPenForUnit" : _extract_enum_value(_get_if_not_None(v.IdPenForUnit)),

        "ClkFmt_" : _extract_enum_value(_get_if_not_None(v.ClkFmt)),

        "PUnit_" : _extract_enum_value(_get_if_not_None(v.PUnit)),

        "DateFmt_" : _extract_enum_value(_get_if_not_None(v.DateFmt)),


    })
def fromJson_IntrLiAmbLiSetg(data):
    # type: (str) -> datatypes.IntrLiAmbLiSetg
    j = json.loads(data)
    v = datatypes.IntrLiAmbLiSetg()
    v.LiInten = j["LiInten"]
    v.LiTintg = j["LiTintg"]
    v.Pen = j["Pen"]
    return v


def toJson_IntrLiAmbLiSetg(v):
    # type: (datatypes.IntrLiAmbLiSetg) -> str
    return json.dumps({
        "LiInten" : _extract_enum_value(_get_if_not_None(v.LiInten)),

        "LiTintg" : _extract_enum_value(_get_if_not_None(v.LiTintg)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_IntrLiSurrndgsLiSetg(data):
    # type: (str) -> datatypes.IntrLiSurrndgsLiSetg
    j = json.loads(data)
    v = datatypes.IntrLiSurrndgsLiSetg()
    v.LiInten = j["LiInten"]
    v.LiLvl = j["LiLvl"]
    v.Pen = j["Pen"]
    return v


def toJson_IntrLiSurrndgsLiSetg(v):
    # type: (datatypes.IntrLiSurrndgsLiSetg) -> str
    return json.dumps({
        "LiInten" : _extract_enum_value(_get_if_not_None(v.LiInten)),

        "LiLvl" : _extract_enum_value(_get_if_not_None(v.LiLvl)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_KeyProfMpgUpd1(data):
    # type: (str) -> datatypes.KeyProfMpgUpd1
    j = json.loads(data)
    v = datatypes.KeyProfMpgUpd1()
    v.KeyProfMpgUpdForIdPen = j["KeyProfMpgUpdForIdPen"]
    v.KeyProfMpgUpdOff = j["KeyProfMpgUpdOff"]
    return v


def toJson_KeyProfMpgUpd1(v):
    # type: (datatypes.KeyProfMpgUpd1) -> str
    return json.dumps({
        "KeyProfMpgUpdForIdPen" : _extract_enum_value(_get_if_not_None(v.KeyProfMpgUpdForIdPen)),

        "KeyProfMpgUpdOff" : _get_if_not_None(v.KeyProfMpgUpdOff),

    })
def fromJson_KeyReadStsToProfCtrl(data):
    # type: (str) -> datatypes.KeyReadStsToProfCtrl
    j = json.loads(data)
    v = datatypes.KeyReadStsToProfCtrl()
    v.KeyId = j["KeyId"]
    v.Boolean = j["Boolean"]
    return v


def toJson_KeyReadStsToProfCtrl(v):
    # type: (datatypes.KeyReadStsToProfCtrl) -> str
    return json.dumps({
        "KeyId" : _extract_enum_value(_get_if_not_None(v.KeyId)),

        "Boolean" : _get_if_not_None(v.Boolean),

    })
def fromJson_LiAutTranPen1(data):
    # type: (str) -> datatypes.LiAutTranPen1
    j = json.loads(data)
    v = datatypes.LiAutTranPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_LiAutTranPen1(v):
    # type: (datatypes.LiAutTranPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_LiTiPen2(data):
    # type: (str) -> datatypes.LiTiPen2
    j = json.loads(data)
    v = datatypes.LiTiPen2()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_LiTiPen2(v):
    # type: (datatypes.LiTiPen2) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_ListOfNodAv(data):
    # type: (str) -> datatypes.ListOfNodAv
    j = json.loads(data)
    v = datatypes.ListOfNodAv()
    v.ListOfNodAv1 = j["ListOfNodAv1"]
    v.ListOfNodAv2 = j["ListOfNodAv2"]
    v.ListOfNodAv3 = j["ListOfNodAv3"]
    v.ListOfNodAv4 = j["ListOfNodAv4"]
    v.ListOfNodAv5 = j["ListOfNodAv5"]
    v.ListOfNodAv6 = j["ListOfNodAv6"]
    v.ListOfNodAv7 = j["ListOfNodAv7"]
    v.ListOfNodAv8 = j["ListOfNodAv8"]
    return v


def toJson_ListOfNodAv(v):
    # type: (datatypes.ListOfNodAv) -> str
    return json.dumps({
        "ListOfNodAv1" : _get_if_not_None(v.ListOfNodAv1),
        "ListOfNodAv2" : _get_if_not_None(v.ListOfNodAv2),
        "ListOfNodAv3" : _get_if_not_None(v.ListOfNodAv3),
        "ListOfNodAv4" : _get_if_not_None(v.ListOfNodAv4),
        "ListOfNodAv5" : _get_if_not_None(v.ListOfNodAv5),
        "ListOfNodAv6" : _get_if_not_None(v.ListOfNodAv6),
        "ListOfNodAv7" : _get_if_not_None(v.ListOfNodAv7),
        "ListOfNodAv8" : _get_if_not_None(v.ListOfNodAv8),

    })
def fromJson_LockgCenSts3(data):
    # type: (str) -> datatypes.LockgCenSts3
    j = json.loads(data)
    v = datatypes.LockgCenSts3()
    v.LockSt = j["LockSt"]
    v.TrigSrc = j["TrigSrc"]
    v.UpdEve = j["UpdEve"]
    return v


def toJson_LockgCenSts3(v):
    # type: (datatypes.LockgCenSts3) -> str
    return json.dumps({
        "LockSt" : _extract_enum_value(_get_if_not_None(v.LockSt)),

        "TrigSrc" : _extract_enum_value(_get_if_not_None(v.TrigSrc)),

        "UpdEve" : _get_if_not_None(v.UpdEve),

    })
def fromJson_LvlSeldForSpdLimAdpvPen1(data):
    # type: (str) -> datatypes.LvlSeldForSpdLimAdpvPen1
    j = json.loads(data)
    v = datatypes.LvlSeldForSpdLimAdpvPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_LvlSeldForSpdLimAdpvPen1(v):
    # type: (datatypes.LvlSeldForSpdLimAdpvPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_MassgFctActv(data):
    # type: (str) -> datatypes.MassgFctActv
    j = json.loads(data)
    v = datatypes.MassgFctActv()
    v.DrvrMassgFctActv = j["DrvrMassgFctActv"]
    v.PassMassgFctActv = j["PassMassgFctActv"]
    return v


def toJson_MassgFctActv(v):
    # type: (datatypes.MassgFctActv) -> str
    return json.dumps({
        "DrvrMassgFctActv" : _extract_enum_value(_get_if_not_None(v.DrvrMassgFctActv)),

        "PassMassgFctActv" : _extract_enum_value(_get_if_not_None(v.PassMassgFctActv)),


    })
def fromJson_MirrDimPen(data):
    # type: (str) -> datatypes.MirrDimPen
    j = json.loads(data)
    v = datatypes.MirrDimPen()
    v.MirrDimLvl = j["MirrDimLvl"]
    v.Pen = j["Pen"]
    return v


def toJson_MirrDimPen(v):
    # type: (datatypes.MirrDimPen) -> str
    return json.dumps({
        "MirrDimLvl" : _extract_enum_value(_get_if_not_None(v.MirrDimLvl)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_MtrlSnsrT(data):
    # type: (str) -> datatypes.MtrlSnsrT
    j = json.loads(data)
    v = datatypes.MtrlSnsrT()
    v.MtrlSnsrT = j["MtrlSnsrT"]
    v.MtrlSnsrTFacQly = j["MtrlSnsrTFacQly_"]
    return v


def toJson_MtrlSnsrT(v):
    # type: (datatypes.MtrlSnsrT) -> str
    return json.dumps({
        "MtrlSnsrT" : _get_if_not_None(v.MtrlSnsrT),
        "MtrlSnsrTFacQly_" : _extract_enum_value(_get_if_not_None(v.MtrlSnsrTFacQly)),


    })
def fromJson_NetActvtRec1(data):
    # type: (str) -> datatypes.NetActvtRec1
    j = json.loads(data)
    v = datatypes.NetActvtRec1()
    v.ResourceGroup = j["ResourceGroup"]
    v.Prio = j["Prio"]
    return v


def toJson_NetActvtRec1(v):
    # type: (datatypes.NetActvtRec1) -> str
    return json.dumps({
        "ResourceGroup" : _get_if_not_None(v.ResourceGroup),
        "Prio" : _extract_enum_value(_get_if_not_None(v.Prio)),


    })
def fromJson_NormSptPen(data):
    # type: (str) -> datatypes.NormSptPen
    j = json.loads(data)
    v = datatypes.NormSptPen()
    v.NormSpt = j["NormSpt"]
    v.Pen = j["Pen"]
    return v


def toJson_NormSptPen(v):
    # type: (datatypes.NormSptPen) -> str
    return json.dumps({
        "NormSpt" : _extract_enum_value(_get_if_not_None(v.NormSpt)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_NrSerlNodLIN(data):
    # type: (str) -> datatypes.NrSerlNodLIN
    j = json.loads(data)
    v = datatypes.NrSerlNodLIN()
    v.Nr1 = j["Nr1"]
    v.Nr2 = j["Nr2"]
    v.Nr3 = j["Nr3"]
    v.Nr4 = j["Nr4"]
    return v


def toJson_NrSerlNodLIN(v):
    # type: (datatypes.NrSerlNodLIN) -> str
    return json.dumps({
        "Nr1" : _get_if_not_None(v.Nr1),
        "Nr2" : _get_if_not_None(v.Nr2),
        "Nr3" : _get_if_not_None(v.Nr3),
        "Nr4" : _get_if_not_None(v.Nr4),

    })
def fromJson_OffsForSpdWarnSetgPen(data):
    # type: (str) -> datatypes.OffsForSpdWarnSetgPen
    j = json.loads(data)
    v = datatypes.OffsForSpdWarnSetgPen()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_OffsForSpdWarnSetgPen(v):
    # type: (datatypes.OffsForSpdWarnSetgPen) -> str
    return json.dumps({
        "Sts" : _get_if_not_None(v.Sts),
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_OnOffPen(data):
    # type: (str) -> datatypes.OnOffPen
    j = json.loads(data)
    v = datatypes.OnOffPen()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_OnOffPen(v):
    # type: (datatypes.OnOffPen) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_PartNrNodLIN(data):
    # type: (str) -> datatypes.PartNrNodLIN
    j = json.loads(data)
    v = datatypes.PartNrNodLIN()
    v.Nr1 = j["Nr1"]
    v.Nr2 = j["Nr2"]
    v.Nr3 = j["Nr3"]
    v.Nr4 = j["Nr4"]
    v.EndSgn1 = j["EndSgn1"]
    v.EndSgn2 = j["EndSgn2"]
    v.EndSgn3 = j["EndSgn3"]
    return v


def toJson_PartNrNodLIN(v):
    # type: (datatypes.PartNrNodLIN) -> str
    return json.dumps({
        "Nr1" : _get_if_not_None(v.Nr1),
        "Nr2" : _get_if_not_None(v.Nr2),
        "Nr3" : _get_if_not_None(v.Nr3),
        "Nr4" : _get_if_not_None(v.Nr4),
        "EndSgn1" : _get_if_not_None(v.EndSgn1),
        "EndSgn2" : _get_if_not_None(v.EndSgn2),
        "EndSgn3" : _get_if_not_None(v.EndSgn3),

    })
def fromJson_PassSeatSwtSts2(data):
    # type: (str) -> datatypes.PassSeatSwtSts2
    j = json.loads(data)
    v = datatypes.PassSeatSwtSts2()
    v.PassSeatSwtSldSts = j["PassSeatSwtSldSts"]
    v.PassSeatSwtHeiSts = j["PassSeatSwtHeiSts"]
    v.PassSeatSwtHeiFrntSts = j["PassSeatSwtHeiFrntSts"]
    v.PassSeatSwtInclSts = j["PassSeatSwtInclSts"]
    v.PassSeatSwtAdjmtOfSpplFctHozlSts = j["PassSeatSwtAdjmtOfSpplFctHozlSts"]
    v.PassSeatSwtAdjmtOfSpplFctVerSts = j["PassSeatSwtAdjmtOfSpplFctVerSts"]
    v.PassSeatSwtSelnOfSpplFctStsSts = j["PassSeatSwtSelnOfSpplFctStsSts"]
    return v


def toJson_PassSeatSwtSts2(v):
    # type: (datatypes.PassSeatSwtSts2) -> str
    return json.dumps({
        "PassSeatSwtSldSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtSldSts)),

        "PassSeatSwtHeiSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtHeiSts)),

        "PassSeatSwtHeiFrntSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtHeiFrntSts)),

        "PassSeatSwtInclSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtInclSts)),

        "PassSeatSwtAdjmtOfSpplFctHozlSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtAdjmtOfSpplFctHozlSts)),

        "PassSeatSwtAdjmtOfSpplFctVerSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtAdjmtOfSpplFctVerSts)),

        "PassSeatSwtSelnOfSpplFctStsSts" : _extract_enum_value(_get_if_not_None(v.PassSeatSwtSelnOfSpplFctStsSts)),


    })
def fromJson_PinionSteerAg1Rec(data):
    # type: (str) -> datatypes.PinionSteerAg1Rec
    j = json.loads(data)
    v = datatypes.PinionSteerAg1Rec()
    v.PinionSteerAg1 = j["PinionSteerAg1"]
    v.PinionSteerAg1Qf = j["PinionSteerAg1Qf"]
    return v


def toJson_PinionSteerAg1Rec(v):
    # type: (datatypes.PinionSteerAg1Rec) -> str
    return json.dumps({
        "PinionSteerAg1" : _get_if_not_None(v.PinionSteerAg1),
        "PinionSteerAg1Qf" : _extract_enum_value(_get_if_not_None(v.PinionSteerAg1Qf)),


    })
def fromJson_PosnFromNav(data):
    # type: (str) -> datatypes.PosnFromNav
    j = json.loads(data)
    v = datatypes.PosnFromNav()
    v.PosnLat = j["PosnLat"]
    v.PosnLgt = j["PosnLgt"]
    v.PosnAlti = j["PosnAlti"]
    v.PosnSpd = j["PosnSpd"]
    v.PosnDir = j["PosnDir"]
    v.PosnQly = j["PosnQly"]
    return v


def toJson_PosnFromNav(v):
    # type: (datatypes.PosnFromNav) -> str
    return json.dumps({
        "PosnLat" : _get_if_not_None(v.PosnLat),
        "PosnLgt" : _get_if_not_None(v.PosnLgt),
        "PosnAlti" : _get_if_not_None(v.PosnAlti),
        "PosnSpd" : _get_if_not_None(v.PosnSpd),
        "PosnDir" : _get_if_not_None(v.PosnDir),
        "PosnQly" : _extract_enum_value(_get_if_not_None(v.PosnQly)),


    })
def fromJson_PosnFromSatlt(data):
    # type: (str) -> datatypes.PosnFromSatlt
    j = json.loads(data)
    v = datatypes.PosnFromSatlt()
    v.PosnLat = j["PosnLat"]
    v.PosnLgt = j["PosnLgt"]
    v.PosnAlti = j["PosnAlti"]
    v.PosnSpd = j["PosnSpd"]
    v.PosnVHozl = j["PosnVHozl"]
    v.PosnVVert = j["PosnVVert"]
    v.PosnDir = j["PosnDir"]
    v.TiForYr = j["TiForYr"]
    v.TiForMth = j["TiForMth"]
    v.TiForDay = j["TiForDay"]
    v.TiForHr = j["TiForHr"]
    v.TiForMins = j["TiForMins"]
    v.TiForSec = j["TiForSec"]
    v.SatltSysNo1InUse = j["SatltSysNo1InUse"]
    v.SatltSysNo2InUse = j["SatltSysNo2InUse"]
    v.SatltSysNo3InUse = j["SatltSysNo3InUse"]
    v.SatltSysNo4InUse = j["SatltSysNo4InUse"]
    v.SatltSysNo5InUse = j["SatltSysNo5InUse"]
    v.SatltSysNo6InUse = j["SatltSysNo6InUse"]
    v.SatltPosnStsPrm1 = j["SatltPosnStsPrm1"]
    v.SatltPosnStsPrm2 = j["SatltPosnStsPrm2"]
    v.SatltPosnStsPrm3 = j["SatltPosnStsPrm3"]
    v.NoOfSatltForSysNo1 = j["NoOfSatltForSysNo1"]
    v.NoOfSatltForSysNo2 = j["NoOfSatltForSysNo2"]
    v.NoOfSatltForSysNo3 = j["NoOfSatltForSysNo3"]
    v.NoOfSatltForSysNo4 = j["NoOfSatltForSysNo4"]
    v.NoOfSatltForSysNo5 = j["NoOfSatltForSysNo5"]
    v.NoOfSatltForSysNo6 = j["NoOfSatltForSysNo6"]
    v.PrePosnDil = j["PrePosnDil"]
    v.PreHozlDil = j["PreHozlDil"]
    v.PreVertDil = j["PreVertDil"]
    v.PreTiDil = j["PreTiDil"]
    return v


def toJson_PosnFromSatlt(v):
    # type: (datatypes.PosnFromSatlt) -> str
    return json.dumps({
        "PosnLat" : _get_if_not_None(v.PosnLat),
        "PosnLgt" : _get_if_not_None(v.PosnLgt),
        "PosnAlti" : _get_if_not_None(v.PosnAlti),
        "PosnSpd" : _get_if_not_None(v.PosnSpd),
        "PosnVHozl" : _get_if_not_None(v.PosnVHozl),
        "PosnVVert" : _get_if_not_None(v.PosnVVert),
        "PosnDir" : _get_if_not_None(v.PosnDir),
        "TiForYr" : _get_if_not_None(v.TiForYr),
        "TiForMth" : _get_if_not_None(v.TiForMth),
        "TiForDay" : _get_if_not_None(v.TiForDay),
        "TiForHr" : _get_if_not_None(v.TiForHr),
        "TiForMins" : _get_if_not_None(v.TiForMins),
        "TiForSec" : _get_if_not_None(v.TiForSec),
        "SatltSysNo1InUse" : _extract_enum_value(_get_if_not_None(v.SatltSysNo1InUse)),

        "SatltSysNo2InUse" : _extract_enum_value(_get_if_not_None(v.SatltSysNo2InUse)),

        "SatltSysNo3InUse" : _extract_enum_value(_get_if_not_None(v.SatltSysNo3InUse)),

        "SatltSysNo4InUse" : _extract_enum_value(_get_if_not_None(v.SatltSysNo4InUse)),

        "SatltSysNo5InUse" : _extract_enum_value(_get_if_not_None(v.SatltSysNo5InUse)),

        "SatltSysNo6InUse" : _extract_enum_value(_get_if_not_None(v.SatltSysNo6InUse)),

        "SatltPosnStsPrm1" : _extract_enum_value(_get_if_not_None(v.SatltPosnStsPrm1)),

        "SatltPosnStsPrm2" : _get_if_not_None(v.SatltPosnStsPrm2),
        "SatltPosnStsPrm3" : _get_if_not_None(v.SatltPosnStsPrm3),
        "NoOfSatltForSysNo1" : _get_if_not_None(v.NoOfSatltForSysNo1),
        "NoOfSatltForSysNo2" : _get_if_not_None(v.NoOfSatltForSysNo2),
        "NoOfSatltForSysNo3" : _get_if_not_None(v.NoOfSatltForSysNo3),
        "NoOfSatltForSysNo4" : _get_if_not_None(v.NoOfSatltForSysNo4),
        "NoOfSatltForSysNo5" : _get_if_not_None(v.NoOfSatltForSysNo5),
        "NoOfSatltForSysNo6" : _get_if_not_None(v.NoOfSatltForSysNo6),
        "PrePosnDil" : _get_if_not_None(v.PrePosnDil),
        "PreHozlDil" : _get_if_not_None(v.PreHozlDil),
        "PreVertDil" : _get_if_not_None(v.PreVertDil),
        "PreTiDil" : _get_if_not_None(v.PreTiDil),

    })
def fromJson_PrkgAssiAudWarn(data):
    # type: (str) -> datatypes.PrkgAssiAudWarn
    j = json.loads(data)
    v = datatypes.PrkgAssiAudWarn()
    v.AudWarnDir = j["AudWarnDir"]
    v.AudSideWarn = j["AudSideWarn"]
    return v


def toJson_PrkgAssiAudWarn(v):
    # type: (datatypes.PrkgAssiAudWarn) -> str
    return json.dumps({
        "AudWarnDir" : _extract_enum_value(_get_if_not_None(v.AudWarnDir)),

        "AudSideWarn" : _extract_enum_value(_get_if_not_None(v.AudSideWarn)),


    })
def fromJson_ProfAct1(data):
    # type: (str) -> datatypes.ProfAct1
    j = json.loads(data)
    v = datatypes.ProfAct1()
    v.Prof1 = j["Prof1"]
    v.Prof2 = j["Prof2"]
    v.Prof3 = j["Prof3"]
    v.Prof4 = j["Prof4"]
    v.Prof5 = j["Prof5"]
    v.Prof6 = j["Prof6"]
    v.Prof7 = j["Prof7"]
    v.Prof8 = j["Prof8"]
    v.Prof9 = j["Prof9"]
    v.Prof10 = j["Prof10"]
    v.Prof11 = j["Prof11"]
    v.Prof12 = j["Prof12"]
    return v


def toJson_ProfAct1(v):
    # type: (datatypes.ProfAct1) -> str
    return json.dumps({
        "Prof1" : _get_if_not_None(v.Prof1),
        "Prof2" : _get_if_not_None(v.Prof2),
        "Prof3" : _get_if_not_None(v.Prof3),
        "Prof4" : _get_if_not_None(v.Prof4),
        "Prof5" : _get_if_not_None(v.Prof5),
        "Prof6" : _get_if_not_None(v.Prof6),
        "Prof7" : _get_if_not_None(v.Prof7),
        "Prof8" : _get_if_not_None(v.Prof8),
        "Prof9" : _get_if_not_None(v.Prof9),
        "Prof10" : _get_if_not_None(v.Prof10),
        "Prof11" : _get_if_not_None(v.Prof11),
        "Prof12" : _get_if_not_None(v.Prof12),

    })
def fromJson_ProfLimd1(data):
    # type: (str) -> datatypes.ProfLimd1
    j = json.loads(data)
    v = datatypes.ProfLimd1()
    v.Prof1 = j["Prof1"]
    v.Prof2 = j["Prof2"]
    v.Prof3 = j["Prof3"]
    v.Prof4 = j["Prof4"]
    v.Prof5 = j["Prof5"]
    v.Prof6 = j["Prof6"]
    v.Prof7 = j["Prof7"]
    v.Prof8 = j["Prof8"]
    v.Prof9 = j["Prof9"]
    v.Prof10 = j["Prof10"]
    v.Prof11 = j["Prof11"]
    v.Prof12 = j["Prof12"]
    return v


def toJson_ProfLimd1(v):
    # type: (datatypes.ProfLimd1) -> str
    return json.dumps({
        "Prof1" : _get_if_not_None(v.Prof1),
        "Prof2" : _get_if_not_None(v.Prof2),
        "Prof3" : _get_if_not_None(v.Prof3),
        "Prof4" : _get_if_not_None(v.Prof4),
        "Prof5" : _get_if_not_None(v.Prof5),
        "Prof6" : _get_if_not_None(v.Prof6),
        "Prof7" : _get_if_not_None(v.Prof7),
        "Prof8" : _get_if_not_None(v.Prof8),
        "Prof9" : _get_if_not_None(v.Prof9),
        "Prof10" : _get_if_not_None(v.Prof10),
        "Prof11" : _get_if_not_None(v.Prof11),
        "Prof12" : _get_if_not_None(v.Prof12),

    })
def fromJson_PtCluTq1(data):
    # type: (str) -> datatypes.PtCluTq1
    j = json.loads(data)
    v = datatypes.PtCluTq1()
    v.PtCluTq = j["PtCluTq"]
    v.PtCluTqDyn = j["PtCluTqDyn"]
    v.PtCluTqQly = j["PtCluTqQly"]
    return v


def toJson_PtCluTq1(v):
    # type: (datatypes.PtCluTq1) -> str
    return json.dumps({
        "PtCluTq" : _get_if_not_None(v.PtCluTq),
        "PtCluTqDyn" : _get_if_not_None(v.PtCluTqDyn),
        "PtCluTqQly" : _extract_enum_value(_get_if_not_None(v.PtCluTqQly)),


    })
def fromJson_PtDrvrSetg2(data):
    # type: (str) -> datatypes.PtDrvrSetg2
    j = json.loads(data)
    v = datatypes.PtDrvrSetg2()
    v.Pen = j["Pen"]
    v.Sts = j["Sts"]
    return v


def toJson_PtDrvrSetg2(v):
    # type: (datatypes.PtDrvrSetg2) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),


    })
def fromJson_PtTqAtWhlFrntActRec1(data):
    # type: (str) -> datatypes.PtTqAtWhlFrntActRec1
    j = json.loads(data)
    v = datatypes.PtTqAtWhlFrntActRec1()
    v.PtTqAtWhlFrntLeAct = j["PtTqAtWhlFrntLeAct"]
    v.PtTqAtWhlFrntRiAct = j["PtTqAtWhlFrntRiAct"]
    v.PtTqAtAxleFrntAct = j["PtTqAtAxleFrntAct"]
    v.PtTqAtWhlsFrntQly = j["PtTqAtWhlsFrntQly"]
    return v


def toJson_PtTqAtWhlFrntActRec1(v):
    # type: (datatypes.PtTqAtWhlFrntActRec1) -> str
    return json.dumps({
        "PtTqAtWhlFrntLeAct" : _get_if_not_None(v.PtTqAtWhlFrntLeAct),
        "PtTqAtWhlFrntRiAct" : _get_if_not_None(v.PtTqAtWhlFrntRiAct),
        "PtTqAtAxleFrntAct" : _get_if_not_None(v.PtTqAtAxleFrntAct),
        "PtTqAtWhlsFrntQly" : _extract_enum_value(_get_if_not_None(v.PtTqAtWhlsFrntQly)),


    })
def fromJson_RngBdIllmnCmdPen1(data):
    # type: (str) -> datatypes.RngBdIllmnCmdPen1
    j = json.loads(data)
    v = datatypes.RngBdIllmnCmdPen1()
    v.Cmd = j["Cmd"]
    v.Pen = j["Pen"]
    return v


def toJson_RngBdIllmnCmdPen1(v):
    # type: (datatypes.RngBdIllmnCmdPen1) -> str
    return json.dumps({
        "Cmd" : _extract_enum_value(_get_if_not_None(v.Cmd)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_RouteInfoRec1(data):
    # type: (str) -> datatypes.RouteInfoRec1
    j = json.loads(data)
    v = datatypes.RouteInfoRec1()
    v.DestInfo = j["DestInfo"]
    v.IdOfDest = j["IdOfDest"]
    v.DestProblty = j["DestProblty"]
    v.NrOfDestCmpl = j["NrOfDestCmpl"]
    v.DestSrc = j["DestSrc"]
    return v


def toJson_RouteInfoRec1(v):
    # type: (datatypes.RouteInfoRec1) -> str
    return json.dumps({
        "DestInfo" : _get_if_not_None(v.DestInfo),
        "IdOfDest" : _get_if_not_None(v.IdOfDest),
        "DestProblty" : _get_if_not_None(v.DestProblty),
        "NrOfDestCmpl" : _get_if_not_None(v.NrOfDestCmpl),
        "DestSrc" : _get_if_not_None(v.DestSrc),

    })
def fromJson_RsdsSysSts(data):
    # type: (str) -> datatypes.RsdsSysSts
    j = json.loads(data)
    v = datatypes.RsdsSysSts()
    v.RsdsSts = j["RsdsSts"]
    v.LcmaSts = j["LcmaSts"]
    v.LcmaSts1 = j["LcmaSts1"]
    v.CtraSts = j["CtraSts"]
    v.CtraSts1 = j["CtraSts1"]
    v.RcwSts = j["RcwSts"]
    v.RcmSts = j["RcmSts"]
    return v


def toJson_RsdsSysSts(v):
    # type: (datatypes.RsdsSysSts) -> str
    return json.dumps({
        "RsdsSts" : _extract_enum_value(_get_if_not_None(v.RsdsSts)),

        "LcmaSts" : _extract_enum_value(_get_if_not_None(v.LcmaSts)),

        "LcmaSts1" : _extract_enum_value(_get_if_not_None(v.LcmaSts1)),

        "CtraSts" : _extract_enum_value(_get_if_not_None(v.CtraSts)),

        "CtraSts1" : _extract_enum_value(_get_if_not_None(v.CtraSts1)),

        "RcwSts" : _extract_enum_value(_get_if_not_None(v.RcwSts)),

        "RcmSts" : _extract_enum_value(_get_if_not_None(v.RcmSts)),


    })
def fromJson_SeatBackUnlckd(data):
    # type: (str) -> datatypes.SeatBackUnlckd
    j = json.loads(data)
    v = datatypes.SeatBackUnlckd()
    v.SeatBackUnlckdLe = j["SeatBackUnlckdLe"]
    v.SeatBackUnlckdRi = j["SeatBackUnlckdRi"]
    return v


def toJson_SeatBackUnlckd(v):
    # type: (datatypes.SeatBackUnlckd) -> str
    return json.dumps({
        "SeatBackUnlckdLe" : _get_if_not_None(v.SeatBackUnlckdLe),
        "SeatBackUnlckdRi" : _get_if_not_None(v.SeatBackUnlckdRi),

    })
def fromJson_SeatDispBtnPsd(data):
    # type: (str) -> datatypes.SeatDispBtnPsd
    j = json.loads(data)
    v = datatypes.SeatDispBtnPsd()
    v.DrvrSeatDispBtnPsd = j["DrvrSeatDispBtnPsd"]
    v.PassSeatDispBtnPsd = j["PassSeatDispBtnPsd"]
    return v


def toJson_SeatDispBtnPsd(v):
    # type: (datatypes.SeatDispBtnPsd) -> str
    return json.dumps({
        "DrvrSeatDispBtnPsd" : _get_if_not_None(v.DrvrSeatDispBtnPsd),
        "PassSeatDispBtnPsd" : _get_if_not_None(v.PassSeatDispBtnPsd),

    })
def fromJson_SeatMassgFct(data):
    # type: (str) -> datatypes.SeatMassgFct
    j = json.loads(data)
    v = datatypes.SeatMassgFct()
    v.OnOff = j["OnOff"]
    v.MassgProg = j["MassgProg"]
    v.MassgInten = j["MassgInten"]
    v.MassgSpdLvl = j["MassgSpdLvl"]
    return v


def toJson_SeatMassgFct(v):
    # type: (datatypes.SeatMassgFct) -> str
    return json.dumps({
        "OnOff" : _extract_enum_value(_get_if_not_None(v.OnOff)),

        "MassgProg" : _extract_enum_value(_get_if_not_None(v.MassgProg)),

        "MassgInten" : _extract_enum_value(_get_if_not_None(v.MassgInten)),

        "MassgSpdLvl" : _extract_enum_value(_get_if_not_None(v.MassgSpdLvl)),


    })
def fromJson_SetOfLang(data):
    # type: (str) -> datatypes.SetOfLang
    j = json.loads(data)
    v = datatypes.SetOfLang()
    v.IdPen = j["IdPen_"]
    v.LangTyp = j["LangTyp_"]
    return v


def toJson_SetOfLang(v):
    # type: (datatypes.SetOfLang) -> str
    return json.dumps({
        "IdPen_" : _extract_enum_value(_get_if_not_None(v.IdPen)),

        "LangTyp_" : _extract_enum_value(_get_if_not_None(v.LangTyp)),


    })
def fromJson_SetgAndRstOfTripForDrvr(data):
    # type: (str) -> datatypes.SetgAndRstOfTripForDrvr
    j = json.loads(data)
    v = datatypes.SetgAndRstOfTripForDrvr()
    v.SetgTripForDstLong = j["SetgTripForDstLong"]
    v.SetgTripForFuCns = j["SetgTripForFuCns"]
    v.SetgTripForFuDst = j["SetgTripForFuDst"]
    v.SetgTripForEgyDst = j["SetgTripForEgyDst"]
    v.SetgTripForDstSho1 = j["SetgTripForDstSho1"]
    v.SetgTripForFuCnsAvg1 = j["SetgTripForFuCnsAvg1"]
    v.SetgTripForEgyCnsAvg1 = j["SetgTripForEgyCnsAvg1"]
    v.SetgTripForSpdAvg1 = j["SetgTripForSpdAvg1"]
    v.SetgTripForTiDrv1 = j["SetgTripForTiDrv1"]
    v.SetgTripForDstSho2 = j["SetgTripForDstSho2"]
    v.SetgTripForFuCnsAvg2 = j["SetgTripForFuCnsAvg2"]
    v.SetgTripForEgyCnsAvg2 = j["SetgTripForEgyCnsAvg2"]
    v.SetgTripForSpdAvg2 = j["SetgTripForSpdAvg2"]
    v.SetgTripForTiDrv2 = j["SetgTripForTiDrv2"]
    v.SetgTripForDrvrMtr = j["SetgTripForDrvrMtr"]
    v.SetgTripForSpdPtr = j["SetgTripForSpdPtr"]
    v.SetgTripForSpdDig = j["SetgTripForSpdDig"]
    v.SetgTripForSpdTrvl = j["SetgTripForSpdTrvl"]
    v.SetgTripForRstOfAllTripFct1 = j["SetgTripForRstOfAllTripFct1"]
    v.SetgTripForTiRstCdn = j["SetgTripForTiRstCdn"]
    v.SetgTripOffOn = j["SetgTripOffOn"]
    v.SetgTripWithProfID = j["SetgTripWithProfID"]
    return v


def toJson_SetgAndRstOfTripForDrvr(v):
    # type: (datatypes.SetgAndRstOfTripForDrvr) -> str
    return json.dumps({
        "SetgTripForDstLong" : _extract_enum_value(_get_if_not_None(v.SetgTripForDstLong)),

        "SetgTripForFuCns" : _extract_enum_value(_get_if_not_None(v.SetgTripForFuCns)),

        "SetgTripForFuDst" : _extract_enum_value(_get_if_not_None(v.SetgTripForFuDst)),

        "SetgTripForEgyDst" : _extract_enum_value(_get_if_not_None(v.SetgTripForEgyDst)),

        "SetgTripForDstSho1" : _extract_enum_value(_get_if_not_None(v.SetgTripForDstSho1)),

        "SetgTripForFuCnsAvg1" : _extract_enum_value(_get_if_not_None(v.SetgTripForFuCnsAvg1)),

        "SetgTripForEgyCnsAvg1" : _extract_enum_value(_get_if_not_None(v.SetgTripForEgyCnsAvg1)),

        "SetgTripForSpdAvg1" : _extract_enum_value(_get_if_not_None(v.SetgTripForSpdAvg1)),

        "SetgTripForTiDrv1" : _extract_enum_value(_get_if_not_None(v.SetgTripForTiDrv1)),

        "SetgTripForDstSho2" : _extract_enum_value(_get_if_not_None(v.SetgTripForDstSho2)),

        "SetgTripForFuCnsAvg2" : _extract_enum_value(_get_if_not_None(v.SetgTripForFuCnsAvg2)),

        "SetgTripForEgyCnsAvg2" : _extract_enum_value(_get_if_not_None(v.SetgTripForEgyCnsAvg2)),

        "SetgTripForSpdAvg2" : _extract_enum_value(_get_if_not_None(v.SetgTripForSpdAvg2)),

        "SetgTripForTiDrv2" : _extract_enum_value(_get_if_not_None(v.SetgTripForTiDrv2)),

        "SetgTripForDrvrMtr" : _extract_enum_value(_get_if_not_None(v.SetgTripForDrvrMtr)),

        "SetgTripForSpdPtr" : _extract_enum_value(_get_if_not_None(v.SetgTripForSpdPtr)),

        "SetgTripForSpdDig" : _extract_enum_value(_get_if_not_None(v.SetgTripForSpdDig)),

        "SetgTripForSpdTrvl" : _extract_enum_value(_get_if_not_None(v.SetgTripForSpdTrvl)),

        "SetgTripForRstOfAllTripFct1" : _extract_enum_value(_get_if_not_None(v.SetgTripForRstOfAllTripFct1)),

        "SetgTripForTiRstCdn" : _get_if_not_None(v.SetgTripForTiRstCdn),
        "SetgTripOffOn" : _extract_enum_value(_get_if_not_None(v.SetgTripOffOn)),

        "SetgTripWithProfID" : _extract_enum_value(_get_if_not_None(v.SetgTripWithProfID)),


    })
def fromJson_SftyCchActvnSts2(data):
    # type: (str) -> datatypes.SftyCchActvnSts2
    j = json.loads(data)
    v = datatypes.SftyCchActvnSts2()
    v.Actv = j["Actv"]
    v.Yr = j["Yr"]
    v.Mth = j["Mth"]
    v.Day = j["Day"]
    return v


def toJson_SftyCchActvnSts2(v):
    # type: (datatypes.SftyCchActvnSts2) -> str
    return json.dumps({
        "Actv" : _extract_enum_value(_get_if_not_None(v.Actv)),

        "Yr" : _get_if_not_None(v.Yr),
        "Mth" : _get_if_not_None(v.Mth),
        "Day" : _get_if_not_None(v.Day),

    })
def fromJson_SftyCchIdx1(data):
    # type: (str) -> datatypes.SftyCchIdx1
    j = json.loads(data)
    v = datatypes.SftyCchIdx1()
    v.Tot = j["Tot"]
    v.TotAvl = j["TotAvl"]
    v.DstToVeh = j["DstToVeh"]
    v.DstToVehAvl = j["DstToVehAvl"]
    v.LaneKeep = j["LaneKeep"]
    v.LaneKeepAvl = j["LaneKeepAvl"]
    v.Attention = j["Attention"]
    v.AttentionAvl = j["AttentionAvl"]
    v.KeepSpdLim = j["KeepSpdLim"]
    v.KeepSpdLimAvl = j["KeepSpdLimAvl"]
    return v


def toJson_SftyCchIdx1(v):
    # type: (datatypes.SftyCchIdx1) -> str
    return json.dumps({
        "Tot" : _get_if_not_None(v.Tot),
        "TotAvl" : _extract_enum_value(_get_if_not_None(v.TotAvl)),

        "DstToVeh" : _get_if_not_None(v.DstToVeh),
        "DstToVehAvl" : _extract_enum_value(_get_if_not_None(v.DstToVehAvl)),

        "LaneKeep" : _get_if_not_None(v.LaneKeep),
        "LaneKeepAvl" : _extract_enum_value(_get_if_not_None(v.LaneKeepAvl)),

        "Attention" : _get_if_not_None(v.Attention),
        "AttentionAvl" : _extract_enum_value(_get_if_not_None(v.AttentionAvl)),

        "KeepSpdLim" : _get_if_not_None(v.KeepSpdLim),
        "KeepSpdLimAvl" : _extract_enum_value(_get_if_not_None(v.KeepSpdLimAvl)),


    })
def fromJson_SftyCchIdx2(data):
    # type: (str) -> datatypes.SftyCchIdx2
    j = json.loads(data)
    v = datatypes.SftyCchIdx2()
    v.Tot = j["Tot"]
    v.TotAvl = j["TotAvl"]
    v.TotCntxt = j["TotCntxt"]
    v.DstToVeh = j["DstToVeh"]
    v.DstToVehAvl = j["DstToVehAvl"]
    v.DstToVehCntxt = j["DstToVehCntxt"]
    v.LaneKeep = j["LaneKeep"]
    v.LaneKeepAvl = j["LaneKeepAvl"]
    v.LaneKeepCntxt = j["LaneKeepCntxt"]
    v.Attention = j["Attention"]
    v.AttentionAvl = j["AttentionAvl"]
    v.AttentionCntxt = j["AttentionCntxt"]
    v.KeepSpdLim = j["KeepSpdLim"]
    v.KeepSpdLimAvl = j["KeepSpdLimAvl"]
    v.KeepSpdLimCntxt = j["KeepSpdLimCntxt"]
    v.FldTot = j["FldTot"]
    v.FldTotAvl = j["FldTotAvl"]
    return v


def toJson_SftyCchIdx2(v):
    # type: (datatypes.SftyCchIdx2) -> str
    return json.dumps({
        "Tot" : _get_if_not_None(v.Tot),
        "TotAvl" : _extract_enum_value(_get_if_not_None(v.TotAvl)),

        "TotCntxt" : _extract_enum_value(_get_if_not_None(v.TotCntxt)),

        "DstToVeh" : _get_if_not_None(v.DstToVeh),
        "DstToVehAvl" : _extract_enum_value(_get_if_not_None(v.DstToVehAvl)),

        "DstToVehCntxt" : _extract_enum_value(_get_if_not_None(v.DstToVehCntxt)),

        "LaneKeep" : _get_if_not_None(v.LaneKeep),
        "LaneKeepAvl" : _extract_enum_value(_get_if_not_None(v.LaneKeepAvl)),

        "LaneKeepCntxt" : _extract_enum_value(_get_if_not_None(v.LaneKeepCntxt)),

        "Attention" : _get_if_not_None(v.Attention),
        "AttentionAvl" : _extract_enum_value(_get_if_not_None(v.AttentionAvl)),

        "AttentionCntxt" : _extract_enum_value(_get_if_not_None(v.AttentionCntxt)),

        "KeepSpdLim" : _get_if_not_None(v.KeepSpdLim),
        "KeepSpdLimAvl" : _extract_enum_value(_get_if_not_None(v.KeepSpdLimAvl)),

        "KeepSpdLimCntxt" : _extract_enum_value(_get_if_not_None(v.KeepSpdLimCntxt)),

        "FldTot" : _get_if_not_None(v.FldTot),
        "FldTotAvl" : _extract_enum_value(_get_if_not_None(v.FldTotAvl)),


    })
def fromJson_SftySigGroupFromAudSafe1(data):
    # type: (str) -> datatypes.SftySigGroupFromAudSafe1
    j = json.loads(data)
    v = datatypes.SftySigGroupFromAudSafe1()
    v.SftySigFaildDetdByAud = j["SftySigFaildDetdByAud"]
    v.SftyAudDend = j["SftyAudDend"]
    v.SftyAudEna = j["SftyAudEna"]
    v.SftySigGroupFromAudSafeChks = j["SftySigGroupFromAudSafeChks"]
    v.SftySigGroupFromAudSafeCntr = j["SftySigGroupFromAudSafeCntr"]
    return v


def toJson_SftySigGroupFromAudSafe1(v):
    # type: (datatypes.SftySigGroupFromAudSafe1) -> str
    return json.dumps({
        "SftySigFaildDetdByAud" : _extract_enum_value(_get_if_not_None(v.SftySigFaildDetdByAud)),

        "SftyAudDend" : _extract_enum_value(_get_if_not_None(v.SftyAudDend)),

        "SftyAudEna" : _extract_enum_value(_get_if_not_None(v.SftyAudEna)),

        "SftySigGroupFromAudSafeChks" : _get_if_not_None(v.SftySigGroupFromAudSafeChks),
        "SftySigGroupFromAudSafeCntr" : _get_if_not_None(v.SftySigGroupFromAudSafeCntr),

    })
def fromJson_SftyWarnGroupFromAsySafe1(data):
    # type: (str) -> datatypes.SftyWarnGroupFromAsySafe1
    j = json.loads(data)
    v = datatypes.SftyWarnGroupFromAsySafe1()
    v.CnclWarnLatForAutDrv = j["CnclWarnLatForAutDrv"]
    v.SteerOvrdWarnReqForAutDrv = j["SteerOvrdWarnReqForAutDrv"]
    v.SteerStsForAutDrv = j["SteerStsForAutDrv"]
    v.SftyWarnGroupFromAsySafeChks = j["SftyWarnGroupFromAsySafeChks"]
    v.SftyWarnGroupFromAsySafeCntr = j["SftyWarnGroupFromAsySafeCntr"]
    return v


def toJson_SftyWarnGroupFromAsySafe1(v):
    # type: (datatypes.SftyWarnGroupFromAsySafe1) -> str
    return json.dumps({
        "CnclWarnLatForAutDrv" : _extract_enum_value(_get_if_not_None(v.CnclWarnLatForAutDrv)),

        "SteerOvrdWarnReqForAutDrv" : _extract_enum_value(_get_if_not_None(v.SteerOvrdWarnReqForAutDrv)),

        "SteerStsForAutDrv" : _extract_enum_value(_get_if_not_None(v.SteerStsForAutDrv)),

        "SftyWarnGroupFromAsySafeChks" : _get_if_not_None(v.SftyWarnGroupFromAsySafeChks),
        "SftyWarnGroupFromAsySafeCntr" : _get_if_not_None(v.SftyWarnGroupFromAsySafeCntr),

    })
def fromJson_SlowNormFastPen(data):
    # type: (str) -> datatypes.SlowNormFastPen
    j = json.loads(data)
    v = datatypes.SlowNormFastPen()
    v.SlowNormFast = j["SlowNormFast_"]
    v.IdPen = j["IdPen_"]
    return v


def toJson_SlowNormFastPen(v):
    # type: (datatypes.SlowNormFastPen) -> str
    return json.dumps({
        "SlowNormFast_" : _extract_enum_value(_get_if_not_None(v.SlowNormFast)),

        "IdPen_" : _extract_enum_value(_get_if_not_None(v.IdPen)),


    })
def fromJson_SnsrParkAssi1(data):
    # type: (str) -> datatypes.SnsrParkAssi1
    j = json.loads(data)
    v = datatypes.SnsrParkAssi1()
    v.DstOfSnsrInsdLe = j["DstOfSnsrInsdLe"]
    v.DstOfSnsrOutdLe = j["DstOfSnsrOutdLe"]
    v.DstOfSnsrLeSide = j["DstOfSnsrLeSide"]
    v.DstOfSnsrInsdRi = j["DstOfSnsrInsdRi"]
    v.DstOfSnsrOutdRi = j["DstOfSnsrOutdRi"]
    v.DstOfSnsrRiSide = j["DstOfSnsrRiSide"]
    v.AudWarnDir = j["AudWarnDir"]
    v.AudSideWarn = j["AudSideWarn"]
    return v


def toJson_SnsrParkAssi1(v):
    # type: (datatypes.SnsrParkAssi1) -> str
    return json.dumps({
        "DstOfSnsrInsdLe" : _extract_enum_value(_get_if_not_None(v.DstOfSnsrInsdLe)),

        "DstOfSnsrOutdLe" : _extract_enum_value(_get_if_not_None(v.DstOfSnsrOutdLe)),

        "DstOfSnsrLeSide" : _extract_enum_value(_get_if_not_None(v.DstOfSnsrLeSide)),

        "DstOfSnsrInsdRi" : _extract_enum_value(_get_if_not_None(v.DstOfSnsrInsdRi)),

        "DstOfSnsrOutdRi" : _extract_enum_value(_get_if_not_None(v.DstOfSnsrOutdRi)),

        "DstOfSnsrRiSide" : _extract_enum_value(_get_if_not_None(v.DstOfSnsrRiSide)),

        "AudWarnDir" : _extract_enum_value(_get_if_not_None(v.AudWarnDir)),

        "AudSideWarn" : _extract_enum_value(_get_if_not_None(v.AudSideWarn)),


    })
def fromJson_SnsrPrkgAssi2(data):
    # type: (str) -> datatypes.SnsrPrkgAssi2
    j = json.loads(data)
    v = datatypes.SnsrPrkgAssi2()
    v.SnsrDstInsdLe = j["SnsrDstInsdLe"]
    v.SnsrDstOutdLe = j["SnsrDstOutdLe"]
    v.SnsrDstSideLe = j["SnsrDstSideLe"]
    v.SnsrDstInsdRi = j["SnsrDstInsdRi"]
    v.SnsrDstOutdRi = j["SnsrDstOutdRi"]
    v.SnsrDstSideRi = j["SnsrDstSideRi"]
    v.AudWarnDir = j["AudWarnDir"]
    v.AudSideWarn = j["AudSideWarn"]
    return v


def toJson_SnsrPrkgAssi2(v):
    # type: (datatypes.SnsrPrkgAssi2) -> str
    return json.dumps({
        "SnsrDstInsdLe" : _get_if_not_None(v.SnsrDstInsdLe),
        "SnsrDstOutdLe" : _get_if_not_None(v.SnsrDstOutdLe),
        "SnsrDstSideLe" : _get_if_not_None(v.SnsrDstSideLe),
        "SnsrDstInsdRi" : _get_if_not_None(v.SnsrDstInsdRi),
        "SnsrDstOutdRi" : _get_if_not_None(v.SnsrDstOutdRi),
        "SnsrDstSideRi" : _get_if_not_None(v.SnsrDstSideRi),
        "AudWarnDir" : _extract_enum_value(_get_if_not_None(v.AudWarnDir)),

        "AudSideWarn" : _extract_enum_value(_get_if_not_None(v.AudSideWarn)),


    })
def fromJson_SnsrPrkgAssi3(data):
    # type: (str) -> datatypes.SnsrPrkgAssi3
    j = json.loads(data)
    v = datatypes.SnsrPrkgAssi3()
    v.SnsrDstInsdLe = j["SnsrDstInsdLe"]
    v.SnsrDstOutdLe = j["SnsrDstOutdLe"]
    v.SnsrDstSideLe = j["SnsrDstSideLe"]
    v.SnsrDstInsdRi = j["SnsrDstInsdRi"]
    v.SnsrDstOutdRi = j["SnsrDstOutdRi"]
    v.SnsrDstSideRi = j["SnsrDstSideRi"]
    return v


def toJson_SnsrPrkgAssi3(v):
    # type: (datatypes.SnsrPrkgAssi3) -> str
    return json.dumps({
        "SnsrDstInsdLe" : _get_if_not_None(v.SnsrDstInsdLe),
        "SnsrDstOutdLe" : _get_if_not_None(v.SnsrDstOutdLe),
        "SnsrDstSideLe" : _get_if_not_None(v.SnsrDstSideLe),
        "SnsrDstInsdRi" : _get_if_not_None(v.SnsrDstInsdRi),
        "SnsrDstOutdRi" : _get_if_not_None(v.SnsrDstOutdRi),
        "SnsrDstSideRi" : _get_if_not_None(v.SnsrDstSideRi),

    })
def fromJson_SnvtyPen1(data):
    # type: (str) -> datatypes.SnvtyPen1
    j = json.loads(data)
    v = datatypes.SnvtyPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_SnvtyPen1(v):
    # type: (datatypes.SnvtyPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_SnvtyPen3(data):
    # type: (str) -> datatypes.SnvtyPen3
    j = json.loads(data)
    v = datatypes.SnvtyPen3()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_SnvtyPen3(v):
    # type: (datatypes.SnvtyPen3) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_SteerAssiLvlForAutDrvPen1(data):
    # type: (str) -> datatypes.SteerAssiLvlForAutDrvPen1
    j = json.loads(data)
    v = datatypes.SteerAssiLvlForAutDrvPen1()
    v.Pen = j["Pen"]
    v.Sts = j["Sts"]
    return v


def toJson_SteerAssiLvlForAutDrvPen1(v):
    # type: (datatypes.SteerAssiLvlForAutDrvPen1) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),


    })
def fromJson_SteerSetg(data):
    # type: (str) -> datatypes.SteerSetg
    j = json.loads(data)
    v = datatypes.SteerSetg()
    v.SteerAsscLvl = j["SteerAsscLvl_"]
    v.SteerMod = j["SteerMod_"]
    v.Pen = j["Pen"]
    return v


def toJson_SteerSetg(v):
    # type: (datatypes.SteerSetg) -> str
    return json.dumps({
        "SteerAsscLvl_" : _extract_enum_value(_get_if_not_None(v.SteerAsscLvl)),

        "SteerMod_" : _extract_enum_value(_get_if_not_None(v.SteerMod)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_SteerWhlSnsrSafe1(data):
    # type: (str) -> datatypes.SteerWhlSnsrSafe1
    j = json.loads(data)
    v = datatypes.SteerWhlSnsrSafe1()
    v.SteerWhlAg = j["SteerWhlAg"]
    v.SteerWhlAgSpd = j["SteerWhlAgSpd"]
    v.SteerWhlSnsrQf = j["SteerWhlSnsrQf"]
    v.SteerWhlSnsrChks = j["SteerWhlSnsrChks"]
    v.SteerWhlSnsrCntr = j["SteerWhlSnsrCntr"]
    return v


def toJson_SteerWhlSnsrSafe1(v):
    # type: (datatypes.SteerWhlSnsrSafe1) -> str
    return json.dumps({
        "SteerWhlAg" : _get_if_not_None(v.SteerWhlAg),
        "SteerWhlAgSpd" : _get_if_not_None(v.SteerWhlAgSpd),
        "SteerWhlSnsrQf" : _extract_enum_value(_get_if_not_None(v.SteerWhlSnsrQf)),

        "SteerWhlSnsrChks" : _get_if_not_None(v.SteerWhlSnsrChks),
        "SteerWhlSnsrCntr" : _get_if_not_None(v.SteerWhlSnsrCntr),

    })
def fromJson_SuspSetgRec(data):
    # type: (str) -> datatypes.SuspSetgRec
    j = json.loads(data)
    v = datatypes.SuspSetgRec()
    v.SuspIdPen = j["SuspIdPen"]
    v.LvlCtrlSetg = j["LvlCtrlSetg"]
    v.SuspCtrlSetg = j["SuspCtrlSetg"]
    return v


def toJson_SuspSetgRec(v):
    # type: (datatypes.SuspSetgRec) -> str
    return json.dumps({
        "SuspIdPen" : _extract_enum_value(_get_if_not_None(v.SuspIdPen)),

        "LvlCtrlSetg" : _extract_enum_value(_get_if_not_None(v.LvlCtrlSetg)),

        "SuspCtrlSetg" : _extract_enum_value(_get_if_not_None(v.SuspCtrlSetg)),


    })
def fromJson_SysU(data):
    # type: (str) -> datatypes.SysU
    j = json.loads(data)
    v = datatypes.SysU()
    v.SysU = j["SysU"]
    v.SysUQf = j["SysUQf"]
    return v


def toJson_SysU(v):
    # type: (datatypes.SysU) -> str
    return json.dumps({
        "SysU" : _get_if_not_None(v.SysU),
        "SysUQf" : _extract_enum_value(_get_if_not_None(v.SysUQf)),


    })
def fromJson_TiCorrn(data):
    # type: (str) -> datatypes.TiCorrn
    j = json.loads(data)
    v = datatypes.TiCorrn()
    v.HrCorrn = j["HrCorrn"]
    v.MinsCorrn = j["MinsCorrn"]
    v.DayLiSaveTi = j["DayLiSaveTi"]
    v.SpprtForFct = j["SpprtForFct"]
    return v


def toJson_TiCorrn(v):
    # type: (datatypes.TiCorrn) -> str
    return json.dumps({
        "HrCorrn" : _get_if_not_None(v.HrCorrn),
        "MinsCorrn" : _get_if_not_None(v.MinsCorrn),
        "DayLiSaveTi" : _get_if_not_None(v.DayLiSaveTi),
        "SpprtForFct" : _extract_enum_value(_get_if_not_None(v.SpprtForFct)),


    })
def fromJson_TiGapAdpvSeldPen1(data):
    # type: (str) -> datatypes.TiGapAdpvSeldPen1
    j = json.loads(data)
    v = datatypes.TiGapAdpvSeldPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_TiGapAdpvSeldPen1(v):
    # type: (datatypes.TiGapAdpvSeldPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_TiGapPen1(data):
    # type: (str) -> datatypes.TiGapPen1
    j = json.loads(data)
    v = datatypes.TiGapPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_TiGapPen1(v):
    # type: (datatypes.TiGapPen1) -> str
    return json.dumps({
        "Sts" : _get_if_not_None(v.Sts),
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_TireCircumCalByNav(data):
    # type: (str) -> datatypes.TireCircumCalByNav
    j = json.loads(data)
    v = datatypes.TireCircumCalByNav()
    v.TireCircum = j["TireCircum"]
    v.HiQly = j["HiQly"]
    v.SpprtForFct = j["SpprtForFct"]
    return v


def toJson_TireCircumCalByNav(v):
    # type: (datatypes.TireCircumCalByNav) -> str
    return json.dumps({
        "TireCircum" : _get_if_not_None(v.TireCircum),
        "HiQly" : _extract_enum_value(_get_if_not_None(v.HiQly)),

        "SpprtForFct" : _extract_enum_value(_get_if_not_None(v.SpprtForFct)),


    })
def fromJson_TirePAbsltValFrntLe3(data):
    # type: (str) -> datatypes.TirePAbsltValFrntLe3
    j = json.loads(data)
    v = datatypes.TirePAbsltValFrntLe3()
    v.TirepabsltVal1 = j["TirepabsltVal1"]
    v.TirePabsltValQF = j["TirePabsltValQF"]
    v.TirePPosn = j["TirePPosn_"]
    return v


def toJson_TirePAbsltValFrntLe3(v):
    # type: (datatypes.TirePAbsltValFrntLe3) -> str
    return json.dumps({
        "TirepabsltVal1" : _get_if_not_None(v.TirepabsltVal1),
        "TirePabsltValQF" : _extract_enum_value(_get_if_not_None(v.TirePabsltValQF)),

        "TirePPosn_" : _extract_enum_value(_get_if_not_None(v.TirePPosn)),


    })
def fromJson_TirePAbsltValFrntRi1(data):
    # type: (str) -> datatypes.TirePAbsltValFrntRi1
    j = json.loads(data)
    v = datatypes.TirePAbsltValFrntRi1()
    v.TirePAbsltVal1 = j["TirePAbsltVal1"]
    v.TirePAbsltValQF = j["TirePAbsltValQF"]
    v.TirePPosn = j["TirePPosn_"]
    return v


def toJson_TirePAbsltValFrntRi1(v):
    # type: (datatypes.TirePAbsltValFrntRi1) -> str
    return json.dumps({
        "TirePAbsltVal1" : _get_if_not_None(v.TirePAbsltVal1),
        "TirePAbsltValQF" : _extract_enum_value(_get_if_not_None(v.TirePAbsltValQF)),

        "TirePPosn_" : _extract_enum_value(_get_if_not_None(v.TirePPosn)),


    })
def fromJson_TirePAbsltValReLe1(data):
    # type: (str) -> datatypes.TirePAbsltValReLe1
    j = json.loads(data)
    v = datatypes.TirePAbsltValReLe1()
    v.TirePAbsltVal1 = j["TirePAbsltVal1"]
    v.TirePAbsltValQF = j["TirePAbsltValQF"]
    v.TirePPosn = j["TirePPosn_"]
    return v


def toJson_TirePAbsltValReLe1(v):
    # type: (datatypes.TirePAbsltValReLe1) -> str
    return json.dumps({
        "TirePAbsltVal1" : _get_if_not_None(v.TirePAbsltVal1),
        "TirePAbsltValQF" : _extract_enum_value(_get_if_not_None(v.TirePAbsltValQF)),

        "TirePPosn_" : _extract_enum_value(_get_if_not_None(v.TirePPosn)),


    })
def fromJson_TirePAbsltValReRi1(data):
    # type: (str) -> datatypes.TirePAbsltValReRi1
    j = json.loads(data)
    v = datatypes.TirePAbsltValReRi1()
    v.TirePAbsltVal1 = j["TirePAbsltVal1"]
    v.TirePAbsltValQF = j["TirePAbsltValQF"]
    v.TirePPosn = j["TirePPosn_"]
    return v


def toJson_TirePAbsltValReRi1(v):
    # type: (datatypes.TirePAbsltValReRi1) -> str
    return json.dumps({
        "TirePAbsltVal1" : _get_if_not_None(v.TirePAbsltVal1),
        "TirePAbsltValQF" : _extract_enum_value(_get_if_not_None(v.TirePAbsltValQF)),

        "TirePPosn_" : _extract_enum_value(_get_if_not_None(v.TirePPosn)),


    })
def fromJson_TirePMonData1(data):
    # type: (str) -> datatypes.TirePMonData1
    j = json.loads(data)
    v = datatypes.TirePMonData1()
    v.TirePMonSts1 = j["TirePMonSts1_"]
    v.TirePReRi = j["TirePReRi"]
    v.TirePReLe = j["TirePReLe"]
    v.TirePFrntLe = j["TirePFrntLe"]
    v.TirePFrntRe = j["TirePFrntRe"]
    return v


def toJson_TirePMonData1(v):
    # type: (datatypes.TirePMonData1) -> str
    return json.dumps({
        "TirePMonSts1_" : _extract_enum_value(_get_if_not_None(v.TirePMonSts1)),

        "TirePReRi" : _extract_enum_value(_get_if_not_None(v.TirePReRi)),

        "TirePReLe" : _extract_enum_value(_get_if_not_None(v.TirePReLe)),

        "TirePFrntLe" : _extract_enum_value(_get_if_not_None(v.TirePFrntLe)),

        "TirePFrntRe" : _extract_enum_value(_get_if_not_None(v.TirePFrntRe)),


    })
def fromJson_TqSafe2(data):
    # type: (str) -> datatypes.TqSafe2
    j = json.loads(data)
    v = datatypes.TqSafe2()
    v.TqAct = j["TqAct"]
    v.TqActChks = j["TqActChks"]
    v.TqActCntr = j["TqActCntr"]
    v.TqActQlyFac = j["TqActQlyFac"]
    v.SpdLimn = j["SpdLimn"]
    return v


def toJson_TqSafe2(v):
    # type: (datatypes.TqSafe2) -> str
    return json.dumps({
        "TqAct" : _get_if_not_None(v.TqAct),
        "TqActChks" : _get_if_not_None(v.TqActChks),
        "TqActCntr" : _get_if_not_None(v.TqActCntr),
        "TqActQlyFac" : _extract_enum_value(_get_if_not_None(v.TqActQlyFac)),

        "SpdLimn" : _get_if_not_None(v.SpdLimn),

    })
def fromJson_TwliBriRaw(data):
    # type: (str) -> datatypes.TwliBriRaw
    j = json.loads(data)
    v = datatypes.TwliBriRaw()
    v.TwliBriRaw1 = j["TwliBriRaw1"]
    v.TwliBriRawQf = j["TwliBriRawQf"]
    return v


def toJson_TwliBriRaw(v):
    # type: (datatypes.TwliBriRaw) -> str
    return json.dumps({
        "TwliBriRaw1" : _get_if_not_None(v.TwliBriRaw1),
        "TwliBriRawQf" : _extract_enum_value(_get_if_not_None(v.TwliBriRawQf)),


    })
def fromJson_UInt64Rec(data):
    # type: (str) -> datatypes.UInt64Rec
    j = json.loads(data)
    v = datatypes.UInt64Rec()
    v.Byte0 = j["Byte0"]
    v.Byte1 = j["Byte1"]
    v.Byte2 = j["Byte2"]
    v.Byte3 = j["Byte3"]
    v.Byte4 = j["Byte4"]
    v.Byte5 = j["Byte5"]
    v.Byte6 = j["Byte6"]
    v.Byte7 = j["Byte7"]
    return v


def toJson_UInt64Rec(v):
    # type: (datatypes.UInt64Rec) -> str
    return json.dumps({
        "Byte0" : _get_if_not_None(v.Byte0),
        "Byte1" : _get_if_not_None(v.Byte1),
        "Byte2" : _get_if_not_None(v.Byte2),
        "Byte3" : _get_if_not_None(v.Byte3),
        "Byte4" : _get_if_not_None(v.Byte4),
        "Byte5" : _get_if_not_None(v.Byte5),
        "Byte6" : _get_if_not_None(v.Byte6),
        "Byte7" : _get_if_not_None(v.Byte7),

    })
def fromJson_UnlckKeylsCfgPen2(data):
    # type: (str) -> datatypes.UnlckKeylsCfgPen2
    j = json.loads(data)
    v = datatypes.UnlckKeylsCfgPen2()
    v.IdPen = j["IdPen_"]
    v.KeylsCfg = j["KeylsCfg"]
    return v


def toJson_UnlckKeylsCfgPen2(v):
    # type: (datatypes.UnlckKeylsCfgPen2) -> str
    return json.dumps({
        "IdPen_" : _extract_enum_value(_get_if_not_None(v.IdPen)),

        "KeylsCfg" : _extract_enum_value(_get_if_not_None(v.KeylsCfg)),


    })
def fromJson_UnlckRemCfgPen1(data):
    # type: (str) -> datatypes.UnlckRemCfgPen1
    j = json.loads(data)
    v = datatypes.UnlckRemCfgPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_UnlckRemCfgPen1(v):
    # type: (datatypes.UnlckRemCfgPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_UsrSetSpdForKeySpdLimn(data):
    # type: (str) -> datatypes.UsrSetSpdForKeySpdLimn
    j = json.loads(data)
    v = datatypes.UsrSetSpdForKeySpdLimn()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_UsrSetSpdForKeySpdLimn(v):
    # type: (datatypes.UsrSetSpdForKeySpdLimn) -> str
    return json.dumps({
        "Sts" : _get_if_not_None(v.Sts),
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_UsrSetSpdForKeySpdWarn(data):
    # type: (str) -> datatypes.UsrSetSpdForKeySpdWarn
    j = json.loads(data)
    v = datatypes.UsrSetSpdForKeySpdWarn()
    v.UsrSetSpdForKeySpdWarn1 = j["UsrSetSpdForKeySpdWarn1"]
    v.UsrSetSpdForKeySpdWarn2 = j["UsrSetSpdForKeySpdWarn2"]
    v.UsrSetSpdForKeySpdWarn3 = j["UsrSetSpdForKeySpdWarn3"]
    v.UsrSetSpdForKeySpdWarn4 = j["UsrSetSpdForKeySpdWarn4"]
    v.UsrSetSpdForKeySpdWarn5 = j["UsrSetSpdForKeySpdWarn5"]
    v.UsrSetSpdForKeySpdWarn6 = j["UsrSetSpdForKeySpdWarn6"]
    v.UsrSetSpdForKeySpdWarnPen = j["UsrSetSpdForKeySpdWarnPen"]
    return v


def toJson_UsrSetSpdForKeySpdWarn(v):
    # type: (datatypes.UsrSetSpdForKeySpdWarn) -> str
    return json.dumps({
        "UsrSetSpdForKeySpdWarn1" : _get_if_not_None(v.UsrSetSpdForKeySpdWarn1),
        "UsrSetSpdForKeySpdWarn2" : _get_if_not_None(v.UsrSetSpdForKeySpdWarn2),
        "UsrSetSpdForKeySpdWarn3" : _get_if_not_None(v.UsrSetSpdForKeySpdWarn3),
        "UsrSetSpdForKeySpdWarn4" : _get_if_not_None(v.UsrSetSpdForKeySpdWarn4),
        "UsrSetSpdForKeySpdWarn5" : _get_if_not_None(v.UsrSetSpdForKeySpdWarn5),
        "UsrSetSpdForKeySpdWarn6" : _get_if_not_None(v.UsrSetSpdForKeySpdWarn6),
        "UsrSetSpdForKeySpdWarnPen" : _extract_enum_value(_get_if_not_None(v.UsrSetSpdForKeySpdWarnPen)),


    })
def fromJson_UsrSetVolMaxForKeyVolLimn(data):
    # type: (str) -> datatypes.UsrSetVolMaxForKeyVolLimn
    j = json.loads(data)
    v = datatypes.UsrSetVolMaxForKeyVolLimn()
    v.Pen = j["Pen"]
    v.SetVol = j["SetVol_"]
    return v


def toJson_UsrSetVolMaxForKeyVolLimn(v):
    # type: (datatypes.UsrSetVolMaxForKeyVolLimn) -> str
    return json.dumps({
        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),

        "SetVol_" : _extract_enum_value(_get_if_not_None(v.SetVol)),


    })
def fromJson_UsrSwtDispClimaReqForRowSec(data):
    # type: (str) -> datatypes.UsrSwtDispClimaReqForRowSec
    j = json.loads(data)
    v = datatypes.UsrSwtDispClimaReqForRowSec()
    v.UsrSwtDispClimaReqForTSpForRowSecLe = j["UsrSwtDispClimaReqForTSpForRowSecLe"]
    v.UsrSwtDispUpdClimaReqForTSpForRowSecLe = j["UsrSwtDispUpdClimaReqForTSpForRowSecLe"]
    v.UsrSwtDispClimaReqForTSpForRowSecRi = j["UsrSwtDispClimaReqForTSpForRowSecRi"]
    v.UsrSwtDispUpdClimaReqForTSpForRowSecRi = j["UsrSwtDispUpdClimaReqForTSpForRowSecRi"]
    v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe = j["UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe"]
    v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe = j["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe"]
    v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi = j["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi"]
    v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi = j["UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi"]
    v.UsrSwtDispReqForFanLvlForRowSec = j["UsrSwtDispReqForFanLvlForRowSec"]
    v.UsrSwtDispUpdReqForFanLvlForRowSec = j["UsrSwtDispUpdReqForFanLvlForRowSec"]
    v.UsrSwtDispClimaReqForTSpSpclForRowSecLe = j["UsrSwtDispClimaReqForTSpSpclForRowSecLe"]
    v.UsrSwtDispClimaReqForTSpSpclForRowSecRi = j["UsrSwtDispClimaReqForTSpSpclForRowSecRi"]
    return v


def toJson_UsrSwtDispClimaReqForRowSec(v):
    # type: (datatypes.UsrSwtDispClimaReqForRowSec) -> str
    return json.dumps({
        "UsrSwtDispClimaReqForTSpForRowSecLe" : _get_if_not_None(v.UsrSwtDispClimaReqForTSpForRowSecLe),
        "UsrSwtDispUpdClimaReqForTSpForRowSecLe" : _get_if_not_None(v.UsrSwtDispUpdClimaReqForTSpForRowSecLe),
        "UsrSwtDispClimaReqForTSpForRowSecRi" : _get_if_not_None(v.UsrSwtDispClimaReqForTSpForRowSecRi),
        "UsrSwtDispUpdClimaReqForTSpForRowSecRi" : _get_if_not_None(v.UsrSwtDispUpdClimaReqForTSpForRowSecRi),
        "UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe)),

        "UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe" : _get_if_not_None(v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe),
        "UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi" : _get_if_not_None(v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi),
        "UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi)),

        "UsrSwtDispReqForFanLvlForRowSec" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispReqForFanLvlForRowSec)),

        "UsrSwtDispUpdReqForFanLvlForRowSec" : _get_if_not_None(v.UsrSwtDispUpdReqForFanLvlForRowSec),
        "UsrSwtDispClimaReqForTSpSpclForRowSecLe" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispClimaReqForTSpSpclForRowSecLe)),

        "UsrSwtDispClimaReqForTSpSpclForRowSecRi" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispClimaReqForTSpSpclForRowSecRi)),


    })
def fromJson_UsrSwtDispClimaSts(data):
    # type: (str) -> datatypes.UsrSwtDispClimaSts
    j = json.loads(data)
    v = datatypes.UsrSwtDispClimaSts()
    v.UsrSwtDispClimaTSpForRowSecLe = j["UsrSwtDispClimaTSpForRowSecLe"]
    v.UsrSwtDispClimaTSpForRowSecRi = j["UsrSwtDispClimaTSpForRowSecRi"]
    v.UsrSwtDispSeatHeatLvlForRowSecLe = j["UsrSwtDispSeatHeatLvlForRowSecLe"]
    v.UsrSwtDispSeatHeatLvlForRowSecRi = j["UsrSwtDispSeatHeatLvlForRowSecRi"]
    v.UsrSwtDispFanLvlForRowSec = j["UsrSwtDispFanLvlForRowSec"]
    v.UsrSwtDispClimaTSpSpclForRowSecLe = j["UsrSwtDispClimaTSpSpclForRowSecLe"]
    v.UsrSwtDispClimaTSpSpclForRowSecRi = j["UsrSwtDispClimaTSpSpclForRowSecRi"]
    return v


def toJson_UsrSwtDispClimaSts(v):
    # type: (datatypes.UsrSwtDispClimaSts) -> str
    return json.dumps({
        "UsrSwtDispClimaTSpForRowSecLe" : _get_if_not_None(v.UsrSwtDispClimaTSpForRowSecLe),
        "UsrSwtDispClimaTSpForRowSecRi" : _get_if_not_None(v.UsrSwtDispClimaTSpForRowSecRi),
        "UsrSwtDispSeatHeatLvlForRowSecLe" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispSeatHeatLvlForRowSecLe)),

        "UsrSwtDispSeatHeatLvlForRowSecRi" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispSeatHeatLvlForRowSecRi)),

        "UsrSwtDispFanLvlForRowSec" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispFanLvlForRowSec)),

        "UsrSwtDispClimaTSpSpclForRowSecLe" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispClimaTSpSpclForRowSecLe)),

        "UsrSwtDispClimaTSpSpclForRowSecRi" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispClimaTSpSpclForRowSecRi)),


    })
def fromJson_UsrSwtDispForSecRowSeatVentn(data):
    # type: (str) -> datatypes.UsrSwtDispForSecRowSeatVentn
    j = json.loads(data)
    v = datatypes.UsrSwtDispForSecRowSeatVentn()
    v.UsrSwtDispForSecRowSeatVentnLe = j["UsrSwtDispForSecRowSeatVentnLe"]
    v.UsrSwtDispForSecRowSeatVentnRi = j["UsrSwtDispForSecRowSeatVentnRi"]
    return v


def toJson_UsrSwtDispForSecRowSeatVentn(v):
    # type: (datatypes.UsrSwtDispForSecRowSeatVentn) -> str
    return json.dumps({
        "UsrSwtDispForSecRowSeatVentnLe" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispForSecRowSeatVentnLe)),

        "UsrSwtDispForSecRowSeatVentnRi" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispForSecRowSeatVentnRi)),


    })
def fromJson_UsrSwtDispReqForSecRowSeatVentn(data):
    # type: (str) -> datatypes.UsrSwtDispReqForSecRowSeatVentn
    j = json.loads(data)
    v = datatypes.UsrSwtDispReqForSecRowSeatVentn()
    v.UsrSwtDispReqForSecRowSeatVentnLe = j["UsrSwtDispReqForSecRowSeatVentnLe"]
    v.UsrSwtDispReqForSecRowSeatVentnRi = j["UsrSwtDispReqForSecRowSeatVentnRi"]
    v.usrSwtDispUpdReqForSecRowSeatVentnLe = j["usrSwtDispUpdReqForSecRowSeatVentnLe"]
    v.usrSwtDispUpdReqForSecRowSeatVentnRi = j["usrSwtDispUpdReqForSecRowSeatVentnRi"]
    return v


def toJson_UsrSwtDispReqForSecRowSeatVentn(v):
    # type: (datatypes.UsrSwtDispReqForSecRowSeatVentn) -> str
    return json.dumps({
        "UsrSwtDispReqForSecRowSeatVentnLe" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispReqForSecRowSeatVentnLe)),

        "UsrSwtDispReqForSecRowSeatVentnRi" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispReqForSecRowSeatVentnRi)),

        "usrSwtDispUpdReqForSecRowSeatVentnLe" : _get_if_not_None(v.usrSwtDispUpdReqForSecRowSeatVentnLe),
        "usrSwtDispUpdReqForSecRowSeatVentnRi" : _get_if_not_None(v.usrSwtDispUpdReqForSecRowSeatVentnRi),

    })
def fromJson_UsrSwtDispReqVrnt(data):
    # type: (str) -> datatypes.UsrSwtDispReqVrnt
    j = json.loads(data)
    v = datatypes.UsrSwtDispReqVrnt()
    v.UsrSwtDispSeatHeatFct = j["UsrSwtDispSeatHeatFct"]
    v.UsrSwtDispTUnit = j["UsrSwtDispTUnit_"]
    return v


def toJson_UsrSwtDispReqVrnt(v):
    # type: (datatypes.UsrSwtDispReqVrnt) -> str
    return json.dumps({
        "UsrSwtDispSeatHeatFct" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispSeatHeatFct)),

        "UsrSwtDispTUnit_" : _extract_enum_value(_get_if_not_None(v.UsrSwtDispTUnit)),


    })
def fromJson_VFCGrp(data):
    # type: (str) -> datatypes.VFCGrp
    j = json.loads(data)
    v = datatypes.VFCGrp()
    v.Grp1 = j["Grp1"]
    v.Grp2 = j["Grp2"]
    v.Grp3 = j["Grp3"]
    return v


def toJson_VFCGrp(v):
    # type: (datatypes.VFCGrp) -> str
    return json.dumps({
        "Grp1" : _get_if_not_None(v.Grp1),
        "Grp2" : _get_if_not_None(v.Grp2),
        "Grp3" : _get_if_not_None(v.Grp3),

    })
def fromJson_VehCfgPrm(data):
    # type: (str) -> datatypes.VehCfgPrm
    j = json.loads(data)
    v = datatypes.VehCfgPrm()
    v.BlkIDBytePosn1 = j["BlkIDBytePosn1"]
    v.CCPBytePosn2 = j["CCPBytePosn2"]
    v.CCPBytePosn3 = j["CCPBytePosn3"]
    v.CCPBytePosn4 = j["CCPBytePosn4"]
    v.CCPBytePosn5 = j["CCPBytePosn5"]
    v.CCPBytePosn6 = j["CCPBytePosn6"]
    v.CCPBytePosn7 = j["CCPBytePosn7"]
    v.CCPBytePosn8 = j["CCPBytePosn8"]
    return v


def toJson_VehCfgPrm(v):
    # type: (datatypes.VehCfgPrm) -> str
    return json.dumps({
        "BlkIDBytePosn1" : _get_if_not_None(v.BlkIDBytePosn1),
        "CCPBytePosn2" : _get_if_not_None(v.CCPBytePosn2),
        "CCPBytePosn3" : _get_if_not_None(v.CCPBytePosn3),
        "CCPBytePosn4" : _get_if_not_None(v.CCPBytePosn4),
        "CCPBytePosn5" : _get_if_not_None(v.CCPBytePosn5),
        "CCPBytePosn6" : _get_if_not_None(v.CCPBytePosn6),
        "CCPBytePosn7" : _get_if_not_None(v.CCPBytePosn7),
        "CCPBytePosn8" : _get_if_not_None(v.CCPBytePosn8),

    })
def fromJson_VehMNomRec1(data):
    # type: (str) -> datatypes.VehMNomRec1
    j = json.loads(data)
    v = datatypes.VehMNomRec1()
    v.VehM = j["VehM"]
    v.VehMQly = j["VehMQly"]
    v.TrlrM = j["TrlrM_"]
    return v


def toJson_VehMNomRec1(v):
    # type: (datatypes.VehMNomRec1) -> str
    return json.dumps({
        "VehM" : _get_if_not_None(v.VehM),
        "VehMQly" : _extract_enum_value(_get_if_not_None(v.VehMQly)),

        "TrlrM_" : _extract_enum_value(_get_if_not_None(v.TrlrM)),


    })
def fromJson_VehModMngtGlbSafe1(data):
    # type: (str) -> datatypes.VehModMngtGlbSafe1
    j = json.loads(data)
    v = datatypes.VehModMngtGlbSafe1()
    v.UsgModSts = j["UsgModSts"]
    v.CarModSts1 = j["CarModSts1_"]
    v.CarModSubtypWdCarModSubtyp = j["CarModSubtypWdCarModSubtyp"]
    v.EgyLvlElecMai = j["EgyLvlElecMai"]
    v.EgyLvlElecSubtyp = j["EgyLvlElecSubtyp"]
    v.PwrLvlElecMai = j["PwrLvlElecMai"]
    v.PwrLvlElecSubtyp = j["PwrLvlElecSubtyp"]
    v.FltEgyCnsWdSts = j["FltEgyCnsWdSts"]
    v.Chks = j["Chks"]
    v.Cntr = j["Cntr"]
    return v


def toJson_VehModMngtGlbSafe1(v):
    # type: (datatypes.VehModMngtGlbSafe1) -> str
    return json.dumps({
        "UsgModSts" : _extract_enum_value(_get_if_not_None(v.UsgModSts)),

        "CarModSts1_" : _extract_enum_value(_get_if_not_None(v.CarModSts1)),

        "CarModSubtypWdCarModSubtyp" : _get_if_not_None(v.CarModSubtypWdCarModSubtyp),
        "EgyLvlElecMai" : _get_if_not_None(v.EgyLvlElecMai),
        "EgyLvlElecSubtyp" : _get_if_not_None(v.EgyLvlElecSubtyp),
        "PwrLvlElecMai" : _get_if_not_None(v.PwrLvlElecMai),
        "PwrLvlElecSubtyp" : _get_if_not_None(v.PwrLvlElecSubtyp),
        "FltEgyCnsWdSts" : _extract_enum_value(_get_if_not_None(v.FltEgyCnsWdSts)),

        "Chks" : _get_if_not_None(v.Chks),
        "Cntr" : _get_if_not_None(v.Cntr),

    })
def fromJson_VehMtnStSafe1(data):
    # type: (str) -> datatypes.VehMtnStSafe1
    j = json.loads(data)
    v = datatypes.VehMtnStSafe1()
    v.VehMtnSt = j["VehMtnSt"]
    v.VehMtnStChks = j["VehMtnStChks"]
    v.VehMtnStCntr = j["VehMtnStCntr"]
    return v


def toJson_VehMtnStSafe1(v):
    # type: (datatypes.VehMtnStSafe1) -> str
    return json.dumps({
        "VehMtnSt" : _extract_enum_value(_get_if_not_None(v.VehMtnSt)),

        "VehMtnStChks" : _get_if_not_None(v.VehMtnStChks),
        "VehMtnStCntr" : _get_if_not_None(v.VehMtnStCntr),

    })
def fromJson_VehSpdIndcd1(data):
    # type: (str) -> datatypes.VehSpdIndcd1
    j = json.loads(data)
    v = datatypes.VehSpdIndcd1()
    v.VehSpdIndcd = j["VehSpdIndcd"]
    v.VeSpdIndcdUnit = j["VeSpdIndcdUnit"]
    return v


def toJson_VehSpdIndcd1(v):
    # type: (datatypes.VehSpdIndcd1) -> str
    return json.dumps({
        "VehSpdIndcd" : _get_if_not_None(v.VehSpdIndcd),
        "VeSpdIndcdUnit" : _extract_enum_value(_get_if_not_None(v.VeSpdIndcdUnit)),


    })
def fromJson_VehSpdLgtSafe1(data):
    # type: (str) -> datatypes.VehSpdLgtSafe1
    j = json.loads(data)
    v = datatypes.VehSpdLgtSafe1()
    v.VehSpdLgt = j["VehSpdLgt"]
    v.VehSpdLgtQf = j["VehSpdLgtQf"]
    v.VehSpdLgtCntr = j["VehSpdLgtCntr"]
    v.VehSpdLgtChks = j["VehSpdLgtChks"]
    return v


def toJson_VehSpdLgtSafe1(v):
    # type: (datatypes.VehSpdLgtSafe1) -> str
    return json.dumps({
        "VehSpdLgt" : _get_if_not_None(v.VehSpdLgt),
        "VehSpdLgtQf" : _extract_enum_value(_get_if_not_None(v.VehSpdLgtQf)),

        "VehSpdLgtCntr" : _get_if_not_None(v.VehSpdLgtCntr),
        "VehSpdLgtChks" : _get_if_not_None(v.VehSpdLgtChks),

    })
def fromJson_Vin1(data):
    # type: (str) -> datatypes.Vin1
    j = json.loads(data)
    v = datatypes.Vin1()
    v.BlockNr = j["BlockNr"]
    v.VINSignalPos1 = j["VINSignalPos1"]
    v.VINSignalPos2 = j["VINSignalPos2"]
    v.VINSignalPos3 = j["VINSignalPos3"]
    v.VINSignalPos4 = j["VINSignalPos4"]
    v.VINSignalPos5 = j["VINSignalPos5"]
    v.VINSignalPos6 = j["VINSignalPos6"]
    v.VINSignalPos7 = j["VINSignalPos7"]
    return v


def toJson_Vin1(v):
    # type: (datatypes.Vin1) -> str
    return json.dumps({
        "BlockNr" : _get_if_not_None(v.BlockNr),
        "VINSignalPos1" : _get_if_not_None(v.VINSignalPos1),
        "VINSignalPos2" : _get_if_not_None(v.VINSignalPos2),
        "VINSignalPos3" : _get_if_not_None(v.VINSignalPos3),
        "VINSignalPos4" : _get_if_not_None(v.VINSignalPos4),
        "VINSignalPos5" : _get_if_not_None(v.VINSignalPos5),
        "VINSignalPos6" : _get_if_not_None(v.VINSignalPos6),
        "VINSignalPos7" : _get_if_not_None(v.VINSignalPos7),

    })
def fromJson_VisnAgWideCfg1(data):
    # type: (str) -> datatypes.VisnAgWideCfg1
    j = json.loads(data)
    v = datatypes.VisnAgWideCfg1()
    v.SteerPahRe = j["SteerPahRe"]
    v.SteerPahFrnt = j["SteerPahFrnt"]
    v.SteerPahSide = j["SteerPahSide"]
    v.DstInfoRe = j["DstInfoRe"]
    v.DstInfoFrnt = j["DstInfoFrnt"]
    v.DstInfoSide = j["DstInfoSide"]
    v.CtraInfo = j["CtraInfo"]
    v.TwbrPah = j["TwbrPah"]
    v.ObjDetn = j["ObjDetn"]
    return v


def toJson_VisnAgWideCfg1(v):
    # type: (datatypes.VisnAgWideCfg1) -> str
    return json.dumps({
        "SteerPahRe" : _extract_enum_value(_get_if_not_None(v.SteerPahRe)),

        "SteerPahFrnt" : _extract_enum_value(_get_if_not_None(v.SteerPahFrnt)),

        "SteerPahSide" : _extract_enum_value(_get_if_not_None(v.SteerPahSide)),

        "DstInfoRe" : _extract_enum_value(_get_if_not_None(v.DstInfoRe)),

        "DstInfoFrnt" : _extract_enum_value(_get_if_not_None(v.DstInfoFrnt)),

        "DstInfoSide" : _extract_enum_value(_get_if_not_None(v.DstInfoSide)),

        "CtraInfo" : _extract_enum_value(_get_if_not_None(v.CtraInfo)),

        "TwbrPah" : _extract_enum_value(_get_if_not_None(v.TwbrPah)),

        "ObjDetn" : _extract_enum_value(_get_if_not_None(v.ObjDetn)),


    })
def fromJson_WarnAndIntvPen1(data):
    # type: (str) -> datatypes.WarnAndIntvPen1
    j = json.loads(data)
    v = datatypes.WarnAndIntvPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_WarnAndIntvPen1(v):
    # type: (datatypes.WarnAndIntvPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_WarnTypForLaneChgWarnPen1(data):
    # type: (str) -> datatypes.WarnTypForLaneChgWarnPen1
    j = json.loads(data)
    v = datatypes.WarnTypForLaneChgWarnPen1()
    v.Sts = j["Sts"]
    v.Pen = j["Pen"]
    return v


def toJson_WarnTypForLaneChgWarnPen1(v):
    # type: (datatypes.WarnTypForLaneChgWarnPen1) -> str
    return json.dumps({
        "Sts" : _extract_enum_value(_get_if_not_None(v.Sts)),

        "Pen" : _extract_enum_value(_get_if_not_None(v.Pen)),


    })
def fromJson_WhlRotToothCntrRec(data):
    # type: (str) -> datatypes.WhlRotToothCntrRec
    j = json.loads(data)
    v = datatypes.WhlRotToothCntrRec()
    v.WhlRotToothCntrFrntLe = j["WhlRotToothCntrFrntLe"]
    v.WhlRotToothCntrFrntRi = j["WhlRotToothCntrFrntRi"]
    v.WhlRotToothCntrReLe = j["WhlRotToothCntrReLe"]
    v.WhlRotToothCntrReRi = j["WhlRotToothCntrReRi"]
    return v


def toJson_WhlRotToothCntrRec(v):
    # type: (datatypes.WhlRotToothCntrRec) -> str
    return json.dumps({
        "WhlRotToothCntrFrntLe" : _get_if_not_None(v.WhlRotToothCntrFrntLe),
        "WhlRotToothCntrFrntRi" : _get_if_not_None(v.WhlRotToothCntrFrntRi),
        "WhlRotToothCntrReLe" : _get_if_not_None(v.WhlRotToothCntrReLe),
        "WhlRotToothCntrReRi" : _get_if_not_None(v.WhlRotToothCntrReRi),

    })
def fromJson_WipgInfo(data):
    # type: (str) -> datatypes.WipgInfo
    j = json.loads(data)
    v = datatypes.WipgInfo()
    v.WipgSpdInfo = j["WipgSpdInfo_"]
    v.WiprActv = j["WiprActv"]
    v.WiprInWipgAr = j["WiprInWipgAr"]
    return v


def toJson_WipgInfo(v):
    # type: (datatypes.WipgInfo) -> str
    return json.dumps({
        "WipgSpdInfo_" : _extract_enum_value(_get_if_not_None(v.WipgSpdInfo)),

        "WiprActv" : _extract_enum_value(_get_if_not_None(v.WiprActv)),

        "WiprInWipgAr" : _extract_enum_value(_get_if_not_None(v.WiprInWipgAr)),


    })



type_map = {
    "ADataRawSafe" : "ADataRawSafe1",
    "AccAdprSpdLimActv" : "OnOffPen",
    "AccAdprTurnSpdActv" : "OnOffPen",
    "AccAutResuWarnReq" : "OnOff1",
    "AccSts" : "AccSts1",
    "ActrPosnFrnReqForOsaAndRecirc" : "VALUETYPE:d1d4fec3b3ad41d3815f35ededfab8f7",
    "ActvNoiseCtrlReq" : "NormSptPen",
    "ActvOfHorn" : "OnOff1",
    "ActvnOfCrsEco" : "ActvnOfCrsEco",
    "ActvnOfPrkgAssi" : "OnOff1",
    "ActvnOfPrkgAut" : "ActvnOfPrkgAut",
    "ActvnOfSwtIllmnCen" : "bool",
    "ActvnOfSwtIllmnClima" : "bool",
    "ActvnOfSwtIllmnDrvMod" : "bool",
    "ActvnOfSwtIllmnForSeatHeatrRe" : "bool",
    "ActvtAutnmsPrkgCtrl" : "ActvtAutnmsPrkgCtrl",
    "AdjSpdLimnSts" : "AdjSpdLimnSts2",
    "AdjSpdLimnWarn" : "AdjSpdLimnWarnCoding",
    "AdprTurnSpdMod" : "AdprTurnSpdModPen1",
    "AgDataRawSafe" : "AgDataRawSafe1",
    "LiExtReq1WdReq5" : "SlowNormFastPen",
    "AirClngReq" : "OnOffNoReq",
    "AmbTIndcdWithUnit" : "AmbTIndcdWithUnit",
    "AmbTRaw" : "AmbTWithQly",
    "AntithftRednReq" : "IDENTICALTYPE:179c10b795774c6883e77a3296000cd1",
    "AsyFctDeactvnWarn" : "IDENTICALTYPE:19eb5dd48e1345e18043109a1bc11908",
    "AsyLaneChg" : "AsyLaneChg1",
    "AsySteerApplyRqrd" : "LvlWarn1",
    "AsySteerFctDeactvdWarn" : "OnOff1",
    "AudMsgReq" : "IDENTICALTYPE:86f272a2b4bd43a7ae2bf42a7f55bcb8",
    "AudWarnActv" : "NoYesCrit1",
    "AutnmsPrkgActvScenario" : "AutnmsPrkgActvScenario",
    "AutnmsPrkgActvn" : "OnOff1",
    "AutnmsPrkgAvlScenario" : "AutnmsPrkgAvlScenario",
    "AutnmsPrkgEnaResu" : "EnaResu",
    "AutnmsPrkgProgs" : "IDENTICALTYPE:7b525a9c4b86498784492aedffd1b562",
    "AutnmsPrkgSeldScenario" : "AutnmsPrkgSeldScenario",
    "AvlStsForLatAutDrv" : "AvlStsForLatAutDrv5",
    "BackCntrForMissCom" : "IDENTICALTYPE:d9b323f142794c459c3a4a0b340421b3",
    "BlisSwOnOff" : "OnOff1",
    "BltLockStAtDrvr" : "BltLockStFrnt",
    "BltLockStAtPass" : "BltLockStFrnt",
    "BltLockStAtRowSecLe" : "BltLockStRe",
    "BltLockStAtRowSecMid" : "BltLockStRe",
    "BltLockStAtRowSecRi" : "BltLockStRe",
    "BltLockStAtRowThrdLe" : "BltLockStRe",
    "BltLockStAtRowThrdRi" : "BltLockStRe",
    "BltRmnSound1" : "Trig1",
    "BltRmnSound2" : "Trig1",
    "BltRmnSound3" : "Trig1",
    "BltRmnSound4" : "Trig1",
    "BrkAndAbsWarnIndcnReq" : "BrkAndAbsWarnIndcnReqRec1",
    "BrkFldLvl" : "FldLvl",
    "BrkFricTqAtWhlAct" : "BrkFricTqAtWhlActRec",
    "BrkRelsWarnReq" : "NoYes1",
    "Btn1ForUsrSwtPanFrntReq" : "PsdNotPsd",
    "Btn2ForUsrSwtPanFrntReq" : "PsdNotPsd",
    "Btn3ForUsrSwtPanFrntReq" : "PsdNotPsd",
    "Btn4ForUsrSwtPanFrntReq" : "PsdNotPsd",
    "Btn5ForUsrSwtPanFrntReq" : "PsdNotPsd",
    "BtnAudFbSts" : "PsdNotPsd",
    "BtnAudVolSts" : "BtnAudVolSts",
    "BtnIllmnForWinDefrstFrnt" : "OnOff1",
    "BtnMmedLeRiSts" : "BtnMmedLeRiSts",
    "BtnMmedModSts" : "PsdNotPsd",
    "BtnMmedSetSts" : "BtnMmedSetSts",
    "CCSMBtn6" : "PsdNotPsd",
    "CallSts" : "CallSts",
    "CarTiGlb" : "VALUETYPE:ff1ba0f507b2484489037b6a1cbd866c",
    "CchForFuEco" : "CchForFuEco1",
    "ChdLockgProtnStsToHmi" : "OnOffSafe1",
    "ChdWinProtnStsAtDrvrRe" : "OnOff1",
    "ChdWinProtnStsAtPassRe" : "OnOff1",
    "ChrgrHwAprvdWirelsAdr" : "HwAprvdWirelsAdr1",
    "ChrgrWirelsSts" : "ChrgrWirelsSts",
    "ClimaActv" : "OnOff1",
    "ClimaPwrCns" : "VALUETYPE:8db9cbd5df624dabba459322159c65eb",
    "ClimaRqrdFromHmi" : "OnOffNoReq",
    "ClimaTmr" : "VALUETYPE:e26bd5a4ac0441bdb420ca3955cdab72",
    "ClimaTmrSts" : "OnOff1",
    "ClimaTmrStsRqrd" : "OnOff1",
    "ClimaWarn" : "ClimaWarn",
    "CllsnAidSnvtySeld" : "SnvtyPen1",
    "CllsnFwdWarnActv" : "OnOffPen",
    "CllsnFwdWarnReq" : "OnOff1",
    "CllsnWarnReq" : "OnOff1",
    "CllsnWarnSide" : "CllsnWarnSide1",
    "ClngActv" : "OnOffNoReq",
    "ClngRqrdFromHmi" : "OnOffNoReq",
    "ClsAutEna" : "EnableDisableCoding",
    "CmftFctActv" : "OnOff1",
    "CmftFctSts" : "FctSts2",
    "CmptmtAirTEstimdExtd" : "CmptmtAirTEstimdExtd",
    "CmptmtTFrnt" : "CmptmtTFrnt",
    "CnclWarnForCrsCtrl" : "NoYesCrit1",
    "CnclWarnLgtForAutDrv" : "NoYesCrit1",
    "CnsPrm" : "CnsPrmRec1",
    "ComLostExtrSound" : "bool",
    "ConSftyDataSharingAllwd" : "OnOff1",
    "ConSftyDataSharingSts" : "OnOff1",
    "ConSftyWarn" : "ConSftyWarn1",
    "CoolgReqForDispCen" : "CoolgReqForDisp",
    "CoolgStsForDisp" : "CoolgStsForDisp",
    "CrsCtrlrSts" : "CrsCtrlrSts1",
    "CrvtSpdAdpvSts" : "OffOnNotAvlSrvRqrd",
    "CrvtSpdEstimrSnvtySeld" : "SnvtyPen3",
    "CrvtSpdWarnActv" : "OnOffPen",
    "CrvtSpdWarnReq" : "TurnSpdWarnReq1",
    "CrvtSpdWarnSts" : "FctSts2",
    "CtraIndcnLe" : "CtraIndcn1",
    "CtraIndcnRi" : "CtraIndcn1",
    "CtraOn1" : "LcmaCtraOn",
    "CtraSwOnOff" : "OnOff1",
    "CurtActvnReReq" : "CurtActvnTyp1",
    "DataDistbnAllwdForEgyPred" : "OnOffNoReq",
    "DataDistbnReqForEgyPred" : "OnOffNoReq",
    "DataSpclDMSM" : "DataSpclTyp",
    "DayToSrv" : "IDENTICALTYPE:bba91bed6bf3472ebc49558f607eb5f3",
    "DeactvtLvlgCtrl" : "bool",
    "DefrstDurgClimaEnad" : "OnOff1",
    "DestSrcAndCfmRqrd" : "IDENTICALTYPE:3ada3504745f4e2a931f1f34792ce852",
    "DiagcCCSM" : "DiagcForPanCenCtrl2",
    "DiagcRCSM" : "DiagcForRCSM2",
    "DiagcRSHC" : "DiagcForRCSM2",
    "DiagcStsDMSM" : "DiagcStsTypDMSM",
    "DispAndHomeBtnSts" : "Trig1",
    "DispBattEgyIn" : "VALUETYPE:dd585741cdcd4447a665ea6a8d76ef2b",
    "DispBattEgyOut" : "VALUETYPE:bbd2720f121e4d05a34502dc5bfaec54",
    "DispOfPrpsnMod" : "DispOfPrpsnMod3",
    "DispStsCen" : "FltCfmd1",
    "DoorDrvrLockReSts" : "LockSts2",
    "DoorDrvrLockSts" : "LockSts2",
    "DoorDrvrReSts" : "DoorSts2",
    "DoorDrvrSts" : "DoorSts2",
    "DoorPassLockReSts" : "LockSts2",
    "DoorPassLockSts" : "LockSts2",
    "DoorPassReSts" : "DoorSts2",
    "DoorPassSts" : "DoorSts2",
    "DriveAwayInfoActvReq" : "OnOff1",
    "DriveAwayInfoActvSts" : "FctSts2",
    "DriveAwayInfoSoundWarnActvReq" : "OnOff1",
    "DriveAwayInfoWarnReq" : "DriveAwayInfoWarnReq1",
    "DrvModDispd" : "bool",
    "DrvModReq" : "DrvModReqType1",
    "DrvgInWrgDirOfTrfc" : "NoYes1",
    "DrvrCtrlOfPassSeatFrntReqd" : "OnOff1",
    "DrvrCtrlOfPassSeatFrntSts" : "ActvnAvl1",
    "DrvrDesDir" : "DrvrDesDir1",
    "DrvrDispSetg" : "DrvrDispSetgReq",
    "DrvrEntryLoReq" : "NoYes1",
    "DrvrHmiBackGndForHmiCen" : "DrvrHmiForHmiCen",
    "DrvrHmiBackGndInfoSetg" : "DrvrHmiBackGndInfoSetg",
    "DrvrHmiDispdMod" : "DrvrHmiDispdModPen",
    "DrvrHmiForHmiCen" : "DrvrHmiForHmiCen",
    "DrvrHmiSpdLimAdpnSts" : "OnOff1",
    "DrvrHmiTmrForHmiCen" : "Array8ByteU8",
    "DrvrHmiUsrIfSetg" : "DrvrHmiUserIfSetgReq",
    "DrvrMassgRunng" : "OnOff1",
    "DrvrPfmncLvl" : "DrvrPfmncLvl2",
    "DrvrPfmncMonActv" : "OnOffPen",
    "DrvrPfmncSts" : "FctSts2",
    "DrvrPfmncWarnReq" : "DrvrPfmncWarnReq1",
    "DrvrPfmncWarnReqToNav" : "DrvrPfmncWarnReq1",
    "DrvrSeatActvSpplFct" : "SeatActvSpplFct1",
    "DrvrSeatDispMassgFct" : "SeatMassgFct",
    "DrvrSeatDispSpplFct" : "SeatActvSpplFct1",
    "DrvrSeatMassgFct" : "SeatMassgFct",
    "DrvrSeatSwtSts" : "DrvrSeatSwtSts1",
    "DrvrWLoadLvl" : "Lvl30",
    "DstEstimdToEmptyForDrvgElec" : "IDENTICALTYPE:c4f1fb9ab76f433eaca4c949fd2c0acd",
    "DstEstimdToEmptyForDrvgElecEco" : "IDENTICALTYPE:ec9fd82571c641948b3e0d19618fcc93",
    "DstNotifActv" : "OnOffPen",
    "DstNotifSts" : "FctSts2",
    "DstToEmptyIndcd" : "DstToEmptyWithUnit",
    "DstToManvLocn" : "DstToManvLocnByNav",
    "DstToSrv" : "IDENTICALTYPE:3fc436e8289742b594a3857697fb71d6",
    "DstTrvld1" : "VALUETYPE:6a8a37ffc88c4443b63e99887a1993b7",
    "DstTrvld2" : "VALUETYPE:159f2c7105ff41909598137d24876fb5",
    "DstTrvldHiResl" : "IDENTICALTYPE:dc8c62ef3cfe4910b114be0c0d047fc7",
    "BkpOfDstTrvld" : "VALUETYPE:d75f995b60ae4f819a5f47defe69672a",
    "DstTrvldMst2" : "IDENTICALTYPE:bc94d2feea4a43fbb3da6105b2e9d039",
    "EasyInOutDrvrSeatAdjmt" : "OnOffPen",
    "EgyCostForRoute" : "EgyCostForRouteRec1",
    "EgySave" : "OnOff1",
    "EmgyAsscSts" : "AsscSts",
    "EmgyAsscStsConnGen2" : "AsscSts",
    "EmgyVehWarnActv" : "OnOff1",
    "EmgyVehWarnSts" : "FctSts2",
    "EngCooltIndcnReq" : "EngCooltIndcn",
    "EngCooltLvl" : "FldLvl",
    "EngFuCns" : "VALUETYPE:1cbb7384c17b45c484c8feb2623c3e43",
    "EngFuCnsFild" : "VALUETYPE:1603bc52a7fc457db1fe1bcee41d410a",
    "EngHrToSrv" : "IDENTICALTYPE:c1f6da4979cc439c87279e5642671ed5",
    "EngIdleEco1" : "EngIdleEco1",
    "EngNSafe" : "EngNSafe",
    "EngOilLvl" : "VALUETYPE:6035220518fe4fde8584f16b67bba75c",
    "EngOilLvlSts" : "EngOilLvlSts1",
    "EngOilPWarn" : "EngOilPWarn1",
    "EngSpdDispd" : "VALUETYPE:628cde156d4d475692697cf146f4d0a6",
    "EngStrtStopDrvModIndSetg" : "OnOffPen",
    "EngStrtStopSetg" : "OnOff2",
    "EpbApplyAutSetg" : "OnOffPen",
    "EpbEngStallApplySetg" : "OnOffPen",
    "EpbLampReq" : "EpbLampReqRec",
    "EscSptModReqdByDrvr" : "EscSptModReqdByDrvrRec1",
    "EscSt" : "EscStSafe1",
    "EscWarnIndcnReq" : "EscWarnIndcnReqRec1",
    "ExtrMirrFoldSetg" : "OnOffPen",
    "ExtrMirrTiltSetg2" : "ExtrMirrTiltSetg",
    "ExtrMirrTintgSetg" : "MirrDimPen",
    "FRNetworkStatus" : "IDENTICALTYPE:dd4941f521f7452fae4aba4fba2d538e",
    "FltIndcrTurnLeFrnt" : "DevErrSts2",
    "FltIndcrTurnLeRe" : "DevErrSts2",
    "FltIndcrTurnRiFrnt" : "DevErrSts2",
    "FltIndcrTurnRiRe" : "DevErrSts2",
    "FltOfLiDaytiRunngRi" : "DevErrSts2",
    "FrntAxleWarn" : "AxleWarn",
    "FrntWiprLvrReq2" : "FrntWiprLvrReq2",
    "FuHeatrActv" : "OnOff1",
    "FuHeatrFuCns1" : "VALUETYPE:43d2cd015cb14790839d097cd766085b",
    "FuHeatrFuCnsDurgCyc1" : "VALUETYPE:c8e46b30c4244013bd0d6189ce398fd0",
    "FuLvlIndcd" : "FuLvlValWithQly",
    "FuLvlLoIndcn" : "Trig1",
    "FuLvlLoIndcnToNav" : "FuLvlLoIndcnToNav1",
    "FuLvlLoWarn" : "Trig1",
    "GearIndcnRec" : "GearIndcnRec2",
    "GlbRstForSetgAndData" : "GlbRstForSetgAndData",
    "HdHwAprvdWirelsAdr" : "HwAprvdWirelsAdr1",
    "HdrestFoldReq2" : "bool",
    "HeatrDurgDrvgReqd" : "OffOnAut1",
    "HeatrPreCdngTyp" : "HeatrPreCdngTyp",
    "HmiAudSts" : "FltCfmd1",
    "HmiCenForDrvrHmi" : "HmiCenForDrvrHmi",
    "HmiCmptmtAirDistbnFrnt" : "HmiCmptmtAirDistbnFrnt",
    "HmiCmptmtCoolgReq" : "HmiCmptmtCoolgReq",
    "HmiCmptmtTSp" : "HmiCmptmtTSp",
    "HmiDefrstElecReq" : "HmiDefrstElecReq",
    "HmiDefrstElecSts" : "HmiDefrstElecSts",
    "HmiDefrstMaxReq" : "ActrReq",
    "HmiHvacFanLvlFrnt" : "HmiHvacFanLvl",
    "HmiHvacFanLvlRe" : "HmiHvacFanLvl",
    "HmiHvacReCtrl" : "HmiHvacReCtrl",
    "HmiHvacRecircCmd" : "HmiHvacRecircCmd",
    "HmiSeatClima" : "HmiSeatClima",
    "HmiSeatClimaExtd" : "HmiSeatClimaExtd",
    "HoodSts" : "DoorSts2",
    "HptcWarnSeldForCllsnFwdWarn" : "OnOffPen",
    "HudActvReq" : "OnOffPen",
    "HudActvSts" : "OnOff1",
    "HudAdjmtReq" : "OnOff1",
    "HudDiagc" : "HudDiagc",
    "HudErgoSetgReq" : "OnOff1",
    "HudSts" : "HudStsForHmi",
    "HudVisFctSetg" : "HudVisFctSetgReq",
    "HvBattEgyAvlDcha3" : "VALUETYPE:64b9ca3a37844f09afa6f44c623881e7",
    "HvBattSmtActv" : "Flg1",
    "HvBattSmtSeld" : "OnOffNoReq",
    "HvacAirMFlowEstimd" : "VALUETYPE:c279aa6f37b94c77af3e872412da58f1",
    "HznData" : "HznDataGroup2",
    "HznEdge" : "HznEdgeGroup2",
    "HznPosn" : "HznPosnGroup3",
    "HznPosnExtd" : "HznPosnExtdGroup1",
    "HznPosnExtdOffs" : "HznPosnExtdOffs",
    "HznProfLong" : "HznProfLongGroup3",
    "HznProfLongExtd" : "HznProfLongExtdGroup1",
    "HznProfSho" : "HznProfSho2",
    "HznRstExtd" : "bool",
    "HznSeg" : "HznSegGroup2",
    "HznSplyElectcSts" : "HznSplyElectcSts1",
    "HzrdLiWarnActv" : "OnOff1",
    "HzrdLiWarnSts" : "FctSts2",
    "IndcnOfParkAssiSts" : "OnOff1",
    "IndcnOfPrkgAutSts" : "IndOfPrkgAutSts",
    "IndcnUnit" : "IndcnUnit",
    "IndcrDisp1WdSts" : "IndcrSts1",
    "IndcrTurnSts1WdSts" : "IndcrSts1",
    "IniValSigCfgIDBackboneFR" : "IDENTICALTYPE:d71414fa56404d5c9a68887c9c1c084a",
    "InsdCarNoiseMeasd" : "VALUETYPE:f1abe1b14dc04478ba8dc30d0b476999",
    "IntAudCnclWarn" : "YesNo2",
    "IntrBriSts" : "IDENTICALTYPE:7079f70cd4214d43ad19ab8de8b87f2c",
    "IntrBriStsForSeatHeatrRe" : "IDENTICALTYPE:31b04edbb66c4fd58931c9c4d73f0327",
    "IntrLiAmbLiSetg" : "IntrLiAmbLiSetg",
    "IntrLiSurrndgsLiSetgLi" : "IntrLiSurrndgsLiSetg",
    "IntrMirrTintgSetg" : "MirrDimPen",
    "IntvAndWarnModForLaneKeepAid" : "WarnAndIntvPen1",
    "KeyLostWarnIndcn" : "KeyLostWarnIndcn",
    "KeyProfMpgUpd" : "KeyProfMpgUpd1",
    "KeyReadReqFromSetgMgr" : "KeyLocn1",
    "KeyReadStsToProfCtrl" : "KeyReadStsToProfCtrl",
    "KeyRmnIndcn" : "OnOff1",
    "KeySpdWarn" : "Trig1",
    "LampSuppSrv" : "SrvSts",
    "LaneChgWarnActv" : "OnOffPen",
    "LaneChgWarnSts" : "FctSts2",
    "LaneDetnStsForAutDrv" : "LaneDetnSts1",
    "LaneDetnStsForLaneKeepAid" : "LaneDetnSts1",
    "LaneKeepAidActv" : "OnOffPen",
    "LaneKeepAidRoadEdgeActv" : "OnOffPen",
    "LaneKeepAidSts" : "FctSts2",
    "LatMovmtWarn" : "WarnLeRi1",
    "LcmaOn1" : "LcmaCtraOn",
    "LiExtReq1WdReq1" : "OnOffPen",
    "LiExtReq2WdReq1" : "LiAutTranPen1",
    "LiExtReq1WdReq4" : "OnOffPen",
    "LiExtReq1WdReq6" : "OnOffPen",
    "LiExtReq1WdReq2" : "OnOffPen",
    "LiDrvrFltIndcrTurn" : "DevErrSts2",
    "LiForBtn4ForUsrSwtPanFrntCmd" : "OnOff1",
    "LiForBtn5ForUsrSwtPanFrntCmd" : "OnOff1",
    "LiExtSafe1WdReq2" : "OnOffPen",
    "LiExtSafe1WdReq1" : "LiTiPen2",
    "LiLvrSwt1Req" : "SwtPush",
    "LiPassFltIndcrTurn" : "DevErrSts2",
    "LiSeldForDrvrPfmncMon" : "OnOffPen",
    "LiTrfcSide1WdReq1" : "LiTrfcSide1",
    "ListOfNodAv" : "ListOfNodAv",
    "LockSpdReq" : "OnOffPen",
    "LockgCenSts" : "LockgCenSts3",
    "LockgCenStsForUsrFb" : "LockSt2",
    "LockgFbSoundReq" : "OnOffPen",
    "LockgFbVisReq" : "OnOffPen",
    "LockgPrsnlReqFromHmi" : "OnOffNoReq",
    "LockgPrsnlSts" : "OnOff1",
    "LvlOfClimaCmft" : "LvlOfClimaCmft",
    "LvlSeldForSpdLimAdpv" : "LvlSeldForSpdLimAdpvPen1",
    "MassgFctActv" : "MassgFctActv",
    "MemBtnSound" : "bool",
    "MirrDwnStsAtDrvr" : "MirrDwnStsTyp",
    "MirrDwnStsAtPass" : "MirrDwnStsTyp",
    "MirrFoldStsAtDrvr" : "MirrFoldStsTyp",
    "MirrFoldStsAtPass" : "MirrFoldStsTyp",
    "MmedHdPwrMod" : "MmedMaiPwrMod",
    "MmedHmiModStd" : "MmedHmiModStd2",
    "MmedTvmPwerMod" : "MmedTvmPwerMod",
    "MstCfgIDBackboneFR" : "IDENTICALTYPE:91d70d4e79214c1f961f456b506122f9",
    "MtrlSnsrT" : "MtrlSnsrT",
    "NFSDataFront" : "SnsrPrkgAssi3",
    "NFSDataRear" : "SnsrPrkgAssi3",
    "NetCtrlrActvt" : "NetActvtRec1",
    "NetHdActvt" : "NetActvtRec1",
    "NetTelmActvt" : "NetActvtRec1",
    "NewTripCdn" : "Trig1",
    "NoEntryWarnReq" : "Warn2",
    "NoSoundSys" : "bool",
    "NotifChkDistbn" : "IDENTICALTYPE:c9e5f82e68054219a96c94d6bf6ae5de",
    "NrOfBltAppld" : "IDENTICALTYPE:2aa9e84ba5d74c6f904c52af25ea5fcf",
    "NrOfKeyAvl" : "bool",
    "NrSerlDMSM" : "NrSerlNodLIN",
    "OffsForDrvrSpprtFctActvSts" : "OnOff1",
    "OffsForSpdWarnSetg" : "OffsForSpdWarnSetgPen",
    "PartNrDMSM" : "PartNrNodLIN",
    "PasAlrmDeactvnReq" : "OnOff1",
    "PasAlrmSts" : "NoReqOffReq",
    "PassMassgRunng" : "OnOff1",
    "PassSeatActvSpplFct" : "SeatActvSpplFct1",
    "PassSeatDispMassgFct" : "SeatMassgFct",
    "PassSeatDispSpplFct" : "SeatActvSpplFct1",
    "PassSeatMassgFct" : "SeatMassgFct",
    "PassSeatSts" : "PassSeatSts1",
    "PassSeatSwtSts2" : "PassSeatSwtSts2",
    "PinionSteerAg1" : "PinionSteerAg1Rec",
    "PosnFromNav" : "PosnFromNav",
    "PosnFromSatlt" : "PosnFromSatlt",
    "PostDrvgClimaAvl" : "OnOff1",
    "PostDrvgClimaReq" : "OnOff1",
    "PreClngNotif" : "PreClngNotif",
    "PrkgAssiActvSts" : "OnOff1",
    "PrkgAssiAudWarnFront" : "PrkgAssiAudWarn",
    "PrkgAssiAudWarnRear" : "PrkgAssiAudWarn",
    "PrkgAssiFailr" : "PrkgAssiFailr2",
    "PrkgAssiManvActvSts" : "OnOff1",
    "PrkgAssiManvProgs" : "IDENTICALTYPE:8bef6df351674b33bdd6f6ef0b42e01e",
    "PrkgAssiSts" : "PrkgAssiSts3",
    "PrkgAutSts" : "PrkgAutSts3",
    "PrkgCamSysAvlSts" : "FctSts2",
    "PrkgOutCfm" : "OnOff1",
    "PrkgTypVld" : "PrkgTypVld1",
    "ProfAct" : "ProfAct1",
    "ProfChg" : "IdPen",
    "ProfLimd" : "ProfLimd1",
    "ProfPenSts1" : "IdPen",
    "PrpsnDrvMod" : "PrpsnDrvMod",
    "PrpsnHvBattUsgModAct" : "HvBattUsgType",
    "PrpsnHvBattUsgModReq" : "HvBattUsgType",
    "PrpsnHvBattUsgOfChrgBlkd" : "bool",
    "PrpsnHvBattUsgOfChrgBlkd2" : "HvBattUsgBlkdTyp",
    "PrpsnHvBattUsgOfHldBlkd" : "bool",
    "PrpsnHvBattUsgOfHldBlkd2" : "HvBattUsgBlkdTyp",
    "PrpsnHvBattUsgOfHldSmtBlkd" : "bool",
    "PrpsnHvBattUsgOfHldSmtBlkd2" : "HvBattUsgBlkdTyp",
    "PrpsnHvBattUsgOfHldSpd" : "IDENTICALTYPE:60b9242cd9764868877b178dda061ad3",
    "PrpsnHvBattUsgStsDispd" : "bool",
    "PrpsnModElecDrvBlkd" : "Typ1",
    "PrpsnModOfSaveBlkd" : "Typ1",
    "PrpsnModOfTracBlkd" : "Typ1",
    "PrpsnModOffroadBlkd" : "Typ1",
    "PrpsnModSptBlkd" : "Typ1",
    "PtCluTq" : "PtCluTq1",
    "PtDrvrSetg" : "PtDrvrSetg2",
    "PtGearTar" : "PtGearAct1",
    "PtTqAtWhlFrntAct" : "PtTqAtWhlFrntActRec1",
    "PwrChrgDetdForPrkgHeatrElec" : "bool",
    "PwrSplyErrSts" : "PwrSplyErrSts1",
    "RadioFrqAn" : "IDENTICALTYPE:60764ea9819845b39c84631574041220",
    "RainSenMemdReq" : "OnOffPen",
    "ReAxleWarn" : "AxleWarn",
    "RlyPwrDistbnCmd1WdBattSaveCmd" : "OnOff1",
    "RlyPwrDistbnCmd1WdIgnRlyCmd" : "OnOff1",
    "RlyPwrDistbnCmd1WdIgnRlyExtCmd" : "OnOff1",
    "RlyPwrDistbnCmd1WdPreBattSaveCmd" : "OnOff1",
    "RmnLockgPrsnlReq" : "OnOff1",
    "RngbdIllmnCmd" : "RngBdIllmnCmdPen1",
    "RoadFricIndcnActv" : "OnOffPen",
    "RoadFricIndcnSts" : "FctSts2",
    "RoadFricWarnReq" : "RoadFricWarnReq1",
    "RoadSgnInfoActv" : "OnOffPen",
    "RoadSgnInfoSts" : "FctSts2",
    "RoadSpdLimActvSts" : "OnOff1",
    "RoadUsrProtnActv" : "OnOff1",
    "RoadUsrProtnSts" : "FctSts2",
    "RotyDirReq2" : "RotyDirUI",
    "RotyDirReq1" : "RotyDirUI1",
    "RotyPosReq2" : "IDENTICALTYPE:743d2d739c18498b80628a21b1ebc086",
    "RotyPosReq1" : "IDENTICALTYPE:3d63e441567a47c4b74cfc2785ad2d4d",
    "RouteInfo" : "RouteInfoRec1",
    "RsdsSysStsLe" : "RsdsSysSts",
    "RsdsSysStsRi" : "RsdsSysSts",
    "SaveSetgToMemPrmnt" : "OffOnAut1",
    "ScrBarVolIndcn" : "VALUETYPE:c78f14c398d040858a859fc392051f3b",
    "ScrMaxFillgVol" : "IDENTICALTYPE:a9422a1c180c43058e8ef87cf5a05fc0",
    "ScrReagentTankVol" : "IDENTICALTYPE:189160e8b86f42fe94514ba657ebc06d",
    "SeatBackUnlckdThrd" : "SeatBackUnlckd",
    "SeatDispBtnPsd" : "SeatDispBtnPsd",
    "SeatHeatDurgClimaEnad" : "SeatHeatDurgClimaEnad",
    "SeatHeatgAutCdn" : "Flg1",
    "SeatSwtLeSigThrd" : "SwtVertSts1",
    "SeatSwtRiSigThrd" : "SwtVertSts1",
    "SeatVentnAutCdn" : "Flg1",
    "SetOfLang" : "SetOfLang",
    "SetOfLangAck" : "FailrNoFailr1",
    "SetTiAndDate" : "DateTi1ForSet",
    "SetgAndRstOfTripForDrvr" : "SetgAndRstOfTripForDrvr",
    "SftyCchAccSts" : "SftyCchActvnSts2",
    "SftyCchDrvgCycIdx" : "SftyCchIdx2",
    "SftyCchLaneKeepAidSts" : "SftyCchActvnSts2",
    "SftyCchLongTermIdx" : "SftyCchIdx1",
    "SftyCchLongTermIdxRst" : "NoYes1",
    "SftyCchMidTermIdx" : "SftyCchIdx1",
    "SftyCchPilotAssiSts" : "SftyCchActvnSts2",
    "SftyCchPostTripSts" : "SftyCchPostTripSts1",
    "SftyCchPrimFbAftDrvg" : "SftyCchFb1",
    "SftyCchPrimFbDurgDrvg" : "SftyCchFb1",
    "SftyCchSecFbAftDrvg" : "SftyCchFb1",
    "SftyCchSecFbDurgDrvg" : "SftyCchFb1",
    "SftyCchShortTermIdx" : "SftyCchIdx1",
    "SftyCchSts" : "FctSts2",
    "SftySigGroupFromAudSafe" : "SftySigGroupFromAudSafe1",
    "SftyWarnGroupFromAsySafe" : "SftyWarnGroupFromAsySafe1",
    "SnsrParkAssiFrnt" : "SnsrParkAssi1",
    "SnsrParkAssiRe" : "SnsrParkAssi1",
    "SnsrPrkgAssiFrnt" : "SnsrPrkgAssi2",
    "SnsrPrkgAssiRe" : "SnsrPrkgAssi2",
    "SoundExtActvSts" : "OnOff1",
    "WhlMotSysSpdAct" : "VALUETYPE:0fc38e2746c24f5fa75d6bad497f937b",
    "SpdAlrmActvForRoadSgnInfo" : "OnOffPen",
    "SpdCameraInfoSeldForRoadSgnInfo" : "OnOffPen",
    "SpdLimAdpvSts" : "OffOnNotAvlSrvRqrd",
    "SpdLimWarnReq" : "SpdLimWarnReq1",
    "SpdWarn" : "Trig1",
    "SrvRst" : "Trig1",
    "SrvTrig" : "VALUETYPE:18b7dba5e8004d269c6314c445668f3a",
    "SteerAssiActvForAutDrv" : "OnOffPen",
    "SteerAssiLvlForAutDrv" : "SteerAssiLvlForAutDrvPen1",
    "SteerSetg" : "SteerSetg",
    "SteerWarnReqForAutDrv" : "SteerWarnReqForAutDrv1",
    "SteerWarnReqForLaneKeepAid" : "WarnLeRi",
    "SteerWhlHeatgDurgClimaEnad" : "OnOff1",
    "SteerWhlHeatgOnReq" : "SteerWhlHeatgOnCmdTyp",
    "SteerWhlHeatgStrtAutCdnOk" : "OnOff1",
    "SteerWhlSnsrSafe" : "SteerWhlSnsrSafe1",
    "SteerWhlSwtPwr" : "OnOff1",
    "StopStrtInhb" : "IDENTICALTYPE:4ee9154e378347c295c29a9a18f350ff",
    "StrtInProgs" : "StrtInProgs1",
    "SunRoofPosnSts" : "WinAndRoofAndCurtPosnTyp",
    "SuspSetg" : "SuspSetgRec",
    "SwtAcptReq" : "PsdNotPsd",
    "SwtAtCenSts" : "Trig1",
    "SwtForPassAirbCutOffSt" : "OnOff2",
    "TankFlapSts" : "DoorSts2",
    "TelmSts" : "TelmSts",
    "TelmStsConnGen2" : "TelmSts",
    "TiAndDateIndcn" : "DateTi30",
    "TiGapAdpvSeld" : "TiGapAdpvSeldPen1",
    "TiGapLimd" : "TiGapPen1",
    "TireCircumCalByNav" : "TireCircumCalByNav",
    "TireMonCalReq" : "StrtStopReq1",
    "TireMonrDispReq" : "OnOff1",
    "TirePAbsltValFrntRi" : "TirePAbsltValFrntRi1",
    "TirePAbsltValFrntLe" : "TirePAbsltValFrntLe3",
    "TirePAbsltValReLe" : "TirePAbsltValReLe1",
    "TirePAbsltValReRi" : "TirePAbsltValReRi1",
    "TirePCalSts" : "TirePCalSts2",
    "TirePMonData" : "TirePMonData1",
    "TnlEntryOrEnd" : "NoYes1",
    "TrSts" : "DoorSts2",
    "TrfcLiSpprtActv" : "OnOff1",
    "TrfcLiSpprtSts" : "FctSts2",
    "TrfcRglnSeldForRoadSgnInfo" : "OnOffPen",
    "TripModSeln" : "TripModSeln",
    "TrlrLampChkSts1WdSts1" : "TrlrLampChkSts1",
    "TrlrLampChkAutReq" : "OnOffPen",
    "TrlrLampChkRemReq" : "StrtStopReq1",
    "TrlrLampChkSts1WdSts2" : "TrlrLampChkSts2",
    "TrlrPrsntSts1WdSts" : "TrlrPrsntSts",
    "LiExtReq1WdReq3" : "OnOffPen",
    "TwliBriRaw" : "TwliBriRaw",
    "TwliBriSts" : "TwliBriSts1",
    "TwliBriStsForSeatHeatrRe" : "TwliBriSts1",
    "UkwnCptReqToInfoMgr" : "UInt64Rec",
    "UkwnCptReqToSoundMgr" : "UInt64Rec",
    "UkwnCptRespFromInfoMgr" : "UInt64Rec",
    "UkwnCptRespFromSoundMgr" : "UInt64Rec",
    "UnlckFbVisReq" : "OnOffPen",
    "UnlckKeylsReq" : "UnlckKeylsCfgPen2",
    "UnlckRemReq" : "UnlckRemCfgPen1",
    "UsrSeldDrvrSpprtFct" : "DrvrSpprtFct",
    "UsrSetSpdForKeySpdLimn" : "UsrSetSpdForKeySpdLimn",
    "UsrSetSpdForKeySpdWarn" : "UsrSetSpdForKeySpdWarn",
    "UsrSetVolMaxForKeyVolLimn" : "UsrSetVolMaxForKeyVolLimn",
    "UsrSwtDispClimaReq" : "OnOffIdle1",
    "UsrSwtDispClimaReqForRowSec" : "UsrSwtDispClimaReqForRowSec",
    "SeatHeatReLeSts" : "SeatClimaLvl",
    "SeatHeatReRiSts" : "SeatClimaLvl",
    "UsrSwtDispClimaSts" : "UsrSwtDispClimaSts",
    "UsrSwtDispForSecRowSeatVentn" : "UsrSwtDispForSecRowSeatVentn",
    "UsrSwtDispReqForSecRowSeatVentn" : "UsrSwtDispReqForSecRowSeatVentn",
    "UsrSwtDispReqVrnt" : "UsrSwtDispReqVrnt",
    "SeatHeatLvlReqLe" : "SeatClimaLvl",
    "SeatHeatLvlReqRi" : "SeatClimaLvl",
    "VFCVectorIHU" : "VFCGrp",
    "VehActvMsgToDrvr" : "VehActv1",
    "VehBattU" : "SysU",
    "VehCfgPrm" : "VehCfgPrm",
    "VehDecelCtrlSetg" : "NormSptPen",
    "VehMNom" : "VehMNomRec1",
    "VehModMngtGlbSafe1" : "VehModMngtGlbSafe1",
    "VehMtnStSafe" : "VehMtnStSafe1",
    "AudWarn" : "bool",
    "VehSpdAvgIndcd" : "VehSpdIndcd1",
    "VehSpdCtrlActvSts" : "OnOff1",
    "VehSpdIndcd" : "VehSpdIndcd1",
    "VehSpdLgtSafe" : "VehSpdLgtSafe1",
    "Vin" : "Vin1",
    "VisnAgWideCfg" : "VisnAgWideCfg1",
    "VisnAgWideSts" : "AvlSts2",
    "VisnImgAgWideInUse" : "VisnImgAgWideInUse",
    "VisnImgAgWideReq" : "VisnImgAgWideInUse",
    "VisnImgCamReq" : "CamIndReq",
    "WarnTypForLaneChgWarn" : "WarnTypForLaneChgWarnPen1",
    "WarnTypForLaneKeepAid" : "WarnTypForLaneChgWarnPen1",
    "WhlCircum" : "IDENTICALTYPE:7c505b6af886426898ca88735c22419b",
    "WhlMotSysTqEst" : "TqSafe2",
    "WhlRotToothCntr" : "WhlRotToothCntrRec",
    "WinPosnStsAtDrv" : "WinAndRoofAndCurtPosnTyp",
    "WinPosnStsAtDrvrRe" : "WinAndRoofAndCurtPosnTyp",
    "WinPosnStsAtPass" : "WinAndRoofAndCurtPosnTyp",
    "WinPosnStsAtPassRe" : "WinAndRoofAndCurtPosnTyp",
    "WipgInfo" : "WipgInfo",
    "WiprFrntSrvModReq" : "WiprFrntSrvModReq",
    "WiprInPosnForSrv" : "OnOff1",
    "WiprReAutReq" : "OnOffPen",
    "WshrFldSts1WdElmHMI" : "OnOff1",
    "iTPMSCalPsbl" : "CalPsbl",
    "iTPMSCalReq" : "StrtStopReq1",
    "iTPMSCalSts" : "iTPMSTirePCalSts",
    "iTPMSTirePMSts" : "iTPMSTirePMSts",
}

is_insignal_map = {
"ADataRawSafe" : True,
"AccAdprSpdLimActv" : False,
"AccAdprTurnSpdActv" : False,
"AccAutResuWarnReq" : True,
"AccSts" : True,
"ActrPosnFrnReqForOsaAndRecirc" : True,
"ActvNoiseCtrlReq" : False,
"ActvOfHorn" : True,
"ActvnOfCrsEco" : False,
"ActvnOfPrkgAssi" : False,
"ActvnOfPrkgAut" : False,
"ActvnOfSwtIllmnCen" : False,
"ActvnOfSwtIllmnClima" : False,
"ActvnOfSwtIllmnDrvMod" : False,
"ActvnOfSwtIllmnForSeatHeatrRe" : False,
"ActvtAutnmsPrkgCtrl" : False,
"AdjSpdLimnSts" : True,
"AdjSpdLimnWarn" : True,
"AdprTurnSpdMod" : False,
"AgDataRawSafe" : True,
"LiExtReq1WdReq5" : False,
"AirClngReq" : False,
"AmbTIndcdWithUnit" : True,
"AmbTRaw" : True,
"AntithftRednReq" : False,
"AsyFctDeactvnWarn" : True,
"AsyLaneChg" : True,
"AsySteerApplyRqrd" : True,
"AsySteerFctDeactvdWarn" : True,
"AudMsgReq" : True,
"AudWarnActv" : False,
"AutnmsPrkgActvScenario" : True,
"AutnmsPrkgActvn" : False,
"AutnmsPrkgAvlScenario" : True,
"AutnmsPrkgEnaResu" : True,
"AutnmsPrkgProgs" : True,
"AutnmsPrkgSeldScenario" : False,
"AvlStsForLatAutDrv" : True,
"BackCntrForMissCom" : True,
"BlisSwOnOff" : False,
"BltLockStAtDrvr" : True,
"BltLockStAtPass" : True,
"BltLockStAtRowSecLe" : True,
"BltLockStAtRowSecMid" : True,
"BltLockStAtRowSecRi" : True,
"BltLockStAtRowThrdLe" : True,
"BltLockStAtRowThrdRi" : True,
"BltRmnSound1" : True,
"BltRmnSound2" : True,
"BltRmnSound3" : True,
"BltRmnSound4" : True,
"BrkAndAbsWarnIndcnReq" : True,
"BrkFldLvl" : True,
"BrkFricTqAtWhlAct" : True,
"BrkRelsWarnReq" : True,
"Btn1ForUsrSwtPanFrntReq" : True,
"Btn2ForUsrSwtPanFrntReq" : True,
"Btn3ForUsrSwtPanFrntReq" : True,
"Btn4ForUsrSwtPanFrntReq" : True,
"Btn5ForUsrSwtPanFrntReq" : True,
"BtnAudFbSts" : True,
"BtnAudVolSts" : True,
"BtnIllmnForWinDefrstFrnt" : False,
"BtnMmedLeRiSts" : True,
"BtnMmedModSts" : True,
"BtnMmedSetSts" : True,
"CCSMBtn6" : True,
"CallSts" : False,
"CarTiGlb" : True,
"CchForFuEco" : True,
"ChdLockgProtnStsToHmi" : True,
"ChdWinProtnStsAtDrvrRe" : True,
"ChdWinProtnStsAtPassRe" : True,
"ChrgrHwAprvdWirelsAdr" : True,
"ChrgrWirelsSts" : True,
"ClimaActv" : True,
"ClimaPwrCns" : True,
"ClimaRqrdFromHmi" : False,
"ClimaTmr" : False,
"ClimaTmrSts" : True,
"ClimaTmrStsRqrd" : False,
"ClimaWarn" : True,
"CllsnAidSnvtySeld" : False,
"CllsnFwdWarnActv" : False,
"CllsnFwdWarnReq" : True,
"CllsnWarnReq" : True,
"CllsnWarnSide" : True,
"ClngActv" : True,
"ClngRqrdFromHmi" : False,
"ClsAutEna" : False,
"CmftFctActv" : False,
"CmftFctSts" : True,
"CmptmtAirTEstimdExtd" : True,
"CmptmtTFrnt" : True,
"CnclWarnForCrsCtrl" : True,
"CnclWarnLgtForAutDrv" : True,
"CnsPrm" : True,
"ComLostExtrSound" : True,
"ConSftyDataSharingAllwd" : False,
"ConSftyDataSharingSts" : True,
"ConSftyWarn" : True,
"CoolgReqForDispCen" : False,
"CoolgStsForDisp" : True,
"CrsCtrlrSts" : True,
"CrvtSpdAdpvSts" : True,
"CrvtSpdEstimrSnvtySeld" : False,
"CrvtSpdWarnActv" : False,
"CrvtSpdWarnReq" : True,
"CrvtSpdWarnSts" : True,
"CtraIndcnLe" : True,
"CtraIndcnRi" : True,
"CtraOn1" : True,
"CtraSwOnOff" : False,
"CurtActvnReReq" : False,
"DataDistbnAllwdForEgyPred" : False,
"DataDistbnReqForEgyPred" : True,
"DataSpclDMSM" : True,
"DayToSrv" : True,
"DeactvtLvlgCtrl" : False,
"DefrstDurgClimaEnad" : False,
"DestSrcAndCfmRqrd" : True,
"DiagcCCSM" : True,
"DiagcRCSM" : True,
"DiagcRSHC" : True,
"DiagcStsDMSM" : True,
"DispAndHomeBtnSts" : False,
"DispBattEgyIn" : True,
"DispBattEgyOut" : True,
"DispOfPrpsnMod" : True,
"DispStsCen" : False,
"DoorDrvrLockReSts" : True,
"DoorDrvrLockSts" : True,
"DoorDrvrReSts" : True,
"DoorDrvrSts" : True,
"DoorPassLockReSts" : True,
"DoorPassLockSts" : True,
"DoorPassReSts" : True,
"DoorPassSts" : True,
"DriveAwayInfoActvReq" : False,
"DriveAwayInfoActvSts" : True,
"DriveAwayInfoSoundWarnActvReq" : False,
"DriveAwayInfoWarnReq" : True,
"DrvModDispd" : False,
"DrvModReq" : False,
"DrvgInWrgDirOfTrfc" : False,
"DrvrCtrlOfPassSeatFrntReqd" : False,
"DrvrCtrlOfPassSeatFrntSts" : True,
"DrvrDesDir" : True,
"DrvrDispSetg" : False,
"DrvrEntryLoReq" : False,
"DrvrHmiBackGndForHmiCen" : False,
"DrvrHmiBackGndInfoSetg" : False,
"DrvrHmiDispdMod" : False,
"DrvrHmiForHmiCen" : False,
"DrvrHmiSpdLimAdpnSts" : True,
"DrvrHmiTmrForHmiCen" : False,
"DrvrHmiUsrIfSetg" : False,
"DrvrMassgRunng" : True,
"DrvrPfmncLvl" : True,
"DrvrPfmncMonActv" : False,
"DrvrPfmncSts" : True,
"DrvrPfmncWarnReq" : True,
"DrvrPfmncWarnReqToNav" : False,
"DrvrSeatActvSpplFct" : True,
"DrvrSeatDispMassgFct" : False,
"DrvrSeatDispSpplFct" : False,
"DrvrSeatMassgFct" : True,
"DrvrSeatSwtSts" : True,
"DrvrWLoadLvl" : True,
"DstEstimdToEmptyForDrvgElec" : True,
"DstEstimdToEmptyForDrvgElecEco" : True,
"DstNotifActv" : False,
"DstNotifSts" : True,
"DstToEmptyIndcd" : True,
"DstToManvLocn" : False,
"DstToSrv" : True,
"DstTrvld1" : True,
"DstTrvld2" : True,
"DstTrvldHiResl" : True,
"BkpOfDstTrvld" : True,
"DstTrvldMst2" : True,
"EasyInOutDrvrSeatAdjmt" : False,
"EgyCostForRoute" : True,
"EgySave" : False,
"EmgyAsscSts" : True,
"EmgyAsscStsConnGen2" : True,
"EmgyVehWarnActv" : False,
"EmgyVehWarnSts" : True,
"EngCooltIndcnReq" : True,
"EngCooltLvl" : True,
"EngFuCns" : True,
"EngFuCnsFild" : True,
"EngHrToSrv" : True,
"EngIdleEco1" : False,
"EngNSafe" : True,
"EngOilLvl" : True,
"EngOilLvlSts" : True,
"EngOilPWarn" : True,
"EngSpdDispd" : True,
"EngStrtStopDrvModIndSetg" : False,
"EngStrtStopSetg" : False,
"EpbApplyAutSetg" : False,
"EpbEngStallApplySetg" : False,
"EpbLampReq" : True,
"EscSptModReqdByDrvr" : False,
"EscSt" : True,
"EscWarnIndcnReq" : True,
"ExtrMirrFoldSetg" : False,
"ExtrMirrTiltSetg2" : False,
"ExtrMirrTintgSetg" : False,
"FRNetworkStatus" : True,
"FltIndcrTurnLeFrnt" : True,
"FltIndcrTurnLeRe" : True,
"FltIndcrTurnRiFrnt" : True,
"FltIndcrTurnRiRe" : True,
"FltOfLiDaytiRunngRi" : True,
"FrntAxleWarn" : True,
"FrntWiprLvrReq2" : True,
"FuHeatrActv" : True,
"FuHeatrFuCns1" : True,
"FuHeatrFuCnsDurgCyc1" : True,
"FuLvlIndcd" : True,
"FuLvlLoIndcn" : True,
"FuLvlLoIndcnToNav" : False,
"FuLvlLoWarn" : True,
"GearIndcnRec" : True,
"GlbRstForSetgAndData" : False,
"HdHwAprvdWirelsAdr" : False,
"HdrestFoldReq2" : False,
"HeatrDurgDrvgReqd" : False,
"HeatrPreCdngTyp" : False,
"HmiAudSts" : False,
"HmiCenForDrvrHmi" : True,
"HmiCmptmtAirDistbnFrnt" : False,
"HmiCmptmtCoolgReq" : False,
"HmiCmptmtTSp" : False,
"HmiDefrstElecReq" : False,
"HmiDefrstElecSts" : True,
"HmiDefrstMaxReq" : False,
"HmiHvacFanLvlFrnt" : False,
"HmiHvacFanLvlRe" : False,
"HmiHvacReCtrl" : False,
"HmiHvacRecircCmd" : False,
"HmiSeatClima" : False,
"HmiSeatClimaExtd" : False,
"HoodSts" : True,
"HptcWarnSeldForCllsnFwdWarn" : False,
"HudActvReq" : False,
"HudActvSts" : True,
"HudAdjmtReq" : False,
"HudDiagc" : True,
"HudErgoSetgReq" : False,
"HudSts" : True,
"HudVisFctSetg" : False,
"HvBattEgyAvlDcha3" : True,
"HvBattSmtActv" : True,
"HvBattSmtSeld" : False,
"HvacAirMFlowEstimd" : True,
"HznData" : False,
"HznEdge" : False,
"HznPosn" : False,
"HznPosnExtd" : False,
"HznPosnExtdOffs" : False,
"HznProfLong" : False,
"HznProfLongExtd" : False,
"HznProfSho" : False,
"HznRstExtd" : True,
"HznSeg" : False,
"HznSplyElectcSts" : False,
"HzrdLiWarnActv" : False,
"HzrdLiWarnSts" : True,
"IndcnOfParkAssiSts" : True,
"IndcnOfPrkgAutSts" : True,
"IndcnUnit" : False,
"IndcrDisp1WdSts" : True,
"IndcrTurnSts1WdSts" : True,
"IniValSigCfgIDBackboneFR" : True,
"InsdCarNoiseMeasd" : False,
"IntAudCnclWarn" : False,
"IntrBriSts" : True,
"IntrBriStsForSeatHeatrRe" : False,
"IntrLiAmbLiSetg" : False,
"IntrLiSurrndgsLiSetgLi" : False,
"IntrMirrTintgSetg" : False,
"IntvAndWarnModForLaneKeepAid" : False,
"KeyLostWarnIndcn" : True,
"KeyProfMpgUpd" : False,
"KeyReadReqFromSetgMgr" : False,
"KeyReadStsToProfCtrl" : True,
"KeyRmnIndcn" : True,
"KeySpdWarn" : True,
"LampSuppSrv" : False,
"LaneChgWarnActv" : False,
"LaneChgWarnSts" : True,
"LaneDetnStsForAutDrv" : True,
"LaneDetnStsForLaneKeepAid" : True,
"LaneKeepAidActv" : False,
"LaneKeepAidRoadEdgeActv" : False,
"LaneKeepAidSts" : True,
"LatMovmtWarn" : True,
"LcmaOn1" : True,
"LiExtReq1WdReq1" : False,
"LiExtReq2WdReq1" : False,
"LiExtReq1WdReq4" : False,
"LiExtReq1WdReq6" : False,
"LiExtReq1WdReq2" : False,
"LiDrvrFltIndcrTurn" : True,
"LiForBtn4ForUsrSwtPanFrntCmd" : False,
"LiForBtn5ForUsrSwtPanFrntCmd" : False,
"LiExtSafe1WdReq2" : False,
"LiExtSafe1WdReq1" : False,
"LiLvrSwt1Req" : True,
"LiPassFltIndcrTurn" : True,
"LiSeldForDrvrPfmncMon" : False,
"LiTrfcSide1WdReq1" : False,
"ListOfNodAv" : True,
"LockSpdReq" : False,
"LockgCenSts" : True,
"LockgCenStsForUsrFb" : True,
"LockgFbSoundReq" : False,
"LockgFbVisReq" : False,
"LockgPrsnlReqFromHmi" : False,
"LockgPrsnlSts" : True,
"LvlOfClimaCmft" : True,
"LvlSeldForSpdLimAdpv" : False,
"MassgFctActv" : False,
"MemBtnSound" : True,
"MirrDwnStsAtDrvr" : True,
"MirrDwnStsAtPass" : True,
"MirrFoldStsAtDrvr" : True,
"MirrFoldStsAtPass" : True,
"MmedHdPwrMod" : False,
"MmedHmiModStd" : False,
"MmedTvmPwerMod" : False,
"MstCfgIDBackboneFR" : True,
"MtrlSnsrT" : True,
"NFSDataFront" : True,
"NFSDataRear" : True,
"NetCtrlrActvt" : True,
"NetHdActvt" : False,
"NetTelmActvt" : True,
"NewTripCdn" : True,
"NoEntryWarnReq" : True,
"NoSoundSys" : False,
"NotifChkDistbn" : True,
"NrOfBltAppld" : True,
"NrOfKeyAvl" : True,
"NrSerlDMSM" : True,
"OffsForDrvrSpprtFctActvSts" : False,
"OffsForSpdWarnSetg" : False,
"PartNrDMSM" : True,
"PasAlrmDeactvnReq" : False,
"PasAlrmSts" : True,
"PassMassgRunng" : True,
"PassSeatActvSpplFct" : True,
"PassSeatDispMassgFct" : False,
"PassSeatDispSpplFct" : False,
"PassSeatMassgFct" : True,
"PassSeatSts" : True,
"PassSeatSwtSts2" : True,
"PinionSteerAg1" : True,
"PosnFromNav" : False,
"PosnFromSatlt" : True,
"PostDrvgClimaAvl" : True,
"PostDrvgClimaReq" : False,
"PreClngNotif" : True,
"PrkgAssiActvSts" : True,
"PrkgAssiAudWarnFront" : True,
"PrkgAssiAudWarnRear" : True,
"PrkgAssiFailr" : True,
"PrkgAssiManvActvSts" : True,
"PrkgAssiManvProgs" : True,
"PrkgAssiSts" : True,
"PrkgAutSts" : True,
"PrkgCamSysAvlSts" : True,
"PrkgOutCfm" : False,
"PrkgTypVld" : True,
"ProfAct" : True,
"ProfChg" : False,
"ProfLimd" : True,
"ProfPenSts1" : True,
"PrpsnDrvMod" : True,
"PrpsnHvBattUsgModAct" : True,
"PrpsnHvBattUsgModReq" : False,
"PrpsnHvBattUsgOfChrgBlkd" : True,
"PrpsnHvBattUsgOfChrgBlkd2" : True,
"PrpsnHvBattUsgOfHldBlkd" : True,
"PrpsnHvBattUsgOfHldBlkd2" : True,
"PrpsnHvBattUsgOfHldSmtBlkd" : True,
"PrpsnHvBattUsgOfHldSmtBlkd2" : True,
"PrpsnHvBattUsgOfHldSpd" : False,
"PrpsnHvBattUsgStsDispd" : False,
"PrpsnModElecDrvBlkd" : True,
"PrpsnModOfSaveBlkd" : True,
"PrpsnModOfTracBlkd" : True,
"PrpsnModOffroadBlkd" : True,
"PrpsnModSptBlkd" : True,
"PtCluTq" : True,
"PtDrvrSetg" : False,
"PtGearTar" : True,
"PtTqAtWhlFrntAct" : True,
"PwrChrgDetdForPrkgHeatrElec" : True,
"PwrSplyErrSts" : True,
"RadioFrqAn" : False,
"RainSenMemdReq" : False,
"ReAxleWarn" : True,
"RlyPwrDistbnCmd1WdBattSaveCmd" : True,
"RlyPwrDistbnCmd1WdIgnRlyCmd" : True,
"RlyPwrDistbnCmd1WdIgnRlyExtCmd" : True,
"RlyPwrDistbnCmd1WdPreBattSaveCmd" : True,
"RmnLockgPrsnlReq" : True,
"RngbdIllmnCmd" : False,
"RoadFricIndcnActv" : False,
"RoadFricIndcnSts" : True,
"RoadFricWarnReq" : True,
"RoadSgnInfoActv" : False,
"RoadSgnInfoSts" : True,
"RoadSpdLimActvSts" : False,
"RoadUsrProtnActv" : False,
"RoadUsrProtnSts" : True,
"RotyDirReq2" : True,
"RotyDirReq1" : True,
"RotyPosReq2" : True,
"RotyPosReq1" : True,
"RouteInfo" : False,
"RsdsSysStsLe" : True,
"RsdsSysStsRi" : True,
"SaveSetgToMemPrmnt" : False,
"ScrBarVolIndcn" : True,
"ScrMaxFillgVol" : True,
"ScrReagentTankVol" : True,
"SeatBackUnlckdThrd" : True,
"SeatDispBtnPsd" : False,
"SeatHeatDurgClimaEnad" : False,
"SeatHeatgAutCdn" : True,
"SeatSwtLeSigThrd" : False,
"SeatSwtRiSigThrd" : False,
"SeatVentnAutCdn" : True,
"SetOfLang" : False,
"SetOfLangAck" : True,
"SetTiAndDate" : False,
"SetgAndRstOfTripForDrvr" : False,
"SftyCchAccSts" : True,
"SftyCchDrvgCycIdx" : True,
"SftyCchLaneKeepAidSts" : True,
"SftyCchLongTermIdx" : True,
"SftyCchLongTermIdxRst" : False,
"SftyCchMidTermIdx" : True,
"SftyCchPilotAssiSts" : True,
"SftyCchPostTripSts" : False,
"SftyCchPrimFbAftDrvg" : True,
"SftyCchPrimFbDurgDrvg" : True,
"SftyCchSecFbAftDrvg" : True,
"SftyCchSecFbDurgDrvg" : True,
"SftyCchShortTermIdx" : True,
"SftyCchSts" : True,
"SftySigGroupFromAudSafe" : False,
"SftyWarnGroupFromAsySafe" : True,
"SnsrParkAssiFrnt" : True,
"SnsrParkAssiRe" : True,
"SnsrPrkgAssiFrnt" : True,
"SnsrPrkgAssiRe" : True,
"SoundExtActvSts" : False,
"WhlMotSysSpdAct" : True,
"SpdAlrmActvForRoadSgnInfo" : False,
"SpdCameraInfoSeldForRoadSgnInfo" : False,
"SpdLimAdpvSts" : True,
"SpdLimWarnReq" : True,
"SpdWarn" : True,
"SrvRst" : True,
"SrvTrig" : True,
"SteerAssiActvForAutDrv" : False,
"SteerAssiLvlForAutDrv" : False,
"SteerSetg" : False,
"SteerWarnReqForAutDrv" : True,
"SteerWarnReqForLaneKeepAid" : True,
"SteerWhlHeatgDurgClimaEnad" : False,
"SteerWhlHeatgOnReq" : False,
"SteerWhlHeatgStrtAutCdnOk" : True,
"SteerWhlSnsrSafe" : True,
"SteerWhlSwtPwr" : True,
"StopStrtInhb" : True,
"StrtInProgs" : True,
"SunRoofPosnSts" : True,
"SuspSetg" : False,
"SwtAcptReq" : True,
"SwtAtCenSts" : False,
"SwtForPassAirbCutOffSt" : True,
"TankFlapSts" : True,
"TelmSts" : True,
"TelmStsConnGen2" : True,
"TiAndDateIndcn" : True,
"TiGapAdpvSeld" : False,
"TiGapLimd" : False,
"TireCircumCalByNav" : False,
"TireMonCalReq" : False,
"TireMonrDispReq" : False,
"TirePAbsltValFrntRi" : True,
"TirePAbsltValFrntLe" : True,
"TirePAbsltValReLe" : True,
"TirePAbsltValReRi" : True,
"TirePCalSts" : True,
"TirePMonData" : True,
"TnlEntryOrEnd" : True,
"TrSts" : True,
"TrfcLiSpprtActv" : False,
"TrfcLiSpprtSts" : True,
"TrfcRglnSeldForRoadSgnInfo" : False,
"TripModSeln" : False,
"TrlrLampChkSts1WdSts1" : True,
"TrlrLampChkAutReq" : False,
"TrlrLampChkRemReq" : False,
"TrlrLampChkSts1WdSts2" : True,
"TrlrPrsntSts1WdSts" : True,
"LiExtReq1WdReq3" : False,
"TwliBriRaw" : True,
"TwliBriSts" : True,
"TwliBriStsForSeatHeatrRe" : False,
"UkwnCptReqToInfoMgr" : True,
"UkwnCptReqToSoundMgr" : True,
"UkwnCptRespFromInfoMgr" : False,
"UkwnCptRespFromSoundMgr" : False,
"UnlckFbVisReq" : False,
"UnlckKeylsReq" : False,
"UnlckRemReq" : False,
"UsrSeldDrvrSpprtFct" : False,
"UsrSetSpdForKeySpdLimn" : False,
"UsrSetSpdForKeySpdWarn" : False,
"UsrSetVolMaxForKeyVolLimn" : False,
"UsrSwtDispClimaReq" : False,
"UsrSwtDispClimaReqForRowSec" : True,
"SeatHeatReLeSts" : False,
"SeatHeatReRiSts" : False,
"UsrSwtDispClimaSts" : False,
"UsrSwtDispForSecRowSeatVentn" : False,
"UsrSwtDispReqForSecRowSeatVentn" : True,
"UsrSwtDispReqVrnt" : False,
"SeatHeatLvlReqLe" : True,
"SeatHeatLvlReqRi" : True,
"VFCVectorIHU" : False,
"VehActvMsgToDrvr" : True,
"VehBattU" : True,
"VehCfgPrm" : True,
"VehDecelCtrlSetg" : False,
"VehMNom" : True,
"VehModMngtGlbSafe1" : True,
"VehMtnStSafe" : True,
"AudWarn" : True,
"VehSpdAvgIndcd" : True,
"VehSpdCtrlActvSts" : False,
"VehSpdIndcd" : True,
"VehSpdLgtSafe" : True,
"Vin" : True,
"VisnAgWideCfg" : False,
"VisnAgWideSts" : True,
"VisnImgAgWideInUse" : True,
"VisnImgAgWideReq" : False,
"VisnImgCamReq" : False,
"WarnTypForLaneChgWarn" : False,
"WarnTypForLaneKeepAid" : False,
"WhlCircum" : True,
"WhlMotSysTqEst" : True,
"WhlRotToothCntr" : True,
"WinPosnStsAtDrv" : True,
"WinPosnStsAtDrvrRe" : True,
"WinPosnStsAtPass" : True,
"WinPosnStsAtPassRe" : True,
"WipgInfo" : True,
"WiprFrntSrvModReq" : False,
"WiprInPosnForSrv" : True,
"WiprReAutReq" : False,
"WshrFldSts1WdElmHMI" : True,
"iTPMSCalPsbl" : True,
"iTPMSCalReq" : False,
"iTPMSCalSts" : True,
"iTPMSTirePMSts" : True,

}
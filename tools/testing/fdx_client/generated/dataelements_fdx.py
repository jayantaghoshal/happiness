#coding: utf-8

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#AUTO GENERATED FILE, do not modify by hand.
# Generated with args: generate_signal_scaling.py --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml --fdxdescriptionfile=../CANoe/SPA2610/FDXDescriptionFile.xml --out=generated/pyDataElements.py
SWC_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml"
COM_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml"
from . import datatypes
from . import dataelements_abc
import time
import threading
import os
import logging
import typing
from enum import Enum
from fdx import fdx_description_file_parser
from fdx import fdx_client






def _get_if_not_None(x):
    if x is None:
        raise AttributeError("Trying to send signal with empty data")
    return x

def _extract_enum_value(x):
    if isinstance(x, Enum):
        return x.value
    return x

def p2r_float(raw, scale, offset):
    if raw is None:
        raise AttributeError("Trying to send signal with empty data")
    return (raw - offset) / scale

def r2p_float(phyiscal, scale, offset):
    return (phyiscal * scale) + offset

ns_per_ms = 1000000

# ag = assign_and_assert_same_group
def _ag(item, value, group):
    # type: (fdx_description_file_parser.Item, typing.Any, fdx_description_file_parser.Group) -> None
    assert item.parent_group == group, "All signals in a datagroup should be in the same frame"
    item.value_raw = value

# gg = get_and_assert_same_group
def _gg(item, group):
    # type: (fdx_description_file_parser.Item, fdx_description_file_parser.Group) -> typing.Any
    assert item.parent_group == group, "All signals in a datagroup should be in the same frame"
    return item.value_raw



class FrSignalInterface(dataelements_abc.DataElements):

    def __init__(self, ip=None):

        self.connected = False
        self.logger = logging.getLogger(__name__)

        (self.groups, self.sysvar_list, self.signal_list) = fdx_description_file_parser.parse(
            os.path.join(os.path.dirname(__file__), "../../CANoe/SPA2610/FDXDescriptionFile.xml"))
        self.fdx_items_by_rte_name = {s.name: s for s in self.signal_list}  # type: typing.Dict[str, fdx_description_file_parser.Item]

        self.group_id_map = {g.group_id: g for g in self.groups}

        received_group_ids = set()

        def data_exchange(group_id, data):
            received_group_ids.add(group_id)
            group = self.group_id_map[group_id]
            group.receive_data(data)

        if ip is None:
            ip = os.environ['VECTOR_FDX_IP']

        try:
            self.connection = fdx_client.FDXConnection(
                data_exchange,
                ip,
                int(os.environ.get('VECTOR_FDX_PORT', '2809')))
            self.connection.confirmed_stop()    # Stop in case previous test failed to stop
            self.connection.confirmed_start()
            self.verify_simulation_version()
            groups_to_subscribe = [g for g in self.groups if "ihubackbone" in g.name.lower() or "ihulin19" in g.name.lower()]
            expected_group_ids = set([g.group_id for g in groups_to_subscribe])
            for g in groups_to_subscribe:
                # Sleep is Super ugly, super important. Seems like the Vector some times miss some free_running_request, this fixes it.
                time.sleep(0.3)
                self.connection.send_free_running_request(g.group_id, fdx_client.kFreeRunningFlag.transmitCyclic, 500 * ns_per_ms, 0)

            # Extra check that free_running_request for all group ids is working, it has shown some reliability issues (see sleep above)
            deadline = time.time() + 3
            while True:
                missing_group_ids = expected_group_ids - received_group_ids
                if len(missing_group_ids) == 0:
                    break
                time.sleep(0.05)
                if time.time() < deadline:
                    continue
                else:
                    raise Exception("Failed to subscribe to all groupids, missing: %r" % missing_group_ids)

            self.connected = True
        except:
            self.connection.close()
            raise


        self.send_timer = threading.Timer(0.1, self._send_timer_func)
        self.send_timer.daemon = True
        self.timer_mutex = threading.Lock()
        self.groups_on_timer = set()   # type: typing.Set[fdx_description_file_parser.Group]

    def verify_simulation_version(self):
        # SPA2210/FR_Body_LIN/SimulationDB/Simulation.vsysvar
        EXPECTED_VERSION = 10
        simulation_version = next((s for s in self.sysvar_list if s.msg_or_namespace == "Configuration::VersionInfo" and s.name == "SimulationVersion"))
        deadline = time.time() + 20
        while simulation_version.value_raw != EXPECTED_VERSION and time.time() < deadline:
            self.connection.send_data_request(simulation_version.parent_group.group_id)
            time.sleep(0.5)
        if simulation_version.value_raw != EXPECTED_VERSION:
            raise Exception("Simulation version mismatch! CANoe simulation version=%r, expected version=%d" % (simulation_version.value_raw, EXPECTED_VERSION))

    def close(self):
        with self.timer_mutex:
            self.groups_on_timer.clear()
        if self.connected:
            self.connection.confirmed_stop()
            self.connection.close()

    def restart_timer(self):
        # TODO: Investigate if 100ms is ok for all signals
        self.send_timer = threading.Timer(0.1, self._send_timer_func)
        self.send_timer.daemon = True
        self.send_timer.start()

    def _send_timer_func(self):
        with self.timer_mutex:
            for g in self.groups_on_timer:
                self.connection.send_data_exchange(g.group_id, g.size, g.build_data())
            if len(self.groups_on_timer) > 0:
                self.restart_timer()

    def add_group_to_send_timer(self, group):
        # type: (fdx_description_file_parser.Group) -> None
        with self.timer_mutex:
            if len(self.groups_on_timer) == 0:
                self.restart_timer()
            self.groups_on_timer.add(group)

    def remove_group_from_send_timer(self, group):
        # type: (fdx_description_file_parser.Group) -> None
        with self.timer_mutex:
            self.groups_on_timer.remove(group)





    def send_ADataRawSafe(self, data):
        # type: (datatypes.ADataRawSafe1) -> None
        group = self.fdx_items_by_rte_name["ALat1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ALat1"], p2r_float(data.ALat, 0.0085, 0.0), group)
            _ag(self.fdx_items_by_rte_name["ALat1Qf1"], _extract_enum_value(_get_if_not_None(data.ALat1Qf)), group)
            _ag(self.fdx_items_by_rte_name["ALgt1"], p2r_float(data.ALgt, 0.0085, 0.0), group)
            _ag(self.fdx_items_by_rte_name["ALgt1Qf1"], _extract_enum_value(_get_if_not_None(data.ALgt1Qf)), group)
            _ag(self.fdx_items_by_rte_name["AVert2"], p2r_float(data.AVert, 0.0085, 0.0), group)
            _ag(self.fdx_items_by_rte_name["AVert2Qf1"], _extract_enum_value(_get_if_not_None(data.AVertQf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ADataRawSafe(self):
        group = self.fdx_items_by_rte_name["ALat1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_AccAdprSpdLimActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["AccAdprSpdLimActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['AccAdprSpdLimActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['AccAdprSpdLimActvPen'], group)
        return data

    def get_AccAdprTurnSpdActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["AccAdprTurnSpdActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['AccAdprTurnSpdActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['AccAdprTurnSpdActvPen'], group)
        return data

    def send_AccAutResuWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["AccAutResuWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AccAutResuWarnReq(self):
        group = self.fdx_items_by_rte_name["AccAutResuWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AccSts(self, data):
        # type: (datatypes.AccSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["AccSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AccSts(self):
        group = self.fdx_items_by_rte_name["AccSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ActrPosnFrnReqForOsaAndRecirc(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["ActrPosnFrnReqForOsaAndRecirc"]
        fdx_item.value_raw = p2r_float(data, 0.1, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ActrPosnFrnReqForOsaAndRecirc(self):
        group = self.fdx_items_by_rte_name["ActrPosnFrnReqForOsaAndRecirc"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ActvNoiseCtrlReq(self):
        # type: () -> datatypes.NormSptPen
        data = datatypes.NormSptPen()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.NormSpt = _gg(self.fdx_items_by_rte_name[''], group)
            data.Pen = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def send_ActvOfHorn(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["ActvOfHorn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ActvOfHorn(self):
        group = self.fdx_items_by_rte_name["ActvOfHorn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ActvnOfCrsEco(self):
        # type: () -> datatypes.ActvnOfCrsEco
        data = datatypes.ActvnOfCrsEco()
        group = self.fdx_items_by_rte_name["ActvnOfCrsEcoSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['ActvnOfCrsEcoSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['ActvnOfCrsEcoPen'], group)
        return data

    def get_ActvnOfPrkgAssi(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["ActvnOfPrkgAssi"]
        return fdx_item.value_raw

    def get_ActvnOfPrkgAut(self):
        # type: () -> datatypes.ActvnOfPrkgAut
        fdx_item = self.fdx_items_by_rte_name["ActvnOfPrkgAut"]
        return fdx_item.value_raw

    def get_ActvnOfSwtIllmnCen(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["ActvnOfSwtIllmnCen"]
        return fdx_item.value_raw

    def get_ActvnOfSwtIllmnClima(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["ActvnOfSwtIllmnClima"]
        return fdx_item.value_raw

    def get_ActvnOfSwtIllmnDrvMod(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["ActvnOfSwtIllmnDrvMod"]
        return fdx_item.value_raw

    def get_ActvnOfSwtIllmnForSeatHeatrRe(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["ActvnOfSwtIllmnForSeatHeatrRe"]
        return fdx_item.value_raw

    def get_ActvtAutnmsPrkgCtrl(self):
        # type: () -> datatypes.ActvtAutnmsPrkgCtrl
        fdx_item = self.fdx_items_by_rte_name["ActvtAutnmsPrkgCtrl"]
        return fdx_item.value_raw

    def send_AdjSpdLimnSts(self, data):
        # type: (datatypes.AdjSpdLimnSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["AdjSpdLimnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AdjSpdLimnSts(self):
        group = self.fdx_items_by_rte_name["AdjSpdLimnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AdjSpdLimnWarn(self, data):
        # type: (datatypes.AdjSpdLimnWarnCoding) -> None
        fdx_item = self.fdx_items_by_rte_name["AdjSpdLimnWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AdjSpdLimnWarn(self):
        group = self.fdx_items_by_rte_name["AdjSpdLimnWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_AdprTurnSpdMod(self):
        # type: () -> datatypes.AdprTurnSpdModPen1
        data = datatypes.AdprTurnSpdModPen1()
        group = self.fdx_items_by_rte_name["AdprTurnSpdModSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['AdprTurnSpdModSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['AdprTurnSpdModPen'], group)
        return data

    def send_AgDataRawSafe(self, data):
        # type: (datatypes.AgDataRawSafe1) -> None
        group = self.fdx_items_by_rte_name["RollRate1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["RollRate1"], p2r_float(data.RollRate, 0.000244140625, 0.0), group)
            _ag(self.fdx_items_by_rte_name["RollRate1Qf1"], _extract_enum_value(_get_if_not_None(data.RollRateQf)), group)
            _ag(self.fdx_items_by_rte_name["YawRate1"], p2r_float(data.YawRate, 0.000244140625, 0.0), group)
            _ag(self.fdx_items_by_rte_name["YawRate1Qf1"], _extract_enum_value(_get_if_not_None(data.YawRateQf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AgDataRawSafe(self):
        group = self.fdx_items_by_rte_name["RollRate1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LiExtReq1WdReq5(self):
        # type: () -> datatypes.SlowNormFastPen
        data = datatypes.SlowNormFastPen()
        group = self.fdx_items_by_rte_name["LiExtReq1WdReq5SlowNormFast"].parent_group #should be same in all
        with group.mutex_lock:
            data.SlowNormFast = _gg(self.fdx_items_by_rte_name['LiExtReq1WdReq5SlowNormFast'], group)
            data.IdPen = _gg(self.fdx_items_by_rte_name['LiExtReq1WdReq5IdPen'], group)
        return data

    def get_AirClngReq(self):
        # type: () -> datatypes.OnOffNoReq
        fdx_item = self.fdx_items_by_rte_name["AirClngReq"]
        return fdx_item.value_raw

    def send_AmbTIndcdWithUnit(self, data):
        # type: (datatypes.AmbTIndcdWithUnit) -> None
        group = self.fdx_items_by_rte_name["AmbTIndcd"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["AmbTIndcd"], p2r_float(data.AmbTIndcd, 0.1, -100.0), group)
            _ag(self.fdx_items_by_rte_name["AmbTIndcdUnit"], _extract_enum_value(_get_if_not_None(data.AmbTIndcdUnit)), group)
            _ag(self.fdx_items_by_rte_name["AmbTIndcdQf"], _extract_enum_value(_get_if_not_None(data.QF)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AmbTIndcdWithUnit(self):
        group = self.fdx_items_by_rte_name["AmbTIndcd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AmbTRaw(self, data):
        # type: (datatypes.AmbTWithQly) -> None
        group = self.fdx_items_by_rte_name["AmbTRawVal"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["AmbTRawVal"], p2r_float(data.AmbTVal, 0.1, -70.0), group)
            _ag(self.fdx_items_by_rte_name["AmbTRawQly"], _extract_enum_value(_get_if_not_None(data.Qly)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AmbTRaw(self):
        group = self.fdx_items_by_rte_name["AmbTRawVal"].parent_group
        self.remove_group_from_send_timer(group)

    def get_AntithftRednReq(self):
        # type: () -> int
        fdx_item = self.fdx_items_by_rte_name["AntithftRednReq"]
        return fdx_item.value_raw

    def send_AsyFctDeactvnWarn(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["AsyFctDeactvnWarn"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AsyFctDeactvnWarn(self):
        group = self.fdx_items_by_rte_name["AsyFctDeactvnWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AsyLaneChg(self, data):
        # type: (datatypes.AsyLaneChg1) -> None
        group = self.fdx_items_by_rte_name["AsyLaneChgTyp"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["AsyLaneChgTyp"], _extract_enum_value(_get_if_not_None(data.Typ)), group)
            _ag(self.fdx_items_by_rte_name["AsyLaneChgSts"], _extract_enum_value(_get_if_not_None(data.Sts)), group)
            _ag(self.fdx_items_by_rte_name["AsyLaneChgPsbl"], _extract_enum_value(_get_if_not_None(data.Psbl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AsyLaneChg(self):
        group = self.fdx_items_by_rte_name["AsyLaneChgTyp"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AsySteerApplyRqrd(self, data):
        # type: (datatypes.LvlWarn1) -> None
        fdx_item = self.fdx_items_by_rte_name["AsySteerApplyRqrd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AsySteerApplyRqrd(self):
        group = self.fdx_items_by_rte_name["AsySteerApplyRqrd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AsySteerFctDeactvdWarn(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["AsySteerFctDeactvdWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AsySteerFctDeactvdWarn(self):
        group = self.fdx_items_by_rte_name["AsySteerFctDeactvdWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AudMsgReq(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["AudMsgReq"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AudMsgReq(self):
        group = self.fdx_items_by_rte_name["AudMsgReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_AudWarnActv(self):
        # type: () -> datatypes.NoYesCrit1
        fdx_item = self.fdx_items_by_rte_name["AudWarnActv"]
        return fdx_item.value_raw

    def send_AutnmsPrkgActvScenario(self, data):
        # type: (datatypes.AutnmsPrkgActvScenario) -> None
        group = self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveState"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveState"], _extract_enum_value(_get_if_not_None(data.APActiveState)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveParkingSide"], _extract_enum_value(_get_if_not_None(data.APActiveParkingSide)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveSlotType"], _extract_enum_value(_get_if_not_None(data.APActiveSlotType)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveParkingScenario"], _extract_enum_value(_get_if_not_None(data.APActiveParkingScenario)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveParkingDirection"], _extract_enum_value(_get_if_not_None(data.APActiveParkingDirection)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveRemote"], _extract_enum_value(_get_if_not_None(data.APActiveRemote)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPPauseStatus"], _extract_enum_value(_get_if_not_None(data.APPauseStatus)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPCancelStatus"], _extract_enum_value(_get_if_not_None(data.APCancelStatus)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveDirectParking"], _extract_enum_value(_get_if_not_None(data.APActiveDirectParking)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AutnmsPrkgActvScenario(self):
        group = self.fdx_items_by_rte_name["AutnmsPrkgActvScenarioAPActiveState"].parent_group
        self.remove_group_from_send_timer(group)

    def get_AutnmsPrkgActvn(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["AutnmsPrkgActvn"]
        return fdx_item.value_raw

    def send_AutnmsPrkgAvlScenario(self, data):
        # type: (datatypes.AutnmsPrkgAvlScenario) -> None
        group = self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableParkingSide"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableParkingSide"], _extract_enum_value(_get_if_not_None(data.APAvailableParkingSide)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableParkingScenario"], _extract_enum_value(_get_if_not_None(data.APAvailableParkingScenario)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableSlotType"], _extract_enum_value(_get_if_not_None(data.APAvailableSlotType)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableParkingDirection"], _extract_enum_value(_get_if_not_None(data.APAvailableParkingDirection)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableRemote"], _extract_enum_value(_get_if_not_None(data.APAvailableRemote)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPRecommendedParkingSide"], _extract_enum_value(_get_if_not_None(data.APRecommendedParkingSide)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPRecommendedSlotType"], _extract_enum_value(_get_if_not_None(data.APRecommendedSlotType)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPRecommendedParkingDirection"], _extract_enum_value(_get_if_not_None(data.APRecommendedParkingDirection)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPRecommendedRemote"], _extract_enum_value(_get_if_not_None(data.APRecommendedRemote)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPRecommendedParkingScenario"], _extract_enum_value(_get_if_not_None(data.APRecommendedParkingScenario)), group)
            _ag(self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableDirectParking"], _extract_enum_value(_get_if_not_None(data.APAvailableDirectParking)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AutnmsPrkgAvlScenario(self):
        group = self.fdx_items_by_rte_name["AutnmsPrkgAvlScenarioAPAvailableParkingSide"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AutnmsPrkgEnaResu(self, data):
        # type: (datatypes.EnaResu) -> None
        fdx_item = self.fdx_items_by_rte_name["AutnmsPrkgEnaResu"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AutnmsPrkgEnaResu(self):
        group = self.fdx_items_by_rte_name["AutnmsPrkgEnaResu"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AutnmsPrkgProgs(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["AutnmsPrkgProgs"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AutnmsPrkgProgs(self):
        group = self.fdx_items_by_rte_name["AutnmsPrkgProgs"].parent_group
        self.remove_group_from_send_timer(group)

    def get_AutnmsPrkgSeldScenario(self):
        # type: () -> datatypes.AutnmsPrkgSeldScenario
        data = datatypes.AutnmsPrkgSeldScenario()
        group = self.fdx_items_by_rte_name["AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp"].parent_group #should be same in all
        with group.mutex_lock:
            data.AutnmsPrkgSeldSlotTyp = _gg(self.fdx_items_by_rte_name['AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp'], group)
            data.AutnmsPrkgSeldPrkgScenario = _gg(self.fdx_items_by_rte_name['AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario'], group)
            data.AutnmsPrkgSeldPrkgDir = _gg(self.fdx_items_by_rte_name['AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir'], group)
            data.AutnmsPrkgSeldPrkgSide = _gg(self.fdx_items_by_rte_name['AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide'], group)
            data.AutnmsPrkgSeldRem = _gg(self.fdx_items_by_rte_name['AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem'], group)
            data.AutnmsPrkgSeldDirectPrkg = _gg(self.fdx_items_by_rte_name['AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg'], group)
        return data

    def send_AvlStsForLatAutDrv(self, data):
        # type: (datatypes.AvlStsForLatAutDrv5) -> None
        fdx_item = self.fdx_items_by_rte_name["AvlStsForLatAutDrv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AvlStsForLatAutDrv(self):
        group = self.fdx_items_by_rte_name["AvlStsForLatAutDrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BackCntrForMissCom(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["BackCntrForMissCom"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BackCntrForMissCom(self):
        group = self.fdx_items_by_rte_name["BackCntrForMissCom"].parent_group
        self.remove_group_from_send_timer(group)

    def get_BlisSwOnOff(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["BlisSwOnOff"]
        return fdx_item.value_raw

    def send_BltLockStAtDrvr(self, data):
        # type: (datatypes.BltLockStFrnt) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtDrvrForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtDrvrForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtDrvrForDevErrSts2"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtDrvr(self):
        group = self.fdx_items_by_rte_name["BltLockStAtDrvrForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltLockStAtPass(self, data):
        # type: (datatypes.BltLockStFrnt) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtPassForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtPassForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtPassForBltLockSts"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtPass(self):
        group = self.fdx_items_by_rte_name["BltLockStAtPassForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltLockStAtRowSecLe(self, data):
        # type: (datatypes.BltLockStRe) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtRowSecLeForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecLeForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecLeForBltLockSts"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecLeForBltLockEquid"], _extract_enum_value(_get_if_not_None(data.BltLockEquid)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtRowSecLe(self):
        group = self.fdx_items_by_rte_name["BltLockStAtRowSecLeForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltLockStAtRowSecMid(self, data):
        # type: (datatypes.BltLockStRe) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtRowSecMidForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecMidForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecMidForBltLockSts"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecMidForBltLockEquid"], _extract_enum_value(_get_if_not_None(data.BltLockEquid)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtRowSecMid(self):
        group = self.fdx_items_by_rte_name["BltLockStAtRowSecMidForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltLockStAtRowSecRi(self, data):
        # type: (datatypes.BltLockStRe) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtRowSecRiForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecRiForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecRiForBltLockSts"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowSecRiForBltLockEquid"], _extract_enum_value(_get_if_not_None(data.BltLockEquid)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtRowSecRi(self):
        group = self.fdx_items_by_rte_name["BltLockStAtRowSecRiForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltLockStAtRowThrdLe(self, data):
        # type: (datatypes.BltLockStRe) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtRowThrdLeForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowThrdLeForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowThrdLeForBltLockSts"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowThrdLeForBltLockEquid"], _extract_enum_value(_get_if_not_None(data.BltLockEquid)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtRowThrdLe(self):
        group = self.fdx_items_by_rte_name["BltLockStAtRowThrdLeForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltLockStAtRowThrdRi(self, data):
        # type: (datatypes.BltLockStRe) -> None
        group = self.fdx_items_by_rte_name["BltLockStAtRowThrdRiForBltLockSt1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowThrdRiForBltLockSt1"], _extract_enum_value(_get_if_not_None(data.BltLockSt1)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowThrdRiForBltLockSts"], _extract_enum_value(_get_if_not_None(data.BltLockSts)), group)
            _ag(self.fdx_items_by_rte_name["BltLockStAtRowThrdRiForBltLockEquid"], _extract_enum_value(_get_if_not_None(data.BltLockEquid)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltLockStAtRowThrdRi(self):
        group = self.fdx_items_by_rte_name["BltLockStAtRowThrdRiForBltLockSt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltRmnSound1(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["BltRmnSound1"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltRmnSound1(self):
        group = self.fdx_items_by_rte_name["BltRmnSound1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltRmnSound2(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["BltRmnSound2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltRmnSound2(self):
        group = self.fdx_items_by_rte_name["BltRmnSound2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltRmnSound3(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["BltRmnSound3"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltRmnSound3(self):
        group = self.fdx_items_by_rte_name["BltRmnSound3"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BltRmnSound4(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["BltRmnSound4"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BltRmnSound4(self):
        group = self.fdx_items_by_rte_name["BltRmnSound4"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BrkAndAbsWarnIndcnReq(self, data):
        # type: (datatypes.BrkAndAbsWarnIndcnReqRec1) -> None
        group = self.fdx_items_by_rte_name["BrkWarnIndcnReq"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BrkWarnIndcnReq"], _extract_enum_value(_get_if_not_None(data.BrkWarnIndcnReq)), group)
            _ag(self.fdx_items_by_rte_name["AbsWarnIndcnReq"], _extract_enum_value(_get_if_not_None(data.AbsWarnIndcnReq)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BrkAndAbsWarnIndcnReq(self):
        group = self.fdx_items_by_rte_name["BrkWarnIndcnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BrkFldLvl(self, data):
        # type: (datatypes.FldLvl) -> None
        fdx_item = self.fdx_items_by_rte_name["BrkFldLvl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BrkFldLvl(self):
        group = self.fdx_items_by_rte_name["BrkFldLvl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BrkFricTqAtWhlAct(self, data):
        # type: (datatypes.BrkFricTqAtWhlActRec) -> None
        group = self.fdx_items_by_rte_name["BrkFricTqAtWhlFrntLeAct"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BrkFricTqAtWhlFrntLeAct"], p2r_float(data.BrkFricTqAtWhlFrntLeAct, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["BrkFricTqAtWhlFrntRiAct"], p2r_float(data.BrkFricTqAtWhlFrntRiAct, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["BrkFricTqAtWhlReLeAct"], p2r_float(data.BrkFricTqAtWhlReLeAct, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["BrkFricTqAtWhlReRiAct"], p2r_float(data.BrkFricTqAtWhlReRiAct, 1.0, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BrkFricTqAtWhlAct(self):
        group = self.fdx_items_by_rte_name["BrkFricTqAtWhlFrntLeAct"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BrkRelsWarnReq(self, data):
        # type: (datatypes.NoYes1) -> None
        fdx_item = self.fdx_items_by_rte_name["BrkRelsWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BrkRelsWarnReq(self):
        group = self.fdx_items_by_rte_name["BrkRelsWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_Btn1ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["Btn1ForUsrSwtPanFrntReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_Btn1ForUsrSwtPanFrntReq(self):
        group = self.fdx_items_by_rte_name["Btn1ForUsrSwtPanFrntReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_Btn2ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["Btn2ForUsrSwtPanFrntReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_Btn2ForUsrSwtPanFrntReq(self):
        group = self.fdx_items_by_rte_name["Btn2ForUsrSwtPanFrntReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_Btn3ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["Btn3ForUsrSwtPanFrntReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_Btn3ForUsrSwtPanFrntReq(self):
        group = self.fdx_items_by_rte_name["Btn3ForUsrSwtPanFrntReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_Btn4ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["Btn4ForUsrSwtPanFrntReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_Btn4ForUsrSwtPanFrntReq(self):
        group = self.fdx_items_by_rte_name["Btn4ForUsrSwtPanFrntReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_Btn5ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["Btn5ForUsrSwtPanFrntReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_Btn5ForUsrSwtPanFrntReq(self):
        group = self.fdx_items_by_rte_name["Btn5ForUsrSwtPanFrntReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BtnAudFbSts(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["BtnAudFbSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BtnAudFbSts(self):
        group = self.fdx_items_by_rte_name["BtnAudFbSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BtnAudVolSts(self, data):
        # type: (datatypes.BtnAudVolSts) -> None
        fdx_item = self.fdx_items_by_rte_name["BtnAudVolSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BtnAudVolSts(self):
        group = self.fdx_items_by_rte_name["BtnAudVolSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_BtnIllmnForWinDefrstFrnt(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["BtnIllmnForWinDefrstFrnt"]
        return fdx_item.value_raw

    def send_BtnMmedLeRiSts(self, data):
        # type: (datatypes.BtnMmedLeRiSts) -> None
        fdx_item = self.fdx_items_by_rte_name["BtnMmedLeRiSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BtnMmedLeRiSts(self):
        group = self.fdx_items_by_rte_name["BtnMmedLeRiSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BtnMmedModSts(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["BtnMmedModSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BtnMmedModSts(self):
        group = self.fdx_items_by_rte_name["BtnMmedModSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BtnMmedSetSts(self, data):
        # type: (datatypes.BtnMmedSetSts) -> None
        fdx_item = self.fdx_items_by_rte_name["BtnMmedSetSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BtnMmedSetSts(self):
        group = self.fdx_items_by_rte_name["BtnMmedSetSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CCSMBtn6(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["CCSMBtn6"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CCSMBtn6(self):
        group = self.fdx_items_by_rte_name["CCSMBtn6"].parent_group
        self.remove_group_from_send_timer(group)

    def get_CallSts(self):
        # type: () -> datatypes.CallSts
        fdx_item = self.fdx_items_by_rte_name["CallStsIndcn"]
        return fdx_item.value_raw

    def send_CarTiGlb(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["CarTiGlb"]
        fdx_item.value_raw = p2r_float(data, 0.1, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CarTiGlb(self):
        group = self.fdx_items_by_rte_name["CarTiGlb"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CchForFuEco(self, data):
        # type: (datatypes.CchForFuEco1) -> None
        group = self.fdx_items_by_rte_name["BarForFuEco"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["BarForFuEco"], p2r_float(data.BarForFuEco, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["GrdForFuEco"], p2r_float(data.GrdForFuEco, 0.1, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CchForFuEco(self):
        group = self.fdx_items_by_rte_name["BarForFuEco"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ChdLockgProtnStsToHmi(self, data):
        # type: (datatypes.OnOffSafe1) -> None
        fdx_item = self.fdx_items_by_rte_name["ChdLockgProtnStsToHmi"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ChdLockgProtnStsToHmi(self):
        group = self.fdx_items_by_rte_name["ChdLockgProtnStsToHmi"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ChdWinProtnStsAtDrvrRe(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["ChdWinProtnStsAtDrvrRe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ChdWinProtnStsAtDrvrRe(self):
        group = self.fdx_items_by_rte_name["ChdWinProtnStsAtDrvrRe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ChdWinProtnStsAtPassRe(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["ChdWinProtnStsAtPassRe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ChdWinProtnStsAtPassRe(self):
        group = self.fdx_items_by_rte_name["ChdWinProtnStsAtPassRe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ChrgrHwAprvdWirelsAdr(self, data):
        # type: (datatypes.HwAprvdWirelsAdr1) -> None
        group = self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr1"], _get_if_not_None(data.HwOffsAdr1), group)
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr2"], _get_if_not_None(data.HwOffsAdr2), group)
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr3"], _get_if_not_None(data.HwOffsAdr3), group)
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr4"], _get_if_not_None(data.HwOffsAdr4), group)
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr5"], _get_if_not_None(data.HwOffsAdr5), group)
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr6"], _get_if_not_None(data.HwOffsAdr6), group)
            _ag(self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrAprvdSts"], _extract_enum_value(_get_if_not_None(data.AprvdSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ChrgrHwAprvdWirelsAdr(self):
        group = self.fdx_items_by_rte_name["ChrgrHwAprvdWirelsAdrHwOffsAdr1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ChrgrWirelsSts(self, data):
        # type: (datatypes.ChrgrWirelsSts) -> None
        fdx_item = self.fdx_items_by_rte_name["ChrgrWirelsSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ChrgrWirelsSts(self):
        group = self.fdx_items_by_rte_name["ChrgrWirelsSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ClimaActv(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["ClimaActv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ClimaActv(self):
        group = self.fdx_items_by_rte_name["ClimaActv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ClimaPwrCns(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["ClimaPwrCns"]
        fdx_item.value_raw = p2r_float(data, 1.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ClimaPwrCns(self):
        group = self.fdx_items_by_rte_name["ClimaPwrCns"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ClimaRqrdFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        fdx_item = self.fdx_items_by_rte_name["ClimaRqrd"]
        return fdx_item.value_raw

    def get_ClimaTmr(self):
        # type: () -> float
        fdx_item = self.fdx_items_by_rte_name["ClimaTmr"]
        return r2p_float(fdx_item.value_raw, 0.1, 0.0)

    def send_ClimaTmrSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["ClimaTmrSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ClimaTmrSts(self):
        group = self.fdx_items_by_rte_name["ClimaTmrSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ClimaTmrStsRqrd(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["ClimaTmrStsRqrd"]
        return fdx_item.value_raw

    def send_ClimaWarn(self, data):
        # type: (datatypes.ClimaWarn) -> None
        fdx_item = self.fdx_items_by_rte_name["ClimaWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ClimaWarn(self):
        group = self.fdx_items_by_rte_name["ClimaWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_CllsnAidSnvtySeld(self):
        # type: () -> datatypes.SnvtyPen1
        data = datatypes.SnvtyPen1()
        group = self.fdx_items_by_rte_name["CllsnAidSnvtySeldSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['CllsnAidSnvtySeldSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['CllsnAidSnvtySeldPen'], group)
        return data

    def get_CllsnFwdWarnActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["CllsnFwdWarnActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['CllsnFwdWarnActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['CllsnFwdWarnActvPen'], group)
        return data

    def send_CllsnFwdWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["CllsnFwdWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CllsnFwdWarnReq(self):
        group = self.fdx_items_by_rte_name["CllsnFwdWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CllsnWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["CllsnWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CllsnWarnReq(self):
        group = self.fdx_items_by_rte_name["CllsnWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CllsnWarnSide(self, data):
        # type: (datatypes.CllsnWarnSide1) -> None
        group = self.fdx_items_by_rte_name["CllsnWarnSideLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["CllsnWarnSideLe"], _extract_enum_value(_get_if_not_None(data.Le)), group)
            _ag(self.fdx_items_by_rte_name["CllsnWarnSideRi"], _extract_enum_value(_get_if_not_None(data.Ri)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CllsnWarnSide(self):
        group = self.fdx_items_by_rte_name["CllsnWarnSideLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ClngActv(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        fdx_item = self.fdx_items_by_rte_name["ClngActv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ClngActv(self):
        group = self.fdx_items_by_rte_name["ClngActv"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ClngRqrdFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        fdx_item = self.fdx_items_by_rte_name["ClngRqrdFromHmi"]
        return fdx_item.value_raw

    def get_ClsAutEna(self):
        # type: () -> datatypes.EnableDisableCoding
        fdx_item = self.fdx_items_by_rte_name["ClsAutEna"]
        return fdx_item.value_raw

    def get_CmftFctActv(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["CmftFctActv"]
        return fdx_item.value_raw

    def send_CmftFctSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["CmftFctSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CmftFctSts(self):
        group = self.fdx_items_by_rte_name["CmftFctSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CmptmtAirTEstimdExtd(self, data):
        # type: (datatypes.CmptmtAirTEstimdExtd) -> None
        group = self.fdx_items_by_rte_name["CmptmtAirTEstimdExtdComptmtT"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["CmptmtAirTEstimdExtdComptmtT"], p2r_float(data.ComptmtT, 0.1, -60.0), group)
            _ag(self.fdx_items_by_rte_name["CmptmtAirTEstimdExtdQlyFlg"], _extract_enum_value(_get_if_not_None(data.QlyFlg)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CmptmtAirTEstimdExtd(self):
        group = self.fdx_items_by_rte_name["CmptmtAirTEstimdExtdComptmtT"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CmptmtTFrnt(self, data):
        # type: (datatypes.CmptmtTFrnt) -> None
        group = self.fdx_items_by_rte_name["CmptmtTFrnt"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["CmptmtTFrnt"], p2r_float(data.CmptmtTFrnt, 0.1, -60.0), group)
            _ag(self.fdx_items_by_rte_name["CmptmtTFrntQf"], _extract_enum_value(_get_if_not_None(data.CmptmtTFrntQf)), group)
            _ag(self.fdx_items_by_rte_name["FanForCmptmtTRunng"], _extract_enum_value(_get_if_not_None(data.FanForCmptmtTRunng)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CmptmtTFrnt(self):
        group = self.fdx_items_by_rte_name["CmptmtTFrnt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CnclWarnForCrsCtrl(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        fdx_item = self.fdx_items_by_rte_name["CnclWarnForCrsCtrl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CnclWarnForCrsCtrl(self):
        group = self.fdx_items_by_rte_name["CnclWarnForCrsCtrl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CnclWarnLgtForAutDrv(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        fdx_item = self.fdx_items_by_rte_name["CnclWarnLgtForAutDrv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CnclWarnLgtForAutDrv(self):
        group = self.fdx_items_by_rte_name["CnclWarnLgtForAutDrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CnsPrm(self, data):
        # type: (datatypes.CnsPrmRec1) -> None
        group = self.fdx_items_by_rte_name["CnsPrmSpdVal"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["CnsPrmSpdVal"], p2r_float(data.SpdVal, 0.001, -0.4548), group)
            _ag(self.fdx_items_by_rte_name["CnsPrmSpdIdx"], _get_if_not_None(data.SpdIdx), group)
            _ag(self.fdx_items_by_rte_name["CnsPrmAuxPwrPrsnt"], p2r_float(data.AuxPwrPrsnt, 20.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["CnsPrmAuxPwrLvlInct"], p2r_float(data.AuxPwrLvlInct, 20.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["CnsPrmAuxTiPrsnt"], p2r_float(data.AuxTiPrsnt, 10.0, -1.0), group)
            _ag(self.fdx_items_by_rte_name["CnsPrmAuxTiTranPha"], p2r_float(data.AuxTiTranPha, 10.0, -1.0), group)
            _ag(self.fdx_items_by_rte_name["CnsPrmAVal"], p2r_float(data.AVal, 0.01, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CnsPrm(self):
        group = self.fdx_items_by_rte_name["CnsPrmSpdVal"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ComLostExtrSound(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["ComLostExtrSound"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ComLostExtrSound(self):
        group = self.fdx_items_by_rte_name["ComLostExtrSound"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ConSftyDataSharingAllwd(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["ConSftyDataSharingAllwd"]
        return fdx_item.value_raw

    def send_ConSftyDataSharingSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["ConSftyDataSharingSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ConSftyDataSharingSts(self):
        group = self.fdx_items_by_rte_name["ConSftyDataSharingSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ConSftyWarn(self, data):
        # type: (datatypes.ConSftyWarn1) -> None
        group = self.fdx_items_by_rte_name["ConSftyWarnConSftyWarnId"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ConSftyWarnConSftyWarnId"], _get_if_not_None(data.ConSftyWarnId), group)
            _ag(self.fdx_items_by_rte_name["ConSftyWarnConSftyWarnSnd"], _extract_enum_value(_get_if_not_None(data.ConSftyWarnSnd)), group)
            _ag(self.fdx_items_by_rte_name["ConSftyWarnConSftyWarnTyp"], _get_if_not_None(data.ConSftyWarnTyp), group)
            _ag(self.fdx_items_by_rte_name["ConSftyWarnConSftyWarnLvl"], _extract_enum_value(_get_if_not_None(data.ConSftyWarnLvl)), group)
            _ag(self.fdx_items_by_rte_name["ConSftyWarnDistanceToWarning"], _get_if_not_None(data.DistanceToWarning), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ConSftyWarn(self):
        group = self.fdx_items_by_rte_name["ConSftyWarnConSftyWarnId"].parent_group
        self.remove_group_from_send_timer(group)

    def get_CoolgReqForDispCen(self):
        # type: () -> datatypes.CoolgReqForDisp
        fdx_item = self.fdx_items_by_rte_name["CoolgReqForDispCen"]
        return fdx_item.value_raw

    def send_CoolgStsForDisp(self, data):
        # type: (datatypes.CoolgStsForDisp) -> None
        fdx_item = self.fdx_items_by_rte_name["CoolgStsForDisp"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CoolgStsForDisp(self):
        group = self.fdx_items_by_rte_name["CoolgStsForDisp"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CrsCtrlrSts(self, data):
        # type: (datatypes.CrsCtrlrSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["CrsCtrlrSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CrsCtrlrSts(self):
        group = self.fdx_items_by_rte_name["CrsCtrlrSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CrvtSpdAdpvSts(self, data):
        # type: (datatypes.OffOnNotAvlSrvRqrd) -> None
        fdx_item = self.fdx_items_by_rte_name["CrvtSpdAdpvSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CrvtSpdAdpvSts(self):
        group = self.fdx_items_by_rte_name["CrvtSpdAdpvSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_CrvtSpdEstimrSnvtySeld(self):
        # type: () -> datatypes.SnvtyPen3
        data = datatypes.SnvtyPen3()
        group = self.fdx_items_by_rte_name["TurnSpdWarnSnvtyActsSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['TurnSpdWarnSnvtyActsSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['TurnSpdWarnSnvtyActPen'], group)
        return data

    def get_CrvtSpdWarnActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["TurnSpdWarnActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['TurnSpdWarnActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['TurnSpdWarnActvPen'], group)
        return data

    def send_CrvtSpdWarnReq(self, data):
        # type: (datatypes.TurnSpdWarnReq1) -> None
        fdx_item = self.fdx_items_by_rte_name["CrvtSpdWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CrvtSpdWarnReq(self):
        group = self.fdx_items_by_rte_name["CrvtSpdWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CrvtSpdWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["CrvtSpdWarnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CrvtSpdWarnSts(self):
        group = self.fdx_items_by_rte_name["CrvtSpdWarnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CtraIndcnLe(self, data):
        # type: (datatypes.CtraIndcn1) -> None
        fdx_item = self.fdx_items_by_rte_name["CtraIndcnLe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CtraIndcnLe(self):
        group = self.fdx_items_by_rte_name["CtraIndcnLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CtraIndcnRi(self, data):
        # type: (datatypes.CtraIndcn1) -> None
        fdx_item = self.fdx_items_by_rte_name["CtraIndcnRi"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CtraIndcnRi(self):
        group = self.fdx_items_by_rte_name["CtraIndcnRi"].parent_group
        self.remove_group_from_send_timer(group)

    def send_CtraOn1(self, data):
        # type: (datatypes.LcmaCtraOn) -> None
        fdx_item = self.fdx_items_by_rte_name["CtraOn1"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_CtraOn1(self):
        group = self.fdx_items_by_rte_name["CtraOn1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_CtraSwOnOff(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["CtraSwOnOff"]
        return fdx_item.value_raw

    def get_CurtActvnReReq(self):
        # type: () -> datatypes.CurtActvnTyp1
        fdx_item = self.fdx_items_by_rte_name["CurtActvnReReq"]
        return fdx_item.value_raw

    def get_DataDistbnAllwdForEgyPred(self):
        # type: () -> datatypes.OnOffNoReq
        fdx_item = self.fdx_items_by_rte_name["DataDistbnAllwdForEgyPred"]
        return fdx_item.value_raw

    def send_DataDistbnReqForEgyPred(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        fdx_item = self.fdx_items_by_rte_name["DataDistbnReqForEgyPred"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DataDistbnReqForEgyPred(self):
        group = self.fdx_items_by_rte_name["DataDistbnReqForEgyPred"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DataSpclDMSM(self, data):
        # type: (datatypes.DataSpclTyp) -> None
        group = self.fdx_items_by_rte_name["DataSpclDMSMDataIdn"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["DataSpclDMSMDataIdn"], _get_if_not_None(data.DataIdn), group)
            _ag(self.fdx_items_by_rte_name["DataSpclDMSMDataNrSpcl1"], _get_if_not_None(data.DataNrSpcl1), group)
            _ag(self.fdx_items_by_rte_name["DataSpclDMSMDataNrSpcl2"], _get_if_not_None(data.DataNrSpcl2), group)
            _ag(self.fdx_items_by_rte_name["DataSpclDMSMDataNrSpcl3"], _get_if_not_None(data.DataNrSpcl3), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DataSpclDMSM(self):
        group = self.fdx_items_by_rte_name["DataSpclDMSMDataIdn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DayToSrv(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DayToSrv"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DayToSrv(self):
        group = self.fdx_items_by_rte_name["DayToSrv"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DeactvtLvlgCtrl(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["DeactvtLvlgCtrl"]
        return fdx_item.value_raw

    def get_DefrstDurgClimaEnad(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["None"]
        return fdx_item.value_raw

    def send_DestSrcAndCfmRqrd(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DestSrcAndCfmRqrd"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DestSrcAndCfmRqrd(self):
        group = self.fdx_items_by_rte_name["DestSrcAndCfmRqrd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DiagcCCSM(self, data):
        # type: (datatypes.DiagcForPanCenCtrl2) -> None
        fdx_item = self.fdx_items_by_rte_name["DiagcCCSM"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DiagcCCSM(self):
        group = self.fdx_items_by_rte_name["DiagcCCSM"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DiagcRCSM(self, data):
        # type: (datatypes.DiagcForRCSM2) -> None
        fdx_item = self.fdx_items_by_rte_name["DiagcRCSM"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DiagcRCSM(self):
        group = self.fdx_items_by_rte_name["DiagcRCSM"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DiagcRSHC(self, data):
        # type: (datatypes.DiagcForRCSM2) -> None
        fdx_item = self.fdx_items_by_rte_name["DiagcRSHC"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DiagcRSHC(self):
        group = self.fdx_items_by_rte_name["DiagcRSHC"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DiagcStsDMSM(self, data):
        # type: (datatypes.DiagcStsTypDMSM) -> None
        fdx_item = self.fdx_items_by_rte_name["DiagcStsDMSM"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DiagcStsDMSM(self):
        group = self.fdx_items_by_rte_name["DiagcStsDMSM"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DispAndHomeBtnSts(self):
        # type: () -> datatypes.Trig1
        fdx_item = self.fdx_items_by_rte_name["DispAndHomeBtnSts"]
        return fdx_item.value_raw

    def send_DispBattEgyIn(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["DispBattEgyIn"]
        fdx_item.value_raw = p2r_float(data, 0.5, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DispBattEgyIn(self):
        group = self.fdx_items_by_rte_name["DispBattEgyIn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DispBattEgyOut(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["DispBattEgyOut"]
        fdx_item.value_raw = p2r_float(data, 0.5, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DispBattEgyOut(self):
        group = self.fdx_items_by_rte_name["DispBattEgyOut"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DispOfPrpsnMod(self, data):
        # type: (datatypes.DispOfPrpsnMod3) -> None
        fdx_item = self.fdx_items_by_rte_name["DispOfPrpsnMod"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DispOfPrpsnMod(self):
        group = self.fdx_items_by_rte_name["DispOfPrpsnMod"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DispStsCen(self):
        # type: () -> datatypes.FltCfmd1
        fdx_item = self.fdx_items_by_rte_name["DispStsCen"]
        return fdx_item.value_raw

    def send_DoorDrvrLockReSts(self, data):
        # type: (datatypes.LockSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorDrvrLockReSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorDrvrLockReSts(self):
        group = self.fdx_items_by_rte_name["DoorDrvrLockReSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorDrvrLockSts(self, data):
        # type: (datatypes.LockSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorDrvrLockSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorDrvrLockSts(self):
        group = self.fdx_items_by_rte_name["DoorDrvrLockSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorDrvrReSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorDrvrReSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorDrvrReSts(self):
        group = self.fdx_items_by_rte_name["DoorDrvrReSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorDrvrSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorDrvrSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorDrvrSts(self):
        group = self.fdx_items_by_rte_name["DoorDrvrSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorPassLockReSts(self, data):
        # type: (datatypes.LockSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorPassLockReSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorPassLockReSts(self):
        group = self.fdx_items_by_rte_name["DoorPassLockReSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorPassLockSts(self, data):
        # type: (datatypes.LockSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorPassLockSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorPassLockSts(self):
        group = self.fdx_items_by_rte_name["DoorPassLockSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorPassReSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorPassReSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorPassReSts(self):
        group = self.fdx_items_by_rte_name["DoorPassReSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DoorPassSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DoorPassSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DoorPassSts(self):
        group = self.fdx_items_by_rte_name["DoorPassSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DriveAwayInfoActvReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["DriveAwayInfoActvReq"]
        return fdx_item.value_raw

    def send_DriveAwayInfoActvSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DriveAwayInfoActvSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DriveAwayInfoActvSts(self):
        group = self.fdx_items_by_rte_name["DriveAwayInfoActvSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DriveAwayInfoSoundWarnActvReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["DriveAwayInfoSoundWarnActvReq"]
        return fdx_item.value_raw

    def send_DriveAwayInfoWarnReq(self, data):
        # type: (datatypes.DriveAwayInfoWarnReq1) -> None
        group = self.fdx_items_by_rte_name["DriveAwayInfoWarnReqReqSrc"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["DriveAwayInfoWarnReqReqSrc"], _extract_enum_value(_get_if_not_None(data.ReqSrc)), group)
            _ag(self.fdx_items_by_rte_name["DriveAwayInfoWarnReqTiToDrvCntDwnTi"], _get_if_not_None(data.TiToDrvCntDwnTi), group)
            _ag(self.fdx_items_by_rte_name["DriveAwayInfoWarnReqCtrlDirOfTrfcLi"], _extract_enum_value(_get_if_not_None(data.CtrlDirOfTrfcLi)), group)
            _ag(self.fdx_items_by_rte_name["DriveAwayInfoWarnReqVisWarnReq"], _extract_enum_value(_get_if_not_None(data.VisWarnReq)), group)
            _ag(self.fdx_items_by_rte_name["DriveAwayInfoWarnReqAudWarnReq"], _extract_enum_value(_get_if_not_None(data.AudWarnReq)), group)
            _ag(self.fdx_items_by_rte_name["DriveAwayInfoWarnReqInterruptionSrc"], _extract_enum_value(_get_if_not_None(data.InterruptionSrc)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DriveAwayInfoWarnReq(self):
        group = self.fdx_items_by_rte_name["DriveAwayInfoWarnReqReqSrc"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DrvModDispd(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["DrvModDispd"]
        return fdx_item.value_raw

    def get_DrvModReq(self):
        # type: () -> datatypes.DrvModReqType1
        fdx_item = self.fdx_items_by_rte_name["DrvModReq"]
        return fdx_item.value_raw

    def get_DrvgInWrgDirOfTrfc(self):
        # type: () -> datatypes.NoYes1
        fdx_item = self.fdx_items_by_rte_name["DrvgInWrgDirOfTrfc"]
        return fdx_item.value_raw

    def get_DrvrCtrlOfPassSeatFrntReqd(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["DrvrCtrlOfPassSeatFrntReqd"]
        return fdx_item.value_raw

    def send_DrvrCtrlOfPassSeatFrntSts(self, data):
        # type: (datatypes.ActvnAvl1) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrCtrlOfPassSeatFrntSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrCtrlOfPassSeatFrntSts(self):
        group = self.fdx_items_by_rte_name["DrvrCtrlOfPassSeatFrntSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DrvrDesDir(self, data):
        # type: (datatypes.DrvrDesDir1) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrDesDir"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrDesDir(self):
        group = self.fdx_items_by_rte_name["DrvrDesDir"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DrvrDispSetg(self):
        # type: () -> datatypes.DrvrDispSetgReq
        data = datatypes.DrvrDispSetgReq()
        group = self.fdx_items_by_rte_name["DrvrDispSetgPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name['DrvrDispSetgPen'], group)
            data.Sts = _gg(self.fdx_items_by_rte_name['DrvrDispSetgSts'], group)
        return data

    def get_DrvrEntryLoReq(self):
        # type: () -> datatypes.NoYes1
        fdx_item = self.fdx_items_by_rte_name["DrvrEntryLoReq"]
        return fdx_item.value_raw

    def get_DrvrHmiBackGndForHmiCen(self):
        # type: () -> datatypes.DrvrHmiForHmiCen
        fdx_item = self.fdx_items_by_rte_name["DrvrHmiBackGndForHmiCen"]
        return fdx_item.value_raw

    def get_DrvrHmiBackGndInfoSetg(self):
        # type: () -> datatypes.DrvrHmiBackGndInfoSetg
        data = datatypes.DrvrHmiBackGndInfoSetg()
        group = self.fdx_items_by_rte_name["DrvrHmiBackGndInfoSetgPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name['DrvrHmiBackGndInfoSetgPen'], group)
            data.Setg = _gg(self.fdx_items_by_rte_name['DrvrHmiBackGndInfoSetgSetg'], group)
        return data

    def get_DrvrHmiDispdMod(self):
        # type: () -> datatypes.DrvrHmiDispdModPen
        data = datatypes.DrvrHmiDispdModPen()
        group = self.fdx_items_by_rte_name["DrvrHmiDispdModSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['DrvrHmiDispdModSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['DrvrHmiDispdModPen'], group)
        return data

    def get_DrvrHmiForHmiCen(self):
        # type: () -> datatypes.DrvrHmiForHmiCen
        fdx_item = self.fdx_items_by_rte_name["DrvrHmiForHmiCen"]
        return fdx_item.value_raw

    def send_DrvrHmiSpdLimAdpnSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrHmiSpdLimAdpnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrHmiSpdLimAdpnSts(self):
        group = self.fdx_items_by_rte_name["DrvrHmiSpdLimAdpnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DrvrHmiTmrForHmiCen(self):
        # type: () -> datatypes.Array8ByteU8
        fdx_item = self.fdx_items_by_rte_name["DrvrHmiTmrForHmiCen"]
        return fdx_item.value_raw

    def get_DrvrHmiUsrIfSetg(self):
        # type: () -> datatypes.DrvrHmiUserIfSetgReq
        data = datatypes.DrvrHmiUserIfSetgReq()
        group = self.fdx_items_by_rte_name["DrvrHmiUsrIfSetgPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name['DrvrHmiUsrIfSetgPen'], group)
            data.Setg = _gg(self.fdx_items_by_rte_name['DrvrHmiUsrIfSetgSetg'], group)
        return data

    def send_DrvrMassgRunng(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrMassgRunng"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrMassgRunng(self):
        group = self.fdx_items_by_rte_name["DrvrMassgRunng"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DrvrPfmncLvl(self, data):
        # type: (datatypes.DrvrPfmncLvl2) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrPfmncLvl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrPfmncLvl(self):
        group = self.fdx_items_by_rte_name["DrvrPfmncLvl"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DrvrPfmncMonActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["DrvrPfmncMonActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['DrvrPfmncMonActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['DrvrPfmncMonActvPen'], group)
        return data

    def send_DrvrPfmncSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrPfmncSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrPfmncSts(self):
        group = self.fdx_items_by_rte_name["DrvrPfmncSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DrvrPfmncWarnReq(self, data):
        # type: (datatypes.DrvrPfmncWarnReq1) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrPfmncWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrPfmncWarnReq(self):
        group = self.fdx_items_by_rte_name["DrvrPfmncWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DrvrPfmncWarnReqToNav(self):
        # type: () -> datatypes.DrvrPfmncWarnReq1
        fdx_item = self.fdx_items_by_rte_name["None"]
        return fdx_item.value_raw

    def send_DrvrSeatActvSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrSeatActvSpplFct"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrSeatActvSpplFct(self):
        group = self.fdx_items_by_rte_name["DrvrSeatActvSpplFct"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DrvrSeatDispMassgFct(self):
        # type: () -> datatypes.SeatMassgFct
        data = datatypes.SeatMassgFct()
        group = self.fdx_items_by_rte_name["DrvrSeatDispMassgFctOnOff"].parent_group #should be same in all
        with group.mutex_lock:
            data.OnOff = _gg(self.fdx_items_by_rte_name['DrvrSeatDispMassgFctOnOff'], group)
            data.MassgProg = _gg(self.fdx_items_by_rte_name['DrvrSeatDispMassgFctMassgProg'], group)
            data.MassgInten = _gg(self.fdx_items_by_rte_name['DrvrSeatDispMassgFctMassgInten'], group)
            data.MassgSpdLvl = _gg(self.fdx_items_by_rte_name['DrvrSeatDispMassgFctMassgSpdLvl'], group)
        return data

    def get_DrvrSeatDispSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        fdx_item = self.fdx_items_by_rte_name["DrvrSeatDispSpplFct"]
        return fdx_item.value_raw

    def send_DrvrSeatMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        group = self.fdx_items_by_rte_name["DrvrSeatMassgFctOnOff"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["DrvrSeatMassgFctOnOff"], _extract_enum_value(_get_if_not_None(data.OnOff)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatMassgFctMassgProg"], _extract_enum_value(_get_if_not_None(data.MassgProg)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatMassgFctMassgInten"], _extract_enum_value(_get_if_not_None(data.MassgInten)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatMassgFctMassgSpdLvl"], _extract_enum_value(_get_if_not_None(data.MassgSpdLvl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrSeatMassgFct(self):
        group = self.fdx_items_by_rte_name["DrvrSeatMassgFctOnOff"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DrvrSeatSwtSts(self, data):
        # type: (datatypes.DrvrSeatSwtSts1) -> None
        group = self.fdx_items_by_rte_name["DrvrSeatSwtSldSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtSldSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtSldSts)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtHeiSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtHeiSts)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtHeiFrntSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtHeiFrntSts)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtInclSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtInclSts)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtAdjmtOfSpplFctHozlSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtAdjmtOfSpplFctHozlSts)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtAdjmtOfSpplFctVertSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtAdjmtOfSpplFctVertSts)), group)
            _ag(self.fdx_items_by_rte_name["DrvrSeatSwtSelnOfSpplFctSts"], _extract_enum_value(_get_if_not_None(data.DrvrSeatSwtSelnOfSpplFctSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrSeatSwtSts(self):
        group = self.fdx_items_by_rte_name["DrvrSeatSwtSldSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DrvrWLoadLvl(self, data):
        # type: (datatypes.Lvl30) -> None
        fdx_item = self.fdx_items_by_rte_name["DrvrWLoadLvl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DrvrWLoadLvl(self):
        group = self.fdx_items_by_rte_name["DrvrWLoadLvl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstEstimdToEmptyForDrvgElec(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DstEstimdToEmptyForDrvgElec"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstEstimdToEmptyForDrvgElec(self):
        group = self.fdx_items_by_rte_name["DstEstimdToEmptyForDrvgElec"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstEstimdToEmptyForDrvgElecEco(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DstEstimdToEmptyForDrvgElecEco"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstEstimdToEmptyForDrvgElecEco(self):
        group = self.fdx_items_by_rte_name["DstEstimdToEmptyForDrvgElecEco"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DstNotifActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["DstNotifActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['DstNotifActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['DstNotifActvPen'], group)
        return data

    def send_DstNotifSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["DstNotifSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstNotifSts(self):
        group = self.fdx_items_by_rte_name["DstNotifSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstToEmptyIndcd(self, data):
        # type: (datatypes.DstToEmptyWithUnit) -> None
        group = self.fdx_items_by_rte_name["DstToEmpty"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["DstToEmpty"], _get_if_not_None(data.DstToEmpty), group)
            _ag(self.fdx_items_by_rte_name["DstUnit"], _extract_enum_value(_get_if_not_None(data.DstUnit)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstToEmptyIndcd(self):
        group = self.fdx_items_by_rte_name["DstToEmpty"].parent_group
        self.remove_group_from_send_timer(group)

    def get_DstToManvLocn(self):
        # type: () -> datatypes.DstToManvLocnByNav
        data = datatypes.DstToManvLocnByNav()
        group = self.fdx_items_by_rte_name["DstToManvInDstToManvLocn"].parent_group #should be same in all
        with group.mutex_lock:
            data.DstToManv = _gg(self.fdx_items_by_rte_name['DstToManvInDstToManvLocn'], group)
            data.CntDwnToManvStrt = _gg(self.fdx_items_by_rte_name['CntDwnToManvStrtInDstToManvLocn'], group)
            data.PosnQly = _gg(self.fdx_items_by_rte_name['PosnQlyInDstToManvLocn'], group)
            data.SpprtForFct = _gg(self.fdx_items_by_rte_name['SpprtForFctInDstToManvLocn'], group)
        return data

    def send_DstToSrv(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DstToSrv"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstToSrv(self):
        group = self.fdx_items_by_rte_name["DstToSrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstTrvld1(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["DstTrvld1"]
        fdx_item.value_raw = p2r_float(data, 0.1, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstTrvld1(self):
        group = self.fdx_items_by_rte_name["DstTrvld1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstTrvld2(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["DstTrvld2"]
        fdx_item.value_raw = p2r_float(data, 0.1, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstTrvld2(self):
        group = self.fdx_items_by_rte_name["DstTrvld2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstTrvldHiResl(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DstTrvldHiResl"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstTrvldHiResl(self):
        group = self.fdx_items_by_rte_name["DstTrvldHiResl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_BkpOfDstTrvld(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["BkpOfDstTrvld"]
        fdx_item.value_raw = p2r_float(data, 1.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_BkpOfDstTrvld(self):
        group = self.fdx_items_by_rte_name["BkpOfDstTrvld"].parent_group
        self.remove_group_from_send_timer(group)

    def send_DstTrvldMst2(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["DstTrvldMst2"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_DstTrvldMst2(self):
        group = self.fdx_items_by_rte_name["DstTrvldMst2"].parent_group
        self.remove_group_from_send_timer(group)

    def get_EasyInOutDrvrSeatAdjmt(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["EasyInOutDrvrSeatAdjmtSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['EasyInOutDrvrSeatAdjmtSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['EasyInOutDrvrSeatAdjmtPen'], group)
        return data

    def send_EgyCostForRoute(self, data):
        # type: (datatypes.EgyCostForRouteRec1) -> None
        group = self.fdx_items_by_rte_name["EgyCostForRouteEgy"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["EgyCostForRouteEgy"], p2r_float(data.Egy, 50.0, -500.0), group)
            _ag(self.fdx_items_by_rte_name["EgyCostForRouteIdx"], _get_if_not_None(data.Idx), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EgyCostForRoute(self):
        group = self.fdx_items_by_rte_name["EgyCostForRouteEgy"].parent_group
        self.remove_group_from_send_timer(group)

    def get_EgySave(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["EgySave"]
        return fdx_item.value_raw

    def send_EmgyAsscSts(self, data):
        # type: (datatypes.AsscSts) -> None
        fdx_item = self.fdx_items_by_rte_name["EmgyAsscSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EmgyAsscSts(self):
        group = self.fdx_items_by_rte_name["EmgyAsscSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EmgyAsscStsConnGen2(self, data):
        # type: (datatypes.AsscSts) -> None
        fdx_item = self.fdx_items_by_rte_name["EmgyAsscStsConnGen2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EmgyAsscStsConnGen2(self):
        group = self.fdx_items_by_rte_name["EmgyAsscStsConnGen2"].parent_group
        self.remove_group_from_send_timer(group)

    def get_EmgyVehWarnActv(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["EmgyVehWarnActv"]
        return fdx_item.value_raw

    def send_EmgyVehWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["EmgyVehWarnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EmgyVehWarnSts(self):
        group = self.fdx_items_by_rte_name["EmgyVehWarnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngCooltIndcnReq(self, data):
        # type: (datatypes.EngCooltIndcn) -> None
        fdx_item = self.fdx_items_by_rte_name["EngCooltIndcnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngCooltIndcnReq(self):
        group = self.fdx_items_by_rte_name["EngCooltIndcnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngCooltLvl(self, data):
        # type: (datatypes.FldLvl) -> None
        fdx_item = self.fdx_items_by_rte_name["EngCooltLvl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngCooltLvl(self):
        group = self.fdx_items_by_rte_name["EngCooltLvl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngFuCns(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["EngFuCns"]
        fdx_item.value_raw = p2r_float(data, 200.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngFuCns(self):
        group = self.fdx_items_by_rte_name["EngFuCns"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngFuCnsFild(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["EngFuCnsFild"]
        fdx_item.value_raw = p2r_float(data, 200.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngFuCnsFild(self):
        group = self.fdx_items_by_rte_name["EngFuCnsFild"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngHrToSrv(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["EngHrToSrv"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngHrToSrv(self):
        group = self.fdx_items_by_rte_name["EngHrToSrv"].parent_group
        self.remove_group_from_send_timer(group)

    def get_EngIdleEco1(self):
        # type: () -> datatypes.EngIdleEco1
        data = datatypes.EngIdleEco1()
        group = self.fdx_items_by_rte_name["EngIdleEco1Sts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['EngIdleEco1Sts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['EngIdleEco1Pen'], group)
        return data

    def send_EngNSafe(self, data):
        # type: (datatypes.EngNSafe) -> None
        group = self.fdx_items_by_rte_name["EngN"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["EngN"], p2r_float(data.EngN, 0.5, 0.0), group)
            _ag(self.fdx_items_by_rte_name["EngNSafeEngNGrdt"], p2r_float(data.EngNGrdt, 1.0, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngNSafe(self):
        group = self.fdx_items_by_rte_name["EngN"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngOilLvl(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["EngOilLvl"]
        fdx_item.value_raw = p2r_float(data, 0.5, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngOilLvl(self):
        group = self.fdx_items_by_rte_name["EngOilLvl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngOilLvlSts(self, data):
        # type: (datatypes.EngOilLvlSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["EngOilLvlSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngOilLvlSts(self):
        group = self.fdx_items_by_rte_name["EngOilLvlSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngOilPWarn(self, data):
        # type: (datatypes.EngOilPWarn1) -> None
        fdx_item = self.fdx_items_by_rte_name["EngOilPWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngOilPWarn(self):
        group = self.fdx_items_by_rte_name["EngOilPWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EngSpdDispd(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["EngSpdDispd"]
        fdx_item.value_raw = p2r_float(data, 0.5, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EngSpdDispd(self):
        group = self.fdx_items_by_rte_name["EngSpdDispd"].parent_group
        self.remove_group_from_send_timer(group)

    def get_EngStrtStopDrvModIndSetg(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["EngStrtStopDrvModIndSetgSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['EngStrtStopDrvModIndSetgSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['EngStrtStopDrvModIndSetgPen'], group)
        return data

    def get_EngStrtStopSetg(self):
        # type: () -> datatypes.OnOff2
        fdx_item = self.fdx_items_by_rte_name["EngStrtStopSetg"]
        return fdx_item.value_raw

    def get_EpbApplyAutSetg(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["EpbApplyAutSetgSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['EpbApplyAutSetgSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['EpbApplyAutSetgPen'], group)
        return data

    def get_EpbEngStallApplySetg(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["EpbEngStallApplySetgSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['EpbEngStallApplySetgSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['EpbEngStallApplySetgPen'], group)
        return data

    def send_EpbLampReq(self, data):
        # type: (datatypes.EpbLampReqRec) -> None
        group = self.fdx_items_by_rte_name["EpbLampReq"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["EpbLampReq"], _extract_enum_value(_get_if_not_None(data.EpbLampReq)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EpbLampReq(self):
        group = self.fdx_items_by_rte_name["EpbLampReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_EscSptModReqdByDrvr(self):
        # type: () -> datatypes.EscSptModReqdByDrvrRec1
        data = datatypes.EscSptModReqdByDrvrRec1()
        group = self.fdx_items_by_rte_name["EscSptModReqdByDrvr"].parent_group #should be same in all
        with group.mutex_lock:
            data.EscSptModReqdByDrvr = _gg(self.fdx_items_by_rte_name['EscSptModReqdByDrvr'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['EscSptModReqdByDrvrPen'], group)
        return data

    def send_EscSt(self, data):
        # type: (datatypes.EscStSafe1) -> None
        group = self.fdx_items_by_rte_name["EscSt"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["EscSt"], _extract_enum_value(_get_if_not_None(data.EscSt)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EscSt(self):
        group = self.fdx_items_by_rte_name["EscSt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_EscWarnIndcnReq(self, data):
        # type: (datatypes.EscWarnIndcnReqRec1) -> None
        group = self.fdx_items_by_rte_name["EscWarnIndcnReq"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["EscWarnIndcnReq"], _extract_enum_value(_get_if_not_None(data.EscWarnIndcnReq)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_EscWarnIndcnReq(self):
        group = self.fdx_items_by_rte_name["EscWarnIndcnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ExtrMirrFoldSetg(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["ExtrMirrFoldSetgSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['ExtrMirrFoldSetgSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['ExtrMirrFoldSetgPen'], group)
        return data

    def get_ExtrMirrTiltSetg2(self):
        # type: () -> datatypes.ExtrMirrTiltSetg
        data = datatypes.ExtrMirrTiltSetg()
        group = self.fdx_items_by_rte_name["ExtrMirrTiltSetg2MirrDrvr"].parent_group #should be same in all
        with group.mutex_lock:
            data.MirrDrvr = _gg(self.fdx_items_by_rte_name['ExtrMirrTiltSetg2MirrDrvr'], group)
            data.MirrPass = _gg(self.fdx_items_by_rte_name['ExtrMirrTiltSetg2MirrPass'], group)
            data.IdPen = _gg(self.fdx_items_by_rte_name['ExtrMirrTiltSetg2IdPen'], group)
        return data

    def get_ExtrMirrTintgSetg(self):
        # type: () -> datatypes.MirrDimPen
        data = datatypes.MirrDimPen()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.MirrDimLvl = _gg(self.fdx_items_by_rte_name[''], group)
            data.Pen = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def send_FRNetworkStatus(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["FRNetworkStatus"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FRNetworkStatus(self):
        group = self.fdx_items_by_rte_name["FRNetworkStatus"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FltIndcrTurnLeFrnt(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["FltIndcrTurnLeFrnt"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FltIndcrTurnLeFrnt(self):
        group = self.fdx_items_by_rte_name["FltIndcrTurnLeFrnt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FltIndcrTurnLeRe(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["FltIndcrTurnLeRe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FltIndcrTurnLeRe(self):
        group = self.fdx_items_by_rte_name["FltIndcrTurnLeRe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FltIndcrTurnRiFrnt(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["FltIndcrTurnRiFrnt"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FltIndcrTurnRiFrnt(self):
        group = self.fdx_items_by_rte_name["FltIndcrTurnRiFrnt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FltIndcrTurnRiRe(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["FltIndcrTurnRiRe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FltIndcrTurnRiRe(self):
        group = self.fdx_items_by_rte_name["FltIndcrTurnRiRe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FltOfLiDaytiRunngRi(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["FltOfLiDaytiRunngRi"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FltOfLiDaytiRunngRi(self):
        group = self.fdx_items_by_rte_name["FltOfLiDaytiRunngRi"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FrntAxleWarn(self, data):
        # type: (datatypes.AxleWarn) -> None
        fdx_item = self.fdx_items_by_rte_name["FrntAxleWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FrntAxleWarn(self):
        group = self.fdx_items_by_rte_name["FrntAxleWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FrntWiprLvrReq2(self, data):
        # type: (datatypes.FrntWiprLvrReq2) -> None
        group = self.fdx_items_by_rte_name["FrntWiprLvrReq2FrntWiprLvrCmd1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["FrntWiprLvrReq2FrntWiprLvrCmd1"], _extract_enum_value(_get_if_not_None(data.FrntWiprLvrCmd1)), group)
            _ag(self.fdx_items_by_rte_name["FrntWiprLvrReq2FrntWiprLvrQf"], _extract_enum_value(_get_if_not_None(data.FrntWiprLvrQf)), group)
            _ag(self.fdx_items_by_rte_name["FrntWiprLvrReq2FrntWiprLvrCrc"], _get_if_not_None(data.FrntWiprLvrCrc), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FrntWiprLvrReq2(self):
        group = self.fdx_items_by_rte_name["FrntWiprLvrReq2FrntWiprLvrCmd1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FuHeatrActv(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["FuHeatrActv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FuHeatrActv(self):
        group = self.fdx_items_by_rte_name["FuHeatrActv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FuHeatrFuCns1(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["FuHeatrFuCns1"]
        fdx_item.value_raw = p2r_float(data, 200.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FuHeatrFuCns1(self):
        group = self.fdx_items_by_rte_name["FuHeatrFuCns1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FuHeatrFuCnsDurgCyc1(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["FuHeatrFuCnsDurgCyc1"]
        fdx_item.value_raw = p2r_float(data, 0.01, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FuHeatrFuCnsDurgCyc1(self):
        group = self.fdx_items_by_rte_name["FuHeatrFuCnsDurgCyc1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FuLvlIndcd(self, data):
        # type: (datatypes.FuLvlValWithQly) -> None
        group = self.fdx_items_by_rte_name["FuLvlIndcdVal"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["FuLvlIndcdVal"], p2r_float(data.FuLvlValFromFuTbl, 0.2, 0.0), group)
            _ag(self.fdx_items_by_rte_name["FuLvlIndcdQly"], _extract_enum_value(_get_if_not_None(data.GenQF)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FuLvlIndcd(self):
        group = self.fdx_items_by_rte_name["FuLvlIndcdVal"].parent_group
        self.remove_group_from_send_timer(group)

    def send_FuLvlLoIndcn(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["FuLvlLoIndcn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FuLvlLoIndcn(self):
        group = self.fdx_items_by_rte_name["FuLvlLoIndcn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_FuLvlLoIndcnToNav(self):
        # type: () -> datatypes.FuLvlLoIndcnToNav1
        fdx_item = self.fdx_items_by_rte_name["None"]
        return fdx_item.value_raw

    def send_FuLvlLoWarn(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["FuLvlLoWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_FuLvlLoWarn(self):
        group = self.fdx_items_by_rte_name["FuLvlLoWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_GearIndcnRec(self, data):
        # type: (datatypes.GearIndcnRec2) -> None
        group = self.fdx_items_by_rte_name["GearIndcn"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["GearIndcn"], _extract_enum_value(_get_if_not_None(data.GearIndcn)), group)
            _ag(self.fdx_items_by_rte_name["GearShiftIndcn"], _extract_enum_value(_get_if_not_None(data.GearShiftIndcn)), group)
            _ag(self.fdx_items_by_rte_name["GearTarIndcn"], _extract_enum_value(_get_if_not_None(data.GearTarIndcn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_GearIndcnRec(self):
        group = self.fdx_items_by_rte_name["GearIndcn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_GlbRstForSetgAndData(self):
        # type: () -> datatypes.GlbRstForSetgAndData
        data = datatypes.GlbRstForSetgAndData()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.GlbRst = _gg(self.fdx_items_by_rte_name[''], group)
            data.Pen = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def get_HdHwAprvdWirelsAdr(self):
        # type: () -> datatypes.HwAprvdWirelsAdr1
        data = datatypes.HwAprvdWirelsAdr1()
        group = self.fdx_items_by_rte_name["HdHwAprvdWirelsAdrHwOffsAdr1"].parent_group #should be same in all
        with group.mutex_lock:
            data.HwOffsAdr1 = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrHwOffsAdr1'], group)
            data.HwOffsAdr2 = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrHwOffsAdr2'], group)
            data.HwOffsAdr3 = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrHwOffsAdr3'], group)
            data.HwOffsAdr4 = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrHwOffsAdr4'], group)
            data.HwOffsAdr5 = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrHwOffsAdr5'], group)
            data.HwOffsAdr6 = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrHwOffsAdr6'], group)
            data.AprvdSts = _gg(self.fdx_items_by_rte_name['HdHwAprvdWirelsAdrAprvdSts'], group)
        return data

    def get_HdrestFoldReq2(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["HdrestFoldReq2"]
        return fdx_item.value_raw

    def get_HeatrDurgDrvgReqd(self):
        # type: () -> datatypes.OffOnAut1
        fdx_item = self.fdx_items_by_rte_name["HeatrDurgDrvgReqd"]
        return fdx_item.value_raw

    def get_HeatrPreCdngTyp(self):
        # type: () -> datatypes.HeatrPreCdngTyp
        fdx_item = self.fdx_items_by_rte_name["HeatrPreCdngTyp"]
        return fdx_item.value_raw

    def get_HmiAudSts(self):
        # type: () -> datatypes.FltCfmd1
        fdx_item = self.fdx_items_by_rte_name["HmiAudSts"]
        return fdx_item.value_raw

    def send_HmiCenForDrvrHmi(self, data):
        # type: (datatypes.HmiCenForDrvrHmi) -> None
        fdx_item = self.fdx_items_by_rte_name["HmiCenForDrvrHmi"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HmiCenForDrvrHmi(self):
        group = self.fdx_items_by_rte_name["HmiCenForDrvrHmi"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HmiCmptmtAirDistbnFrnt(self):
        # type: () -> datatypes.HmiCmptmtAirDistbnFrnt
        fdx_item = self.fdx_items_by_rte_name["HmiCmptmtAirDistbnFrnt"]
        return fdx_item.value_raw

    def get_HmiCmptmtCoolgReq(self):
        # type: () -> datatypes.HmiCmptmtCoolgReq
        fdx_item = self.fdx_items_by_rte_name["HmiCmptmtCoolgReq"]
        return fdx_item.value_raw

    def get_HmiCmptmtTSp(self):
        # type: () -> datatypes.HmiCmptmtTSp
        data = datatypes.HmiCmptmtTSp()
        group = self.fdx_items_by_rte_name["HmiCmptmtTSpForRowFirstLe"].parent_group #should be same in all
        with group.mutex_lock:
            data.HmiCmptmtTSpForRowFirstLe = r2p_float(_gg(self.fdx_items_by_rte_name['HmiCmptmtTSpForRowFirstLe'], group), 0.5, 15.0)
            data.HmiCmptmtTSpSpclForRowFirstLe = _gg(self.fdx_items_by_rte_name['HmiCmptmtTSpSpclForRowFirstLe'], group)
            data.HmiCmptmtTSpForRowFirstRi = r2p_float(_gg(self.fdx_items_by_rte_name['HmiCmptmtTSpForRowFirstRi'], group), 0.5, 15.0)
            data.HmiCmptmtTSpSpclForRowFirstRi = _gg(self.fdx_items_by_rte_name['HmiCmptmtTSpSpclForRowFirstRi'], group)
            data.HmiCmptmtTSpForRowSecLe = r2p_float(_gg(self.fdx_items_by_rte_name['HmiCmptmtTSpForRowSecLe'], group), 0.5, 15.0)
            data.HmiCmptmtTSpSpclForRowSecLe = _gg(self.fdx_items_by_rte_name['HmiCmptmtTSpSpclForRowSecLe'], group)
            data.HmiCmptmtTSpForRowSecRi = r2p_float(_gg(self.fdx_items_by_rte_name['HmiCmptmtTSpForRowSecRi'], group), 0.5, 15.0)
            data.HmiCmptmtTSpSpclForRowSecRi = _gg(self.fdx_items_by_rte_name['HmiCmptmtTSpSpclForRowSecRi'], group)
        return data

    def get_HmiDefrstElecReq(self):
        # type: () -> datatypes.HmiDefrstElecReq
        data = datatypes.HmiDefrstElecReq()
        group = self.fdx_items_by_rte_name["HmiDefrstElecFrntReq"].parent_group #should be same in all
        with group.mutex_lock:
            data.FrntElecReq = _gg(self.fdx_items_by_rte_name['HmiDefrstElecFrntReq'], group)
            data.ReElecReq = _gg(self.fdx_items_by_rte_name['HmiDefrstElecReReq'], group)
            data.MirrElecReq = _gg(self.fdx_items_by_rte_name['HmiDefrstElecForMirrReq'], group)
        return data

    def send_HmiDefrstElecSts(self, data):
        # type: (datatypes.HmiDefrstElecSts) -> None
        group = self.fdx_items_by_rte_name["HmiDefrstFrntSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["HmiDefrstFrntSts"], _extract_enum_value(_get_if_not_None(data.Frnt)), group)
            _ag(self.fdx_items_by_rte_name["HmiDfrstReSts"], _extract_enum_value(_get_if_not_None(data.Re)), group)
            _ag(self.fdx_items_by_rte_name["HmiMirrDefrstSts"], _extract_enum_value(_get_if_not_None(data.Mirrr)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HmiDefrstElecSts(self):
        group = self.fdx_items_by_rte_name["HmiDefrstFrntSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HmiDefrstMaxReq(self):
        # type: () -> datatypes.ActrReq
        fdx_item = self.fdx_items_by_rte_name["HmiDefrstMaxReq"]
        return fdx_item.value_raw

    def get_HmiHvacFanLvlFrnt(self):
        # type: () -> datatypes.HmiHvacFanLvl
        fdx_item = self.fdx_items_by_rte_name["HmiHvacFanLvlFrnt"]
        return fdx_item.value_raw

    def get_HmiHvacFanLvlRe(self):
        # type: () -> datatypes.HmiHvacFanLvl
        fdx_item = self.fdx_items_by_rte_name["HmiHvacFanLvlRe"]
        return fdx_item.value_raw

    def get_HmiHvacReCtrl(self):
        # type: () -> datatypes.HmiHvacReCtrl
        fdx_item = self.fdx_items_by_rte_name["HmiHvacReCtrl"]
        return fdx_item.value_raw

    def get_HmiHvacRecircCmd(self):
        # type: () -> datatypes.HmiHvacRecircCmd
        fdx_item = self.fdx_items_by_rte_name["HmiHvacRecircCmd"]
        return fdx_item.value_raw

    def get_HmiSeatClima(self):
        # type: () -> datatypes.HmiSeatClima
        data = datatypes.HmiSeatClima()
        group = self.fdx_items_by_rte_name["HmiSeatHeatgForRowFirstLe"].parent_group #should be same in all
        with group.mutex_lock:
            data.HmiSeatHeatgForRowFirstLe = _gg(self.fdx_items_by_rte_name['HmiSeatHeatgForRowFirstLe'], group)
            data.HmiSeatHeatgForRowFirstRi = _gg(self.fdx_items_by_rte_name['HmiSeatHeatgForRowFirstRi'], group)
            data.HmiSeatHeatgForRowSecLe = _gg(self.fdx_items_by_rte_name['HmiSeatHeatgForRowSecLe'], group)
            data.HmiSeatHeatgForRowSecRi = _gg(self.fdx_items_by_rte_name['HmiSeatHeatgForRowSecRi'], group)
            data.HmiSeatVentnForRowFirstLe = _gg(self.fdx_items_by_rte_name['HmiSeatVentnForRowFirstLe'], group)
            data.HmiSeatVentnForRowFirstRi = _gg(self.fdx_items_by_rte_name['HmiSeatVentnForRowFirstRi'], group)
        return data

    def get_HmiSeatClimaExtd(self):
        # type: () -> datatypes.HmiSeatClimaExtd
        data = datatypes.HmiSeatClimaExtd()
        group = self.fdx_items_by_rte_name["HmiSeatClimaExtdHmiSeatVentnForRowSecLe"].parent_group #should be same in all
        with group.mutex_lock:
            data.HmiSeatVentnForRowSecLe = _gg(self.fdx_items_by_rte_name['HmiSeatClimaExtdHmiSeatVentnForRowSecLe'], group)
            data.HmiSeatVentnForRowSecRi = _gg(self.fdx_items_by_rte_name['HmiSeatClimaExtdHmiSeatVentnForRowSecRi'], group)
        return data

    def send_HoodSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["HoodSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HoodSts(self):
        group = self.fdx_items_by_rte_name["HoodSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HptcWarnSeldForCllsnFwdWarn(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name[''], group)
            data.Pen = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def get_HudActvReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["HudActvReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['HudActvReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['HudActvReqPen'], group)
        return data

    def send_HudActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["HudActvSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HudActvSts(self):
        group = self.fdx_items_by_rte_name["HudActvSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HudAdjmtReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["HudAdjmtReq"]
        return fdx_item.value_raw

    def send_HudDiagc(self, data):
        # type: (datatypes.HudDiagc) -> None
        group = self.fdx_items_by_rte_name["HudDiagcHudTSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["HudDiagcHudTSts"], _extract_enum_value(_get_if_not_None(data.HudTSts)), group)
            _ag(self.fdx_items_by_rte_name["HudDiagcHudCircShoSts"], _extract_enum_value(_get_if_not_None(data.HudCircShoSts)), group)
            _ag(self.fdx_items_by_rte_name["HudDiagcHudCricOpenSts"], _extract_enum_value(_get_if_not_None(data.HudCricOpenSts)), group)
            _ag(self.fdx_items_by_rte_name["HudDiagcImgHudTmpNotAvlSts"], _extract_enum_value(_get_if_not_None(data.ImgHudTmpNotAvlSts)), group)
            _ag(self.fdx_items_by_rte_name["HudDiagcImgHudErrSts"], _extract_enum_value(_get_if_not_None(data.ImgHudErrSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HudDiagc(self):
        group = self.fdx_items_by_rte_name["HudDiagcHudTSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HudErgoSetgReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["HudErgoSetgReq"]
        return fdx_item.value_raw

    def send_HudSts(self, data):
        # type: (datatypes.HudStsForHmi) -> None
        fdx_item = self.fdx_items_by_rte_name["HudSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HudSts(self):
        group = self.fdx_items_by_rte_name["HudSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HudVisFctSetg(self):
        # type: () -> datatypes.HudVisFctSetgReq
        data = datatypes.HudVisFctSetgReq()
        group = self.fdx_items_by_rte_name["HudVisFctSetgPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name['HudVisFctSetgPen'], group)
            data.HudFct00 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct00'], group)
            data.HudFct01 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct01'], group)
            data.HudFct02 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct02'], group)
            data.HudFct03 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct03'], group)
            data.HudFct04 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct04'], group)
            data.HudFct05 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct05'], group)
            data.HudFct06 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct06'], group)
            data.HudFct07 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct07'], group)
            data.HudFct08 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct08'], group)
            data.HudFct09 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct09'], group)
            data.HudFct10 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct10'], group)
            data.HudFct11 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct11'], group)
            data.HudFct12 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct12'], group)
            data.HudFct13 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct13'], group)
            data.HudFct14 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct14'], group)
            data.HudFct15 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct15'], group)
            data.HudFct16 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct16'], group)
            data.HudFct17 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct17'], group)
            data.HudFct18 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct18'], group)
            data.HudFct19 = _gg(self.fdx_items_by_rte_name['HudVisFctSetgHudFct19'], group)
        return data

    def send_HvBattEgyAvlDcha3(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["None"]
        fdx_item.value_raw = p2r_float(data, 50.0, -500.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HvBattEgyAvlDcha3(self):
        group = self.fdx_items_by_rte_name["None"].parent_group
        self.remove_group_from_send_timer(group)

    def send_HvBattSmtActv(self, data):
        # type: (datatypes.Flg1) -> None
        fdx_item = self.fdx_items_by_rte_name["HvBattSmtActv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HvBattSmtActv(self):
        group = self.fdx_items_by_rte_name["HvBattSmtActv"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HvBattSmtSeld(self):
        # type: () -> datatypes.OnOffNoReq
        fdx_item = self.fdx_items_by_rte_name["HvBattSmtSeld"]
        return fdx_item.value_raw

    def send_HvacAirMFlowEstimd(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["HvacAirMFlowEstimd"]
        fdx_item.value_raw = p2r_float(data, 1.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HvacAirMFlowEstimd(self):
        group = self.fdx_items_by_rte_name["HvacAirMFlowEstimd"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HznData(self):
        # type: () -> datatypes.HznDataGroup2
        data = datatypes.HznDataGroup2()
        group = self.fdx_items_by_rte_name["MsgTypInHznData"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['MsgTypInHznData'], group)
            data.CtryCod = _gg(self.fdx_items_by_rte_name['CtryCodInHznData'], group)
            data.RegnCod = _gg(self.fdx_items_by_rte_name['RegnCodInHznData'], group)
            data.DrvgSide = _gg(self.fdx_items_by_rte_name['DrvgSideInHznData'], group)
            data.SpdUnit = _gg(self.fdx_items_by_rte_name['SpdUnitInHznData'], group)
            data.HdTxVers = _gg(self.fdx_items_by_rte_name['HdTxVersInHznData'], group)
            data.TxVers = _gg(self.fdx_items_by_rte_name['TxVersInHznData'], group)
            data.ChdTxVers = _gg(self.fdx_items_by_rte_name['ChdTxVersInHznData'], group)
            data.HwVers = _gg(self.fdx_items_by_rte_name['HwVersInHznData'], group)
            data.MapSrc = _gg(self.fdx_items_by_rte_name['MapSrcInHznData'], group)
            data.YrVersOfMap = _gg(self.fdx_items_by_rte_name['YrVersOfMapInHznData'], group)
            data.PartOfYrVersOfMap = _gg(self.fdx_items_by_rte_name['PartOfYrVersOfMapInHznData'], group)
        return data

    def get_HznEdge(self):
        # type: () -> datatypes.HznEdgeGroup2
        data = datatypes.HznEdgeGroup2()
        group = self.fdx_items_by_rte_name["MsgTypInHznEdge"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['MsgTypInHznEdge'], group)
            data.TxPrev = _gg(self.fdx_items_by_rte_name['TxPrevInHznEdge'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['PahIdxInHznEdge'], group)
            data.Offs = _gg(self.fdx_items_by_rte_name['OffsInHznEdge'], group)
            data.Upd = _gg(self.fdx_items_by_rte_name['UpdInHznEdge'], group)
            data.PahIdxNew = _gg(self.fdx_items_by_rte_name['PahIdxNewInHznEdge'], group)
            data.TurnAg = r2p_float(_gg(self.fdx_items_by_rte_name['TurnAgInHznEdge'], group), 1.4173228346456692, 0.0)
            data.RelProblty = r2p_float(_gg(self.fdx_items_by_rte_name['RelProbltyInHznEdge'], group), 3.33333333, 0.0)
            data.TypOfWay = _gg(self.fdx_items_by_rte_name['TypOfWayInHznEdge'], group)
            data.NrOfLaneInDrvgDir = _gg(self.fdx_items_by_rte_name['NrOfLaneInDrvgDirInHznEdge'], group)
            data.NrOfLaneInSecDir = _gg(self.fdx_items_by_rte_name['NrOfLaneInSecDirInHznEdge'], group)
            data.IntscnCmplx = _gg(self.fdx_items_by_rte_name['IntscnCmplxInHznEdge'], group)
            data.RiOfWay = _gg(self.fdx_items_by_rte_name['RiOfWayInHznEdge'], group)
            data.RoadClass = _gg(self.fdx_items_by_rte_name['RoadClassInHznEdge'], group)
            data.PartOfCourseCalcd = _gg(self.fdx_items_by_rte_name['PartOfCourseCalcdInHznEdge'], group)
            data.LstEdgeAtOffs = _gg(self.fdx_items_by_rte_name['LstEdgeAtOffsInHznEdge'], group)
        return data

    def get_HznPosn(self):
        # type: () -> datatypes.HznPosnGroup3
        data = datatypes.HznPosnGroup3()
        group = self.fdx_items_by_rte_name["MsgTypInHznPosn"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['MsgTypInHznPosn'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['PahIdxInHznPosn'], group)
            data.Offs = _gg(self.fdx_items_by_rte_name['OffsInHznPosn'], group)
            data.PosnIdx = _gg(self.fdx_items_by_rte_name['PosnIdxInHznPosn'], group)
            data.PosnTiDif = r2p_float(_gg(self.fdx_items_by_rte_name['PosnTiDifInHznPosn'], group), 5.0, 0.0)
            data.Spd = r2p_float(_gg(self.fdx_items_by_rte_name['SpdInHznPosn'], group), 0.2, -12.8)
            data.RelDir = r2p_float(_gg(self.fdx_items_by_rte_name['RelDirInHznPosn'], group), 1.4173228346456692, 0.0)
            data.PosnProblty = r2p_float(_gg(self.fdx_items_by_rte_name['PosnProbltyInHznPosn'], group), 3.33333333, 0.0)
            data.PosnQly = _gg(self.fdx_items_by_rte_name['PosnQlyInHznPosn'], group)
            data.LanePrsnt = _gg(self.fdx_items_by_rte_name['LanePrsntInHznPosn'], group)
        return data

    def get_HznPosnExtd(self):
        # type: () -> datatypes.HznPosnExtdGroup1
        data = datatypes.HznPosnExtdGroup1()
        group = self.fdx_items_by_rte_name["HznPosnExtdMsgTyp"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['HznPosnExtdMsgTyp'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['HznPosnExtdPahIdx'], group)
            data.PosnIdx = _gg(self.fdx_items_by_rte_name['HznPosnExtdPosnIdx'], group)
            data.PosnTiDif = r2p_float(_gg(self.fdx_items_by_rte_name['HznPosnExtdPosnTiDif'], group), 5.0, 0.0)
            data.Spd = r2p_float(_gg(self.fdx_items_by_rte_name['HznPosnExtdSpd'], group), 0.2, -12.8)
            data.RelDir = r2p_float(_gg(self.fdx_items_by_rte_name['HznPosnExtdRelDir'], group), 1.4173228346456692, 0.0)
            data.PosnProblty = r2p_float(_gg(self.fdx_items_by_rte_name['HznPosnExtdPosnProblty'], group), 3.33333333, 0.0)
            data.PosnQly = _gg(self.fdx_items_by_rte_name['HznPosnExtdPosnQly'], group)
            data.LanePrsnt = _gg(self.fdx_items_by_rte_name['HznPosnExtdLanePrsnt'], group)
        return data

    def get_HznPosnExtdOffs(self):
        # type: () -> datatypes.HznPosnExtdOffs
        data = datatypes.HznPosnExtdOffs()
        group = self.fdx_items_by_rte_name["HznPosnExtdOffsOffs"].parent_group #should be same in all
        with group.mutex_lock:
            data.Offs = r2p_float(_gg(self.fdx_items_by_rte_name['HznPosnExtdOffsOffs'], group), 128.0, 0.0)
        return data

    def get_HznProfLong(self):
        # type: () -> datatypes.HznProfLongGroup3
        data = datatypes.HznProfLongGroup3()
        group = self.fdx_items_by_rte_name["MsgTypInHznProfLong"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['MsgTypInHznProfLong'], group)
            data.TxPrev = _gg(self.fdx_items_by_rte_name['TxPrevInHznProfLong'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['PahIdxInHznProfLong'], group)
            data.Offs = _gg(self.fdx_items_by_rte_name['OffsInHznProfLong'], group)
            data.Upd = _gg(self.fdx_items_by_rte_name['UpdInHznProfLong'], group)
            data.ProfTyp = _gg(self.fdx_items_by_rte_name['ProfTypInHznProfLong'], group)
            data.NodCtrl = _gg(self.fdx_items_by_rte_name['NodCtrlInHznProfLong'], group)
            data.Val = _gg(self.fdx_items_by_rte_name['ValInHznProfLong'], group)
        return data

    def get_HznProfLongExtd(self):
        # type: () -> datatypes.HznProfLongExtdGroup1
        data = datatypes.HznProfLongExtdGroup1()
        group = self.fdx_items_by_rte_name["HznProfLongExtdMsgTyp"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['HznProfLongExtdMsgTyp'], group)
            data.TxPrev = _gg(self.fdx_items_by_rte_name['HznProfLongExtdTxPrev'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['HznProfLongExtdPahIdx'], group)
            data.Offs = r2p_float(_gg(self.fdx_items_by_rte_name['HznProfLongExtdOffs'], group), 128.0, 0.0)
            data.Upd = _gg(self.fdx_items_by_rte_name['HznProfLongExtdUpd'], group)
            data.ProfTyp = _gg(self.fdx_items_by_rte_name['HznProfLongExtdProfTyp'], group)
            data.NodCtrl = _gg(self.fdx_items_by_rte_name['HznProfLongExtdNodCtrl'], group)
            data.Val = _gg(self.fdx_items_by_rte_name['HznProfLongExtdVal'], group)
        return data

    def get_HznProfSho(self):
        # type: () -> datatypes.HznProfSho2
        data = datatypes.HznProfSho2()
        group = self.fdx_items_by_rte_name["MsgTypInHznProfSho"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['MsgTypInHznProfSho'], group)
            data.TxPrev = _gg(self.fdx_items_by_rte_name['TxPrevInHznProfSho'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['PahIdxInHznProfSho'], group)
            data.Offs = _gg(self.fdx_items_by_rte_name['OffsInHznProfSho'], group)
            data.Upd = _gg(self.fdx_items_by_rte_name['UpdInHznProfSho'], group)
            data.ProfTyp = _gg(self.fdx_items_by_rte_name['ProfTypInHznProfSho'], group)
            data.NodCtrl = _gg(self.fdx_items_by_rte_name['NodCtrlInHznProfSho'], group)
            data.Val0 = _gg(self.fdx_items_by_rte_name['Val0InHznProfSho'], group)
            data.Dst1 = _gg(self.fdx_items_by_rte_name['Dst1InHznProfSho'], group)
            data.Val1 = _gg(self.fdx_items_by_rte_name['Val1InHznProfSho'], group)
            data.ProfTypQly = _gg(self.fdx_items_by_rte_name['ProfTypQlyInHznProfSho'], group)
        return data

    def send_HznRstExtd(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["HznRstExtd"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HznRstExtd(self):
        group = self.fdx_items_by_rte_name["HznRstExtd"].parent_group
        self.remove_group_from_send_timer(group)

    def get_HznSeg(self):
        # type: () -> datatypes.HznSegGroup2
        data = datatypes.HznSegGroup2()
        group = self.fdx_items_by_rte_name["MsgTypInHznSeg"].parent_group #should be same in all
        with group.mutex_lock:
            data.MsgTyp = _gg(self.fdx_items_by_rte_name['MsgTypInHznSeg'], group)
            data.TxPrev = _gg(self.fdx_items_by_rte_name['TxPrevInHznSeg'], group)
            data.PahIdx = _gg(self.fdx_items_by_rte_name['PahIdxInHznSeg'], group)
            data.Offs = _gg(self.fdx_items_by_rte_name['OffsInHznSeg'], group)
            data.Upd = _gg(self.fdx_items_by_rte_name['UpdInHznSeg'], group)
            data.RoadClass = _gg(self.fdx_items_by_rte_name['RoadClassInHznSeg'], group)
            data.TypOfWay = _gg(self.fdx_items_by_rte_name['TypOfWayInHznSeg'], group)
            data.SpdLimEfc = _gg(self.fdx_items_by_rte_name['SpdLimEfcInHznSeg'], group)
            data.SpdLimTypEfc = _gg(self.fdx_items_by_rte_name['SpdLimTypEfcInHznSeg'], group)
            data.NrOfLaneInDrvgDir = _gg(self.fdx_items_by_rte_name['NrOfLaneInDrvgDirInHznSeg'], group)
            data.NrOfLaneInSecDir = _gg(self.fdx_items_by_rte_name['NrOfLaneInSecDirInHznSeg'], group)
            data.Tnl = _gg(self.fdx_items_by_rte_name['TnlInHznSeg'], group)
            data.Brdg = _gg(self.fdx_items_by_rte_name['BrdgInHznSeg'], group)
            data.RoadMpl = _gg(self.fdx_items_by_rte_name['RoadMplInHznSeg'], group)
            data.RoadLiAr = _gg(self.fdx_items_by_rte_name['RoadLiArInHznSeg'], group)
            data.IntscnCmplx = _gg(self.fdx_items_by_rte_name['IntscnCmplxInHznSeg'], group)
            data.RelProblty = r2p_float(_gg(self.fdx_items_by_rte_name['RelProbltyInHznSeg'], group), 3.33333333, 0.0)
            data.PartOfCourseCalcd = _gg(self.fdx_items_by_rte_name['PartOfCourseCalcdInHznSeg'], group)
        return data

    def get_HznSplyElectcSts(self):
        # type: () -> datatypes.HznSplyElectcSts1
        fdx_item = self.fdx_items_by_rte_name["HznSplyElectcSts"]
        return fdx_item.value_raw

    def get_HzrdLiWarnActv(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["HzrdLiWarnActv"]
        return fdx_item.value_raw

    def send_HzrdLiWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["HzrdLiWarnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_HzrdLiWarnSts(self):
        group = self.fdx_items_by_rte_name["HzrdLiWarnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_IndcnOfParkAssiSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["IndcnOfParkAssiSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_IndcnOfParkAssiSts(self):
        group = self.fdx_items_by_rte_name["IndcnOfParkAssiSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_IndcnOfPrkgAutSts(self, data):
        # type: (datatypes.IndOfPrkgAutSts) -> None
        fdx_item = self.fdx_items_by_rte_name["IndcnOfPrkgAutSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_IndcnOfPrkgAutSts(self):
        group = self.fdx_items_by_rte_name["IndcnOfPrkgAutSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_IndcnUnit(self):
        # type: () -> datatypes.IndcnUnit
        data = datatypes.IndcnUnit()
        group = self.fdx_items_by_rte_name["TUnit"].parent_group #should be same in all
        with group.mutex_lock:
            data.TUnit = _gg(self.fdx_items_by_rte_name['TUnit'], group)
            data.SpdUnit = _gg(self.fdx_items_by_rte_name['SpdUnit'], group)
            data.DstLong = _gg(self.fdx_items_by_rte_name['DstLong'], group)
            data.DstSho = _gg(self.fdx_items_by_rte_name['DstSho'], group)
            data.FuCnsUnit = _gg(self.fdx_items_by_rte_name['FuCnsUnit'], group)
            data.VolUnit = _gg(self.fdx_items_by_rte_name['VolUnit'], group)
            data.IdPenForUnit = _gg(self.fdx_items_by_rte_name['IdPenForUnits'], group)
            data.ClkFmt = _gg(self.fdx_items_by_rte_name['ClkFmt'], group)
            data.PUnit = _gg(self.fdx_items_by_rte_name['PUnit'], group)
            data.DateFmt = _gg(self.fdx_items_by_rte_name['IndcnUnitDateFmt'], group)
        return data

    def send_IndcrDisp1WdSts(self, data):
        # type: (datatypes.IndcrSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["IndcrDisp1WdSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_IndcrDisp1WdSts(self):
        group = self.fdx_items_by_rte_name["IndcrDisp1WdSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_IndcrTurnSts1WdSts(self, data):
        # type: (datatypes.IndcrSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["IndcrTurnSts1WdSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_IndcrTurnSts1WdSts(self):
        group = self.fdx_items_by_rte_name["IndcrTurnSts1WdSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_IniValSigCfgIDBackboneFR(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["IniValSigCfgIDBackboneFR"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_IniValSigCfgIDBackboneFR(self):
        group = self.fdx_items_by_rte_name["IniValSigCfgIDBackboneFR"].parent_group
        self.remove_group_from_send_timer(group)

    def get_InsdCarNoiseMeasd(self):
        # type: () -> float
        fdx_item = self.fdx_items_by_rte_name["InsdCarNoiseMeasd"]
        return r2p_float(fdx_item.value_raw, 0.5, 0.0)

    def get_IntAudCnclWarn(self):
        # type: () -> datatypes.YesNo2
        fdx_item = self.fdx_items_by_rte_name["None"]
        return fdx_item.value_raw

    def send_IntrBriSts(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["IntrBriSts"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_IntrBriSts(self):
        group = self.fdx_items_by_rte_name["IntrBriSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_IntrBriStsForSeatHeatrRe(self):
        # type: () -> int
        fdx_item = self.fdx_items_by_rte_name["IntrBriStsForSeatHeatrRe"]
        return fdx_item.value_raw

    def get_IntrLiAmbLiSetg(self):
        # type: () -> datatypes.IntrLiAmbLiSetg
        data = datatypes.IntrLiAmbLiSetg()
        group = self.fdx_items_by_rte_name["IntrLiAmbLiSetgForLiInten"].parent_group #should be same in all
        with group.mutex_lock:
            data.LiInten = _gg(self.fdx_items_by_rte_name['IntrLiAmbLiSetgForLiInten'], group)
            data.LiTintg = _gg(self.fdx_items_by_rte_name['IntrLiAmbLiSetgForLiTintg'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['IntrLiAmbLiSetgPen'], group)
        return data

    def get_IntrLiSurrndgsLiSetgLi(self):
        # type: () -> datatypes.IntrLiSurrndgsLiSetg
        data = datatypes.IntrLiSurrndgsLiSetg()
        group = self.fdx_items_by_rte_name["IntrLiSurrndgsLiSetgForLiInten"].parent_group #should be same in all
        with group.mutex_lock:
            data.LiInten = _gg(self.fdx_items_by_rte_name['IntrLiSurrndgsLiSetgForLiInten'], group)
            data.LiLvl = _gg(self.fdx_items_by_rte_name['IntrLiSurrndgsLiSetgForLiLvl'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['IntrLiSurrndgsLiSetgPen'], group)
        return data

    def get_IntrMirrTintgSetg(self):
        # type: () -> datatypes.MirrDimPen
        data = datatypes.MirrDimPen()
        group = self.fdx_items_by_rte_name["IntrMirrTintgSetgSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.MirrDimLvl = _gg(self.fdx_items_by_rte_name['IntrMirrTintgSetgSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['IntrMirrTintgSetgPen'], group)
        return data

    def get_IntvAndWarnModForLaneKeepAid(self):
        # type: () -> datatypes.WarnAndIntvPen1
        data = datatypes.WarnAndIntvPen1()
        group = self.fdx_items_by_rte_name["IntvAndWarnModForLaneKeepAidSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['IntvAndWarnModForLaneKeepAidSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['IntvAndWarnModForLaneKeepAidPen'], group)
        return data

    def send_KeyLostWarnIndcn(self, data):
        # type: (datatypes.KeyLostWarnIndcn) -> None
        fdx_item = self.fdx_items_by_rte_name["KeyLostWarnIndcn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_KeyLostWarnIndcn(self):
        group = self.fdx_items_by_rte_name["KeyLostWarnIndcn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_KeyProfMpgUpd(self):
        # type: () -> datatypes.KeyProfMpgUpd1
        data = datatypes.KeyProfMpgUpd1()
        group = self.fdx_items_by_rte_name["KeyProfMpgUpdForIdPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.KeyProfMpgUpdForIdPen = _gg(self.fdx_items_by_rte_name['KeyProfMpgUpdForIdPen'], group)
            data.KeyProfMpgUpdOff = _gg(self.fdx_items_by_rte_name['KeyProfMpgUpdKeyProfMpgUpdOff'], group)
        return data

    def get_KeyReadReqFromSetgMgr(self):
        # type: () -> datatypes.KeyLocn1
        fdx_item = self.fdx_items_by_rte_name["KeyReadReqFromSetgMgr"]
        return fdx_item.value_raw

    def send_KeyReadStsToProfCtrl(self, data):
        # type: (datatypes.KeyReadStsToProfCtrl) -> None
        group = self.fdx_items_by_rte_name["KeyReadStsToProfCtrlKeyId"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["KeyReadStsToProfCtrlKeyId"], _extract_enum_value(_get_if_not_None(data.KeyId)), group)
            _ag(self.fdx_items_by_rte_name["KeyReadStsToProfCtrlBoolean"], _get_if_not_None(data.Boolean), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_KeyReadStsToProfCtrl(self):
        group = self.fdx_items_by_rte_name["KeyReadStsToProfCtrlKeyId"].parent_group
        self.remove_group_from_send_timer(group)

    def send_KeyRmnIndcn(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["KeyRmnIndcn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_KeyRmnIndcn(self):
        group = self.fdx_items_by_rte_name["KeyRmnIndcn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_KeySpdWarn(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["KeySpdWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_KeySpdWarn(self):
        group = self.fdx_items_by_rte_name["KeySpdWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LampSuppSrv(self):
        # type: () -> datatypes.SrvSts
        fdx_item = self.fdx_items_by_rte_name["LampSuppSrv"]
        return fdx_item.value_raw

    def get_LaneChgWarnActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LaneChgWarnActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LaneChgWarnActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LaneChgWarnActvPen'], group)
        return data

    def send_LaneChgWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["LaneChgWarnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LaneChgWarnSts(self):
        group = self.fdx_items_by_rte_name["LaneChgWarnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LaneDetnStsForAutDrv(self, data):
        # type: (datatypes.LaneDetnSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["LaneDetnStsForAutDrv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LaneDetnStsForAutDrv(self):
        group = self.fdx_items_by_rte_name["LaneDetnStsForAutDrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LaneDetnStsForLaneKeepAid(self, data):
        # type: (datatypes.LaneDetnSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["LaneDetnStsForLaneKeepAid"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LaneDetnStsForLaneKeepAid(self):
        group = self.fdx_items_by_rte_name["LaneDetnStsForLaneKeepAid"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LaneKeepAidActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LaneKeepAidActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LaneKeepAidActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LaneKeepAidActvPen'], group)
        return data

    def get_LaneKeepAidRoadEdgeActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LaneKeepAidRoadEdgeActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LaneKeepAidRoadEdgeActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LaneKeepAidRoadEdgeActvPen'], group)
        return data

    def send_LaneKeepAidSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["LaneKeepAidSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LaneKeepAidSts(self):
        group = self.fdx_items_by_rte_name["LaneKeepAidSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LatMovmtWarn(self, data):
        # type: (datatypes.WarnLeRi1) -> None
        fdx_item = self.fdx_items_by_rte_name["LatMovmtWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LatMovmtWarn(self):
        group = self.fdx_items_by_rte_name["LatMovmtWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LcmaOn1(self, data):
        # type: (datatypes.LcmaCtraOn) -> None
        fdx_item = self.fdx_items_by_rte_name["LcmaOn1"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LcmaOn1(self):
        group = self.fdx_items_by_rte_name["LcmaOn1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LiExtReq1WdReq1(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LiAdpvReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiAdpvReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiAdpvReqPen'], group)
        return data

    def get_LiExtReq2WdReq1(self):
        # type: () -> datatypes.LiAutTranPen1
        data = datatypes.LiAutTranPen1()
        group = self.fdx_items_by_rte_name["LiAutTranReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiAutTranReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiAutTranReqPen'], group)
        return data

    def get_LiExtReq1WdReq4(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LiBeamHiAuxReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiBeamHiAuxReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiBeamHiAuxReqPen'], group)
        return data

    def get_LiExtReq1WdReq6(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LiCornrgReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiCornrgReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiCornrgReqPen'], group)
        return data

    def get_LiExtReq1WdReq2(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LiDaytiRunngReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiDaytiRunngReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiDaytiRunngReqPen'], group)
        return data

    def send_LiDrvrFltIndcrTurn(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["LiDrvrFltIndcrTurn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LiDrvrFltIndcrTurn(self):
        group = self.fdx_items_by_rte_name["LiDrvrFltIndcrTurn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LiForBtn4ForUsrSwtPanFrntCmd(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["LiForBtn4ForUsrSwtPanFrntCmd"]
        return fdx_item.value_raw

    def get_LiForBtn5ForUsrSwtPanFrntCmd(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["LiForBtn5ForUsrSwtPanFrntCmd"]
        return fdx_item.value_raw

    def get_LiExtSafe1WdReq2(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LiHomeLvngReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiHomeLvngReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiHomeLvngReqPen'], group)
        return data

    def get_LiExtSafe1WdReq1(self):
        # type: () -> datatypes.LiTiPen2
        data = datatypes.LiTiPen2()
        group = self.fdx_items_by_rte_name["LiHomeSafeReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiHomeSafeReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiHomeSafeReqPen'], group)
        return data

    def send_LiLvrSwt1Req(self, data):
        # type: (datatypes.SwtPush) -> None
        fdx_item = self.fdx_items_by_rte_name["LiLvrSwt1"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LiLvrSwt1Req(self):
        group = self.fdx_items_by_rte_name["LiLvrSwt1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LiPassFltIndcrTurn(self, data):
        # type: (datatypes.DevErrSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["LiPassFltIndcrTurn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LiPassFltIndcrTurn(self):
        group = self.fdx_items_by_rte_name["LiPassFltIndcrTurn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LiSeldForDrvrPfmncMon(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LiSeldForDrvrPfmncMonSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LiSeldForDrvrPfmncMonSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LiSeldForDrvrPfmncMonPen'], group)
        return data

    def get_LiTrfcSide1WdReq1(self):
        # type: () -> datatypes.LiTrfcSide1
        fdx_item = self.fdx_items_by_rte_name["LiTrfcSideReq"]
        return fdx_item.value_raw

    def send_ListOfNodAv(self, data):
        # type: (datatypes.ListOfNodAv) -> None
        group = self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv1"], _get_if_not_None(data.ListOfNodAv1), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv2"], _get_if_not_None(data.ListOfNodAv2), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv3"], _get_if_not_None(data.ListOfNodAv3), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv4"], _get_if_not_None(data.ListOfNodAv4), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv5"], _get_if_not_None(data.ListOfNodAv5), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv6"], _get_if_not_None(data.ListOfNodAv6), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv7"], _get_if_not_None(data.ListOfNodAv7), group)
            _ag(self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv8"], _get_if_not_None(data.ListOfNodAv8), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ListOfNodAv(self):
        group = self.fdx_items_by_rte_name["ListOfNodAvListOfNodAv1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LockSpdReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LockSpdReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LockSpdReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LockSpdReqPen'], group)
        return data

    def send_LockgCenSts(self, data):
        # type: (datatypes.LockgCenSts3) -> None
        group = self.fdx_items_by_rte_name["LockgCenStsLockSt"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["LockgCenStsLockSt"], _extract_enum_value(_get_if_not_None(data.LockSt)), group)
            _ag(self.fdx_items_by_rte_name["LockgCenTrigSrc"], _extract_enum_value(_get_if_not_None(data.TrigSrc)), group)
            _ag(self.fdx_items_by_rte_name["LockgCenStsUpdEve"], _get_if_not_None(data.UpdEve), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LockgCenSts(self):
        group = self.fdx_items_by_rte_name["LockgCenStsLockSt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LockgCenStsForUsrFb(self, data):
        # type: (datatypes.LockSt2) -> None
        fdx_item = self.fdx_items_by_rte_name["LockgCenStsForUsrFb"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LockgCenStsForUsrFb(self):
        group = self.fdx_items_by_rte_name["LockgCenStsForUsrFb"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LockgFbSoundReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LockgFbSoundReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LockgFbSoundReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LockgFbSoundReqPen'], group)
        return data

    def get_LockgFbVisReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["LockgFbVisReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LockgFbVisReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LockgFbVisReqPen'], group)
        return data

    def get_LockgPrsnlReqFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        fdx_item = self.fdx_items_by_rte_name["LockgPrsnlReqFromHmi"]
        return fdx_item.value_raw

    def send_LockgPrsnlSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["LockgPrsnlSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LockgPrsnlSts(self):
        group = self.fdx_items_by_rte_name["LockgPrsnlSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_LvlOfClimaCmft(self, data):
        # type: (datatypes.LvlOfClimaCmft) -> None
        fdx_item = self.fdx_items_by_rte_name["LvlOfClimaCmft"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_LvlOfClimaCmft(self):
        group = self.fdx_items_by_rte_name["LvlOfClimaCmft"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LvlSeldForSpdLimAdpv(self):
        # type: () -> datatypes.LvlSeldForSpdLimAdpvPen1
        data = datatypes.LvlSeldForSpdLimAdpvPen1()
        group = self.fdx_items_by_rte_name["LvlSeldForSpdLimAdpvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['LvlSeldForSpdLimAdpvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['LvlSeldForSpdLimAdpvPen'], group)
        return data

    def get_MassgFctActv(self):
        # type: () -> datatypes.MassgFctActv
        data = datatypes.MassgFctActv()
        group = self.fdx_items_by_rte_name["MassgFctActvDrvrMassgFctActv"].parent_group #should be same in all
        with group.mutex_lock:
            data.DrvrMassgFctActv = _gg(self.fdx_items_by_rte_name['MassgFctActvDrvrMassgFctActv'], group)
            data.PassMassgFctActv = _gg(self.fdx_items_by_rte_name['MassgFctActvPassMassgFctActv'], group)
        return data

    def send_MemBtnSound(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["MemBtnSound"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MemBtnSound(self):
        group = self.fdx_items_by_rte_name["MemBtnSound"].parent_group
        self.remove_group_from_send_timer(group)

    def send_MirrDwnStsAtDrvr(self, data):
        # type: (datatypes.MirrDwnStsTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["MirrDwnStsAtDrvr"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MirrDwnStsAtDrvr(self):
        group = self.fdx_items_by_rte_name["MirrDwnStsAtDrvr"].parent_group
        self.remove_group_from_send_timer(group)

    def send_MirrDwnStsAtPass(self, data):
        # type: (datatypes.MirrDwnStsTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["MirrDwnStsAtPass"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MirrDwnStsAtPass(self):
        group = self.fdx_items_by_rte_name["MirrDwnStsAtPass"].parent_group
        self.remove_group_from_send_timer(group)

    def send_MirrFoldStsAtDrvr(self, data):
        # type: (datatypes.MirrFoldStsTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["MirrFoldStsAtDrvr"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MirrFoldStsAtDrvr(self):
        group = self.fdx_items_by_rte_name["MirrFoldStsAtDrvr"].parent_group
        self.remove_group_from_send_timer(group)

    def send_MirrFoldStsAtPass(self, data):
        # type: (datatypes.MirrFoldStsTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["MirrFoldStsAtPass"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MirrFoldStsAtPass(self):
        group = self.fdx_items_by_rte_name["MirrFoldStsAtPass"].parent_group
        self.remove_group_from_send_timer(group)

    def get_MmedHdPwrMod(self):
        # type: () -> datatypes.MmedMaiPwrMod
        fdx_item = self.fdx_items_by_rte_name["MmedMaiPwrMod"]
        return fdx_item.value_raw

    def get_MmedHmiModStd(self):
        # type: () -> datatypes.MmedHmiModStd2
        fdx_item = self.fdx_items_by_rte_name["MmedHmiModStd"]
        return fdx_item.value_raw

    def get_MmedTvmPwerMod(self):
        # type: () -> datatypes.MmedTvmPwerMod
        fdx_item = self.fdx_items_by_rte_name["MmedTvmPwerMod"]
        return fdx_item.value_raw

    def send_MstCfgIDBackboneFR(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["MstCfgIDBackboneFR"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MstCfgIDBackboneFR(self):
        group = self.fdx_items_by_rte_name["MstCfgIDBackboneFR"].parent_group
        self.remove_group_from_send_timer(group)

    def send_MtrlSnsrT(self, data):
        # type: (datatypes.MtrlSnsrT) -> None
        group = self.fdx_items_by_rte_name[""].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name[""], p2r_float(data.MtrlSnsrT, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name[""], _extract_enum_value(_get_if_not_None(data.MtrlSnsrTFacQly)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_MtrlSnsrT(self):
        group = self.fdx_items_by_rte_name[""].parent_group
        self.remove_group_from_send_timer(group)

    def send_NFSDataFront(self, data):
        # type: (datatypes.SnsrPrkgAssi3) -> None
        group = self.fdx_items_by_rte_name["NFSDataFrontSnsrDstInsdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["NFSDataFrontSnsrDstInsdLe"], p2r_float(data.SnsrDstInsdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataFrontSnsrDstOutdLe"], p2r_float(data.SnsrDstOutdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataFrontSnsrDstSideLe"], p2r_float(data.SnsrDstSideLe, -1.0, 511.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataFrontSnsrDstInsdRi"], p2r_float(data.SnsrDstInsdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataFrontSnsrDstOutdRi"], p2r_float(data.SnsrDstOutdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataFrontSnsrDstSideRi"], p2r_float(data.SnsrDstSideRi, -1.0, 511.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NFSDataFront(self):
        group = self.fdx_items_by_rte_name["NFSDataFrontSnsrDstInsdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NFSDataRear(self, data):
        # type: (datatypes.SnsrPrkgAssi3) -> None
        group = self.fdx_items_by_rte_name["NFSDataRearSnsrDstInsdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["NFSDataRearSnsrDstInsdLe"], p2r_float(data.SnsrDstInsdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataRearSnsrDstOutdLe"], p2r_float(data.SnsrDstOutdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataRearSnsrDstSideLe"], p2r_float(data.SnsrDstSideLe, -1.0, 511.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataRearSnsrDstInsdRi"], p2r_float(data.SnsrDstInsdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataRearSnsrDstOutdRi"], p2r_float(data.SnsrDstOutdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["NFSDataRearSnsrDstSideRi"], p2r_float(data.SnsrDstSideRi, -1.0, 511.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NFSDataRear(self):
        group = self.fdx_items_by_rte_name["NFSDataRearSnsrDstInsdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NetCtrlrActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        group = self.fdx_items_by_rte_name["NetCtrlrActvtResourceGroup"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["NetCtrlrActvtResourceGroup"], _get_if_not_None(data.ResourceGroup), group)
            _ag(self.fdx_items_by_rte_name["NetCtrlrActvtPrio"], _extract_enum_value(_get_if_not_None(data.Prio)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NetCtrlrActvt(self):
        group = self.fdx_items_by_rte_name["NetCtrlrActvtResourceGroup"].parent_group
        self.remove_group_from_send_timer(group)

    def get_NetHdActvt(self):
        # type: () -> datatypes.NetActvtRec1
        data = datatypes.NetActvtRec1()
        group = self.fdx_items_by_rte_name["NetHdActvtResourceGroup"].parent_group #should be same in all
        with group.mutex_lock:
            data.ResourceGroup = _gg(self.fdx_items_by_rte_name['NetHdActvtResourceGroup'], group)
            data.Prio = _gg(self.fdx_items_by_rte_name['NetHdActvtPrio'], group)
        return data

    def send_NetTelmActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        group = self.fdx_items_by_rte_name["NetTelmActvtResourceGroup"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["NetTelmActvtResourceGroup"], _get_if_not_None(data.ResourceGroup), group)
            _ag(self.fdx_items_by_rte_name["NetTelmActvtPrio"], _extract_enum_value(_get_if_not_None(data.Prio)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NetTelmActvt(self):
        group = self.fdx_items_by_rte_name["NetTelmActvtResourceGroup"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NewTripCdn(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["NewTripCdn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NewTripCdn(self):
        group = self.fdx_items_by_rte_name["NewTripCdn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NoEntryWarnReq(self, data):
        # type: (datatypes.Warn2) -> None
        fdx_item = self.fdx_items_by_rte_name["NoEntryWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NoEntryWarnReq(self):
        group = self.fdx_items_by_rte_name["NoEntryWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_NoSoundSys(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["NoSoundSys"]
        return fdx_item.value_raw

    def send_NotifChkDistbn(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["NotifChkDistbn"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NotifChkDistbn(self):
        group = self.fdx_items_by_rte_name["NotifChkDistbn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NrOfBltAppld(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["NrOfBltAppld"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NrOfBltAppld(self):
        group = self.fdx_items_by_rte_name["NrOfBltAppld"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NrOfKeyAvl(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["NrOfKeyAvl"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NrOfKeyAvl(self):
        group = self.fdx_items_by_rte_name["NrOfKeyAvl"].parent_group
        self.remove_group_from_send_timer(group)

    def send_NrSerlDMSM(self, data):
        # type: (datatypes.NrSerlNodLIN) -> None
        group = self.fdx_items_by_rte_name["NrSerlDMSMNr1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["NrSerlDMSMNr1"], _get_if_not_None(data.Nr1), group)
            _ag(self.fdx_items_by_rte_name["NrSerlDMSMNr2"], _get_if_not_None(data.Nr2), group)
            _ag(self.fdx_items_by_rte_name["NrSerlDMSMNr3"], _get_if_not_None(data.Nr3), group)
            _ag(self.fdx_items_by_rte_name["NrSerlDMSMNr4"], _get_if_not_None(data.Nr4), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_NrSerlDMSM(self):
        group = self.fdx_items_by_rte_name["NrSerlDMSMNr1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_OffsForDrvrSpprtFctActvSts(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["OffsForDrvrSpprtFctActvSts"]
        return fdx_item.value_raw

    def get_OffsForSpdWarnSetg(self):
        # type: () -> datatypes.OffsForSpdWarnSetgPen
        data = datatypes.OffsForSpdWarnSetgPen()
        group = self.fdx_items_by_rte_name["OffsForSpdWarnSetgSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = r2p_float(_gg(self.fdx_items_by_rte_name['OffsForSpdWarnSetgSts'], group), 1.0, 0.0)
            data.Pen = _gg(self.fdx_items_by_rte_name['OffsForSpdWarnSetgPen'], group)
        return data

    def send_PartNrDMSM(self, data):
        # type: (datatypes.PartNrNodLIN) -> None
        group = self.fdx_items_by_rte_name["PartNrDMSMNr1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PartNrDMSMNr1"], _get_if_not_None(data.Nr1), group)
            _ag(self.fdx_items_by_rte_name["PartNrDMSMNr2"], _get_if_not_None(data.Nr2), group)
            _ag(self.fdx_items_by_rte_name["PartNrDMSMNr3"], _get_if_not_None(data.Nr3), group)
            _ag(self.fdx_items_by_rte_name["PartNrDMSMNr4"], _get_if_not_None(data.Nr4), group)
            _ag(self.fdx_items_by_rte_name["PartNrDMSMEndSgn1"], _get_if_not_None(data.EndSgn1), group)
            _ag(self.fdx_items_by_rte_name["PartNrDMSMEndSgn2"], _get_if_not_None(data.EndSgn2), group)
            _ag(self.fdx_items_by_rte_name["PartNrDMSMEndSgn3"], _get_if_not_None(data.EndSgn3), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PartNrDMSM(self):
        group = self.fdx_items_by_rte_name["PartNrDMSMNr1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PasAlrmDeactvnReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["PasAlrmDeactvnReq"]
        return fdx_item.value_raw

    def send_PasAlrmSts(self, data):
        # type: (datatypes.NoReqOffReq) -> None
        fdx_item = self.fdx_items_by_rte_name["PasAlrmSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PasAlrmSts(self):
        group = self.fdx_items_by_rte_name["PasAlrmSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PassMassgRunng(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["PassMassgRunng"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PassMassgRunng(self):
        group = self.fdx_items_by_rte_name["PassMassgRunng"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PassSeatActvSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        fdx_item = self.fdx_items_by_rte_name["PassSeatActvSpplFct"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PassSeatActvSpplFct(self):
        group = self.fdx_items_by_rte_name["PassSeatActvSpplFct"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PassSeatDispMassgFct(self):
        # type: () -> datatypes.SeatMassgFct
        data = datatypes.SeatMassgFct()
        group = self.fdx_items_by_rte_name["PassSeatDispMassgFctOnOff"].parent_group #should be same in all
        with group.mutex_lock:
            data.OnOff = _gg(self.fdx_items_by_rte_name['PassSeatDispMassgFctOnOff'], group)
            data.MassgProg = _gg(self.fdx_items_by_rte_name['PassSeatDispMassgFctMassgProg'], group)
            data.MassgInten = _gg(self.fdx_items_by_rte_name['PassSeatDispMassgFctMassgInten'], group)
            data.MassgSpdLvl = _gg(self.fdx_items_by_rte_name['PassSeatDispMassgFctMassgSpdLvl'], group)
        return data

    def get_PassSeatDispSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        fdx_item = self.fdx_items_by_rte_name["PassSeatDispSpplFct"]
        return fdx_item.value_raw

    def send_PassSeatMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        group = self.fdx_items_by_rte_name["PassSeatMassgFctOnOff"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PassSeatMassgFctOnOff"], _extract_enum_value(_get_if_not_None(data.OnOff)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatMassgFctMassgProg"], _extract_enum_value(_get_if_not_None(data.MassgProg)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatMassgFctMassgInten"], _extract_enum_value(_get_if_not_None(data.MassgInten)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatMassgFctMassgSpdLvl"], _extract_enum_value(_get_if_not_None(data.MassgSpdLvl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PassSeatMassgFct(self):
        group = self.fdx_items_by_rte_name["PassSeatMassgFctOnOff"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PassSeatSts(self, data):
        # type: (datatypes.PassSeatSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["PassSeatSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PassSeatSts(self):
        group = self.fdx_items_by_rte_name["PassSeatSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PassSeatSwtSts2(self, data):
        # type: (datatypes.PassSeatSwtSts2) -> None
        group = self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtSldSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtSldSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtSldSts)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtHeiSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtHeiSts)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtHeiFrntSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtHeiFrntSts)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtInclSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtInclSts)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtAdjmtOfSpplFctHozlSts)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtAdjmtOfSpplFctVerSts)), group)
            _ag(self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts"], _extract_enum_value(_get_if_not_None(data.PassSeatSwtSelnOfSpplFctStsSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PassSeatSwtSts2(self):
        group = self.fdx_items_by_rte_name["PassSeatSwtSts2PassSeatSwtSldSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PinionSteerAg1(self, data):
        # type: (datatypes.PinionSteerAg1Rec) -> None
        group = self.fdx_items_by_rte_name["PinionSteerAg1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PinionSteerAg1"], p2r_float(data.PinionSteerAg1, 0.0009765625, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PinionSteerAg1Qf"], _extract_enum_value(_get_if_not_None(data.PinionSteerAg1Qf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PinionSteerAg1(self):
        group = self.fdx_items_by_rte_name["PinionSteerAg1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PosnFromNav(self):
        # type: () -> datatypes.PosnFromNav
        data = datatypes.PosnFromNav()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.PosnLat = r2p_float(_gg(self.fdx_items_by_rte_name[''], group), 2.7777777777777776e-07, 0.0)
            data.PosnLgt = r2p_float(_gg(self.fdx_items_by_rte_name[''], group), 2.7777777777777776e-07, 0.0)
            data.PosnAlti = r2p_float(_gg(self.fdx_items_by_rte_name[''], group), 0.1, -100.0)
            data.PosnSpd = r2p_float(_gg(self.fdx_items_by_rte_name[''], group), 0.001, 0.0)
            data.PosnDir = r2p_float(_gg(self.fdx_items_by_rte_name[''], group), 0.01, 0.0)
            data.PosnQly = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def send_PosnFromSatlt(self, data):
        # type: (datatypes.PosnFromSatlt) -> None
        group = self.fdx_items_by_rte_name["PosnLatInPosnFromSatlt"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PosnLatInPosnFromSatlt"], p2r_float(data.PosnLat, 2.7777777777777776e-07, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PosnLgtInPosnFromSatlt"], p2r_float(data.PosnLgt, 2.7777777777777776e-07, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PosnAltiInPosnFromSatlt"], p2r_float(data.PosnAlti, 0.1, -100.0), group)
            _ag(self.fdx_items_by_rte_name["PosnSpdInPosnFromSatlt"], p2r_float(data.PosnSpd, 0.001, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PosnVHozlInPosnFromSatlt"], p2r_float(data.PosnVHozl, 0.001, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PosnVVertInPosnFromSatlt"], p2r_float(data.PosnVVert, 0.001, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PosnDirInPosnFromSatlt"], p2r_float(data.PosnDir, 0.01, 0.0), group)
            _ag(self.fdx_items_by_rte_name["TiForYrInPosnFromSatlt"], _get_if_not_None(data.TiForYr), group)
            _ag(self.fdx_items_by_rte_name["TiForMthInPosnFromSatlt"], _get_if_not_None(data.TiForMth), group)
            _ag(self.fdx_items_by_rte_name["TiForDayInPosnFromSatlt"], _get_if_not_None(data.TiForDay), group)
            _ag(self.fdx_items_by_rte_name["TiForHrInPosnFromSatlt"], _get_if_not_None(data.TiForHr), group)
            _ag(self.fdx_items_by_rte_name["TiForMinsInPosnFromSatlt"], _get_if_not_None(data.TiForMins), group)
            _ag(self.fdx_items_by_rte_name["TiForSecInPosnFromSatlt"], _get_if_not_None(data.TiForSec), group)
            _ag(self.fdx_items_by_rte_name["SatltSysNo1InUseInPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltSysNo1InUse)), group)
            _ag(self.fdx_items_by_rte_name["SatltSysNo2InUseInPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltSysNo2InUse)), group)
            _ag(self.fdx_items_by_rte_name["SatltSysNo3InUseInPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltSysNo3InUse)), group)
            _ag(self.fdx_items_by_rte_name["SatltSysNo4InUseInPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltSysNo4InUse)), group)
            _ag(self.fdx_items_by_rte_name["SatltSysNo5InUseInPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltSysNo5InUse)), group)
            _ag(self.fdx_items_by_rte_name["SatltSysNo6InUseInPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltSysNo6InUse)), group)
            _ag(self.fdx_items_by_rte_name["SatltPosnStsPrm1InPosnFromSatlt"], _extract_enum_value(_get_if_not_None(data.SatltPosnStsPrm1)), group)
            _ag(self.fdx_items_by_rte_name["SatltPosnStsPrm2InPosnFromSatlt"], _get_if_not_None(data.SatltPosnStsPrm2), group)
            _ag(self.fdx_items_by_rte_name["SatltPosnStsPrm3InPosnFromSatlt"], _get_if_not_None(data.SatltPosnStsPrm3), group)
            _ag(self.fdx_items_by_rte_name["NoOfSatltForSysNo1InPosnFromSatlt"], _get_if_not_None(data.NoOfSatltForSysNo1), group)
            _ag(self.fdx_items_by_rte_name["NoOfSatltForSysNo2InPosnFromSatlt"], _get_if_not_None(data.NoOfSatltForSysNo2), group)
            _ag(self.fdx_items_by_rte_name["NoOfSatltForSysNo3InPosnFromSatlt"], _get_if_not_None(data.NoOfSatltForSysNo3), group)
            _ag(self.fdx_items_by_rte_name["NoOfSatltForSysNo4InPosnFromSatlt"], _get_if_not_None(data.NoOfSatltForSysNo4), group)
            _ag(self.fdx_items_by_rte_name["NoOfSatltForSysNo5InPosnFromSatlt"], _get_if_not_None(data.NoOfSatltForSysNo5), group)
            _ag(self.fdx_items_by_rte_name["NoOfSatltForSysNo6InPosnFromSatlt"], _get_if_not_None(data.NoOfSatltForSysNo6), group)
            _ag(self.fdx_items_by_rte_name["PrePosnDilInPosnFromSatlt"], p2r_float(data.PrePosnDil, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PreHozlDilInPosnFromSatlt"], p2r_float(data.PreHozlDil, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PreVertDilInPosnFromSatlt"], p2r_float(data.PreVertDil, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PreTiDilInPosnFromSatlt"], p2r_float(data.PreTiDil, 0.1, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PosnFromSatlt(self):
        group = self.fdx_items_by_rte_name["PosnLatInPosnFromSatlt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PostDrvgClimaAvl(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["PostDrvgClimaAvl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PostDrvgClimaAvl(self):
        group = self.fdx_items_by_rte_name["PostDrvgClimaAvl"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PostDrvgClimaReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["ClimaEcoModRqrd"]
        return fdx_item.value_raw

    def send_PreClngNotif(self, data):
        # type: (datatypes.PreClngNotif) -> None
        fdx_item = self.fdx_items_by_rte_name["PreClngNotif"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PreClngNotif(self):
        group = self.fdx_items_by_rte_name["PreClngNotif"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgAssiActvSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiActvSts(self):
        group = self.fdx_items_by_rte_name["PrkgAssiActvSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiAudWarnFront(self, data):
        # type: (datatypes.PrkgAssiAudWarn) -> None
        group = self.fdx_items_by_rte_name["PrkgAssiAudWarnFrontAudWarnDir"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PrkgAssiAudWarnFrontAudWarnDir"], _extract_enum_value(_get_if_not_None(data.AudWarnDir)), group)
            _ag(self.fdx_items_by_rte_name["PrkgAssiAudWarnFrontAudSideWarn"], _extract_enum_value(_get_if_not_None(data.AudSideWarn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiAudWarnFront(self):
        group = self.fdx_items_by_rte_name["PrkgAssiAudWarnFrontAudWarnDir"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiAudWarnRear(self, data):
        # type: (datatypes.PrkgAssiAudWarn) -> None
        group = self.fdx_items_by_rte_name["PrkgAssiAudWarnRearAudWarnDir"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PrkgAssiAudWarnRearAudWarnDir"], _extract_enum_value(_get_if_not_None(data.AudWarnDir)), group)
            _ag(self.fdx_items_by_rte_name["PrkgAssiAudWarnRearAudSideWarn"], _extract_enum_value(_get_if_not_None(data.AudSideWarn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiAudWarnRear(self):
        group = self.fdx_items_by_rte_name["PrkgAssiAudWarnRearAudWarnDir"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiFailr(self, data):
        # type: (datatypes.PrkgAssiFailr2) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgAssiFailr"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiFailr(self):
        group = self.fdx_items_by_rte_name["PrkgAssiFailr"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiManvActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgAssiManvActvSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiManvActvSts(self):
        group = self.fdx_items_by_rte_name["PrkgAssiManvActvSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiManvProgs(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgAssiManvProgs"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiManvProgs(self):
        group = self.fdx_items_by_rte_name["PrkgAssiManvProgs"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAssiSts(self, data):
        # type: (datatypes.PrkgAssiSts3) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgAssiSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAssiSts(self):
        group = self.fdx_items_by_rte_name["PrkgAssiSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgAutSts(self, data):
        # type: (datatypes.PrkgAutSts3) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgAutSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgAutSts(self):
        group = self.fdx_items_by_rte_name["PrkgAutSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrkgCamSysAvlSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgCamSysAvlSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgCamSysAvlSts(self):
        group = self.fdx_items_by_rte_name["PrkgCamSysAvlSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PrkgOutCfm(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["PrkgOutCfm"]
        return fdx_item.value_raw

    def send_PrkgTypVld(self, data):
        # type: (datatypes.PrkgTypVld1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrkgTypVld"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrkgTypVld(self):
        group = self.fdx_items_by_rte_name["PrkgTypVld"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ProfAct(self, data):
        # type: (datatypes.ProfAct1) -> None
        group = self.fdx_items_by_rte_name["ProfActProf1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ProfActProf1"], _get_if_not_None(data.Prof1), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf2"], _get_if_not_None(data.Prof2), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf3"], _get_if_not_None(data.Prof3), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf4"], _get_if_not_None(data.Prof4), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf5"], _get_if_not_None(data.Prof5), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf6"], _get_if_not_None(data.Prof6), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf7"], _get_if_not_None(data.Prof7), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf8"], _get_if_not_None(data.Prof8), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf9"], _get_if_not_None(data.Prof9), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf10"], _get_if_not_None(data.Prof10), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf11"], _get_if_not_None(data.Prof11), group)
            _ag(self.fdx_items_by_rte_name["ProfActProf12"], _get_if_not_None(data.Prof12), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ProfAct(self):
        group = self.fdx_items_by_rte_name["ProfActProf1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_ProfChg(self):
        # type: () -> datatypes.IdPen
        fdx_item = self.fdx_items_by_rte_name["ProfChg"]
        return fdx_item.value_raw

    def send_ProfLimd(self, data):
        # type: (datatypes.ProfLimd1) -> None
        group = self.fdx_items_by_rte_name["ProfLimdProf1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ProfLimdProf1"], _get_if_not_None(data.Prof1), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf2"], _get_if_not_None(data.Prof2), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf3"], _get_if_not_None(data.Prof3), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf4"], _get_if_not_None(data.Prof4), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf5"], _get_if_not_None(data.Prof5), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf6"], _get_if_not_None(data.Prof6), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf7"], _get_if_not_None(data.Prof7), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf8"], _get_if_not_None(data.Prof8), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf9"], _get_if_not_None(data.Prof9), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf10"], _get_if_not_None(data.Prof10), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf11"], _get_if_not_None(data.Prof11), group)
            _ag(self.fdx_items_by_rte_name["ProfLimdProf12"], _get_if_not_None(data.Prof12), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ProfLimd(self):
        group = self.fdx_items_by_rte_name["ProfLimdProf1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ProfPenSts1(self, data):
        # type: (datatypes.IdPen) -> None
        fdx_item = self.fdx_items_by_rte_name["ProfPenSts1"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ProfPenSts1(self):
        group = self.fdx_items_by_rte_name["ProfPenSts1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnDrvMod(self, data):
        # type: (datatypes.PrpsnDrvMod) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnDrvMod"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnDrvMod(self):
        group = self.fdx_items_by_rte_name["PrpsnDrvMod"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnHvBattUsgModAct(self, data):
        # type: (datatypes.HvBattUsgType) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgModAct"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgModAct(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgModAct"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PrpsnHvBattUsgModReq(self):
        # type: () -> datatypes.HvBattUsgType
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgModReq"]
        return fdx_item.value_raw

    def send_PrpsnHvBattUsgOfChrgBlkd(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfChrgBlkd"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgOfChrgBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfChrgBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnHvBattUsgOfChrgBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfChrgBlkd2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgOfChrgBlkd2(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfChrgBlkd2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnHvBattUsgOfHldBlkd(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldBlkd"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgOfHldBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnHvBattUsgOfHldBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldBlkd2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgOfHldBlkd2(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldBlkd2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnHvBattUsgOfHldSmtBlkd(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldSmtBlkd"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgOfHldSmtBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldSmtBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnHvBattUsgOfHldSmtBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldSmtBlkd2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnHvBattUsgOfHldSmtBlkd2(self):
        group = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldSmtBlkd2"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PrpsnHvBattUsgOfHldSpd(self):
        # type: () -> int
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgOfHldSpd"]
        return fdx_item.value_raw

    def get_PrpsnHvBattUsgStsDispd(self):
        # type: () -> bool
        fdx_item = self.fdx_items_by_rte_name["PrpsnHvBattUsgStsDispd"]
        return fdx_item.value_raw

    def send_PrpsnModElecDrvBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnModElecDrvBlkd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnModElecDrvBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnModElecDrvBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnModOfSaveBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnModOfSaveBlkd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnModOfSaveBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnModOfSaveBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnModOfTracBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnModOfTracBlkd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnModOfTracBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnModOfTracBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnModOffroadBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnModOffroadBlkd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnModOffroadBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnModOffroadBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PrpsnModSptBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        fdx_item = self.fdx_items_by_rte_name["PrpsnModSptBlkd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PrpsnModSptBlkd(self):
        group = self.fdx_items_by_rte_name["PrpsnModSptBlkd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PtCluTq(self, data):
        # type: (datatypes.PtCluTq1) -> None
        group = self.fdx_items_by_rte_name["PtCluTqPtCluTq"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PtCluTqPtCluTq"], p2r_float(data.PtCluTq, 1.0, -1024.0), group)
            _ag(self.fdx_items_by_rte_name["PtCluTqPtCluTqDyn"], p2r_float(data.PtCluTqDyn, 1.0, -1024.0), group)
            _ag(self.fdx_items_by_rte_name["PtCluTqPtCluTqQly"], _extract_enum_value(_get_if_not_None(data.PtCluTqQly)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PtCluTq(self):
        group = self.fdx_items_by_rte_name["PtCluTqPtCluTq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_PtDrvrSetg(self):
        # type: () -> datatypes.PtDrvrSetg2
        data = datatypes.PtDrvrSetg2()
        group = self.fdx_items_by_rte_name["PtDrvrSetgPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name['PtDrvrSetgPen'], group)
            data.Sts = _gg(self.fdx_items_by_rte_name['PtDrvrSetgSts'], group)
        return data

    def send_PtGearTar(self, data):
        # type: (datatypes.PtGearAct1) -> None
        fdx_item = self.fdx_items_by_rte_name["PtGearTar"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PtGearTar(self):
        group = self.fdx_items_by_rte_name["PtGearTar"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PtTqAtWhlFrntAct(self, data):
        # type: (datatypes.PtTqAtWhlFrntActRec1) -> None
        group = self.fdx_items_by_rte_name["PtTqAtWhlFrntLeAct"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["PtTqAtWhlFrntLeAct"], p2r_float(data.PtTqAtWhlFrntLeAct, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PtTqAtWhlFrntRiAct"], p2r_float(data.PtTqAtWhlFrntRiAct, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PtTqAtAxleFrntAct"], p2r_float(data.PtTqAtAxleFrntAct, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["PtTqAtWhlsFrntQly"], _extract_enum_value(_get_if_not_None(data.PtTqAtWhlsFrntQly)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PtTqAtWhlFrntAct(self):
        group = self.fdx_items_by_rte_name["PtTqAtWhlFrntLeAct"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PwrChrgDetdForPrkgHeatrElec(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["PwrChrgDetdForPrkgHeatrElec"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PwrChrgDetdForPrkgHeatrElec(self):
        group = self.fdx_items_by_rte_name["PwrChrgDetdForPrkgHeatrElec"].parent_group
        self.remove_group_from_send_timer(group)

    def send_PwrSplyErrSts(self, data):
        # type: (datatypes.PwrSplyErrSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["PwrSplyErrSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_PwrSplyErrSts(self):
        group = self.fdx_items_by_rte_name["PwrSplyErrSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_RadioFrqAn(self):
        # type: () -> int
        fdx_item = self.fdx_items_by_rte_name["RadioFrqAn"]
        return fdx_item.value_raw

    def get_RainSenMemdReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["RainSenMemdReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['RainSenMemdReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['RainSenMemdReqPen'], group)
        return data

    def send_ReAxleWarn(self, data):
        # type: (datatypes.AxleWarn) -> None
        fdx_item = self.fdx_items_by_rte_name["ReAxleWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ReAxleWarn(self):
        group = self.fdx_items_by_rte_name["ReAxleWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RlyPwrDistbnCmd1WdBattSaveCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdBattSaveCmd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RlyPwrDistbnCmd1WdBattSaveCmd(self):
        group = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdBattSaveCmd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RlyPwrDistbnCmd1WdIgnRlyCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdIgnRlyCmd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RlyPwrDistbnCmd1WdIgnRlyCmd(self):
        group = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdIgnRlyCmd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdIgnRlyExtCmd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self):
        group = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdIgnRlyExtCmd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RlyPwrDistbnCmd1WdPreBattSaveCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdPreBattSaveCmd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RlyPwrDistbnCmd1WdPreBattSaveCmd(self):
        group = self.fdx_items_by_rte_name["RlyPwrDistbnCmd1WdPreBattSaveCmd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RmnLockgPrsnlReq(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["RmnLockgPrsnlReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RmnLockgPrsnlReq(self):
        group = self.fdx_items_by_rte_name["RmnLockgPrsnlReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_RngbdIllmnCmd(self):
        # type: () -> datatypes.RngBdIllmnCmdPen1
        data = datatypes.RngBdIllmnCmdPen1()
        group = self.fdx_items_by_rte_name["RngbdIllmnCmdSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Cmd = _gg(self.fdx_items_by_rte_name['RngbdIllmnCmdSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['RngbdIllmnCmdPen'], group)
        return data

    def get_RoadFricIndcnActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["RoadFricIndcnActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['RoadFricIndcnActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['RoadFricIndcnActvPen'], group)
        return data

    def send_RoadFricIndcnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["RoadFricIndcnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RoadFricIndcnSts(self):
        group = self.fdx_items_by_rte_name["RoadFricIndcnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RoadFricWarnReq(self, data):
        # type: (datatypes.RoadFricWarnReq1) -> None
        fdx_item = self.fdx_items_by_rte_name["RoadFricWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RoadFricWarnReq(self):
        group = self.fdx_items_by_rte_name["RoadFricWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_RoadSgnInfoActv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["RoadSgnInfoActvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['RoadSgnInfoActvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['RoadSgnInfoActvPen'], group)
        return data

    def send_RoadSgnInfoSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["RoadSgnInfoSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RoadSgnInfoSts(self):
        group = self.fdx_items_by_rte_name["RoadSgnInfoSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_RoadSpdLimActvSts(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["RoadSpdLimActvSts"]
        return fdx_item.value_raw

    def get_RoadUsrProtnActv(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["RoadUsrProtnActv"]
        return fdx_item.value_raw

    def send_RoadUsrProtnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["RoadUsrProtnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RoadUsrProtnSts(self):
        group = self.fdx_items_by_rte_name["RoadUsrProtnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RotyDirReq2(self, data):
        # type: (datatypes.RotyDirUI) -> None
        fdx_item = self.fdx_items_by_rte_name["RotyDirReq2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RotyDirReq2(self):
        group = self.fdx_items_by_rte_name["RotyDirReq2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RotyDirReq1(self, data):
        # type: (datatypes.RotyDirUI1) -> None
        fdx_item = self.fdx_items_by_rte_name["RotyDirReq1"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RotyDirReq1(self):
        group = self.fdx_items_by_rte_name["RotyDirReq1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RotyPosReq2(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["RotyPosReq2"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RotyPosReq2(self):
        group = self.fdx_items_by_rte_name["RotyPosReq2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RotyPosReq1(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["RotyPosReq1"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RotyPosReq1(self):
        group = self.fdx_items_by_rte_name["RotyPosReq1"].parent_group
        self.remove_group_from_send_timer(group)

    def get_RouteInfo(self):
        # type: () -> datatypes.RouteInfoRec1
        data = datatypes.RouteInfoRec1()
        group = self.fdx_items_by_rte_name["RouteInfoDestInfo"].parent_group #should be same in all
        with group.mutex_lock:
            data.DestInfo = _gg(self.fdx_items_by_rte_name['RouteInfoDestInfo'], group)
            data.IdOfDest = _gg(self.fdx_items_by_rte_name['RouteInfoIdOfDest'], group)
            data.DestProblty = _gg(self.fdx_items_by_rte_name['RouteInfoDestProblty'], group)
            data.NrOfDestCmpl = _gg(self.fdx_items_by_rte_name['RouteInfoNrOfDestCmpl'], group)
            data.DestSrc = _gg(self.fdx_items_by_rte_name['RouteInfoDestSrc'], group)
        return data

    def send_RsdsSysStsLe(self, data):
        # type: (datatypes.RsdsSysSts) -> None
        group = self.fdx_items_by_rte_name["RsdsSysStsLeRsdsSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeRsdsSts"], _extract_enum_value(_get_if_not_None(data.RsdsSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeLcmaSts"], _extract_enum_value(_get_if_not_None(data.LcmaSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeLcmaSts1"], _extract_enum_value(_get_if_not_None(data.LcmaSts1)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeCtraSts"], _extract_enum_value(_get_if_not_None(data.CtraSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeCtraSts1"], _extract_enum_value(_get_if_not_None(data.CtraSts1)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeRcwSts"], _extract_enum_value(_get_if_not_None(data.RcwSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsLeRcmSts"], _extract_enum_value(_get_if_not_None(data.RcmSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RsdsSysStsLe(self):
        group = self.fdx_items_by_rte_name["RsdsSysStsLeRsdsSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_RsdsSysStsRi(self, data):
        # type: (datatypes.RsdsSysSts) -> None
        group = self.fdx_items_by_rte_name["RsdsSysStsRiRsdsSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiRsdsSts"], _extract_enum_value(_get_if_not_None(data.RsdsSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiLcmaSts"], _extract_enum_value(_get_if_not_None(data.LcmaSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiLcmaSts1"], _extract_enum_value(_get_if_not_None(data.LcmaSts1)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiCtraSts"], _extract_enum_value(_get_if_not_None(data.CtraSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiCtraSts1"], _extract_enum_value(_get_if_not_None(data.CtraSts1)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiRcwSts"], _extract_enum_value(_get_if_not_None(data.RcwSts)), group)
            _ag(self.fdx_items_by_rte_name["RsdsSysStsRiRcmSts"], _extract_enum_value(_get_if_not_None(data.RcmSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_RsdsSysStsRi(self):
        group = self.fdx_items_by_rte_name["RsdsSysStsRiRsdsSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SaveSetgToMemPrmnt(self):
        # type: () -> datatypes.OffOnAut1
        fdx_item = self.fdx_items_by_rte_name["SaveSetgToMemPrmnt"]
        return fdx_item.value_raw

    def send_ScrBarVolIndcn(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["ScrBarVolIndcn"]
        fdx_item.value_raw = p2r_float(data, 5.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ScrBarVolIndcn(self):
        group = self.fdx_items_by_rte_name["ScrBarVolIndcn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ScrMaxFillgVol(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["ScrMaxFillgVol"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ScrMaxFillgVol(self):
        group = self.fdx_items_by_rte_name["ScrMaxFillgVol"].parent_group
        self.remove_group_from_send_timer(group)

    def send_ScrReagentTankVol(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["ScrReagentTankVol"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_ScrReagentTankVol(self):
        group = self.fdx_items_by_rte_name["ScrReagentTankVol"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SeatBackUnlckdThrd(self, data):
        # type: (datatypes.SeatBackUnlckd) -> None
        group = self.fdx_items_by_rte_name["SeatBackUnlckdThrdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SeatBackUnlckdThrdLe"], _get_if_not_None(data.SeatBackUnlckdLe), group)
            _ag(self.fdx_items_by_rte_name["SeatBackUnlckdThrdRi"], _get_if_not_None(data.SeatBackUnlckdRi), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SeatBackUnlckdThrd(self):
        group = self.fdx_items_by_rte_name["SeatBackUnlckdThrdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SeatDispBtnPsd(self):
        # type: () -> datatypes.SeatDispBtnPsd
        data = datatypes.SeatDispBtnPsd()
        group = self.fdx_items_by_rte_name["SeatDispBtnPsdDrvrSeatDispBtnPsd"].parent_group #should be same in all
        with group.mutex_lock:
            data.DrvrSeatDispBtnPsd = _gg(self.fdx_items_by_rte_name['SeatDispBtnPsdDrvrSeatDispBtnPsd'], group)
            data.PassSeatDispBtnPsd = _gg(self.fdx_items_by_rte_name['SeatDispBtnPsdPassSeatDispBtnPsd'], group)
        return data

    def get_SeatHeatDurgClimaEnad(self):
        # type: () -> datatypes.SeatHeatDurgClimaEnad
        fdx_item = self.fdx_items_by_rte_name["SeatHeatDurgClimaEnad"]
        return fdx_item.value_raw

    def send_SeatHeatgAutCdn(self, data):
        # type: (datatypes.Flg1) -> None
        fdx_item = self.fdx_items_by_rte_name["SeatHeatgAutCdn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SeatHeatgAutCdn(self):
        group = self.fdx_items_by_rte_name["SeatHeatgAutCdn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SeatSwtLeSigThrd(self):
        # type: () -> datatypes.SwtVertSts1
        fdx_item = self.fdx_items_by_rte_name["SeatSwtLeSigThrd"]
        return fdx_item.value_raw

    def get_SeatSwtRiSigThrd(self):
        # type: () -> datatypes.SwtVertSts1
        fdx_item = self.fdx_items_by_rte_name["SeatSwtRiSigThrd"]
        return fdx_item.value_raw

    def send_SeatVentnAutCdn(self, data):
        # type: (datatypes.Flg1) -> None
        fdx_item = self.fdx_items_by_rte_name["SeatVentnAutCdn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SeatVentnAutCdn(self):
        group = self.fdx_items_by_rte_name["SeatVentnAutCdn"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SetOfLang(self):
        # type: () -> datatypes.SetOfLang
        data = datatypes.SetOfLang()
        group = self.fdx_items_by_rte_name["SetOfLangIdPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.IdPen = _gg(self.fdx_items_by_rte_name['SetOfLangIdPen'], group)
            data.LangTyp = _gg(self.fdx_items_by_rte_name['SetOfLangLangTyp'], group)
        return data

    def send_SetOfLangAck(self, data):
        # type: (datatypes.FailrNoFailr1) -> None
        fdx_item = self.fdx_items_by_rte_name["None"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SetOfLangAck(self):
        group = self.fdx_items_by_rte_name["None"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SetTiAndDate(self):
        # type: () -> datatypes.DateTi1ForSet
        data = datatypes.DateTi1ForSet()
        group = self.fdx_items_by_rte_name["DateOrTi"].parent_group #should be same in all
        with group.mutex_lock:
            data.DateOrTi = _gg(self.fdx_items_by_rte_name['DateOrTi'], group)
            data.Year = _gg(self.fdx_items_by_rte_name['Yr1'], group)
            data.Month = _gg(self.fdx_items_by_rte_name['Mth1'], group)
            data.Day = _gg(self.fdx_items_by_rte_name['Day1'], group)
            data.Hour = _gg(self.fdx_items_by_rte_name['Hr1'], group)
            data.Minute = _gg(self.fdx_items_by_rte_name['Mins1'], group)
            data.Second = _gg(self.fdx_items_by_rte_name['Sec1'], group)
        return data

    def get_SetgAndRstOfTripForDrvr(self):
        # type: () -> datatypes.SetgAndRstOfTripForDrvr
        data = datatypes.SetgAndRstOfTripForDrvr()
        group = self.fdx_items_by_rte_name["SetgAndRstOfTripForDrvrSetgTripForDstLong"].parent_group #should be same in all
        with group.mutex_lock:
            data.SetgTripForDstLong = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForDstLong'], group)
            data.SetgTripForFuCns = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForFuCns'], group)
            data.SetgTripForFuDst = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForFuDst'], group)
            data.SetgTripForEgyDst = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForEgyDst'], group)
            data.SetgTripForDstSho1 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForDstSho1'], group)
            data.SetgTripForFuCnsAvg1 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1'], group)
            data.SetgTripForEgyCnsAvg1 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1'], group)
            data.SetgTripForSpdAvg1 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForSpdAvg1'], group)
            data.SetgTripForTiDrv1 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForTiDrv1'], group)
            data.SetgTripForDstSho2 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForDstSho2'], group)
            data.SetgTripForFuCnsAvg2 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2'], group)
            data.SetgTripForEgyCnsAvg2 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2'], group)
            data.SetgTripForSpdAvg2 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForSpdAvg2'], group)
            data.SetgTripForTiDrv2 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForTiDrv2'], group)
            data.SetgTripForDrvrMtr = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForDrvrMtr'], group)
            data.SetgTripForSpdPtr = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForSpdPtr'], group)
            data.SetgTripForSpdDig = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForSpdDig'], group)
            data.SetgTripForSpdTrvl = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForSpdTrvl'], group)
            data.SetgTripForRstOfAllTripFct1 = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1'], group)
            data.SetgTripForTiRstCdn = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripForTiRstCdn'], group)
            data.SetgTripOffOn = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripOffOn'], group)
            data.SetgTripWithProfID = _gg(self.fdx_items_by_rte_name['SetgAndRstOfTripForDrvrSetgTripWithProfID'], group)
        return data

    def send_SftyCchAccSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        group = self.fdx_items_by_rte_name["SftyCchAccStsActv"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchAccStsActv"], _extract_enum_value(_get_if_not_None(data.Actv)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchAccStsYr"], _get_if_not_None(data.Yr), group)
            _ag(self.fdx_items_by_rte_name["SftyCchAccStsMth"], _get_if_not_None(data.Mth), group)
            _ag(self.fdx_items_by_rte_name["SftyCchAccStsDay"], _get_if_not_None(data.Day), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchAccSts(self):
        group = self.fdx_items_by_rte_name["SftyCchAccStsActv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchDrvgCycIdx(self, data):
        # type: (datatypes.SftyCchIdx2) -> None
        group = self.fdx_items_by_rte_name["SftyCchDrvgCycIdxTot"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxTot"], p2r_float(data.Tot, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxTotAvl"], _extract_enum_value(_get_if_not_None(data.TotAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxTotCntxt"], _extract_enum_value(_get_if_not_None(data.TotCntxt)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxDstToVeh"], p2r_float(data.DstToVeh, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxDstToVehAvl"], _extract_enum_value(_get_if_not_None(data.DstToVehAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxDstToVehCntxt"], _extract_enum_value(_get_if_not_None(data.DstToVehCntxt)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxLaneKeep"], p2r_float(data.LaneKeep, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxLaneKeepAvl"], _extract_enum_value(_get_if_not_None(data.LaneKeepAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxLaneKeepCntxt"], _extract_enum_value(_get_if_not_None(data.LaneKeepCntxt)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxAttention"], p2r_float(data.Attention, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxAttentionAvl"], _extract_enum_value(_get_if_not_None(data.AttentionAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxAttentionCntxt"], _extract_enum_value(_get_if_not_None(data.AttentionCntxt)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxKeepSpdLim"], p2r_float(data.KeepSpdLim, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxKeepSpdLimAvl"], _extract_enum_value(_get_if_not_None(data.KeepSpdLimAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxKeepSpdLimCntxt"], _extract_enum_value(_get_if_not_None(data.KeepSpdLimCntxt)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxFldTot"], p2r_float(data.FldTot, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchDrvgCycIdxFldTotAvl"], _extract_enum_value(_get_if_not_None(data.FldTotAvl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchDrvgCycIdx(self):
        group = self.fdx_items_by_rte_name["SftyCchDrvgCycIdxTot"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchLaneKeepAidSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        group = self.fdx_items_by_rte_name["SftyCchLaneKeepAidStsActv"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchLaneKeepAidStsActv"], _extract_enum_value(_get_if_not_None(data.Actv)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLaneKeepAidStsYr"], _get_if_not_None(data.Yr), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLaneKeepAidStsMth"], _get_if_not_None(data.Mth), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLaneKeepAidStsDay"], _get_if_not_None(data.Day), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchLaneKeepAidSts(self):
        group = self.fdx_items_by_rte_name["SftyCchLaneKeepAidStsActv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchLongTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        group = self.fdx_items_by_rte_name["SftyCchLongTermIdxTot"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxTot"], p2r_float(data.Tot, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxTotAvl"], _extract_enum_value(_get_if_not_None(data.TotAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxDstToVeh"], p2r_float(data.DstToVeh, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxDstToVehAvl"], _extract_enum_value(_get_if_not_None(data.DstToVehAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxLaneKeep"], p2r_float(data.LaneKeep, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxLaneKeepAvl"], _extract_enum_value(_get_if_not_None(data.LaneKeepAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxAttention"], p2r_float(data.Attention, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxAttentionAvl"], _extract_enum_value(_get_if_not_None(data.AttentionAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxKeepSpdLim"], p2r_float(data.KeepSpdLim, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchLongTermIdxKeepSpdLimAvl"], _extract_enum_value(_get_if_not_None(data.KeepSpdLimAvl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchLongTermIdx(self):
        group = self.fdx_items_by_rte_name["SftyCchLongTermIdxTot"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SftyCchLongTermIdxRst(self):
        # type: () -> datatypes.NoYes1
        fdx_item = self.fdx_items_by_rte_name["SftyCchLongTermIdxRst"]
        return fdx_item.value_raw

    def send_SftyCchMidTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        group = self.fdx_items_by_rte_name["SftyCchMidTermIdxTot"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxTot"], p2r_float(data.Tot, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxTotAvl"], _extract_enum_value(_get_if_not_None(data.TotAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxDstToVeh"], p2r_float(data.DstToVeh, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxDstToVehAvl"], _extract_enum_value(_get_if_not_None(data.DstToVehAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxLaneKeep"], p2r_float(data.LaneKeep, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxLaneKeepAvl"], _extract_enum_value(_get_if_not_None(data.LaneKeepAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxAttention"], p2r_float(data.Attention, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxAttentionAvl"], _extract_enum_value(_get_if_not_None(data.AttentionAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxKeepSpdLim"], p2r_float(data.KeepSpdLim, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchMidTermIdxKeepSpdLimAvl"], _extract_enum_value(_get_if_not_None(data.KeepSpdLimAvl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchMidTermIdx(self):
        group = self.fdx_items_by_rte_name["SftyCchMidTermIdxTot"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchPilotAssiSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        group = self.fdx_items_by_rte_name["SftyCchPilotAssiStsActv"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchPilotAssiStsActv"], _extract_enum_value(_get_if_not_None(data.Actv)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchPilotAssiStsYr"], _get_if_not_None(data.Yr), group)
            _ag(self.fdx_items_by_rte_name["SftyCchPilotAssiStsMth"], _get_if_not_None(data.Mth), group)
            _ag(self.fdx_items_by_rte_name["SftyCchPilotAssiStsDay"], _get_if_not_None(data.Day), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchPilotAssiSts(self):
        group = self.fdx_items_by_rte_name["SftyCchPilotAssiStsActv"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SftyCchPostTripSts(self):
        # type: () -> datatypes.SftyCchPostTripSts1
        fdx_item = self.fdx_items_by_rte_name["SftyCchPostTripSts"]
        return fdx_item.value_raw

    def send_SftyCchPrimFbAftDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        fdx_item = self.fdx_items_by_rte_name["SftyCchPrimFbAftDrvg"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchPrimFbAftDrvg(self):
        group = self.fdx_items_by_rte_name["SftyCchPrimFbAftDrvg"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchPrimFbDurgDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        fdx_item = self.fdx_items_by_rte_name["SftyCchPrimFbDurgDrvg"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchPrimFbDurgDrvg(self):
        group = self.fdx_items_by_rte_name["SftyCchPrimFbDurgDrvg"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchSecFbAftDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        fdx_item = self.fdx_items_by_rte_name["SftyCchSecFbAftDrvg"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchSecFbAftDrvg(self):
        group = self.fdx_items_by_rte_name["SftyCchSecFbAftDrvg"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchSecFbDurgDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        fdx_item = self.fdx_items_by_rte_name["SftyCchSecFbDurgDrvg"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchSecFbDurgDrvg(self):
        group = self.fdx_items_by_rte_name["SftyCchSecFbDurgDrvg"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchShortTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        group = self.fdx_items_by_rte_name["SftyCchShortTermIdxTot"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxTot"], p2r_float(data.Tot, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxTotAvl"], _extract_enum_value(_get_if_not_None(data.TotAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxDstToVeh"], p2r_float(data.DstToVeh, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxDstToVehAvl"], _extract_enum_value(_get_if_not_None(data.DstToVehAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxLaneKeep"], p2r_float(data.LaneKeep, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxLaneKeepAvl"], _extract_enum_value(_get_if_not_None(data.LaneKeepAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxAttention"], p2r_float(data.Attention, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxAttentionAvl"], _extract_enum_value(_get_if_not_None(data.AttentionAvl)), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxKeepSpdLim"], p2r_float(data.KeepSpdLim, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SftyCchShortTermIdxKeepSpdLimAvl"], _extract_enum_value(_get_if_not_None(data.KeepSpdLimAvl)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchShortTermIdx(self):
        group = self.fdx_items_by_rte_name["SftyCchShortTermIdxTot"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SftyCchSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["SftyCchSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyCchSts(self):
        group = self.fdx_items_by_rte_name["SftyCchSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SftySigGroupFromAudSafe(self):
        # type: () -> datatypes.SftySigGroupFromAudSafe1
        data = datatypes.SftySigGroupFromAudSafe1()
        group = self.fdx_items_by_rte_name["SftySigFaildDetdByAud"].parent_group #should be same in all
        with group.mutex_lock:
            data.SftySigFaildDetdByAud = _gg(self.fdx_items_by_rte_name['SftySigFaildDetdByAud'], group)
            data.SftyAudDend = _gg(self.fdx_items_by_rte_name['SftyAudDend'], group)
            data.SftyAudEna = _gg(self.fdx_items_by_rte_name['SftySigGroupFromAudSafeSftyAudEna'], group)
        return data

    def send_SftyWarnGroupFromAsySafe(self, data):
        # type: (datatypes.SftyWarnGroupFromAsySafe1) -> None
        group = self.fdx_items_by_rte_name["CnclWarnLatForAutDrv"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["CnclWarnLatForAutDrv"], _extract_enum_value(_get_if_not_None(data.CnclWarnLatForAutDrv)), group)
            _ag(self.fdx_items_by_rte_name["SteerOvrdWarnReqForAutDrv"], _extract_enum_value(_get_if_not_None(data.SteerOvrdWarnReqForAutDrv)), group)
            _ag(self.fdx_items_by_rte_name["SteerStsForAutDrv"], _extract_enum_value(_get_if_not_None(data.SteerStsForAutDrv)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SftyWarnGroupFromAsySafe(self):
        group = self.fdx_items_by_rte_name["CnclWarnLatForAutDrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SnsrParkAssiFrnt(self, data):
        # type: (datatypes.SnsrParkAssi1) -> None
        group = self.fdx_items_by_rte_name["ParkAssiDstFrntOfSnsrInsdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfSnsrInsdLe"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrInsdLe)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfSnsrOutdLe"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrOutdLe)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfDstOfSnsrLeSide"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrLeSide)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfDstOfSnsrInsdRi"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrInsdRi)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfDstOfSnsrOutdRi"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrOutdRi)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfDstOfSnsrRiSide"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrRiSide)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfAudWarnDir"], _extract_enum_value(_get_if_not_None(data.AudWarnDir)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstFrntOfAudSideWarn"], _extract_enum_value(_get_if_not_None(data.AudSideWarn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SnsrParkAssiFrnt(self):
        group = self.fdx_items_by_rte_name["ParkAssiDstFrntOfSnsrInsdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SnsrParkAssiRe(self, data):
        # type: (datatypes.SnsrParkAssi1) -> None
        group = self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrInsdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrInsdLe"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrInsdLe)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrOutdLe"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrOutdLe)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrLeSide"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrLeSide)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrInsdRi"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrInsdRi)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrOutdRi"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrOutdRi)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrRiSide"], _extract_enum_value(_get_if_not_None(data.DstOfSnsrRiSide)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfAudWarnDir"], _extract_enum_value(_get_if_not_None(data.AudWarnDir)), group)
            _ag(self.fdx_items_by_rte_name["ParkAssiDstReOfAudSideWarn"], _extract_enum_value(_get_if_not_None(data.AudSideWarn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SnsrParkAssiRe(self):
        group = self.fdx_items_by_rte_name["ParkAssiDstReOfDstOfSnsrInsdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SnsrPrkgAssiFrnt(self, data):
        # type: (datatypes.SnsrPrkgAssi2) -> None
        group = self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstInsdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstInsdLe"], p2r_float(data.SnsrDstInsdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstOutdLe"], p2r_float(data.SnsrDstOutdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstSideLe"], p2r_float(data.SnsrDstSideLe, -1.0, 511.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstInsdRi"], p2r_float(data.SnsrDstInsdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstOutdRi"], p2r_float(data.SnsrDstOutdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstSideRi"], p2r_float(data.SnsrDstSideRi, -1.0, 511.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntAudWarnDir"], _extract_enum_value(_get_if_not_None(data.AudWarnDir)), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiFrntAudSideWarn"], _extract_enum_value(_get_if_not_None(data.AudSideWarn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SnsrPrkgAssiFrnt(self):
        group = self.fdx_items_by_rte_name["SnsrPrkgAssiFrntSnsrDstInsdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SnsrPrkgAssiRe(self, data):
        # type: (datatypes.SnsrPrkgAssi2) -> None
        group = self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstInsdLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstInsdLe"], p2r_float(data.SnsrDstInsdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstOutdLe"], p2r_float(data.SnsrDstOutdLe, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstSideLe"], p2r_float(data.SnsrDstSideLe, -1.0, 511.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstInsdRi"], p2r_float(data.SnsrDstInsdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstOutdRi"], p2r_float(data.SnsrDstOutdRi, -1.0, 1023.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstSideRi"], p2r_float(data.SnsrDstSideRi, -1.0, 511.0), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReAudWarnDir"], _extract_enum_value(_get_if_not_None(data.AudWarnDir)), group)
            _ag(self.fdx_items_by_rte_name["SnsrPrkgAssiReAudSideWarn"], _extract_enum_value(_get_if_not_None(data.AudSideWarn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SnsrPrkgAssiRe(self):
        group = self.fdx_items_by_rte_name["SnsrPrkgAssiReSnsrDstInsdLe"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SoundExtActvSts(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["SoundExtActvSts"]
        return fdx_item.value_raw

    def send_WhlMotSysSpdAct(self, data):
        # type: (float) -> None
        fdx_item = self.fdx_items_by_rte_name["WhlMotSysSpdAct"]
        fdx_item.value_raw = p2r_float(data, 0.1, -1638.4)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WhlMotSysSpdAct(self):
        group = self.fdx_items_by_rte_name["WhlMotSysSpdAct"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SpdAlrmActvForRoadSgnInfo(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["SpdAlrmActvForRoadSgnInfoSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['SpdAlrmActvForRoadSgnInfoSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['SpdAlrmActvForRoadSgnInfoPen'], group)
        return data

    def get_SpdCameraInfoSeldForRoadSgnInfo(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["SpdCameraInfoSeldForRoadSgnInfoSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['SpdCameraInfoSeldForRoadSgnInfoSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['SpdCameraInfoSeldForRoadSgnInfoPen'], group)
        return data

    def send_SpdLimAdpvSts(self, data):
        # type: (datatypes.OffOnNotAvlSrvRqrd) -> None
        fdx_item = self.fdx_items_by_rte_name["SpdLimAdpvSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SpdLimAdpvSts(self):
        group = self.fdx_items_by_rte_name["SpdLimAdpvSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SpdLimWarnReq(self, data):
        # type: (datatypes.SpdLimWarnReq1) -> None
        fdx_item = self.fdx_items_by_rte_name["SpdLimWarnReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SpdLimWarnReq(self):
        group = self.fdx_items_by_rte_name["SpdLimWarnReq"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SpdWarn(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["SpdWarn"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SpdWarn(self):
        group = self.fdx_items_by_rte_name["SpdWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SrvRst(self, data):
        # type: (datatypes.Trig1) -> None
        fdx_item = self.fdx_items_by_rte_name["SrvRst"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SrvRst(self):
        group = self.fdx_items_by_rte_name["SrvRst"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SrvTrig(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["SrvTrig"]
        fdx_item.value_raw = p2r_float(data, 1.0, 0.0)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SrvTrig(self):
        group = self.fdx_items_by_rte_name["SrvTrig"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SteerAssiActvForAutDrv(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["SteerAssiActvForAutDrvSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['SteerAssiActvForAutDrvSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['SteerAssiActvForAutDrvPen'], group)
        return data

    def get_SteerAssiLvlForAutDrv(self):
        # type: () -> datatypes.SteerAssiLvlForAutDrvPen1
        data = datatypes.SteerAssiLvlForAutDrvPen1()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name[''], group)
            data.Sts = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def get_SteerSetg(self):
        # type: () -> datatypes.SteerSetg
        data = datatypes.SteerSetg()
        group = self.fdx_items_by_rte_name["SteerAsscLvl"].parent_group #should be same in all
        with group.mutex_lock:
            data.SteerAsscLvl = _gg(self.fdx_items_by_rte_name['SteerAsscLvl'], group)
            data.SteerMod = _gg(self.fdx_items_by_rte_name['SteerMod'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['SteerSetgPen'], group)
        return data

    def send_SteerWarnReqForAutDrv(self, data):
        # type: (datatypes.SteerWarnReqForAutDrv1) -> None
        fdx_item = self.fdx_items_by_rte_name["SteerWarnReqForAutDrv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SteerWarnReqForAutDrv(self):
        group = self.fdx_items_by_rte_name["SteerWarnReqForAutDrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SteerWarnReqForLaneKeepAid(self, data):
        # type: (datatypes.WarnLeRi) -> None
        fdx_item = self.fdx_items_by_rte_name["SteerWarnReqForLaneKeepAid"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SteerWarnReqForLaneKeepAid(self):
        group = self.fdx_items_by_rte_name["SteerWarnReqForLaneKeepAid"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SteerWhlHeatgDurgClimaEnad(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["SteerWhlHeatgDurgClimaEnad"]
        return fdx_item.value_raw

    def get_SteerWhlHeatgOnReq(self):
        # type: () -> datatypes.SteerWhlHeatgOnCmdTyp
        fdx_item = self.fdx_items_by_rte_name["SteerWhlHeatgOnReq"]
        return fdx_item.value_raw

    def send_SteerWhlHeatgStrtAutCdnOk(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["SteerWhlHeatgStrtAutCdnOk"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SteerWhlHeatgStrtAutCdnOk(self):
        group = self.fdx_items_by_rte_name["SteerWhlHeatgStrtAutCdnOk"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SteerWhlSnsrSafe(self, data):
        # type: (datatypes.SteerWhlSnsrSafe1) -> None
        group = self.fdx_items_by_rte_name["SteerWhlAgSafe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["SteerWhlAgSafe"], p2r_float(data.SteerWhlAg, 0.0009765625, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SteerWhlAgSpdSafe"], p2r_float(data.SteerWhlAgSpd, 0.0078125, 0.0), group)
            _ag(self.fdx_items_by_rte_name["SteerWhlSnsrQf"], _extract_enum_value(_get_if_not_None(data.SteerWhlSnsrQf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SteerWhlSnsrSafe(self):
        group = self.fdx_items_by_rte_name["SteerWhlAgSafe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SteerWhlSwtPwr(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["SteerWhlSwtPwr"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SteerWhlSwtPwr(self):
        group = self.fdx_items_by_rte_name["SteerWhlSwtPwr"].parent_group
        self.remove_group_from_send_timer(group)

    def send_StopStrtInhb(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["StopStrtInhb"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_StopStrtInhb(self):
        group = self.fdx_items_by_rte_name["StopStrtInhb"].parent_group
        self.remove_group_from_send_timer(group)

    def send_StrtInProgs(self, data):
        # type: (datatypes.StrtInProgs1) -> None
        fdx_item = self.fdx_items_by_rte_name["StrtInProgs"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_StrtInProgs(self):
        group = self.fdx_items_by_rte_name["StrtInProgs"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SunRoofPosnSts(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["SunRoofPosnSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SunRoofPosnSts(self):
        group = self.fdx_items_by_rte_name["SunRoofPosnSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SuspSetg(self):
        # type: () -> datatypes.SuspSetgRec
        data = datatypes.SuspSetgRec()
        group = self.fdx_items_by_rte_name["SuspIdPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.SuspIdPen = _gg(self.fdx_items_by_rte_name['SuspIdPen'], group)
            data.LvlCtrlSetg = _gg(self.fdx_items_by_rte_name['LvlCtrlSetg'], group)
            data.SuspCtrlSetg = _gg(self.fdx_items_by_rte_name['SuspCtrlSetg'], group)
        return data

    def send_SwtAcptReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        fdx_item = self.fdx_items_by_rte_name["SwtAcptReq"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SwtAcptReq(self):
        group = self.fdx_items_by_rte_name["SwtAcptReq"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SwtAtCenSts(self):
        # type: () -> datatypes.Trig1
        fdx_item = self.fdx_items_by_rte_name["SwtAtCenSts"]
        return fdx_item.value_raw

    def send_SwtForPassAirbCutOffSt(self, data):
        # type: (datatypes.OnOff2) -> None
        fdx_item = self.fdx_items_by_rte_name["SwtForPassAirbCutOffSt"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SwtForPassAirbCutOffSt(self):
        group = self.fdx_items_by_rte_name["SwtForPassAirbCutOffSt"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TankFlapSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["TankFlapSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TankFlapSts(self):
        group = self.fdx_items_by_rte_name["TankFlapSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TelmSts(self, data):
        # type: (datatypes.TelmSts) -> None
        fdx_item = self.fdx_items_by_rte_name["TelmSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TelmSts(self):
        group = self.fdx_items_by_rte_name["TelmSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TelmStsConnGen2(self, data):
        # type: (datatypes.TelmSts) -> None
        fdx_item = self.fdx_items_by_rte_name["TelmStsConnGen2"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TelmStsConnGen2(self):
        group = self.fdx_items_by_rte_name["TelmStsConnGen2"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TiAndDateIndcn(self, data):
        # type: (datatypes.DateTi30) -> None
        group = self.fdx_items_by_rte_name["Yr"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["Yr"], _get_if_not_None(data.Yr1), group)
            _ag(self.fdx_items_by_rte_name["Mth"], _get_if_not_None(data.Mth1), group)
            _ag(self.fdx_items_by_rte_name["Day"], _get_if_not_None(data.Day), group)
            _ag(self.fdx_items_by_rte_name["Hr"], _get_if_not_None(data.Hr1), group)
            _ag(self.fdx_items_by_rte_name["Mins"], _get_if_not_None(data.Mins1), group)
            _ag(self.fdx_items_by_rte_name["Sec"], _get_if_not_None(data.Sec1), group)
            _ag(self.fdx_items_by_rte_name["TiAndDateVld"], _extract_enum_value(_get_if_not_None(data.DataValid)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TiAndDateIndcn(self):
        group = self.fdx_items_by_rte_name["Yr"].parent_group
        self.remove_group_from_send_timer(group)

    def get_TiGapAdpvSeld(self):
        # type: () -> datatypes.TiGapAdpvSeldPen1
        data = datatypes.TiGapAdpvSeldPen1()
        group = self.fdx_items_by_rte_name["TiGapAdpvSeldSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['TiGapAdpvSeldSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['TiGapAdpvSeldPen'], group)
        return data

    def get_TiGapLimd(self):
        # type: () -> datatypes.TiGapPen1
        data = datatypes.TiGapPen1()
        group = self.fdx_items_by_rte_name["TiGapLimdSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = r2p_float(_gg(self.fdx_items_by_rte_name['TiGapLimdSts'], group), 0.1, 0.0)
            data.Pen = _gg(self.fdx_items_by_rte_name['TiGapLimdPen'], group)
        return data

    def get_TireCircumCalByNav(self):
        # type: () -> datatypes.TireCircumCalByNav
        data = datatypes.TireCircumCalByNav()
        group = self.fdx_items_by_rte_name["TireCircumInTireCircumCalByNav"].parent_group #should be same in all
        with group.mutex_lock:
            data.TireCircum = _gg(self.fdx_items_by_rte_name['TireCircumInTireCircumCalByNav'], group)
            data.HiQly = _gg(self.fdx_items_by_rte_name['HiQlyInTireCircumCalByNav'], group)
            data.SpprtForFct = _gg(self.fdx_items_by_rte_name['SpprtForFctInTireCircumCalByNav'], group)
        return data

    def get_TireMonCalReq(self):
        # type: () -> datatypes.StrtStopReq1
        fdx_item = self.fdx_items_by_rte_name["TireMonCalReq"]
        return fdx_item.value_raw

    def get_TireMonrDispReq(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["TireMonrDispReq"]
        return fdx_item.value_raw

    def send_TirePAbsltValFrntRi(self, data):
        # type: (datatypes.TirePAbsltValFrntRi1) -> None
        group = self.fdx_items_by_rte_name["TirePAbsltValFrntRiTirePAbsltVal1"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["TirePAbsltValFrntRiTirePAbsltVal1"], p2r_float(data.TirePAbsltVal1, 2.5, 0.0), group)
            _ag(self.fdx_items_by_rte_name["TirePAbsltValQFForFrnRi"], _extract_enum_value(_get_if_not_None(data.TirePAbsltValQF)), group)
            _ag(self.fdx_items_by_rte_name["TirePAbsltValFrntRiTirePPosn"], _extract_enum_value(_get_if_not_None(data.TirePPosn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TirePAbsltValFrntRi(self):
        group = self.fdx_items_by_rte_name["TirePAbsltValFrntRiTirePAbsltVal1"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TirePAbsltValFrntLe(self, data):
        # type: (datatypes.TirePAbsltValFrntLe3) -> None
        group = self.fdx_items_by_rte_name["TirePAbsltValForFrntLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["TirePAbsltValForFrntLe"], p2r_float(data.TirepabsltVal1, 2.5, 0.0), group)
            _ag(self.fdx_items_by_rte_name["TirePAbsltVaQflForFrntLe"], _extract_enum_value(_get_if_not_None(data.TirePabsltValQF)), group)
            _ag(self.fdx_items_by_rte_name["TirePPosnForFrntLe"], _extract_enum_value(_get_if_not_None(data.TirePPosn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TirePAbsltValFrntLe(self):
        group = self.fdx_items_by_rte_name["TirePAbsltValForFrntLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TirePAbsltValReLe(self, data):
        # type: (datatypes.TirePAbsltValReLe1) -> None
        group = self.fdx_items_by_rte_name["TirePAbsltValForReLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["TirePAbsltValForReLe"], p2r_float(data.TirePAbsltVal1, 2.5, 0.0), group)
            _ag(self.fdx_items_by_rte_name["TirePAbsltValQfForReLe"], _extract_enum_value(_get_if_not_None(data.TirePAbsltValQF)), group)
            _ag(self.fdx_items_by_rte_name["TirePPosnForReLe"], _extract_enum_value(_get_if_not_None(data.TirePPosn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TirePAbsltValReLe(self):
        group = self.fdx_items_by_rte_name["TirePAbsltValForReLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TirePAbsltValReRi(self, data):
        # type: (datatypes.TirePAbsltValReRi1) -> None
        group = self.fdx_items_by_rte_name["TirePAbsltValForReRi"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["TirePAbsltValForReRi"], p2r_float(data.TirePAbsltVal1, 2.5, 0.0), group)
            _ag(self.fdx_items_by_rte_name["TirePAbsltValQfForReRi"], _extract_enum_value(_get_if_not_None(data.TirePAbsltValQF)), group)
            _ag(self.fdx_items_by_rte_name["TirePPosnForReRi"], _extract_enum_value(_get_if_not_None(data.TirePPosn)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TirePAbsltValReRi(self):
        group = self.fdx_items_by_rte_name["TirePAbsltValForReRi"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TirePCalSts(self, data):
        # type: (datatypes.TirePCalSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["TirePCalSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TirePCalSts(self):
        group = self.fdx_items_by_rte_name["TirePCalSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TirePMonData(self, data):
        # type: (datatypes.TirePMonData1) -> None
        group = self.fdx_items_by_rte_name["TirePMonSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["TirePMonSts"], _extract_enum_value(_get_if_not_None(data.TirePMonSts1)), group)
            _ag(self.fdx_items_by_rte_name["TirePReRi"], _extract_enum_value(_get_if_not_None(data.TirePReRi)), group)
            _ag(self.fdx_items_by_rte_name["TirePReLe"], _extract_enum_value(_get_if_not_None(data.TirePReLe)), group)
            _ag(self.fdx_items_by_rte_name["TirePFrntLe"], _extract_enum_value(_get_if_not_None(data.TirePFrntLe)), group)
            _ag(self.fdx_items_by_rte_name["TirePFrntRi"], _extract_enum_value(_get_if_not_None(data.TirePFrntRe)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TirePMonData(self):
        group = self.fdx_items_by_rte_name["TirePMonSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TnlEntryOrEnd(self, data):
        # type: (datatypes.NoYes1) -> None
        fdx_item = self.fdx_items_by_rte_name["TnlEntryOrEnd"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TnlEntryOrEnd(self):
        group = self.fdx_items_by_rte_name["TnlEntryOrEnd"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TrSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["TrSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TrSts(self):
        group = self.fdx_items_by_rte_name["TrSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_TrfcLiSpprtActv(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["TrfcLiSpprtActv"]
        return fdx_item.value_raw

    def send_TrfcLiSpprtSts(self, data):
        # type: (datatypes.FctSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["TrfcLiSpprtSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TrfcLiSpprtSts(self):
        group = self.fdx_items_by_rte_name["TrfcLiSpprtSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_TrfcRglnSeldForRoadSgnInfo(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["TrfcRglnSeldForRoadSgnInfoSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['TrfcRglnSeldForRoadSgnInfoSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['TrfcRglnSeldForRoadSgnInfoPen'], group)
        return data

    def get_TripModSeln(self):
        # type: () -> datatypes.TripModSeln
        fdx_item = self.fdx_items_by_rte_name["TripModSeln"]
        return fdx_item.value_raw

    def send_TrlrLampChkSts1WdSts1(self, data):
        # type: (datatypes.TrlrLampChkSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["TrlrLampActvtChk"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TrlrLampChkSts1WdSts1(self):
        group = self.fdx_items_by_rte_name["TrlrLampActvtChk"].parent_group
        self.remove_group_from_send_timer(group)

    def get_TrlrLampChkAutReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["TrlrLampChkAutReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['TrlrLampChkAutReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['TrlrLampChkAutReqPen'], group)
        return data

    def get_TrlrLampChkRemReq(self):
        # type: () -> datatypes.StrtStopReq1
        fdx_item = self.fdx_items_by_rte_name["TrlrLampChkRemReq"]
        return fdx_item.value_raw

    def send_TrlrLampChkSts1WdSts2(self, data):
        # type: (datatypes.TrlrLampChkSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["TrlrLampChkSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TrlrLampChkSts1WdSts2(self):
        group = self.fdx_items_by_rte_name["TrlrLampChkSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TrlrPrsntSts1WdSts(self, data):
        # type: (datatypes.TrlrPrsntSts) -> None
        fdx_item = self.fdx_items_by_rte_name["TrlrPrsnt"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TrlrPrsntSts1WdSts(self):
        group = self.fdx_items_by_rte_name["TrlrPrsnt"].parent_group
        self.remove_group_from_send_timer(group)

    def get_LiExtReq1WdReq3(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["TurnAutFlsgReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['TurnAutFlsgReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['TurnAutFlsgReqPen'], group)
        return data

    def send_TwliBriRaw(self, data):
        # type: (datatypes.TwliBriRaw) -> None
        group = self.fdx_items_by_rte_name["TwliBriRaw"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["TwliBriRaw"], _get_if_not_None(data.TwliBriRaw1), group)
            _ag(self.fdx_items_by_rte_name["TwliBriRawQf"], _extract_enum_value(_get_if_not_None(data.TwliBriRawQf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TwliBriRaw(self):
        group = self.fdx_items_by_rte_name["TwliBriRaw"].parent_group
        self.remove_group_from_send_timer(group)

    def send_TwliBriSts(self, data):
        # type: (datatypes.TwliBriSts1) -> None
        fdx_item = self.fdx_items_by_rte_name["TwliBriSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_TwliBriSts(self):
        group = self.fdx_items_by_rte_name["TwliBriSts"].parent_group
        self.remove_group_from_send_timer(group)

    def get_TwliBriStsForSeatHeatrRe(self):
        # type: () -> datatypes.TwliBriSts1
        fdx_item = self.fdx_items_by_rte_name["TwliBriStsForSeatHeatrRe"]
        return fdx_item.value_raw

    def send_UkwnCptReqToInfoMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        group = self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte0"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte0"], _get_if_not_None(data.Byte0), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte1"], _get_if_not_None(data.Byte1), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte2"], _get_if_not_None(data.Byte2), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte3"], _get_if_not_None(data.Byte3), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte4"], _get_if_not_None(data.Byte4), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte5"], _get_if_not_None(data.Byte5), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte6"], _get_if_not_None(data.Byte6), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte7"], _get_if_not_None(data.Byte7), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_UkwnCptReqToInfoMgr(self):
        group = self.fdx_items_by_rte_name["UkwnCptReqToInfoMgrByte0"].parent_group
        self.remove_group_from_send_timer(group)

    def send_UkwnCptReqToSoundMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        group = self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte0"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte0"], _get_if_not_None(data.Byte0), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte1"], _get_if_not_None(data.Byte1), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte2"], _get_if_not_None(data.Byte2), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte3"], _get_if_not_None(data.Byte3), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte4"], _get_if_not_None(data.Byte4), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte5"], _get_if_not_None(data.Byte5), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte6"], _get_if_not_None(data.Byte6), group)
            _ag(self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte7"], _get_if_not_None(data.Byte7), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_UkwnCptReqToSoundMgr(self):
        group = self.fdx_items_by_rte_name["UkwnCptReqToSoundMgrByte0"].parent_group
        self.remove_group_from_send_timer(group)

    def get_UkwnCptRespFromInfoMgr(self):
        # type: () -> datatypes.UInt64Rec
        data = datatypes.UInt64Rec()
        group = self.fdx_items_by_rte_name["UkwnCptRespFromInfoMgrByte0"].parent_group #should be same in all
        with group.mutex_lock:
            data.Byte0 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte0'], group)
            data.Byte1 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte1'], group)
            data.Byte2 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte2'], group)
            data.Byte3 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte3'], group)
            data.Byte4 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte4'], group)
            data.Byte5 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte5'], group)
            data.Byte6 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte6'], group)
            data.Byte7 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromInfoMgrByte7'], group)
        return data

    def get_UkwnCptRespFromSoundMgr(self):
        # type: () -> datatypes.UInt64Rec
        data = datatypes.UInt64Rec()
        group = self.fdx_items_by_rte_name["UkwnCptRespFromSoundMgrByte0"].parent_group #should be same in all
        with group.mutex_lock:
            data.Byte0 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte0'], group)
            data.Byte1 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte1'], group)
            data.Byte2 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte2'], group)
            data.Byte3 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte3'], group)
            data.Byte4 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte4'], group)
            data.Byte5 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte5'], group)
            data.Byte6 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte6'], group)
            data.Byte7 = _gg(self.fdx_items_by_rte_name['UkwnCptRespFromSoundMgrByte7'], group)
        return data

    def get_UnlckFbVisReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["UnlckFbVisReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['UnlckFbVisReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['UnlckFbVisReqPen'], group)
        return data

    def get_UnlckKeylsReq(self):
        # type: () -> datatypes.UnlckKeylsCfgPen2
        data = datatypes.UnlckKeylsCfgPen2()
        group = self.fdx_items_by_rte_name["UnlckKeylsReqPen"].parent_group #should be same in all
        with group.mutex_lock:
            data.IdPen = _gg(self.fdx_items_by_rte_name['UnlckKeylsReqPen'], group)
            data.KeylsCfg = _gg(self.fdx_items_by_rte_name['UnlckKeylsReqSts'], group)
        return data

    def get_UnlckRemReq(self):
        # type: () -> datatypes.UnlckRemCfgPen1
        data = datatypes.UnlckRemCfgPen1()
        group = self.fdx_items_by_rte_name["UnlckRemReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['UnlckRemReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['UnlckRemReqPen'], group)
        return data

    def get_UsrSeldDrvrSpprtFct(self):
        # type: () -> datatypes.DrvrSpprtFct
        fdx_item = self.fdx_items_by_rte_name["UsrSeldDrvrSpprtFct"]
        return fdx_item.value_raw

    def get_UsrSetSpdForKeySpdLimn(self):
        # type: () -> datatypes.UsrSetSpdForKeySpdLimn
        data = datatypes.UsrSetSpdForKeySpdLimn()
        group = self.fdx_items_by_rte_name["UsrSetSpdForKeySpdLimnSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdLimnSts'], group), 0.03125, 0.0)
            data.Pen = _gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdLimnPen'], group)
        return data

    def get_UsrSetSpdForKeySpdWarn(self):
        # type: () -> datatypes.UsrSetSpdForKeySpdWarn
        data = datatypes.UsrSetSpdForKeySpdWarn()
        group = self.fdx_items_by_rte_name["UsrSetSpdForKeySpdWarn1"].parent_group #should be same in all
        with group.mutex_lock:
            data.UsrSetSpdForKeySpdWarn1 = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarn1'], group), 0.03125, 0.0)
            data.UsrSetSpdForKeySpdWarn2 = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarn2'], group), 0.03125, 0.0)
            data.UsrSetSpdForKeySpdWarn3 = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarn3'], group), 0.03125, 0.0)
            data.UsrSetSpdForKeySpdWarn4 = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarn4'], group), 0.03125, 0.0)
            data.UsrSetSpdForKeySpdWarn5 = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarn5'], group), 0.03125, 0.0)
            data.UsrSetSpdForKeySpdWarn6 = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarn6'], group), 0.03125, 0.0)
            data.UsrSetSpdForKeySpdWarnPen = _gg(self.fdx_items_by_rte_name['UsrSetSpdForKeySpdWarnPen'], group)
        return data

    def get_UsrSetVolMaxForKeyVolLimn(self):
        # type: () -> datatypes.UsrSetVolMaxForKeyVolLimn
        data = datatypes.UsrSetVolMaxForKeyVolLimn()
        group = self.fdx_items_by_rte_name[""].parent_group #should be same in all
        with group.mutex_lock:
            data.Pen = _gg(self.fdx_items_by_rte_name[''], group)
            data.SetVol = _gg(self.fdx_items_by_rte_name[''], group)
        return data

    def get_UsrSwtDispClimaReq(self):
        # type: () -> datatypes.OnOffIdle1
        fdx_item = self.fdx_items_by_rte_name["UsrSwtDispClimaReq"]
        return fdx_item.value_raw

    def send_UsrSwtDispClimaReqForRowSec(self, data):
        # type: (datatypes.UsrSwtDispClimaReqForRowSec) -> None
        group = self.fdx_items_by_rte_name["UsrSwtDispClimaReqForTSpForRowSecLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["UsrSwtDispClimaReqForTSpForRowSecLe"], p2r_float(data.UsrSwtDispClimaReqForTSpForRowSecLe, 0.5, 15.0), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispUpdClimaReqForTSpForRowSecLe"], _get_if_not_None(data.UsrSwtDispUpdClimaReqForTSpForRowSecLe), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispClimaReqForTSpForRowSecRi"], p2r_float(data.UsrSwtDispClimaReqForTSpForRowSecRi, 0.5, 15.0), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispUpdClimaReqForTSpForRowSecRi"], _get_if_not_None(data.UsrSwtDispUpdClimaReqForTSpForRowSecRi), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe)), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe"], _get_if_not_None(data.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi"], _get_if_not_None(data.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi)), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispReqForFanLvlForRowSec"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispReqForFanLvlForRowSec)), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispUpdReqForFanLvlForRowSec"], _get_if_not_None(data.UsrSwtDispUpdReqForFanLvlForRowSec), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispClimaReqForTSpSpclForRowSecLe"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispClimaReqForTSpSpclForRowSecLe)), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispClimaReqForTSpSpclForRowSecRi"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispClimaReqForTSpSpclForRowSecRi)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_UsrSwtDispClimaReqForRowSec(self):
        group = self.fdx_items_by_rte_name["UsrSwtDispClimaReqForTSpForRowSecLe"].parent_group
        self.remove_group_from_send_timer(group)

    def get_SeatHeatReLeSts(self):
        # type: () -> datatypes.SeatClimaLvl
        fdx_item = self.fdx_items_by_rte_name["SeatHeatReLeSts"]
        return fdx_item.value_raw

    def get_SeatHeatReRiSts(self):
        # type: () -> datatypes.SeatClimaLvl
        fdx_item = self.fdx_items_by_rte_name["SeatHeatReRiSts"]
        return fdx_item.value_raw

    def get_UsrSwtDispClimaSts(self):
        # type: () -> datatypes.UsrSwtDispClimaSts
        data = datatypes.UsrSwtDispClimaSts()
        group = self.fdx_items_by_rte_name["UsrSwtDispClimaTSpForRowSecLe"].parent_group #should be same in all
        with group.mutex_lock:
            data.UsrSwtDispClimaTSpForRowSecLe = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSwtDispClimaTSpForRowSecLe'], group), 0.5, 15.0)
            data.UsrSwtDispClimaTSpForRowSecRi = r2p_float(_gg(self.fdx_items_by_rte_name['UsrSwtDispClimaTSpForRowSecRi'], group), 0.5, 15.0)
            data.UsrSwtDispSeatHeatLvlForRowSecLe = _gg(self.fdx_items_by_rte_name['UsrSwtDispSeatHeatLvlForRowSecLe'], group)
            data.UsrSwtDispSeatHeatLvlForRowSecRi = _gg(self.fdx_items_by_rte_name['UsrSwtDispSeatHeatLvlForRowSecRi'], group)
            data.UsrSwtDispFanLvlForRowSec = _gg(self.fdx_items_by_rte_name['UsrSwtDispFanLvlForRowSec'], group)
            data.UsrSwtDispClimaTSpSpclForRowSecLe = _gg(self.fdx_items_by_rte_name['UsrSwtDispClimaTSpSpclForRowSecLe'], group)
            data.UsrSwtDispClimaTSpSpclForRowSecRi = _gg(self.fdx_items_by_rte_name['UsrSwtDispClimaTSpSpclForRowSecRi'], group)
        return data

    def get_UsrSwtDispForSecRowSeatVentn(self):
        # type: () -> datatypes.UsrSwtDispForSecRowSeatVentn
        data = datatypes.UsrSwtDispForSecRowSeatVentn()
        group = self.fdx_items_by_rte_name["UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnLe"].parent_group #should be same in all
        with group.mutex_lock:
            data.UsrSwtDispForSecRowSeatVentnLe = _gg(self.fdx_items_by_rte_name['UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnLe'], group)
            data.UsrSwtDispForSecRowSeatVentnRi = _gg(self.fdx_items_by_rte_name['UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnRi'], group)
        return data

    def send_UsrSwtDispReqForSecRowSeatVentn(self, data):
        # type: (datatypes.UsrSwtDispReqForSecRowSeatVentn) -> None
        group = self.fdx_items_by_rte_name["UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispReqForSecRowSeatVentnLe)), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi"], _extract_enum_value(_get_if_not_None(data.UsrSwtDispReqForSecRowSeatVentnRi)), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe"], _get_if_not_None(data.usrSwtDispUpdReqForSecRowSeatVentnLe), group)
            _ag(self.fdx_items_by_rte_name["UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi"], _get_if_not_None(data.usrSwtDispUpdReqForSecRowSeatVentnRi), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_UsrSwtDispReqForSecRowSeatVentn(self):
        group = self.fdx_items_by_rte_name["UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe"].parent_group
        self.remove_group_from_send_timer(group)

    def get_UsrSwtDispReqVrnt(self):
        # type: () -> datatypes.UsrSwtDispReqVrnt
        data = datatypes.UsrSwtDispReqVrnt()
        group = self.fdx_items_by_rte_name["UsrSwtDispSeatHeatFct"].parent_group #should be same in all
        with group.mutex_lock:
            data.UsrSwtDispSeatHeatFct = _gg(self.fdx_items_by_rte_name['UsrSwtDispSeatHeatFct'], group)
            data.UsrSwtDispTUnit = _gg(self.fdx_items_by_rte_name['UsrSwtDispTUnit'], group)
        return data

    def send_SeatHeatLvlReqLe(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        fdx_item = self.fdx_items_by_rte_name["SeatHeatLvlReqLe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SeatHeatLvlReqLe(self):
        group = self.fdx_items_by_rte_name["SeatHeatLvlReqLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_SeatHeatLvlReqRi(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        fdx_item = self.fdx_items_by_rte_name["SeatHeatLvlReqRi"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_SeatHeatLvlReqRi(self):
        group = self.fdx_items_by_rte_name["SeatHeatLvlReqRi"].parent_group
        self.remove_group_from_send_timer(group)

    def get_VFCVectorIHU(self):
        # type: () -> datatypes.VFCGrp
        data = datatypes.VFCGrp()
        group = self.fdx_items_by_rte_name["VFCVectorIHUGrp1"].parent_group #should be same in all
        with group.mutex_lock:
            data.Grp1 = _gg(self.fdx_items_by_rte_name['VFCVectorIHUGrp1'], group)
            data.Grp2 = _gg(self.fdx_items_by_rte_name['VFCVectorIHUGrp2'], group)
            data.Grp3 = _gg(self.fdx_items_by_rte_name['VFCVectorIHUGrp3'], group)
        return data

    def send_VehActvMsgToDrvr(self, data):
        # type: (datatypes.VehActv1) -> None
        fdx_item = self.fdx_items_by_rte_name["VehActvMsgToDrvr"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehActvMsgToDrvr(self):
        group = self.fdx_items_by_rte_name["VehActvMsgToDrvr"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VehBattU(self, data):
        # type: (datatypes.SysU) -> None
        group = self.fdx_items_by_rte_name["VehBattUSysU"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehBattUSysU"], p2r_float(data.SysU, 0.1, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehBattUSysUQf"], _extract_enum_value(_get_if_not_None(data.SysUQf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehBattU(self):
        group = self.fdx_items_by_rte_name["VehBattUSysU"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VehCfgPrm(self, data):
        # type: (datatypes.VehCfgPrm) -> None
        group = self.fdx_items_by_rte_name["VehCfgPrmBlk"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehCfgPrmBlk"], p2r_float(data.BlkIDBytePosn1, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal1"], p2r_float(data.CCPBytePosn2, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal2"], p2r_float(data.CCPBytePosn3, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal3"], p2r_float(data.CCPBytePosn4, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal4"], p2r_float(data.CCPBytePosn5, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal5"], p2r_float(data.CCPBytePosn6, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal6"], p2r_float(data.CCPBytePosn7, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehCfgPrmVal7"], p2r_float(data.CCPBytePosn8, 1.0, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehCfgPrm(self):
        group = self.fdx_items_by_rte_name["VehCfgPrmBlk"].parent_group
        self.remove_group_from_send_timer(group)

    def get_VehDecelCtrlSetg(self):
        # type: () -> datatypes.NormSptPen
        data = datatypes.NormSptPen()
        group = self.fdx_items_by_rte_name["VehDecelCtrlSetgNormSpt"].parent_group #should be same in all
        with group.mutex_lock:
            data.NormSpt = _gg(self.fdx_items_by_rte_name['VehDecelCtrlSetgNormSpt'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['VehDecelCtrlSetgPen'], group)
        return data

    def send_VehMNom(self, data):
        # type: (datatypes.VehMNomRec1) -> None
        group = self.fdx_items_by_rte_name["VehM"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehM"], p2r_float(data.VehM, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehMQly"], _extract_enum_value(_get_if_not_None(data.VehMQly)), group)
            _ag(self.fdx_items_by_rte_name["VehMNomTrlrM"], _extract_enum_value(_get_if_not_None(data.TrlrM)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehMNom(self):
        group = self.fdx_items_by_rte_name["VehM"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VehModMngtGlbSafe1(self, data):
        # type: (datatypes.VehModMngtGlbSafe1) -> None
        group = self.fdx_items_by_rte_name["UsgModSts"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["UsgModSts"], _extract_enum_value(_get_if_not_None(data.UsgModSts)), group)
            _ag(self.fdx_items_by_rte_name["CarModSts1"], _extract_enum_value(_get_if_not_None(data.CarModSts1)), group)
            _ag(self.fdx_items_by_rte_name["CarModSubtypWdCarModSubtyp"], _get_if_not_None(data.CarModSubtypWdCarModSubtyp), group)
            _ag(self.fdx_items_by_rte_name["EgyLvlElecMai"], _get_if_not_None(data.EgyLvlElecMai), group)
            _ag(self.fdx_items_by_rte_name["EgyLvlElecSubtyp"], _get_if_not_None(data.EgyLvlElecSubtyp), group)
            _ag(self.fdx_items_by_rte_name["PwrLvlElecMai"], _get_if_not_None(data.PwrLvlElecMai), group)
            _ag(self.fdx_items_by_rte_name["PwrLvlElecSubtyp"], _get_if_not_None(data.PwrLvlElecSubtyp), group)
            _ag(self.fdx_items_by_rte_name["FltEgyCnsWdSts"], _extract_enum_value(_get_if_not_None(data.FltEgyCnsWdSts)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehModMngtGlbSafe1(self):
        group = self.fdx_items_by_rte_name["UsgModSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VehMtnStSafe(self, data):
        # type: (datatypes.VehMtnStSafe1) -> None
        group = self.fdx_items_by_rte_name["VehMtnStSafe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehMtnStSafe"], _extract_enum_value(_get_if_not_None(data.VehMtnSt)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehMtnStSafe(self):
        group = self.fdx_items_by_rte_name["VehMtnStSafe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_AudWarn(self, data):
        # type: (bool) -> None
        fdx_item = self.fdx_items_by_rte_name["AudWarn"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_AudWarn(self):
        group = self.fdx_items_by_rte_name["AudWarn"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VehSpdAvgIndcd(self, data):
        # type: (datatypes.VehSpdIndcd1) -> None
        group = self.fdx_items_by_rte_name["VehSpdAvg"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehSpdAvg"], _get_if_not_None(data.VehSpdIndcd), group)
            _ag(self.fdx_items_by_rte_name["VehSpdAvgUnit"], _extract_enum_value(_get_if_not_None(data.VeSpdIndcdUnit)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehSpdAvgIndcd(self):
        group = self.fdx_items_by_rte_name["VehSpdAvg"].parent_group
        self.remove_group_from_send_timer(group)

    def get_VehSpdCtrlActvSts(self):
        # type: () -> datatypes.OnOff1
        fdx_item = self.fdx_items_by_rte_name["VehSpdCtrlActvSts"]
        return fdx_item.value_raw

    def send_VehSpdIndcd(self, data):
        # type: (datatypes.VehSpdIndcd1) -> None
        group = self.fdx_items_by_rte_name["VehSpdIndcdVal"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehSpdIndcdVal"], _get_if_not_None(data.VehSpdIndcd), group)
            _ag(self.fdx_items_by_rte_name["VehSpdIndcdUnit"], _extract_enum_value(_get_if_not_None(data.VeSpdIndcdUnit)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehSpdIndcd(self):
        group = self.fdx_items_by_rte_name["VehSpdIndcdVal"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VehSpdLgtSafe(self, data):
        # type: (datatypes.VehSpdLgtSafe1) -> None
        group = self.fdx_items_by_rte_name["VehSpdLgtSafe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VehSpdLgtSafe"], p2r_float(data.VehSpdLgt, 0.00391, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VehSpdLgtSafeQf"], _extract_enum_value(_get_if_not_None(data.VehSpdLgtQf)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VehSpdLgtSafe(self):
        group = self.fdx_items_by_rte_name["VehSpdLgtSafe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_Vin(self, data):
        # type: (datatypes.Vin1) -> None
        group = self.fdx_items_by_rte_name["VinBlk"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["VinBlk"], p2r_float(data.BlockNr, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn1"], p2r_float(data.VINSignalPos1, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn2"], p2r_float(data.VINSignalPos2, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn3"], p2r_float(data.VINSignalPos3, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn4"], p2r_float(data.VINSignalPos4, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn5"], p2r_float(data.VINSignalPos5, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn6"], p2r_float(data.VINSignalPos6, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["VinPosn7"], p2r_float(data.VINSignalPos7, 1.0, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_Vin(self):
        group = self.fdx_items_by_rte_name["VinBlk"].parent_group
        self.remove_group_from_send_timer(group)

    def get_VisnAgWideCfg(self):
        # type: () -> datatypes.VisnAgWideCfg1
        data = datatypes.VisnAgWideCfg1()
        group = self.fdx_items_by_rte_name["VisnAgWideCfgSteerPahRe"].parent_group #should be same in all
        with group.mutex_lock:
            data.SteerPahRe = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgSteerPahRe'], group)
            data.SteerPahFrnt = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgSteerPahFrnt'], group)
            data.SteerPahSide = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgSteerPahSide'], group)
            data.DstInfoRe = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgDstInfoRe'], group)
            data.DstInfoFrnt = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgDstInfoFrnt'], group)
            data.DstInfoSide = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgDstInfoSide'], group)
            data.CtraInfo = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgCtraInfo'], group)
            data.TwbrPah = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgTwbrPah'], group)
            data.ObjDetn = _gg(self.fdx_items_by_rte_name['VisnAgWideCfgObjDetn'], group)
        return data

    def send_VisnAgWideSts(self, data):
        # type: (datatypes.AvlSts2) -> None
        fdx_item = self.fdx_items_by_rte_name["VisnAgWideSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VisnAgWideSts(self):
        group = self.fdx_items_by_rte_name["VisnAgWideSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_VisnImgAgWideInUse(self, data):
        # type: (datatypes.VisnImgAgWideInUse) -> None
        fdx_item = self.fdx_items_by_rte_name["VisnImgAgWideInUse"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_VisnImgAgWideInUse(self):
        group = self.fdx_items_by_rte_name["VisnImgAgWideInUse"].parent_group
        self.remove_group_from_send_timer(group)

    def get_VisnImgAgWideReq(self):
        # type: () -> datatypes.VisnImgAgWideInUse
        fdx_item = self.fdx_items_by_rte_name["VisnImgAgWideReq"]
        return fdx_item.value_raw

    def get_VisnImgCamReq(self):
        # type: () -> datatypes.CamIndReq
        data = datatypes.CamIndReq()
        group = self.fdx_items_by_rte_name["VisnImgCamReqCamFrnt"].parent_group #should be same in all
        with group.mutex_lock:
            data.CamFrnt = _gg(self.fdx_items_by_rte_name['VisnImgCamReqCamFrnt'], group)
            data.CamRe = _gg(self.fdx_items_by_rte_name['VisnImgCamReqCamRe'], group)
            data.CamLe = _gg(self.fdx_items_by_rte_name['VisnImgCamReqCamLe'], group)
            data.CamRi = _gg(self.fdx_items_by_rte_name['VisnImgCamReqCamRi'], group)
        return data

    def get_WarnTypForLaneChgWarn(self):
        # type: () -> datatypes.WarnTypForLaneChgWarnPen1
        data = datatypes.WarnTypForLaneChgWarnPen1()
        group = self.fdx_items_by_rte_name["WarnTypForLaneChgWarnSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['WarnTypForLaneChgWarnSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['WarnTypForLaneChgWarnPen'], group)
        return data

    def get_WarnTypForLaneKeepAid(self):
        # type: () -> datatypes.WarnTypForLaneChgWarnPen1
        data = datatypes.WarnTypForLaneChgWarnPen1()
        group = self.fdx_items_by_rte_name["WarnTypForLaneKeepAidSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['WarnTypForLaneKeepAidSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['WarnTypForLaneKeepAidPen'], group)
        return data

    def send_WhlCircum(self, data):
        # type: (int) -> None
        fdx_item = self.fdx_items_by_rte_name["WhlCircum"]
        fdx_item.value_raw = _get_if_not_None(data)
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WhlCircum(self):
        group = self.fdx_items_by_rte_name["WhlCircum"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WhlMotSysTqEst(self, data):
        # type: (datatypes.TqSafe2) -> None
        group = self.fdx_items_by_rte_name["WhlMotSysTqEst"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["WhlMotSysTqEst"], p2r_float(data.TqAct, 4.0, -8188.0), group)
            _ag(self.fdx_items_by_rte_name["WhlMotSysTqEstQlyFac"], _extract_enum_value(_get_if_not_None(data.TqActQlyFac)), group)
            _ag(self.fdx_items_by_rte_name["WhlMotSysTqEstSpdLimn"], p2r_float(data.SpdLimn, 10.0, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WhlMotSysTqEst(self):
        group = self.fdx_items_by_rte_name["WhlMotSysTqEst"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WhlRotToothCntr(self, data):
        # type: (datatypes.WhlRotToothCntrRec) -> None
        group = self.fdx_items_by_rte_name["WhlRotToothCntrFrntLe"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["WhlRotToothCntrFrntLe"], p2r_float(data.WhlRotToothCntrFrntLe, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["WhlRotToothCntrFrntRi"], p2r_float(data.WhlRotToothCntrFrntRi, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["WhlRotToothCntrReLe"], p2r_float(data.WhlRotToothCntrReLe, 1.0, 0.0), group)
            _ag(self.fdx_items_by_rte_name["WhlRotToothCntrReRi"], p2r_float(data.WhlRotToothCntrReRi, 1.0, 0.0), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WhlRotToothCntr(self):
        group = self.fdx_items_by_rte_name["WhlRotToothCntrFrntLe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WinPosnStsAtDrv(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["WinPosnStsDrv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WinPosnStsAtDrv(self):
        group = self.fdx_items_by_rte_name["WinPosnStsDrv"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WinPosnStsAtDrvrRe(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["WinPosnStsAtDrvrRe"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WinPosnStsAtDrvrRe(self):
        group = self.fdx_items_by_rte_name["WinPosnStsAtDrvrRe"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WinPosnStsAtPass(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["WinPosnStsPass"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WinPosnStsAtPass(self):
        group = self.fdx_items_by_rte_name["WinPosnStsPass"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WinPosnStsAtPassRe(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        fdx_item = self.fdx_items_by_rte_name["WinPosnStsRePass"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WinPosnStsAtPassRe(self):
        group = self.fdx_items_by_rte_name["WinPosnStsRePass"].parent_group
        self.remove_group_from_send_timer(group)

    def send_WipgInfo(self, data):
        # type: (datatypes.WipgInfo) -> None
        group = self.fdx_items_by_rte_name["WipgInfoWipgSpdInfo"].parent_group   #should be same in all
        with group.mutex_lock:
            _ag(self.fdx_items_by_rte_name["WipgInfoWipgSpdInfo"], _extract_enum_value(_get_if_not_None(data.WipgSpdInfo)), group)
            _ag(self.fdx_items_by_rte_name["WipgInfoWiprActv"], _extract_enum_value(_get_if_not_None(data.WiprActv)), group)
            _ag(self.fdx_items_by_rte_name["WipgInfoWiprInWipgAr"], _extract_enum_value(_get_if_not_None(data.WiprInWipgAr)), group)
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WipgInfo(self):
        group = self.fdx_items_by_rte_name["WipgInfoWipgSpdInfo"].parent_group
        self.remove_group_from_send_timer(group)

    def get_WiprFrntSrvModReq(self):
        # type: () -> datatypes.WiprFrntSrvModReq
        fdx_item = self.fdx_items_by_rte_name["WiprFrntSrvModReq"]
        return fdx_item.value_raw

    def send_WiprInPosnForSrv(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["WiprInPosnForSrv"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WiprInPosnForSrv(self):
        group = self.fdx_items_by_rte_name["WiprInPosnForSrv"].parent_group
        self.remove_group_from_send_timer(group)

    def get_WiprReAutReq(self):
        # type: () -> datatypes.OnOffPen
        data = datatypes.OnOffPen()
        group = self.fdx_items_by_rte_name["WiprReAutReqSts"].parent_group #should be same in all
        with group.mutex_lock:
            data.Sts = _gg(self.fdx_items_by_rte_name['WiprReAutReqSts'], group)
            data.Pen = _gg(self.fdx_items_by_rte_name['WiprReAutReqPen'], group)
        return data

    def send_WshrFldSts1WdElmHMI(self, data):
        # type: (datatypes.OnOff1) -> None
        fdx_item = self.fdx_items_by_rte_name["WshrFldSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_WshrFldSts1WdElmHMI(self):
        group = self.fdx_items_by_rte_name["WshrFldSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_iTPMSCalPsbl(self, data):
        # type: (datatypes.CalPsbl) -> None
        fdx_item = self.fdx_items_by_rte_name["iTPMSCalPsbl"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_iTPMSCalPsbl(self):
        group = self.fdx_items_by_rte_name["iTPMSCalPsbl"].parent_group
        self.remove_group_from_send_timer(group)

    def get_iTPMSCalReq(self):
        # type: () -> datatypes.StrtStopReq1
        fdx_item = self.fdx_items_by_rte_name["iTPMSCalReq"]
        return fdx_item.value_raw

    def send_iTPMSCalSts(self, data):
        # type: (datatypes.iTPMSTirePCalSts) -> None
        fdx_item = self.fdx_items_by_rte_name["iTPMSCalSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_iTPMSCalSts(self):
        group = self.fdx_items_by_rte_name["iTPMSCalSts"].parent_group
        self.remove_group_from_send_timer(group)

    def send_iTPMSTirePMSts(self, data):
        # type: (datatypes.iTPMSTirePMSts) -> None
        fdx_item = self.fdx_items_by_rte_name["iTPMSTirePMSts"]
        fdx_item.value_raw = _extract_enum_value(_get_if_not_None(data))
        group = fdx_item.parent_group
        self.connection.send_data_exchange(group.group_id, group.size, group.build_data())
        self.add_group_to_send_timer(group)

    def stop_iTPMSTirePMSts(self):
        group = self.fdx_items_by_rte_name["iTPMSTirePMSts"].parent_group
        self.remove_group_from_send_timer(group)

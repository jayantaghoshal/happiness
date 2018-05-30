# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import time
import sys
sys.path.append('/usr/local/lib/python2.7/dist-packages')
import typing
from Sym.adb_connection import AdbCarsimConnection
from Sym.carsimconnection import CarsimConnection
from generated import dataelements_fdx
from generated import datatypes as DE
from generated.dataelements_abc import DataElements
from generated.dataelements_carsim import CarsimSerializer
from generated.pyDataElements import \
    FrSignalInterface, \
    BaseEnumSender, \
    BaseBoolSender, \
    BaseFloatSender, \
    BaseIntegerSender
from vts.runners.host import asserts
from vts.utils.python.controllers.adb import AdbProxy

PydataElementsSenderType = typing.Union[BaseEnumSender, BaseBoolSender, BaseFloatSender, BaseIntegerSender]

class Signals:
    def __init__(self, fr):
        # type: (DataElements) -> None
        self.fr = fr
        self.vehmod = GetDefaultInitiated_VehModMngtGlbSafe1()

    def set_usage_mode(self, usgMod, carmod=None):
        # type: (DE.UsgModSts1, typing.Optional[DE.CarModSts1]) -> None
        self.vehmod.UsgModSts = usgMod
        if carmod is not None:
            self.vehmod.CarModSts1 = carmod
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)


def wait_for_signal(fr_interface, fdx_signal, expected_value, timeout_sec=3):
    # type: (FrSignalInterface, PydataElementsSenderType, int, float) -> None

    if fr_interface.connected:
        end = time.time() + timeout_sec
        read_value = fdx_signal.get()
        while time.time() < end:
            read_value = fdx_signal.get()
            if read_value == expected_value:
                break
            time.sleep(0.2)
        asserts.assertEqual(read_value, expected_value,
                            "Expected signal %s to be %d within %d sec, got %d)" %
                            (fdx_signal.fdx_name, expected_value, timeout_sec, fdx_signal.get()))

GETFUNC_T = typing.TypeVar('GETFUNC_T')
GETFUNC_MEMBER_T = typing.TypeVar('GETFUNC_MEMBER_T')
def wait_for_fr_signal(fr_get_func, expected_value, timeout_sec=3, message=""):
    # type: (typing.Callable[[], GETFUNC_T], GETFUNC_T, float, str) -> None

    end = time.time() + timeout_sec
    read_value = fr_get_func()
    while time.time() < end:
        read_value = fr_get_func()
        if read_value == expected_value:
            break
        time.sleep(0.2)
    asserts.assertEqual(read_value, expected_value,
                        "Expected signal %s to be %r within %d sec, got %r. %s)" %
                        (fr_get_func.__name__, expected_value, timeout_sec, read_value, message))

def wait_for_fr_subsignal(fr_get_func, member_getter, expected_value, timeout_sec=3, message=""):

    end = time.time() + timeout_sec
    full_frame = fr_get_func()
    read_value = member_getter(full_frame)
    while time.time() < end:
        full_frame = fr_get_func()
        read_value = member_getter(full_frame)
        if read_value == expected_value:
            break
        time.sleep(0.2)
    asserts.assertEqual(read_value, expected_value,
                        "Expected sub-signal in %s to be %r within %d sec, Extracted value:%r, Full frame: %r . %s)" %
                        (fr_get_func.__name__, expected_value, timeout_sec, read_value, full_frame, message))

def assert_fr_signal_equals(fr_get_func, expected_value, message=""):
    # type: (typing.Callable[[], GETFUNC_T], GETFUNC_T, str) -> None
    read_value = fr_get_func()
    asserts.assertEqual(read_value, expected_value,
                        "Expected signal %s to be %r, got %r. %s)" %
                        (fr_get_func.__name__, expected_value, read_value, message))


def get_dataelements_connection(adb, mode=None):
    # type: (typing.Optional[AdbProxy], str) -> DataElements
    if mode is None:
        mode = os.environ.get("DATAELEMENTS_MODE", "adb")
    if mode == "adb":
        adbconnection = AdbCarsimConnection(None if adb is None else adb.shell, None if adb is None else adb.forward)
        fr = CarsimSerializer(adbconnection, None)
        adbconnection.connect()
        return fr
    elif mode == "ethernet":
        connection = CarsimConnection()
        fr = CarsimSerializer(connection, None)
        connection.connect("localhost", 8080)
        return fr
    elif mode == "canoe":
        return dataelements_fdx.FrSignalInterface()
    else:
        raise RuntimeError("Invalid DATAELEMENTS_MODE %r" % mode)

def GetDefaultInitiated_VehModMngtGlbSafe1():
    vehmod = DE.VehModMngtGlbSafe1()
    vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
    vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
    vehmod.Chks = 0
    vehmod.Cntr = 0
    vehmod.CarModSubtypWdCarModSubtyp = 0
    vehmod.EgyLvlElecMai = 0
    vehmod.EgyLvlElecSubtyp = 0
    vehmod.PwrLvlElecMai = 0
    vehmod.PwrLvlElecSubtyp = 0
    vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt
    return vehmod

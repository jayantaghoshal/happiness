#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os
import re
import random
import subprocess
import itertools

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.controllers import adb
from vts.utils.python.precondition import precondition_utils
from subprocess import call
from time import sleep
from subprocess import PIPE
from collections import Counter

from gpsinject import GPSPositionInjector
import threading

import datetime

sys.path.append('/usr/local/lib/python2.7/dist-packages')

import pytz
from generated import datatypes as DE
import vehiclehalcommon

class VtsSystemClockTest(base_test.BaseTestClass):
    """Host side tests for the TimeUpdateService"""

    def setUpClass(self):

        self.logger = logging.getLogger('SystemClockTest')
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("adb_shell")
        result = self.dut.shell.adb_shell.Execute("echo VtsSystemClockTest")
        self.logger.info(str(result[const.STDOUT]))

        # Set Location to Africa/Abidjan co-ordinates - UTC + 0
        latitude = 5.3096600
        longitude = -4.0126600

        Gps_set_year = 2018
        Gps_set_month = 5
        Gps_set_day = 4
        Gps_set_hour = 6
        Gps_set_minute = 10
        Gps_set_second = 20

        # Set TimeZone settings to manual and set it to UTC + 0 offset
        self.dut.shell.adb_shell.Execute("settings put global auto_time_zone 0")
        self.dut.shell.adb_shell.Execute("settings put global time_zone Africa/Abidjan")

        # GPS time
        self.hostsettime = datetime.datetime(Gps_set_year, Gps_set_month, Gps_set_day, Gps_set_hour,Gps_set_minute,Gps_set_second)

        logging.info("Host UTC time =%s",self.hostsettime)

        self.gpsObj = GPSPositionInjector.GPSPositionInjector(self.hostsettime,latitude,longitude)
        self.gpsObj.ConnecttoIHUviaEth(False)

        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)


    def tearDownClass(self):

        self.logger.info("System Clock test executed")
        self.fr.close()
        self.gpsObj.DisconnectNetInterface()


    def getPackagePid(self, package_name):
        """Gets the pid for a given package. Returns None if not running.
        Args:
            package_name: The name of the package.
        Returns:
            The first pid found under a given package name. None if no process
            was found running the package.
        Raises:
            AndroidDeviceError if the output of the phone's process list was
            in an unexpected format.
        """
        for cmd in ("ps -A", "ps"):
            try:
                out = self.dut.shell.adb_shell.Execute('%s | grep "S %s"' % (cmd, package_name))
                split_out = str(out['stdouts']).split()
                print(split_out)
                if package_name not in str(out['stdouts']) and split_out[7] is 'S':
                    self.logger.warn("No package name found in the stdouts")
                    continue
                try:
                    pid = int(split_out[1])
                    self.logger.info('apk %s has pid %s.', package_name, pid)
                    return pid
                except (IndexError, ValueError) as e:
                    # Possible ValueError from string to int cast.
                    # Possible IndexError from split.
                    self.logger.warn('Command \"%s\" returned output line: '
                                '\"%s\".\nError: %s', cmd, out, e)
            except Exception as e:
                self.logger.warn(
                    'Device fails to check if %s running with \"%s\"\n'
                    'Exception %s', package_name, cmd, e)
        self.logger.debug("apk %s is not running", package_name)
        return None


    def ParseInputManualdate(self,time_date,type):

        """Parse the Input Manual time to Year/Day/Month/Hour/Minute/Second
        Args:
            time_date: Time and Date in format -mmddHHMMYYYY.SS
            type : Parse type in String : Year/Day/Month/Hour/Minute/Second
        Returns:
            String that corresponding time/date attributes
        """

        if type == "Month":
            return time_date[0:2]
        elif type == "Day":
            return time_date[2:4]
        elif type == "Hour":
            return time_date[4:6]
        elif type == "Minute":
            return time_date[6:8]
        elif type == "Year":
            return time_date[10:12]
        elif type == "Second":
            return time_date[13:15]


    def ParseInputAutodate(self,time_date,type):

        """Parse the Input Auto time to Year/Day/Month/Hour/Minute/Second
        Args:
            time_date: Time and Date in format - YYYY-MM-DD HH:MM:SS
            type : Parse type in String : Year/Day/Month/Hour/Minute/Second
        Returns:
            String that corresponding time/date attributes
        """

        if type == "Month":
            return time_date[5:7]
        elif type == "Day":
            return time_date[8:10]
        elif type == "Hour":
            return time_date[11:13]
        elif type == "Minute":
            return time_date[14:16]
        elif type == "Year":
            return time_date[2:4]
        elif type == "Second":
            return time_date[17:19]

    def CheckTiAndDate(self,inputdatetime,date_time_in,settings):

        if settings == "Manual":
            month = int(self.ParseInputManualdate(inputdatetime,"Month"))
            day = int(self.ParseInputManualdate(inputdatetime,"Day"))
            hour = int(self.ParseInputManualdate(inputdatetime,"Hour"))
            minute = int(self.ParseInputManualdate(inputdatetime,"Minute"))
            year = int(self.ParseInputManualdate(inputdatetime,"Year"))
            second = int(self.ParseInputManualdate(inputdatetime,"Second"))

        elif settings == "Auto":

            month = int(self.ParseInputAutodate(inputdatetime,"Month"))
            day = int(self.ParseInputAutodate(inputdatetime,"Day"))
            hour = int(self.ParseInputAutodate(inputdatetime,"Hour"))
            minute = int(self.ParseInputAutodate(inputdatetime,"Minute"))
            year = int(self.ParseInputAutodate(inputdatetime,"Year"))
            second = int(self.ParseInputAutodate(inputdatetime,"Second"))


        logging.info("Input Date and time Month =%d Day =%d Hour=%d Minute=%d Year=%d Second=%d",\
            month,day,hour,minute,year,second)
        logging.info("CEM Date and time Month =%d Day =%d Hour=%d Minute=%d Year=%d Second=%d", \
            date_time_in.Month,date_time_in.Day,date_time_in.Hour,date_time_in.Minute,date_time_in.Year,date_time_in.Second)


        if month == date_time_in.Month and day == date_time_in.Day and \
           hour == date_time_in.Hour and year == date_time_in.Year and \
           minute == date_time_in.Minute:

            date_time_fr = DE.DateTi30()
            date_time_fr.DataValid = DE.NoYes1.Yes
            date_time_fr.Yr1 = date_time_in.Year
            date_time_fr.Mth1 = date_time_in.Month
            date_time_fr.Day = date_time_in.Day
            date_time_fr.Hr1 = date_time_in.Hour
            date_time_fr.Mins1 = date_time_in.Minute
            date_time_fr.Sec1 = date_time_in.Second
            self.SendTiAndDateCEMtoIHU(date_time_fr)

            return True
        else:
            return False

    def RxCEMTiAndDateThread(self,arg,input_date,Ev,settings):

        ''' Thread that listens for FlexRay signal SetTiAndDate
            Args:
                arg:Thread name
                input_date:Input time
                Ev: Threading Event that need to set on Success
                settings: Time Settings type - Auto or Manual
        '''

        thhandle = threading.currentThread()
        connection = self.fr
        retry = 0
        print("Started --->",arg)

        while True:

            try:
                date_time_flry = connection.get_SetTiAndDate()
            except Exception, e:
                logging.info("KeyError ")
                time.sleep(0.2)
                continue

            if retry <= 4:
                if not self.CheckTiAndDate(input_date,date_time_flry,settings):
                    time.sleep(0.5)
                    retry = retry + 1
                else:
                    logging.info("Success Event Set ")
                    Ev.set()
                    break
            else:
                 retry = 0
                 asserts.assertTrue(False,"TiAndDate not received in CEM after retry !!")
                 break

        print("Stopped --->",arg)


    def SendTiAndDateCEMtoIHU(self,flry_datetime):

        connection = self.fr
        connection.send_TiAndDateIndcn(flry_datetime)


    def StartGPSPositionTxThread(self):

        self.gpsObj.StartTxGPSData()

    def StopGPSPositionTxThread(self):

        self.gpsObj.StopTxGPSData()


    def testCheckTimeUpdateService(self):

        self.logger.info("Check for TimeUpdateService running status:")
        pid = self.getPackagePid("com.volvocars.timeupdateservice")
        asserts.assertNotEqual(pid, None, "Timeupdate service not running")

    def testCheckAutoToManualTimeSetting(self):

        self.logger.info(" Time and Date : Auto --> Manual settings change test")
        '''
            Test verifies whether the manual time set from IHU is received in CEM
        '''

        isTimeSettingManual = False
        InputManualtime ="040718462018.29"
        ManualtimeSuccessEv = threading.Event()
        RxCEMThread = threading.Thread(target = self.RxCEMTiAndDateThread, args = ("RxCEMTiAndDateThread",InputManualtime,ManualtimeSuccessEv,"Manual"))

        # Change Time and Date setting to Auto
        self.dut.shell.adb_shell.Execute("settings put global auto_time 1")

        time.sleep(1)

        # Change Time and Date,TimeZone setting to Manual
        self.dut.shell.adb_shell.Execute("settings put global auto_time 0")


        # Start Rx Thread that listens for SetTiAndDate signal
        RxCEMThread.start()

        # Input format - mmddHHMMYYYY.S
        cmd_toybox = "date"
        execmd_timedate = cmd_toybox + " " + InputManualtime
        timezone_set = self.dut.shell.adb_shell.Execute(execmd_timedate)

        # On successful reception of CEM signal Event is received
        if ManualtimeSuccessEv.wait(3.0):
            logging.info("Success Event Received ")
            isTimeSettingManual = True

        asserts.assertTrue(isTimeSettingManual,"Auto to Manual time check failed !!")



    def testCheckManualToAutoTimeSetting(self):
        self.logger.info(" Time and Date : Manual --> Auto Transition test")

        '''
            Test verifies whether the Auto time set from GPS Mocker/Injector is
            received in IHU and Clock time is set accordingly.

            Also verifies whether the Auto time set is received in CEM
        '''


        AutotimeSuccessEv = threading.Event()
        inputAutotime = str(self.hostsettime)
        RxCEMThread = threading.Thread(target = self.RxCEMTiAndDateThread, args = ("RxCEMTiAndDateThread",inputAutotime,AutotimeSuccessEv,"Auto"))


        isTimeSettingAuto= [False,False]

        # Change Time and Date setting to Manual
        self.dut.shell.adb_shell.Execute("settings put global auto_time 0")

        cmd_toybox = "toybox date"
        input_datetime = " 123123462016.29"
        execmd_timedate = cmd_toybox + input_datetime
        self.dut.shell.adb_shell.Execute(execmd_timedate)

        # Change Time and Date setting to Auto
        self.dut.shell.adb_shell.Execute("settings put global auto_time 1")

        time.sleep(1)

        RxCEMThread.start()

        # Start inject GPS data
        self.StartGPSPositionTxThread()


        if AutotimeSuccessEv.wait(3.0):
            logging.info("Auto time verified Event received")
            isTimeSettingAuto[0] = True
            time.sleep(5)
        else:
            self.StopGPSPositionTxThread()

        asserts.assertTrue(isTimeSettingAuto[0],"Auto Time not correct due to failure in CEM reception !!")


        hostClocktime = self.hostsettime.strftime('%Y%m%d%H%M')

        logging.info("Host Auto time: %s ",hostClocktime)


        Targettimeset = self.dut.shell.adb_shell.Execute("date +%Y%m%d%H%M")[const.STDOUT][0]
        logging.info("Target Auto time: %s ",Targettimeset)


        self.StopGPSPositionTxThread()

        if int(hostClocktime) == int(Targettimeset):
          isTimeSettingAuto[1] = True

        asserts.assertTrue(isTimeSettingAuto[1],"Auto Time not correct !!")


    def testCheckTimeSetFromCEM(self):

        '''
            Test verifies time send from CEM is successfully set in IHU

        '''

        IsTimeSetfromCEM = [False,False]

        year = 17
        Month = 10
        Day = 12
        Hour = 11
        Minute = 12
        Sec = 15

        date_time_fr = DE.DateTi30()
        date_time_fr.DataValid = DE.NoYes1.Yes
        date_time_fr.Yr1 = year
        date_time_fr.Mth1 = Month
        date_time_fr.Day = Day
        date_time_fr.Hr1 = Hour
        date_time_fr.Mins1 = Minute
        date_time_fr.Sec1 = Sec
        self.SendTiAndDateCEMtoIHU(date_time_fr)
        time.sleep(1)
        date_time_fr.Sec1 = Sec + 1
        self.SendTiAndDateCEMtoIHU(date_time_fr)

        if year == 17:
            yr = 2017

        CEMtime = str(yr) + str(Month) + str(Day) + str(Hour) + str(Minute)

        logging.info("CEM DateAndTi 1 =%s ",CEMtime)

        IHUtime = self.dut.shell.adb_shell.Execute("date +%Y%m%d%H%M")[const.STDOUT][0]

        logging.info("IHU Date and time 1 =%s ",IHUtime)

        if int(CEMtime) == int(IHUtime):
             IsTimeSetfromCEM[0] = True

        asserts.assertTrue(IsTimeSetfromCEM[0]," TimeSetFromCEM 1 failed")


        # Change Minutes to 45 from 12
        Minute = 45


        logging.info("CEM DateAndTi 2 =%s ",CEMtime)

        date_time_fr.Mins1 = Minute
        date_time_fr.Sec1 = Sec

        self.SendTiAndDateCEMtoIHU(date_time_fr)
        date_time_fr.Sec1 = Sec + 1
        time.sleep(1)
        self.SendTiAndDateCEMtoIHU(date_time_fr)

        CEMtime = str(yr) + str(Month) + str(Day) + str(Hour) + str(Minute)

        IHUtime = self.dut.shell.adb_shell.Execute("date +%Y%m%d%H%M")[const.STDOUT][0]

        logging.info("IHU Date and time 2 =%s ",IHUtime)

        if int(CEMtime) == int(IHUtime):
             IsTimeSetfromCEM[1] = True

        asserts.assertTrue(IsTimeSetfromCEM[1]," TimeSetFromCEM 2 failed")



if __name__ == "__main__":
    test_runner.main()
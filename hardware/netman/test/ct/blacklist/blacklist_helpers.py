#!/usr/bin/env python

import logging
import time
import sys
import os
import errno
import re
from vts.runners.host import utils
from vts.runners.host import const
from subprocess import check_output
from subprocess import CalledProcessError

class BlacklistHelper:

    def __init__(self):
        self.adb_logcat_process = None

    def silentFileRemove(self, filePath):
        # attempts to remove a file without stopping execution if file does not exist
        try:
            os.remove(filePath)
        except OSError as e:
            if e.errno != errno.ENOENT:
                raise
            else:
                logging.info("Attempted to remove non-existing file, moving on...")
                pass

    def silentAppUninstall(self, packetName):
        try:
             logging.info(check_output(["adb", "uninstall", packetName]))
        except CalledProcessError as e:
            logging.info("silentAppUninstall: Uninstall packet failed")
            pass

    def cleanUp(self, logfilePath):
        logging.info("cleanUp Called")
        self.stopAdbLogcat()
        self.silentFileRemove(logfilePath)

    def startAdbLogcat(self, logFilePath, tag):
        if self.adb_logcat_process != None:
            self.stopAdbLogcat
        logcatFilters = '\"%s\"' %tag
        cmd = "adb logcat -s %s >> %s" % (logcatFilters, logFilePath)
        self.adb_logcat_process = utils.start_standing_subprocess(cmd)
        logging.info("Logcat started with filters: %s"%logcatFilters)

    def stopAdbLogcat(self):
        try:
            utils.stop_standing_subprocess(self.adb_logcat_process)
        except utils.VTSUtilsError as e:
            logging.error("Cannot stop adb logcat. %s", e)
        self.adb_logcat_process = None
        logging.info("logcat was terminated successfully")

    def printInterfaces(self, interfaceList):
        for iface in interfaceList:
            logging.info("Found interface: " + iface) 

    def readTestResult(self, filepath):
        # Parse interfacenames from logstrings of this format:
        # "[VtsNetdBlacklistComponentTest] 09-08 06:27:01.651 INFO 11-12 02:22:31.840 31137 31137
        #  I <logcatFilters>: name:eth1 (eth1)"
        # and returns all interfaces in a List.
        ifaceList = []
        with open(filepath) as resultFile:
            for line in resultFile:
                startPos = line.find("name")
                if startPos != -1:
                    endPos = line.find(' ', startPos)
                    ifaceList.append(line[startPos+5:endPos])
        return ifaceList
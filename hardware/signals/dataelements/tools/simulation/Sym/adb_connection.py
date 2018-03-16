# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import re
import typing
import subprocess
import logging
from . import carsimconnection


class AdbCarsimConnection(carsimconnection.CarsimConnection):

    def __init__(self, adb_shell=None, adb_forward=None):
        super(AdbCarsimConnection, self).__init__()
        self.adb_carsim_bridge_process = None

        #NOTE: Need some special handling to use the same adb connection as VTS does because if we use subprocess adb
        #      while VTS is running then VTS will fail during teardown when trying the following command.
        #           adb -s R1J58E60a938f1 shell am force-stop com.googlecode.android_scripting, stdout: , stderr: , ret: 0
        #TODO: Try to find a solution for this, now it's forcing us to run the binary through daemonize which means
        #      we can't kill it cleanly in the end

        if adb_shell is None:
            self.adb_shell = self.manual_adb_shell
        else:
            self.adb_shell = adb_shell

        if adb_forward is None:
            self.adb_forward = self.manual_adb_forward
        else:
            self.adb_forward = adb_forward

        self.carsim_bridge_pid = None  # type: typing.Optional[int]

    def manual_adb_shell(self, args):
        try:
            return subprocess.check_output("adb shell " + args, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as cpe:
            raise RuntimeError("Failed to issue adb command %s %s. Error: %s" % (cpe.cmd, cpe.args, cpe.output))


    def manual_adb_forward(self, args):
        try:
            return subprocess.check_output("adb forward " + args, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as cpe:
            raise RuntimeError("Failed to issue adb command %s %s. Error: %s" % (cpe.cmd, cpe.args, cpe.output))

    def connect(self):
        self.adb_forward("tcp:8080 tcp:8080")
        self.adb_shell("stop carsim_bridge")
        # TODO: Can daemonize be replaced with something giving PID?
        launch_output = self.adb_shell('daemonize /vendor/bin/carsim_bridge')
        #self.carsim_bridge_pid = int(re.match(r"\[\d+\]\s*(\d*)", launch_output).group(1))

        logging.info("adb launched carsim_bridge")
        # TODO: Hack, give some time for carsim_bridge to start listening, adb forward accepts connection tricking
        #       the client that it is connected so hard to make retry-loop also
        time.sleep(1.0)
        super(AdbCarsimConnection, self).connect("localhost", 8080)
        print("connected")

    def close(self):
        super(AdbCarsimConnection, self).close()
        if self.carsim_bridge_pid is not None:
            self.adb_shell("killall -9 carsim_bridge")

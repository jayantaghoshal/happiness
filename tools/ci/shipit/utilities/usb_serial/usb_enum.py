"""Class module to process a list of slave device connections
and output corresponding serial device"""

import os.path
import json
import argparse
import serial
import serial.tools.list_ports


class UsbSerialEnum(object):
    """Encapsulates Methods for enumerating serial devices on the USB hub"""

    def __init__(self, configfile):
        self._configfile = configfile
        self._valid_file()
        self._json_to_dictionary()

    def _valid_file(self):
        """Checking valid file extentions"""
        base, ext = os.path.splitext(self._configfile)
        if ext.lower() not in '.json':
            raise argparse.ArgumentTypeError('File must have a json extention')

    def _json_to_dictionary(self):
        """Populating Dictionary"""
        with open(self._configfile) as json_data:
            self._slave_dict = json.load(json_data)

    def get_slave_dict(self):
        """Returns a dictionary extracted from the connection.json file"""
        return self._slave_dict

    @staticmethod
    def get_device_from_port(port):
        """Get Serial device path"""
        ports = serial.tools.list_ports.comports()
        serial_ports_dict = {}
        for prt in ports:
            serial_ports_dict.update({prt.location : prt.device})
        return serial_ports_dict[port]

    def get_device_dict(self):
        """Returns a dictionary of location as key and serial device path as value"""
        ports = serial.tools.list_ports.comports()
        serial_ports_dict = {}
        for prt in ports:
            serial_ports_dict.update({prt.location : prt.device})
        return serial_ports_dict












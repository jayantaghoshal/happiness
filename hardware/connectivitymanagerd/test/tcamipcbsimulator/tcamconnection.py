#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import sys
import os
import socket
import subprocess
import threading
import time
import json


class TcamConnection(object):

    def __init__(self, receive_message_callback):

        self._logger = logging.getLogger('TCAM CONNECTION')

        self._receive_callback = receive_message_callback
        self._automotive_eth_interfcae_name = ''

        self._vlan_interface_name = "tcam0"

        self._product_out_dir_path = os.environ['ANDROID_PRODUCT_OUT']
        self._localconfig_path = self._product_out_dir_path + \
            '/vendor/etc/localconfig/localconfig.json'

        self._host_ipcb_address = self._read_local_config(
            'IIPS_IpAddress_TCAM')
        self._host_ipcb_port = self._read_local_config('IIPS_Port_TCAM')
        self._host_ipcb_subnet_mask = '255.255.255.224'

        self.ihu_ipcb_address = self._read_local_config('IIPS_IpAddress_LOCAL')
        self.ihu_ipcb_port = self._read_local_config('IIPS_Port_LOCAL')

        if not self._is_eth_interface_up():
            raise ValueError(
                'Network Interface not found. Connect Ethernet to USB converter to Host !!!')
        if not self._is_vlan_set(self._vlan_interface_name):
            self._create_vlan_interface(
                self._vlan_interface_name, self._automotive_eth_interfcae_name)

        self._connect_to_ihu_via_eth(False)

        # Create UDP Socket
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._sock.bind((self._host_ipcb_address, self._host_ipcb_port))

        if self._sock is None:
            raise ValueError('Socket Creation failed !!!')

        self._receive_thread = threading.Thread(target=self._receive)

    def start_listening(self):
        self._receive_thread.start()

    def _is_vlan_set(self, vlan_name):

        interface_list = os.listdir('/sys/class/net/')
        vlan_eth = filter(lambda x: vlan_name in x, interface_list)

        if not vlan_eth:
            return False
        else:
            return vlan_eth[0] == vlan_name

    def send_packet(self, data):
        if type(data) is str:
            self._logger.info(
                "Sending String Based Message [%s]", data.encode('hex'))
        elif type(data) is bytearray:
            self._logger.info(
                "Sending Byte Array Based Message [%s]", ''.join('{:02x}'.format(x) for x in data))
        else:
            self._logger.error('Unrecognized data type [%s]..', type(data))

        size = self._sock.sendto(
            data, (self.ihu_ipcb_address, self.ihu_ipcb_port))
        if(size != len(data)):
            self._logger.error("Failed to send data properly")

    def _receive(self):
        self.listen = True
        while self.listen:
            try:
                data, address = self._sock.recvfrom(1024)
            except socket.timeout, e:
                err = e.args[0]
                # this next if/else is a bit redundant, but illustrates how the
                # timeout exception is setup
                if err == 'timed out':
                    self._logger.error('recv timed out, retry later')
                    continue
                else:
                    self._logger.error('%s', e)
                    sys.exit(1)
            except socket.error, e:
                # Something else happened, handle error, exit, etc.
                self._logger.error('%s', e)
                sys.exit(1)
            else:
                if len(data) == 0:
                    self._logger.info('Orderly shutdown')
                    self.listen = False
                    self._sock.close()
                else:
                    self._receive_callback(data, address)

    def stop_listening(self):
        self._logger.info("Closing Socket")
        self._sock.sendto('', (self._host_ipcb_address, self._host_ipcb_port))
        try:
            self._sock.shutdown(socket.SHUT_RDWR)
        except socket.error, e:
            self._logger.debug('Failed Shutting Down Socket: %s', e)
        self._sock.close()
        self._logger.info("Socket Closed")
        self._receive_thread.join()

    def _is_eth_interface_up(self):
        interface_list = os.listdir('/sys/class/net/')
        self._automotive_eth_interfcae_name = filter(
            lambda x: 'enx' in x, interface_list)
        if not self._automotive_eth_interfcae_name:
            return False
        else:
            return True

    def _connect_to_ihu_via_eth(self, reset):
        if not reset:
            self._logger.info(
                "Host IP address: %s:%s ", self._host_ipcb_address, self._host_ipcb_port)
            self._logger.info(
                "Ethernet Interface name: %s ", self._automotive_eth_interfcae_name)
            self._logger.info(
                "VLAN Interface name: %s ", self._vlan_interface_name)

            # Set VLAN IP address
            subprocess.check_output(
                ['sudo', 'ifconfig', self._vlan_interface_name, self._host_ipcb_address, "netmask", self._host_ipcb_subnet_mask])

            if not self._check_ping(self.ihu_ipcb_address):
                raise ValueError(
                    'Destination Host unreachable!! Check the IHU <-> Host connection and IP/Port settings!!!')

        else:
            subprocess.check_output(
                ['sudo', 'ifconfig', self._vlan_interface_name, 'down'])
            time.sleep(8)
            subprocess.check_output(
                ['sudo', 'ifconfig', self._vlan_interface_name, 'up'])
            time.sleep(8)
            subprocess.check_output(
                ['sudo', 'ifconfig', self._vlan_interface_name, self._host_ipcb_address, "netmask", self._host_ipcb_subnet_mask])

            if not self._check_ping(self.ihu_ipcb_address):
                raise ValueError(
                    'Destination Host unreachable!! Check the IHU <-> Host connection !!!')

    def _isconnect_net_interface(self):
        if self._is_vlan_set(self._vlan_interface_name):
            subprocess.check_output(
                ['sudo', 'ifconfig', self._vlan_interface_name, 'down'])
            subprocess.check_output(
                ['sudo', 'ip', 'link', 'delete', self._vlan_interface_name])

    def _create_vlan_interface(self, vlan_name, auto_eth):
        subprocess.check_output(
            ['sudo', 'ip', 'link', 'add', 'link', auto_eth[0], 'name', vlan_name, 'type', 'vlan', 'id', '172'])

    def _check_ping(self, ip_address):
        response = os.system("ping -c 1 " + ip_address)
        #  check the response and return.
        return response == 0

    def _read_local_config(self, key):
        config = json.loads(open(self._localconfig_path).read())
        value = config[key]
        return value

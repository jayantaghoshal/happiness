#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import sys
import os
import socket
import subprocess
import json
import time
import datetime
import threading
import logging

from . import asncodec

sys.path.append('/usr/local/lib/python2.7/dist-packages')



class GPSPositionInjector(object):

    """ GPS Date,time and Location Injector
        Packets transffered are compliant to VCC IP command Protocol
    """

    def __init__(self,epoch_time,latitude,longitude):

        self.latitude = latitude
        self.longitude = longitude

        self.__InitASNCodec()

        # Initialized  VCM/TCAM  PDU format
        self.vccPDUheader = { 'serviceID':"00a9",'operationID':"0902",'msgLength':"0000003D",
                                'senderHandle':"a9020500",'protoVersion':"03",
                                'opType':"05",'dataType':"00",'reserved':"00"}

        self.vccPDUheader_append = self.vccPDUheader['serviceID'] + self.vccPDUheader['operationID'] + \
                                   self.vccPDUheader['msgLength'] + self.vccPDUheader['senderHandle'] + \
                                   self.vccPDUheader['protoVersion'] + self.vccPDUheader['opType'] + \
                                   self.vccPDUheader['dataType'] + self.vccPDUheader['reserved']

        logging.info(" VCC PDU header : %s ",self.vccPDUheader_append)

        self.vccPDUheader_decoded = self.vccPDUheader_append.decode("hex")
        self.product_out_dir_path = os.environ.get('ANDROID_PRODUCT_OUT')
        self.localconfig_path = self.product_out_dir_path + '/vendor/etc/localconfig/localconfig.json'

        self.SetGPSTimeAndDate(epoch_time)
        self.SetGPSPosition(self.latitude,self.longitude)

        self.remoteIHUipAddress = self.ReadLocalConfig('IIPS_IpAddress_LOCAL')
        self.remotePort = self.ReadLocalConfig('IIPS_Port_LOCAL')

        self.host_ipaddress = self.ReadLocalConfig('IIPS_IpAddress_VCM')

        self.VLANInterfaceName = "tcam0"
        self.AutomotiveEthInterfaceName =''

        if self.IsEthInterfaceUp() == False:
             raise ValueError('Network Interface not found. Connect Ethernet to USB converter to Host !!!')

        if not self.IsVLANset(self.VLANInterfaceName):
            self.CreateVLANInterface(self.VLANInterfaceName,self.AutomotiveEthInterfaceName)


        logging.info(" Remote IP Address : %s ",self.remoteIHUipAddress)
        logging.info(" Remote Port : %d ",self.remotePort)

        # Create UDP Socket
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

        if self.sock is None:
            raise ValueError('Socket Creation failed !!!')

        # Create Tx Thread
        self.thandle = threading.Thread(target = self.TxUDPPacketThread, args = ("TxUDPPacketThread",epoch_time))


    def __InitASNCodec(self):

        self.asnobj = asncodec.ASNGPSParse()

        if self.asnobj is None:
            raise ValueError('Invalid ASN Object !!!')

    def IsVLANset(self,vlan_name):

        interface_list = os.listdir('/sys/class/net/')
        vlan_eth = filter(lambda x: vlan_name in x,interface_list)

        if not vlan_eth:
            return False
        else :
            return vlan_eth[0] == vlan_name


    def EncodeGPSData(self):

        self.encodedPayload = self.asnobj.encode()
        self.UDP_payload = self.vccPDUheader_decoded + self.encodedPayload


    def ReadLocalConfig(self,key):

        localcfg_path = self.localconfig_path
        lclcfg = open(localcfg_path).read()
        config = json.loads(open(localcfg_path).read())
        value = config[key]
        return value

    def SendPacket(self,data):

        size = self.sock.sendto(data, (self.remoteIHUipAddress, self.remotePort))
        print("Size of UDP Packet send ", size)


    def SetGPSTimeAndDate(self,epoch_time):

        self.asnobj.SetGPSTimeAndDate(epoch_time)
        self.EncodeGPSData()

    def SetGPSPosition(self,latitude,longitude):

         self.asnobj.SetGPSPosition(latitude,longitude)
         self.EncodeGPSData()

    def IsEthInterfaceUp(self):

        interface_list = os.listdir('/sys/class/net/')
        self.AutomotiveEthInterfaceName = filter(lambda x: 'enx' in x,interface_list)
        if not self.AutomotiveEthInterfaceName:
            return False
        else:
            return True


    def ConnecttoIHUviaEth(self,reset):

        if not reset:

            logging.info("Host IP address : : %s ",self.host_ipaddress)
            logging.info("Ethernet Interface name : : %s ",self.AutomotiveEthInterfaceName)
            logging.info("VLAN Interface name : : %s ",self.VLANInterfaceName)

            # Set VLAN IP address
            subprocess.check_output(['sudo','ifconfig',self.VLANInterfaceName,self.host_ipaddress])

            if self.check_ping(self.remoteIHUipAddress) == False:
                raise ValueError('Destination Host unreachable!! Check the IHU <-> Host connection and IP/Port settings!!!')

        else:
             subprocess.check_output(['sudo','ifconfig',self.VLANInterfaceName,'down'])
             time.sleep(8)
             subprocess.check_output(['sudo','ifconfig',self.VLANInterfaceName,'up'])
             time.sleep(8)
             subprocess.check_output(['sudo','ifconfig',self.VLANInterfaceName,self.host_ipaddress])

             if not self.check_ping(self.remoteIHUipAddress):
                raise ValueError('Destination Host unreachable!! Check the IHU <-> Host connection !!!')

    def DisconnectNetInterface(self):

        if self.IsVLANset(self.VLANInterfaceName):
            subprocess.check_output(['sudo','ifconfig',self.VLANInterfaceName,'down'])
            subprocess.check_output(['sudo','ip','link','delete',self.VLANInterfaceName])


    def CreateVLANInterface(self,vlan_name,auto_eth):

        subprocess.check_output(['sudo','ip','link','add','link',auto_eth[0],'name',vlan_name,'type','vlan','id','172'])

    def check_ping(self,ipaddress):

        response = os.system("ping -c 1 " + ipaddress)
        #  check the response and return.
        return response == 0

    def TxUDPPacketThread(self,arg,epochautotime):

        thhandle = threading.currentThread()

        year = int (epochautotime.strftime('%Y'))
        month = int(epochautotime.strftime('%m'))
        day = int(epochautotime.strftime('%d'))
        hour = int(epochautotime.strftime('%H'))
        minute = int(epochautotime.strftime('%M'))
        second = int(epochautotime.strftime('%S')) + 1

        starttime = int(round(time.time() * 1000))
        print("Started --->",arg)

        while getattr(thhandle, "do_run", True):

            endtime = int(round(time.time() * 1000))
            #Avoid Infinite loop in case Stop GPS is not called
            if endtime - starttime > 10000:
                break

            # Transmit Packet every 1000ms
            self.SendPacket(self.UDP_payload)
            #break
            time.sleep(1)
            # Update GPS date and time
            self.SetGPSTimeAndDate(datetime.datetime(year, month, day, hour, minute, second))
            second = second + 1

            # Check connection
            if not self.check_ping(self.remoteIHUipAddress):
                print "Ethernet Connection down Resetting !!!"
                self.ConnecttoIHUviaEth(True)

        print("Stopped --->",arg)

    def StartTxGPSData(self):

        if self.thandle is None :
            print "StartTxGPSData failed due to failure in thread creation"

        if not self.thandle.isAlive():
            self.thandle.start()


    def StopTxGPSData(self):

        if self.thandle.isAlive():
            self.thandle.do_run = False

#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from datetime import datetime

import os
import time
import sys
sys.path.append('/usr/local/lib/python2.7/dist-packages')
import asn1tools #type: ignore
import pytz




class ASNGPSParse(object):

    ''' GPS data dictionary declaration '''

    def __init__(self):

        dirpath = os.environ.get('ANDROID_BUILD_TOP')
        asnfilepath = dirpath + '/vendor/volvocars/hardware/infotainmentIpBus/spec/infotainmentIpBus.asn'
        # Compile the ASN.1 specification with Unaligned PER codec
        self.asnlib = asn1tools.compile_files(asnfilepath,'uper')

        self.DateTime = { 'year':2017,'month':4,'day':5,'hour':15,'minute':53,'second':0 }
        self.GPSSystemTime = { 'weekNumber':101,'timeOfWeek':1234567 }
        self.CoordinatesLongLat = { 'longitude':214748364 , 'latitude' : 107374182 }
        self.GeographicalPosition = { 'longLat': self.CoordinatesLongLat,'altitude': 1000 }
        self.Velocity = { 'speed':1234,'horizontalVelocity':567,'verticalVelocity': -987 }
        self.GNSSUsage = { 'gpsIsUsed':	True,'glonassIsUsed':False,'galileoIsUsed':True,'sbasIsUsed':	False,'qzssL1IsUsed':	False,'qzssL1SAIFIsUsed':False }
        self.GnssFixType = 'fix3D'
        self.GNSSStatus = { 'fixType':self.GnssFixType,'dgpsIsUsed':True,'selfEphemerisDataUsage':False }
        self.NrOfSatellitesPerSystem = {'gps':5,'glonass':5,'galileo':5,'sbas':5,'qzssL1':5,'qzssL1SAIF':5 }

        self.SatelliteUsage = {'nrOfSatellitesVisible': self.NrOfSatellitesPerSystem,'nrOfSatellitesUsed': self.NrOfSatellitesPerSystem }
        self.DOPValues = {'hdop':1,'vdop':2,'pdop':3,'tdop':4 }

        self.SatelliteTrackingStatusType = 'collectedAndUsed'
        self.ExtendedChannelData = ('notSupported', None)

        self.ChannelData = { 'prn':7,'trackingStatus':self.SatelliteTrackingStatusType,'svacc':0,'snr':0,'azimuthAngle':10,'elevationAngle':200,'extendedData':self.ExtendedChannelData }

        self.ReceiverChannelData = [self.ChannelData,self.ChannelData]

        self.GNSSData = {'utcTime':	self.DateTime,'gpsTime':self.GPSSystemTime,'position':self.GeographicalPosition,'movement':self.Velocity,'heading': 18000,
                        'gnssStatus':self.GNSSUsage,
                        'positioningStatus':self.GNSSStatus,
                        'satelliteInfo':self.SatelliteUsage,
                        'precision':self.DOPValues,
                        'receiverChannels':self.ReceiverChannelData
                        }


    def SetGPSTimeAndDate(self,date_time):

        date = date_time
        #self.ParseEpochtime(date_time)
        self.DateTime['year'] = int(date.strftime('%Y'))
        self.DateTime['month'] = int(date.strftime('%m'))
        self.DateTime['day'] = int(date.strftime('%d'))
        self.DateTime['hour'] = int(date.strftime('%H'))
        self.DateTime['minute'] = int(date.strftime('%M'))
        self.DateTime['second'] = int(date.strftime('%S'))

    def SetGPSPosition(self,latitude,longitude):

        self.CoordinatesLongLat['longitude'] = int(float(longitude) * ((float(1 << 30)) / 90.0))
        self.CoordinatesLongLat['latitude'] = int(float(latitude) * ((float(1 << 30)) / 90.0))

    def ParseEpochtime(self,date_time):

        posix_timestamp = time.mktime(date_time.timetuple())
        # get time in UTC
        utc_dt = datetime.utcfromtimestamp(posix_timestamp).replace(tzinfo=pytz.utc)

        return utc_dt

    def encode(self):
        OpGNSSPositionDataResponse = {'gnssPositionData':	self.GNSSData }
        return self.asnlib.encode('OpGNSSPositionData-Response',OpGNSSPositionDataResponse)

    def decode(self,en):
        OpGNSSPositionDataResponse = {'gnssPositionData':	self.GNSSData }
        return self.asnlib.decode('OpGNSSPositionData-Response',en)

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import xml.etree.ElementTree as ET

FX = '{http://www.asam.net/xml/fbx}'
HO = '{http://www.asam.net/xml}'


class FlexRayDBParser:
    """ Parse through a FlexRay fibex file """
    def __init__(self, flexray_xml):
        if os.path.isfile(flexray_xml):
            self.flexray_file = flexray_xml
            self.root = ET.parse(self.flexray_file).getroot()
        else:
            print 'Not a valid file'

    def find_pdu_for_datagroup(self, datagroup):
        """Find PDU for datagroup"""
        pdu_id_ref = None
        elements = self.root.find(FX + 'ELEMENTS')
        frames = elements.find(FX + 'FRAMES')
        for frame in frames.findall(FX + 'FRAME'):
            if datagroup == frame.find(HO + 'SHORT-NAME').text:
                pdu_instances = frame.find(FX + 'PDU-INSTANCES')
                for pdu_instance in pdu_instances.findall(FX + 'PDU-INSTANCE'):
                    pdu_ref = pdu_instance.find(FX + 'PDU-REF')
                    if pdu_ref is not None:
                        pdu_id_ref = pdu_ref.get('ID-REF')
        if pdu_id_ref is not None:
            pdus = elements.find(FX + 'PDUS')
            for pdu in pdus.findall(FX + 'PDU'):
                if pdu_id_ref == pdu.get('ID'):
                    return pdu.find(HO + 'SHORT-NAME').text


def main():
    """Main function for the FlexRay DB Parser"""
    fd_parser = FlexRayDBParser('SPA_1710\\Backbone_SPA1710.xml')
    print fd_parser.find_pdu_for_datagroup('DimBackBoneDiagResFrame3')
    print fd_parser.find_pdu_for_datagroup('SrsBackBoneFr02')
    print fd_parser.find_pdu_for_datagroup('IHUBackBoneFr10')
    print fd_parser.find_pdu_for_datagroup('VcmBackBoneFr02')
    print fd_parser.find_pdu_for_datagroup('VcmBackBoneDiagReqFrame5')

if __name__ == "__main__":
    main()


# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import xml.etree.ElementTree as ET
from xml.dom import minidom
from collections import OrderedDict


class FDXWriter:
    def __init__(self, start_group_id, output_folder):
        self.canoe_fdx_description = None
        self.__create_new_xml_tree()
        self.fdx_read_and_write_group_list = []
        self.fdx_write_group_list = []
        self.data_group_id = start_group_id
        self.offset = 0
        if not os.path.exists(output_folder):
            os.mkdir(output_folder)
        self.output_folder = os.path.abspath(output_folder)

    def __create_new_xml_tree(self):
        """
        Create a new XML-tree from the input file and returns the root of that tree.
        Initiates global variables and creates the first element of the tree."""

        self.canoe_fdx_description = ET.Element("canoefdxdescription")
        self.canoe_fdx_description.set("version", "1.0")

    def generate_new_datagroup(self):
        """Create data group and increase the group ID."""
        datagroup = ET.SubElement(self.canoe_fdx_description, "datagroup")
        datagroup.set("groupID", str(self.data_group_id))
        self.offset = 0
        return datagroup

    def generate_new_sysvar_group_identifier(self, datagroup, name_of_namespace):
        """Generates a new sysvar group"""
        identifier = ET.SubElement(datagroup, "identifier")
        namespaces = name_of_namespace.split("::")
        identifier.text = namespaces[-1]
        self.fdx_write_group_list.append(self.data_group_id)
        self.data_group_id += 1

    def generate_sysvar(self, item, variable, name_of_namespace, sysvar_name=None):
        """
        :param item
        :param variable
        :param name_of_namespace
        Create sysvar and set name and namespace to that sysvar."""
        try:
            sysvar = ET.SubElement(item, "sysvar")
            sysvar.set("namespace", name_of_namespace)
            if sysvar_name:
                sysvar.set('name', sysvar_name)
            else:
                sysvar.set("name", variable.get("name"))
        except:
            print "Error while creating sysvar."
            raise

    def setup_signals(self, signals, flexray_parser):
        sorted_dict = OrderedDict(sorted(signals.items(), key=lambda t: t[1]))
        last_group = ''
        datagroup = None
        for signal, bus_group in sorted_dict.iteritems():
            group = bus_group.split(',')[0]
            bus = bus_group.split(',')[1]
            signal = signal.split('&')[0]
            if group != last_group:
                if last_group != '':
                    datagroup.set("size", str(self.offset))
                datagroup = self.generate_new_datagroup()
                self.create_signal_group_identifier(datagroup, group)
            item = self.generate_item(datagroup, {'type': 'int'})
            if item == '':
                continue
            self.create_signal(item, signal, bus, group, flexray_parser)
            last_group = group
        datagroup.set("size", str(self.offset))

    def generate_item(self, data_group, variable):
        """
        :param data_group
        :param variable
        Create item and set type, size and offset to that item."""
        try:
            data_type = variable.get("type")
            if data_type == "int":
                item = ET.SubElement(data_group, "item")
                data_type += "32"
                item.set("type", str(data_type))
                size = 4  # int(variable.get("bitcount", 8))/8
                item.set("size", str(size))
                item.set("offset", str(self.offset))
                self.offset += size
            elif data_type == "float":
                item = ET.SubElement(data_group, "item")
                item.set("type", str(data_type))
                size = 8  # int(variable.get("bitcount", 8))/8
                item.set("size", str(size))
                item.set("offset", str(self.offset))
                self.offset += size
            elif data_type == 'intarray':
                item = ET.SubElement(data_group, "item")
                item.set("type", "int32array")
                size = (int(variable.get("bitcount", 10))/8)*(int(variable.get("arrayLength", 10)))
                item.set("size", str(size))
                item.set("offset", str(self.offset))
                self.offset += size
            elif data_type == 'string':
                item = ET.SubElement(data_group, "item")
                item.set("type", str(data_type))
                size = 1024  # int(variable.get("bitcount", 8))/8
                item.set("size", str(size))
                item.set("offset", str(self.offset))
                self.offset += size
            else:
                raise Exception('{0} is not a compatible type'.format(data_type))
            return item
        except Exception as e:
            print "Error while creating an item. \"{0}\"".format(e.message)
            raise


    def create_description_file_from_xml_tree(self, output_filename):
        """Create an XML-file from the XML-tree."""
        try:
            xml_str = minidom.parseString(ET.tostring(self.canoe_fdx_description)).toprettyxml(indent="   ")
            with open(os.path.join(self.output_folder, output_filename), "w") as f:
                f.write(xml_str)
        except:
            print "Error while creating an XML-file from XML-tree."
            raise

    def create_vector_maport_config_file(self, output_filename):
        """Create VectorMAPortConfig.xml which defines which data groups that are read and write."""
        try:
            port = ET.Element("Port")
            port.set("type", "MAPort")
            ET.SubElement(port, "ModelFile")
            ip_address = ET.SubElement(port, "IpAddress")
            ip_address.text = "169.254.4.16"
            fdx_port = ET.SubElement(port, "FdxPort")
            fdx_port.text = "2809"
            xil_port = ET.SubElement(port, "XilPort")
            xil_port.text = "3030"
            fdx_file = ET.SubElement(port, "FdxFile")
            fdx_file.text = os.path.join(self.output_folder,output_filename)
            # Write Group of System Variables
            for element in self.fdx_write_group_list:
                fdx_write_group = ET.SubElement(port, "FdxWriteGroup")
                fdx_write_group.set("cycleTime", "100")
                fdx_write_group.text = str(element)

            for element in self.fdx_read_and_write_group_list:
                fdx_write_group = ET.SubElement(port, "FdxWriteGroup")
                fdx_write_group.set("cycleTime", "100")
                fdx_write_group.text = str(element)
                fdx_read_group = ET.SubElement(port, "FdxReadGroup")
                fdx_read_group.set("cycleTime", "100")
                fdx_read_group.text = str(element)

            xml_str = minidom.parseString(ET.tostring(port)).toprettyxml(indent="   ")
            with open(os.path.join(self.output_folder, "VectorMAPortConfig.xml"), "w") as f:
                f.write(xml_str)
        except:
            print "Error while creating VectorMAPortConfig.xml."
            raise

    def create_signal_group_identifier(self, datagroup, frame):
        """Create identifier which sets frame name as datagroup identifier"""
        identifier = ET.SubElement(datagroup, "identifier")
        identifier.text = '' + frame
        self.fdx_read_and_write_group_list.append(self.data_group_id)
        self.data_group_id += 1

    def create_signal(self, item, signal_name, bus, datagroup, flexray_parser):
        """Create signal and set name to that signal."""
        try:
            signal = ET.SubElement(item, "signal")
            signal.set("name", signal_name)
            if bus == 'Backbone':
                if 'BackBoneFr' in datagroup:
                    pdu_name = flexray_parser.find_pdu_for_datagroup(datagroup)
                    signal.set('msg', pdu_name)
                else:
                    signal.set('msg', datagroup)
            else:
                signal.set('msg', datagroup)
            signal.set('bus', bus)
            signal.set('channel', '')
            signal.set('database', '')
            signal.set('dir', 'auto')
            signal.set('value', 'raw')
        except:
            print "Error while creating signal."
            raise

    def set_size(self, datagroup):
        datagroup.set("size", str(self.offset))

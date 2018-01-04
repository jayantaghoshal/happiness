# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import xml.etree.ElementTree as ET
import os
from collections import OrderedDict

__author__ = 'tolsso52'

io_output_dict = {'AOUT0': 'IO::IOPiggy1',
                  'AOUT1': 'IO::IOPiggy1',
                  'DOUT0': 'IO::IOPiggy1',
                  'DOUT1': 'IO::IOPiggy1',
                  'DOUT2': 'IO::IOPiggy1',
                  'DOUT3': 'IO::IOPiggy1',
                  'PWM': 'IO::IOPiggy1',
                  'PWMOutFreq': 'IO::IOPiggy1'}

io_input_dict = {'AIN0': 'IO::IOPiggy1',
                 'AIN1': 'IO::IOPiggy1',
                 'DIN0': 'IO::IOPiggy1',
                 'DIN1': 'IO::IOPiggy1',
                 'DIN2': 'IO::IOPiggy1',
                 'DIN3': 'IO::IOPiggy1'}


class SysvarParser:
    """ Parse system variable files """
    def __init__(self, sysvar_file, fdx_writer, add_io):
        if os.path.isfile(sysvar_file):
            self.sysvar_file = sysvar_file
            self.root = ET.parse(self.sysvar_file).getroot()
            self.fdx_writer = fdx_writer
            self.dict_struct = OrderedDict()
            self.dict_struct_names = OrderedDict()
            if add_io:
                self.add_io_to_fdx()

    def parse_contents_in_vsysvar_file(self, skip_read_sysvar):
        """Loop through the content of the .vsysvar file."""
        for namespace in self.root.findall('namespace'):
            for namespace2 in namespace.findall('namespace'):
                self.populate_struct_dicts(namespace2)
                name_of_namespace2 = namespace2.get('name')
                if len(namespace2.findall('namespace')) > 0:
                    for namespace3 in namespace2.findall('namespace'):
                        if skip_read_sysvar:
                            continue
                        name_of_namespace3 = '{}::{}'.format(name_of_namespace2, namespace3.get('name'))
                        for namespace4 in namespace3.findall('namespace'):
                            name_of_namespace4 = '{}::{}'.format(name_of_namespace3, namespace4.get('name'))
                            for namespace5 in namespace4.findall('namespace'):
                                name_of_namespace5 = '{}::{}'.format(name_of_namespace4, namespace5.get('name'))
                                self.parse_namespace(namespace5, name_of_namespace5)
                                for namespace6 in namespace5.findall('namespace'):
                                    name_of_namespace6 = '{}::{}'.format(name_of_namespace5, namespace6.get('name'))
                                    for namespace7 in namespace6.findall('namespace'):
                                        name_of_namespace7 = '{}::{}'.format(name_of_namespace6, namespace7.get('name'))
                                        self.parse_namespace(namespace7, name_of_namespace7)
                                    self.parse_namespace(namespace6, name_of_namespace6)
                                self.parse_namespace(namespace5, name_of_namespace5)
                            self.parse_namespace(namespace4, name_of_namespace4)
                        self.parse_namespace(namespace3, name_of_namespace3)
                    self.parse_namespace(namespace2, name_of_namespace2)
                else:
                    if skip_read_sysvar:
                        continue
                    elif name_of_namespace2 == 'IL':
                        self.parse_namespace(namespace2, name_of_namespace2)
                    else:
                        name_of_namespace3 = '{}::{}'.format(name_of_namespace2, namespace2.get('name'))
                        self.parse_namespace(namespace2, name_of_namespace3)

    def populate_struct_dicts(self, namespace):
        """
        Populate one dictionary containing structs and one dictionary containing struct names.
        :param namespace: Namespace to find structs in.
        :return:
        """
        for struct_item in namespace.findall('struct'):
            struct_list = []
            struct_names_list = []
            for item in struct_item.findall('structMember'):
                struct_list.append(item)
                struct_names_list.append(item.get('name'))
            self.dict_struct[struct_item.get('name')] = struct_list
            self.dict_struct_names[struct_item.get('name')] = struct_names_list

        struct_is_nestled = True
        while struct_is_nestled:
            dict_struct = OrderedDict()
            dict_struct_names = OrderedDict()
            for struct_item in namespace.findall('struct'):
                struct_list = []
                struct_names_list = []
                for item in struct_item.findall('structMember'):
                    if item.get('type') != 'struct':
                        struct_list.append(item)
                        struct_names_list.append(item.get('name'))
                    else:
                        for struct_member in self.dict_struct[item.get('structDefinition').split('::')[-1]]:
                            struct_list.append(struct_member)
                        for struct_member_name in self.dict_struct_names[item.get('structDefinition').split('::')[-1]]:
                            struct_names_list.append('{}.{}'.format(item.get('name'), struct_member_name))
                dict_struct[struct_item.get('name')] = struct_list
                dict_struct_names[struct_item.get('name')] = struct_names_list
            if dict_struct == self.dict_struct:
                struct_is_nestled = False
            self.dict_struct = dict_struct.copy()
            self.dict_struct_names = dict_struct_names.copy()

    def parse_namespace(self, namespace, name_of_namespace):
        """
        Parse the namespace from the .vsysvar-files, siglist-files and fibex files into a data group with
        signals and sysvars.
        :param namespace: The fdx namespace object to search for variables in
        :param name_of_namespace: The name of the namespace to name the sysvar
        :param read_or_write: Is this a read or write parameter
        """
        if namespace.find("variable") > 0:
            for variable in namespace.findall("variable"):
                if not verify_item(variable):
                    continue

                datagroup = self.fdx_writer.generate_new_datagroup()

                if variable.get("type") == 'struct' or variable.get('type') == 'genericarray':
                    self.fdx_writer.generate_new_sysvar_group_identifier(datagroup, variable.get('name'))
                    struct_definition = variable.get('structDefinition').split('::')[-1]
                    self.parse_struct(struct_definition, datagroup, variable, name_of_namespace, '', None)
                else:
                    self.fdx_writer.generate_new_sysvar_group_identifier(datagroup, name_of_namespace)
                    item = self.fdx_writer.generate_item(datagroup, variable)
                    self.fdx_writer.generate_sysvar(item, variable, name_of_namespace)
                self.fdx_writer.set_size(datagroup)

    def parse_struct(self, struct_definition, datagroup, variable, name_of_namespace, struct_name, array_index):
        """
        Parse struct in vsysvar-file.
        :param struct_definition: Struct definition
        :param datagroup: Datagroup
        :param variable: Variable
        :param name_of_namespace: Name of the namespace
        :param struct_name: Struct name
        :param array_index: Array index
        :return:
        """
        for i, struct_member in enumerate(self.dict_struct[struct_definition]):
            if not verify_item(struct_member):
                continue
            struct_name2 = self.dict_struct_names[struct_definition][i]
            if struct_member.get('type') == 'genericarray':
                struct_name3 = self.append_to_sysvar_name(struct_name, struct_name2, array_index)
                array_length = int(struct_member.get('arrayLength'))
                # If you have problems with the index number, this is probably where the bug is.
                if array_length > 20:
                    array_length = 1
                for index in range(0, array_length):
                    self.parse_generic_array(datagroup, variable, name_of_namespace, struct_member, struct_name3, index)
            else:
                item = self.fdx_writer.generate_item(datagroup, struct_member)
                sysvar_name = '{}.{}'.format(variable.get('name'), self.append_to_sysvar_name(struct_name, struct_name2,
                                                                                              array_index))
                self.fdx_writer.generate_sysvar(item, variable, name_of_namespace, sysvar_name)

    def append_to_sysvar_name(self, parent_struct_name, child_struct_name, array_index):
        """
        Append to sysvar name.
        :param parent_struct_name: Parent struct name
        :param child_struct_name: Child struct name
        :param array_index: Array index
        :return: Sysvar name
        """
        if array_index is None:
            return '{}{}'.format(parent_struct_name, child_struct_name)
        else:
            return '{}[{}].{}'.format(parent_struct_name, array_index, child_struct_name)

    def parse_generic_array(self, datagroup, variable, name_of_namespace, struct_member, struct_name, array_index):
        """
        Parse generic array.
        :param datagroup: Datagroup
        :param variable: Variable
        :param name_of_namespace: Name of the namespace
        :param struct_member: Struct member
        :param struct_name: Struct name
        :param array_index: Array index
        :return:
        """
        for element_prototype in struct_member.findall('elementPrototype'):
            struct_definition = element_prototype.get('structDefinition').split('::')[-1]
            self.parse_struct(struct_definition, datagroup, variable, name_of_namespace, struct_name, array_index)

    def add_io_to_fdx(self):
        """Add IO to FDX"""
        for key, value in io_output_dict.iteritems():
            variable = {'type': 'int', 'name': key}
            datagroup = self.fdx_writer.generate_new_datagroup()
            self.fdx_writer.generate_new_sysvar_group_identifier(datagroup, value)
            item = self.fdx_writer.generate_item(datagroup, variable)
            self.fdx_writer.generate_sysvar(item, variable, value)
            self.fdx_writer.set_size(datagroup)
        for key, value in io_input_dict.iteritems():
            variable = {'type': 'int', 'name': key}
            datagroup = self.fdx_writer.generate_new_datagroup()
            self.fdx_writer.generate_new_sysvar_group_identifier(datagroup, value)
            item = self.fdx_writer.generate_item(datagroup, variable)
            self.fdx_writer.generate_sysvar(item, variable, value)
            self.fdx_writer.set_size(datagroup)


def verify_item(variable):
    """Verify the data type of the item is supported"""
    try:
        data_type = variable.get("type")
        if data_type == "int":
            return True
        elif data_type == "float":
            return True
        elif data_type == 'string':
            return True
        elif data_type == 'intarray':
            return True
        elif data_type == 'struct':
            return True
        elif data_type == 'genericarray':
            return True
        else:
            return False
    except Exception as e:
        print "Error while verifying an item. \"{0}\"".format(e.message)
        raise

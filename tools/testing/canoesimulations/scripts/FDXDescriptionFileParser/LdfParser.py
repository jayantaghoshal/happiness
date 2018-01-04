# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os

__author__ = 'rchiarit'


class LdfParser:
    """ Parse LIN database files """

    def __init__(self, ldf_file, fdx_writer=None):
        if os.path.isfile(ldf_file):
            self.ldf_file = ldf_file
            self.fdx_writer = fdx_writer

    def parse_ldf_file(self):
        """Extract information from ldf file"""
        bus = ''
        frame = ''
        datagroup = ''
        in_frames = False
        number_of_brackets = 0
        with open(self.ldf_file, 'rt') as f:
            for line in f:
                if 'Channel_name' in line:
                    bus = line.split('=')[1]
                    bus = bus.replace('"', '')
                    bus = bus.replace(';', '')
                    bus = bus.strip()
                if 'Frames' in line:
                    in_frames = True
                if in_frames:
                    if number_of_brackets == 1 and '}' not in line:
                        datagroup = self.fdx_writer.generate_new_datagroup()
                        frame = line.strip().split(':')[0]
                        frame = '{}_{}'.format(frame, bus) #TODO: remove when LIN frame names are unchanged by Vector
                        self.fdx_writer.create_signal_group_identifier(datagroup, frame)
                    if number_of_brackets == 2 and '}' not in line:
                        signal = line.strip().split(',')[0]
                        signal = '{}_{}'.format(signal, bus) #TODO: remove when LIN signal names are unchanged by Vector
                        item = self.fdx_writer.generate_item(datagroup, {'type': 'int'})
                        self.fdx_writer.set_size(datagroup)
                        if item == '':
                            continue
                        self.fdx_writer.create_signal(item, signal, bus, frame, None)
                if '{' in line:
                    number_of_brackets += 1
                if '}' in line:
                    number_of_brackets -= 1
                if number_of_brackets == 0:
                    in_frames = False

    def get_nodes_from_ldf_file(self):
        master_node = ''
        bus = ''
        list_of_slave_nodes = []
        in_nodes = False
        with open(self.ldf_file, 'rt') as f:
            for line in f:
                if 'Channel_name' in line:
                    bus = line.split('=')[1]
                    bus = bus.replace('"', '')
                    bus = bus.replace(';', '')
                    bus = bus.strip()
                if 'Nodes' in line:
                    in_nodes = True
                if in_nodes:
                    if 'Master' in line:
                        master_node = line.replace('Master:', '')
                        master_node = master_node.split(',')[0]
                        master_node = master_node.strip()
                    if 'Slaves' in line:
                        slave_node_string = line.replace('Slaves:', '')
                        slave_node_string = slave_node_string.replace(';', '')
                        slave_node_string = slave_node_string.strip()
                        for slave_node in slave_node_string.split(', '):
                            list_of_slave_nodes.append('{}_{}'.format(slave_node, bus))
                        break
        return master_node, list_of_slave_nodes

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import csv
import glob
import os
import traceback
from LdfParser import LdfParser
from SysvarParser import SysvarParser
from FlexRayDBParser import FlexRayDBParser

__author__ = 'tolsso52'


class FolderParser:
    def __init__(self, fdx_writer):
        self.fdx_writer = fdx_writer

    def parse_multiple_files(self, directory, skip_read_sysvar, add_io=False):
        """
        :param skip_read_sysvar     as option to add system variables as read
        Parse all .vsysvar files in the directory into FDX-description files in .xml."""
        try:
            os.chdir(directory)
            number_of_vsysvar_files_parsed = 0
            number_of_siglist2_files_parsed = 0
            number_of_ldf_files_parsed = 0

            for filename in glob.glob("*.vsysvar"):
                sysvar_parser = SysvarParser(filename, self.fdx_writer, add_io=add_io)
                sysvar_parser.parse_contents_in_vsysvar_file(skip_read_sysvar)
                number_of_vsysvar_files_parsed += 1
                add_io = False
            for filename in glob.glob("*.siglist2"):
                signals = parse_siglist(filename)
                fibex = glob.glob("*Backbone*.xml")[0]
                flexray_parser = FlexRayDBParser(fibex)
                self.fdx_writer.setup_signals(signals, flexray_parser)
                number_of_siglist2_files_parsed += 1
            for filename in glob.glob("*.ldf"):
                ldf_parser = LdfParser(filename, self.fdx_writer)
                ldf_parser.parse_ldf_file()
                number_of_ldf_files_parsed += 1

            self.fdx_writer.create_description_file_from_xml_tree('FDXDescriptionFile.xml')
            self.fdx_writer.create_vector_maport_config_file('FDXDescriptionFile.xml')

            if number_of_vsysvar_files_parsed > 0:
                print "Successfully parsed {} .vsysvar-files.".format(number_of_vsysvar_files_parsed)
            else:
                print "No .vsysvar-files could be found in the specified directory."
            if number_of_siglist2_files_parsed > 0:
                print "Successfully parsed {} .siglist2-files.".format(number_of_siglist2_files_parsed)
            if number_of_ldf_files_parsed > 0:
                print "Successfully parsed {} .ldf-files.".format(number_of_ldf_files_parsed)

        except os.error as err:
            print str(err)
        except Exception as e:
            traceback.print_exc()
            print "Error while parsing .vsysvar-file! " + e.message

    def parse_single_file(self, filename, skip_read_sysvar, add_io=False):
        """
        :param filename     name of the file to parse in it's not a folder
        :param skip_read_sysvar     as option to add system variables as read
        Parse the .vsysvar file into a FDX-description file in .xml."""
        try:
            sysvar_parser = SysvarParser(filename, self.fdx_writer, add_io=add_io)
            sysvar_parser.parse_contents_in_vsysvar_file(skip_read_sysvar)

            self.fdx_writer.create_description_file_from_xml_tree('FDXDescriptionFile.xml')
            self.fdx_writer.create_vector_maport_config_file('FDXDescriptionFile.xml')
            print("Successfully parsed 1 .vsysvar-file.")
        except Exception as e:
            traceback.print_exc()
            print "Error while parsing .vsysvar-file! " + e.message


def valid_signal_name(signal):
    """Verify that the signal name is valid"""
    if signal.endswith('_UB'):
        return False
    if signal.endswith('Chks'):
        return False
    if signal.endswith('Cntr'):
        return False
    if signal == 'Sig':
        return False
    return True


def parse_siglist(filename):
    """Read siglist file and parse the signals to a list."""
    signal_dict = {}
    try:
        with open(filename, 'rb') as f:
            reader = csv.reader(f, delimiter='\t')
            for row in reader:
                if valid_signal_name(row[6]):
                        if row[5]=='controlCAN':
                            row[4] = row[0]+ '::' + row[4]
                        signal_dict.update({row[6] + '&' + row[5]: row[4] + ',' + row[5]})
        return signal_dict
    except IOError:
        print "File was not possible to read."
    except Exception as e:
        print 'Error when parsing siglist: ' + e.message

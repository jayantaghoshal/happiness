# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from FDXDescriptionFileParser.Parser import FolderParser
from FDXDescriptionFileParser.FDXWriter import FDXWriter
from FDXDescriptionFileParser.LdfParser import LdfParser
from win32com.universal import com_error
from ArgumentParser import argument_parser
import os
import sys
import shutil
import time
import win32com.client
import fileinput
import os.path

__author__ = 'tkarls53'

canoe = None
number_of_warnings = 0
number_of_errors = 0
args = None


def verify_configuration_folder():
    """ Validates the configuration folder has Databases in it """
    if os.path.isfile(os.path.join(args.configuration_folder, 'Backbone.cfg')):
        return True
    else:
        raise Exception('Invalid configuration folder, {} does not have the Backbone.cfg file'
                        .format(args.configuration_folder))


def dispatch_canoe():
    """ Dispatch CANoe """
    try:
        global canoe
        global number_of_errors
        print 'Dispatching CANoe...'
        canoe = win32com.client.Dispatch('CANoe.Application')
        # What if file name is renamed? Is there any chance for that?
        canoe.Open(os.path.join(args.configuration_folder, 'Backbone.cfg'))
        print 'Done!'
    except com_error as err:
        number_of_errors += 1
        raise com_error('Failed to dispatch CANoe due to: "{}"'.format(parse_error_message(err)))


def remove_ub_handling_in_gateway_files_and_copy():
    global number_of_errors
    print '\nRemoving UB handling in GW files'
    output_folder = os.path.join(args.configuration_folder, 'Output')
    try:
        for file_name in os.listdir(output_folder):
            if file_name.endswith('GW.cin') and file_name != 'VCM_GW.cin':  # VCM shall not gateway signals.
                print 'Adapting {}'.format(file_name)
                abs_path = os.path.join(output_folder, file_name)
                f = fileinput.input(abs_path, inplace=1)
                remove_counter = 0
                for line in f:
                    # Remove (don't write) four consecutive lines if one ends with _UB, Chks or Cntr
                    if line.endswith('_UB\n') or line.endswith('Chks\n') or line.endswith('Cntr\n'):
                        remove_counter = 4
                    if remove_counter > 0:
                        remove_counter -= 1
                    else:
                        sys.stdout.write(line)
                fileinput.close()
                print 'Copying to Nodes folder'
                copy_files(abs_path, os.path.join(args.configuration_folder, 'Nodes', file_name))
        print 'Done!'
    except Exception as err:
        number_of_errors += 1
        print 'Error: Failed to fix GW files due to: "{}"'.format(err)


def copy_adaptation_files():
    """ Copy adaptation files from CAPL-folder to Nodes folder """
    global number_of_errors
    print '\nCopying adaptation files...'
    if not os.path.isdir(args.configuration_folder):
        number_of_errors += 1
        raise Exception('"{}" is not a valid folder. Please select a valid configuration '
                        'folder.'.format(args.configuration_folder))

    script_folder = os.path.dirname(os.path.realpath(__file__))
    source_folder = os.path.join(script_folder, '..', 'CAPL')
    target_folder = os.path.join(args.configuration_folder, 'Nodes')
    architecture = '' if not args.architecture else args.architecture.lower()
    file_list = [x.lower() for x in os.listdir(source_folder)]
    for file_name in file_list:
        if 'adaptation' in file_name:
            file_check = check_if_this_is_the_expected_revision_file(architecture, source_folder, file_name)
            if file_check[0]:
                copy_files(os.path.join(source_folder, file_name), os.path.join(target_folder, file_check[1]))
                print 'Copied {} to the Nodes folder as {}.'.format(file_name, file_check[1])
    print 'Done!'


def check_if_this_is_the_expected_revision_file(architecture, source_folder, file_name):
    """
    Checks for a revision of file_name for the given architecture
    :param architecture - architecture name icup, legacy
    :param source_folder - canoe simulation folder
    :param file_name - name of the file
    :return bool, str(filename)
    """
    if architecture == '' and file_name.endswith('adaptation.cin'):
        return True, file_name
    elif architecture is not '':
        split_names = file_name.split('_')
        if os.path.isfile(os.path.join(source_folder, file_name.replace('.cin', '_{}.cin'.format(architecture)))):
            print '{} revision exist skipping this file'.format(architecture)
            return False, None
        else:
            if split_names[len(split_names) - 1] == '{}.cin'.format(architecture):
                split_names.pop(len(split_names) - 1)
                split_names[len(split_names) - 1] = split_names[len(split_names) - 1] + '.cin'
                return True, '_'.join(split_names)
            elif split_names[len(split_names) - 1] == 'adaptation.cin':
                return True, '_'.join(split_names)
    return False, None


def add_sysvar_files():
    """ Add sysvar files to the CAPL-folder """
    print '\nAdding sysvar files...'

    capl_dir = os.path.abspath(os.path.join(args.configuration_folder, "PostProcessFiles"))

    add_sysvar(os.path.join(capl_dir, 'ASDM', 'ASDM_roadfriction.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'CCSM', 'CCSM_include.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'common', 'ConfigurationParameters.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'SWSM', 'SWSM_include.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'VDDM', 'VDDM_vehspd.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'MOST', '_MOST.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'Ethernet', '_VCCIP.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'Ethernet', '_VCCIP_IPLM.vsysvar'))
    add_sysvar(os.path.join(capl_dir, 'SRS', 'SRS_gyro.vsysvar'), False)

    copy_files(os.path.join(args.configuration_folder, 'Modules', 'SPAModule_CarConfig.vsysvar'),
               os.path.join(args.configuration_folder, 'Databases'))
    print 'Done!'


def add_sysvar(sysvar_file, copy_and_add_to_fdx_description_file=True):
    """
    Add sysvar file
    :param sysvar_file: Sysvar file to add
    :param copy_and_add_to_fdx_description_file: True if the system variable file shall be accessible by Awesome
    Framework
    :return:
    """
    assert sysvar_file.startswith(args.configuration_folder), "All added files must be in config directory"
    if copy_and_add_to_fdx_description_file:
        copy_files(sysvar_file, os.path.join(args.configuration_folder, 'Databases'))

    canoe.System.VariablesFiles.Add(sysvar_file)
    print 'Added {}'.format(sysvar_file)


def configure_lin():
    """ Configure LIN bus """
    global number_of_errors
    print '\nConfiguring LIN...'
    #try:
    database_folder = os.path.join(args.configuration_folder, 'Databases')
    assert (len(database_folder) > 0), "No LIN databases found"
    for file_name in os.listdir(database_folder):
        if file_name.endswith('.ldf'):
            if 'lin19' in file_name.lower():
                path_to_lin19_database = os.path.join(database_folder, file_name)
                ldf_parser = LdfParser(path_to_lin19_database)
                master_node, list_of_slave_nodes = ldf_parser.get_nodes_from_ldf_file()
                canoe.Configuration.SimulationSetup.Buses.AddWithType("LIN19", 5)   #5=LIN
                lin19 = canoe.Configuration.SimulationSetup.Buses("LIN19")
                lin19.Databases.Add(path_to_lin19_database)
                lin19.Nodes.Add("{}".format(master_node))
                for slave_node in list_of_slave_nodes:
                    lin19.Nodes.Add("{}".format(slave_node))
                print 'Added database {} for LIN19.'.format(file_name)
            if 'lin20' in file_name.lower():
                path_to_lin20_database = os.path.join(database_folder, file_name)
                ldf_parser = LdfParser(path_to_lin20_database)
                master_node, list_of_slave_nodes = ldf_parser.get_nodes_from_ldf_file()
                lin20 = canoe.Configuration.SimulationSetup.Buses("LIN20")
                lin20.Databases.Add(path_to_lin20_database)
                lin20.Nodes.Add("{}".format(master_node))
                for slave_node in list_of_slave_nodes:
                    lin20.Nodes.Add("{}".format(slave_node))
                print 'Added database {} for LIN20.'.format(file_name)
    print 'Done!'
    #except com_error as err:
    #    number_of_errors += 1
    #    print 'Error: Failed to configure LIN due to: "{}"'.format(parse_error_message(err))


def configure_control_can():
    """ Configuring ControlCAN """
    print '\nConfiguring ControlCAN...'
    global number_of_errors
    try:
        can2 = canoe.Configuration.SimulationSetup.Buses('controlCAN')
        remove_node('ADMM_17397')
        remove_node('CSM_GmbH')
        remove_node('CanGeneral_controlCAN')

        try:
            can2.Nodes.Add('XIL_addon')
        except com_error as err:
            number_of_errors += 1
            print parse_error_message(err)
        try:
            xil = canoe.Configuration.SimulationSetup.Nodes('XIL_addon')
            capl = os.path.join(args.configuration_folder, '..', 'canoesimulations', 'CAPL', 'common', 'XIL_addon.can')
            xil.FullName = capl
            print 'Done!'
        except com_error as err:
            number_of_errors += 1
            print err
    except com_error as err:
        number_of_errors += 1
        print 'Error: Failed to configure controlCAN due to: "{}"'.format(err.args)
        print 'HINT! Is this the second time you run this script? If that is the case, restart the Model Generation ' \
              'Wizard.'


def remove_node(node):
    """
    Remove node from configuration
    :param node: Node to remove
    :return:
    """
    global number_of_errors
    try:
        canoe.Configuration.SimulationSetup.Nodes.Remove(node)
        print 'Removed {}'.format(node)
    except com_error as err:
        number_of_errors += 1
        print 'Error: Failed to remove {} due to: "{}"'.format(node, parse_error_message(err))


def configure_most():
    """ Configure MOST bus """
    global number_of_errors
    print '\nConfiguring MOST...'
    database_folder = os.path.join(args.configuration_folder, 'MOST')
    if not os.path.isdir(database_folder):
        number_of_errors += 1
        print 'Error: Could not find this path: {}.'.format(database_folder)
        return

    for file_name in os.listdir(database_folder):
        if file_name.endswith('.xml'):
            db = os.path.join(database_folder, file_name)
            most_bus = canoe.Configuration.SimulationSetup.Buses('MOST')
            try:
                most_bus.Databases.Add(db)
            except com_error as err:
                number_of_errors += 1
                print 'Failed to add database to MOST bus due to: "{}"'.format(parse_error_message(err))
            try:
                most_bus.Nodes.Add('MOSTListener')
            except com_error as err:
                number_of_errors += 1
                print 'Failed to add MOSTListener to the nodes due to: "{}"'.format(parse_error_message(err))
            try:
                listener = canoe.Configuration.SimulationSetup.Nodes('MOSTListener')
                capl = os.path.join(args.configuration_folder, '..', 'canoesimulations', 'CAPL', 'MOST',
                                    'MOSTListener.can')
                listener.FullName = capl
                print 'Done!'
            except com_error as err:
                number_of_errors += 1
                print 'Failed to setup MOSTListener due to: "{}"'.format(parse_error_message(err))


def configure_vccip():
    """ Configuring Ethernet bus """
    global number_of_errors
    print '\nConfiguring Ethernet...'
    try:
        vccip = canoe.Configuration.SimulationSetup.Buses('VCCIP_CommandBus')
        vccip.Nodes.Add('EthernetSniffer')
        node = canoe.Configuration.SimulationSetup.Nodes('EthernetSniffer')
        capl = os.path.join(args.configuration_folder, '..', 'canoesimulations', 'CAPL', 'Ethernet',
                            'IPCommandbusToSysvarEthernetSniffer.can')
        node.FullName = capl
        ethernet_dll = os.path.join(os.environ.get('CANoe_InstallDir'), 'Ethernet_IL.dll')
        if not os.path.isfile(ethernet_dll):
            ethernet_dll = r'C:\Program Files\Vector CANoe 9.0\Exec32\Ethernet_IL.dll'
            if not os.path.isfile(ethernet_dll):
                ethernet_dll = r'C:\Program Files (x86)\Exec32\Ethernet_IL.dll'
        copy_files(ethernet_dll, args.configuration_folder)
        ethernet_dll = os.path.join(args.configuration_folder, 'Ethernet_IL.dll')
        node.Modules.Add(ethernet_dll)
        vccip_dll = os.path.join(os.environ.get('CANoe_InstallDir'), 'VCC_IP_CP_NL.dll')
        if not os.path.isfile(vccip_dll):
            vccip_dll = r'C:\Program Files\Vector CANoe 9.0\Exec32\VCC_IP_CP_NL.dll'
            if not os.path.isfile(vccip_dll):
                r'C:\Program Files (x86)\Exec32\VCC_IP_CP_NL.dll'
        copy_files(vccip_dll, args.configuration_folder)
        vccip_dll = os.path.join(args.configuration_folder, 'VCC_IP_CP_NL.dll')
        node.Modules.Add(vccip_dll)
        canoe.Configuration.UserFiles.Add(vccip_dll)
        database_folder = os.path.join(args.configuration_folder, 'Vector_CN_ASNV')
        for file_name in os.listdir(database_folder):
            canoe.Configuration.UserFiles.Add(os.path.join(database_folder, file_name))
        print 'Done!'
    except com_error as err:
        number_of_errors += 1
        print 'Failed to add VCCIP listener due to: "{}"'.format(parse_error_message(err))


def configure_fdx():
    """ Configure FDX """
    global number_of_errors
    print '\nConfiguring FDX...'
    #copy_files(r'C:\ProgramData\ASAM\XIL\Implementation\Vector.CANoe.imf', args.configuration_folder)
    copy_files(os.path.join(args.configuration_folder, 'SysVarDef.xml'),
               os.path.join(args.configuration_folder, 'Databases', 'SysVarDef.vsysvar'))
    copy_files(os.path.join(args.configuration_folder, 'Output', 'SignalList.txt'),
               os.path.join(args.configuration_folder, 'Databases', 'SignalList.siglist2'))
    writer = FDXWriter(1100, args.configuration_folder)
    folder_parser = FolderParser(writer)
    folder_parser.parse_multiple_files(os.path.join(args.configuration_folder, 'Databases'), False, True)
    fdx_file = os.path.join(args.configuration_folder, 'FDXDescriptionFile.xml')
    try:
        canoe.Configuration.FDXFiles.Add(fdx_file)
        print 'Done!'
    except com_error as err:
        number_of_errors += 1
        print 'Error: Failed to add FDXDescription file due to: "{}"'.format(parse_error_message(err))
        print 'HINT! Is this the second time you run this script? If that is the case, restart the Model Generation ' \
              'Wizard.'


def add_panels():
    """ Add panels """
    print '\nAdding panels...'
    capl_dir = os.path.abspath(os.path.join(args.configuration_folder, "PostProcessFiles"))

    add_panel(os.path.join(capl_dir, 'common', 'IOPiggy_Control.xvp'))
    add_panel(os.path.join(capl_dir, 'ASDM', 'RoadFriction.xvp'))
    add_panel(os.path.join(capl_dir, 'CCSM', 'CCSM_Panel.xvp'))
    add_panel(os.path.join(capl_dir, 'SWSM', 'SWSM_Panel.xvp'))
    add_panel(os.path.join(capl_dir, 'VDDM', 'SimpleSpeed.xvp'))
    print 'Done!'


def add_panel(panel):
    """
    Add panel to configuration.
    :param panel: Panel to add
    :return:
    """

    assert panel.startswith(args.configuration_folder), "All added files must be in config directory"

    opanels = canoe.Configuration.GeneralSetup.PanelSetup.Panels(0)
    opanels.Add(panel)
    print 'Added panel {}'.format(panel)


def minimize_panel(panel):
    """
    Minimize panel
    :param panel: Panel to minimize
    :return:
    """
    print '\nMinimizing panel {}...'.format(panel)
    opanels = canoe.Configuration.GeneralSetup.PanelSetup.Panels(0)
    opanel = opanels(panel)
    opanel.Visible = False
    print 'Done!'


def deactivate_nodes():
    """ Deactivate nodes """
    print '\nDeactivating nodes...'
    deactivate_node('IHU')
    #deactivate_node('VCM')
    #deactivate_node('DIM')
    #deactivate_node('VGM')
    #deactivate_node('TCAM')
    #deactivate_node('WAM')
    print 'Done!'


def deactivate_node(node):
    """
    Deactivate node
    :param node: Node to deactivate
    :return:
    """
    print 'Deactivating {}...'.format(node)
    canoe.Configuration.SimulationSetup.Nodes(node).Active = False


def save_configuration():
    """ Save CANoe configuration """
    print '\nSaving configuration...'
    canoe.Configuration.Save()
    print 'Done!'

def verify_solution():
    """ Verify the solution and to see if it compiles """
    global number_of_errors
    print '\nVerifying solution...'

    canoe.CAPL.Compile()
    time.sleep(10)
    if canoe.CAPL.CompileResult.Result == 0:
        print 'Success!!! (At least it compiles...)'
    else:
        print 'Error: Something went wrong during compilation!'


def parse_error_message(err):
    """
    Parse error message from CANoe.
    :param err: Original error message
    :return: Parsed error message
    """
    if err.excepinfo[2]:
        return err.excepinfo[2]
    else:
        return err


def copy_files(from_file, to_file):
    shutil.copy2(from_file, to_file)

def main():
    """ Main function for post processing the CANoe configuration """

    global args
    args = argument_parser()
    if not os.path.isabs(args.configuration_folder):
        args.configuration_folder = os.path.abspath(os.path.join(os.getcwd(), args.configuration_folder))

    capl_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "CAPL"))
    capl_dest_dir = os.path.abspath(os.path.join(args.configuration_folder, "PostProcessFiles"))


    verify_configuration_folder()
    dispatch_canoe()
    if args.which == 'build':
        # Copy the whole CAPL dir into the simulation directory to make the configuration self contained
        shutil.copytree(capl_dir, capl_dest_dir)

        copy_adaptation_files()
        remove_ub_handling_in_gateway_files_and_copy()
        add_sysvar_files()
        configure_lin()
        #if not args.exclude_control_can:
        #    configure_control_can()
        #if not args.exclude_most:
        #    configure_most()
        #if not args.exclude_vcc_ip:
        #    configure_vccip()
        configure_fdx()
        #if not args.exclude_panels:
        #    add_panels()
        #    minimize_panel('IOPiggy_Control')
        deactivate_nodes()
        save_configuration()
        verify_solution()
    print '\n{}\nScript was completed with {} errors and {} warnings.'.format('-' * 120, number_of_errors,
                                                                              number_of_warnings)

    print("Remaining tasks to do:")
    print(" 1. Update Usage=0 in Backbone_AutosarFlexRayNM32.INI")
    print(" 2. File -> options ->extenstions -> FDX -> Enable FDX")
    print(" 3. Enable one LIN channel in Channel hardware and assign it to the LIN net in simulation setup")


if __name__ == '__main__':
    main()

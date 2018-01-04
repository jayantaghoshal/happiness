# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json
import os
import re
import sys
import textwrap

from argparse import ArgumentParser
from collections import defaultdict
from ConfigParser import SafeConfigParser as ConfigParser
from functools import partial
from itertools import chain
from os.path import abspath, dirname, join as path_join
from pprint import PrettyPrinter
from xml.etree import ElementTree as ET

from django.db.utils import IntegrityError


DEFAULT_COMMENT='Created by fdxdescription_parser'
SYSVAR_SEPARATOR='/'
SIGNAL_SEPARATOR='::'
RIG_NAME=None # Set from config file at run time

def _p(dict_):
    PrettyPrinter(indent=2).pprint(dict_)

def handle_arguments():
    current_dir = abspath(dirname(__file__))
    parser = ArgumentParser(
        description='Parse FDXDescription.xml and MAPortConfig.xml to create '
                    'Variables, Symbols, mappings between them automatically '
                    'in AwesomeFramework.')
    for flags, opts in (
        (['-c', '--config'],
         {'dest': 'config_file',
          'metavar': 'config_file_path',
          'default': path_join(current_dir,
                               'fdxdescription_parser_config.cfg')}),
        (['awesomeframework_path'],
         {'nargs': 1,
          'help': 'Absolute path to the folder where AwesomeFramework is '
                  'installed.'}),
        (['--dryrun'],
         {'action': 'store_true',
          'help' : 'Parses xml files and creates the database objects, but '
                   'does not save them to the database.'})):
        parser.add_argument(*flags, **opts)
    return parser, parser.parse_args()

def parse_config_file(config_file_path):
    config = ConfigParser()
    config.read(config_file_path)
    config = dict(config.items('fdxdescription_parser'))
    for valid_key in ('fdxdescription_path', 'ma_port_config_path',
                      'backbone_config_path', 'rig_name'):
        assert valid_key in config.keys(), ('"{}" not found in config '
                                            'file'.format(valid_key))
        assert config[valid_key], ('"{}" does not have any value in config '
                                   'file'.format(valid_key))
    return config

def do_setup_and_imports(arg_parser, awesomeframework_path):
    # We do AwesomeFramework-related database model imports here instead of the
    # beginning of the file because we want fdxdescription_parser.py -h to work.
    #
    # We expose them globally so they are usabe for the rest of this script.
    global Autosar
    global Rig
    global StateValueList
    global Symbol
    global SymbolMapping
    global VariableMapping

    virtualenv = os.path.join(awesomeframework_path,
                              'env',
                              'Scripts',
                              'activate_this.py')
    execfile(virtualenv, dict(__file__=virtualenv))
    if not awesomeframework_path in sys.path:
        sys.path.append(path_join(awesomeframework_path, 'src'))
    if not os.environ.get('DJANGO_SETTINGS_MODULE', False):
        os.environ['DJANGO_SETTINGS_MODULE'] = \
            'af_django_root.af_django.settings'
    # Django requires this setup call and for that, it needs access to Django
    # settings
    import django
    try:
        django.setup()
        from af_django_root.af_django_webapp.models import (Autosar,
                                                            Rig,
                                                            StateValueList,
                                                            Symbol,
                                                            SymbolMapping,
                                                            VariableMapping)
    except (django.core.exceptions.ImproperlyConfigured, ImportError) as e:
        arg_parser.print_help()
        raise

# Based on:
# https://stackoverflow.com/a/10077069
def xml_to_dict(xml_node):
    ret = {xml_node.tag: {} if xml_node.attrib else None}
    children = list(xml_node)
    if children:
        child = defaultdict(list)
        for child_as_dict in map(xml_to_dict, children):
            for k, v in child_as_dict.items():
                child[k].append(v)
        ret = {xml_node.tag: {k: v[0] if len(v) == 1 else v for k,v in child.items()}}
    if xml_node.attrib:
        ret[xml_node.tag].update(('@%s' % k, v) for k, v in xml_node.attrib.items())
    if xml_node.text:
        element_text = xml_node.text.strip()
        if children or xml_node.attrib:
            if element_text:
                ret[xml_node.tag]['#text'] = element_text
        else:
            ret[xml_node.tag] = element_text
    return ret

def get_namespace(value):
    namespace = re.match(r'{(.*)}', value)
    if not namespace:
        raise Exception('Could not extract XML namespace, cannot proceed')
    return namespace.group(1)

def lower_and_remove_namespaces_from_keys(dict_):
    #This method modifies the keys *in-place*
    for key, value in dict_.iteritems():
        key_in_small = key.lower()
        dict_[key_in_small] = dict_.pop(key)
        key = key_in_small
        if '{http' in key:
            stripped_key = key.rsplit('}', 1)[1]
            dict_[stripped_key] = dict_.pop(key)
        if not isinstance(value, list):
            value = [value]
        for v in value:
            if isinstance(v, dict):
                lower_and_remove_namespaces_from_keys(v)

def extract_state_values(backbone_dict):
    # Define helpers to add namespace as well as formatting when accessing
    # backbone_dict.
    #
    # Although we clean the keys and so on later on, we do this
    # acccessing first to reduce the number of items needed to handle.
    namespace = get_namespace(backbone_dict.keys().pop())
    ns = partial(lambda x: '{' + namespace + '}' + x.upper())
    ns2 = partial(lambda x: '{' + namespace.rsplit('/', 1)[0] + '}' + x.upper())
    coding_refs = {}
    # same coding is used in many signals; use set() to remove duplicates
    unique_coding_refs = set()
    for el in backbone_dict[ns('fibex')][ns('elements')][ns('signals')][ns('signal')]:
        name, coding_ref = el[ns2('short-name')], el[ns('coding-ref')]['@ID-REF']
        coding_refs[name] = coding_ref
        unique_coding_refs.add(coding_ref)
    codings = filter(lambda el: el['@ID'] in unique_coding_refs,
                     backbone_dict[ns('fibex')][ns('processing-information')][ns('codings')][ns('coding')])
    map(lower_and_remove_namespaces_from_keys, codings)
    # Save only relevant part of the whole coding
    for ref_name, ref_id in coding_refs.iteritems():
        coding = filter(lambda e: e['@id'] == ref_id, codings).pop()
        try:
            coding_refs[ref_name] = coding['compu-methods']['compu-method']
        except KeyError:
            # TODO FIXME: handle exceptions of the format:
            # 'coded-type': {
            #     '@category': 'STANDARD-LENGTH-TYPE',
            #     'base-data-type': 'A_UINT16',
            #     'bit-length': '10'
            #  }
            # _p(coding)
            coding_refs[ref_name] = {}
    return coding_refs

def get_testbench_variable_name(item):
    # this intentionally breaks down if `item` does not have
    # child called either 'sysvar' or 'signal'
    sysvar = item.get('sysvar', None)
    if sysvar:
        separator = SYSVAR_SEPARATOR
        parts = (sysvar['@namespace'], sysvar['@name'])
    else:
        separator = SIGNAL_SEPARATOR
        parts = (item['signal']['@bus'],
                 item['signal']['@msg'],
                 item['signal']['@name'])
    var_name = separator.join(parts)
    autosar_name = parts[-1]
    assert not var_name.startswith(separator), 'First part of name was empty string'
    assert not var_name.endswith(separator), 'Last part of name was empty string'
    assert not separator * 2 in var_name, 'Middle part of name was empty string'
    return var_name, autosar_name

def extract_items(datagroup, ma_port_config_dict, item_to_state_value_mapping):
    find_id = partial(filter,
                      lambda elem: elem['#text'] == datagroup['@groupID'])
    variable_readable = len(find_id(ma_port_config_dict['FdxReadGroup'])) > 0
    variable_writable = len(find_id(ma_port_config_dict['FdxWriteGroup'])) > 0
    try:
        items = datagroup['item']
    except KeyError:
        # TODO FIXME: collect to a list: these are invalid sysvars to begin with
        #_p(datagroup)
        return []
    if not isinstance(items, list):
        #`items` in some cases is only single item; normalize to a list()
        items = [items]
    # save needed stuff for each element so we can easily
    # check them later on
    for i in items:
        i['variable_readable'] = variable_readable
        i['variable_writable'] = variable_writable
        i['testbench_variable_name'], i['autosar_name'] = get_testbench_variable_name(i)
        i['state_value_list'] = item_to_state_value_mapping.get(i['autosar_name'], None)
    return items

def parse_xmls(fdxdescription_path, maportconfig_path, backbone_spa_path):
    fdx_dict = xml_to_dict(ET.parse(fdxdescription_path).getroot())
    ma_port_config_dict = xml_to_dict(ET.parse(maportconfig_path).getroot())
    backbone_dict = xml_to_dict(ET.parse(backbone_spa_path).getroot())
    item_to_state_value_mapping = extract_state_values(backbone_dict)
    items = map(lambda dg: extract_items(dg, ma_port_config_dict['Port'], item_to_state_value_mapping),
                fdx_dict['canoefdxdescription']['datagroup'])
    return items

def skip_if_dryrun(func):
    def wrapper(*args, **kwargs):
        if not skip_if_dryrun.dryrun:
            return func(*args, **kwargs)
        return (None, False)
    return wrapper

@skip_if_dryrun
def get_or_create_db_object(model, **kwargs):
    return model.objects.get_or_create(**kwargs)

def extract_computations(state_value_list):
    computation = VariableMapping.IDENTICAL
    svl_object = None
    if not state_value_list:
        return computation, svl_object
    computation = {
        'IDENTICAL': VariableMapping.IDENTICAL,
        'LINEAR': VariableMapping.LINEAR,
        'TEXTTABLE': VariableMapping.TEXTTABLE
    }.get(state_value_list['category'], VariableMapping.IDENTICAL)
    if not computation == VariableMapping.TEXTTABLE:
        return computation, svl_object
    scales = state_value_list['compu-internal-to-phys']['compu-scales']['compu-scale']
    # Get id from first compu-scale element, name being in the following format:
    # <id>_<scale name>
    id_ = scales[0]['compu-const']['vt'].split('_', 1)[0]
    assert id_, 'Could not extract id for StateValueList that is TEXTTABLE'
    svl_object, was_newly_created = get_or_create_db_object(StateValueList,
                                                            identifier=id_,
                                                            defaults={'key_value_pairs': '{}'})
    if was_newly_created:
        json_dict = {}
        for scale in scales:
            key = scale['compu-const']['vt'].split('_', 1)[1]
            value = float(scale['lower-limit']['#text'])
            assert key is not None, 'Could not extract scale key: {}'.format(scale)
            assert value is not None, 'Could not extract scale value: {}'.format(scale)
            json_dict[key] = value
        assert json_dict, 'Parsed json was empty for StateValueList that is TEXTTABLE'
        svl_object.key_value_pairs = json.dumps(json_dict)
        svl_object.save()
    return computation, svl_object

def print_progress(msg):
    sys.stdout.write('\r' + msg)
    sys.stdout.flush()

@skip_if_dryrun
def try_db_action(func, *args, **kwargs):
    return func(*args, **kwargs)

def create_variable_mappings(items):
    # `items` is a list of list of items
    vms, duplicates = [], []
    items = chain(*items)
    for index, item in enumerate(items):
        # if testbench_variable_name is "FOO::BAR BAZ::CUX" and rig name is
        # "CORGI", then identifier should be "FOO_CUX_CORGI"
        identifier = item['testbench_variable_name']
        if SYSVAR_SEPARATOR in identifier:
            identifier = identifier.replace(SYSVAR_SEPARATOR, '_')
        elif SIGNAL_SEPARATOR in identifier:
            first, _, last = identifier.split('::', 2)
            identifier = '_'.join([first, last])
        identifier = identifier + '_' + RIG_NAME
        identifier = identifier.replace(' ', '_')
        type_ = {
            'int32': VariableMapping.INT,
            'float': VariableMapping.FLOAT,
            'string': VariableMapping.UINTSTR
        }.get(item['@type'], None)
        if not type_:
            # TODO FIXME: does not handle int32array
            continue
        (computation,
         computation_kv_pairs) = extract_computations(item['state_value_list'])
        vm = VariableMapping(
            identifier=identifier,
            testbench_variable_type=type_,
            testbench_variable_name=item['testbench_variable_name'],
            testbench_port_id='ASAM_MAPort',
            testbench_variable_readable=item['variable_readable'],
            testbench_variable_writable=item['variable_writable'],
            framework_computation=computation,
            framework_computation_factor=None,
            framework_computation_offset=None,
            framework_computation_key_value_pairs=computation_kv_pairs,
            comment=DEFAULT_COMMENT)
        try:
            try_db_action(vm.save)
        except IntegrityError:
            duplicates.append(vm)
        vms.append((vm, item['autosar_name']))
        print_progress(str(index + 1))
    print
    return vms, duplicates

def get_symbol_identifier(autosar_name, testbench_variable_name):
    if not SIGNAL_SEPARATOR in testbench_variable_name:
        return autosar_name
    bus = testbench_variable_name.split('::', 1)[0]
    # per convention, 'Backbone' is not added as a bus to id
    if bus.lower() == 'backbone':
        return autosar_name
    return '{}_{}'.format(autosar_name, bus)

def create_symbols(vms_and_autosar_names):
    vms_and_symbols, duplicates = [], []
    for index, (variable_mapping, autosar_name) in enumerate(vms_and_autosar_names):
        type_ = {VariableMapping.INT: Symbol.INT,
                 VariableMapping.FLOAT: Symbol.FLOAT,
                 VariableMapping.UINTSTR: Symbol.STRING}[variable_mapping.testbench_variable_type]

        min_, max_ = None, None
        if type_ == Symbol.INT:
            min_, max_ = 0, 255
        if type_ == Symbol.FLOAT:
            min_, max_ = 0, 1.0

        id_ = get_symbol_identifier(autosar_name,
                                    variable_mapping.testbench_variable_name)
        autosar_obj, _ = get_or_create_db_object(Autosar, name=autosar_name)
        sym, just_created = get_or_create_db_object(Symbol, identifier=id_)

        if just_created:
            sym.autosar_name=autosar_obj
            sym.description=''
            sym.comment=DEFAULT_COMMENT
            sym.type=type_
            sym.max=max_
            sym.min=min_
            sym.unit=None
            sym.state_list=None
            # this call will ONLY update the db, not insert a new object
            # more info here:
            # https://docs.djangoproject.com/en/2.0/ref/models/instances/#what-happens-when-you-save
            try_db_action(sym.save)

        vms_and_symbols.append((variable_mapping, sym))
        print_progress(str(index + 1))
    print
    return vms_and_symbols, duplicates

def create_symbol_mappings(variable_mappings_and_symbols):
    symbol_mappings, duplicates = [], []
    rig, _ = get_or_create_db_object(Rig, name=RIG_NAME)
    for index, (vm, symbol) in enumerate(variable_mappings_and_symbols):
        sm, just_created = get_or_create_db_object(SymbolMapping,
                                                   symbol=symbol,
                                                   rig=rig)
        target_list = duplicates
        if just_created:
            sm.read_mapping = vm if vm.testbench_variable_readable else None
            sm.write_mapping = vm if vm.testbench_variable_writable else None
            # this call will ONLY update the db, not insert a new object
            # more info here:
            # https://docs.djangoproject.com/en/2.0/ref/models/instances/#what-happens-when-you-save
            try_db_action(sm.save)
            target_list = symbol_mappings
        target_list.append(sm)
        print_progress(str(index + 1))
    print
    return symbol_mappings, duplicates

def append_infotainment_subset(vms_and_symbols, info_subset):
    all_vms_and_symbols = list(vms_and_symbols)
    count = 0
    for index, (vm, symbol) in enumerate(vms_and_symbols):
        # NOTE! the following call will fail with an exception in dryrun
        # since the Autosar object of `symbol` is None, hence, we skip
        # appending the infotainment subset in this case
        if not skip_if_dryrun.dryrun:
            symbol_list = [sm.symbol for sm in info_subset if sm.symbol.autosar_name == symbol.autosar_name]
            for sym in symbol_list:
                if (vm, sym) not in all_vms_and_symbols:
                    all_vms_and_symbols.append((vm, sym))
                    print_progress(str(count + 1))
                    count += 1
    print
    return all_vms_and_symbols

def get_infotainment_subset():
    info_rig = Rig.objects.filter(name='InfotainmentSubset')
    return SymbolMapping.objects.filter(rig=info_rig).exclude(symbol__autosar_name__isnull=True)

@skip_if_dryrun
def print_end_help():
    try:
        import curses
        _, width = curses.initscr().getmaxyx()
    except Exception as e:
        width = 70
    msg = '''!ONE MORE STEP!

This script has now changed the database AwesomeFramework uses and thus, the changes are not yet in full effect. At this point, you can still go back by running:

> python path/to/AwesomeFramework.py reloaddb

If you want to go through with the changes, follow these steps:

1) Open AwesomeFramework and make sure you are in your workspace:
develop -> <your username>

2) Run in the command line the following command:

> python path/to/AwesomeFramework.py dumpdb

3) In AwesomeFramework, verify that you have expected changes. If you have and are still completely sure you want to make these changes effective, publish your workspace.
'''
    fill = partial(textwrap.fill, width=width)
    print '\n'.join(map(fill, msg.splitlines()))

def main(fdxdescription_path, maportconfig_path, backbone_spa_path):
    print 'Dryrun is ON!' if skip_if_dryrun.dryrun else ''
    info_subset = get_infotainment_subset()
    print 'Parsing xml...'
    items = parse_xmls(fdxdescription_path, maportconfig_path, backbone_spa_path)
    print 'Creating Variable Mappings...'
    variable_mappings_and_autosar_names, duplicate_variable_mappings = \
        create_variable_mappings(items)
    print 'Creating Symbols...'
    variable_mappings_and_symbols, duplicate_symbols = \
        create_symbols(variable_mappings_and_autosar_names)
    # We cannot use set()'s to remove duplicates, because we need to preserve
    # order. However, The Internet(tm) thought this would be performant enough:
    # https://stackoverflow.com/a/4211228 (see comments)
    duplicate_variable_mappings = set(duplicate_variable_mappings)
    duplicate_symbols = set(duplicate_symbols)
    variable_mappings_and_symbols = [e for e in variable_mappings_and_symbols
        if e[0] not in duplicate_variable_mappings
        or e[1] not in duplicate_symbols]
    print 'Appending InfotainmentSubset database...'
    all_variable_mappings_and_symbols= \
        append_infotainment_subset(variable_mappings_and_symbols, info_subset)
    print 'Creating Symbol Mappings...'
    symbol_mappings, duplicate_symbol_mappings = \
        create_symbol_mappings(all_variable_mappings_and_symbols)
    print '-----\nDONE!'
    msg = ('Created {} new variable mappings, {} duplicates, {} in total\n'
           'Created {} new symbols, {} duplicates, {} in total\n'
           'Created {} new symbol mappings, {} modified, {} in total')
    # If all created vms and symbols were duplicate,
    # `variable_mappings_and_symbols` is empty
    new_vms, new_symbols = [], []
    if variable_mappings_and_symbols:
        new_vms, new_symbols = zip(*variable_mappings_and_symbols)
    print msg.format(*map(len, [
        new_vms,
        duplicate_variable_mappings,
        all_variable_mappings_and_symbols + list(duplicate_variable_mappings),
        new_symbols,
        duplicate_symbols,
        all_variable_mappings_and_symbols + list(duplicate_symbols),
        symbol_mappings,
        duplicate_symbol_mappings,
        symbol_mappings + duplicate_symbol_mappings]))
    print '-----'
    print_end_help()

if __name__ == '__main__':
    parser, args = handle_arguments()
    skip_if_dryrun.dryrun = args.dryrun
    config = parse_config_file(args.config_file)
    RIG_NAME = config['rig_name']
    # awesomeframework_path is a list with single item; just pass the item
    do_setup_and_imports(parser, args.awesomeframework_path.pop())
    main(config['fdxdescription_path'],
         config['ma_port_config_path'],
         config['backbone_config_path'])


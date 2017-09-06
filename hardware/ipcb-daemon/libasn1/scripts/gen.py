#!/usr/bin/env python

"""
Generate Service ID and Operation ID enums, ToString() and ServiceIdIsValid() functions based on
VCC specs in doc/docx format (requires external application antiword).
Requires clang-format.
"""

import argparse
import os
import re
import subprocess
import sys


def name_to_identifier(identifier):
    """ Create an identifier from a text description.
        Essentially removes all non-alphanumeric characters. """
    return re.sub(r"[^\w]", r"", identifier)


def replace_in_c_file(in_filename, out_filename, replace_dict):
    """ Replace all in replacestrings in replaceDict in inFilename and write to outFilename,
        then run clang-format on the result file. """
    with open(in_filename, "r") as f_in:
        content = f_in.read()
        with open(out_filename, "w") as f_out:
            for replace_pattern, replace_string in replace_dict.items():
                content = re.sub(r"\$\(%s\)" % replace_pattern, replace_string, content)
            f_out.write(content)
        subprocess.check_call(["clang-format", "-i", out_filename])


def cleanpath(path):
    return os.path.abspath(os.path.normpath(os.path.expandvars(os.path.expanduser(path))))


class Id(object):

    Undefined = 0

    def __init__(self, numeric_id, name):
        self.numeric_id = numeric_id
        self.name = name

    def __eq__(self, other):
        return self.numeric_id == other.numeric_id and self.name == other.name

    def __hash__(self):
        return self.numeric_id

    def identifier(self):
        return name_to_identifier(self.name)


class IdCombined(object):

    def __init__(self, service_id, operation_id):
        self.service = service_id
        self.operation = operation_id

    def __eq__(self, other):
        return self.service == other.service and self.operation == other.operation

    def __hash__(self):
        return (self.service.numeric_id << 16) | self.operation.numeric_id


class IdCombinedList(object):

    def __init__(self):
        self.id_list = []

    def __contains__(self, item):
        return item in self.id_list

    def add(self, id_combined):
        self.id_list.append(id_combined)

    def sorted_service_id_list(self):
        return sorted(set([x.service for x in self.id_list]), key=lambda svc: svc.numeric_id)

    def sorted_operation_id_list(self, sid=None):
        if sid is None:
            return sorted(set([x.operation for x in self.id_list]), key=lambda op: op.numeric_id)
        return sorted(set([x.operation for x in self.id_list if x.service == sid or x.operation.numeric_id == Id.Undefined]),
                      key=lambda op: op.numeric_id)

    def sorted_id_combined_list(self):
        return sorted(set([x for x in self.id_list]), key=lambda c: c.__hash__())


def read_file(sids, filename):
    """ Read files and fill sids list """
    if re.search(r"\.docx{0,1}$", filename, re.IGNORECASE):
        sid = None
        op_name = ""
        for line in subprocess.check_output(["antiword", "-w", "500", filename]).splitlines():
            match = re.search(r"^\|Service\s+\|(.*)\s+0x(\w+)", line)
            if match:
                sid = Id(int(match.group(2), 16), name_to_identifier(match.group(1)))
            match = re.search(r"^\|Operation Name.*\|(\w+)", line)
            if match:
                if sid:
                    op_name = match.group(1)
                else:
                    raise RuntimeError("No ServiceID when Operation Name encountered: %s" % op_name)
            match = re.search(r"^\|Unique( Identifier|).*\|0x(\w+)", line)
            if match:
                if sid and op_name != "":
                    sids.add(IdCombined(sid, Id(int(match.group(2), 16), op_name)))
                else:
                    raise RuntimeError("No ServiceID or operation name when unique id encountered: 0x%04X" %
                                       int(match.group(2), 16))
                sid = None
                op_name = ""
    else:
        raise RuntimeError("Unsupported file type: %s" % filename)
    return sids


def main():
    parser = argparse.ArgumentParser("VccIpCmdApi source/header generator")
    parser.add_argument('--template-dir', action="store", help="Template file directory")
    parser.add_argument('--output-header-dir', action="store", help="Header (.h) output directory")
    parser.add_argument('--output-source-dir', action="store", help="Source (.cpp) output directory")
    parser.add_argument('specs', nargs='*', help="Spec .doc/.docx file or .asn file")
    option = parser.parse_args(sys.argv[1:])

    if not option.template_dir:
        option.template_dir = os.path.abspath(os.path.dirname(sys.argv[0]))
        print("Setting template dir to %s" % option.template_dir)

    if not option.output_header_dir:
        option.output_header_dir = os.path.abspath(os.path.dirname(sys.argv[0]))
        print("Setting header dir to %s" % option.output_header_dir)

    if not option.output_source_dir:
        option.output_source_dir = os.path.abspath(os.path.dirname(sys.argv[0]))
        print("Setting source dir to %s" % option.output_source_dir)

    # Create list and add a few default entries.
    id_list = IdCombinedList()
    id_list.add(IdCombined(Id(Id.Undefined, "Undefined"), Id(Id.Undefined, "Undefined")))
    # Reference: VDR-NOTE-SWRS-31843799-INS-01-14 VCCIP.pdf [VCC IP Prot: 0073/;-0]
    id_list.add(IdCombined(Id(0xFFFF, "LM"), Id(0xFF01, "IpActivity")))

    for filename in option.specs:
        id_list = read_file(id_list, filename)

    document_list = ", ".join(["'%s'" % os.path.basename(f) for f in option.specs])

    replacements = {}
    replacements['SERVICE_ID_ENUM'] = ",\n".join(["%s = 0x%04X" % (sid.identifier(), sid.numeric_id)
                                                  for sid in id_list.sorted_service_id_list()])
    replacements['OPERATION_ID_ENUM'] = "\n".join(["%s = 0x%04X, // %s (0x%04X)" % (
        cid.operation.identifier(),
        cid.operation.numeric_id,
        cid.service.name,
        cid.service.numeric_id) for cid in id_list.sorted_id_combined_list()])
    replacements['SPEC_FILES'] = document_list
    replacements['TEMPLATE_FILE'] = "VccIpCmdApi.h.in"
    replacements['GENERATE_COMMAND'] = " ".join(sys.argv)
    replace_in_c_file(os.path.join(cleanpath(option.template_dir), "VccIpCmdApi.h.in"),
                      os.path.join(cleanpath(option.output_header_dir), "VccIpCmdApi.h"),
                      replacements)

    replacements = {}
    replacements['SERVICEID_STRING_MAPPING'] = "\n".join(["{ServiceId::%s, \"%s\"}," % (sid.identifier(),
                                                                                        sid.identifier())
                                                          for sid in id_list.sorted_service_id_list()])
    replacements['COMBINEDID_STRING_MAPPING'] = "\n".join(["{CombinedId(ServiceId::%s, OperationId::%s), \"%s\"}," %
                                                           (sid.identifier(), oid.identifier(), oid.identifier())
                                                           for sid in id_list.sorted_service_id_list()
                                                           for oid in id_list.sorted_operation_id_list(sid)])
    replacements['SERVICE_ID_VALID_FUNCTION'] = "\n".join(["case ServiceId::%s:" % sid.identifier()
                                                           for sid in id_list.sorted_service_id_list()
                                                           if sid.numeric_id != 0])
    replacements['TEMPLATE_FILE'] = "VccIpCmdApi.cpp.in"
    replacements['GENERATE_COMMAND'] = " ".join(sys.argv)
    replace_in_c_file(os.path.join(cleanpath(option.template_dir), "VccIpCmdApi.cpp.in"),
                      os.path.join(cleanpath(option.output_source_dir), "VccIpCmdApi.cpp"),
                      replacements)

# Main
if __name__ == "__main__":
    main()

#!/usr/bin/env python
# coding:UTF-8

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
Car Configuration Database Parser.
The parser takes a spreadsheet extract from
VCC car configuration database and creates
a header file.

Usage:
  parse_ccdb.py CCDB TEMPLATE OUTPUTFILE

Arguments:
  CCDB          A VCC car config database spreadsheet export in
                    xls, xlsx, xlsm, csv, or ods
  TEMPLATE      The template to base the generated header file on.
  OUTPUTFILE    The file to write the generated header file to.

Options:
  -h --help     Show this screen.
  --version     Show version.

"""

import os
import sys
import string
import datetime
import re
try:
    import docopt
except ImportError:
    pass

import jinja2

PARSER_VERSION = "0.2"

VALID_EXTENSIONS = (".xls", ".xlsx", ".xlsm", ".csv", ".ods") #Update __doc__ if this changes

DIGITS = { '1':'One',
           '2':'Two',
           '3':'Three',
           '4':'Four',
           '5':'Five',
           '6':'Six',
           '7':'Seven',
           '8':'Eight',
           '9':'Nine',
           '0':'Zero'}

# Class for storing Parameter Values
class CC_Value:
    value = ''
    description = ''
    def __init__(self, value, desc):
        self.value = value
        self.desc = desc


class PrintToTerminal:
    '''
    Provides a static method in order to print to the terminal
    enables ASCII escape code in order to color the output
    for more visibiliy
    usage: PrintToTerminal.printc(level, text)
    '''
    FAIL = '\033[91m'
    WARN = '\033[93m'
    ENDC = '\033[0m'

    @staticmethod
    def printc(level, text):
        if level == 'warning':
            print(PrintToTerminal.WARN + text + PrintToTerminal.ENDC)
        elif level == 'error':
            print >> sys.stderr, PrintToTerminal.FAIL + 'ERROR:' + text + PrintToTerminal.ENDC
        else:
            print(text + PrintToTerminal.ENDC)

# Class for storing Parameters
class CC_Param:
    number = 0
    name = ''
    desc = ''
    values = []

    def __init__(self, number, name, desc):
        self.number = number
        self.name = name
        # Make sure 'desc' is not None, instead empty string
        if desc != None:
            self.desc = desc
        self.values = []

    # Add a new parameter value
    def addValue(self, value, desc):
        for iterVal in self.values:
            #Ensure there are no duplicate descriptions
            if desc == iterVal.desc:
                desc = desc + '_X'
        self.values.append(CC_Value(value, desc))

# Helper function to fix non-ascii characters
def fixUnicode(text):
    CharFix = { str.decode('å', 'UTF-8'): 'a',
                str.decode('ä', 'UTF-8'): 'a',
                str.decode('ö', 'UTF-8'): 'o'}
    for letter in text:
        if letter in CharFix.keys():
            text = text.replace(letter, CharFix[letter], 1)
        else:
            if ord(letter) > 127:
                text = text.replace(letter, '_', 1)
    return text

# Main script function.
def parse_ccdb(filename, template_file, output_file, caller = ""):
    if len(caller) == 0:
        caller = "parse_ccdb"

    paramlist = []

    filename = os.path.abspath(filename)

    ##Checking paths
    if not os.path.exists(filename):
        print("Carconfig file: " + filename + " is missing")
        return 0
    if not os.path.exists(template_file):
        print("Template file: " + template_file + " is missing")
        return 0

    extension = os.path.splitext(filename)[1]

    if extension not in VALID_EXTENSIONS:
        print(caller + " needs a car config database spreadsheet with formats:")
        for ext in VALID_EXTENSIONS:
            if ext == VALID_EXTENSIONS[-1]:
                print ("or " + ext)
            else:
                print (ext)
        print ("")
        return 0

    rows = None

    ## Handle Excel <2010 files (xls)
    if extension == ".xls":
        try:
            import xlrd
        except ImportError:
            PrintToTerminal.printc('error', 'Please install pythonpackage xlrd (e.g. apt-get install python-xlrd) to use XLS-files')
            sys.exit(1)
        with xlrd.open_workbook(filename) as wb:
            sheet = wb.sheet_by_index(0)
            rows = []
            for rowno in range(sheet.nrows):
                rows.append(sheet.row_values(rowno))

    ## Handle Excel >=2010 files (xlsx, xlsm)
    if extension == ".xlsx" or extension == ".xlsm":
        if extension == ".xlsm":
            print ("Warning, " + extension + " has not been tested")
        try:
            import openpyxl
        except ImportError:
            PrintToTerminal.printc('error', 'Please install pythonpackage openpyxl (e.g. apt-get install python-openpyxl) to use XLSX-files')
            sys.exit(1)
        wb = openpyxl.load_workbook(filename)
        sheet = wb.get_active_sheet()
        rows = []
        for row in sheet.rows:
            newrow = []
            for cell in row:
                if cell.value == None:
                    newrow.append('')
                else:
                    try:
                        newrow.append(str(cell.value))
                    except UnicodeEncodeError:
                        newrow.append(str(fixUnicode(cell.value)))
            rows.append(newrow)

    ## Handle comma-separated files (csv)
    if extension == ".csv":
        import csv
        csvfile = open(filename, 'r')
        rows = csv.reader(csvfile)

    ## Handle Open Office Spreadsheets (ods)
    if extension == ".ods":
        try:
            import pyexcel
        except ImportError:
            PrintToTerminal.printc('error', 'Please install pythonpackage pyexcel (e.g. sudo -E pip install pyexcel) to use ODS-files')
            sys.exit(1)
        try:
            import pyexcel.ext.ods
        except ImportError:
            PrintToTerminal.printc('error', 'Error: Please install pythonpackage pyexcel-ods (e.g. sudo -E pip install pyexcel-ods) to use ODS-files')
            sys.exit(1)
        sheet = pyexcel.get_sheet(file_name = filename)
        rows = sheet.rows()

    ## Simple check of aquired data
    if rows == None:
        print ("ERROR: Unsupported extension of " + filename)
        print (" please use a supported format!")
        return 1

    ## Parse the parameter list
    for row in rows:
        if len(row[0]) > 0:
             if row[0].isdigit():
                rawName = row[1]
                nameWords = rawName.split()
                if len(nameWords) > 1:
                    name = ''
                    for nameWord in nameWords:
                        name = name + nameWord.capitalize()
                else:
                    name = rawName.capitalize()
                paramlist.append(CC_Param(int(row[0]), name, row[2]))
                paramlist[-1].addValue(row[3], row[4].strip())
        elif len(row[3]) > 0:
            paramlist[-1].addValue(row[3], row[4].strip())

    ##Fix strings so they will be accepted as code:
    remove = '[]'
    for param in paramlist:
        for letter in param.name:
            if string.ascii_letters.find(letter) == -1:
                if string.digits.find(letter) == -1:
                    if letter != '_':
                        if remove.find(letter) == -1:
                            if letter == '-':
                                letter = '\-'
                            remove = remove[0:-1] + letter + remove[-1]
        if remove != '[]':
            param.name = re.sub(remove, '', param.name)
        for value in param.values:
            for letter in value.desc:
                if string.ascii_letters.find(letter) == -1:
                    if string.digits.find(letter) == -1:
                        if letter != '_':
                            if remove.find(letter) == -1:
                                if letter == '-':
                                    letter = '\-'
                                remove = remove[0:-1] + letter + remove[-1]
            if remove != '[]':
                value.desc = re.sub(remove, '_', value.desc)
            value.desc = re.sub('_+', '_', value.desc)
            if string.digits.find(value.desc[0]) > -1:
                value.desc = DIGITS[value.desc[0]] + value.desc[1:]

    with open(output_file, 'w') as file:
        with open(template_file, 'r') as template_f:
            template_contents = template_f.read()
        template = jinja2.Template(template_contents)
        file.write(
            template.render(
                filename=filename,
                paramlist=paramlist
                ))

## MAIN with some error checking.
if __name__ == '__main__':
    #use docopt if installed.
    if 'docopt' in sys.modules:
        arguments = docopt.docopt(__doc__, version=PARSER_VERSION)
        if not os.path.exists(arguments['CCDB']) or not os.path.exists(arguments['TEMPLATE']):
            print (__doc__)
            sys.exit(0)
    else:
        #very simple docopt replacement, does not handle all combinations of options.
        if len(sys.argv) < 2 or len(sys.argv) > 6:
            print (__doc__)
            sys.exit(0)
        if sys.argv[1] == '-h' or sys.argv[1] == '--help':
            print (__doc__)
            sys.exit(0)
        if sys.argv[1] == '--version':
            print (PARSER_VERSION)
            sys.exit(0)
        if not os.path.exists(sys.argv[-2]) or not os.path.exists(sys.argv[-3]):
            print (__doc__)
            sys.exit(0)

        arguments = {'CCDB':sys.argv[-3], 'TEMPLATE':sys.argv[-2], 'OUTPUTFILE':sys.argv[-1]}

    sys.exit(parse_ccdb(arguments['CCDB'], arguments['TEMPLATE'], arguments['OUTPUTFILE'], sys.argv[0]))
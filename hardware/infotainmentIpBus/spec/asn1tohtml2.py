# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys

if len(sys.argv) != 2:
    print('Usage: asn1tohtml2 asn1file')
    exit(0)

filename = sys.argv[1]


'''
Scan file for lines like:
ATypeName ::= ....
and put ATypeName in a set
'''
all_types = set()
with open(filename, 'r') as lines:
    for line in lines:
        if line.find(' ::= ') != -1:
            splitted = line.split()
            all_types.add(splitted[0])


'''
Scan text again and replace:

ATypeName ::=
 with
<a name=ATypeName>ATypeName</a>

and

xyz  ATypeName
 with
xyz  <a href="#ATypeName">ATypeName</a>
'''
with open(filename, 'r') as lines:
    full_text = '<!DOCTYPE html><html><body><pre>'
    for line in lines:
        if line.find(' ::= ') != -1:
            splitted = line.split()
            name = '<a name={0}>{0}</a>'.format(splitted[0])
            line = line.replace(splitted[0], name)
        else:
            for t in all_types:
                t1 = ' ' + t + ','
                t2 = ' ' + t + ' '
                if line.find(t1) != -1:
                    href = ' <a href="#{0}">{0}</a>,'.format(t)
                    line = line.replace(t1, href)
                elif line.find(t2) != -1:
                    href = ' <a href="#{0}">{0}</a> '.format(t)
                    line = line.replace(t2, href)
        full_text = full_text + line
    full_text = full_text + '</pre></body></html>'
    html_file = open(filename.split('.')[0] + '.html', 'w')
    html_file.write(full_text)
    html_file.close()

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import typing
#########################################
# This file contains global hashmaps
#########################################


# referenced path = [ refering objects ]
references = dict()  # type: typing.Dict[typing.Any, typing.Any]

def addReference(ref, obj):
    '''
       Add Reference to global references map.

       :param ref: AUTOSAR Reference path
       :param obj: Reference to ElementTree.Element() containing reference

       :return: Nothing
    '''
    try:
        references[ref].append(obj)
    except KeyError:
        references[ref] = [obj]


def addReferenceList(li):
    '''
       Add a list of reference elements.

       :param li: List containing ElementTree.Element() objects containing references

       :return: Nothing
    '''
    for l in li:
        addReference(l.text, l)



def updateReference(oldref, newref):
    '''
       Update all objecte with references to *oldref* to point to *newref*.

       :param oldref: Old reference path (AUTOSAR Style)
       :param newref: New reference path (AUTOSAR Style)
    '''
    try:
        for r in references[oldref]:
            r.text = newref

        if newref != '':
            references[newref] = references[oldref]

        references.pop(oldref)
    except KeyError:
        pass

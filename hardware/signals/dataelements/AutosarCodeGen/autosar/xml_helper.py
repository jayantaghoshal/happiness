# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from . import base


def find(element, xp):
    '''
        Find child element in XML matching XPATH *xp*. Using AUTOSAR
        namespace for XML.

        :param element: Parent element of type ElementTree.Element to search
        :param xp: XPATH expression to find
        :return: Returns matching element or None.
    '''
    return element.find(base.add_schema(xp))


def find2(element, xp):
    '''
        Find child element in XML matching XPATH *xp* on **ANY** level. Using AUTOSAR
        namespace for XML.

        :param element: Parent element of type ElementTree.Element to search
        :param xp: XPATH expression to find
        :return: Returns matching element or None.
    '''
    return element.find('.//'+base.add_schema(xp))


def findall2(element, xp):
    '''
        Find all child elements in XML matching XPATH *xp*. Using
        AUTOSAR namespace for XML.

        :param element: Parent element of type ElementTree.Element to search
        :param xp: XPATH expression to find
        :return: Returns list of matching elements or empty list
    '''
    return element.findall(base.add_schema(xp))


def findall(element, xp):
    '''
        Find all child elements in XML matching XPATH *xp* on **ANY** level.
        Using AUTOSAR namespace for XML.

        :param element: Parent element of type ElementTree.Element to search
        :param xp: XPATH expression to find
        :return: Returns list of matching elements or empty list
    '''
    return element.findall('.//'+base.add_schema(xp))


def find_path(element, parent_dict):
    '''
        Resolve the Autosar path to the current element.

        :param element: Any element of type ElementTree.Element
        :param parent_dict: Dictionary of parent elements
        :return: Returns Autosar path of element
    '''
    my_name_elem = find(element, 'SHORT-NAME')
    if my_name_elem is not None:
        my_name = '/' + my_name_elem.text
    else:
        my_name = ''
    if element in parent_dict:
        my_parent_path = find_path(parent_dict[element], parent_dict)
    else:
        my_parent_path = ''
    my_path = my_parent_path + my_name
    return my_path


def find_path2(element, parent_dict, tag):
    '''
        Resolve the Autosar path to the current element.

        :param element: Any element of type ElementTree.Element
        :param parent_dict: Dictionary of parent elements
        :return: Returns Autosar path of element
    '''
    my_name_elem = find(element, tag)
    if my_name_elem is not None:
        my_name = my_name_elem.text.split('/')[-1]
        return my_name
    else:
        return None


def getiterator(element, tag=None, ns=None):
    '''
        Return iterator of xml elements with namespace

        :param element: Element to create iterator from
        :param tag: xml-tag on subelements to iterate over
        :param ns: Namespace to use. NOTE: Currently not used
        :return: iterator on found elements
    '''
    if not ns:
        return element.iter(base.add_schema(tag))
    else:
        return element.iter(tag)


def indent(elem, level=0):
    '''
        Properly indent the xml text for serialization.

        :param elem: XML Element to start indenting from
        :param level: Indentation level this element is at
        :return: Nothing
    '''
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i
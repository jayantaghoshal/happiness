######################################
# Base classes for AUTOSAR library
#
# Copyright (c) Volvo Cars AB
######################################

import string
import copy
from . import maps
from . import options
from xml.etree import ElementTree as ET

schema ="{http://autosar.org/schema/r4.0}"

#######################################
# Auxilliary functions

def GetShortName(element):
	''' 
	    This function searches for sub xml-node SHORT-NAME and returns its value.

	    :param element:   Element to find shorname of 
	    :return: String representing element shortname
	'''
	# MATS
	shortname = "NoneType"
	if element is not None:
		shortname = element.findtext("%sSHORT-NAME"%(schema))
	# Fallback for elements created without namespace
		if shortname is None:
			shortname = element.findtext('SHORT-NAME')
	return shortname


def getiterator(element, tag=None):
	'''
	   Return iterator of xml elements.

	   :param element: Element to create iterator from 
	   :param tag: xml-tag on subelements to iterate over
	   :return: iterator on found elements
	'''
	return element.getiterator(tag=add_schema(tag))

def add_schema(xp):
	''' 
	   Add AUTOSAR schema prefix to XPATH expression 

	   :param xp: XPATH to add prefix to
	   :return: Prefixed XPATH expression
	'''
	num = xp.count('/') + 1
	paths = xp.split('/')
	for i in range(len(paths)):
		if paths[i] == '' or paths[i] == '*' or paths[i] == '.':
			paths[i] = paths[i]
			num = num -1
		else:
			paths[i] = '%s' + paths[i]
	new_xp = "/".join(paths)
	ns_tup = (schema,) * num
	return new_xp % ns_tup



def create_element(element, value=None, attrib=None):
	''' 
	   Create arbitrary AUTOSAR XML element 
	   
	   :param element: New element name
	   :param value: New element value
	   :param attrib: Attribute of new element
	   :return: Newly create element in ARObject class.
	'''
	if attrib is not None:
		new_elem = ET.Element('{http://autosar.org/schema/r4.0}%s'%(element), attrib)
	else:
		new_elem = ET.Element('{http://autosar.org/schema/r4.0}%s'%(element))

	if value is not None:
		new_elem.text = value

	return ARObject(new_elem)

#######################################
# Object Classes
#######################################
class ARObject:
	'''
	   ARObject is a generic Base class used by all other objects representing
	   Autosar elements.
	'''
	def __init__(self, element,path=None,fileref=None):
		'''
		   Constructor that creates new Autosar object from ElementTree Element

		   :param element: ElementTree element to create object from
		   :param path: Autosar element path (Not the same as XPATH)
		   :param fileref: ARXML fileobject reference
		'''
		self.xmlref = element
		self.shortname = GetShortName(element)
		self.path = path
		self.arxml = fileref

	def setShortname(self,shortname):
		'''
		   Sets the shortname of the element. 
		   This will update the <SHORT-NAME></SHORT-NAME> xml.

		   :param shortname: ShortName to set for the object
		'''
		elem = self.xmlref.find(add_schema('SHORT-NAME'))

		if elem is not None:
			elem.text = shortname


	def getSubElem(self, xp):
		'''
		   Get generic subElement of this ARObject.

		   NOTE: This works on ElementTree xml objects. XPATH query
		   fetched relative this ARObject

		   :param xp: XPATH to query for.

		   :return: ElementTree.Element() Object
		'''
		return self.xmlref.find(add_schema(xp))

	def getSubElem2(self, xp):
		return self.xmlref.find('.//'+add_schema(xp))


	def getAllSubElems(self, xp):
		'''
		   Get generic subElements of this ARObject.

		   Work in same way as *getSubElem* but uses findall instead of find.

		   :param xp: XPATH to query for

		   :return: ElementTree.Element() Object
		'''
		return self.xmlref.findall(add_schema(xp))

	def getAllSubElems2(self,xp):
		return self.xmlref.findall('.//'+add_schema(xp))

	def removeElement(self, element = None):
		'''
			Remove a ARObject as child to this object.
			: param elemen: Any Autosar Element or path, remove self if None
			: return: Reference to the element to remove
		''' 
		
		if not element:
			# Remove self
			d = self.arxml.class_map[self.__class__]
			del(d[self.path])
			self.arxml.parents[self.xmlref].remove(self.xmlref)

		else:
			# Remove child object
			if type(element) is str:
				print('Removing element %s'%(element))
				elementpath = element
			else:
				elementpath = element.path
				
			# Check if element exists
			elem_found = None
			for k,c in self.arxml.class_map:
				if elementpath in c:
					elem_found = c[elementpath]
					del(c[elementpath])

					# Get object parent
					self.arxml.parents[elem_found.xmlref].remove(elem_found.xmlref)

					# Remove all references to object
					maps.updateReference(elementpath, '')

					return self

			return None


			

	def addElement(self, element, UpdateReferences = True):
		'''
		   Add a new ARObject as child to this object.

		   :param element: Any Autosar Element

		   :return: Reference to added element
		'''

		# Check if element already exists
		elem_found = False
		for k,c in self.arxml.class_map.iteritems():
			#if element.path in c:
			if '%s/%s'%(self.path, element.shortname) in c:
				elem_found = True

		if elem_found:
			if options.verbose:
				print('Warning: Element cannot be added %s, already exists'%(element.path))
			return None

		elements = self.xmlref.find(add_schema('ELEMENTS'))
		if elements is None:
			elements = self.xmlref.find('ELEMENTS')
			if elements is None:
				elements = ET.SubElement(self.xmlref,'{http://autosar.org/schema/r4.0}ELEMENTS')

		elem_shortname = element.shortname
		if elem_shortname is not None:
			old_path = element.path
			element.path = self.path + '/' + elem_shortname
			elements.append(element.xmlref)
			
			if UpdateReferences:
				# Update references
				if old_path in maps.references and old_path != element.path:
					if options.verbose:
						print("Updating %s -> %s"%(old_path, element.path))
					maps.updateReference(old_path, element.path)
					#for r in maps.references[old_path]:
					#	r.text = element.path
	
				element.arxml = self.arxml
	
				self.arxml.updateReferences(element,old_path)
	
				# Update element children paths
				element.updateReferences(old_path)


		return element

	def addPropertyElement(self, element, position=None):
		'''
		   Add a ARObject as child element.

		   :param element: Any Autosar object
		   :param position: Insert at specific child position (default not used)

		   :return: Added element
		'''
		if position is None:
			self.xmlref.append(element.xmlref)
		else:
			self.xmlref.insert(position,element.xmlref)

		return element

	def updateReferences(self, old_path):
		''' Not Implemented '''
		pass

	def moveElement(self, new_parent):
		''' Not Implemented '''
		pass


class ARPackage(ARObject):
	''' 
	   This class represent any AR-PACKAGE element in AUTOSAR xml.

	'''

	def __repr__(self):
		return "%s (%s)"%(self.shortname, self.path)


	def addNewPackage(self, package_name):
		'''
		   Creates a new ARPackage and adds it as a subpackage.

		   :param package_name: SHORT-NAME of the AR-PACKAGE
		   :return: Returns a reference to the new ARPackage instance
		'''
		packages = self.xmlref.find(add_schema('AR-PACKAGES'))
		if packages is None:
			packages = self.xmlref.find('AR-PACKAGES')
			if packages is None:
				packages = ET.SubElement(self.xmlref,'{http://autosar.org/schema/r4.0}AR-PACKAGES')

		new_package = ET.SubElement(packages,'{http://autosar.org/schema/r4.0}AR-PACKAGE')
		new_package_name = ET.SubElement(new_package, '{http://autosar.org/schema/r4.0}SHORT-NAME')
		new_package_name.text = package_name
		if self.path is not None:
			new_package_path = '/'.join([self.path,package_name])
		else:
			new_package_path = None
		p = ARPackage(new_package,new_package_path,self.arxml)
		p.arxml = self.arxml
		self.arxml.packages[new_package_path] = p

		return p

	def addPackage(self, ar_package):
		'''
		   Add an existing ARPackage as a subpackage.

		   :param ar_package: ARPackage to add
		   :return: Reference to new package if successfully added
		'''
		packages = self.xmlref.find(add_schema('AR-PACKAGES'))
		if packages is None:
			packages = self.xmlref.find('AR-PACKAGES')
			if packages is None:
				packages = ET.SubElement(self.xmlref,'{http://autosar.org/schema/r4.0}AR-PACKAGES')

		new_package_path = '/'.join([self.path,ar_package.shortname])
		if new_package_path not in self.arxml.packages:
			packages.append(ar_package.xmlref)
			new_package = ar_package
			old_path = ar_package.path
			if old_path in self.arxml.packages:
				del(self.arxml.packages[old_path])
			new_package.path = new_package_path
			self.arxml.packages[new_package_path] = new_package
			new_package.arxml = self.arxml
		else:
			new_package = self.arxml.packages.get(new_package_path)
		return new_package

	def getElements(self):
		elements = self.xmlref.find(add_schema('ELEMENTS'))

	def mergePackage(self, package):
		'''
		   Merge *package* into this package.
		   :param package: ARPackage to merge into this ARPackage

		   :return: Return updated package
		'''
		elements = self.xmlref.find(add_schema('ELEMENTS'))

		elements2 = package.xmlref.find(add_schema('ELEMENTS'))

		if elements2 is not None:

			for e in elements2:
				print("Merging element %s"%(e))
				elements.append(e)

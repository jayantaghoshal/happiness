#######################################
# AUTOSAR ARXML File handling
#
#######################################

'''
The ARXML module handles all file loading/parsing/saving of AUTOSAR ARXML files.
'''


import os.path as path
import sys
from xml.etree import cElementTree as ET

from .xml_helper import find, findall, find_path, find_path2, getiterator, indent
from . import base
from . import components
from . import maps
from . import options

arxml_files = dict()


def printProgress(message = None):
	'''
		Local function used to print progressbar.

		:param message: Print additional message and start newline
		:return: Nothing
	'''
	if not options.silent:
		if message is None:
			#print '.',
			sys.stdout.write('.')
		else:
			sys.stdout.write('%s'%(message))
			#print '.'
			#print '%s.'%(message),
		sys.stdout.flush()

def printProgressDone():
	'''
		Local function to finish progressbar printout
		:return: Nothing
	'''
	sys.stdout.write('\n')
	sys.stdout.flush()


def load(filename):
	'''
		Load Autosar XML file.

		:param filename: Autosar XML file to load
		:return: ARXml class object with newly opened file
	'''
	printProgress("Loading %s.."%(filename))
	try:
		return arxml_files[filename]
	except:
		arxml_files[filename] = ARXml(filename)
		return arxml_files[filename]

def create(filename, replace=False):
	'''
		Create new empty Autosar XML file.

		:param filename: Default filename to store file
		:param replace: True if ouput shall be replaced
		:return: New ARXml class else None if error
	'''
	printProgress("Creating new file %s.."%(filename))
	if not path.exists(filename) and not filename in arxml_files or replace:
		arxml_files[filename] = ARXml()
		arxml_files[filename].filename = filename
		printProgressDone()
		return arxml_files[filename]
	else:
		printProgressDone()
		print("ERROR: file %s already exists"%(filename))
		return None


def save(filename, saveas=None):
	'''
		Save ARXml class to disk. 

		:param filename: ARXml file to save
		:param saveas: Optional new filename to save as
		:return: Nothing
	'''
	arxml_files[filename].save(saveas)
		


class ARXml:
	'''
		This class instantiates one AUTOSAR XML file and handles loading and saving files
	'''
	def __init__(self, filename=None):
		if filename is not None:
			self.xml = ET.parse(filename)
			self.filename = filename
		else:
			self.xml = ET.ElementTree(ET.Element('{http://autosar.org/schema/r4.0}AUTOSAR'))
			self.filename = None

		self.parents = dict()
		self.packages = dict()
		self.compositions = dict()
		self.port_interfaces = dict()
		self.swcs = dict()
		self.impl_mappings = dict()
		self.swc_impls = dict()
		self.system = dict()
		self.datatypes = dict()
		self.recordDatatypes = dict()
		self.recordElements = dict()
		self.impl_datatypes = dict()
		self.constants = dict()
		self.data_constr = dict()
		self.swbasetype = dict()
		self.units = dict()
		self.phys_dimension = dict()
		self.compu_methods = dict()
		self.isignal = dict()
		self.syssignal = dict()
		self.syssignalgroup = dict()
		self.endtoendprotection = dict()
		self.groupmapping = dict()

		self.reload()


		self.class_map = { base.ARPackage: self.packages,
				 components.ARComposition: self.compositions,
				 components.ARSwc: self.swcs,
				 components.ARSwcImplementation: self.swc_impls,
				 components.ARPortInterface: self.port_interfaces,
				 components.ARSystem: self.system,
				 components.ARConstant: self.constants,
				 components.ARDatatype: self.datatypes,
				 components.ARArrayDatatype: self.datatypes,
				 components.ARRecordDatatype: self.recordDatatypes,
				 components.ARRecordElement: self.recordElements,
				 components.ARImplDatatype: self.impl_datatypes,
				 components.ARDataConstraint: self.data_constr,
				 components.ARSwBaseType: self.swbasetype,
				 components.ARUnit: self.units,
				 components.ARPhysDimension: self.phys_dimension,
				 components.ARCompuMethod: self.compu_methods,
				 components.ARIsignal: self.isignal,
				 components.ARSysSignal: self.syssignal,
				 components.ARIsignalGroup: self.syssignalgroup,
				 components.AREndToEndProtection: self.endtoendprotection,
				 components.ARGroupMap: self.groupmapping}
		
	
	def reload(self):
		'''
		Loads/Reloads all AUTOSAR elements found in the file
		'''
		self.parents = get_parent_dict(self.xml)
		printProgress()
		self.packages = get_package_dict(self.xml.getroot(),self.parents,self)
		printProgress()
		self.constants.update(get_elem_dict(self.xml.getroot(), self.parents, 'CONSTANT-SPECIFICATION', components.ARConstant, self))
		printProgress()
		self.compositions = get_elem_dict(self.xml.getroot(), self.parents, 'COMPOSITION-SW-COMPONENT-TYPE', components.ARComposition,self)
		printProgress()
		self.port_interfaces = get_elem_dict(self.xml.getroot(), self.parents, 'SENDER-RECEIVER-INTERFACE', components.ARPortInterface,self)
		printProgress()
		self.port_interfaces.update(get_elem_dict(self.xml.getroot(), self.parents, 'CLIENT-SERVER-INTERFACE', components.ARPortInterface, self))
		printProgress()
		self.swcs = get_elem_dict(self.xml.getroot(), self.parents, 'APPLICATION-SW-COMPONENT-TYPE', components.ARSwc,self)
		self.swcs.update(get_elem_dict(self.xml.getroot(), self.parents, 'COMPOSITION-SW-COMPONENT-TYPE', components.ARSwc,self))
		self.swcs.update(get_elem_dict(self.xml.getroot(), self.parents, 'SENSOR-ACTUATOR-SW-COMPONENT-TYPE', components.ARSwc,self))
		printProgress()

		(self.swc_impls,self.impl_mappings) = get_swc_to_impl_mapping(self.xml.getroot(), self.parents,self)
		printProgress()
		self.system = get_elem_dict(self.xml.getroot(), self.parents, 'SYSTEM', components.ARSystem,self)
		self.datatypes = dict()
		self.datatypes.update(get_elem_dict(self.xml.getroot(), self.parents, 'APPLICATION-PRIMITIVE-DATA-TYPE', components.ARDatatype, self))
		self.datatypes.update(get_elem_dict(self.xml.getroot(), self.parents, 'APPLICATION-RECORD-DATA-TYPE', components.ARRecordDatatype, self))
		self.datatypes.update(get_elem_dict(self.xml.getroot(), self.parents, 'APPLICATION-ARRAY-DATA-TYPE', components.ARArrayDatatype, self))
		self.recordElements = dict()
		self.recordElements.update(get_elem_dict(self.xml.getroot(), self.parents, 'APPLICATION-RECORD-ELEMENT', components.ARRecordElement, self))
		self.impl_datatypes.update(get_elem_dict(self.xml.getroot(), self.parents, 'IMPLEMENTATION-DATA-TYPE', components.ARImplDatatype, self))
		#self.constants.update(get_elem_dict(self.xml.getroot(), self.parents, 'CONSTANT-SPECIFICATION', components.ARConstant, self))
		self.data_constr.update(get_elem_dict(self.xml.getroot(), self.parents, 'DATA-CONSTR', components.ARDataConstraint, self))
		self.swbasetype.update(get_elem_dict(self.xml.getroot(), self.parents, 'SW-BASE-TYPE', components.ARSwBaseType, self))
		self.units.update(get_elem_dict(self.xml.getroot(), self.parents, 'UNIT', components.ARUnit, self))
		self.phys_dimension.update(get_elem_dict(self.xml.getroot(), self.parents, 'PHYSICAL-DIMENSION', components.ARPhysDimension, self))
		self.compu_methods.update(get_elem_dict(self.xml.getroot(), self.parents, 'COMPU-METHOD', components.ARCompuMethod, self))	
		self.isignal = dict()
		self.isignal.update(get_elem_dict(self.xml.getroot(), self.parents, 'I-SIGNAL', components.ARIsignal, self))	
		self.syssignal = dict()
		self.syssignal.update(get_elem_dict(self.xml.getroot(), self.parents, 'SYSTEM-SIGNAL', components.ARSysSignal, self))
		#self.ecu_instance = get_elem_dict(self.xml.getroot(), self.parents, 'ECU-INSTANCE', base.ARObject)
		self.isignalgroup = dict()
		self.isignalgroup.update(get_elem_dict(self.xml.getroot(), self.parents, 'I-SIGNAL-GROUP', components.ARIsignalGroup, self))
		self.endtoendprotection = get_elem_dict(self.xml.getroot(), self.parents, 'END-TO-END-PROTECTION', components.AREndToEndProtection, self)
		
		self.sigtrig = dict()
		self.sigtrig.update(get_elem_dict(self.xml.getroot(), self.parents, 'I-SIGNAL-TRIGGERING', components.ARSigTrig, self))
		
		self.groupmapping = get_elem_dict2(self.xml.getroot(), self.parents, 'SENDER-RECEIVER-TO-SIGNAL-GROUP-MAPPING', 'DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF', components.ARGroupMap,self)
		self.groupmapping.update(get_elem_dict2(self.xml.getroot(), self.parents, 'SENDER-RECEIVER-TO-SIGNAL-MAPPING', 'DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF', components.ARGroupMap,self))
		printProgress()


		
		get_timings(self.xml.getroot(), self.parents)
		get_end_to_end(self.xml.getroot(), self.parents)

		self.class_map = { base.ARPackage: self.packages,
				 components.ARComposition: self.compositions,
				 components.ARSwc: self.swcs,
				 components.ARSwcImplementation: self.swc_impls,
				 components.ARPortInterface: self.port_interfaces,
				 components.ARSystem: self.system,
				 components.ARConstant: self.constants,
				 components.ARDatatype: self.datatypes,
				 components.ARArrayDatatype: self.datatypes,
				 components.ARRecordDatatype: self.recordDatatypes,
				 components.ARRecordElement: self.recordElements,
				 components.ARImplDatatype: self.impl_datatypes, 
				 components.ARDataConstraint: self.data_constr,
				 components.ARSwBaseType: self.swbasetype,
				 components.ARUnit: self.units,
				 components.ARPhysDimension: self.phys_dimension,
				 components.ARCompuMethod: self.compu_methods,
				 components.ARSysSignal: self.syssignal,
				 components.ARIsignal: self.isignal,
				 components.ARIsignalGroup: self.isignalgroup,
				 components.AREndToEndProtection: self.endtoendprotection,
				 components.ARGroupMap: self.groupmapping}

		printProgressDone()

	def reloadPath(self, path_in):
		path = self.packages[path_in].xmlref
		self.parents = get_parent_dict(self.xml)
		printProgress()
		self.packages.update(get_package_dict(path,self.parents,self))
		printProgress()
		self.constants.update(get_elem_dict(path, self.parents, 'CONSTANT-SPECIFICATION', components.ARConstant, self))
		printProgress()
		self.compositions.update(get_elem_dict(path, self.parents, 'COMPOSITION-SW-COMPONENT-TYPE', components.ARComposition,self))
		printProgress()
		self.port_interfaces.update(get_elem_dict(path, self.parents, 'SENDER-RECEIVER-INTERFACE', components.ARPortInterface,self))
		printProgress()
		self.port_interfaces.update(get_elem_dict(path, self.parents, 'CLIENT-SERVER-INTERFACE', components.ARPortInterface, self))
		printProgress()
		self.swcs.update(get_elem_dict(path, self.parents, 'APPLICATION-SW-COMPONENT-TYPE', components.ARSwc,self))
		self.swcs.update(get_elem_dict(path, self.parents, 'SENSOR-ACTUATOR-SW-COMPONENT-TYPE', components.ARSwc,self))
		printProgress()

		(self.swc_impls,self.impl_mappings) = get_swc_to_impl_mapping(path, self.parents,self)
		printProgress()
		self.system = get_elem_dict(path, self.parents, 'SYSTEM', components.ARSystem,self)
		self.datatypes = dict()
		self.datatypes.update(get_elem_dict(path, self.parents, 'APPLICATION-PRIMITIVE-DATA-TYPE', components.ARDatatype, self))
		self.datatypes.update(get_elem_dict(path, self.parents, 'APPLICATION-RECORD-DATA-TYPE', components.ARRecordDatatype, self))
		self.datatypes.update(get_elem_dict(path, self.parents, 'APPLICATION-ARRAY-DATA-TYPE', components.ARArrayDatatype, self))
		self.recordElements = dict()
		self.recordElements.update(get_elem_dict(path, self.parents, 'APPLICATION-RECORD-ELEMENT', components.ARRecordElement, self))
		self.impl_datatypes.update(get_elem_dict(self.xml.getroot(), self.parents, 'IMPLEMENTATION-DATA-TYPE', components.ARImplDatatype, self))
		self.constants.update(get_elem_dict(path, self.parents, 'CONSTANT-SPECIFICATION', components.ARConstant, self))
		self.data_constr.update(get_elem_dict(path, self.parents, 'DATA-CONSTR', components.ARDataConstraint, self))
		self.swbasetype.update(get_elem_dict(path, self.parents, 'SW-BASE-TYPE', components.ARSwBaseType, self))
		self.units.update(get_elem_dict(path, self.parents, 'UNIT', components.ARUnit, self))
		self.phys_dimension.update(get_elem_dict(path, self.parents, 'PHYSICAL-DIMENSION', components.ARPhysDimension, self))
		self.compu_methods.update(get_elem_dict(path, self.parents, 'COMPU-METHOD', components.ARCompuMethod, self))
		self.isignal = dict()
		self.isignal.update(get_elem_dict(path, self.parents, 'I-SIGNAL', components.ARIsignal, self))	
		self.syssignal = dict()
		self.syssignal.update(get_elem_dict(path, self.parents, 'SYSTEM-SIGNAL', components.ARSysSignal, self))
		self.isignalgroup = dict()
		self.isignalgroup.update(get_elem_dict(path, self.parents, 'I-SIGNAL-Group', components.ARIsignalGroup, self))	
		self.endtoendprotection.update(get_elem_dict(path, self.parents, 'END-TO-END-PROTECTION', components.AREndToEndProtection, self))
		
		self.groupmapping = get_elem_dict2(self.xml.getroot(), self.parents, 'SENDER-RECEIVER-TO-SIGNAL-GROUP-MAPPING', 'DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF', components.ARGroupMap,self)
		self.groupmapping.update(get_elem_dict2(self.xml.getroot(), self.parents, 'SENDER-RECEIVER-TO-SIGNAL-MAPPING', 'DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF', components.ARGroupMap,self))

		printProgress()
		
		#self.ecu_instance = get_elem_dict(self.xml.getroot(), self.parents, 'ECU-INSTANCE', base.ARObject)

		get_timings(path, self.parents)
		get_end_to_end(path, self.parents)

		self.class_map = { base.ARPackage: self.packages,
				 components.ARComposition: self.compositions,
				 components.ARSwc: self.swcs,
				 components.ARSwcImplementation: self.swc_impls,
				 components.ARPortInterface: self.port_interfaces,
				 components.ARSystem: self.system,
				 components.ARConstant: self.constants,
				 components.ARDatatype: self.datatypes,
				 components.ARArrayDatatype: self.datatypes,
				 components.ARRecordDatatype: self.recordDatatypes,
				 components.ARRecordElement: self.recordElements,
				 components.ARImplDatatype: self.impl_datatypes, 
				 components.ARDataConstraint: self.data_constr,
				 components.ARSwBaseType: self.swbasetype,
				 components.ARUnit: self.units,
				 components.ARPhysDimension: self.phys_dimension,
				 components.ARCompuMethod: self.compu_methods,
				 components.ARSysSignal: self.syssignal,
				 components.ARIsignal: self.isignal,
				 components.AREndToEndProtection: self.endtoendprotection,
				 components.ARGroupMap: self.groupmapping}		

		printProgressDone()





	def addPackage(self, packagename):
		'''
			Add new ARPackage to ARXml with SHORT-NAME *packagename*.

			:param packagename: Name of package to create

			:return: New ARPackage object
		'''

		merge_package = False
		# Check if package already exists
		if type(packagename) is not str:
			merge_package = True
			name = '/%s'%(packagename.shortname)
		else:
			name = packagename

			if name[0] != '/':
				name = '/%s'%(name)

		if name in self.packages:
			if merge_package:
				print('INFO: Merging package %s'%(name))
				self.packages[name].mergePackage(packagename)

			return self.packages[name]
		else:
			root = self.xml.getroot()
			arpackages = root.find(base.add_schema('AR-PACKAGES'))
			if arpackages is None:
				arpackages = root.find('AR-PACKAGES')
				if arpackages is None:
					arpackages = ET.SubElement(root,'{http://autosar.org/schema/r4.0}AR-PACKAGES')

			if type(packagename) is not str:
				arpackages.append(packagename.xmlref)
				package_path = '/' + packagename.shortname
				self.packages[package_path] = packagename
				packagename.package_path = package_path
				p = packagename
			else:
				package = ET.SubElement(arpackages,'{http://autosar.org/schema/r4.0}AR-PACKAGE')
				package_name = ET.SubElement(package, '{http://autosar.org/schema/r4.0}SHORT-NAME')
				package_name.text = packagename
				new_path = '/'+packagename
				p = base.ARPackage(package, new_path,self)
				self.packages[new_path] = p

			p.arxml = self
			return p

	def addPackagePath(self, packagepath):
		'''
			Add new ARPackages recursively to create the path on input.

			:param packagepath: Path with packages to create

			:return: Last ARPackage of path
		'''
		
		pkgs = packagepath.path.split('/')[1:-1]

		pkg = self

		for p in pkgs:
			pkg = pkg.addPackage(p)


		p = pkg.addPackage(packagepath)

		return p



	def updateReferences(self, elem, oldpath):
		'''
			Update references to AUTOSAR objects.

			This function is called when moving or adding a new element/object.

			:param elem: Element added/moved
			:param oldpath: Previous path of element
		'''
		# Update with global references dict
		try:
			self.class_map[elem.__class__][elem.path] = elem
		except KeyError:
			pass # Generic objects that will be reparsed on a arxml.reload() call



	def getReferences(self):
		'''
			Get all references in this ARXml file.

			:Note: Currently not all references are returned, only the most frequently used ones.
		'''
		return dict(self.packages.items() + self.compositions.items() + self.port_interfaces.items() +  self.swcs.items())

	def getPackages(self):
		''' 
			Get all *ARPackage* objects in this *ARXml* instance. 
			
			Returns a list of *ARPackage* objects.
		'''
		return self.packages

	def getCompositions(self):
		''' 
			Get all composition (*ARComposition*) elements in *ARXml* instance.
			
			Returns a list of *ARComposition* objects.
		'''
		return self.compositions

	def getComposition(self, composition):
		'''
			Get composition *composition* from this *ARXml* instance.

			:param composition: is an Autosar Path to the composition.

			Returns *ARComposition* object. If not found *KeyError* is thrown.
		'''
		return self.compositions[composition]

	def getComponents(self):
		'''
			Get all *ARComponent* objects in this *ARXml* instance.

			Returns a list of *ARComponent* objects.
		'''
		return self.swcs

	def getComponent(self, component):
		'''
			Get component *component* from this *ARXml* instance.

			:param component: Autosar path to component.

			Returns *ARComponent* instance. If not found *KeyError* is thrown.
		'''
		return self.swcs[component]

	def getSwcImplementations(self):
		'''
			Get all *ARSwcImplementation* in this *ARXml* instance.

			Returns a list of *ARSwcImplementation* instances.
		'''
		return self.swc_impls

	def getSwcImplementation(self, implementation):
		'''
			Get SWC-Implementation *implementation* from this *ARXml* instance.

			:param implementation: Autosar path to SWC-Implementation
		'''
		return self.swc_impls[implementation]
	

	def getPackage(self, package):
		'''
			Get package *package* from this *ARXml* instance.

			:param package: Autosar path to package.

			Returns *ARPackage* instance for this package or *KeyError* if not found.
		'''
		return self.packages[package]

	def getSystem(self, system=None):
		'''
			Get specified system instance or first system instance if *system* is not defined.

			:param system: Optional Autosar path to system instance.

			Returns *ARSystem* instance. Throws *KeyError* exception if not found.
		'''
		if not system:
			return self.system[self.system.keys()[0]]
		else:
			return self.system[system]

	def save(self, filename=None, comment=None):
		'''
			Save ARXml instance to file. If *ARXml* instance is loaded from file it 
			will save back  to the same file by default, unless *filename* is defined.
			If *ARXml* file is created new it will save to the *filename* specified there.

			Optionally add a *comment* to the end of the file, used to indicate patches.

			:param filename: Optional filename to "save file as"

			:param comment: Optional comment added at the end of the file.
		'''
		if comment:
			self.xml.getroot().append(ET.Comment(comment))
		indent(self.xml.getroot())
		if filename is None:
			printProgress("Writing file %s.."%(self.filename))
			self.xml.write(self.filename,encoding="utf-8", xml_declaration=True)
			printProgressDone()
		else:
			printProgress("Writing file %s.."%(filename))
			self.xml.write(filename,encoding="utf-8", xml_declaration=True)
			printProgressDone()


	def find(self, xp):
		return self.xml.getroot().find(base.add_schema(xp))

	def findall(self, xp):
		return self.xml.getroot().findall(base.add_schema(xp))

	def findall2(self, xp):
		return self.xml.getroot().findall('.//'+base.add_schema(xp))


##############################################
# Global auxiliary functions
#

def get_parent_dict(tree):
	'''
		Auxiliary function to get parent information for every XML element.

		:param tree: ElementTree to get dictionary of parents from
		:return: dictionary of parents.
	'''
	return dict((c, p) for p in tree.getiterator() for c in p)

def get_package_dict(root, parent_dict, fileref):
	'''
		This function will create a dictionary of all packages in xml from root element.

		:param root: Root element of type ElementTree.Element
		:param parent_dict: Dictionary of parent elements
		:param fileref: *ARXml* file instance

		:return: Dictionary of all packages
	'''
	packages = getiterator(root, tag='AR-PACKAGE')
	package_dict = dict()
	root_package = base.ARPackage(root,'/',fileref)
	package_dict['/'] = root_package
	package_dict[''] = root_package
	for package in packages:
		package_name = find(package, 'SHORT-NAME').text
		package_path = find_path(package, parent_dict)
		package_dict[package_path] = base.ARPackage(package,package_path,fileref)
	return package_dict


def get_elem_dict(root, parent_dict, tag, elem_class, fileref=None):
	'''
		This function will create a dictionary of elements of class *elem_class* type.
		It will iterate the whole tree from *root* element.

		:param root: Root element of ElementTree.Element type
		:param parent_dict: Dictionary of parent elements
		:param tag: XPATH tag to search for
		:param elem_class: Autosar Element class to create from element
		:param fileref: *ARXml* file instance

		:return: Returns a dictionary of elements of type *elem_class* where key is 
				the Autosar path to the respective element.
	'''
	elems = getiterator(root, tag=tag)
	elem_dict = dict()
	for elem in elems:
		elem_name = find(elem, 'SHORT-NAME').text
		elem_path = find_path(elem, parent_dict)
		elem_dict[elem_path] = elem_class(elem, elem_path, fileref)

	return elem_dict

def get_elem_dict2(root, parent_dict, tag, subtag, elem_class, fileref=None):
	'''
		This function will create a dictionary of elements of class *elem_class* type.
		It will iterate the whole tree from *root* element.

		:param root: Root element of ElementTree.Element type
		:param parent_dict: Dictionary of parent elements
		:param tag: XPATH tag to search for
		:param elem_class: Autosar Element class to create from element
		:param fileref: *ARXml* file instance

		:return: Returns a dictionary of elements of type *elem_class* where key is 
				the Autosar path to the respective element.
	'''
	elems = getiterator(root, tag=tag)
	elem_dict = dict()
	for elem in elems:
		#elem_name = find(elem, subtag).text
		elem_path = find_path2(elem, parent_dict, subtag)
		elem_dict[elem_path] = elem_class(elem, elem_path, fileref)

	return elem_dict

def get_swc_to_impl_mapping(root, parent_dict, fileref):
	'''
		This function will lookup all SWC-IMPLEMENTATION instances and
		find which SWC is mapping towards this implementation.

		:param root: Root element of ElementTree.Element type
		:param parent_dict: Dictionary of parent elements
		:param fileref: *ARXml* file instance

		:return: This function returns a tuple with two dictionaries.
				on the form (swc_implementations, impl_swc_mapping).
	'''
	mapping = getiterator(root, tag='SWC-TO-IMPL-MAPPING')
	mapping_dict = dict()

	impls = getiterator(root, tag='SWC-IMPLEMENTATION')
	impl_dict = dict()
	for i in impls:
		i_name = find(i, 'SHORT-NAME').text
		i_path = find_path(i, parent_dict)

		#print "SwcImplementation: %s"%(i_path)
		impl_dict[i_path] = components.ARSwcImplementation(i,i_path,fileref)
	
	for m in mapping:
		impl_obj = find(m, 'COMPONENT-IMPLEMENTATION-REF')
		impl = impl_obj.text
		comps = findall(m, 'TARGET-COMPONENT-REF')

		maps.addReference(impl,impl_obj)

		for c in comps:
			try:
				mapping_dict[c.text] = impl_dict[impl]
				maps.addReference(c.text, c)
			except KeyError:
				pass

		for c in findall(m, 'CONTEXT-COMPONENT-REF'):
			maps.addReference(c.text, c)

	return (impl_dict, mapping_dict)



###########################################
# ONLY LOAD REFERENCES
#
# NOTE: Cannot move these objects


def get_timings(root, parent_dict):
	'''
		This function loads all VFB-TIMING references.

		:param root: Root element of ElementTree.Element type
		:param parent_dict: Dictionary of parent elements
		:return: Nothing
	'''
	timings = getiterator(root, tag='VFB-TIMING')

	for t in timings:
		a = findall(t, 'TARGET-COMPONENT-REF')
		b = findall(t, 'PORT-REF')
		c = findall(t, 'DATA-ELEMENT-REF')
		
		maps.addReferenceList(a)
		maps.addReferenceList(b)
		maps.addReferenceList(c)

		maps.addReferenceList(findall(t, 'COMPONENT-REF'))


def get_end_to_end(root, parent_dict):
	'''
		This function loads all END-TO-END-PROTECTION references.

		:param root: Root element of ElementTree.Element type
		:param parent_dict: Dictionary of parent elements
		:return: Nothing
	'''	
	e2e = getiterator(root, tag='END-TO-END-PROTECTION')

	for e in e2e:
		maps.addReferenceList(findall(e, 'CONTEXT-COMPONENT-REF'))
		maps.addReferenceList(findall(e, 'CONTEXT-COMPOSITION-REF'))
		maps.addReferenceList(findall(e, 'CONTEXT-PORT-REF'))
		maps.addReferenceList(findall(e, 'TARGET-DATA-PROTOTYPE-REF'))






#############################################
# AUXILLARY XML Functions


def set_namespaces(etree):
	'''
		Set namespace for ElementTree class
		
		:param etree: ElementTree class
	'''
	if etree.VERSION[0:3] == '1.2':
		#in etree < 1.3, this is a workaround for supressing prefixes

		def fixtag(tag, namespaces):
			import string
			# given a decorated tag (of the form {uri}tag), return prefixed
			# tag and namespace declaration, if any
			if isinstance(tag, etree.QName):
				tag = tag.text
			namespace_uri, tag = string.split(tag[1:], "}", 1)
			prefix = namespaces.get(namespace_uri)
			if namespace_uri not in namespaces:
				prefix = etree._namespace_map.get(namespace_uri)
				if namespace_uri not in etree._namespace_map:
					prefix = "ns%d" % len(namespaces)
				namespaces[namespace_uri] = prefix
				if prefix == "xml":
					xmlns = None
				else:
					if prefix is not None:
						nsprefix = ':' + prefix
					else:
						nsprefix = ''
					xmlns = ("xmlns%s" % nsprefix, namespace_uri)
			else:
				xmlns = None
			if prefix is not None:
				prefix += ":"
			else:
				prefix = ''

			return "%s%s" % (prefix, tag), xmlns

		etree.fixtag = fixtag
		etree._namespace_map['http://autosar.org/schema/r4.0'] = None
		etree._namespace_map['http://www.w3.org/2001/XMLSchema-instance'] = 'xsi'
	else:
		#For etree > 1.3, use register_namespace function
		etree.register_namespace('', 'http://autosar.org/schema/r4.0')
		etree.register_namespace('xsi', 'http://www.w3.org/2001/XMLSchema-instance')

# Set namespaces for output
set_namespaces(ET)

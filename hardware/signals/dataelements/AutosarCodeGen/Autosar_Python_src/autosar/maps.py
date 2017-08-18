#########################################
# This file contains global hashmaps
#########################################

# copy = original
object_copy = dict()

# referenced path = [ refering objects ]
references = dict()

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

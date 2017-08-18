#!/bin/python

from optparse import OptionParser

import autosar
import sys
import os


VERSION = '1.0'


def checkCompositions(infile, comp_list=None):

	arxml = autosar.arxml.load(infile)

	results = dict()

	if comp_list is not None:
		comp_dict = dict(zip(comp_list,comp_list))

	for k,c in arxml.getCompositions().iteritems():
		if comp_list is not None and k in comp_dict:
			results[k] = checkUnconnectedPortsOnComposition(c)
		elif comp_list is not None:
			continue
		else:
			results[k] = checkUnconnectedPortsOnComposition(c)

	
	return results



def checkUnconnectedPortsOnComposition(composition):
	if options.verbose:
		print '== Checking composition %s'%(composition.path)

	inner_port = dict()

	fan_in = dict()
	unconnected = list()


	# Load all port-prototypes from inner components/compositions
	pd = dict()

	if not len(composition.swcs) > 0:
		return {'unconnected':unconnected , 'fan-in': fan_in}


	for k,c in composition.swcs.iteritems():
		if c in composition.arxml.compositions:
			inner_port.update(composition.arxml.getComposition(c).ports)
			for p,v in composition.arxml.getComposition(c).ports.iteritems():
				pd[c+'/'+p] = v
		elif c in composition.arxml.swcs:
			inner_port.update(composition.arxml.getComponent(c).ports)
			for p,v in composition.arxml.getComponent(c).ports.iteritems():
				pd[c+'/'+p] = v
			
	# Load all port prototypes in own composition
	for p,v in composition.ports.iteritems():
		pd[composition.path+'/'+p] = v

	# Load all assemblies and delegations
	dl = dict()
	for k,v in composition.delegations.iteritems():
		if v.inner_port.text in dl:
			dl[v.inner_port.text].append(v.outer_port.text)
			if pd[v.inner_port.text].__class__ == autosar.components.AR_RPort:
				#fan_in.append(v.inner_port.text)
				fan_in[v.inner_port.text] = dl[v.outer_port.text]
			
		else:
			dl[v.inner_port.text] = [v.outer_port.text]


		if v.outer_port.text in dl:
			dl[v.outer_port.text].append(v.inner_port.text)
			if composition.ports[v.outer_port.text.split('/')[-1]].__class__ == autosar.components.AR_PPort:
				#fan_in.append(v.outer_port.text)
				fan_in[v.outer_port.text] = dl[v.inner_port.text]
		else:
			dl[v.outer_port.text] = [v.inner_port.text]



	al = dict()
	for k,v in composition.assemblys.iteritems():
		if v.requester_port.text in al:
			al[v.requester_port.text].append(v.provider_port.text)
			#fan_in.append(v.requester_port.text)
			fan_in[v.requester_port.text] = al[v.provider_port.text]
		elif v.requester_port.text in dl:
                        if not v.requester_port.text in fan_in:
                                fan_in[v.requester_port.text] = [v.provider_port.text, dl[v.requester_port.text]]
                        else:
                                fan_in[v.requester_port.text].append(v.provider_port.text)
		else:
			al[v.requester_port.text] = [v.provider_port.text]
	
		if v.provider_port.text in al:
			al[v.provider_port.text].append(v.requester_port.text)
			#fan_in.append(v.provider_port.text)
		else:
			al[v.provider_port.text] = [v.requester_port.text]




		

	# All inner ports not connected via delegation or assembly
	
	for p in  set(pd) - (set(dl) | set(al)):
		if p.split('/')[-1] != 'EcuMVccActivationMode':
			if options.verbose:
				print "%s is unconnected"%(p)
			unconnected.append((p,pd[p].__class__))

	if options.verbose:
		for k,f in fan_in.iteritems():
			print "%s is a fan-in"%(k)
			for i in f:
				print "\t from %s"%(i)


	return {'unconnected':unconnected , 'fan-in': fan_in}


def prettyPrint(result):
	print "======================= RESULT =========================="
	for c in result:
		if len(result[c]['unconnected']) > 0 or len(result[c]['fan-in']) > 0:
			print "|  Composition   | %s  "%(c)
			if len(result[c]['unconnected']) > 0:
				print "|  Unconnected   |"
				for u in result[c]['unconnected']:
					print "|                | %s (%s)  "%(u)
			
			if len(result[c]['fan-in']) > 0:
				print "|  Fan-In        |"
				for u in result[c]['fan-in']:
					print "|                | %s"%(u)
					for f in result[c]['fan-in'][u]:
						print "|                |   - %s"%(f)
		
			print "---------------------------------------------------------"
	print "========================================================="




if __name__ == "__main__":
	usage = "Usage: %prog [options] <file>"
	parser = OptionParser(version="%%prog %s (%s)"%(VERSION, autosar.VERSION), usage=usage)

	parser.add_option('-v', '--verbose', dest='verbose', action='store_true', help='Enable verbose output')

	(options, args) = parser.parse_args()

	error = False

	if len(args) < 1:
		parser.error('Missing file(s) on input')
	else:
		for a in args:
			if os.path.isfile(a):
				r = checkCompositions(a)
				print ""
				prettyPrint(r)
				for c in r:
					if len(r[c]['unconnected']) > 0:
						error = True
					if len(r[c]['fan-in'])> 0:
						error = True
			else:
				print "ERROR: File not found %s"%(a)

		if error:
			parser.error('Unconnected or Fan-ins detected')

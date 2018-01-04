# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys		#Argument parsing dependancy

class AsnvOperation:

	def __init__(self, opname, ident):
		self.OpTypeList = [0,2,4,5,112,224]
		self.op = opname
		self.id = ident
		self.operation = []

	def GetOp(self):
		x = self.operation.pop(0)
		return x

	def HasOp(self):
		return len(self.operation)

#Generates an ASNV_Configuration from an ASN release.

def ASNV_CONF(file):

	try:
		temp = file.split('\\')
	except:
		ASNV = 'ASNV_Configuration.xml' % (file.split('.')[0], )
	else:
		ASNV = ''
		for x in temp:
			if x == temp[len(temp)-1]:
				ASNV += 'ASNV_Configuration.xml'
			else:
				ASNV += '%s\\'%(x, )
	try:
		tf = open(ASNV, 'w')
		try:
			tf.write('<?xml version="1.0" encoding="utf-8"?>\n<asnv>\n')
		finally:
			tf.close()
	except IOError:
		pass

	subln = ''
	oplist = []

	with open(file) as f:
		content = f.readlines()
	for line in content:
		if str(line).find('OperationID:') != -1:
			subln = line.split(' ')
			print subln[2]
			if subln[2].find('(') != -1:
				subln[2] = AsnvOperation(subln[2].split('(')[0], int(subln[3][3:7], 16))
			else:
				subln[2] = AsnvOperation(subln[2], int(subln[3][3:7], 16))
			oplist += [(subln[2], subln[2].id)]
		elif subln:
			if str(line).find('::= SEQUENCE') != -1:
				try:
					subln[2].operation += [(line.split(' ')[0], opnr(line.split(' ')[0].split('-')[1]))]
				except:
					pass
		else:
			pass

	try:
		tf = open(ASNV, 'a')
		try:
			for (op,id) in oplist:
				while op.HasOp():
					(subOp, opType) = op.GetOp()
					tf.write('\t<protocol name=\"%s\">\n' % (subOp, ))
					tf.write('\t\t<encoding entry-point=\"%s\" method=\"per\" />\n' % (subOp, ))
					tf.write('\t\t<lower-layer name=\"vccipcp\" value1=\"%d\" value2=\"%d\" />\n' % (id, opType))
					tf.write('\t\t<source path=\"ASNV_Module_VccPduHeader.xml\" />\n')
					tf.write('\t</protocol>\n')
					try:
						op.OpTypeList.remove(opType)
					except ValueError:
						print "Warning: unknown opType"

				for missingType in op.OpTypeList:
					tf.write('\t<protocol name=\"%s-%s\">\n' % (op.op, optypeF(missingType)))
					tf.write('\t\t<encoding entry-point=\"%s-%s\" method=\"per\" />\n' % (op.op, optypeF(missingType)))
					tf.write('\t\t<lower-layer name=\"vccipcp\" value1=\"%d\" value2=\"%d\" />\n' % (id, missingType))
					tf.write('\t\t<source path=\"ASNV_Module_VccPduHeader.xml\" />\n')
					tf.write('\t</protocol>\n')

			tf.write('</asnv>')
		finally:
			tf.close()
	except IOError:
		pass

def ASNV_CONF_MOD(file, module):

	try:
		temp = file.split('\\')
	except:
		ASNV = 'ASNV_Configuration.xml' % (file.split('.')[0], )
	else:
		ASNV = ''
		for x in temp:
			if x == temp[len(temp)-1]:
				ASNV += 'ASNV_Configuration.xml'
			else:
				ASNV += '%s\\'%(x, )
	try:
		tf = open(ASNV, 'w')
		try:
			tf.write('<?xml version="1.0" encoding="utf-8"?>\n<asnv>\n')
		finally:
			tf.close()
	except IOError:
		pass

	subln = ''
	oplist = []

	with open(file) as f:
		content = f.readlines()
	for line in content:
		if str(line).find('OperationID:') != -1:
			subln = line.split(' ')
			print subln[2]
			if subln[2].find('(') != -1:
				subln[2] = AsnvOperation(subln[2].split('(')[0], int(subln[3][3:7], 16))
			else:
				subln[2] = AsnvOperation(subln[2], int(subln[3][3:7], 16))
			oplist += [(subln[2], subln[2].id)]
		elif subln:
			if str(line).find('::= SEQUENCE') != -1:
				try:
					subln[2].operation += [(line.split(' ')[0], opnr(line.split(' ')[0].split('-')[1]))]
				except:
					pass
		else:
			pass

	try:
		tf = open(ASNV, 'a')
		with open (module) as m:
			content = m.readlines()
		tm = open(module, 'w')
		for line in content:
			if not "</module>" in line:
				tm.write(line)
		try:
			for (op,id) in oplist:
				while op.HasOp():
					(subOp, opType) = op.GetOp()
					tf.write('\t<protocol name=\"%s\">\n' % (subOp, ))
					tf.write('\t\t<encoding entry-point=\"%s\" method=\"per\" />\n' % (subOp, ))
					tf.write('\t\t<lower-layer name=\"vccipcp\" value1=\"%d\" value2=\"%d\" />\n' % (id, opType))
					tf.write('\t\t<source path=\"ASNV_Module_VccPduHeader.xml\" />\n')
					tf.write('\t</protocol>\n')
					try:
						op.OpTypeList.remove(opType)
					except ValueError:
						print "Warning: unknown opType"

				for missingType in op.OpTypeList:

					tf.write('\t<protocol name=\"%s-%s\">\n' % (op.op, optypeF(missingType)))
					tf.write('\t\t<encoding entry-point=\"%s-%s\" method=\"per\" />\n' % (op.op, optypeF(missingType)))
					tf.write('\t\t<lower-layer name=\"vccipcp\" value1=\"%d\" value2=\"%d\" />\n' % (id, missingType))
					tf.write('\t\t<source path=\"ASNV_Module_VccPduHeader.xml\" />\n')
					tf.write('\t</protocol>\n')

					tm.write('\n  <sequence name=\"%s-%s\">\n' % (op.op, optypeF(missingType)))
					tm.write('    <field name="Placeholder" type="DataCounter"/>\n')
					tm.write('    <extension/>\n')
					tm.write('  </sequence>\n')


			tm.write('</module>')
			tf.write('</asnv>')
		finally:
			tf.close()
			tm.close()
	except IOError:
		pass


def opnr(type):
	return {
        'SetRequest': 2,
        'Request': 0,
        'Response': 4,
        'Notification': 5,
		'Ack':112,
		'Error': 224
        }[type]

def optypeF(nr):
	return {
	    2: 'SetRequest',
        0: 'Request',
        4: 'Response',
        5: 'Notification',
		112: 'Ack',
		224: 'Error'
        }[nr]

if __name__ == '__main__':
	if len(sys.argv) == 2:
		ASNV_CONF(sys.argv[1])
	elif len(sys.argv) > 2:
		ASNV_CONF_MOD(sys.argv[1],sys.argv[2])
	else:
		print '\nUsage: ASNV_CONF_make.py <path>filename.asn'
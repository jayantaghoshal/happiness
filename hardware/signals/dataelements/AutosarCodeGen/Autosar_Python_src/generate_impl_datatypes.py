from optparse import OptionParser
from xml.etree import ElementTree as ET
import autosar


VERSION = '1.2'
DESCRIPTION = 'AUTOSAR Implementation Datatype generator'
COPYRIGHT = 'Confidential and exclusive property. (c) Volvo Car Corporation 2013. \nUnauthorized usage or disclosure prohibited. All rights reserved, \neven in the event of applications for industrial property rights'

if __name__ == "__main__":
    print '%s (%s)\n%s\n'%(DESCRIPTION, VERSION, COPYRIGHT)

#########################################################
# Handle datatypes

def conversionError(datatype):
    return "ERROR: Not supported datatype CATEGORY"


def convertBoolean(datatype):
    #print 'Boolean'
    return (datatype.shortname, 'boolean', datatype.getCompuMethod().path)


def convertValue(datatype):

    compu = datatype.getCompuMethod()
    category = None


    if 'Float' in datatype.shortname or 'float' in datatype.shortname:
        return (datatype.shortname, 'float32', compu.path)


    if compu is not None:
        category = compu.getCategory()

    basetype = get_compu[category](datatype)

    #print 'Value with basetype %s'%(str(basetype))
    return basetype


def convertStructure(datatype):

    elems = list()

    for e in datatype.getElements():
        #basetype = get_conversion[e.getCategory()](e)
        #elems.append(basetype)
        basetype = (e.shortname, e.getType())
        elems.append(basetype)

    return (datatype.shortname, elems)


def convertArray(datatype):
    elemtype = datatype.getElementType()
    #print 'Array of %s'%(elemtype)
    return (datatype.shortname, elemtype['datatype'], int(elemtype['max_elems']), elemtype['semantics'])


get_conversion = {'BOOLEAN':convertBoolean,
                  'LINEAR':conversionError,
                  'TEXTTABLE':conversionError,
                  'IDENTICAL':conversionError,
                  'VALUE':convertValue,
                  'STRUCTURE':convertStructure,
                  'ARRAY':convertArray}


###############################################################

def getBaseType(lower, upper, signed):

    if signed:
        if (upper - lower) > 0.0:
            if upper <= 127.0:
                return 'sint8'
            elif upper <= 32767.0:
                return 'sint16'
            elif upper <= 2147483647.0:
                return 'sint32'
            else:
                return None
        else:
            if lower >= -128.0:
                return 'sint8'
            elif lower >= -32768.0:
                return 'sint16'
            elif lower >= -2147483648.0:
                return 'sint32'
            else:
                return None
    else:
        if upper <= 255.0:
            return 'uint8'
        elif upper <= 65535.0:
            return 'uint16'
        elif upper <= 4294967295.0:
            return 'uint32'
        else:
            return None





def convertLinear(datatype):
    constr = datatype.getDataConstraint()
    (lower, upper) = constr.getLimits()

    compu = datatype.getCompuMethod()

    numerator = compu.getCompuNumerator()
    denominator = compu.getCompuDenominator()


    offset = float(numerator[0])
    factor = float(numerator[1])


    lower = float(lower)/factor
    upper = float(upper)/factor
    offset = float(offset)/factor

    basetype = getBaseType(float(lower)-offset, float(upper)-offset, float(lower)-offset < 0.0)
    #print float(lower)-offset

    #if float(lower)-offset < 0.0:
    #    pass
    #else:
    #    pass



    #print 'Linear value with limits [%s,%s]'%constr.getLimits()

    return (datatype.shortname, basetype, compu.path)


def convertTextable(datatype):
    compu = datatype.getCompuMethod()

    limit = max(compu.getValues())

    for enum,limits in compu.getEnumerations().iteritems():
        lower, upper = limits
        if lower != upper:
            print 'WARNING: Enumeration %s of datatype %s uses a not supported range (%s,%s)'%(enum, datatype.shortname, lower, upper)
            if options.truncate_enum:
                print 'INFO: Force truncate enum range values used on enumeration %s (%s, %s)'%(enum, lower, lower)
                compu.setEnumeration(enum, (lower,lower))
                DocRev = autosar.arxml.find2(autosar.arxml.find2(datatype.xmlref,'ADMIN-DATA'),'DOC-REVISION')
                OldRev = autosar.arxml.find2(DocRev,'REVISION-LABEL').text
                NewRev = OldRev.split('.')[:-1]
                NewRev.append(str(int(OldRev.split('.')[-1]) + 1))
                NewRev = ''.join(map((lambda x: x + '.'),NewRev))[:-1] 
                Modifications = autosar.base.create_element('MODIFICATIONS')
                Modification = Modifications.addPropertyElement(autosar.base.create_element('MODIFICATION'))
                Change = Modification.addPropertyElement(autosar.base.create_element('CHANGE'))
                Change.addPropertyElement(autosar.base.create_element('L-2',value='REVISION-LABEL has been updated from %s to %s by %s (%s)'%(OldRev,NewRev,DESCRIPTION, VERSION), attrib={'L':'FOR-ALL'}))
                autosar.arxml.find2(DocRev,'REVISION-LABEL').text = NewRev
                autosar.base.ARObject(DocRev).addPropertyElement(Modifications)

    basetype = getBaseType(limit, limit, False)

    #print 'Enum of datatype %s with values %s'%(basetype, compu.getEnumerations())
    return (datatype.shortname, basetype, compu.path)

def convertIdentical(datatype):
    constr = datatype.getDataConstraint()
    compu = datatype.getCompuMethod()
    (lower, upper) = constr.getLimits()

    

    # Check if signed number
    basetype = getBaseType(float(lower), float(upper), float(lower) < 0.0)

    #print 'Identical value with basetype %s'%(basetype)

    return (datatype.shortname, basetype, compu.path)



get_compu = {'LINEAR':convertLinear,
             'TEXTTABLE':convertTextable,
             'IDENTICAL':convertIdentical}



##############################################################


def create_arxml(impl_type):
    # Unpack tuple
    impl_shortname , elems = impl_type[0], impl_type[1:]
    impl_category = ""
    impl_basetype = ""
    #impl_compu = elems[1]

    #print elems

    if type(elems[0]) is list:
        impl_category = 'STRUCTURE'

        tb = ET.TreeBuilder()
        tb.start('IMPLEMENTATION-DATA-TYPE', {})
        tb.start('SHORT-NAME',{})
        tb.data(impl_shortname)
        tb.end('SHORT-NAME')
        tb.start('CATEGORY',{})
        tb.data(impl_category)
        tb.end('CATEGORY')        
        tb.start('SW-DATA-DEF-PROPS',{})
        tb.start('SW-DATA-DEF-PROPS-VARIANTS',{})
        tb.start('SW-DATA-DEF-PROPS-CONDITIONAL',{})
        tb.end('SW-DATA-DEF-PROPS-CONDITIONAL')
        tb.end('SW-DATA-DEF-PROPS-VARIANTS')
        tb.end('SW-DATA-DEF-PROPS')
        tb.start('SUB-ELEMENTS',{})

        for e in elems[0]:
            impl_shortname = str(e[0])
            impl_basetype = str(e[1])
            impl_basetype = impl_basetype.replace('/DataType', '/DataTypeImplementation')
            tb.start('IMPLEMENTATION-DATA-TYPE-ELEMENT', {})
            tb.start('SHORT-NAME',{})
            tb.data(impl_shortname)
            tb.end('SHORT-NAME')
            tb.start('CATEGORY',{})
            tb.data('TYPE_REFERENCE')
            tb.end('CATEGORY')
            tb.start('SW-DATA-DEF-PROPS',{})
            tb.start('SW-DATA-DEF-PROPS-VARIANTS',{})
            tb.start('SW-DATA-DEF-PROPS-CONDITIONAL',{})
            tb.start('IMPLEMENTATION-DATA-TYPE-REF', {'DEST':'IMPLEMENTATION-DATA-TYPE'})
            tb.data(impl_basetype)
            tb.end('IMPLEMENTATION-DATA-TYPE-REF')
            tb.end('SW-DATA-DEF-PROPS-CONDITIONAL')
            tb.end('SW-DATA-DEF-PROPS-VARIANTS')
            tb.end('SW-DATA-DEF-PROPS')
            tb.end('IMPLEMENTATION-DATA-TYPE-ELEMENT')

        


        tb.end('SUB-ELEMENTS')
        tb.end('IMPLEMENTATION-DATA-TYPE')
        elem = tb.close()

    elif len(elems) == 2:
        impl_basetype = '/DataType/SwBaseTypes/%s'%(str(elems[0]))
        impl_compu = str(elems[1])
        impl_category = 'VALUE'
        tb = ET.TreeBuilder()
        tb.start('IMPLEMENTATION-DATA-TYPE',{})
        tb.start('SHORT-NAME',{})
        tb.data(impl_shortname)
        tb.end('SHORT-NAME')
        tb.start('CATEGORY',{})
        tb.data(impl_category)
        tb.end('CATEGORY')
        tb.start('SW-DATA-DEF-PROPS',{})
        tb.start('SW-DATA-DEF-PROPS-VARIANTS',{})
        tb.start('SW-DATA-DEF-PROPS-CONDITIONAL',{})
        tb.start('BASE-TYPE-REF',{'DEST':'SW-BASE-TYPE'})
        tb.data(impl_basetype)
        tb.end('BASE-TYPE-REF')
        tb.start('COMPU-METHOD-REF',{'DEST':'COMPU-METHOD'})
        tb.data(impl_compu)
        tb.end('COMPU-METHOD-REF')
        tb.start('SW-CALIBRATION-ACCESS',{})
        tb.data('READ-WRITE')
        tb.end('SW-CALIBRATION-ACCESS')
        tb.end('SW-DATA-DEF-PROPS-CONDITIONAL')
        tb.end('SW-DATA-DEF-PROPS-VARIANTS')
        tb.end('SW-DATA-DEF-PROPS')
        tb.end('IMPLEMENTATION-DATA-TYPE')

        elem = tb.close()

    else:
        impl_category = 'ARRAY'
        impl_basetype = str(elems[0])
        impl_basetype = impl_basetype.replace('/DataType', '/DataTypeImplementation')
        impl_arraysize = str(elems[1])
        impl_semantics = str(elems[2])
        tb = ET.TreeBuilder()
        tb.start('IMPLEMENTATION-DATA-TYPE', {})
        tb.start('SHORT-NAME',{})
        tb.data(impl_shortname)
        tb.end('SHORT-NAME')
        tb.start('CATEGORY',{})
        tb.data(impl_category)
        tb.end('CATEGORY')        
        tb.start('SW-DATA-DEF-PROPS',{})
        tb.start('SW-DATA-DEF-PROPS-VARIANTS',{})
        tb.start('SW-DATA-DEF-PROPS-CONDITIONAL',{})
        tb.end('SW-DATA-DEF-PROPS-CONDITIONAL')
        tb.end('SW-DATA-DEF-PROPS-VARIANTS')
        tb.end('SW-DATA-DEF-PROPS')
        tb.start('SUB-ELEMENTS',{})
        tb.start('IMPLEMENTATION-DATA-TYPE-ELEMENT', {})
        tb.start('SHORT-NAME', {})
        tb.data('%s_elementsImpl'%(impl_shortname))
        tb.end('SHORT-NAME')
        tb.start('CATEGORY',{})
        tb.data('TYPE_REFERENCE')
        tb.end('CATEGORY')
        tb.start('ARRAY-SIZE',{})
        tb.data(impl_arraysize)
        tb.end('ARRAY-SIZE')
        tb.start('ARRAY-SIZE-SEMANTICS',{})
        tb.data('FIXED-SIZE')
        tb.end('ARRAY-SIZE-SEMANTICS')
        tb.start('SW-DATA-DEF-PROPS',{})
        tb.start('SW-DATA-DEF-PROPS-VARIANTS',{})
        tb.start('SW-DATA-DEF-PROPS-CONDITIONAL',{})
        tb.start('IMPLEMENTATION-DATA-TYPE-REF', {'DEST':'IMPLEMENTATION-DATA-TYPE'})
        tb.data(impl_basetype)
        tb.end('IMPLEMENTATION-DATA-TYPE-REF')
        tb.end('SW-DATA-DEF-PROPS-CONDITIONAL')
        tb.end('SW-DATA-DEF-PROPS-VARIANTS')
        tb.end('SW-DATA-DEF-PROPS')
        tb.end('IMPLEMENTATION-DATA-TYPE-ELEMENT')
        tb.end('SUB-ELEMENTS')
        tb.end('IMPLEMENTATION-DATA-TYPE')

        elem = tb.close()
       
    return (impl_category, elem)
    


def mergeSwBaseTypes(basetypes, elektrafile):

    swbase = basetypes.getPackage('/DataType/SwBaseTypes')
    ef_dt = elektrafile.getPackage('/DataType')


    ef_dt.addPackage(swbase)





def createDataTypeMapping(appl_type_path, impl_type_path, category):

    cat = {'VALUE':'APPLICATION-PRIMITIVE-DATA-TYPE',
           'STRUCTURE': 'APPLICATION-RECORD-DATA-TYPE',
           'ARRAY': 'APPLICATION-ARRAY-DATA-TYPE'}



    appl_type_type = cat[category]

    tb = ET.TreeBuilder()

    tb.start('DATA-TYPE-MAP', {})
    tb.start('APPLICATION-DATA-TYPE-REF',{'DEST':appl_type_type})
    tb.data(appl_type_path)
    tb.end('APPLICATION-DATA-TYPE-REF')
    tb.start('IMPLEMENTATION-DATA-TYPE-REF', {'DEST':'IMPLEMENTATION-DATA-TYPE'})
    tb.data(impl_type_path)
    tb.end('IMPLEMENTATION-DATA-TYPE-REF')
    tb.end('DATA-TYPE-MAP')

    elem = tb.close()

    return elem



def generate_implementation_datatypes(elektrafile, outputfile):

    generated_datatypes = 0


    impl_package = elektrafile.addPackage('DataTypeImplementation')

    
    try:
        impl_mappings = elektrafile.getPackage('/DataTypeMappings')

    except KeyError:
        impl_mappings = elektrafile.addPackage('DataTypeMappings')


    ms = ET.Element('DATA-TYPE-MAPPING-SET')
    sn = ET.Element('SHORT-NAME')
    sn.text = 'DataTypeMappingSet'
    dms = ET.Element('DATA-TYPE-MAPS')
    ms.append(sn)
    ms.append(dms)
    
    #impl_mappings.xmlref.append(ms)
    impl_mappings.addElement(autosar.base.ARObject(ms))



    for k,d in elektrafile.datatypes.iteritems():
        print "Creating implementation datatype for %s"%(d.path)

        category = d.getCategory()

        impltype = get_conversion[category](d)

        category, xml = create_arxml(impltype)

        if xml is not None:
            e = impl_package.addElement(autosar.components.ARImplDatatype(xml))

            m = createDataTypeMapping(d.path, e.path, category)

            dms.append(m)


        #print "\tCreated %s datatype"%(str(impltype))
        generated_datatypes += 1


    print "INFO: Generated %s implementation datatypes"%(generated_datatypes)




if __name__ == "__main__":
#	'''
#	   Generates implementation datatypes to match with the Elektra application datatypes in (param 1) and adds it to the new Autosar XML file (param 2)
#
#	   :param infile: Autosar XML file to load from Elektra
#	   :param outfile: Autosar XML file as output
#	   :return: Nothing
#	'''
    parser = OptionParser(usage='%prog [options] <Elektra ARXML> <Output file>')

    parser.add_option('-t', '--truncate-enums', dest='truncate_enum', action='store_true', help='Truncate enum ranges so lower == upper limit')

    (options, args) = parser.parse_args()

    # ARG1 == Elektra ARXML
    elektrafilename = args[0]

    #ARG2 == Output file
    outputfilename = args[1]

    ef = autosar.arxml.load(elektrafilename)

    generate_implementation_datatypes(ef, outputfilename)
   
    ef.save(outputfilename, 'generate_impl_datatypes.py %s'%(VERSION))

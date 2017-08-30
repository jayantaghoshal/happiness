####################################################
# AUTOSAR SW-C and other related objects
#
####################################################
from . import xml_helper
from . import base
from . import maps
from . import options
import typing
from typing import Dict
from xml.etree import ElementTree as ET


# import pprint

def getChildTextRecursive(e):
    accumulatedText = e.text
    for child in e:
        accumulatedText += getChildTextRecursive(child)
    return accumulatedText


class ARComposition(base.ARObject):
    '''
       This class represent a SW-COMPOSITION object in AUTOSAR XML.
    '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)
        self.__loadSWCs()
        self.__loadPorts()
        self.__loadDelegations()
        self.__loadAssemblys()
        self.__loadPortGroups()

    def __loadSWCs(self):
        ''' Load all SWC references in current composition '''
        self.swcs = dict()  # type: typing.Dict[str, str]
        comp_prot = self.xmlref.findall(base.add_schema('COMPONENTS/SW-COMPONENT-PROTOTYPE'))
        for c in comp_prot:
            shortname = base.GetShortName(c)
            type_ref = c.find(base.add_schema('TYPE-TREF'))

            maps.addReference(type_ref.text, type_ref)
            self.swcs[shortname] = type_ref.text

    def __loadPorts(self):
        ''' Load all Port references in current composition '''
        self.ports = dict()  # type: Dict[str, typing.Union[AR_PPort, AR_RPort]]
        r_ports = self.xmlref.findall(base.add_schema('PORTS/R-PORT-PROTOTYPE'))
        for r in r_ports:
            self.ports[base.GetShortName(r)] = AR_RPort(r, '%s/%s' % (self.path, base.GetShortName(r)), self.arxml)
            DEs = r.findall('.//' + base.add_schema('DATA-ELEMENT-REF'))
            for DE in DEs:
                maps.addReference(DE.text, DE)

        p_ports = self.xmlref.findall(base.add_schema('PORTS/P-PORT-PROTOTYPE'))
        for p in p_ports:
            self.ports[base.GetShortName(p)] = AR_PPort(p, '%s/%s' % (self.path, base.GetShortName(p)), self.arxml)
            DEs = p.findall('.//' + base.add_schema('DATA-ELEMENT-REF'))
            for DE in DEs:
                maps.addReference(DE.text, DE)

    def __loadPortGroups(self):
        ''' Load all PortGroups in current composition '''
        pgs = self.xmlref.findall(base.add_schema('PORT-GROUPS/PORT-GROUP'))

        self.port_groups = dict()  # type: Dict[str, ARComposition]

        for pg in pgs:

            shortname = base.GetShortName(pg)
            self.port_groups[shortname] = pg

            context = pg.findall(base.add_schema('INNER-GROUP-IREFS/INNER-GROUP-IREF/CONTEXT-REF'))
            target = pg.findall(base.add_schema('INNER-GROUP-IREFS/INNER-GROUP-IREF/TARGET-REF'))

            for c in context:
                maps.addReference(c.text, c)

            for t in target:
                maps.addReference(t.text, t)

            outer = pg.findall(base.add_schema('OUTER-PORTS/PORT-PROTOTYPE-REF-CONDITIONAL/PORT-PROTOTYPE-REF'))

            for o in outer:
                maps.addReference(o.text, o)

    def __loadDelegations(self):
        ''' Load all delegations in current composition '''
        del_cons = self.xmlref.findall(base.add_schema('CONNECTORS/DELEGATION-SW-CONNECTOR'))
        self.delegations = dict()  # type: Dict[str, ARDelegationConnector]
        for d in del_cons:
            del_name = xml_helper.find(d, 'SHORT-NAME').text
            self.delegations[del_name] = ARDelegationConnector(d)

    def __loadAssemblys(self):
        ''' Load all delegations in current compostion '''
        ass_cons = self.xmlref.findall(base.add_schema('CONNECTORS/ASSEMBLY-SW-CONNECTOR'))
        self.assemblys = dict()  # type: Dict[str, ARAssemblyConnector]
        for a in ass_cons:
            ass_name = xml_helper.find(a, 'SHORT-NAME').text
            self.assemblys[ass_name] = ARAssemblyConnector(a)

    def updateReferences(self, old_path):
        '''
           Called when moving a ARComposition to update all references.

           :param old_path: Previous path of this composition.
        '''
        self.__loadDelegations()
        self.__loadAssemblys()

        print("Updating %s -> %s" % (old_path, self.path))

        # Update component prototypes
        self.__updateReferences(old_path, self.swcs)
        # Update port prototypes
        self.__updateReferences(old_path, self.ports)
        # Update port groups
        self.__updateReferences(old_path, self.port_groups)

    def __updateReferences(self, old_path, elem_list):
        '''
           Internal helper function to update references of moved composition.

           :param old_path: Previous path of composition
           :param elem_list: List of subelements to update.
        '''
        for prot in elem_list:
            o_path = old_path + '/' + prot
            n_path = self.path + '/' + prot
            print("Checking %s != %s" % (o_path, n_path))

            if o_path in maps.references and o_path != n_path:
                if options.verbose:
                    print("Updating %s -> %s" % (o_path, n_path))
                maps.updateReference(o_path, n_path)
                # for r in maps.references[o_path]:
                #    r.text = n_path

    def addSWC(self, shortname, comp_type, instance=None):
        print('Adding SWC %s as %s' % (comp_type, '%s/%s' % (self.path, shortname)))
        if not shortname in self.swcs:
            components = self.getSubElem('COMPONENTS')

            if components is None:
                components = base.create_element('COMPONENTS').xmlref
                if self.getSubElem('ADMIN-DATA') is None:
                    self.xmlref.insert(1, components)
                else:
                    self.xmlref.insert(2, components)

            # print ET.tostring(components)
            tb = ET.TreeBuilder()
            tb.start('SW-COMPONENT-PROTOTYPE', {})
            tb.start('SHORT-NAME', {})
            tb.data(shortname)
            tb.end('SHORT-NAME')
            if isinstance(instance, ARSwc):
                tt = tb.start('TYPE-TREF', {'DEST': 'APPLICATION-SW-COMPONENT-TYPE'})
            else:
                tt = tb.start('TYPE-TREF', {'DEST': 'COMPOSITION-SW-COMPONENT-TYPE'})
            tb.data(comp_type)
            tb.end('TYPE-TREF')
            tb.end('SW-COMPONENT-PROTOTYPE')
            elem = tb.close()

            maps.addReference(tt.text, tt)
            self.swcs[shortname] = tt.text

            components.append(elem)

    def addAssemblyConnection(self, shortname, rc, rp, pc, pp):
        if options.verbose:
            print('Creating Assembly-connector (%s)' % (shortname))

        connectors = self.getSubElem('CONNECTORS')

        if connectors is None:
            connectors = base.create_element('CONNECTORS').xmlref
            self.xmlref.append(connectors)

        tb = ET.TreeBuilder()
        tb.start('ASSEMBLY-SW-CONNECTOR', {})
        tb.start('SHORT-NAME', {})
        tb.data(shortname)
        tb.end('SHORT-NAME')
        tb.start('PROVIDER-IREF', {})
        tb.start('CONTEXT-COMPONENT-REF', {'DEST': 'SW-COMPONENT-PROTOTYPE'})
        tb.data(pc)
        tb.end('CONTEXT-COMPONENT-REF')
        tb.start('TARGET-P-PORT-REF', {'DEST': 'P-PORT-PROTOTYPE'})
        tb.data(pp)
        tb.end('TARGET-P-PORT-REF')
        tb.end('PROVIDER-IREF')

        tb.start('REQUESTER-IREF', {})
        tb.start('CONTEXT-COMPONENT-REF', {'DEST': 'SW-COMPONENT-PROTOTYPE'})
        tb.data(rc)
        tb.end('CONTEXT-COMPONENT-REF')
        tb.start('TARGET-R-PORT-REF', {'DEST': 'R-PORT-PROTOTYPE'})
        tb.data(rp)
        tb.end('TARGET-R-PORT-REF')
        tb.end('REQUESTER-IREF')
        tb.end('ASSEMBLY-SW-CONNECTOR')
        elem = tb.close()

        connectors.append(elem)

    def addDelegationConnection(self, shortname, cc, ip, op, port_type):
        if options.verbose:
            print('Creating Delegation-connector (%s)' % (shortname))

        connectors = self.getSubElem('CONNECTORS')

        if connectors is None:
            connectors = base.create_element('CONNECTORS').xmlref
            self.xmlref.append(connectors)

        tb = ET.TreeBuilder()
        tb.start('DELEGATION-SW-CONNECTOR', {})
        tb.start('SHORT-NAME', {})
        tb.data(shortname)
        tb.end('SHORT-NAME')
        tb.start('INNER-PORT-IREF', {})
        tb.start('%s-PORT-IN-COMPOSITION-INSTANCE-REF' % (port_type), {})
        tb.start('CONTEXT-COMPONENT-REF', {'DEST': 'SW-COMPONENT-PROTOTYPE'})
        tb.data(cc)
        tb.end('CONTEXT-COMPONENT-REF')
        tb.start('TARGET-%s-PORT-REF' % (port_type), {'DEST': '%s-PORT-PROTOTYPE' % (port_type)})
        tb.data(ip)
        tb.end('TARGET-%s-PORT-REF' % (port_type))
        tb.end('%s-PORT-IN-COMPOSITION-INSTANCE-REF' % (port_type))
        tb.end('INNER-PORT-IREF')
        tb.start('OUTER-PORT-REF', {'DEST': '%s-PORT-PROTOTYPE' % (port_type)})
        tb.data(op)
        tb.end('OUTER-PORT-REF')
        tb.end('DELEGATION-SW-CONNECTOR')

        elem = tb.close()

        connectors.append(elem)

    def compare(self, composition):
        '''
           Compare this composition with another composition. The comparison
           checks the Ports and component prototypes in the composition. The
           matching is done on protype SHORT-NAME.

           The comparison function returns a dictionary of differences wrt
           ports and components.

           :param composition: ARComposition instance to compare to.
           :return: Dictionary containing lists of added/removed ports/swcs.
        '''
        # Compare ports, swcs and connectors on both compositions
        removed_swcs = set(self.swcs) - set(composition.swcs)
        added_swcs = set(composition.swcs) - set(self.swcs)

        port_map = [p for k, p in self.ports]
        comp_port_map = [v for i, v in composition.ports]

        removed_ports = set(port_map) - set(comp_port_map)
        added_ports = set(comp_port_map) - set(port_map)

        # removed_ports = [p.shortname for p in removed_portinterfaces]

        # added_ports = [p.shortname for p in added_portinterfaces]

        return {'added_swcs': added_swcs, 'removed_swcs': removed_swcs, 'added_ports': added_ports,
                'removed_ports': removed_ports}

    def updateContent(self, composition):
        '''
           Updates the content of the composition with the content of *composition*.

           This update will update:

           - Component prototypes
           - Assembly connectors
           - Delegation connectors

           By update it will add all new assembly connectors that are found and
           remove all that are unused in *composition*. The same behaviour when
           updating delegation ports.

           The matching of added removed is done by short-name of port-prototypes.

           :param composition: ARComposition to use when updating this composition.
        '''

        print('Adding new SWCs')
        for s in set(composition.swcs) - set(self.swcs):
            print('Adding SWC %s to composition %s' % (s, self.shortname))
            self.addSWC(s, composition.swcs[s])

        self_assembly = set(["_".join([s.provider_port.text, s.requester_port.text]) for k, s in self.assemblys])
        other_assembly = set(
            ["_".join([s.provider_port.text, s.requester_port.text]) for k, s in composition.assemblys])
        self_assembly_dict = dict(
            [("_".join([s.provider_port.text, s.requester_port.text]), s) for k, s in self.assemblys])
        other_assembly_dict = dict(
            [("_".join([s.provider_port.text, s.requester_port.text]), s) for k, s in composition.assemblys])

        connectors = self.xmlref.find(base.add_schema('CONNECTORS'))

        if connectors is None:
            # Create connectors package
            connectors = base.create_element('CONNECTORS').xmlref
            self.xmlref.append(connectors)

        print("Removing assemblys:")
        for s in self_assembly - other_assembly:
            print("%s (%s)" % (s, self_assembly_dict[s].shortname))
            connectors.remove(self_assembly_dict[s].xmlref)
            del (self.assemblys[self_assembly_dict[s].shortname])

        print("Added assemblys:")
        for s in other_assembly - self_assembly:
            print("%s (%s)" % (s, other_assembly_dict[s].shortname))
            new_assembly = other_assembly_dict[s]
            na_pcomp = new_assembly.provider_comp.text
            na_rcomp = new_assembly.requester_comp.text
            new_assembly.provider_comp.text = self.path + '/' + na_pcomp.split('/')[-1]
            new_assembly.requester_comp.text = self.path + '/' + na_rcomp.split('/')[-1]
            self.assemblys[new_assembly.shortname] = new_assembly
            connectors.append(new_assembly.xmlref)

        print("Updating assemblys:")
        for s in self_assembly & other_assembly:
            self_assembly_dict[s].setShortname(other_assembly_dict[s].shortname)
            self_assembly_dict[s].xmlref.attrib['UUID'] = other_assembly_dict[s].shortname

        self_delegation = set(["_".join([s.inner_port.text]) for k, s in self.delegations])
        other_delegation = set(["_".join([s.inner_port.text]) for k, s in composition.delegations])
        self_delegation_dict = dict([("_".join([s.inner_port.text]), s) for k, s in self.delegations])
        other_delegation_dict = dict([("_".join([s.inner_port.text]), s) for k, s in composition.delegations])

        print("Removing delegations:")
        for s in self_delegation - other_delegation:
            print("%s (%s)" % (s, self_delegation_dict[s].shortname))
            connectors.remove(self_delegation_dict[s].xmlref)
            del (self.delegations[self_delegation_dict[s].shortname])

        print("Added delegations:")
        for s in other_delegation - self_delegation:
            print("%s (%s)" % (s, other_delegation_dict[s].shortname))
            new_delegation = other_delegation_dict[s]
            nd_op = new_delegation.outer_port.text
            nd_ic = new_delegation.inner_comp.text
            new_delegation.outer_port.text = self.path + '/' + nd_op.split('/')[-1]
            new_delegation.inner_comp.text = self.path + '/' + nd_ic.split('/')[-1]
            self.delegations[new_delegation.shortname] = new_delegation
            connectors.append(new_delegation.xmlref)

        print("Updating delegations:")
        for s in self_delegation & other_delegation:
            self_delegation_dict[s].setShortname(other_delegation_dict[s].shortname)
            self_delegation_dict[s].xmlref.attrib['UUID'] = other_delegation_dict[s].shortname

    def getSWCPrototypes(self):
        '''
           Get all component prototypes from this composition.

           :return: List of all swc-prototypes as AUTOSAR Paths.
        '''
        if self.path is not None:
            return [self.path + '/' + s for s in self.swcs]
        else:
            return None

    def addPortGroups(self, pg):
        '''
           Add additional port-group to composition.
           Check that ports in port-group exists in composition, otherwise remove them.

           :param pg: Port Groups XML to add
           :return: Nothing
        '''
        for op in xml_helper.findall(pg, "OUTER-PORTS"):
            for pp in op.findall(base.add_schema('PORT-PROTOTYPE-REF-CONDITIONAL')):
                pname = pp.find(base.add_schema("PORT-PROTOTYPE-REF"))
                ptext = pname.text.split("/")[-1]
                if not ptext in self.ports:
                    op.remove(pp)
                else:
                    if self.ports[ptext].__class__ == AR_RPort:
                        pname.attrib['DEST'] = 'R-PORT-PROTOTYPE'
                    else:
                        pname.attrib['DEST'] = 'P-PORT-PROTOTYPE'

        self.xmlref.append(pg)
        self.__loadPortGroups()


class ARSwc(base.ARObject):
    '''
       This class represents all Atomic Software components.
    '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)
        self.__loadBehavior()
        self.__loadPorts()

    def __loadBehavior(self):
        ''' Load internal behaviour '''
        behavior = self.xmlref.find(base.add_schema('INTERNAL-BEHAVIORS/SWC-INTERNAL-BEHAVIOR'))
        if behavior is not None:
            self.behavior_path = base.GetShortName(behavior)

            self.__loadEvents(behavior)
            self.__loadRunnables(behavior)
            self.__loadParameters(behavior)
            self.__loadExclusiveAreas(behavior)
            self.__loadServices(behavior)

    def __loadEvents(self, behavior):
        '''
           Load events available in Internal behaviour

           :param behavior: Internal behavior to check events in
        '''

        # Get all mode switch events ( NOTE:Assuming no Mode Transition events )
        me = behavior.findall('.//' + base.add_schema('SWC-MODE-SWITCH-EVENT'))
        self.mode_events = dict()  # type: ignore

        for e in me:
            shortname = e.findtext('./' + base.add_schema('SHORT-NAME'))
            event_ref = e.find(base.add_schema('START-ON-EVENT-REF'))
            mode_port = e.findtext(base.add_schema('MODE-IREFS/MODE-IREF/CONTEXT-PORT-REF'))
            target_mode = e.findtext(base.add_schema('MODE-IREFS/MODE-IREF/TARGET-MODE-DECLARATION-REF'))

            # print "%s[%s] (%s) = %s"%(shortname, event_ref, mode_port, target_mode)
            self.mode_events[shortname] = (event_ref.text, mode_port, target_mode)

            maps.addReference(event_ref.text, event_ref)

        # Get all timing events
        te = behavior.findall('.//' + base.add_schema('TIMING-EVENT'))
        self.timing_events = dict()  # type: ignore
        for e in te:
            shortname = e.findtext('./' + base.add_schema('SHORT-NAME'))
            event_ref = e.find(base.add_schema('START-ON-EVENT-REF'))
            period = e.findtext(base.add_schema('PERIOD'))

            mode_disabling = e.findall(base.add_schema('DISABLED-MODE-IREFS/DISABLED-MODE-IREF'))
            modes = list()
            for md in mode_disabling:
                cp = md.findtext(base.add_schema('CONTEXT-PORT-REF'))
                tm = md.findtext(base.add_schema('TARGET-MODE-DECLARATION-REF'))
                modes.append((cp, tm))

            # print "%s[%s] (%s) = %s"%(shortname, event_ref, modes, period)
            self.timing_events[shortname] = (event_ref.text, period, modes)
            maps.addReference(event_ref.text, event_ref)

        # Get all Operation Invoked Events
        oe = behavior.findall('.//' + base.add_schema('OPERATION-INVOKED-EVENT'))
        self.operation_events = dict()  # type: ignore
        for e in oe:
            shortname = e.findtext('./' + base.add_schema('SHORT-NAME'))
            event_ref = e.find(base.add_schema('START-ON-EVENT-REF'))
            operation_port = e.findtext(base.add_schema('OPERATION-IREF/CONTEXT-P-PORT-REF'))
            target_operation = e.findtext(base.add_schema('OPERATION-IREF/TARGET-PROVIDED-OPERATION-REF'))

            # print "%s[%s] (%s) = %s"%(shortname, event_ref, operation_port, target_operation)

            self.operation_events[shortname] = (event_ref.text, operation_port, target_operation)
            maps.addReference(event_ref.text, event_ref)

        # mode_events = behavior.findall('.//'+base.add_schema('SWC-MODE-SWITCH-EVENT')+'/'+base.add_schema('START-ON-EVENT-REF'))
        # timing_events = behavior.findall('.//'+base.add_schema('TIMING-EVENT')+'/'+base.add_schema('START-ON-EVENT-REF'))
        # periods = behavior.findall('.//'+base.add_schema('PERIOD'))

        # self.events = zip([t.text for t in timing_events],[s.text for s in periods])

        # for re in mode_events + timing_events:
        #    maps.addReference(re.text, re)


        mode_ports = behavior.findall('.//' + base.add_schema('CONTEXT-PORT-REF'))

        self.mode_port = None

        for mp in mode_ports:
            maps.addReference(mp.text, mp)
            self.mode_port = mp.text

        operation_invoked_event = behavior.findall('.//' + base.add_schema('CONTEXT-P-PORT-REF'))

        for opi in operation_invoked_event:
            maps.addReference(opi.text, opi)

    def __loadRunnables(self, behavior):
        '''
           Load all runnable entitys in Internal Behavior

           :param behavior: Internal behavior to check events in
        '''
        run = behavior.findall(base.add_schema('RUNNABLES/RUNNABLE-ENTITY'))
        self.runnables = dict()  # type: ignore
        for r in run:
            self.runnables[base.GetShortName(r)] = r
            # print base.GetShortName(r)
            ports = r.findall('.//' + base.add_schema('PORT-PROTOTYPE-REF'))
            for p in ports:
                # print "SWC Port Ref: %s"%(p.text)
                maps.addReference(p.text, p)

            param_access = r.findall('.//' + base.add_schema('LOCAL-PARAMETER-REF'))
            for pa in param_access:
                maps.addReference(pa.text, pa)

            # Add EA reference
            exclusive_area_refs = r.findall('.//' + base.add_schema('RUNS-INSIDE-EXCLUSIVE-AREA-REF'))
            for ea_ref in exclusive_area_refs:
                maps.addReference(ea_ref.text, ea_ref)

            # Add operation reference
            operation_irefs = r.findall('.//' + base.add_schema('CONTEXT-R-PORT-REF'))
            for op_ref in operation_irefs:
                maps.addReference(op_ref.text, op_ref)
            variable_access = r.findall('.//' + base.add_schema('TARGET-DATA-PROTOTYPE-REF'))
            for va_ref in variable_access:
                maps.addReference(va_ref.text, va_ref)

    def __loadParameters(self, behavior):
        '''
           Load all Consants, variables and parameters in this SWC.

           :param behavior: Internal behavior of this SWC.
        '''
        params = behavior.findall(base.add_schema('SHARED-PARAMETERS/PARAMETER-DATA-PROTOTYPE'))
        static_variables = behavior.findall(base.add_schema('STATIC-MEMORYS/VARIABLE-DATA-PROTOTYPE'))
        pim_variables = behavior.findall(base.add_schema('AR-TYPED-PER-INSTANCE-MEMORYS/VARIABLE-DATA-PROTOTYPE'))

        self.parameters = dict()  # type: ignore
        self.variables = dict()  # type: ignore
        self.pims = dict()  # type: ignore

        for p in params:
            self.parameters[base.GetShortName(p)] = p
            c = p.find('.//' + base.add_schema('CONSTANT-REF'))
            if c is not None:
                maps.addReference(c.text, c)

            t = p.findall('.//' + base.add_schema('TYPE-TREF'))
            maps.addReferenceList(t)
        # if t is not None:
        #    maps.addReference(c.text, c)

        for sv in static_variables:
            self.variables[base.GetShortName(sv)] = sv
            c = sv.find('.//' + base.add_schema('CONSTANT-REF'))
            if c is not None:
                maps.addReference(c.text, c)

        for pim in pim_variables:
            self.pims[base.GetShortName(pim)] = pim
            t = pim.findall('.//' + base.add_schema('TYPE-TREF'))
            maps.addReferenceList(t)

    def __loadExclusiveAreas(self, behavior):
        '''
           Load all exclusive areas in this SWC (EXCLUSIVE-AREA)

           :param behavior: Internal behavior of this SWC
        '''
        eas = behavior.findall(base.add_schema('EXCLUSIVE-AREAS/EXCLUSIVE-AREA'))
        self.exclusive_areas = dict()  # type: ignore

        for e in eas:
            self.exclusive_areas[base.GetShortName(e)] = e

    def __loadServices(self, behavior):
        '''
           Load all local parameters and variables.

           :param behavior: Internal behavior of this SWC
        '''
        self.serv_deps = dict(
            [(base.GetShortName(v), ARServiceDependency(v, self.path + base.GetShortName(v), self.arxml)) for v in
             behavior.findall('.//' + base.add_schema('SWC-SERVICE-DEPENDENCY'))])

        param_refs = behavior.findall('.//' + base.add_schema('LOCAL-PARAMETER-REF'))
        variable_refs = behavior.findall('.//' + base.add_schema('LOCAL-VARIABLE-REF'))

        for p in variable_refs + param_refs:
            maps.addReference(p.text, p)

    def __loadPorts(self):
        '''
           Load all Port Prototypes.

           Store them in self.ports.
        '''
        self.ports = dict()  # type: Dict[str, typing.Union[AR_RPort, AR_PPort]]
        r_ports = self.xmlref.findall(base.add_schema('PORTS/R-PORT-PROTOTYPE'))
        for r in r_ports:
            self.ports[base.GetShortName(r)] = AR_RPort(r, '%s/%s' % (self.path, base.GetShortName(r)), self.arxml)

        p_ports = self.xmlref.findall(base.add_schema('PORTS/P-PORT-PROTOTYPE'))
        for p in p_ports:
            self.ports[base.GetShortName(p)] = AR_PPort(p, '%s/%s' % (self.path, base.GetShortName(p)), self.arxml)

    def __updateReferences(self, old_path, new_path, elem_list):
        '''
           Internal helper function to update references of moved component.

           :param old_path: Previous path of component
           :param new_path: New path of component
           :param elem_list: List of subelements to update.
        '''
        for prot in elem_list:
            o_path = old_path + '/' + prot
            n_path = new_path + '/' + prot
            if o_path in maps.references and o_path != n_path:
                if options.verbose:
                    print("Updating SWC %s -> %s" % (o_path, n_path))
                maps.updateReference(o_path, n_path)
                # for r in maps.references[o_path]:
                #    r.text = n_path

    def updateReferences(self, old_path):
        '''
           Update references to this SWC and all prototypes in this SWC when moved.

           :param old_path: Previous path of SWC.
        '''
        self.__updateReferences(old_path, self.path, self.ports)
        self.__updateReferences(old_path + '/' + self.behavior_path, self.path + '/' + self.behavior_path,
                                self.runnables)
        self.__updateReferences(old_path + '/' + self.behavior_path, self.path + '/' + self.behavior_path,
                                self.parameters)
        self.__updateReferences(old_path + '/' + self.behavior_path, self.path + '/' + self.behavior_path,
                                self.variables)
        self.__updateReferences(old_path + '/' + self.behavior_path, self.path + '/' + self.behavior_path, self.pims)
        self.__updateReferences(old_path + '/' + self.behavior_path, self.path + '/' + self.behavior_path,
                                self.exclusive_areas)
        self.__updateReferences(old_path, self.path, [self.behavior_path])

        # def addImplementation(self, impelemntation):


class ARSwcImplementation(base.ARObject):
    '''
       This class represents SWC-IMPLEMENTATION element.

    '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        self.behavior_ref = element.find(base.add_schema('BEHAVIOR-REF'))

        maps.addReference(self.behavior_ref.text, self.behavior_ref)

    def updateReferences(self, old_path):
        '''
           Update references to this ARSwcImplementation when moved.
        '''
        o_path = old_path
        n_path = self.path
        if o_path != n_path and o_path in maps.references:
            if options.verbose:
                print("Updating SWC Implementation %s -> %s" % (o_path, n_path))
            maps.updateReference(o_path, n_path)
            # for r in maps.references[o_path]:
            #    r.text = n_path


class ARPortInterface(base.ARObject):
    '''
       This class represents PORT-INTERFACE element.
    '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        self.data_elements = dict()  # type: Dict[str, str]

        maps.addReferenceList(element.findall(
            base.add_schema('OPERATIONS/CLIENT-SERVER-OPERATION/ARGUMENTS/ARGUMENT-DATA-PROTOTYPE/TYPE-TREF')))

        # Load dataelements
        for d in element.findall(base.add_schema('DATA-ELEMENTS/VARIABLE-DATA-PROTOTYPE')):
            sn = base.GetShortName(d)
            dt = d.findtext(base.add_schema('TYPE-TREF'))
            self.data_elements[sn] = dt
            maps.addReferenceList(element.findall('.//' + base.add_schema('TYPE-TREF')))
            # self.datatype_ref = element.findtext(base.add_schema('TYPE-TREF'))
            # elf.datatype_prototype = element.

    def getDesc(self):
        descs = self.xmlref.findall(base.add_schema('DATA-ELEMENTS/VARIABLE-DATA-PROTOTYPE/DESC'))
        return "\n".join(getChildTextRecursive(t) for t in descs)


class ARAssemblyConnector(base.ARObject):
    '''
       This class represents ASSEMBLY-CONNECTOR element.
    '''

    def __init__(self, element):
        base.ARObject.__init__(self, element, None, None)
        self.provider_comp = element.find(base.add_schema('PROVIDER-IREF/CONTEXT-COMPONENT-REF'))
        if self.provider_comp is not None:
            maps.addReference(self.provider_comp.text, self.provider_comp)
        self.requester_comp = element.find(base.add_schema('REQUESTER-IREF/CONTEXT-COMPONENT-REF'))
        if self.requester_comp is not None:
            maps.addReference(self.requester_comp.text, self.requester_comp)

        self.provider_port = element.find(base.add_schema('PROVIDER-IREF/TARGET-P-PORT-REF'))
        if self.provider_port is not None:
            maps.addReference(self.provider_port.text, self.provider_port)
        self.requester_port = element.find(base.add_schema('REQUESTER-IREF/TARGET-R-PORT-REF'))
        if self.requester_port is not None:
            maps.addReference(self.requester_port.text, self.requester_port)


class ARDelegationConnector(base.ARObject):
    '''
       This class represents DELEGATION-CONNECTOR element.
    '''

    def __init__(self, element):
        base.ARObject.__init__(self, element, None, None)
        self.inner_comp = element.find(
            base.add_schema('INNER-PORT-IREF/P-PORT-IN-COMPOSITION-INSTANCE-REF/CONTEXT-COMPONENT-REF'))
        self.inner_port = element.find(
            base.add_schema('INNER-PORT-IREF/P-PORT-IN-COMPOSITION-INSTANCE-REF/TARGET-P-PORT-REF'))
        if self.inner_comp == None:
            self.inner_comp = element.find(
                base.add_schema('INNER-PORT-IREF/R-PORT-IN-COMPOSITION-INSTANCE-REF/CONTEXT-COMPONENT-REF'))
            self.inner_port = element.find(
                base.add_schema('INNER-PORT-IREF/R-PORT-IN-COMPOSITION-INSTANCE-REF/TARGET-R-PORT-REF'))

        self.outer_port = element.find(base.add_schema('OUTER-PORT-REF'))

        # if self.inner_comp is None or self.inner_port is None:
        #    print self.outer_port.text
        #    return
        maps.addReference(self.inner_comp.text, self.inner_comp)
        maps.addReference(self.inner_port.text, self.inner_port)
        maps.addReference(self.outer_port.text, self.outer_port)


class AR_RPort(base.ARObject):
    '''
       This class represents a R-PORT-PROTOTYPE within a ARSwc or ARComposition.
    '''

    def __init__(self, element, path, arxml):
        base.ARObject.__init__(self, element, path, arxml)

        # Load interface reference
        self.port_interface = element.find(base.add_schema('REQUIRED-INTERFACE-TREF'))

        if self.port_interface is not None:
            maps.addReference(self.port_interface.text, self.port_interface)

        # Load required comspecs
        const_ref = element.findall('.//' + base.add_schema('CONSTANT-REF'))
        if len(const_ref) > 0:
            try:
                self.initvalue = arxml.constants[const_ref[0].text].getValue()
            except KeyError:
                self.initvalue = (None, None)
        else:
            self.initvalue = (None, None)
        try:
            self.initvalue = arxml.constants[const_ref[0].text].getValue()
        except:
            # No comspec exists on the port
            self.initvalue = None

        if const_ref is not None:
            for c in const_ref:
                maps.addReference(c.text, c)

    def getDataElements(self):
        try:
            return self.arxml.port_interfaces[self.port_interface.text].data_elements
        except:
            return None

    def __repr__(self):
        if self.port_interface is None:
            port = 'None'
        else:
            port = self.port_interface.text

        return 'AR_RPort: %s(%s)' % (self.shortname, port)

    '''
        Compare instances of ports
    '''

    def __eq__(self, other):
        if isinstance(other, AR_RPort):
            if self.port_interface.text == other.port_interface.text:
                return True

        return False

    def __hash__(self):
        h = 7654321

        h += hash(self.shortname)

        if self.port_interface is not None:
            # print "ERROR1"
            h += hash(self.port_interface.text)

        return h


class AR_PPort(base.ARObject):
    '''
       This class represents a P-PORT-PROTOTYPE within a ARSwc or ARComposition.
    '''

    def __init__(self, element, path, arxml):
        base.ARObject.__init__(self, element, path, arxml)

        # Load interface reference
        self.port_interface = element.find(base.add_schema('PROVIDED-INTERFACE-TREF'))
        if self.port_interface is not None:
            maps.addReference(self.port_interface.text, self.port_interface)

        # Load required comspecs
        const_ref = element.findall('.//' + base.add_schema('CONSTANT-REF'))
        if len(const_ref) > 0:
            try:
                self.initvalue = arxml.constants[const_ref[0].text].getValue()
            except KeyError:
                self.initvalue = (None, None)
        else:
            self.initvalue = (None, None)
        try:
            self.initvalue = arxml.constants[const_ref[0].text].getValue()
        except:
            # No initvalue exists on the port
            self.initvalue = None

        if const_ref is not None:
            for c in const_ref:
                maps.addReference(c.text, c)

    def getDataElements(self):
        try:
            return self.arxml.port_interfaces[self.port_interface.text].data_elements
        except:
            return None

    def __repr__(self):
        if self.port_interface is None:
            port = 'None'
        else:
            port = self.port_interface.text

        return 'AR_PPort: %s(%s)' % (self.shortname, port)

    '''
        Compare instances of ports
    '''

    def __eq__(self, other):
        if isinstance(other, AR_PPort):
            if self.port_interface.text == other.port_interface.text:
                return True

        return False

    def __hash__(self):
        h = 1234567

        h += hash(self.shortname)

        if self.port_interface is not None:
            # print "ERROR2"
            h += hash(self.port_interface.text)

        return h


class ARSystem(base.ARObject):
    '''
       This class represents a SYSTEM element.
    '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        self.__loadFibexRefs()
        self.__loadMappings()
        self.__loadRootComposition()

    def __loadFibexRefs(self):
        '''
           Load all fibex references
        '''
        fibexrefs = self.xmlref.findall(
            base.add_schema('FIBEX-ELEMENTS/FIBEX-ELEMENT-REF-CONDITIONAL/FIBEX-ELEMENT-REF'))

        for f in fibexrefs:
            maps.addReference(f.text, f)

    def __loadRootComposition(self):
        '''
           Load Root Composition prototype and type.
        '''
        root_component = self.xmlref.find(base.add_schema('ROOT-SOFTWARE-COMPOSITIONS/ROOT-SW-COMPOSITION-PROTOTYPE'))
        if root_component is None:  # MATS
            return
        self.root_prototype = base.GetShortName(root_component)

        ref = root_component.find(base.add_schema('SOFTWARE-COMPOSITION-TREF'))

        self.root_type = ref.text
        self.root_type_elem = ref

        maps.addReference(ref.text, ref)

    def __loadMappings(self):
        '''
           Load all signal/signal-group/pnc/vfc mappings in ARSystem.
        '''

        self.mapped_datatypes = dict()  # type: Dict[str, ET.Element]
        self.mapped_signalsToPort = dict()  # type: Dict[str, ET.Element]
        self.mapped_signalsToElement = dict()  # type: Dict[str, ET.Element]

        sr_signal_mappings = self.xmlref.findall(
            base.add_schema('MAPPINGS/SYSTEM-MAPPING/DATA-MAPPINGS/SENDER-RECEIVER-TO-SIGNAL-MAPPING'))

        for sr in sr_signal_mappings:
            context_comp = sr.find(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-COMPOSITION-REF'))
            context_swc = sr.find(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-COMPONENT-REF'))
            context_port = sr.find(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-PORT-REF'))
            target_data = sr.find(base.add_schema('DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF'))
            system_signal = sr.find(base.add_schema('SYSTEM-SIGNAL-REF'))
            self.mapped_signalsToPort[system_signal.text] = context_port
            self.mapped_datatypes[target_data.text] = target_data

            maps.addReference(context_comp.text, context_comp)
            if context_swc is not None:
                maps.addReference(context_swc.text, context_swc)
            maps.addReference(context_port.text, context_port)
            maps.addReference(target_data.text, target_data)
            maps.addReference(system_signal.text, system_signal)

        sr_signal_group_mappings = self.xmlref.findall(
            base.add_schema('MAPPINGS/SYSTEM-MAPPING/DATA-MAPPINGS/SENDER-RECEIVER-TO-SIGNAL-GROUP-MAPPING'))

        for sr in sr_signal_group_mappings:
            context_comp = sr.find(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-COMPOSITION-REF'))
            context_swc = sr.find(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-COMPONENT-REF'))
            context_port = sr.find(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-PORT-REF'))
            target_data = sr.find(base.add_schema('DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF'))
            system_group = sr.find(base.add_schema('SIGNAL-GROUP-REF'))

            maps.addReference(context_comp.text, context_comp)
            if context_swc is not None:
                maps.addReference(context_swc.text, context_swc)
            maps.addReference(context_port.text, context_port)
            maps.addReference(target_data.text, target_data)
            maps.addReference(system_group.text, system_group)

            # Type mapping
            record_elements = sr.findall(base.add_schema(
                'TYPE-MAPPING/SENDER-REC-RECORD-TYPE-MAPPING/RECORD-ELEMENT-MAPPINGS/SENDER-REC-RECORD-ELEMENT-MAPPING'))

            for r in record_elements:
                appl_record = r.find(base.add_schema('APPLICATION-RECORD-ELEMENT-REF'))
                system_signal = r.find(base.add_schema('SYSTEM-SIGNAL-REF'))
                self.mapped_signalsToPort[system_signal.text] = context_port
                self.mapped_signalsToElement[system_signal.text] = appl_record
                maps.addReference(appl_record.text, appl_record)
                maps.addReference(system_signal.text, system_signal)

        pnc_mappings = self.xmlref.findall(base.add_schema('MAPPINGS/SYSTEM-MAPPING/PNC-MAPPINGS/PNC-MAPPING'))

        self.pg_to_vfc_mapping = dict()  # type: Dict[str, typing.Tuple[ET.Element, ET.Element]]

        for p in pnc_mappings:
            group_refs = p.findall(base.add_schema('PNC-GROUP-REFS/PNC-GROUP-REF'))
            for g in group_refs:
                maps.addReference(g.text, g)

            vfc = p.find(base.add_schema('VFC-IREFS'))
            vfcs = p.findall(base.add_schema('VFC-IREFS/VFC-IREF'))
            for v in vfcs:
                target = v.find(base.add_schema('TARGET-REF'))
                if target is not None:
                    self.pg_to_vfc_mapping[target.text] = (vfc, v)
                    maps.addReference(target.text, target)

            vfc_context = p.findall(base.add_schema('VFC-IREFS/VFC-IREF/CONTEXT-COMPOSITION-REF'))
            # vfc_target = p.findall(base.add_schema('VFC-IREFS/VFC-IREF/TARGET-REF'))


            maps.addReferenceList(vfc_context)
        # maps.addReferenceList(vfc_target)

        # SWC to ECU mapping
        #         cref = swc_mapping.find(base.add_schema('COMPONENT-IREFS'))
        swc_mapping = self.xmlref.findall(base.add_schema(
            'MAPPINGS/SYSTEM-MAPPING/SW-MAPPINGS/SWC-TO-ECU-MAPPING/COMPONENT-IREFS/COMPONENT-IREF/TARGET-COMPONENT-REF'))
        for sm in swc_mapping:
            maps.addReference(sm.text, sm)

    def updateReferences(self, old_path):
        '''
           Update all references to this ARSystem element.

           :param old_path: Previous path of SYSTEM element

           NOTE: Path is AUTOSAR path.
        '''
        # Update references to root-sw-compostition
        op_root = old_path + '/' + self.root_prototype
        new_root = self.path + '/' + self.root_prototype

        maps.updateReference(op_root, new_root)

    def setRootComposition(self, new_comp, old_comp):
        '''
           Update root composition in system and replace prototypes with root composition.

           :param new_comp: ARComposition to set as new Root composition.
           :param old_comp: Reference to old ARComposition used as root.

           :return: Nothing
        '''
        self.__loadFibexRefs()
        self.__loadMappings()
        self.__loadRootComposition()

        print("Changing root composition from %s -> %s" % (old_comp.path, new_comp.path))

        try:
            maps.updateReference(self.path + '/' + self.root_prototype, self.path + '/' + new_comp.shortname)
            maps.updateReference(old_comp.path, new_comp.path)
        # for r in maps.references[self.path + '/' + self.root_prototype]:
        #    r.text = self.path + '/' + new_comp.shortname
        except KeyError:
            pass

        self.root_prototype = new_comp.shortname
        root_component = self.xmlref.find(
            base.add_schema('ROOT-SOFTWARE-COMPOSITIONS/ROOT-SW-COMPOSITION-PROTOTYPE/SHORT-NAME'))

        root_component.text = new_comp.shortname

        # Update references to roottype


        self.root_type_elem.text = new_comp.path

        for pg in new_comp.port_groups:
            try:
                old = old_comp.path + '/' + pg
                new = new_comp.path + '/' + pg
                print("Updating Port Group %s -> %s" % (old, new))
                maps.updateReference(old, new)
            # for r in maps.references[old]:
            #    r.text = new

            except KeyError:
                continue

        for pg in set(old_comp.port_groups) - set(new_comp.port_groups):
            print("Removing Port Group %s" % (pg))

            p, c = self.pg_to_vfc_mapping[old_comp.path + '/' + pg]
            p.remove(c)

        # Update component mapping in root swc prototype
        swc_mapping = self.xmlref.find(base.add_schema('MAPPINGS/SYSTEM-MAPPING/SW-MAPPINGS/SWC-TO-ECU-MAPPING'))
        cref = swc_mapping.find(base.add_schema('COMPONENT-IREFS'))

        sr_signal_mappings = self.xmlref.findall(
            base.add_schema('MAPPINGS/SYSTEM-MAPPING/DATA-MAPPINGS/SENDER-RECEIVER-TO-SIGNAL-MAPPING'))
        sr_signal_group_mappings = self.xmlref.findall(
            base.add_schema('MAPPINGS/SYSTEM-MAPPING/DATA-MAPPINGS/SENDER-RECEIVER-TO-SIGNAL-GROUP-MAPPING'))

        map_dict = dict()  # type: ignore
        for s in sr_signal_mappings + sr_signal_group_mappings:
            try:
                map_dict[s.findtext(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-COMPONENT-REF'))].append(
                    s.find(base.add_schema('DATA-ELEMENT-IREF')))
            except KeyError:
                map_dict[s.findtext(base.add_schema('DATA-ELEMENT-IREF/CONTEXT-COMPONENT-REF'))] = [
                    s.find(base.add_schema('DATA-ELEMENT-IREF'))]

        print("Updating Root composition component prototypes..")
        for c in cref:
            ref = c.findtext(base.add_schema('TARGET-COMPONENT-REF'))
            print("Removing %s component" % (ref))
            maps.updateReference(ref, self.path + '/' + self.root_prototype)
            # for r in maps.references[ref]:
            #    r.text = self.path + '/' + self.root_prototype
            cref.remove(c)

            for m in map_dict[ref]:
                m.remove(m.find(base.add_schema('CONTEXT-COMPONENT-REF')))

        context_comp = self.path + '/' + self.root_prototype

        for c in new_comp.swcs:
            comp_name = new_comp.path + '/' + c
            print("Adding %s component" % (comp_name))
            e = base.create_element('COMPONENT-IREF')
            e.addPropertyElement(base.create_element('CONTEXT-COMPOSITION-REF', value=context_comp,
                                                     attrib={'DEST': 'ROOT-SW-COMPOSITION-PROTOTYPE'}))
            e.addPropertyElement(
                base.create_element('TARGET-COMPONENT-REF', value=comp_name, attrib={'DEST': 'SW-COMPONENT-PROTOTYPE'}))
            cref.append(e.xmlref)
            # maps.addReference(elem.xmlref, comp_name)


class AREndToEndProtection(base.ARObject):
    ''' This class represents END-TO-END-PROTECTION-SET. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)


class ARDatatype(base.ARObject):
    ''' This class represents arbitrary Application Datatype. '''

    def __init__(self, element, path, fileref):
        t = element.findall('.//' + base.add_schema('TYPE-TREF'))
        maps.addReferenceList(t)
        t = element.findall(".//" + base.add_schema('COMPU-METHOD-REF'))
        maps.addReferenceList(t)
        t = element.findall(".//" + base.add_schema('DATA-CONSTR-REF'))
        maps.addReferenceList(t)
        base.ARObject.__init__(self, element, path, fileref)

    def getCategory(self):
        # category = self.xmlref.findtext(".//"+base.add_schema('CATEGORY'))
        # return category
        return self.getSubElem('CATEGORY').text

    def getCompuMethod(self):
        compuMethodPath = self.xmlref.findtext(".//" + base.add_schema('COMPU-METHOD-REF'))
        if compuMethodPath in self.arxml.compu_methods:
            return self.arxml.compu_methods[compuMethodPath]
        else:
            return None

    def getDataConstraint(self):
        constrPath = self.getSubElem2('DATA-CONSTR-REF')

        if constrPath is not None and constrPath.text in self.arxml.data_constr:
            return self.arxml.data_constr[constrPath.text]
        else:
            return None

    def updateReferences(self, old_path):
        maps.updateReference(old_path, self.path)

    def getSwBaseType(self):
        swbasetypes = self.getAllSubElems2('ADMIN-DATA/SDGS/SDG/SDX-REF')
        for e in swbasetypes:
            if e.get('DEST') == 'SW-BASE-TYPE':
                return self.arxml.swbasetype[e.text]
        return None

    def getDesc(self):
        descs = self.getAllSubElems('DESC')
        return "\n".join(getChildTextRecursive(t) for t in descs)


class ARArrayDatatype(ARDatatype):
    def getElementType(self):
        shortname = self.getSubElem('ELEMENT/SHORT-NAME').text
        category = self.getSubElem('ELEMENT/CATEGORY').text
        datatype = self.getSubElem('ELEMENT/TYPE-TREF').text
        max_elems = self.getSubElem('ELEMENT/MAX-NUMBER-OF-ELEMENTS').text
        semantics = self.getSubElem('ELEMENT/ARRAY-SIZE-SEMANTICS').text

        return {'shortname': shortname, 'category': category, 'datatype': datatype, 'max_elems': max_elems,
                'semantics': semantics}

    def getDataType(self):
        d = self.getElementType()
        datatyperef = d["datatype"]
        if datatyperef in self.arxml.datatypes:
            return self.arxml.datatypes[datatyperef]
        else:
            return None


class ARRecordDatatype(ARDatatype):
    ''' This class represents arbitrary Application Record Datatypes. '''

    def getElements(self):
        elements = self.getAllSubElems('ELEMENTS/APPLICATION-RECORD-ELEMENT')

        return [ARRecordElement(e, '%s/%s' % (self.path, base.GetShortName(e)), self.arxml) for e in elements]


# def getRecordElements(self):
#         rElements = self.xmlref.findall(".//"+base.add_schema('APPLICATION-RECORD-ELEMENT'))
#         rElist = list();
#         a = arxml.get_elem_dict(self, self.parents, 'APPLICATION-RECORD-ELEMENT', ARRecordElements, self)
#         for rE in rElements:
#
#             if rE in self.arxml.recordElements:
#                 rElist.append(self.arxml.recordElements[rE])
#         return rElist

class ARRecordElement(ARDatatype):
    ''' This class represents arbitrary Application Datatype. '''

    def getType(self):
        return self.getSubElem('TYPE-TREF').text

    def getDataType(self):
        datatyperef = self.getSubElem('TYPE-TREF').text
        if datatyperef in self.arxml.datatypes:
            return self.arxml.datatypes[datatyperef]
        else:
            return None
            # def __init__(self, element, path, fileref):
            #    base.ARObject.__init__(self, element, path, fileref)


            # def getCategory(self):
            #    category = self.xmlref.findtext(".//"+base.add_schema('CATEGORY'))
            #    return category

            # def getCompuMethod(self):
            #    compuMethodPath = self.xmlref.findtext(".//"+base.add_schema('COMPU-METHOD-REF'))
            #    if compuMethodPath in self.arxml.compu_methods:
            #        return self.arxml.compu_methods[compuMethodPath]
            #    else:
            #        return None


class ARImplDatatype(base.ARObject):
    ''' This class represents arbitrary Implementation Datatype. '''

    def __init__(self, element, path, fileref):
        maps.addReferenceList(element.findall('.//' + base.add_schema('TYPE-TREF')))
        maps.addReferenceList(element.findall('.//' + base.add_schema('BASE-TYPE-REF')))
        maps.addReferenceList(element.findall(".//" + base.add_schema('COMPU-METHOD-REF')))
        maps.addReferenceList(element.findall(".//" + base.add_schema('DATA-CONSTR-REF')))
        base.ARObject.__init__(self, element, path, fileref)


class ARDataConstraint(base.ARObject):
    ''' This class represents a Data Containt element. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

    def getLimits(self):
        lower = self.getSubElem('DATA-CONSTR-RULES/DATA-CONSTR-RULE/PHYS-CONSTRS/LOWER-LIMIT').text
        upper = self.getSubElem('DATA-CONSTR-RULES/DATA-CONSTR-RULE/PHYS-CONSTRS/UPPER-LIMIT').text

        return (lower, upper)


class ARSwBaseType(base.ARObject):
    ''' This class represents a basic type. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

    def getBaseType(self):
        basetype = self.getSubElem('BASE-TYPE-ENCODING').text
        return basetype

    def getBaseTypeLength(self):
        btl = self.getSubElem('BASE-TYPE-SIZE')
        if btl is not None:
            return btl.text
        else:
            return '0'


class ARUnit(base.ARObject):
    ''' This class represents a Unit element. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)


class ARPhysDimension(base.ARObject):
    ''' This class represents a Unit element. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)


class ARCompuMethod(base.ARObject):
    ''' This class represent a Compu Method element. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        self.category = self.getSubElem('CATEGORY').text

    def getCompuNumerator(self):
        compuNumerator = self.xmlref.findall(".//" + base.add_schema('COMPU-NUMERATOR/V'))
        return [e.text for e in compuNumerator]

    def getCompuDenominator(self):
        compuDenominator = self.xmlref.findall(".//" + base.add_schema('COMPU-NUMERATOR/V'))
        return [e.text for e in compuDenominator]

    def getEnumerations(self):
        enumerations = self.xmlref.findall(".//" + base.add_schema('COMPU-SCALE'))

        return dict([(e.findtext(base.add_schema('COMPU-CONST/VT')),
                      (e.findtext(base.add_schema('LOWER-LIMIT')), e.findtext(base.add_schema('UPPER-LIMIT')))) for e in
                     enumerations])

    def setEnumeration(self, enumeration, values):
        ''' Set lower and upper limit of enumeration '''
        enumerations = self.xmlref.findall(".//" + base.add_schema('COMPU-SCALE'))

        for e in enumerations:
            if e.findtext(base.add_schema('COMPU-CONST/VT')) == enumeration:
                l, u = values
                e.find(base.add_schema('LOWER-LIMIT')).text = l
                e.find(base.add_schema('UPPER-LIMIT')).text = u

    def getLimits(self):
        lower = None
        upper = None
        if self.category == 'LINEAR':
            lower = self.getSubElem('COMPU-INTERNAL-TO-PHYS/COMPU-SCALES/COMPU-SCALE/LOWER-LIMIT')
            upper = self.getSubElem('COMPU-INTERNAL-TO-PHYS/COMPU-SCALES/COMPU-SCALE/UPPER-LIMIT')

        return (lower, upper)

    def getValues(self):
        v = self.xmlref.findall(".//" + base.add_schema('LOWER-LIMIT'))
        return [int(e.text) for e in v]

    def getCategory(self):
        # category = self.xmlref.findtext(".//"+base.add_schema('CATEGORY'))
        return self.category


class ARIsignalGroup(base.ARObject):
    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        isignalRefs = element.findall(
            './/' + base.add_schema('I-SIGNAL-REF'))  # self.getSubElem('I-SIGNAL-REFS/I-SIGNAL-REF').text
        maps.addReferenceList(isignalRefs)
        self.isignalList = [x.text for x in isignalRefs]


class ARIsignal(base.ARObject):
    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        self.length = self.getSubElem('LENGTH').text
        try:
            self.initialvalue = self.getSubElem('INIT-VALUE/NUMERICAL-VALUE-SPECIFICATION/VALUE').text
        except:
            self.initialvalue = None

        try:
            self.compumethodref = self.getSubElem(
                'NETWORK-REPRESENTATION-PROPS/SW-DATA-DEF-PROPS-VARIANTS/SW-DATA-DEF-PROPS-CONDITIONAL/COMPU-METHOD-REF').text
        except:
            self.compumethodref = None
        if (self.compumethodref and self.initialvalue) is not None:
            try:  # MATS
                compumethod = fileref.compu_methods[self.compumethodref]
                if compumethod.category == 'LINEAR':
                    self.applinitvalue = str(int(self.initialvalue) * float(compumethod.getCompuNumerator()[1]) + float(
                        compumethod.getCompuNumerator()[0]))
                else:
                    self.applinitvalue = None
            except:
                self.applinitvalue = None
        else:
            self.applinitvalue = None

        self.syssignal = self.getSubElem('SYSTEM-SIGNAL-REF').text


class ARSysSignal(base.ARObject):
    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)


class ARConstant(base.ARObject):
    ''' This class represents arbitrary CONSTANT in file. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        consts = element.findall('.//' + base.add_schema('CONSTANT-REF'))
        if consts is not None:
            for c in consts:
                maps.addReference(c.text, c)

    def getValue(self):
        value = ['0']
        signalName = list()
        if self.shortname == 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA':
            value = ['0']
            if options.verbose:
                print("Defalt constant found, setting 0 initvalue")

        else:
            valSpec = self.getSubElem('VALUE-SPEC/NUMERICAL-VALUE-SPECIFICATION')
            if valSpec == None:

                recValSpec = self.getSubElem('VALUE-SPEC/RECORD-VALUE-SPECIFICATION')
                if recValSpec == None:
                    arrayValSpec = self.getSubElem('VALUE-SPEC/ARRAY-VALUE-SPECIFICATION')
                    fields = self.xmlref.findall('.//' + base.add_schema('NUMERICAL-VALUE-SPECIFICATION'))
                    if len(fields) == 0:
                        value = ['0']
                        if options.verbose:
                            print("Generateted array constant found, setting 0")

                    else:
                        value = list()
                        for f in fields[:]:
                            try:
                                shortLabel = f.findall('.//' + base.add_schema('SHORT-LABEL'))[0].text
                            except:
                                shortLabel = None
                                if options.verbose:
                                    print("" + self.shortname + " lack SHORT-LABEL elements")

                            value.append(f.findall('.//' + base.add_schema('VALUE'))[0].text)
                            signalName.append(shortLabel)

                            # const_ref = f.findall('.//' + base.add_schema('CONSTANT-REF'))
                            # a =  arxml.constants[const_ref[0].text].getValue()
                else:
                    fields = self.xmlref.findall('.//' + base.add_schema('NUMERICAL-VALUE-SPECIFICATION'))
                    if len(fields) == 0:
                        value = ['0']
                        if options.verbose:
                            print("Generateted record constant found, setting 0")

                    else:
                        value = list()

                        for f in fields[:]:
                            shortLabel = f.findall('.//' + base.add_schema('SHORT-LABEL'))[0].text
                            value.append(f.findall('.//' + base.add_schema('VALUE'))[0].text)

                            signalName.append(shortLabel)
                            # const_ref = f.findall('.//' + base.add_schema('CONSTANT-REF'))
                            # a =  arxml.constants[const_ref[0].text].getValue()
            else:
                value = valSpec.findall('.//' + base.add_schema('VALUE'))[0].text

        return (signalName, value)


class ARServiceDependency(base.ARObject):
    ''' This class reperesents a SWC Service Dependency'''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)

        self.rolebased_data = {'ramBlock': [], 'defaultValue': []}  # type: ignore

        # Load datas
        for d in element.findall(base.add_schema('ASSIGNED-DATAS/ROLE-BASED-DATA-ASSIGNMENT')):
            role = d.findtext(base.add_schema('ROLE'))
            # Fix for faulty role in pre E42 SW
            if role == 'defaultData':
                role = 'defaultValue'

            variables = d.findall(base.add_schema('USED-DATA-ELEMENT/LOCAL-VARIABLE-REF'))
            parameters = d.findall(base.add_schema('USED-PARAMETER-ELEMENT/LOCAL-PARAMETER-REF'))

            self.rolebased_data[role].extend(variables)
            self.rolebased_data[role].extend(parameters)


class ARSigTrig(base.ARObject):
    ''' This class represents a Signal Triggering element. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)
        tmp = self.getSubElem('I-SIGNAL-PORT-REFS/I-SIGNAL-PORT-REF').text
        self.isOut = tmp[-4:] == "_Out"
        tmp2 = self.getSubElem('I-SIGNAL-REF')
        if tmp2 is not None:
            self.de = tmp2.text[25:]
        else:
            tmp2 = self.getSubElem('I-SIGNAL-GROUP-REF')
            self.de = tmp2.text[30:]


class ARGroupMap(base.ARObject):
    ''' This class represents a mapping signalgroup -> port. '''

    def __init__(self, element, path, fileref):
        base.ARObject.__init__(self, element, path, fileref)
        self.port = self.getSubElem('DATA-ELEMENT-IREF/TARGET-DATA-PROTOTYPE-REF').text
        tmpelem = self.getSubElem('SIGNAL-GROUP-REF')
        # this map is used to map from dataelement member to signal group member (also called group signal)
        self.groupsignalmap = dict()  # type: Dict[str, str]
        if tmpelem is not None:  # this is a group/dataelement
            self.rtename = tmpelem.text.split('/')[-1]
            self.isSignal = False
            # print("Group: " + self.rtename)
            records = self.getAllSubElems2(
                'TYPE-MAPPING/SENDER-REC-RECORD-TYPE-MAPPING/RECORD-ELEMENT-MAPPINGS/SENDER-REC-RECORD-ELEMENT-MAPPING')
            for r in records:
                key = ""
                value = ""
                for c in r.getchildren():
                    tag = c.tag.split('}')[-1]
                    if tag == 'APPLICATION-RECORD-ELEMENT-REF':
                        key = c.text.split('/')[-1]
                    if tag == 'SYSTEM-SIGNAL-REF':
                        value = c.text.split('/')[-1]
                if key is not None and value is not None:
                    self.groupsignalmap[key] = value

        else:
            tmpelem = self.getSubElem('SYSTEM-SIGNAL-REF')
            if tmpelem is not None:  # this is a signal
                self.rtename = tmpelem.text.split('/')[-1]
                self.isSignal = True
                # print("Signal: " + self.rtename)

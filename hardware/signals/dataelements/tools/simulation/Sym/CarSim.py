import Queue
import time
import Tkinter
import ttk
import tkFont
import json
import sys
import os
import dbus
import dbus.exceptions
import dbus.mainloop.glib
import thread

sys.path.append(os.path.join("../..", "AutosarCodeGen", "Autosar_Python_src"))
import autosar.arxml as arxml
import autosar.components
import traceback
from getDatatypes import getIntTypeStr
from gi.repository import GLib
from gi.repository import GObject



BG_COLOR_SENT = "#81f963"
BG_COLOR_INVALIDATED = "#fc931b"
BG_COLOR_NOT_SENT = "#8e8d8b"

def d2j(data):
    return json.dumps(data,encoding='utf-8')


def j2d(data):
    return  json.loads(data, encoding='utf-8')


class DESignalWidget(Tkinter.Frame):
    def __init__(self, master, **kw):
        Tkinter.Frame.__init__(self, master, **kw)
    def on_change(self):
        pass


class PortSender():
    #Note: This class is not a widget of its own, because the sub-widgets are commonly used in a grid view
    def __init__(self, info_label, send_button, stop_button, senderWidget, portName, dataElementName, isInsignal, first_col, autosend_bindvar):
        self.isInsignal = isInsignal
        self.autosend_bindvar = autosend_bindvar
        self.senderWidget = senderWidget
        self.infoBindVar = Tkinter.StringVar()
        self.infoBindVar.set("x")
        self.infoLabel = info_label
        self.infoLabel.configure(textvariable=self.infoBindVar, justify=Tkinter.LEFT)
        self.infoLabel.config(background=BG_COLOR_NOT_SENT)
        self.send_button = send_button
        self.send_button.configure(command=self.send)
        self.stop_button = stop_button
        self.stop_button.configure(command=self.set_signal_to_error)

        self.portName = portName
        self.senderWidget.on_change = lambda : self.on_change()
        self.dataElementName = dataElementName
        self.first_col = first_col

    def on_change(self):
        self.infoLabel.config(background=BG_COLOR_INVALIDATED)
        self.infoBindVar.set("x")
        if self.autosend_bindvar.get():
            self.send()

    def send(self):
        self.infoBindVar.set("Sent")
        self.infoLabel.config(background=BG_COLOR_SENT)
        toSend = {
            "state": 0,
            "timestamp": int(time.time()),
            "type": self.senderWidget.dataElementsDataType,
            "value": self.senderWidget.get_value()
        }
        self.write_dbus_property(toSend)

    def set_signal_to_error(self):
        self.infoBindVar.set("x")
        self.infoLabel.config(background=BG_COLOR_NOT_SENT)
        toSend = {
            "state": 1,
            "errorCode": 0,
            "timestamp": int(time.time()),
            "type": self.senderWidget.dataElementsDataType,
            "value": self.senderWidget.get_value()
        }

        self.write_dbus_property(toSend)

    def write_dbus_property(self,dataToSend):
        if self.dbus_conn:
            if self.isInsignal:
                signalsDBUSInterface = self.signalsInterfaceIn
            else:
                signalsDBUSInterface = self.signalsInterfaceOut

            signalsDBUSInterface.SetDESignal(self.dataElementName, d2j(dataToSend))
        else:
            print ("No DBUS connection in sender object")

    def setdbusconn(self,dbusconn):
        self.dbus_conn = dbusconn

    def setProxies(self,proxyIn,proxyOut):
        self.dBUSProxyIn = proxyIn
        self.dBUSProxyOut = proxyOut
        self.signalsInterfaceIn = dbus.Interface(proxyIn, 'com.ihu.VehicleSignalsManager.In')
        self.signalsInterfaceOut = dbus.Interface(proxyOut, 'com.ihu.VehicleSignalsManager.Out')

class BoolSender(DESignalWidget):
    def __init__(self, master, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = "bool"
        self.bindVar = Tkinter.IntVar()
        self.e = ttk.Checkbutton(self, width=10, variable=self.bindVar, command=lambda: self.on_change())
        self.e.pack()

    def get_value(self):
        return bool(self.bindVar.get())


class EnumSender(DESignalWidget):
    def __init__(self, master, compuMethod, dataElementsDataType, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = dataElementsDataType
        self.bindVar = Tkinter.StringVar()

        optionDict, defaultOption = enumCompuMethodToOptionMenus(compuMethod)
        self.optionDict = optionDict
        self.bindVar.set(defaultOption)
        options = (k for (k,v) in sorted(optionDict.iteritems(), key=lambda x: x[1]))
        e = ttk.OptionMenu(self, self.bindVar, defaultOption, *options, command=lambda x: self.on_change())
        e.pack()

    def get_value(self):
        return self.optionDict[self.bindVar.get()]


class NumericSender(DESignalWidget):
    def __init__(self, master, dataElementsDataType, arDataTypeValue, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = dataElementsDataType
        self.bindVar = Tkinter.StringVar()
        self.bindVar.set("0")
        self.e = ttk.Entry(self, width=10, textvariable=self.bindVar)
        self.e.bind("<Return>", lambda x: self.on_change())
        self.bindVar.trace("w", lambda name, index, mode: self.strvarchange())
        self.e.pack()

        min, max = arDataTypeValue.getDataConstraint().getLimits()

        self.bindVarInt = Tkinter.IntVar()
        self.bindVarInt.trace("w", lambda name, index, mode: self.intvarchange())
        self.scale = ttk.Scale(self, from_=min, to=max, orient=Tkinter.HORIZONTAL, variable=self.bindVarInt)
        self.scale.pack()

    def strvarchange(self):
        try:
            newIntVar = int(self.bindVar.get())
        except ValueError as v:
            return
        if newIntVar != self.bindVarInt.get():
            self.bindVarInt.set(newIntVar)
            self.on_change()
        else:
            # this means slider changed before entry
            self.on_change()

    def intvarchange(self):
        self.bindVar.set(str(self.bindVarInt.get()))

    def get_value(self):
        return int(self.bindVar.get())


def getDataElementsDatTypeFromCompuMethod(arDataTypeValue):
    compuMethod = arDataTypeValue.getCompuMethod()
    if compuMethod.getCategory() == 'IDENTICAL':
        lim = arDataTypeValue.getDataConstraint().getLimits()
        dataElementsDataType = getIntTypeStr(lim)
    elif compuMethod.getCategory() == 'LINEAR':
        lim = arDataTypeValue.getDataConstraint().getLimits()
        numerator = compuMethod.getCompuNumerator()
        dataElementsDataType = "double"
        if numerator[1] == '1.0':
            dataElementsDataType = getIntTypeStr(lim)
    else:
        dataElementsDataType = arDataTypeValue.shortname
    return dataElementsDataType


class StructSender(DESignalWidget):
    def __init__(self, master, dataElementsDataType, arDataTypeValue, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.subElements = {}   #Key: signal name, Value: SenderWidget
        self.dataElementsDataType = dataElementsDataType

        subElements = arDataTypeValue.getElements()
        structframe = self

        for subEl in subElements:
            if subEl.getCategory() == "BOOLEAN":
                subWidget = BoolSender(structframe)
            elif subEl.getCompuMethod().getCategory() == 'TEXTTABLE':
                subWidget = EnumSender(structframe, subEl.getCompuMethod(), None) #Data type is not used in sub elements
            else:
                subWidget = NumericSender(structframe, None, subEl) #Data type is not used in sub elements
            signalName = subEl.shortname

            dataElementsDataType = getDataElementsDatTypeFromCompuMethod(subEl.getDataType())

            if signalName != "Boolean" and dataElementsDataType == signalName:
                signalName += "_"

            self.subElements[signalName] = subWidget

            l = ttk.Label(structframe, text=subEl.shortname, justify=Tkinter.LEFT)
            l.pack(side=Tkinter.LEFT)
            subWidget.pack(side=Tkinter.LEFT)
            subWidget.on_change = lambda: self.on_change()

    def get_value(self):
        values = {}
        for signalName, widget in self.subElements.iteritems():
            values[signalName] = widget.get_value()
        return values


class SinkConnection:
    def __init__(self, bindableStringVariable, arDataTypeValue, value_label, signalname_label):
        self.arDataTypeValue = arDataTypeValue
        self.bindableStringVariable = bindableStringVariable
        self.value_label = value_label
        self.bg_fadeaway_ratio = 1
        self.signalname_label = signalname_label


class Sink:
    def __init__(self, arxmltype, sinkConnections, autosar_port_name, portname_label, struct_frame):
        self.arxmltype = arxmltype
        self.sinkConnections = sinkConnections
        self.autosar_port_name = autosar_port_name
        self.portname_label = portname_label
        self.struct_frame = struct_frame

class DataElement:
    def __init__(self, de_name, parent_port, datatype_key):
        self.de_name = de_name
        self.parent_port = parent_port
        self.datatype_key = datatype_key

def enumCompuMethodToOptionMenus(compuMethod):
    name_to_value_dict = compuMethod.getEnumerations()  # NOTE: Value here is a tuple
    value_to_name_dict = {v[0]: k for k, v in name_to_value_dict.items()}
    lookupDict = {}  # keys displayed to user with both name and value,

    default = None
    for intValue in compuMethod.getValues():
        ename = value_to_name_dict[str(intValue)].replace(compuMethod.shortname + '_', '')
        name_with_value = ename + " (%d)" % intValue
        lookupDict[name_with_value] = intValue
        if default is None:
            default = name_with_value
    return lookupDict, default


class App:
    def __init__(self, master):
        self.connected = False
        self.all_senders = []
        self.message_handlers = {}
        self.labelFont = tkFont.Font(root=master, family="Courier New", size=12)
        self.valueFont = tkFont.Font(root=master, family="Courier New", size=12, weight=tkFont.BOLD)

        self.knownReceivedMessages = {}
        self.addedSenderElements = set()

        arxmldata = arxml.load("../../AutosarCodeGen/Autosar_Python_src/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml")
        ihuports = {}

        self.element_name_to_data_element = {}

        #for dataTypeKey, arDataTypeValue in arxmldata.datatypes.iteritems():
            #print("type: %s, value: %s, category: %s" % (dataTypeKey, arDataTypeValue, arDataTypeValue.getCategory()))
        #print("_--------------------------------------------------------------------")
        for swcsKey, swcsValue in arxmldata.swcs.iteritems():
            if swcsValue.shortname != "IHU":
                continue
            #print("signal: %s, value: %s" % ( swcsKey, swcsValue))
            for portKey, port in sorted(swcsValue.ports.iteritems()):
                ihuports[portKey] = port
                elems = port.getDataElements()

                #firstElemKey, firstElemValue = elems.items()[0]
                #isInSignal = isinstance(port, autosar.components.AR_RPort)
                #if firstElemKey == portKey:
                #    print ("SAME: %s" % firstElemKey)
                #else:
                #    print ("NOT : isIn: %d port: '%s', elem: '%s'" % (isInSignal, portKey, firstElemKey))

                for de, dataTypeKey in sorted(elems.iteritems()):

                    self.element_name_to_data_element[de] = DataElement(de, port, dataTypeKey)

                    #print("  Port: %s : elems: %s, DataTypeKey: %s " %(portKey, de, dataTypeKey))
                    arDataTypeValue = arxmldata.datatypes[dataTypeKey]
                    category = arDataTypeValue.getCategory()
                    #if category == "STRUCTURE":
                    #    subElements = arDataTypeValue.getElements()
                    #    print("    structure %s" % ( ", ".join([r.shortname + " " + r.getCategory() for r in subElements]))   )
                    #elif category == "ARRAY":
                    #    print("    array")
                    #else:
                    #    print("    " + category)

        self.arxmldata = arxmldata
        self.ihuports = ihuports


        # Scroll widget only works on Canvas so we have to place the frame inside the canvas
        yScrollbar = Tkinter.Scrollbar(master)
        yScrollbar.pack(side=Tkinter.RIGHT, fill=Tkinter.Y)

        xScrollbar = Tkinter.Scrollbar(master, orient=Tkinter.HORIZONTAL)
        xScrollbar.pack(side=Tkinter.BOTTOM, fill=Tkinter.X)

        canvas = Tkinter.Canvas(master, yscrollcommand=yScrollbar.set, xscrollcommand=xScrollbar.set)

        masterFrame = ttk.Frame(canvas)
        masterFrame.pack(side=Tkinter.LEFT, fill=Tkinter.BOTH)

        canvas.pack(side=Tkinter.LEFT, fill=Tkinter.BOTH, expand=True)
        canvas.create_window((0, 0), window=masterFrame, anchor='nw')

        def resizecanvas(event):
            canvas.configure(scrollregion=canvas.bbox("all"))
        masterFrame.bind("<Configure>", resizecanvas)

        yScrollbar.config(command=canvas.yview)
        xScrollbar.config(command=canvas.xview)

        def on_mousewheel(event):
            canvas.yview_scroll(-1 * (event.delta / 120), "units")
        def on_mousewheel_horizontal(event):
            canvas.xview_scroll(-1 * (event.delta / 120), "units")
        def on_mousewheel_up(event):
            canvas.yview_scroll(-2, "units")
        def on_mousewheel_down(event):
            canvas.yview_scroll(2, "units")
        def on_mousewheel_left(event):
            canvas.xview_scroll(-2, "units")
        def on_mousewheel_right(event):
            canvas.xview_scroll(2, "units")

        canvas.bind_all("<MouseWheel>", on_mousewheel)     #windows
        canvas.bind_all("<Shift-MouseWheel>", on_mousewheel_horizontal)     #windows
        canvas.bind_all("<Button-4>", on_mousewheel_up)    #linux
        canvas.bind_all("<Button-5>", on_mousewheel_down)  #linux
        canvas.bind_all("<Shift-Button-4>", on_mousewheel_left)    #linux
        canvas.bind_all("<Shift-Button-5>", on_mousewheel_right)  #linux




        master = masterFrame
        self.master = masterFrame
        buttonRow = 0

        self.dbus_conn = None
        self.dBUSProxyIn = None
        self.dBUSProxyOut = None
        self.signalsInterfaceIn = None
        self.signalsInterfaceOut = None

        ## Connection status
        lConnectionStatus = ttk.Label(master, text="Connection status: ")
        lConnectionStatus.grid(row=buttonRow, column=0, sticky=Tkinter.W)

        self.bindConnectionStatus = Tkinter.Variable()
        self.bindConnectionStatus.set("disconnected")
        self.lActualConnectionStatus = ttk.Label(master, textvariable=self.bindConnectionStatus)
        self.lActualConnectionStatus .grid(row=buttonRow, column=1, sticky=Tkinter.W)
        self.lActualConnectionStatus.config(background="#ff0000")


        ## Connect to remote
        buttonRow += 1
        self.bConnectToRemote = ttk.Button(master, text="ConnectToRemote", command=self.connectToRemote)
        self.bConnectToRemote.grid(row=buttonRow, column=0, sticky=Tkinter.W)
        self.eConnectToRemote = ttk.Entry(master, width=20)
        self.eConnectToRemote.bind("<Return>", lambda x: self.connectToRemote())
        self.eConnectToRemote.insert(0, "198.18.34.1")
        self.eConnectToRemote.grid(row=buttonRow, column=1, sticky=Tkinter.W)


        ## Add from ARXML
        buttonRow += 1
        self.bAddSenderPort = ttk.Button(master, text="Add new Dataelement", command=self.ui_add_port)
        self.bAddSenderPort.grid(row=buttonRow, column=0, sticky=Tkinter.W)

        self.eAddSenderPort = ttk.Entry(master, width=30)
        self.eAddSenderPort.insert(0, "VehSpdLgt")
        self.eAddSenderPort.grid(row=buttonRow, column=1, sticky=Tkinter.W)



        ## Filter Entry
        buttonRow += 1

        def filterChanged(name, index, mode):
            self.filter(self.filterBindVar.get())

        self.filterLabel = ttk.Label(master, text="Filter")
        self.filterLabel.grid(row=buttonRow, column=0, sticky=Tkinter.W)

        self.filterBindVar = Tkinter.StringVar()
        self.filterBindVar.trace("w", filterChanged)
        self.eFilter = ttk.Entry(master, width=30, textvariable=self.filterBindVar)
        self.eFilter.grid(row=buttonRow, column=1, sticky=Tkinter.W)


        ## Auto send on change
        buttonRow += 1

        self.bindAutoSend = Tkinter.BooleanVar()
        self.bindAutoSend.set(True)
        self.autoSendLabel = ttk.Label(master, text="Auto send on change")
        self.autoSendLabel.grid(row=buttonRow, column=0, sticky=Tkinter.W)

        self.autoSendCheckbox = Tkinter.Checkbutton(master, width=10, variable=self.bindAutoSend)
        self.autoSendCheckbox.grid(row=buttonRow, column=1, sticky=Tkinter.W)


        ## Send all
        buttonRow += 1
        self.bSendAll = ttk.Button(master, width=30, text="Send all")
        self.bSendAll.configure(command=self.send_all)
        self.bSendAll.grid(row=buttonRow, column=0, sticky=Tkinter.W)


        #################### separator ##########################
        buttonRow += 1
        separator = ttk.Separator(master)
        separator.grid(row=buttonRow, sticky=Tkinter.EW)


        self.master = master
        self.buttonRow = buttonRow +1

        self.pending_messages = Queue.Queue()
        self.ever_connected = False

    def fade_timer(self):
        for portname, sink in self.knownReceivedMessages.iteritems():
            for sink in sink.sinkConnections:

                if sink.bg_fadeaway_ratio > 0:
                    sink.bg_fadeaway_ratio -= 0.02
                    if sink.bg_fadeaway_ratio < 0:
                        sink.value_label.configure(background="")
                        continue

                    startRGB = (226, 244, 58)
                    finalRGB = (142, 138, 135)
                    rgb = []
                    for sX, fX in zip(startRGB, finalRGB):
                        rgb.append(sX + (fX - sX) * sink.bg_fadeaway_ratio)

                    r, g, b = rgb
                    colorStr = "#%02x%02x%02x" % (r,g,b)
                    sink.value_label.configure(background=colorStr)

        self.master.after(60, self.fade_timer)

    def ui_add_port(self):
        self.add_sender_element(self.eAddSenderPort.get())
        #self.add_sender_port(self.eAddSenderPort.get())

    def connectToRemote(self):
        self.connect(self.eConnectToRemote.get())


    def connect(self, address):
        #TODO: Disconnect first? Reconnecting is a bit unstable
        print("Connecting to %s" % address)

        # DBUS, "tcp:host=198.18.34.1,port=55556"
        if address == "system":
            connectStr = "system"
        else:
            connectStr = "tcp:host=" + address + ",port=55556"
        try:
            if not self.ever_connected:
                self.ever_connected = True
                # Delay the fade timer because at start up ALL signals will be new
                # and this causes the program to run slow

                #GObject.threads_init()
                dbus.mainloop.glib.threads_init()
                dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

                self.master.after(5000, self.fade_timer)
                self.master.after(1000, self.incoming_message_dispatcher)
                thread.start_new_thread(self.startDBUSThread, (None,))

            if connectStr == "system":
                self.dbus_conn = dbus.SystemBus()
            else:
                self.dbus_conn=dbus.bus.BusConnection(connectStr)
            self.dbus_conn.call_on_disconnection(self.onDisconnect)

            self.bindConnectionStatus.set("Connected - " + address)
            self.lActualConnectionStatus.config(background="#00ff00")
            self.connected = True
            self.enable_all_buttons(True)

            print("Connected to " + address)
            self.fetchallDBUSProp()

            self.dBUSProxyIn = self.dbus_conn.get_object('com.ihu.VehicleSignalsManager.In', '/com/ihu/VehicleSignalsManager/In')
            self.dBUSProxyIn.connect_to_signal("DESignalChanged", self.deinsignalchanged_handler,
                                        dbus_interface="com.ihu.VehicleSignalsManager.In")

            self.dBUSProxyOut = self.dbus_conn.get_object('com.ihu.VehicleSignalsManager.Out',
                                                  '/com/ihu/VehicleSignalsManager/Out')
            self.dBUSProxyOut.connect_to_signal("DESignalChanged", self.deoutsignalchanged_handler,
                                        dbus_interface="com.ihu.VehicleSignalsManager.Out")

            self.signalsInterfaceIn = dbus.Interface(self.dBUSProxyIn, 'org.freedesktop.DBus.Properties')
            self.signalsInterfaceOut = dbus.Interface(self.dBUSProxyOut, 'org.freedesktop.DBus.Properties')

            self.update_all_sender_objects()
#            thread.start_new_thread(self.startDBUSThread, (None,))

        except(Exception):
            raise

    def onDisconnect(self,self2):
        print("disconnected")
        self.bindConnectionStatus.set("disconnected")
        self.lActualConnectionStatus.config(background="#ff0000")
        self.dbus_conn = None
        self.connected = False
        self.enable_all_buttons(False)
        self.dBUSProxyIn = None
        self.dBUSProxyOut = None
        self.signalsInterfaceIn = None
        self.signalsInterfaceOut = None

    def startDBUSThread(self,Ignore):
        loop = GLib.MainLoop()
        loop.run()

    def deinsignalchanged_handler(self, signalStr):
        signalData = self.readSignalDataProxy(signalStr, self.dBUSProxyIn, self.signalsInterfaceIn, 'com.ihu.VehicleSignalsManager.In')
        self.on_message_dbus(signalStr,signalData)

    def deoutsignalchanged_handler(self, signalStr):
        signalData = self.readSignalDataProxy(signalStr, self.dBUSProxyOut, self.signalsInterfaceOut, 'com.ihu.VehicleSignalsManager.Out')
        self.on_message_dbus(signalStr, signalData)

    #DBUS
    def fetchallDBUSProp(self):
        self.fetchDBUSPropForInterface('com.ihu.VehicleSignalsManager.In','/com/ihu/VehicleSignalsManager/In')
        #self.fetchDBUSPropForInterface('com.ihu.VehicleSignalsManager.Out', '/com/ihu/VehicleSignalsManager/Out')
        #self.fetchDBUSPropForInterface('com.ihu.VehicleSignalsManager.Internal', '/com/ihu/VehicleSignalsManager/Internal')

    def fetchDBUSPropForInterface(self, dbusinterface, dbusobject):
        dBUSProxy = self.dbus_conn.get_object(dbusinterface,dbusobject)
        signalsInPropInt = dbus.Interface(dBUSProxy,'org.freedesktop.DBus.Properties')
        propsIn = signalsInPropInt.GetAll('')


        for key, value in propsIn.items():
            #print(key + "," +value)
            sigName = str(key)
            sigData = str(value)
            if sigData:
                 self.on_message_dbus(sigName,sigData);

    def readSignalDataProxy(self,signalName, dBUSProxy, signalsInPropInt, dbusInterface):
        propertyData = signalsInPropInt.Get(dbusInterface,signalName)
        return propertyData

    def on_message_dbus(self,signalName, signalData):
        #print("on_message_dbus "  + signalName + " " + signalData)
        self.pending_messages.put_nowait((signalName, signalData))

    def incoming_message_dispatcher(self):
        maxiter = xrange(100)
        for i in maxiter:
            if self.pending_messages.empty():
                break
            msg = self.pending_messages.get()
            self.handle_message_dbus(*msg)
            self.pending_messages.task_done()
        self.master.after(50, self.incoming_message_dispatcher)


    def handle_message_dbus(self, signalName, signalData):
        data = j2d(signalData)
        try:
            handler = self.message_handlers[signalName]
            handler(data)
        except(KeyError):
            pass

        try:
            self.handle_message(signalName,j2d(signalData))
        except Exception as e:
            print("ERROR parsing message: ", e, signalData)

    def handle_message(self, dataelement_name, data):
        #print("Handle message " + dataelement_name +  " " + d2j(data))
        if dataelement_name == "ActivateVfc":
            return  # internal signal not in arxml

        try:
            sink = self.knownReceivedMessages[dataelement_name]
        except KeyError:
            sink = self.create_sink(dataelement_name)           # TODO: This is slow after first connect when all signals come in, investigate
            self.knownReceivedMessages[dataelement_name] = sink
            self.filter(self.filterBindVar.get())               # TODO: This is slow after first connect when all signals come in, investigate

	if data['state'] != 0:
            sink.portname_label.configure(background="#F55")
            return

        sink.portname_label.configure(background="")


        arType = sink.arxmltype
        if arType.getCategory() == "STRUCTURE":
           for sc in sink.sinkConnections:
              signalName = sc.arDataTypeValue.shortname
              dataElementsDataType = getDataElementsDatTypeFromCompuMethod(sc.arDataTypeValue.getDataType())
              if signalName != "Boolean" and dataElementsDataType == signalName:
                  signalName += "_"
              signalValue = data["value"][signalName]
              newVal = str(signalValue)
              if newVal != sc.bindableStringVariable.get():
                 sc.bg_fadeaway_ratio = 1
              sc.bindableStringVariable.set(newVal)

        elif arType.getCategory() == "VALUE":
            sc = sink.sinkConnections[0]
            compuMethod = arType.getCompuMethod()
            if compuMethod.getCategory() == 'TEXTTABLE':
                name_to_value_dict = compuMethod.getEnumerations()  # NOTE: Value here is a tuple
                value_to_name_dict = {v[0]: k for k, v in name_to_value_dict.items()}
                name = value_to_name_dict.get(str(data["value"]), "???")
                newVal = "(%d) %s" % (data["value"], name)
                if newVal != sc.bindableStringVariable.get():
                    sc.bg_fadeaway_ratio = 1
                sc.bindableStringVariable.set(newVal)
            else:
                newVal = str(data["value"])
                if newVal != sc.bindableStringVariable.get():
                    sc.bg_fadeaway_ratio = 1
                sc.bindableStringVariable.set(newVal)
        elif arType.getCategory() == "BOOLEAN":
            sc = sink.sinkConnections[0]
            newVal = str(data["value"])
            if newVal != sc.bindableStringVariable.get():
                sc.bg_fadeaway_ratio = 1
            sc.bindableStringVariable.set(str(data["value"]))
        else:
            pass # not supported by carsim


    def create_sink(self, dataelement_name):
        master = self.master

        try:
            dataTypeKey = self.element_name_to_data_element[dataelement_name].datatype_key
        except KeyError:
            print("WARNING: Input element not found: %s", dataelement_name)
            return

        entrySignalConnections = []
        arDataTypeValue = self.arxmldata.datatypes[dataTypeKey]

        category = arDataTypeValue.getCategory()

        structframe = ttk.Frame(master)
        portname_label = ttk.Label(master, text=dataelement_name)
        self.add_external_button_row(portname_label, structframe)

        if category == "STRUCTURE":
            subElements = arDataTypeValue.getElements()
            for se in subElements:
                signalname_label = ttk.Label(structframe, text=se.shortname + ": ", justify=Tkinter.LEFT, font=self.labelFont)
                signalname_label.pack(side=Tkinter.LEFT)

                v = Tkinter.StringVar()
                value_label = ttk.Label(structframe, width=5, textvariable=v, font=self.valueFont)
                value_label.pack(side=Tkinter.LEFT)

                entrySignalConnections.append(SinkConnection(v, se, value_label, signalname_label))

            #print("    structure %s" % ( ", ".join([r.shortname + " " + r.getCategory() for r in subElements])))
        elif category == "ARRAY":
            print("    WARNING: ARRAY UNSUPPORTED")
        elif category == "VALUE":
            v = Tkinter.StringVar()
            value_label = ttk.Label(structframe, width=50, textvariable=v, font=self.valueFont)
            value_label.pack(side=Tkinter.LEFT)

            entrySignalConnections.append(SinkConnection(v, arDataTypeValue, value_label, None))
        elif category == "BOOLEAN":
            v = Tkinter.StringVar()
            value_label = ttk.Label(structframe, width=10, textvariable=v, font=self.valueFont)
            value_label.pack(side=Tkinter.LEFT)
            entrySignalConnections.append(SinkConnection(v, arDataTypeValue, value_label, None))
        else:
            print("    WARNING: Unsupported category: " + category)

        senderConnection = Sink(arDataTypeValue, entrySignalConnections, dataelement_name, portname_label, structframe)
        return senderConnection

    def add_sender_port(self, portName):
        try:
            port = self.ihuports[portName]
        except KeyError:
            print("Failed to add sender port: %s" % portName)
            traceback.print_exc()
            return
        elements_list = port.getDataElements().items()
        for (e_name, data_type) in elements_list:
            self.add_sender_element(e_name)

    def add_sender_element(self, dataelement_name):
        if dataelement_name in self.addedSenderElements:
            return
        self.addedSenderElements.add(dataelement_name)
        try:
            data_element = self.element_name_to_data_element[dataelement_name]
        except KeyError:
            print("Failed to add sender element: %s" % dataelement_name)
            traceback.print_exc()
            return
        port = data_element.parent_port
        isInsignal = isinstance(port, autosar.components.AR_RPort)

        master = self.master

        arDataTypeValue = self.arxmldata.datatypes[data_element.datatype_key]
        category = arDataTypeValue.getCategory()
        compuMethod = arDataTypeValue.getCompuMethod()

        if category == "STRUCTURE":
            senderWidget = StructSender(master, arDataTypeValue.shortname, arDataTypeValue)
        elif category == "VALUE":
            if compuMethod.getCategory() == 'TEXTTABLE':
                senderWidget = EnumSender(master, compuMethod, arDataTypeValue.shortname)
            else:
                dataElementsDataType = getDataElementsDatTypeFromCompuMethod(arDataTypeValue)
                senderWidget = NumericSender(master, dataElementsDataType, arDataTypeValue)
        elif category == "BOOLEAN":
            senderWidget = BoolSender(master)
        else:
            print("WARNING: adding port for unsupported category: " + category)
            return

        firstCol = Tkinter.Frame(master)
        send_button = ttk.Button(firstCol, text="Send", width=4)
        stop_button = ttk.Button(firstCol, text="Error", width=4)
        port_name_label = ttk.Label(firstCol, text=dataelement_name)
        info_label = ttk.Label(firstCol, width=4)
        info_label.pack(side=Tkinter.LEFT)
        stop_button.pack(side=Tkinter.LEFT)
        send_button.pack(side=Tkinter.LEFT)
        port_name_label.pack(side=Tkinter.LEFT)



        portSender = PortSender(
            info_label,
            send_button,
            stop_button,
            senderWidget,
            dataelement_name,
            data_element.de_name,
            isInsignal,
            firstCol,
            self.bindAutoSend)

        if self.dbus_conn:
            portSender.setdbusconn(self.dbus_conn)
            portSender.setProxies(self.dBUSProxyIn,self.dBUSProxyOut)

        self.add_external_button_row(firstCol, senderWidget)
        self.all_senders.append(portSender)
        self.enable_all_buttons(self.connected)

    def register_message_handler(self, message_id, callback):
        self.message_handlers[message_id] = callback

    def add_external_button_row(self, left, right):
        self.buttonRow += 1
        if left is not None:
            left.grid(row=self.buttonRow, column=0, sticky=Tkinter.W)
        if right is not None:
            right.grid(row=self.buttonRow, column=1, sticky=Tkinter.W)

    def external_send(self, topic, value, type):
        if self.dbus_conn is None:
            # TODO: Need connected-callback to testmodules or queuing to solve this properly
            print("WARNING: external_send called before client was connected")
            return
        #TODO: Auto parse the type based on the name
        #TODO: Update the sender widgets?
        dataelement_name = topic[3:]

        if dataelement_name not in self.element_name_to_data_element.keys():
            print("WARNING: external_send called with unknown element name: %s", topic)

        data = {
            "state": 0,
            "timestamp": 1447941240073,
            "type": type,
            "value": value
        }
        if topic[0:3] == "/o/":
            dbusInterfaceName = 'com.ihu.VehicleSignalsManager.Out'
            dbusObjectName = '/com/ihu/VehicleSignalsManager/Out'
            signalsInPropInt = dbus.Interface(self.dBUSProxyOut, 'com.ihu.VehicleSignalsManager.Out')
        else:
            dbusInterfaceName = 'com.ihu.VehicleSignalsManager.In'
            dbusObjectName = '/com/ihu/VehicleSignalsManager/In'
            signalsInPropInt = dbus.Interface(self.dBUSProxyIn, 'com.ihu.VehicleSignalsManager.In')

        print ("External Send " + dataelement_name + " " + d2j(data) +  "  " + dbusInterfaceName)
        dbusdata = dbus.String(d2j(data),variant_level=1)
        #signalsInPropInt.Set(dbusInterfaceName, dataelement_name, dbusdata)
        signalsInPropInt.SetDESignal(dataelement_name, dbusdata)

    def filter(self, filterString):
        filters = filterString.lower().split()

        def matchfunc(name):
            if len(filters) == 0:
                return True
            matchName = name.lower()
            for f in filters:
                if f in matchName:
                    return True
            return False

        for key, sink in self.knownReceivedMessages.items():
            if matchfunc(key):
                sink.portname_label.grid()
                sink.struct_frame.grid()
            else:
                sink.portname_label.grid_remove()
                sink.struct_frame.grid_remove()

        for portsender in self.all_senders:
            if matchfunc(portsender.portName):
                portsender.first_col.grid()
                portsender.senderWidget.grid()
            else:
                portsender.first_col.grid_remove()
                portsender.senderWidget.grid_remove()

    def enable_all_buttons(self, enable):
        state = Tkinter.NORMAL if enable else Tkinter.DISABLED
        for portsender in self.all_senders:
            portsender.send_button.configure(state=state)
            portsender.stop_button.configure(state=state)
        self.bSendAll.configure(state=state)


    def send_all(self):
        for portsender in self.all_senders:
            portsender.send()

    def update_all_sender_objects(self):
        for portsender in self.all_senders:
            portsender.setdbusconn(self.dbus_conn)
            portsender.setProxies(self.dBUSProxyIn, self.dBUSProxyOut)

    def set_filter(self, filter):
        self.filterBindVar.set(filter)

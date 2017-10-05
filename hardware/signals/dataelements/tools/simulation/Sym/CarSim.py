import queue
import time
import tkinter
import tkinter.ttk
import tkinter.font
import json
import sys
import os
import socket
import select
import threading

sys.path.append(os.path.join(os.path.dirname(__file__), "../../../", "AutosarCodeGen"))
import autosar.arxml as arxml
import autosar.components
import traceback
from dataelements_generator.getDatatypes import getIntTypeStr


BG_COLOR_SENT = "#81f963"
BG_COLOR_INVALIDATED = "#fc931b"
BG_COLOR_NOT_SENT = "#8e8d8b"


def d2j(data):
    return json.dumps(data)


def j2d(data):
    return json.loads(data)


class DESignalWidget(tkinter.Frame):
    def __init__(self, master, **kw):
        tkinter.Frame.__init__(self, master, **kw)
    def on_change(self):
        pass


class PortSender():
    #Note: This class is not a widget of its own, because the sub-widgets are commonly used in a grid view
    #TODO add is_connected instead of connection as parameter
    def __init__(self, info_label, send_button, stop_button, senderWidget, portName,
                 dataElementName, first_col, is_insignal, autosend_bindvar, connection, send_queue):
        self.autosend_bindvar = autosend_bindvar
        self.senderWidget = senderWidget
        self.infoBindVar = tkinter.StringVar()
        self.infoBindVar.set("x")
        self.infoLabel = info_label
        self.infoLabel.configure(textvariable=self.infoBindVar, justify=tkinter.LEFT)
        self.infoLabel.config(background=BG_COLOR_NOT_SENT)
        self.is_insignal = is_insignal
        self.send_button = send_button
        self.send_button.configure(command=self.send)
        self.stop_button = stop_button
        self.stop_button.configure(command=self.set_signal_to_error)
        self.send_queue = send_queue

        self.portName = portName
        self.senderWidget.on_change = lambda : self.on_change()
        self.dataElementName = dataElementName
        self.first_col = first_col
        self.connection = connection

    def on_change(self):
        self.infoLabel.config(background=BG_COLOR_INVALIDATED)
        self.infoBindVar.set("x")
        if self.autosend_bindvar.get():
            self.send()

    def send(self):
        if self.connection.connected():
            self.infoBindVar.set("Sent")
            self.infoLabel.config(background=BG_COLOR_SENT)
            if self.is_insignal:
                dir = 1
            else:
                dir = 0
            toSend = {
                "SignalName": self.portName,
                "Dir": dir,
                "Data":{
                "state": 0,
                "timestamp": int(time.time()),
                "type": self.senderWidget.dataElementsDataType,
                "value": self.senderWidget.get_value()
                }
            }
            self.send_queue.put(d2j(toSend))

    def set_signal_to_error(self):
        if self.connection.connected():
            self.infoBindVar.set("x")
            self.infoLabel.config(background=BG_COLOR_NOT_SENT)
            toSend = {
                "state": 1,
                "errorCode": 0,
                "timestamp": int(time.time()),
                "type": self.senderWidget.dataElementsDataType,
                "value": self.senderWidget.get_value()
            }
            self.send_queue.put(d2j(toSend))


class BoolSender(DESignalWidget):
    def __init__(self, master, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = "bool"
        self.bindVar = tkinter.IntVar()
        self.e = tkinter.ttk.Checkbutton(self, width=10, variable=self.bindVar, command=lambda: self.on_change())
        self.e.pack()

    def get_value(self):
        return bool(self.bindVar.get())


class EnumSender(DESignalWidget):
    def __init__(self, master, compuMethod, dataElementsDataType, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = dataElementsDataType
        self.bindVar = tkinter.StringVar()

        optionDict, defaultOption = enumCompuMethodToOptionMenus(compuMethod)
        self.optionDict = optionDict
        self.bindVar.set(defaultOption)
        options = (k for (k,v) in sorted(iter(optionDict.items()), key=lambda x: x[1]))
        e = tkinter.ttk.OptionMenu(self, self.bindVar, defaultOption, *options, command=lambda x: self.on_change())
        e.pack()

    def get_value(self):
        return self.optionDict[self.bindVar.get()]


class NumericSender(DESignalWidget):
    def __init__(self, master, dataElementsDataType, arDataTypeValue, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = dataElementsDataType
        self.bindVar = tkinter.StringVar()
        self.bindVar.set("0")
        self.e = tkinter.ttk.Entry(self, width=10, textvariable=self.bindVar)
        self.e.bind("<Return>", lambda x: self.on_change())
        self.bindVar.trace("w", lambda name, index, mode: self.strvarchange())
        self.e.pack()

        min, max = arDataTypeValue.getDataConstraint().getLimits()

        self.bindVarInt = tkinter.IntVar()
        self.bindVarInt.trace("w", lambda name, index, mode: self.intvarchange())
        self.scale = tkinter.ttk.Scale(self, from_=min, to=max, orient=tkinter.HORIZONTAL, variable=self.bindVarInt)
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

            l = tkinter.ttk.Label(structframe, text=subEl.shortname, justify=tkinter.LEFT)
            l.pack(side=tkinter.LEFT)
            subWidget.pack(side=tkinter.LEFT)
            subWidget.on_change = lambda: self.on_change()

    def get_value(self):
        values = {}
        for signalName, widget in self.subElements.items():
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
    value_to_name_dict = {v[0]: k for k, v in list(name_to_value_dict.items())}
    lookupDict = {}  # keys displayed to user with both name and value,

    default = None
    for intValue in compuMethod.getValues():
        ename = value_to_name_dict[str(intValue)].replace(compuMethod.shortname + '_', '')
        name_with_value = ename + " (%d)" % intValue
        lookupDict[name_with_value] = intValue
        if default is None:
            default = name_with_value
    return lookupDict, default


#This class starts a thread to handle receiving and sending data.
class Connection():
    def __init__(self, send_queue = None, receive_queue = None):
        self.send_queue = send_queue
        self.receive_queue = receive_queue
        self.socket = socket.socket()
        self._connected = False
        self.lock = threading.RLock()
        self.thread = threading.Thread(target=self.run)
        self.thread.daemon =True
        self.thread.start()

    def run(self):
        received_data = b''
        while True:
            if self.connected():
                try:
                    send_sockets = []
                    if not self.send_queue.empty():
                        send_sockets.append(self.socket)

                    ready_to_read, ready_to_write, in_error = \
                        select.select([self.socket, ], send_sockets, [self.socket], 1)
                    if self.socket in in_error:
                        self._disconnect()
                        continue
                    if self.socket in ready_to_read:
                        received_data = received_data + self.socket.recv(1024)
                        if len(received_data) == 0: #The python way to say that the socket is closed
                            self._disconnect()
                            continue
                        if len(received_data) >= 6:
                            identifier = received_data[:6].decode('ascii')
                            if identifier == 'CarSim':
                                length_element = received_data [6:10].decode('ascii')
                                if len(received_data) >= int(length_element):
                                    self._receive(received_data)
                                    received_data = received_data[int(length_element):]
                            else:
                                #Got trash, lets reset buffer
                                received_data = b''

                    if self.socket in ready_to_write and not self.send_queue.empty():
                        item = self.send_queue.get()
                        self._send(item)
                        self.send_queue.task_done()
                except select.error:
                    self._disconnect()
            else:
                time.sleep(1)

    def connect(self, address):
        print(("Connecting to %s" % address))
        if self.connected() is False:
            # create an INET, STREAMing socket
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                self.socket.connect((address, 8080))
                self._set_connected(True)
                print("Connected to " + address)
            except:
                raise

    def connected(self):
        with self.lock:
            return self._connected

    def _set_connected(self, value):
        with self.lock:
            self._connected = value

    def _send(self, data: str):
        buffer = data.encode(encoding='ascii')
        #Yea, the lenght is encoded in ascii...
        lengthString = '{0:04d}'.format(len(buffer))
        self.socket.sendall('CarSim'.encode(encoding='ascii') + lengthString.encode(encoding='ascii') + buffer)

    # The expected message have the following format:
    #  [6 bytes, ascii "CarSim"][4 bytes,ascii message length ][<<message length>> bytes,ascii message]
    def _receive(self, data):
        decoded_data = data.decode('ascii')
        print(decoded_data)  # Just for debugging
        length = data[6:10].decode('ascii')
        message = data[10:10+length].decode('ascii')
        assert (len(message) == length)
        self.receive_queue.put(message)

    def _disconnect(self):
        self.socket.shutdown(socket.SHUT_RDWR)
        self.socket.close()
        self._set_connected(False)


class App:
    def __init__(self, master):
        self.connected = False
        self.all_senders = []
        self.message_handlers = {}
        self.labelFont = tkinter.font.Font(root=master, family="Courier New", size=12)
        self.valueFont = tkinter.font.Font(root=master, family="Courier New", size=12, weight=tkinter.font.BOLD)

        self.knownReceivedMessages = {}
        self.addedSenderElements = set()
        self.receive_queue = queue.Queue()
        self.send_queue = queue.Queue()
        self.connection = Connection(self.send_queue, self.receive_queue)

        #arxmldata
        arxmldata = arxml.load(os.path.join(os.path.dirname(__file__),
                                           "../../../", "AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml"))

        self.element_name_to_data_element = {}

        for swcsKey, swcsValue in arxmldata.swcs.items():
            if swcsValue.shortname != "IHU":
                continue
            #print("signal: %s, value: %s" % ( swcsKey, swcsValue))
            for portKey, port in sorted(swcsValue.ports.items()):
                elems = port.getDataElements()

                for de, dataTypeKey in sorted(elems.items()):
                    self.element_name_to_data_element[de] = DataElement(de, port, dataTypeKey)

        self.arxmldata = arxmldata

        # Scroll widget only works on Canvas so we have to place the frame inside the canvas
        yScrollbar = tkinter.Scrollbar(master)
        yScrollbar.pack(side=tkinter.RIGHT, fill=tkinter.Y)

        xScrollbar = tkinter.Scrollbar(master, orient=tkinter.HORIZONTAL)
        xScrollbar.pack(side=tkinter.BOTTOM, fill=tkinter.X)

        canvas = tkinter.Canvas(master, yscrollcommand=yScrollbar.set, xscrollcommand=xScrollbar.set)

        masterFrame = tkinter.ttk.Frame(canvas)
        masterFrame.pack(side=tkinter.LEFT, fill=tkinter.BOTH)

        canvas.pack(side=tkinter.LEFT, fill=tkinter.BOTH, expand=True)
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

        ## Connection status
        lConnectionStatus = tkinter.ttk.Label(master, text="Connection status: ")
        lConnectionStatus.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.bindConnectionStatus = tkinter.Variable()
        self.bindConnectionStatus.set("disconnected")
        self.lActualConnectionStatus = tkinter.ttk.Label(master, textvariable=self.bindConnectionStatus)
        self.lActualConnectionStatus .grid(row=buttonRow, column=1, sticky=tkinter.W)
        self.lActualConnectionStatus.config(background="#ff0000")


        ## Connect to remote
        buttonRow += 1
        self.bConnectToRemote = tkinter.ttk.Button(master, text="ConnectToRemote", command=self.connectToRemote)
        self.bConnectToRemote.grid(row=buttonRow, column=0, sticky=tkinter.W)
        self.eConnectToRemote = tkinter.ttk.Entry(master, width=20)
        self.eConnectToRemote.bind("<Return>", lambda x: self.connectToRemote())
        self.eConnectToRemote.insert(0, "127.0.0.1")
        self.eConnectToRemote.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Add from ARXML
        buttonRow += 1
        self.bAddSenderPort = tkinter.ttk.Button(master, text="Add new Dataelement", command=self.ui_add_port)
        self.bAddSenderPort.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.eAddSenderPort = tkinter.ttk.Entry(master, width=30)
        self.eAddSenderPort.insert(0, "VehSpdLgt")
        self.eAddSenderPort.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Filter Entry
        buttonRow += 1

        def filterChanged(name, index, mode):
            self.filter(self.filterBindVar.get())

        self.filterLabel = tkinter.ttk.Label(master, text="Filter")
        self.filterLabel.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.filterBindVar = tkinter.StringVar()
        self.filterBindVar.trace("w", filterChanged)
        self.eFilter = tkinter.ttk.Entry(master, width=30, textvariable=self.filterBindVar)
        self.eFilter.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Auto send on change
        buttonRow += 1

        self.bindAutoSend = tkinter.BooleanVar()
        self.bindAutoSend.set(True)
        self.autoSendLabel = tkinter.ttk.Label(master, text="Auto send on change")
        self.autoSendLabel.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.autoSendCheckbox = tkinter.Checkbutton(master, width=10, variable=self.bindAutoSend)
        self.autoSendCheckbox.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Send all
        buttonRow += 1
        self.bSendAll = tkinter.ttk.Button(master, width=30, text="Send all")
        self.bSendAll.configure(command=self.send_all)
        self.bSendAll.grid(row=buttonRow, column=0, sticky=tkinter.W)


        #################### separator ##########################
        buttonRow += 1
        separator = tkinter.ttk.Separator(master)
        separator.grid(row=buttonRow, sticky=tkinter.EW)


        self.master = master
        self.buttonRow = buttonRow +1

        self.ever_connected = False

    def fade_timer(self):
        for portname, sink in self.knownReceivedMessages.items():
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

    def connectToRemote(self):
        self.connect(self.eConnectToRemote.get())

    def connect(self, address):
        if not self.connection.connected():
            try:
                self.connection.connect(address)
                self.master.after(5000, self.fade_timer)
                self.enable_all_buttons(True)
                self.bindConnectionStatus.set("Connected - " + address)
                self.lActualConnectionStatus.config(background="#00ff00")
                self.master.after(1000, self.incoming_message_dispatcher)
                self.master.after(1000, self.check_connection)
            except:
                print("Could not connect")


    def onDisconnect(self):
        self.bindConnectionStatus.set("disconnected")
        self.lActualConnectionStatus.config(background="#ff0000")
        self.connected = False
        self.enable_all_buttons(False)

    def incoming_message_dispatcher(self):
        maxiter = range(100)
        for i in maxiter:
            if self.receive_queue.empty():
                break
            msg = self.receive_queue.get()
            print(msg)
            self.handle_message(*msg)
            self.receive_queue.task_done()
        self.master.after(50, self.incoming_message_dispatcher)


    def handle_message(self, msg):
        decoded_msg = j2d(msg)
        signal_name = decoded_msg['SignalName']
        try:
            handler = self.message_handlers[signal_name]
            handler(decoded_msg)
        except(KeyError):
            pass

        try:
            self.handle_message(signal_name, decoded_msg)
        except Exception as e:
            print(("ERROR parsing message: ", e, decoded_msg))


    def check_connection(self):
        if not self.connection.connected():
            self.onDisconnect()
        else:
            self.master.after(1000, self.check_connection)

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
                value_to_name_dict = {v[0]: k for k, v in list(name_to_value_dict.items())}
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
            print(("WARNING: Input element not found: %s", dataelement_name))
            return

        entrySignalConnections = []
        arDataTypeValue = self.arxmldata.datatypes[dataTypeKey]

        category = arDataTypeValue.getCategory()

        structframe = tkinter.ttk.Frame(master)
        portname_label = tkinter.ttk.Label(master, text=dataelement_name)
        self.add_external_button_row(portname_label, structframe)

        if category == "STRUCTURE":
            subElements = arDataTypeValue.getElements()
            for se in subElements:
                signalname_label = tkinter.ttk.Label(structframe, text=se.shortname + ": ", justify=tkinter.LEFT, font=self.labelFont)
                signalname_label.pack(side=tkinter.LEFT)

                v = tkinter.StringVar()
                value_label = tkinter.ttk.Label(structframe, width=5, textvariable=v, font=self.valueFont)
                value_label.pack(side=tkinter.LEFT)

                entrySignalConnections.append(SinkConnection(v, se, value_label, signalname_label))

            #print("    structure %s" % ( ", ".join([r.shortname + " " + r.getCategory() for r in subElements])))
        elif category == "ARRAY":
            print("    WARNING: ARRAY UNSUPPORTED")
        elif category == "VALUE":
            v = tkinter.StringVar()
            value_label = tkinter.ttk.Label(structframe, width=50, textvariable=v, font=self.valueFont)
            value_label.pack(side=tkinter.LEFT)

            entrySignalConnections.append(SinkConnection(v, arDataTypeValue, value_label, None))
        elif category == "BOOLEAN":
            v = tkinter.StringVar()
            value_label = tkinter.ttk.Label(structframe, width=10, textvariable=v, font=self.valueFont)
            value_label.pack(side=tkinter.LEFT)
            entrySignalConnections.append(SinkConnection(v, arDataTypeValue, value_label, None))
        else:
            print(("    WARNING: Unsupported category: " + category))

        senderConnection = Sink(arDataTypeValue, entrySignalConnections, dataelement_name, portname_label, structframe)
        return senderConnection

    def add_sender_element(self, dataelement_name):

        if dataelement_name in self.addedSenderElements:
            return
        self.addedSenderElements.add(dataelement_name)
        try:
            data_element = self.element_name_to_data_element[dataelement_name]
        except KeyError:
            print(("Failed to add sender element: %s" % dataelement_name))
            traceback.print_exc()
            return
        port = data_element.parent_port
        is_insignal = isinstance(port, autosar.components.AR_RPort)

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
            print(("WARNING: adding port for unsupported category: " + category))
            return

        firstCol = tkinter.Frame(master)
        send_button = tkinter.ttk.Button(firstCol, text="Send", width=4)
        stop_button = tkinter.ttk.Button(firstCol, text="Error", width=4)
        port_name_label = tkinter.ttk.Label(firstCol, text=dataelement_name)
        info_label = tkinter.ttk.Label(firstCol, width=4)
        info_label.pack(side=tkinter.LEFT)
        stop_button.pack(side=tkinter.LEFT)
        send_button.pack(side=tkinter.LEFT)
        port_name_label.pack(side=tkinter.LEFT)

        portSender = PortSender(
            info_label,
            send_button,
            stop_button,
            senderWidget,
            dataelement_name,
            data_element.de_name,
            is_insignal,
            firstCol,
            self.bindAutoSend,
            self.connection,
            self.send_queue)

        self.add_external_button_row(firstCol, senderWidget)
        self.all_senders.append(portSender)

    def register_message_handler(self, message_id, callback):
        self.message_handlers[message_id] = callback

    def add_external_button_row(self, left, right):
        self.buttonRow += 1
        if left is not None:
            left.grid(row=self.buttonRow, column=0, sticky=tkinter.W)
        if right is not None:
            right.grid(row=self.buttonRow, column=1, sticky=tkinter.W)

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

        for key, sink in list(self.knownReceivedMessages.items()):
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
        state = tkinter.NORMAL if enable else tkinter.DISABLED
        for portsender in self.all_senders:
            portsender.send_button.configure(state=state)
            portsender.stop_button.configure(state=state)
        self.bSendAll.configure(state=state)

    def send_all(self):
        for portsender in self.all_senders:
            portsender.send()

    def set_filter(self, filter):
        self.filterBindVar.set(filter)

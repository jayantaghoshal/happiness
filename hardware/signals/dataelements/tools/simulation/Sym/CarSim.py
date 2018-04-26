# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import queue
import tkinter
import tkinter.ttk
import tkinter.font
import tkinter.filedialog
import json
import sys
import os
import logging
import importlib
import typing
import traceback
sys.path.append(os.path.join(os.path.dirname(__file__), "../../../", "AutosarCodeGen"))
import autosar.components
from dataelements_generator import getDatatypes
from dataelements_generator.render_python_basic import clean_member_name
from dataelements_generator.model import DE_Struct, DE_BaseType, DE_Enum, DE_Array, DE_Boolean, DE_Identical, DE_Value, DE_EnumItem, DE_Element
from .carsimconnection import CarsimConnection, JsonMessage, CarsimDataElementValue
from .adb_connection import AdbCarsimConnection
from generated import dataelements_carsim
from generated import dataelements_fdx
from generated import dataelements_abc

MessageHandlerCallback = typing.Callable[[JsonMessage], None]

BG_COLOR_SENT = "#81f963"
BG_COLOR_INVALIDATED = "#fc931b"
BG_COLOR_NOT_SENT = "#8e8d8b"

class DummyEmptyClass:
    pass

class ConnectionWrapper:
    def __init__(self):
        self.is_connected = False
        self.serializer = None # type: typing.Optional[dataelements_abc.DataElements]
        self.connection = None # type: typing.Optional[CarsimConnection]

    def send(self, dataelement_name, data):
        serialize_func = getattr(self.serializer, "send_" + dataelement_name)
        return serialize_func(data)

    def stop(self, dataelement_name):
        stop_func = getattr(self.serializer, "stop_" + dataelement_name)
        return stop_func()


##################################################################################################################
#### Sender widgets
##################################################################################################################

class DESignalWidget(tkinter.Frame):
    def __init__(self, master, **kw):
        tkinter.Frame.__init__(self, master, **kw)
    def on_change(self):
        pass
    def get_value(self) -> typing.Any:
        # Should return an object corresponing to types in gen_datatypes.py
        raise NotImplementedError("To be overridden")


class PortSender():
    #Note: This class is not a widget of its own, because the sub-widgets are commonly used in a grid view
    def __init__(self,
                 info_label : tkinter.ttk.Label,
                 send_button : tkinter.ttk.Button,
                 stop_button : tkinter.ttk.Button,
                 senderWidget : DESignalWidget,
                 portName : str,
                 dataElementName: str,
                 first_col,
                 is_insignal : bool,
                 autosend_bindvar: tkinter.BooleanVar,
                 connection: ConnectionWrapper):
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
        if self.connection.is_connected:
            self.infoBindVar.set("Sent")
            self.infoLabel.config(background=BG_COLOR_SENT)
            self.connection.send(self.portName, self.senderWidget.get_value())

    def set_signal_to_error(self):
        if self.connection.is_connected:
            self.infoBindVar.set("x")
            self.infoLabel.config(background=BG_COLOR_NOT_SENT)
            self.connection.stop(self.portName)


class BoolSender(DESignalWidget):
    def __init__(self, master : tkinter.Frame, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.bindVar = tkinter.IntVar()
        self.e = tkinter.ttk.Checkbutton(self, width=10, variable=self.bindVar, command=lambda: self.on_change())
        self.e.pack()

    def get_value(self):
        return bool(self.bindVar.get())


class EnumSender(DESignalWidget):
    def __init__(self,
                 master : tkinter.Frame,
                 dataElementsDataType: DE_Enum, **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.bindVar = tkinter.StringVar()

        optionDict, defaultOption = enumCompuMethodToOptionMenus(dataElementsDataType)
        self.optionDict = optionDict
        self.bindVar.set(defaultOption)
        options = (k for (k, v) in sorted(iter(optionDict.items()), key=lambda x: x[1].name))
        e = tkinter.ttk.OptionMenu(self, self.bindVar, defaultOption, *options, command=lambda x: self.on_change())
        e.pack()

    def get_value(self):
        return self.optionDict[self.bindVar.get()].value


class NumericSender(DESignalWidget):
    def __init__(self,
                 master: tkinter.Frame,
                 arDataTypeValue: typing.Union[DE_Value, DE_Identical], **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.bindVar = tkinter.StringVar()
        self.bindVar.set("0")
        self.e = tkinter.ttk.Entry(self, width=10, textvariable=self.bindVar)
        self.e.bind("<Return>", lambda x: self.on_change())
        self.bindVar.trace("w", lambda name, index, mode: self.entry_text_changed())
        self.e.pack()

        self.bindVarInt = tkinter.DoubleVar()
        self.bindVarInt.trace("w", lambda name, index, mode: self.slider_value_changed())
        self.scale = tkinter.ttk.Scale(self, from_=arDataTypeValue.limit_min, to=arDataTypeValue.limit_max, orient=tkinter.HORIZONTAL, variable=self.bindVarInt)
        self.scale.pack()

    # Be careful here, there are some details required to get changes from both textfield and slider to propagate to
    # each other without getting into an infinite loop of data binding triggers.

    def entry_text_changed(self):
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

    def slider_value_changed(self):
        double_value = self.bindVarInt.get()
        rounded_value = int(round(double_value))
        self.bindVar.set(str(rounded_value))
        if double_value != rounded_value:
            # Round to closest integer
            # Hack because ttk.Scale does not support resolution-parameter
            self.bindVarInt.set(rounded_value)

    def get_value(self):
        return int(self.bindVar.get())  #TODO: Float?


class StructSender(DESignalWidget):
    def __init__(self,
                 master : tkinter.ttk.Frame,
                 all_types : typing.Mapping[str, DE_BaseType],
                 dataElementsDataType: DE_Struct,
                 **kw):
        DESignalWidget.__init__(self, master, **kw)
        self.dataElementsDataType = dataElementsDataType
        self.subElements = {}   # type: typing.Dict[str, DESignalWidget]
        structframe = self

        for subEl in dataElementsDataType.children:
            child_type = all_types[subEl.de_type_id]

            if isinstance(child_type, DE_Boolean):
                subWidget = BoolSender(structframe)
            elif isinstance(child_type, DE_Enum):
                subWidget = EnumSender(structframe, child_type)
            elif isinstance(child_type, DE_Value) or isinstance(child_type, DE_Identical):
                subWidget = NumericSender(structframe, child_type)
            else:
                raise NotImplementedError("Subtype not implemented: %r", child_type)
            self.subElements[subEl.member_name] = subWidget

            l = tkinter.ttk.Label(structframe, text=subEl.member_name, justify=tkinter.LEFT)
            l.pack(side=tkinter.LEFT)
            subWidget.pack(side=tkinter.LEFT)
            subWidget.on_change = lambda: self.on_change()

    def get_value(self):
        # Create a class dynamically that matches the types in gen_datatypes.py to be compatible with serializer
        values = DummyEmptyClass()
        for signalName, widget in self.subElements.items():
            values.__dict__[clean_member_name(signalName, "")] = widget.get_value()
        return values


##################################################################################################################
### Sinks are receivers
##################################################################################################################

SINGLE_SINK_MEMBER_ONLY = "ONEANDONLY"

# Sub values inside a Sink
class SinkMember:
    def __init__(self,
                 bindableStringVariable: tkinter.StringVar,
                 arDataTypeValue: DE_BaseType,
                 value_label: tkinter.ttk.Label,
                 signalname_label: typing.Optional[tkinter.ttk.Label]):
        self.arDataTypeValue = arDataTypeValue
        self.bindableStringVariable = bindableStringVariable
        self.value_label = value_label
        self.bg_fadeaway_ratio = 1
        self.signalname_label = signalname_label


# Sink = DataElement Receiver
class Sink:
    def __init__(self,
                 de_type: DE_BaseType,
                 sink_members: typing.Dict[str, SinkMember],
                 autosar_port_name : str,
                 portname_label: tkinter.ttk.Label,
                 struct_frame: tkinter.Frame):
        self.de_type = de_type
        self.sink_members = sink_members
        self.autosar_port_name = autosar_port_name
        self.portname_label = portname_label
        self.struct_frame = struct_frame


def enumCompuMethodToOptionMenus(enum_type: DE_Enum) -> typing.Tuple[typing.Dict[str, DE_EnumItem], str]:
    lookupDict = {}  # type: typing.Dict[str, DE_EnumItem]

    default = None
    for v in enum_type.values:
        ename = clean_member_name(v.name, "")
        name_with_value = ename + " (%d)" % v.value
        lookupDict[name_with_value] = v
        if default is None:
            default = name_with_value
    return lookupDict, default

##################################################################################################################
### Main
##################################################################################################################

class App:
    def __init__(self, root: tkinter.Tk, server_addr: typing.Tuple[str, int]):
        self.all_senders = []   # type: typing.List[PortSender]
        self.message_handlers = {} # type: typing.Dict[str, MessageHandlerCallback]
        self.labelFont = tkinter.font.Font(root=root, family="Courier New", size=12)
        self.valueFont = tkinter.font.Font(root=root, family="Courier New", size=12, weight=tkinter.font.BOLD)

        self.knownReceivedMessages = {}   # type: typing.Dict[str, Sink]
        self.addedSenderElements = set()  # type: typing.Set[str]
        self.added_modules = set()        # type: typing.Set[str]

        self._server_ip = server_addr[0]  # first element of tuple is ip-address
        self._server_port = int(server_addr[1])  # second element of tuple is tcp-port

        menubar = tkinter.Menu(root)
        root.config(menu=menubar)

        # create a pulldown menu, and add it to the menu bar
        filemenu = tkinter.Menu(menubar, tearoff=0)
        filemenu.add_command(label="Load Config...", command=self.load)
        filemenu.add_command(label="Save Config...", command=self.store)
        filemenu.add_separator()
        filemenu.add_command(label="Load Module...", command=self.load_module_file)
        filemenu.add_separator()
        filemenu.add_command(label="Exit", command=root.quit)
        menubar.add_cascade(label="File", menu=filemenu)


        db_dir = os.path.join(os.path.dirname(__file__), "../../../AutosarCodeGen/databases/")
        swc_input_file = os.path.join(db_dir, "SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml")
        com_input_file = os.path.join(db_dir, "SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml")
        assert os.path.split(swc_input_file)[-1] == dataelements_carsim.SWC_INPUT_FILE
        assert os.path.split(com_input_file)[-1] == dataelements_carsim.COM_INPUT_FILE
        assert os.path.split(swc_input_file)[-1] == dataelements_abc.SWC_INPUT_FILE
        assert os.path.split(com_input_file)[-1] == dataelements_abc.COM_INPUT_FILE


        swc_data = autosar.arxml.load(swc_input_file)
        comfile_data = autosar.arxml.load(com_input_file)
        (self.all_types, self.all_de_elements) = getDatatypes.parse(swc_data, comfile_data)
        self.dataelements_by_name= { x.de_dataelementname: x for x in self.all_de_elements  }   # type: typing.Dict[str, DE_Element]



        self.dataelements_rec_queue = queue.Queue()
        self.connection_wrapper = ConnectionWrapper()



        # Scroll widget only works on Canvas so we have to place the frame inside the canvas
        yScrollbar = tkinter.Scrollbar(root)
        yScrollbar.pack(side=tkinter.RIGHT, fill=tkinter.Y)

        xScrollbar = tkinter.Scrollbar(root, orient=tkinter.HORIZONTAL)
        xScrollbar.pack(side=tkinter.BOTTOM, fill=tkinter.X)

        canvas = tkinter.Canvas(root, yscrollcommand=yScrollbar.set, xscrollcommand=xScrollbar.set)

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


        self.master = masterFrame
        buttonRow = 0



        ## Log
        ErrorLogLabel = tkinter.ttk.Label(masterFrame, text="Log: ")
        ErrorLogLabel.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.bindErrorLogText = tkinter.Variable()
        self.bindErrorLogText.set("")
        self.lErrorLogLabel = tkinter.ttk.Label(masterFrame, textvariable=self.bindErrorLogText)
        self.lErrorLogLabel.grid(row=buttonRow, column=1, sticky=tkinter.W)
        self.lErrorLogLabel.config(background=None)



        ## Connection status

        buttonRow += 1
        ConnectionStatus = tkinter.ttk.Label(masterFrame, text="Connection status: ")
        ConnectionStatus.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.bindConnectionStatus = tkinter.Variable()
        self.bindConnectionStatus.set("disconnected")
        self.lActualConnectionStatus = tkinter.ttk.Label(masterFrame, textvariable=self.bindConnectionStatus)
        self.lActualConnectionStatus.grid(row=buttonRow, column=1, sticky=tkinter.W)
        self.lActualConnectionStatus.config(background="#ff0000")


        ## Connect/disconnect to remote
        buttonRow += 1
        self.buttons = tkinter.Frame(masterFrame)

        self.bConnectToRemote = tkinter.ttk.Button(masterFrame, text="ConnectToRemote", command=self.connectToRemote)
        self.bConnectToRemote.pack(in_=self.buttons, side=tkinter.LEFT)

        self.bConnectAdb = tkinter.ttk.Button(masterFrame, text="Connect ADB", command=self.connect_adb)
        self.bConnectAdb.pack(in_=self.buttons, side=tkinter.LEFT)

        # Not 100% ready so hide the button before someone tries it. Receiving doesn't work and disconnect is broken
        #self.bConnectCANoe = tkinter.ttk.Button(masterFrame, text="Connect CANoe", command=self.connect_canoe)
        #self.bConnectCANoe.pack(in_=self.buttons, side=tkinter.LEFT)

        self.bDisconnect = tkinter.ttk.Button(masterFrame, text="Disconnect", command=self.disconnectFromRemote)
        self.bDisconnect.pack(in_=self.buttons, side=tkinter.LEFT)

        self.buttons.grid(row=buttonRow, column=0, sticky=tkinter.W)
        self.eConnectToRemote = tkinter.ttk.Entry(masterFrame, width=20)
        self.eConnectToRemote.bind("<Return>", lambda x: self.connectToRemote())
        self.eConnectToRemote.insert(0, self._server_ip)
        self.eConnectToRemote.grid(row=buttonRow, column=1, sticky=tkinter.W)



        ## Add from ARXML
        buttonRow += 1
        self.bAddSenderPort = tkinter.ttk.Button(masterFrame, text="Add new Dataelement", command=self.ui_add_port)
        self.bAddSenderPort.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.eAddSenderPort = tkinter.ttk.Entry(masterFrame, width=30)
        self.eAddSenderPort.insert(0, "VehSpdLgtSafe")
        self.eAddSenderPort.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Filter Entry
        buttonRow += 1

        def filterChanged(name, index, mode):
            self.filter(self.filterBindVar.get())

        self.filterLabel = tkinter.ttk.Label(masterFrame, text="Filter")
        self.filterLabel.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.filterBindVar = tkinter.StringVar()
        self.filterBindVar.trace("w", filterChanged)
        self.eFilter = tkinter.ttk.Entry(masterFrame, width=30, textvariable=self.filterBindVar)
        self.eFilter.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Auto send on change
        buttonRow += 1

        self.bindAutoSend = tkinter.BooleanVar()
        self.bindAutoSend.set(True)
        self.autoSendLabel = tkinter.ttk.Label(masterFrame, text="Auto send on change")
        self.autoSendLabel.grid(row=buttonRow, column=0, sticky=tkinter.W)

        self.autoSendCheckbox = tkinter.Checkbutton(masterFrame, width=10, variable=self.bindAutoSend)
        self.autoSendCheckbox.grid(row=buttonRow, column=1, sticky=tkinter.W)


        ## Send all
        buttonRow += 1
        self.bSendAll = tkinter.ttk.Button(masterFrame, width=30, text="Send all")
        self.bSendAll.configure(command=self.send_all)
        self.bSendAll.grid(row=buttonRow, column=0, sticky=tkinter.W)


        #################### separator ##########################
        buttonRow += 1
        separator = tkinter.ttk.Separator(masterFrame)
        separator.grid(row=buttonRow, sticky=tkinter.EW)


        self.masterFrame = masterFrame
        self.root = root
        self.buttonRow = buttonRow +1

        self.ever_connected = False

        root.protocol("WM_DELETE_WINDOW", self.on_exit)

    def on_exit(self):
        try:
            self.disconnectFromRemote()
        except:
            pass
        self.root.destroy()

    def load_modules(self, modules: typing.Set[str]):
        all_files = os.listdir("testmodules")
        py_files = [file for file in all_files if file.endswith(".py")]
        py_modules = [p[:-3] for p in py_files]

        for p in py_modules:
            if p not in modules:
                continue
            if p == "__init__":
                continue

            try:
                imported_module = importlib.import_module("testmodules." + p)
                imported_module.init(self)
                self.added_modules.add(p)
            except Exception as e:
                self.log_error("Failed to load module: %s because: %r" % (p, e))
                logging.exception("Failed to load module: ")

    def store(self):
        d = dict()
        d['version'] = "1"
        d['modules'] = list(self.added_modules)
        d['data_elements'] = list(self.addedSenderElements)
        with tkinter.filedialog.asksaveasfile(mode='w', defaultextension=".json") as f:
            f.write(json.dumps(d))

    def load(self):
        fname = tkinter.filedialog.askopenfilename(filetypes=(("CarSim DE list", "*.json"),))
        with open(fname) as f:
            cfg = json.load(f)
            for m in cfg['modules']:
                self.load_modules(m)
            for se in cfg['data_elements']:
                self.add_sender_element(se)

    def load_module_file(self):
        fname = tkinter.filedialog.askopenfilename(filetypes=(("CarSim module", "*.py"),))
        if not fname.endswith('.py'):
            self.log_error('Module filename must be python file')
        self.load_modules({os.path.basename(fname[:-3]),})

    def fade_timer(self):
        for portname, sink in self.knownReceivedMessages.items():
            for s in sink.sink_members.values():
                if s.bg_fadeaway_ratio > 0:
                    s.bg_fadeaway_ratio -= 0.02
                    if s.bg_fadeaway_ratio < 0:
                        s.value_label.configure(background="")
                        continue

                    startRGB = (226, 244, 58)
                    finalRGB = (142, 138, 135)
                    rgb = []
                    for sX, fX in zip(startRGB, finalRGB):
                        rgb.append(sX + (fX - sX) * s.bg_fadeaway_ratio)

                    r, g, b = rgb
                    color_str = "#%02x%02x%02x" % (int(r), int(g), int(b))
                    s.value_label.configure(background=color_str)

        self.master.after(60, self.fade_timer)

    def clear_error_log(self):
        self.lErrorLogLabel.config(background=None)
        self.bindErrorLogText.set("")

    def log_error(self, text):
        logging.error(text)
        self.lErrorLogLabel.config(background="#ff0000")
        new_log = self.bindErrorLogText.get() + os.linesep + text
        new_log = os.linesep.join([line for line in new_log.split(os.linesep) if line][0:6])
        self.bindErrorLogText.set(new_log)

    def ui_add_port(self):
        self.add_sender_element(self.eAddSenderPort.get())

    def connectToRemote(self):
        self.clear_error_log()
        self.connect(self.eConnectToRemote.get())

    def connect_canoe(self):
        self.clear_error_log()
        if self.connection_wrapper.is_connected:
            return

        try:
            self.connection_wrapper.connection = None
            self.connection_wrapper.serializer = dataelements_fdx.FrSignalInterface(self.eConnectToRemote.get())
            self.master.after(5000, self.fade_timer)
            self.enable_all_buttons(True)
            self.bindConnectionStatus.set("Connected - CANoe")
            self.lActualConnectionStatus.config(background="#00ff00")
            self.connection_wrapper.is_connected = True
            self.master.after(1000, self.incoming_message_dispatcher)
            self.master.after(1000, self.check_connection)
        except Exception as e:
            self.log_error("Could not connect over ADB. Error: %r" % e)

    def connect_adb(self):
        self.clear_error_log()
        if self.connection_wrapper.is_connected:
            return
        self.connection_wrapper.connection = AdbCarsimConnection()
        self.connection_wrapper.serializer = dataelements_carsim.CarsimSerializer(self.connection_wrapper.connection, self.dataelements_rec_queue)

        try:
            self.connection_wrapper.connection.connect()
            self.master.after(5000, self.fade_timer)
            self.enable_all_buttons(True)
            self.bindConnectionStatus.set("Connected - ADB")
            self.lActualConnectionStatus.config(background="#00ff00")
            self.connection_wrapper.is_connected = True
            self.master.after(1000, self.incoming_message_dispatcher)
            self.master.after(1000, self.check_connection)
        except Exception as e:
            self.log_error("Could not connect over ADB. Error: %r" % e)

    def connect(self, address):
        if self.connection_wrapper.is_connected:
            return
        self.connection_wrapper.connection = CarsimConnection()
        self.connection_wrapper.serializer = dataelements_carsim.CarsimSerializer(self.connection_wrapper.connection, self.dataelements_rec_queue)

        try:
            self.connection_wrapper.connection.connect(address, self._server_port)
            self.master.after(5000, self.fade_timer)
            self.enable_all_buttons(True)
            self.bindConnectionStatus.set("Connected - " + address)
            self.lActualConnectionStatus.config(background="#00ff00")
            self.connection_wrapper.is_connected = True
            self.master.after(1000, self.incoming_message_dispatcher)
            self.master.after(1000, self.check_connection)
        except Exception as e:
            self.log_error("Could not connect to %s:%s. Error: %r" % (address, self._server_port, e))

    def check_connection(self):
        #TODO: Fdx does not have connected() function
        if self.connection_wrapper.connection is not None and not self.connection_wrapper.connection.connected():
            self.onDisconnect()
        else:
            self.master.after(1000, self.check_connection)

    def disconnectFromRemote(self):
        try:
            self.connection_wrapper.serializer.close()
        except:
            pass

    def onDisconnect(self):
        self.bindConnectionStatus.set("disconnected")
        self.lActualConnectionStatus.config(background="#ff0000")
        self.connection_wrapper.is_connected = False
        self.enable_all_buttons(False)

    def incoming_message_dispatcher(self):
        for i in range(100):
            if self.dataelements_rec_queue.empty():
                break
            msg = self.dataelements_rec_queue.get()  # type: CarsimDataElementValue
            try:
                self.handle_message(msg)
            except Exception as e:
                self.log_error("Exception %s in handle_signal %s" % (e, msg))
                logging.exception("Exception %s in handle_signal %s" % (e, msg))
            self.dataelements_rec_queue.task_done()
        # Register for another call in 50 ms
        self.master.after(50, self.incoming_message_dispatcher)


    def handle_message(self, msg: CarsimDataElementValue):
        # If there is a registered handler, call that first
        if msg.name in self.message_handlers:
            try:
                self.message_handlers[msg.name](msg)
            except Exception as e:
                self.log_error("Exception %s in message_handler for signal %s" % (e, msg.name))

        dataelement_name = msg.name
        if dataelement_name == "ActivateVfc":
            return  # internal signal not in arxml

        if dataelement_name not in self.knownReceivedMessages:
            # TODO: Both lines here are slow after first connect when all signals come in, investigate
            self.knownReceivedMessages[dataelement_name] = self.create_sink(dataelement_name)
            self.filter(self.filterBindVar.get())

        sink = self.knownReceivedMessages[dataelement_name]

        if msg.state != 0:
            sink.portname_label.configure(background="#F55")
            return
        sink.portname_label.configure(background="")

        def value_to_string(value, de_type: DE_BaseType):
            if isinstance(de_type, (DE_Value, DE_Identical, DE_Boolean)):
                return str(value)
            elif isinstance(de_type, DE_Enum):
                name = next((x.name for x in de_type.values if x.value == value), "???UNMATCHED???")
                return "%d/%s" % (value, name)
            else:
                self.log_error("Type not supported by carsim(2): %r" % de_type)
                return "???"

        de_type = sink.de_type
        if isinstance(de_type, DE_Struct):
            for member in de_type.children:
                sc = sink.sink_members[member.member_name]
                signal_value = msg.value.__dict__[clean_member_name(member.member_name, "")]
                new_val = value_to_string(signal_value, self.all_types[member.de_type_id])
                if new_val != sc.bindableStringVariable.get():
                    sc.bg_fadeaway_ratio = 1
                sc.bindableStringVariable.set(new_val)
        elif isinstance(de_type, (DE_Value, DE_Identical, DE_Enum, DE_Boolean)):
            sc = sink.sink_members[SINGLE_SINK_MEMBER_ONLY]
            new_val = value_to_string(msg.value, de_type)
            if new_val != sc.bindableStringVariable.get():
                sc.bg_fadeaway_ratio = 1
            sc.bindableStringVariable.set(new_val)
        else:
            self.log_error("Type not supported by carsim(1): %r" % de_type)


    def create_sink(self, dataelement_name: str) -> Sink:
        master = self.master

        try:
            data_element = self.dataelements_by_name[dataelement_name]
            data_type = self.all_types[data_element.de_type_id]
        except KeyError:
            self.log_error("Input element not found: %s" % dataelement_name)
            raise

        entry_signal_connections = {}  # type: typing.Dict[str, SinkMember]
        structframe = tkinter.ttk.Frame(master)
        portname_label = tkinter.ttk.Label(master, text=dataelement_name)
        self.add_external_button_row(portname_label, structframe)

        if isinstance(data_type, DE_Struct):
            for se in data_type.children:
                signalname_label = tkinter.ttk.Label(structframe, text=se.member_name + ": ", justify=tkinter.LEFT, font=self.labelFont)
                signalname_label.pack(side=tkinter.LEFT)

                v = tkinter.StringVar()
                value_label = tkinter.ttk.Label(structframe, width=15, textvariable=v, font=self.valueFont)
                value_label.pack(side=tkinter.LEFT)
                entry_signal_connections[se.member_name] = SinkMember(v, se, value_label, signalname_label)
        elif isinstance(data_type, DE_Array):
            raise NotImplementedError("ARRAY UNSUPPORTED")
        elif isinstance(data_type, (DE_Value,  DE_Identical, DE_Enum)):
            v = tkinter.StringVar()
            value_label = tkinter.ttk.Label(structframe, width=50, textvariable=v, font=self.valueFont)
            value_label.pack(side=tkinter.LEFT)
            entry_signal_connections[SINGLE_SINK_MEMBER_ONLY] = SinkMember(v, data_type, value_label, None)
        elif isinstance(data_type, DE_Boolean):
            v = tkinter.StringVar()
            value_label = tkinter.ttk.Label(structframe, width=10, textvariable=v, font=self.valueFont)
            value_label.pack(side=tkinter.LEFT)
            entry_signal_connections[SINGLE_SINK_MEMBER_ONLY] = SinkMember(v, data_type, value_label, None)
        else:
            self.log_error("Unsupported category: %r in message %s" % (data_type, dataelement_name))

        return Sink(data_type, entry_signal_connections, dataelement_name, portname_label, structframe)

    def add_sender_element(self, dataelement_name: str):
        if dataelement_name in self.addedSenderElements:
            return
        try:
            data_element = self.dataelements_by_name[dataelement_name]
            data_type = self.all_types[data_element.de_type_id]
        except KeyError:
            self.log_error("Failed to add sender element: %s" % dataelement_name)
            logging.exception(traceback.format_exc())
            return
        self.addedSenderElements.add(dataelement_name)
        master = self.master

        if isinstance(data_type , DE_Struct):
            sender_widget = StructSender(master, self.all_types, data_type)
        elif isinstance(data_type, DE_Enum):
            sender_widget = EnumSender(master, data_type)
        elif isinstance(data_type, DE_Value) or isinstance(data_type, DE_Identical):
            sender_widget = NumericSender(master, data_type)
        elif isinstance(data_type, DE_Enum):
            sender_widget = BoolSender(master)
        else:
            self.log_error("Failed adding port for unsupported data type: %r" % data_type)
            return

        first_col = tkinter.Frame(master)
        send_button = tkinter.ttk.Button(first_col, text="Send", width=4)
        stop_button = tkinter.ttk.Button(first_col, text="Error", width=4)
        port_name_label = tkinter.ttk.Label(first_col, text=dataelement_name + (' (IN)' if data_element.is_insignal else ' (OUT)'))
        info_label = tkinter.ttk.Label(first_col, width=4)
        info_label.pack(side=tkinter.LEFT)
        stop_button.pack(side=tkinter.LEFT)
        send_button.pack(side=tkinter.LEFT)
        port_name_label.pack(side=tkinter.LEFT)

        port_sender = PortSender(
            info_label,
            send_button,
            stop_button,
            sender_widget,
            dataelement_name,
            data_element.de_dataelementname,
            first_col,
            data_element.is_insignal,
            self.bindAutoSend,
            self.connection_wrapper)

        self.add_external_button_row(first_col, sender_widget)
        self.all_senders.append(port_sender)

    def register_message_handler(self, message_id: str, callback: MessageHandlerCallback):
        self.message_handlers[message_id] = callback

    def add_external_button_row(self, left: tkinter.Widget, right: tkinter.Widget):
        self.buttonRow += 1
        if left is not None:
            left.grid(row=self.buttonRow, column=0, sticky=tkinter.W)
        if right is not None:
            right.grid(row=self.buttonRow, column=1, sticky=tkinter.W)

    def filter(self, filter_string: str):
        filters = filter_string.lower().split()

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

    def enable_all_buttons(self, enable: bool):
        state = tkinter.NORMAL if enable else tkinter.DISABLED
        for portsender in self.all_senders:
            portsender.send_button.configure(state=state)
            portsender.stop_button.configure(state=state)
        self.bSendAll.configure(state=state)

    def send_all(self):
        for portsender in self.all_senders:
            portsender.send()

    def set_filter(self, filter: str):
        self.filterBindVar.set(filter)

    # method added to keep working scripts, used external_send
    # DEPRECATED. New test modules that want to assemble messages themselves
    #             should use datatypes.py
    def external_send(self, topic: str, value: typing.Any, value_type: str):
        is_insignal = (topic[0:3] == "/o/")
        dataelement_name = topic[3:]
        self.connection_wrapper.connection.send_serialized_dataelement(dataelement_name, value_type, is_insignal, value)
        # ToDo:
        # Before we send signal of we should update the port_sender/widget
        # so that sent value is shown in HMI. This should be done either by
        # inputing value above to app.handle_message(...) or by
        # app.handle_signal(...). Not sure of which of these should be used.
        # Since handle_message(...) calls code in the module that calls
        # external_send(...), then something circular could happen?
        #
        # Once the port_sender/widget is updated, then could use the send function
        # of that, instead of sending json directly, but not sure there is that
        # much benefit.

    def external_send2(self, dataelement_name: str, value: typing.Any):
        self.connection_wrapper.send(dataelement_name, value)

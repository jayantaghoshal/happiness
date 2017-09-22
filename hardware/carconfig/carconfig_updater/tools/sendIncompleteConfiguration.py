from string import Template
import Queue
import time
import dbus

dataElementName = "VehCfgPrm"
vehSigMngrObjectPath = "/com/ihu/VehicleSignalsManager/In"
vehSigMngrIfaceName = "com.ihu.VehicleSignalsManager.In"
propertiesInterface = "org.freedesktop.DBus.Properties"
toSend = Template('{"timestamp": 1467275798, "state": 0, "type": "VehCfgPrm", "value": {"CCPBytePosn2": $V2, "CCPBytePosn3": $V3, "CCPBytePosn4": $V4, "CCPBytePosn5": $V5, "CCPBytePosn6": $V6, "CCPBytePosn7": $V7, "CCPBytePosn8": $V8, "BlkIDBytePosn1": $P1}}')

bus = dbus.bus.BusConnection("tcp:host=198.18.34.1,port=55556")
vehSigMngr = bus.get_object(vehSigMngrIfaceName, vehSigMngrObjectPath)
vehSigMngrIface = dbus.Interface(vehSigMngr, vehSigMngrIfaceName)

stack = Queue.Queue()
offset = 0
for i in range(1, 20):
    for p in range(1, 8):
        if (p + offset) == 18:  # the only two values that doesn't have 1 as a valid value :-(
            stack.put(40)
        if (p + offset) == 198:  # the second one..
            stack.put(7)
        else:
            stack.put(1)
    offset += 7
    time.sleep(0.02)
    vehSigMngrIface.SetDESignal(dataElementName, toSend.substitute(V2=stack.get(), V3=stack.get(), V4=stack.get(), V5=stack.get(), V6=stack.get(), V7=stack.get(), V8=stack.get(), P1=str(i)))

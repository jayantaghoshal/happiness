#emitter acts as a stub sender
import sys
import dbus
from dbus import service
from dbus.mainloop.glib import DBusGMainLoop

DBusGMainLoop(set_as_default=True)

class Emitter(dbus.service.Object):
    def __init__(self):
        bus_obj=dbus.bus.BusConnection('tcp:host=198.18.34.1,port=55556')
        bus_name = dbus.service.BusName('org.volvocars.ParkingClimate_instance', bus=bus_obj)
        dbus.service.Object.__init__(self, bus_name, '/instance')

    @dbus.service.signal(dbus_interface='org.volvocars.ParkingClimate_instance',
                         signature='ctct')

    def RequestTimerListResponse(self,
                                 callstatus,
                                 returnCode,
                                 msg_target_ecu,
                                 sender_handle_id):
        print "Emitting RequestTimerListResponse"
        print "callstatus: %d" % (callstatus)
        print "returnCode: %d" % (returnCode)
        print "Ecu: %d" % (msg_target_ecu)
        print "SenderHandleId: %d" % (sender_handle_id)

    #TimerListChangedNotification
    def TimerListChangedNotification(self, timerList):
        print "Emitting TimerListChangedNotification"

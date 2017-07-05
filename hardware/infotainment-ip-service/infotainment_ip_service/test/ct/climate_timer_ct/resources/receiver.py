import dbus
from dbus import service
from dbus.mainloop.glib import DBusGMainLoop

from gi.repository import GObject as gobject

class Receiver():
    def __init__(self):
        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
        bus=dbus.bus.BusConnection('tcp:host=198.18.34.1,port=55556')
        self.obj = bus.get_object('org.volvocars.climate.ParkingClimate_instance',
                                '/instance')
    # Internal Callback functions
    def OnRequestTimerListAsync(self, dateTime,ParkingTimers,callbak):
        print "Signal ActivationStateNotification received with ActivationState: %d" % activationState
        try:
            self.RequestTimerListAsyncCallback(dateTime,ParkingTimers,callbak)
        except:
            pass

    def OnGetTimerListAttribute(self, callStatus, parkingTimers):
        print "Signal ActivationStateNotification received with ActivationState: %d" % activationState
        try:
            self.GetTimerListAttribute(callStatus, parkingTimers)
        except:
            pass

    # Setup functions
    def RequestTimerListAsync(self):
        self.obj.connect_to_signal("requestTimerListAsync", self.OnRequestTimerListAsync, dbus_interface="org.volvocars.climate.ParkingClimate_instance")
    def GetTimerListAttribute(self):
        self.obj.connect_to_signal("getTimerListAttribute", self.OnGetTimerListAttribute, dbus_interface="org.volvocars.climate.ParkingClimate_instance")

    # Set External Callbacks
    def setRequestTimerListAsyncCallback(self, func_handle):
        self.RequestTimerListAsyncCallback = func_handle
    def setGetTimerListAttributeCallback(self, func_handle):
        self.GetTimerListAttribute = func_handle

    # Misc
    def initMainLoop(self):
        self.loop = gobject.MainLoop()

    def startMainLoop(self):
        self.loop.run()

    def quitMainLoop(self):
        self.loop.quit()

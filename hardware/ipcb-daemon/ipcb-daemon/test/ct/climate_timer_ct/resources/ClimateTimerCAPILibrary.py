from emitter import Emitter
from receiver import Receiver
from Queue import Queue, Empty

import time
import thread

_reader_setup_lock = thread.allocate_lock()

class ClimateTimerCAPIlLibrary(object):
    def __init__(self):
        self.emitter = None
        self.receiver = None

    # Keywords
    def init_emitter_and_receiver(self):
        if self.emitter == None:
            self.emitter = Emitter()
        if self.receiver == None:
            self.receiver = Receiver()

    def wait(self, seconds):
        time.sleep(int(seconds))

    def start_listening(self):
        _reader_setup_lock.acquire()
        thread.start_new_thread( ListenToAll, () )
        while _reader_setup_lock.locked():
            pass
        time.sleep(2)

    def emit_request_timer_list_response_with(self, callstatus=1, returnCode=0, msg_target_ecu=2, sender_handle_id=4):
        self.emitter.RequestTimerListResponse(int(callstatus),
                                              int(returnCode),
                                              int(msg_target_ecu),
                                              int(sender_handle_id))

    def asyncCallBack_should_be(self):
        try:
            if asyncCallReceived != True:
                raise AssertionError('Async Callback not received')
        except Empty:
            raise AssertionError('No update of asyncCallReceived')

    def getAttributeCallBack_should_be(self):
        try:
            if getAttributeReceived != True:
                raise AssertionError('Get Attribute not received')
        except Empty:
            raise AssertionError('No update of getAttributeReceived')

def asyncCallBack():
    global asyncCallReceived
    asyncCallReceived = True


def getAttributeCallBack():
    global getAttributeReceived
    getAttributeReceived = True


# Threads
def ListenToAll():
    receiver = Receiver()
    receiver.setRequestTimerListAsyncCallback(asyncCallBack)
    receiver.setGetTimerListAttributeCallback(getAttributeCallBack)
    _reader_setup_lock.release()
    receiver.startMainLoop()
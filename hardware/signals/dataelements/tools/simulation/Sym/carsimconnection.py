# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import socket
import threading
import time
import json
import sys
if sys.version_info[0] == 3:
    from queue import Queue
else:
    from Queue import Queue
import typing

JsonMessage = typing.Mapping[str, typing.Any]

class RawCarsimMessage():
    def __init__(self, name, state, data):
        # type: (str, int, typing.Optional[str]) -> None
        self.name = name
        self.state = state
        self.data = data

class CarsimDataElementValue():
    def __init__(self, name, state, value):
        # type: (str, int, typing.Any) -> None
        self.name = name
        self.state = state
        self.value = value

CARSIM_PRE_HEADER = b"CarSim"
CARSIM_HEADER_LENGTH = len(b"CarSimXXXX")

#This class starts a thread to handle receiving and sending data.
class CarsimConnection(object):
    def __init__(self):
        self.receive_queue = Queue()    # type: typing.Any
        self.socket = socket.socket()
        self._connected = False
        self.lock = threading.RLock()
        self.thread = threading.Thread(target=self.run, name="CarsimConnection")
        self.thread.daemon =True
        self.thread.start()

    def run(self):
        received_data = b''
        expected_payload_length = -1
        while True:
            if self.connected():
                # Only try to recv data if really needed, because it is blocking and if we already have enough data
                # it would block
                if (expected_payload_length == -1 and len(received_data) < CARSIM_HEADER_LENGTH) or len(received_data) < (expected_payload_length + CARSIM_HEADER_LENGTH):
                    new_data = self.socket.recv(1024)
                    if len(new_data) > 0:
                        logging.debug("Got new data %s, len %d" % (new_data.decode('ascii'), len(new_data)))
                    else:
                        # The socket is closed
                        self.close()
                        continue
                    received_data = received_data + new_data

                logging.debug("Data to process %s, len %d" % (received_data.decode('ascii'), len(received_data)))
                if len(received_data) < len(CARSIM_PRE_HEADER):
                    continue
                if received_data[:len(CARSIM_PRE_HEADER)] != CARSIM_PRE_HEADER:
                    # Got trash, lets disconnect to start over
                    self.close()
                    continue
                if len(received_data) < CARSIM_HEADER_LENGTH:
                    continue
                try:
                    expected_payload_length = int(received_data[len(CARSIM_PRE_HEADER):CARSIM_HEADER_LENGTH].decode('ascii'))
                except:
                    logging.exception("Failed to parse carsim message length")
                    self.close()
                    continue

                total_message_length = CARSIM_HEADER_LENGTH + expected_payload_length
                if len(received_data) < total_message_length:
                    continue

                message_payload = received_data[CARSIM_HEADER_LENGTH:total_message_length ].decode('ascii')
                logging.debug("json str: %s", message_payload)
                try:
                    json_obj = json.loads(message_payload)
                except Exception as e:
                    logging.exception("json.loads() failed for str %s. Error %r", message_payload, e)
                    self.close()
                    continue

                logging.info("signal received: %s " %json_obj["SignalName"])
                dataelements_data_obj = json.loads(json_obj['Data'])
                if dataelements_data_obj["state"] == 0:
                    self.receive_queue.put(RawCarsimMessage(json_obj["SignalName"], dataelements_data_obj["state"], json.dumps(dataelements_data_obj["value"])))   #TODO: dumps hack
                else:
                    self.receive_queue.put(RawCarsimMessage(json_obj["SignalName"], dataelements_data_obj["state"], None))

                received_data = received_data[total_message_length:]
                expected_payload_length = -1
            else:
                received_data = b''
                time.sleep(0.1)

    def connect(self, address, port):
        logging.info("Connecting to %s" % address)
        if self.connected() is False:
            # create an INET, STREAMing socket
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                self.socket.connect((address, port))
                self._set_connected(True)
                logging.info("Connected to %s" % address)
            except:
                raise

    def connected(self):
        with self.lock:
            return self._connected

    def _set_connected(self, value):
        with self.lock:
            self._connected = value


    def send_serialized_dataelement(self, dataelement_name, typestr, is_insignal, value_json_serialized):
        #type: (str, str, bool, str) -> None
        toSend = {
            "SignalName": dataelement_name,
            "Dir": 0 if is_insignal else 1,
            # NOTE: Data is an opaque string(that happens to contain json) from carsimconnections perspective, not a json object!!!
            "Data": json.dumps({
                "state": 0,
                "timestamp": int(time.time()),
                "type": typestr,
                "value": json.loads(value_json_serialized), #TODO: Ugh...
            })
        }
        self._send(json.dumps(toSend))

    def send_error(self, dataelement_name, is_insignal, typestr):
        # type: (str, bool, str) -> None
        toSend = {
            "SignalName": dataelement_name,
            "Dir": 0 if is_insignal else 1,
            "Data": json.dumps({
                "state": 1,
                "errorCode": 0,
                "timestamp": int(time.time()),
                "type": typestr
            })
        }
        self._send(json.dumps(toSend))

    def _send(self, data):
        # type: (str) -> None
        buffer = data.encode(encoding='ascii')
        #Yea, the lenght is encoded in ascii...
        lengthString = '{0:04d}'.format(len(buffer))
        logging.debug("Sending %r" % data)
        self.socket.sendall('CarSim'.encode(encoding='ascii') + lengthString.encode(encoding='ascii') + buffer)

    def close(self):
        if self._connected:
            self.socket.shutdown(socket.SHUT_RDWR)
            self.socket.close()
            self._set_connected(False)
# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging

from .tcamconnection import TcamConnection
from .wlanservice import WlanService

# import generalhelper


class MessageHandler(object):

    _MESSAGE_TYPES = {
        'REQUEST': '00',
        'SET_REQUEST': '02',
        'RESPONSE': '04',
        'NOTIFICATION': '05',
        'ACK': '70'
    }

    def __init__(self, message_callback):
        self._logger = logging.getLogger('MESSAGE HANDLER')

        self._message_callback = message_callback
        self._tcam = TcamConnection(self.receive_msg)
        self._ihu_ip_address = self._tcam.ihu_ipcb_address
        self._pbu_header_length = 8
        self._ipcb_protocol_version = '03'

    def register_callback(self, message_callback):
        self._message_callback = message_callback

    def receive_msg(self, data, address):
        (source_ip, source_port) = address
        self._logger.info(
            "Received Message from IP: %s:%s with payload: [%s]", source_ip, source_port, data.encode('hex'))

        if source_ip == self._ihu_ip_address:
            msg = self.deconstruct_pdu(data)

            if msg['operation_type'] == self._MESSAGE_TYPES['REQUEST'] or msg['operation_type'] == self._MESSAGE_TYPES['SET_REQUEST']:
                self._logger.info('Sending ACK')
                ack = self._construct_ack(
                    msg['service_id'], msg['operation_id'], msg['sender_handle_id'])
                self.send_message(ack)
            elif msg['operation_type'] == self._MESSAGE_TYPES['ACK']:
                self._logger.info('Recevied ACK')
                return

            self._message_callback(msg)

        else:
            self._logger.info('Not from IHU: Ignoring')

    def _construct_ack(self, service_id, operation_id, sender_handle_id):
        message_length = self._pbu_header_length
        message_length_hex_str = "{0:0{1}x}".format(message_length, 8)

        header = service_id + operation_id + message_length_hex_str + \
            sender_handle_id + self._ipcb_protocol_version + '70' + '0000'
        header_encoded = header.decode('hex')

        return header_encoded + bytearray()

    def deconstruct_pdu(self, pdu):
        self._logger.info('Deconstructing %s', pdu.encode('hex'))

        service_id = pdu[0:2].encode('hex')
        operation_id = pdu[2:4].encode('hex')
        message_length = pdu[4:8].encode('hex')
        sender_handle_id = pdu[8:12].encode('hex')
        protocol_version = pdu[12:13].encode('hex')
        operation_type = pdu[13:14].encode('hex')
        data_type = pdu[14:15].encode('hex')  # Ignore, not used
        reservedII = pdu[15:16].encode('hex')  # Ignore, not used

        payload = pdu[16:].encode('hex')

        self._logger.debug(
            "Deconstructed Message:\nservice_id: %s, " +
            "\noperation_id: %s, \nmessage_length: %s, " +
            "\nSenderHandle: %s, \nprotocol_version: %s, " +
            "\noperation_type: %s, \ndata_type: %s, " +
            "\nReservedII: %s, \nPayload: [%s]",
            service_id, operation_id, message_length, sender_handle_id, protocol_version,
            operation_type, data_type, reservedII, payload)

        msg = {}
        msg['service_id'] = service_id
        msg['operation_id'] = operation_id
        msg['sender_handle_id'] = sender_handle_id
        msg['operation_type'] = operation_type
        msg['payload'] = payload
        return msg

    def construct_pdu(self, service_id, operation_id, operation_type, payload):
        message_length = self._pbu_header_length + len(payload)
        message_length_hex_str = "{0:0{1}x}".format(message_length, 8)

        sender_handle_str = service_id[
            -2:] + operation_id[-2:] + operation_type + '00'

        header = service_id + operation_id + message_length_hex_str + \
            sender_handle_str + self._ipcb_protocol_version + \
            operation_type + '0000'
        header_encoded = header.decode('hex')

        return header_encoded + payload

    def construct_response(self, service_id, operation_id, sender_handle_id, operation_type, payload):
        message_length = self._pbu_header_length + len(payload)
        message_length_hex_str = "{0:0{1}x}".format(message_length, 8)

        header = service_id + operation_id + message_length_hex_str + \
            sender_handle_id + self._ipcb_protocol_version + operation_type + '0000'
        header_encoded = header.decode('hex')

        return header_encoded + payload

    def send_message(self, message):
        self._tcam.send_packet(message)

    def start(self):
        self._tcam.start_listening()

    def stop(self):
        self._tcam.stop_listening()

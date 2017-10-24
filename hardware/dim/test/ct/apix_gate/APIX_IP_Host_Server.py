#!/usr/bin/python
import socket
import sys
from thread import *

class ApixIpHostServer():

    def __init__(self):
        self.HOST = '198.18.24.1'
        self.PORT = 50001
        self.SEP = '\n'

    def run_server(self):
        print('Starting host socket server')
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            server_socket.bind((self.HOST, self.PORT))
        except socket.error as msg:
            print('Binding ServerSocket failed, Error code: ' +  str(msg[0]) + ' Message: ' + msg[1])
            server_socket.close()
            sys.exit()

        server_socket.listen(5)
        print('Listening for incoming connections')
        while(1):
            conn, addr = server_socket.accept()
            print('Connection from: ' + addr[0] + " : " + str(addr[1]))
            start_new_thread(self.client_thread, (conn,))
        server_socket.close()

    def client_thread(self, connection):
        print('Client thread started')
        buf = ''
        while self.SEP not in buf:
            buf = connection.recv(64)
            if not buf:
                break
        if self.is_integer(buf.rstrip('\n'' ')):
            mess_recvd = int(buf)
            print('Server received message: ' + str(mess_recvd))
            mess_resp = str(mess_recvd+2) + self.SEP
            connection.send(mess_resp)
        connection.close()
        print('Client thread killed')

    def is_integer(self, i):
        try:
            int(i)
            return True
        except ValueError:
            return False

    def main(self):
        self.run_server()

if __name__ == "__main__":
     server = ApixIpHostServer()
     server.main()

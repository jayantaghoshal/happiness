# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import socket
import time

str1 = '{"Data":"{\\"state\\":0,\\"timestamp\\":1321034276874,\\"type\\":\\"RotyDirUI1\\",\\"value\\":1}","Dir":0,"SignalName":"RotyDirReq1"}'
str2 = '{"Data":{"state":0,"timestamp":1321034276874,"type":"RotyDirUI1","value":1},"Dir":0,"SignalName":"RotyDirReq1"}'

response1 = bytes('CarSim' + '%04d' % len(str1) + str1, 'ascii')
response2 = bytes('CarSim' + '%04d' % len(str2) + str2, 'ascii')

def run_server(port):
    even = False
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("localhost", 8080))
    s.listen(1)
    print("Waiting for connection...")
    while True:
        q, addr = s.accept()
        print("Accepted connection!")
        while True:
            try:
                res, even = (response2, False) if even else (response1, True)
                print("Send json %s", res)
                q.sendall(res)
                time.sleep(1)
                try:
                    input = q.recv(4096, 0x40)  #recv in non-blocking mode
                except BlockingIOError:
                    continue
                if len(input) > 0:
                    print("Received %s", input.decode('ascii'))
            except (BrokenPipeError, ConnectionResetError):
                print("Connection terminated, waiting for connection...")
                break


if __name__ == '__main__':
    run_server(8080)
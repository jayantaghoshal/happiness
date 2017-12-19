#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
import os
import urllib
import json
from cloud_logic import CloudLogic
from http.server import BaseHTTPRequestHandler, HTTPServer


# HTTPRequestHandler class


class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):
    # GET
    def do_GET(self):
        # Send response status code
        self.send_response(200)

        # Send headers
        self.send_header('Content-type', 'text/html')
        self.end_headers()

        # Create request for and send to CloudLogic
        req = {}
        req['type'] = "GET"
        req['path'] = self.path
        req['version'] = self.protocol_version

        print(json.dumps(req, indent=4))
        message = cloud_logic_.get_rsp(json.dumps(req))

        # Send message back to client
        # Write content as utf-8 data
        self.wfile.write(bytes(message, "utf8"))
        return

    def do_PUT(self):
        path = self.path
        if path.endswith('/'):
            self.send_response(405, "Method Not Allowed")
            self.wfile.write("PUT not allowed on a directory\n".encode())
            return
        else:
            length = int(self.headers['Content-Length'])
            data = self.rfile.read(length)
            data = data.decode("utf8")
            # Call CloudLogic to set up test environment
            cloud_logic_.set_up(data)
            self.send_response(201, "Created")

    def do_GET_Statistics(self):
        # Send response status code
        self.send_response(200)

        # Send headers
        self.send_header('Content-type', 'text/html')
        self.end_headers()

        statistics = cloud_logic_.get_statistics()
        self.wfile.write(bytes(statistics, "utf8"))

    def do_RESET(self):
        cloud_logic_.reset()

def run():
    print('starting server...')

# Server settings
# Choose port 8081, for port 80, which is normally used for a http server, you need root access
    server_address = ('127.0.0.1', 8081)
    httpd = HTTPServer(server_address, testHTTPServer_RequestHandler)
    print('running server...')
    httpd.serve_forever()

cloud_logic_ = CloudLogic()
run()

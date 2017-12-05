#!/usr/bin/env python3

import http.client
import requests
import json


class TestClient:

    conn = ""

    def connect_to_server(self):
        self.conn = http.client.HTTPConnection("127.0.0.1", 8081)

    def close_connection(self):
        self.conn.close()

    def prepare_test_environment(self, rule: str):
        self.connect_to_server()
        self.conn.request("PUT", "127.0.0.1:8081/test", rule)
        self.close_connection()

    def get_statistics(self):
        self.connect_to_server()
        self.conn.request("GET_Statistics",
                          "127.0.0.1:8081/test", json.dumps(req))
        response = self.conn.getresponse()
        data = response.read()
        self.close_connection()
        return data

    def reset_test_environment(self):
        self.connect_to_server()
        self.conn.request("RESET",  "127.0.0.1:8081/test")
        self.close_connection()



###EXAMPLE USAGE###
'''tc = TestClient()

data = {}
req = {}
rsp = {}

req['type'] = "GET"
req['path'] = "/test"
req['version'] = "HTTP/1.0"

rsp['version'] = "HTTP/1.0"
rsp['status_code'] = "200"
rsp['status'] = "OK"
rsp['data'] = "Hello World!"

data['request'] = req
data['response'] = rsp

data = json.dumps(data, indent=4)

tc.prepare_test_environment(data)

tc.connect_to_server()
tc.conn.request("GET", "/test")
response = tc.conn.getresponse()
data = response.read()
print(data)
tc.close_connection()
print(tc.get_statistics())
#tc.reset_test_environment()
#print(tc.get_statistics())'''

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import unittest
import sys
import os
import json

sys.path.insert(1, os.path.join(os.path.dirname(__file__), ".."))
from cloud_logic import CloudLogic


class TestCloudSim(unittest.TestCase):

    cloud_logic_ = CloudLogic()

    #Define requests and expected responsess
    req_1_ = ('{'
              '"type": "GET",'
              '"path": "/path",'
              '"version": "HTTP/1.1",'
              '"headers": [ {"key": "value1"}, {"key2": "value2"}]'
              '}')

    req_2_ = ('{'
              '"type": "GET",'
              '"path": "/path",'
              '"version": "HTTP/1.1",'
              '"headers": [ {"key2": "value2"}]'
              '}')

    req_404_ = ('{'
               '"type": "GET",'
               '"path": "/some/other/path",'
               '"version": "HTTP/1.1",'
               '"headers": [ {"key": "value1"}]'
               '}')

    expected_rsp_1_ = ('{'
                       '"version": "HTTP/1.1",'
                       '"status_code": "200",'
                       '"status": "OK",'
                       '"data": "Hello World!"'
                       '}')
    expected_rsp_2_ = ('{'
                       '"version": "HTTP/1.1",'
                       '"status_code": "200",'
                       '"status": "OK",'
                       '"data": "Hello World, AGAIN!"'
                       '}')

    expected_404_rsp_ = ('{'
                             '"code": 404,'
                             '"error": {'
                             '"errors": ['
                             '{'
                             '"domain": "global",'
                             '"message": "Not Found",'
                             '"reason": "notFound"'
                             '}'
                             ']'
                             '},'
                             '"message": "Not Found"'
                             '}')

    def match(self, expected, actual):
        a,b = json.dumps(expected, sort_keys=True), json.dumps(actual, sort_keys=True)
        if a == b:
            return True
        return False

    def tearDown(self):
        self.cloud_logic_.reset()

    def setUp(self):
        rule_1 = ('{'
                  '"request":'
                  '{'
                  '"type": "GET",'
                  '"path": "/path",'
                  '"version": "HTTP/1.1",'
                  '"headers": [ {"key": "value1"} ]'
                  '},'
                  '"response":'
                  '{'
                  '"version": "HTTP/1.1",'
                  '"status_code": "200",'
                  '"status": "OK",'
                  '"data": "Hello World!"'
                  '}'
                  '}')
        rule_2 = ('{'
                  '"request":'
                  '{'
                  '"type": "GET",'
                  '"path": "/path",'
                  '"version": "HTTP/1.1",'
                  '"headers": [ ]'
                  '},'
                  '"response":'
                  '{'
                  '"version": "HTTP/1.1",'
                  '"status_code": "200",'
                  '"status": "OK",'
                  '"data": "Hello World, AGAIN!"'
                  '}'
                  '}')

        self.cloud_logic_.set_up(rule_1)
        self.cloud_logic_.set_up(rule_2)

    def test_get_correct_rsp_1(self):
        rsp = self.cloud_logic_.get_rsp(self.req_1_)

        self.assertTrue(self.match(json.loads(
            self.expected_rsp_1_), json.loads(rsp)))

    def test_get_correct_rsp_2(self):
        rsp = self.cloud_logic_.get_rsp(self.req_2_)

        self.assertTrue(self.match(json.loads(
            self.expected_rsp_2_), json.loads(rsp)))

    def test_get_404_error_rsp(self):
        rsp = self.cloud_logic_.get_rsp(self.req_404_)

        self.assertTrue(self.match(json.loads(
            self.expected_404_rsp_), json.loads(rsp)))

    def test_muliple_gets_and_get_statistics(self):

        rsp_1 = self.cloud_logic_.get_rsp(self.req_1_)
        rsp_2 = self.cloud_logic_.get_rsp(self.req_2_)
        rsp_3 = self.cloud_logic_.get_rsp(self.req_404_)

        self.assertTrue(self.match(json.loads(
            self.expected_rsp_1_), json.loads(rsp_1)))
        self.assertTrue(self.match(json.loads(
            self.expected_rsp_2_), json.loads(rsp_2)))
        self.assertTrue(self.match(json.loads(
            self.expected_404_rsp_), json.loads(rsp_3)))

        statistics = self.cloud_logic_.get_statistics()

        expected_statistics = ('{'
                               '"nr_of_failed_requests": 1,'
                               '"nr_of_requests": 3'
                               '}')

        self.assertTrue(self.match(json.loads(
            expected_statistics), json.loads(statistics)))

    def test_bad_request(self):
        req = ('{'
               '"type": "GET",'
               '"path": "/some/other/path",'
               '"version": "HTTP/1.1",'
               '"headers": [ {"key": "value1"}]'
               )

        rsp = self.cloud_logic_.get_rsp(req)

        expected_rsp = ('{'
                        '"code": 300,'
                        '"error": {'
                        '"errors": ['
                        '{'
                        '"domain": "global",'
                        '"message": "Bad Request",'
                        '"reason": "badRequest"'
                        '}'
                        ']'
                        '},'
                        '"message": "Bad Request"'
                        '}')

        self.assertTrue(self.match(json.loads(expected_rsp), json.loads(rsp)))


if __name__ == '__main__':
    unittest.main()

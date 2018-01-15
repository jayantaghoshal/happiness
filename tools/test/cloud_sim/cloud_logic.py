#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json

class CloudLogic:
    req_rsp_rules = []
    nr_of_requests = 0
    nr_of_failed_requests = 0

    # Prepare Test Environment
    def set_up(self, data: str):
        try:
            d = json.loads(data)
            self.req_rsp_rules.append(d)
        except ValueError:
            print("Error in set_up, cannot prepare test server...")
            return build_error_msg(300, 'Bad Request', 'global', 'badRequest')

    # Reset Test Environment
    def reset(self):
        self.req_rsp_rules = []
        self.nr_of_failed_requests = 0
        self.nr_of_requests = 0

    # Get Response corresponding to request (according to specified rules)
    # Returns string representation of the response (if request matched any rule)
    def get_rsp(self, request: str):
        try:
            request = json.loads(request)
        except ValueError:
            print("Error in parsing request")
            return self.build_error_msg(300, 'Bad Request', 'global', 'badRequest')

        self.nr_of_requests += 1
        i = 0
        for rule in self.req_rsp_rules:
            req_rule = rule['request']

            if self.match(req_rule, request):
                return self.req_rsp_rules[i]['response']

            i += 1

        self.nr_of_failed_requests += 1
        return self.build_error_msg(404, 'Not Found', 'global', 'notFound')

    # Match rule with request
    # Returns true if all key-value pairs defined in rule matches request (don't care if there is extra keys in the request)
    def match(self, rule, request):
        # check that all keys in rule is defined in request
        # else there is no match
        if set(rule).issubset(set(request)):
            for key in rule.keys():
                # if type of key is list => check that all entries in rule are specified in request (don't care about extra)
                if type(rule.get(key)) is list:
                    for item in rule.get(key):
                        if item not in request.get(key):
                            return False
                elif rule.get(key) == request.get(key):
                    pass
                else:
                    return False
        else:
            return False

        return True

    # https://cloud.google.com/storage/docs/json_api/v1/status-codes (2017-11-30)
    def build_error_msg(self, code, msg, domain, reason):
        error_msg = {}  # outer container
        error = {}  # error obj
        error['domain'] = domain
        error['reason'] = reason
        error['message'] = msg

        error_list = []  # list of errors to add to error obj
        error_list.append(error)

        errors = {}  # obj of list of errors
        errors['errors'] = error_list

        error_msg['error'] = errors
        error_msg['code'] = code
        error_msg['message'] = msg

        return json.dumps(error_msg)

    def get_statistics(self):
        data = {}
        data["nr_of_requests"] = self.nr_of_requests
        data["nr_of_failed_requests"] = self.nr_of_failed_requests
        return json.dumps(data)

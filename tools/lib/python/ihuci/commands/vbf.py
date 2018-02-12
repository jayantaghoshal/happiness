# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import redis
from .base import BaseCommand

class RedisClient(object):

    def __init__(self):
        self._redis = None

    def set(self, key, value):
        return self.redis().set(key, value)

    def get(self, key):
        value = self.redis().get(key)
        return value

    def redis(self):
        if self._redis is None:
            host = "gotsvl1416.got.volvocars.net"
            self._redis = redis.StrictRedis(host=host,
                                            port=6379,
                                            db=0,
                                            decode_responses=True)
        return self._redis

class VBF(object):

    def __init__(self, sw_tag, build_type):
        self._sw_tag = sw_tag
        self._build_type = build_type # eng, userdebug & debug

    def name(self):
        return "%s%s" % (self.pno, self.suffix)

    @property
    def pno(self):
        return RedisClient().get(self._pno_key())

    @pno.setter
    def pno(self, pno):
        return RedisClient().set(self._pno_key(), pno)

    @property
    def suffix(self):
        return RedisClient().get(self._suffix_key())

    @suffix.setter
    def suffix(self, suffix):
        return RedisClient().set(self._suffix_key(), suffix)

    def inc(self):
        suffix_list = list(self.suffix)
        for index, c in reversed(list(enumerate(suffix_list))):
            if ord(c) < ord("Z"):
                suffix_list[index] = chr((ord(c) + 1))
                break
        self.suffix = "".join(suffix_list)
        return self.suffix

    def _pno_key(self):
        return "%s.pno" % self._base_key()

    def _suffix_key(self):
        return "%s.suffix" % self._base_key()

    def _base_key(self):
        return "icup_android.vbf.%s.%s" %(self._sw_tag, self._build_type)


class VBFCommand(BaseCommand):

    def __init__(self, options):
        super().__init__(options)
        self._options = options

    def run(self):
        vbf = VBF(self._options.sw_tag.lower(), self._options.build_type)
        if self._options.action == "get":
           print(vbf.name())
        elif self._options.action == "set":
            vbf.pno = self._options.part_number
            vbf.suffix = self._options.suffix
        elif self._options.action == "inc":
            vbf.inc()





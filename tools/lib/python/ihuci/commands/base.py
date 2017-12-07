# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import abc

class BaseCommand(metaclass=abc.ABCMeta):

    def __init__(self, options):
        self._options = options

    @abc.abstractmethod
    def run(self):
        """Execute command"""

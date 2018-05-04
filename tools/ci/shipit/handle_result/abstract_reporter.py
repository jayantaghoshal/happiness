# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from abc import ABC, abstractmethod
from typing import List
from shipit.testscripts import NamedTestResult
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from utilities.ihuhandler import FlashResult

class abstract_reporter(ABC):

    def __init__(self):
        pass

    @abstractmethod
    def plan_started(self) -> None:
        pass

    @abstractmethod
    def plan_finished(self, test_results: List[NamedTestResult]) -> None:
        pass

    @abstractmethod
    def module_started(self, test: IhuBaseTest) -> None:
        pass

    @abstractmethod
    def module_finished(self, test: IhuBaseTest, test_result: ResultData) -> None:
        pass

    @abstractmethod
    def flash_started(self) -> None:
        pass

    @abstractmethod
    def flash_finished(self, result: FlashResult) -> None:
        pass

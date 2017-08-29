import contextlib
import sys
import logging
import time
sys.path.append("..")
from fdx import fdx_client
from fdx import fdx_description_file_parser
from . import config


ns_per_ms = 1000000


class FDXClientTestBase():
    def __init__(self):
        self.fdx_client = fdx_client.FDXConnection(self.data_exchange, config.vector_fdx_ip, config.vector_fdx_port)
        try:
            self.power_supply = None
            (self.groups, self.sysvar_list, self.signal_list) = fdx_description_file_parser.parse(
                config.fdx_description_file_path)
            self.group_id_map = {g.group_id: g for g in self.groups}

            logging.info("Waiting for simulation to start")
            self.fdx_client.send_start()
            self.fdx_client.confirmed_start()
            logging.info("Simulation started")
            for g in self.groups:
                if "IHUBackBone" in g.name:
                    logging.debug("Subscribing for %s" % g.name)
                    self.fdx_client.send_free_running_request(g.group_id,
                                                              fdx_client.kFreeRunningFlag.transmitCyclic,
                                                              500 * ns_per_ms,
                                                              0)
        except:
            self.fdx_client.close()
            raise

    def close(self):
        self.fdx_client.confirmed_stop()
        self.fdx_client.close()

    def data_exchange(self, group_id, data):
        g = self.group_id_map[group_id]
        g.receive_data(data)

    def get_signal(self, name):
        # type: (str) -> fdx_description_file_parser.Item
        candidate = None # type: fdx_description_file_parser.Item

        parts = name.split("::")
        signame = parts[-1]
        msg = None
        if len(parts) > 1:
            msg = parts[-2]

        for i in self.signal_list:
            if i.name == signame and (msg is None or msg == i.msg_or_namespace):
                if candidate is not None:
                    raise Exception("Signal name '%s' is ambiguous between %s::%s and %s::%s" %
                                    (name, candidate.msg_or_namespace, candidate.name, i.msg_or_namespace, i.name))
                candidate = i
        if candidate is None:
            raise Exception("Signal %s not found" % name)
        return candidate


def wait_for(predicate, timeout_sec, polling_interval_sec=0.05):
    deadline = time.time() + timeout_sec
    while time.time() < deadline:
        if predicate():
            return True
        time.sleep(polling_interval_sec)
    raise Exception("predicate in wait_for did not pass after %d sec" % timeout_sec)


class TestCase(FDXClientTestBase):
    def example_test_case(self):
        self.set_fan_level_in_hmi(5)
        fanlvl_signal = self.get_signal("HmiHvacFanLvlFrnt")
        wait_for(lambda: fanlvl_signal.value_raw == 5, timeout_sec=1)

    def set_fan_level_in_hmi(self, level):
        pass


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.DEBUG)
    with contextlib.closing(TestCase()) as tc:
        tc.example_test_case()

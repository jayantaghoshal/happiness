import contextlib
import logging
import time
from examples import config
import sys
sys.path.append("..")
from fdx import fdx_client, fdx_description_file_parser

ns_per_ms = 1000000


def main():
    (groups, sysvar_list, signal_list) = fdx_description_file_parser.parse(config.fdx_description_file_path)

    group_id_map = {g.group_id: g for g in groups}

    old_values = {}

    def data_exchange(group_id, data):
        g = group_id_map[group_id]
        g.receive_data(data)
        for i in g.items:
            prev = old_values.get(i.name)
            if prev is not None and prev != i.value_raw:
                print("%s::%s = %d" %(g.name, i.name, i.value_raw))
            old_values[i.name] = i.value_raw

    def get_signal(name: str) -> fdx_description_file_parser.Item:
        candidate = None

        parts = name.split("::")
        signame = parts[-1]
        msg = None
        if len(parts) > 1:
            msg = parts[-2]

        for i in signal_list:
            if i.name == signame and (msg is None or msg == i.msg_or_namespace):
                if candidate is not None:
                    raise Exception("Signal name '%s' is ambiguous between %s::%s and %s::%s" %
                                    (name, candidate.msg_or_namespace, candidate.name, i.msg_or_namespace, i.name))
                candidate = i
        if candidate is None:
            raise Exception("Signal %s not found" % name)
        return candidate

    with contextlib.closing(fdx_client.FDXConnection(data_exchange, config.vector_fdx_ip, config.vector_fdx_port)) as conn:
        def send(name: str, raw_value: int):
            item = get_signal(name)
            item._value = raw_value
            g = item.parent_group
            data = g.build_data()
            logging.debug("Sending group: %d" % g.group_id)
            conn.send_data_exchange(g.group_id, g.size, data)

        print("Waiting for simulation to start")
        conn.confirmed_start()
        print("Simulation started")

        send("UsgModSts", 13)

        groups_to_subscribe = [g for g in groups if "IHUBackBone" in g.name]
        for g in groups_to_subscribe:
            print("Subscribing for %s" % g.name)
            conn.send_free_running_request(g.group_id, fdx_client.kFreeRunningFlag.transmitCyclic, 500 * ns_per_ms, 0)
        time.sleep(10)
        for g in groups_to_subscribe:
            conn.send_free_running_cancel(g.group_id)

        conn.send_stop()
        print("-------- DONE -------")


if __name__ == "__main__":
    logging.getLogger().setLevel(logging.INFO)
    main()
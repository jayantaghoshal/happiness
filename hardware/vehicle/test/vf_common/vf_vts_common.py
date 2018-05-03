# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import typing
from vts.runners.host import asserts



def combineMsg(coreMsg, commentMsg):
    if commentMsg:
        return "%s, comment: %s" % (coreMsg, commentMsg)
    return coreMsg


T = typing.TypeVar('T')
def wait_for(get_function, expected_value, timeout_sec, extra_message=None):
    # type: (typing.Callable[[], T], T, int, str) -> None
    end = time.time() + timeout_sec
    read_value = get_function()
    while time.time() < end:
        read_value = get_function()
        if read_value == expected_value:
            break
        time.sleep(0.2)
    asserts.assertEqual(read_value, expected_value,
                        "Expected get function to be %s within %d sec, got %s. %s)" %
                        (expected_value, timeout_sec, read_value, extra_message))

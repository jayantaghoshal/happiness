# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import os
import time
from shipit.process_tools import check_output_logged


logger = logging.getLogger(__name__)

class FlashResult():
    def __init__(self):
        self.success = False
        self.attempts = 0
        self.info = None
        self.flashtime = 0
        self.totaltime = 0


def flash_ihu(max_attempts=3, power_cycle_length=120):
    logger.info("Updating IHU")
    result = FlashResult()
    totalTime = time.time()
    for attempt in range(max_attempts):
        try:
            startFlashTime = time.time()
            result.attempts += 1
            check_output_logged(['ihu_update', '--profile=ci-machinery'])
            logger.debug("Updating IHU complete")
            elapsedFlashTime = time.time() - startFlashTime
            result.info = "Flashing IHU complete"
            result.flashtime = elapsedFlashTime
            result.success = True
            break
        except Exception as e:
            logger.error("Updating ihu failed: {}".format(e))
            if attempt == max_attempts - 1: # The last attempt failed
                result.success = False
                result.info = "Flashing IHU failed!"
            else:
                logger.info("Power cycling ihu and retry")
                boot_script_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, 'jenkins', 'ihu_ipm_reboot.py'))
                power_result = os.system("python3 " + boot_script_path + " " + str(power_cycle_length))
                if power_result != 0:
                    result.success = False
                    result.info = "Could not power cycle ihu"
    totalElapsedTime = time.time() - totalTime
    result.totaltime = totalElapsedTime
    logger.info("Update done with result: {}".format(str(result.success)))
    return result

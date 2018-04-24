# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import os

from shipit.process_tools import check_output_logged


logger = logging.getLogger(__name__)


def flash_ihu(max_attempts=3, power_cycle_length=120):
    logger.info("Updating IHU")
    for attempt in range(max_attempts):
        try:
            check_output_logged(['ihu_update', '--profile=ci-machinery'])
            logger.debug("Updating IHU complete")
            break
        except Exception as e:
            logger.error("Updating ihu failed: {}".format(e))
            if attempt == max_attempts - 1: # The last attempt failed
                raise Exception("Updating IHU failed, reached max attempts: {}".format(max_attempts))
            else:
                logger.info("Power cycling ihu and retry")
                boot_script_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, 'jenkins', 'ihu_ipm_reboot.py'))
                power_result = os.system("python3 " + boot_script_path + " " + str(power_cycle_length))
                if power_result != 0:
                    raise Exception("Could not power cycle ihu")
    logger.info("Update of IHU done")


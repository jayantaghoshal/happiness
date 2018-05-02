# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import tarfile
import sys
import os
from shipit.test_runner.test_env import vcc_root
sys.path.append(vcc_root)
from tools.ci.artifactory.artifactory.artifactory import Artifactory as Artifactory # NOQA

logger = logging.getLogger(__name__)

def download_out(path: str):
    logger.info("Downloading out")
    uri = "ICUP_ANDROID_CI/" + path
    Artifactory().retrieve_artifact(uri, os.getcwd())
    with tarfile.open("out.tgz") as f:
        f.extractall()

def download_latest(project: str, file: str='out.tgz'):
    logger.info("Downloading latest out")
    artifactory = Artifactory()
    uri = "ICUP_ANDROID_CI/" +project
    latest_job = artifactory.retreive_latest_uri(uri)
    uri = latest_job + "/" + file
    artifactory.retrieve_artifact(uri, os.getcwd())
    with tarfile.open(file) as f:
        logger.info("Extracting {}".format(file))
        f.extractall()

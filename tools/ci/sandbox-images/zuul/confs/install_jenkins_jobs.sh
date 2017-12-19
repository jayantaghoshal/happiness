#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Has to be done while jenkins is online, so log onto the docker container manually and do it

export http_proxy=
export https_proxy=
jenkins-jobs --conf=/etc/jenkins_jobs/jenkins_jobs.ini update /etc/jenkins_jobs/jobs/
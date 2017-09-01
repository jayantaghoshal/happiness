#!/bin/bash
# Has to be done while jenkins is online, so log onto the docker container manually and do it

export http_proxy=
export https_proxy=
jenkins-jobs --conf=/etc/jenkins_jobs/jenkins_jobs.ini update /etc/jenkins_jobs/jobs/
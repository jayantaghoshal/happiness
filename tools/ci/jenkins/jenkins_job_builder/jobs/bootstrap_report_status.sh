#!/bin/sh

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set +e

echo "Reporting job status"

get_repos() {
    (bootstrap_docker_run "repo init --reference=/cm/repo-mirror -u ${GIT_URL} -b ${GIT_COMMIT}" || (echo "repo init failed" && exit 1)) &&
    (bootstrap_docker_run "repo sync --no-clone-bundle --current-branch --force-sync -q -j4 vendor/volvocars" || (echo "repo sync failed" && exit 1))
}

clean_all() {
    find . -type f -delete && find . -type l -delete && find . -type d -empty -delete
}

(get_repos || (clean_all && get_repos)) &&
(./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh python3 vendor/volvocars/tools/ci/shipit/report_job_status.py -j "${JOB_NAME}" -n "${BUILD_NUMBER}" -g "${GIT_COMMIT}" || (echo "report job status failed" && exit 1)) ||
exit 0 # always green

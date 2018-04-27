#!/bin/sh

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set +e

echo "Reporting job status"

# This file needs to be created last in the job.
# That only happens if the job has been successful so far.
if [ -f "${JOB_NAME}".success ];
then
    BUILD_STATUS=pass
else
    BUILD_STATUS=fail
fi

bootstrap_docker_run "repo init --reference=/cm/repo-mirror -u ${GIT_URL} -b ${GIT_COMMIT}" || (echo "repo init failed" && exit 0)
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch -q -j4 vendor/volvocars" || (echo "repo sync failed" && exit 0)

./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh python3 vendor/volvocars/tools/ci/shipit/report_job_status.py "${JOB_NAME}" "${BUILD_NUMBER}" "${BUILD_STATUS}" "${GIT_COMMIT}" || (echo "report job status failed" && exit 0)

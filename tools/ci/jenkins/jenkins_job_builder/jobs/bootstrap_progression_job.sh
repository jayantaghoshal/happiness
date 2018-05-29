#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# vendor/volvocars is downloaded through jenkins plugin

## Do an update to the manifest
## Make local commit
## Send commit to Gerrit
## Use rgerrit to set +1 Verify, +2 Review and Automerge +1 to trigger the pipeline and test the manifest change

cd "$WORKSPACE_ROOT"

##
# Repo sync
#

bootstrap_docker_run "repo init --reference=/cm/repo-mirror -u ssh://gotsvl1415.got.volvocars.net:29421/manifest -b master" || die "repo init failed"
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j32 vendor/volvocars" || die "repo sync failed"

./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh --multiuser "python3 $REPO_ROOT_DIR/vendor/volvocars/tools/ci/shipit/bump.py . update_manifest_for_progression ${GERRIT_PROJECT}"
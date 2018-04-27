#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -uex

die() { echo "$@" 1>&2 ; exit 1; }

echo "Gerrit commit that is tested"
echo "https://icup_android.gerrit.cm.volvocars.biz/#/c/${ZUUL_CHANGE}"

################################################################################################
##
# Configure Docker
# During the first steps we run with a fixed revision of the container
# This is only to get a container with "repo" and "zuul" installed
# Once we have cloned the vendor/volvocars repo we use the container specified by vendor/volvocars/tools/docker_build
#
BOOTSTRAP_DOCKER_IMAGE=swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build:If943907d331a19834bdfea658f72144a0e503a08

export WORKSPACE_ROOT
WORKSPACE_ROOT=$(pwd)

bootstrap_docker_run () {
    docker run \
    --hostname aic-docker \
    --volume "$HOME":"$HOME":rw \
    --volume "$WORKSPACE_ROOT":"$WORKSPACE_ROOT" \
    --volume /cm/repo-mirror:/cm/repo-mirror \
    --volume /dev/bus/usb:/dev/bus/usb \
    --env=HOST_UID="$(id -u)" \
    --env=HOST_GID="$(id -g)" \
    --env=HOST_UNAME="$(id -un)" \
    --env=HOME="$HOME" \
    --env ZUUL_UUID \
    --env ZUUL_REF \
    --env ZUUL_COMMIT \
    --env ZUUL_PROJECT \
    --env ZUUL_PIPELINE \
    --env ZUUL_URL \
    --env BASE_LOG_PATH \
    --env LOG_PATH \
    --env ZUUL_VOTING \
    --env ZUUL_BRANCH \
    --env ZUUL_CHANGE \
    --env ZUUL_CHANGES \
    --env ZUUL_CHANGE_IDS \
    --env ZUUL_PATCHSET \
    --env ZUUL_OLDREV \
    --env ZUUL_NEWREV \
    --env ZUUL_SHORT_OLDREV \
    --env ZUUL_SHORT_NEWREV \
    --env ZUUL_PATCHSET \
    --env ZUUL_CHANGE_IDS_CLEANED \
    --env ARTIFACTORY_API_KEY \
    --env WORKSPACE_ROOT \
    --workdir "${WORKSPACE_ROOT}" \
    --device /dev/ttyMP \
    --device /dev/ttyVIP \
    --privileged \
    --rm \
    "${BOOTSTRAP_DOCKER_IMAGE}" \
    "$@"
}

# Make sure there are no other Docker containers left running on slaves that
# might interfer with current job. E.g. it is promplematic to have an adb server
# running in another container if we want to invoke it in the current job.
function docker_killall() {
  local containers
  containers=$(docker ps -q --format="{{.ID}} {{.Image}}" | grep vcc_aosp_build | cut -d " " -f 1 )
  if [ -n "$containers" ]; then
    #shellcheck disable=SC2086
    docker kill $containers
  fi
}

################################################################################################
## Initialize Repo and vendor/volvocars
#
# Important to initialize the repository with "repo init" "repo sync" instead of standard "git init",
# otherwise repo get confused (repo assumes .git-data is in .repo/project-objects/).
# Reset the manifest-repo in case it was modified in a strange way from last commit
# shellcheck disable=SC2015
function get_repos {
    if [[ -d .repo ]]; then
        # Abort rebase if manifest repo has merge conflicts and is still in rebase state
        if [[ -d .repo/manifests/.git/rebase-apply ]]; then
            (cd .repo/manifests && git rebase --abort || true)
        fi
        # Hard reset manifest repo (current branch)
        (cd .repo/manifests && git reset --hard HEAD || true)
    fi
    bootstrap_docker_run "repo init --reference=/cm/repo-mirror -u ssh://gotsvl1415.got.volvocars.net:29421/manifest -b ${ZUUL_BRANCH}" || return $?
    rm -rf ./* || sleep 30 && rm -rf ./*  # Try again if needed, due to dangling processes.
    #bootstrap_docker_run "repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j8 vendor/volvocars" || return $?
}

function clean_all {
    find . -type f -delete && find . -type d -empty -delete
}

get_repos || (clean_all && get_repos)

if ! [ "$ZUUL_PROJECT" == "vendor/volvocars" ]; then
    bootstrap_docker_run "repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j8 ${ZUUL_PROJECT}"
fi

################################################################################################
# repo sync would leave uncommited changes, but zuul cloner below would fail
# if there are unstaged changes. And we want builds to be reproducible so better to reset repos.
bootstrap_docker_run "repo forall -c 'echo -n \"### \"; pwd; git reset --hard ; git clean -fdxq'"

################################################################################################
## Download the commit to check (for vendor/volvocars-repo)
#
# zuul-cloner implicity uses other environment variables as well, such as ZUUL_REF.
#bootstrap_docker_run "GIT_SSH=$HOME/zuul_ssh_wrapper.sh zuul-cloner -v ${ZUUL_URL} ${ZUUL_PROJECT}"
bootstrap_docker_run "GIT_SSH=$HOME/zuul_ssh_wrapper.sh zuul-cloner -v ${ZUUL_URL} vendor/volvocars"

if [ "$(git -C "$ZUUL_PROJECT" rev-parse HEAD)" != "$ZUUL_COMMIT" ]; then
    die "zuul-cloner failed to checkout commit $ZUUL_COMMIT in $ZUUL_PROJECT"
fi

################################################################################################
## This check exists to prevent people from pushing code with old ci scripts
if [ ! -f ./vendor/volvocars/tools/ci/ci_version ]; then
    echo "Your CI version in vendor/volvocars is too old, you will have to rebase your change."
    exit -1
fi

export REQUIRED_CI_VERSION=I9505846e311988b661939b91917e3df7c3dbce50
DETECTED_CI_VERSION=$(cat ./vendor/volvocars/tools/ci/ci_version)
if [ $REQUIRED_CI_VERSION != "$DETECTED_CI_VERSION" ]; then
    # If you are a CI developer, you should update the CI version both here and in the repo.
    echo "***************************************************************************************
    echo "*"
    echo "* Your CI version in vendor/volvocars is too old, you will have to rebase your change."
    echo "* Min changeset version required: $REQUIRED_CI_VERSION"
    echo "***************************************************************************************
    exit -1
fi


################################################################################################
# At this point in the script your directory tree should look something like this:
#   /.repo
#   /vendor/volvocars   - On branch ZUUL_BRANCH
#   /other/repos        - On unknown revision (leftovers from previous build in the workspace,
#                         but in reset/clean state)
#
#
# It is now up to each build step to define the following steps

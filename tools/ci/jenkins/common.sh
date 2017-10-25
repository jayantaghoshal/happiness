#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
export PIP_CONFIG_FILE="/usr/local/pip/pip.conf"

if [ -z "${WORKSPACE}" ]; then
  WORKSPACE=$(readlink -f "${REPO_ROOT_DIR}"/..)
fi


# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
#C_OK="\033[0;32m"
C_OFF="\033[0m"

function die() {
    message=$1
    echo -e "${C_ERROR}${message}${C_OFF}"
    exit 1
}

# Make sure there are no other Docker containers left running on slaves that
# might interfer with current job. E.g. it is promplematic to have an adb server
# running in another container if we want to invoke it in the current job.
function docker_killall() {
  local containers
  containers=$(docker ps -q --format="{{.ID}} {{.Image}}" | grep vcc_aosp_build | cut -d " " -f 1)
  if [ -n "$containers" ]; then
    #shellcheck disable=SC2086
    docker kill $containers
  fi
}

function ihu_update() {
  "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/ihu_update.py \
    --aosp_root_dir "$REPO_ROOT_DIR" \
    --vip_port /dev/ttyVIP \
    --mp_port /dev/ttyMP
}

function repo_sync() {
  repo sync --no-clone-bundle --current-branch -q -j8 "$@" || die "repo sync failed"
}

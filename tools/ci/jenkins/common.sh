#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
DOCKER_BUILD_DIR=${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build

if [ -z "${WORKSPACE}" ]; then
  WORKSPACE=$(readlink -f "${REPO_ROOT_DIR}"/..)
fi

# Setup ccache
if [ -z "${USE_CCACHE}" ]; then
  USE_CCACHE=false
fi
if [ -z "${CCACHE_DIR}" ]; then
  CCACHE_DIR=$(readlink -f "${REPO_ROOT_DIR}"/../vcc-aosp-build-ccache)
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

function docker_run() {
  USE_CCACHE=$USE_CCACHE \
  CCACHE_DIR=$CCACHE_DIR \

  #shellcheck disable=SC2048
  #shellcheck disable=2086
  "${DOCKER_BUILD_DIR}"/run.sh \
  --env-file="${SCRIPT_DIR}"/env.list \
  --volume="$WORKSPACE":"$WORKSPACE" \
 $*
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
  local mp_dev=/dev/ttyMP
  local vip_dev=/dev/ttyVIP

  docker_run --volume=$mp_dev:$mp_dev --volume=$vip_dev:$vip_dev \
    "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/ihu_update.py \
    --aosp_root_dir "$REPO_ROOT_DIR" --vip_port $vip_dev --mp_port $mp_dev
}

function repo_sync() {
  local repos=$*
  docker_run "repo sync --no-clone-bundle --current-branch -q -j8 $repos" || die "repo sync failed"
}

# Passed to find's -regex. Use \| to separate paths to components to disable tests for.
# Example:
#
# TEST_BLACKLIST="vendor/volvocars/hardware/foo/.*\|vendor/volvocars/hardware/bar/.*"
#
# TODO: Make this configurable in a JSON config file or somthing when porting to
#       Python.
TEST_BLACKLIST="vendor/volvocars/hardware/climate/.*\|vendor/volvocars/hardware/signals/vehiclesignalsdaemon/.*\|vendor/volvocars/hardware/powermoding/.*"

# TODO: Add to build system, should replace *_test_build.sh
function build_tests {
    local build_sh_list dir script
    build_sh_list=$(find vendor/volvocars -type l -name '*_test_build.sh' -not -regex "$TEST_BLACKLIST")

    for build_sh in $build_sh_list; do
        dir=$(dirname "${build_sh}")
        script=$(basename "${build_sh}")
        # shellcheck disable=SC2086
        echo "Calling ${script} in directory $(realpath ${dir})"
        pushd "${dir}" >> /dev/null
        docker_run "./${script}" || die "Failed to build tests!"
        popd > /dev/null
    done
}

# TODO: Replace with using VTS/Tradefed test plans
function run_tests {
    local run_sh_list status dir script
    run_sh_list=$(find vendor/volvocars -type l -name '*_test_run.sh' -not -regex "$TEST_BLACKLIST")

    status=0
    for run_sh in $run_sh_list; do
        dir=$(dirname "${run_sh}")
        script=$(basename "${run_sh}")
        # shellcheck disable=SC2086
        echo "Calling ${script} in directory $(realpath ${dir})"
        pushd "${dir}" >> /dev/null
        docker_run "./${script}"
        if [ $status -eq 0 ]; then
            status=$?
        fi
        popd > /dev/null
    done
    
    if [ $status -ne 0 ]; then
        die "Test failed!"
    fi
}

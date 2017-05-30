#!/bin/bash

# Verify required parameters
[[ -z "${HOST_GID}" ]] && {
    echo "HOST_GID env variable not defined!"
    exit 1
}

[[ -z "${HOST_UID}" ]] && {
    echo "HOST_UID env variable not defined!"
    exit 1
}

[[ -z "${HOST_UNAME}" ]] && {
    echo "HOST_UNAME env variable not defined!"
    exit 1
}

# Global parameters
CONTAINER_USERNAME=ihu
CONTAINER_GROUPNAME=ihu
BUILD_ENV_SETUP="${REPO_ROOT_DIR}/build/envsetup.sh"
BASHRC_FILE="${REPO_ROOT_DIR}/vendor/volvocars/tools/docker_build/bashrc"

function Failed() {
    echo "$*"
    exit 1
}

function GetDirOwner() {
    set -o pipefail               # Turn on failing the command, if any command in pipe fails

    local dir="$1"
    local owner=$(stat $dir | egrep -o 'Uid: \([ \t]+[0-9]+\/[ \t]+[a-zA-Z0-9]+\)' | cut -d/ -f2)
    local rc=$?

    set +o pipefail               # Turn off failing the command, if any command in pipe fails

    [[ "$rc" == "0" ]] || return $rc
    [[ -z "${owner}" ]] && return 1     # Unexpected output

    echo ${owner::-1}             # Do not use quotes - variable will be trimmed automatically
    return 0
}

# Add user (matching to the host user)
id -g "${HOST_GID}" &>/dev/null || groupadd --gid ${HOST_GID} ${CONTAINER_GROUPNAME} ||
    Failed "Creating group with group id ${HOST_GID} failed"

id -u "${HOST_UID}" &>/dev/null || {
    useradd --uid ${HOST_UID} --gid ${HOST_GID} --home-dir "${HOME}" ${CONTAINER_USERNAME} ||
        Failed "Creating user with id ${HOST_UID} failed"
    adduser $CONTAINER_USERNAME sudo > /dev/null ||
        Failed "Adding user to sudo group failed"
}

# Create home directory if missing, used if volume $HOME is not mapped (i.e. CI)
[[ ! -d "${HOME}" ]] && {
    echo "Home directory (${HOME}) missing, will create..."
    mkdir ${HOME}
    chown ${CONTAINER_USERNAME}:${CONTAINER_GROUPNAME} ${HOME}
}

# Add user to dialout group. This allows access to /dev/ttyUSB* without sudo
adduser ${CONTAINER_USERNAME} dialout &>/dev/null || Failed "Adding user to dialout group failed"

# Allow user to sudo
echo "$CONTAINER_USERNAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Prepare script - command which will be executed as a specified user
SCRIPT_FILE=/tmp/command_to_run.sh
cat >$SCRIPT_FILE <<EOL
#!/bin/bash

source $BUILD_ENV_SETUP

$@
EOL
chmod +x $SCRIPT_FILE

if [ $# -ne 0 ]; then
  COMMAND_IN_DOCKER="$SCRIPT_FILE"
else
  COMMAND_IN_DOCKER="bash --rcfile ${BASHRC_FILE}"
fi

sudo -E -u ${CONTAINER_USERNAME} BASHRC_FILE=${BASHRC_FILE} BUILD_ENV_SETUP=$BUILD_ENV_SETUP LD_LIBRARY_PATH=$LD_LIBRARY_PATH PATH=/sbin:$PATH USE_CCACHE=${USE_CCACHE} CCACHE_DIR=${CCACHE_DIR} ${COMMAND_IN_DOCKER}


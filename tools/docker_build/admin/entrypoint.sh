#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

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
VOLVO_ENV_SETUP="${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"
BASHRC_FILE="/home/ihu/.bashrc"

PYTHONUSERBASE="/tmp/${CONTAINER_USERNAME}/.local"

function Failed() {
    echo "$*"
    exit 1
}

function GetDirOwner() {
    set -o pipefail               # Turn on failing the command, if any command in pipe fails

    local dir="$1"
    local owner
    owner=$(stat "$dir" | egrep -o 'Uid: \([ \t]+[0-9]+\/[ \t]+[a-zA-Z0-9]+\)' | cut -d/ -f2)
    local rc=$?

    set +o pipefail               # Turn off failing the command, if any command in pipe fails

    [[ "$rc" == "0" ]] || return $rc
    [[ -z "${owner}" ]] && return 1     # Unexpected output


    # shellcheck disable=SC2086
    echo ${owner::-1}             # Do not use quotes - variable will be trimmed automatically
    return 0
}

# Add user (matching to the host user)
id -g "${HOST_GID}" &>/dev/null || groupadd --gid "${HOST_GID}" "${CONTAINER_GROUPNAME}" ||
    Failed "Creating group with group id ${HOST_GID} failed"

id -u "${HOST_UID}" &>/dev/null || {
    useradd --uid "${HOST_UID}" --gid "${HOST_GID}" --home-dir "${HOME}" ${CONTAINER_USERNAME} ||
        Failed "Creating user with id ${HOST_UID} failed"
    adduser "$CONTAINER_USERNAME" sudo > /dev/null ||
        Failed "Adding user to sudo group failed"
}

# Create home directory if missing, used if volume $HOME is not mapped (i.e. CI)
[[ ! -d "${HOME}" ]] && {
    echo "Home directory (${HOME}) missing, will create..."
    mkdir "${HOME}"
    chown ${CONTAINER_USERNAME}:${CONTAINER_GROUPNAME} "${HOME}"
}

# Allow user to sudo
echo "$CONTAINER_USERNAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Prepare script - command which will be executed as a specified user
# NOTE: This is tricky: Inside SCRIPT_FILE we source $BUILD/VOLVO_ENV_SETUP __before__ running $@,
#       When running interactively $@ is equal to bash, and functions included from BUILD/VOLVO_ENV_SETUP are not inherited to the subshell.
#       That's why we need to check if bash is the command to run and start it without the SCRIPT_FILE, the rcfile also includes the BUILD_ENV_SETUP
#       We also can't copy the BASHRC_FILE to ~/.bashrc because we mount the host home as a --volume
SCRIPT_FILE=/tmp/command_to_run.sh
cat >$SCRIPT_FILE <<EOL
#!/bin/bash

if [ -f ${BUILD_ENV_SETUP} ]; then
   source ${BUILD_ENV_SETUP}
   lunch ihu_vcc-eng
fi
if [ -f ${VOLVO_ENV_SETUP} ]; then
   source ${VOLVO_ENV_SETUP}
fi
$@
EOL
chmod +x $SCRIPT_FILE

if [[ "$*" = 'bash' ]]; then
  # If this is an interactive invocation then we should start bash with $BASHRC_FILE.
  COMMAND_IN_DOCKER="bash --rcfile ${BASHRC_FILE}"
else
  # If this is a non-interactive invocation then we should use run the $SCRIPT_FILE.
  COMMAND_IN_DOCKER="$SCRIPT_FILE"
fi

# Add docker user to same groups as host user
IFS='_'
for group_id in $HOST_USER_GROUPS
do
  # Get group name from group id
  group_name=$(getent group "${group_id}" | cut -d : -f 1)
  # If group does not exists create it
  if [[ -z ${group_name} ]]; then
    group_name=host-group-${group_id}
    groupadd -g "${group_id}" "${group_name}"
  fi
  # Add user to group
  echo "Adding user ${CONTAINER_USERNAME} to group ${group_name}"
  usermod -a -G "${group_name}" "$CONTAINER_USERNAME"
done
unset IFS


sudo -E \
    -u "${CONTAINER_USERNAME}" \
    BASHRC_FILE="${BASHRC_FILE}" \
    BUILD_ENV_SETUP="$BUILD_ENV_SETUP" \
    LD_LIBRARY_PATH="$LD_LIBRARY_PATH" \
    PATH=/sbin:"$PATH" \
    PYTHONUSERBASE="${PYTHONUSERBASE}" \
    ${COMMAND_IN_DOCKER}


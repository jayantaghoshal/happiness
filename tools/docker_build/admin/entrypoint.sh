#!/bin/bash
# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project
set -ue

if [[ $(id -u) = "${HOST_UID}" ]]; then
    echo ""
    echo "Fatal error, HOST_UID same as default docker UID"
    echo "This should not happen, should only happen if host user id is equals to container default user id(root)."
    echo "That means you are running user as root from host which you should never do."
    echo "If this becomes a problem in future you need to modify the useradd below to involve usermod also."
fi

# Global parameters
CONTAINER_USERNAME=ihu
BUILD_ENV_SETUP="${REPO_ROOT_DIR}/build/envsetup.sh"
VOLVO_ENV_SETUP="${REPO_ROOT_DIR}/vendor/volvocars/tools/envsetup.sh"
BASHRC_FILE="/home/ihu/.bashrc"

#Set PYTHONUSERBASE to to prevent using python site-packages in ~/.local from host
PYTHONUSERBASE="/tmp/${CONTAINER_USERNAME}/.local"

function Failed() {
    echo "$*"
    exit 1
}

########################################################################################################################
# Create user groups mirroring all the groups on host
if [[ ${MIRROR_HOST_USER_AND_GROUPS} = 1 ]]; then
    echo "Creating users and groups mirroring the host"

    IFS=','
    for group_id in $HOST_USER_GROUPS_COMMA_SEPARATED
    do
      # Get group name from group id
      group_name=$(getent group "${group_id}" | cut -d : -f 1)
      # If group does not exist create it
      if [[ -z ${group_name} ]]; then
        group_name=host-group-${group_id}
        groupadd -g "${group_id}" "${group_name}" || Failed "groupadd failed for ${group_id}"
      fi
    done
    unset IFS

    useradd --uid "${HOST_UID}" --gid "${HOST_GID}" --groups "${HOST_USER_GROUPS_COMMA_SEPARATED}" --home-dir "${HOME}" ${CONTAINER_USERNAME} ||
        Failed "Creating user with id ${HOST_UID} failed"
    adduser "$CONTAINER_USERNAME" sudo > /dev/null ||
        Failed "Adding user to sudo group failed"


    # Create home directory if missing, used if volume $HOME is not mapped (i.e. CI)
    #TODO: This is probably deprecated, investigate if can be removed
    [[ ! -d "${HOME}" ]] && {
        echo "Home directory (${HOME}) missing, will create..."
        mkdir "${HOME}"
        chown ${CONTAINER_USERNAME}:"${HOST_GID}" "${HOME}"
    }

    # Allow user to sudo
    echo "$CONTAINER_USERNAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
fi


########################################################################################################################
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


sudo -E \
    -u "${CONTAINER_USERNAME}" \
    BASHRC_FILE="${BASHRC_FILE}" \
    BUILD_ENV_SETUP="$BUILD_ENV_SETUP" \
    PATH=/sbin:"$PATH" \
    PYTHONUSERBASE="${PYTHONUSERBASE}" \
    ${COMMAND_IN_DOCKER}


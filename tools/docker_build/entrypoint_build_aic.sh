#!/bin/bash

# Maintainer: Sebastian Mangelsen
#             sebastian.mangelsen@volvocars.com
#
# this is our approach to provide a kind of automated
# build script that is executed once the container is
# started, together with CMD which is provided as argument
# after the container name when calling "docker run" the
# docker container could be used to automate the whole
# process of building an Android image
#
# Description:
#  - check whether arguments are provided 
#  - check that it is an folder
#  - cd into that folder
#  - run the neccessary commands in order to build
#    an image
set -ue

if [ $# -eq 0 ]; then
  # no arguments provided
  echo "ERROR: docker run requires a build folder"
  exit -1
fi
BUILD_WORKING_DIR=${1}
shift
BUILD_COMMAND="$*"

if ! test -d ${BUILD_WORKING_DIR}; then
  echo "ERROR couldn't find build folder: \"${BUILD_WORKING_DIR}\""
  exit -1
fi

cd ${BUILD_WORKING_DIR}
if ! test -e build/envsetup.sh; then
  echo "ERROR: couldn't find envsetup file"
  exit -1
fi

USER_ID=${LOCAL_USER_ID:-1000}
GROUP_ID=${LOCAL_GRP_ID:-100}

# check whether a group exists with same GID
GROUP_NAME=$(cat /etc/group | grep :${LOCAL_GRP_ID}: | cut -d ':' -f1)
if [[ ${GROUP_NAME} == "" ]]; then
  groupadd -g ${GROUP_ID} aic_group
  echo "DBG: created group, name: ${GROUP_NAME}, gid: ${GROUP_ID}"
fi

useradd --shell /bin/bash -u ${USER_ID} -g ${GROUP_ID} -o -c "" -m user
/sbin/runuser -l user -c "cd ${BUILD_WORKING_DIR} && export PATH=/sbin:$PATH && export USE_CCACHE=1 && export CCACHE_DIR=${CCACHE_DIR} && source build/envsetup.sh && ${BUILD_COMMAND}"
#/sbin/runuser -l user -c "cd `echo ${1}` && export PATH=/sbin:$PATH; source build/envsetup.sh && lunch ${2} && make ${3} -j7"



# EOF

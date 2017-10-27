#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../..)

FILE_LIST=()

VERIFY=false

EXIT_CODE=0

# Configure glob pattern behavior
shopt -s globstar
shopt -s nullglob

while [[ $# -gt 0 ]]
do
    key="$1"

    case $key in
        -v|--verify)
        VERIFY=true
        shift # past argument
        ;;
        *)    # default
        if [[ -d "$1" ]]; then
           echo "Adding files from directory ${1}"
           FILE_LIST+=(${1}/**/*.bp)
        fi
        if [[ -f "$1" ]]; then
           echo "Adding file ${1}"
           FILE_LIST+=${1}
        fi
        shift # past argument
        ;;
    esac
done
echo "Canonical formatting for ${#FILE_LIST[@]} BP source files"
for f in "${FILE_LIST[@]}"
    do
        TMP_FORMAT_RESULT=$(mktemp)
        "${REPO_ROOT_DIR}/prebuilts/build-tools/linux-x86/bin/bpfmt" "${f}" > "${TMP_FORMAT_RESULT}"
        if ! cmp -s "${f}" "${TMP_FORMAT_RESULT}" ; then
            if [[ ${VERIFY} = true ]] ; then
                 echo "Misformatted ${f}"
                 EXIT_CODE=$((EXIT_CODE+1))
            else
                cp "${TMP_FORMAT_RESULT}" "${f}"
                echo "Formatted ${f}"
            fi
        fi
        rm "${TMP_FORMAT_RESULT}"
    done
echo "Canonical formatting for BP source files - Done."

exit ${EXIT_CODE}

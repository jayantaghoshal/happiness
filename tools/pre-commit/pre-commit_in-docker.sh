#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../..)
CLANG_FORMAT_BINARY="${REPO_ROOT_DIR}/prebuilts/clang/host/linux-x86/clang-3859424/bin/clang-format"

echo "Running vendor/volvocars pre-commit checks..."

format_some_file() {
  formatter="${1}"
  input="${2}"
  echo "Formatting ${input}"
  PATH_TO_COPY=$(mktemp)
  cp "${input}" "${PATH_TO_COPY}"
  ${formatter} "${input}"
  if ! cmp -s "${input}" "${PATH_TO_COPY}" ; then
    echo "Re-adding formatted ${input}"
    git add "${input}"
  fi
  rm "${PATH_TO_COPY}"
}

format_cpp_file() {
  format_some_file "${CLANG_FORMAT_BINARY} -i" "${1}"
}

CPP_FILES_TO_FORMAT=$(git diff --cached --name-only --diff-filter=ACMRT | grep -E "\.(cpp|hpp|c|h)$" || true);
for file in ${CPP_FILES_TO_FORMAT} ; do
  format_cpp_file "${file}"
  python3 "${REPO_ROOT_DIR}/vendor/volvocars/tools/ci/cpp/logging_linter.py" "${file}"
  #install docopt in dockerfile
  #python3 "${REPO_ROOT_DIR}/vendor/volvocars/tools/ci/licensing-components/main.py" fix-copyright-headers "${file}"
done

format_bp_file() {
  format_some_file "${REPO_ROOT_DIR}/prebuilts/build-tools/linux-x86/bin/bpfmt -w" "${1}"
}

BP_FILES_TO_FORMAT=$(git diff --cached --name-only --diff-filter=ACMRT | grep -E "\.(bp)$" || true);
for file in ${BP_FILES_TO_FORMAT} ; do
  format_bp_file "${file}"
  #install docopt in dockerfile
  #python3 "${REPO_ROOT_DIR}/vendor/volvocars/tools/ci/licensing-components/main.py" fix-copyright-headers "${file}"
done

SH_FILES_TO_CHECK=$(git diff --cached --name-only --diff-filter=ACMRT | grep -E "\.(sh)$" || true);
for file in ${SH_FILES_TO_CHECK} ; do
  echo "Shellchecking ${file}"
  shellcheck "${file}"
done

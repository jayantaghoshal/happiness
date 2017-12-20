#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue

echo "-------------------------------------------------------------------"
echo "  SHELLCHECK Analysis"
echo "  This will analyze all shell-scripts in volvocars repository"
echo "                                                           "
echo "  * Please take warnings seriously"
echo "  * If you don't understand the warning: "
echo "      1. Copy the warning code, eg SC1001"
echo "      2. Go to https://github.com/koalaman/shellcheck/wiki/###### where ##### is to be replaced with the warning code"
echo "      3. Google the warning code"
echo "  * If you understand the warning and think it can be ignored, read https://github.com/koalaman/shellcheck/wiki/Ignore"
echo ""
echo "-------------------------------------------------------------------"
shellcheck --version

any_errors=false

shopt -s globstar
shopt -s nullglob
for f in **/*.sh
do
  echo "Shellchecking $f"
  set +e
  shellcheck "$f"
  shellcheck_exitcode=$?
  set -e
  if [ $shellcheck_exitcode -ne 0 ]
  then
    any_errors=true
  fi
done

echo "SHELLCHECK Analysis DONE"
if [ "$any_errors" == true ]; then exit_code=1; else exit_code=0; fi
exit "$exit_code"

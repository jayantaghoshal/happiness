#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
HOOK_SCRIPT_TARGET="$SCRIPT_DIR"/pre-commit.sh

HOOKS_DIR=$(git rev-parse --git-dir)/hooks
HOOK_SCRIPT_SYMLINK="$HOOKS_DIR"/pre-commit

echo "Symlinking $HOOK_SCRIPT_SYMLINK to $HOOK_SCRIPT_TARGET"
ln -nsf "$HOOK_SCRIPT_TARGET" "$HOOK_SCRIPT_SYMLINK"
echo "Installed."
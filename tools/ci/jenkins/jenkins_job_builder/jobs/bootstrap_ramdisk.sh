#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -uex

die() { echo "$@" 1>&2 ; exit 1; }

export RAMDISK_ROOT
RAMDISK_ROOT=/mnt/ramdisk

if [ ! -d "$RAMDISK_ROOT"/"$JOB_NAME" ]; then
  mkdir "$RAMDISK_ROOT"/"$JOB_NAME";
fi

if [ ! -d "$RAMDISK_ROOT"/ccache ]; then
  mkdir "$RAMDISK_ROOT"/ccache;
fi

cd "$RAMDISK_ROOT"/"$JOB_NAME"

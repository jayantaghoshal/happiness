#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

build_dir=aosp

if [[ -d $build_dir ]]; then
  rm -rf $build_dir;
fi

mkdir $build_dir
cd $build_dir

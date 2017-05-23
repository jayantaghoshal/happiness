# IHU Android AOSP Docker build image

## Introduction

Before continuing make sure you know what Docker is, please see
[What is docker](https://www.docker.com/what-docker).

This document describes the Docker image that aims to provide a consistent environment when building AOSP in the IHU Android project.

## Prerequisites

* Docker 17.03.1-ce - All scripts were tested and optimized to this version.

## How to build the docker image

To build the docker image, execute:

```bash
vendor/volvocars/tools/docker_build/build.sh
```

## How to use the image

To run a command inside a container, execute:

```bash
vendor/volvocars/tools/docker_build/run.sh <command>
```

Example 1: Build a full system:

```bash
# From the root of the project run:
vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && make -j8"
```

Example 2: Start an interactive shell

```bash
vendor/volvocars/tools/docker_build/run.sh

# Now you can run interactively in the docker container with
# the AOSP build environment, for example:
lunch ihu_vcc-eng

# Build the complete system
make -j8

# Or just build a module
cd vendor/volvocars/hardware/localconfig
mm
```

## Instructions on how to work with Docker registry

TODO: We want the image to be automatically generated and
promoted to a central Docker registry so that devlopers easily
can pull ready made images.
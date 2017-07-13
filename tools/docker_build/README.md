# IHU Android AOSP Docker build image

## Introduction

Before continuing make sure you know what Docker is, please see
[What is docker](https://www.docker.com/what-docker).

This document describes the Docker image that aims to provide a consistent environment when building AOSP in the IHU Android project.

## Prerequisites

* Docker CE. Tested OK with version: 17.04.0-ce and 17.06.0-ce
  
  DO NOT use the docker that comes in ubuntus apt-get repositories, it is super old. 


## How to use the image

First you need to authenticate with Artifactory
```bash
# Only required once since login information are stored in ~/.docker/config.json
docker login -u $USER swf1.artifactory.cm.volvocars.biz:5002
```

To run a command inside a container, execute:

```bash
vendor/volvocars/tools/docker_build/run.sh <command>
```

### Example 1: Build a full system:

```bash
# From the root of the project run:
vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && make -j8"
```

### Example 2: Start an interactive shell

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




## Instructions on how to work with Docker registry (Adminstrators only)

### How to build the docker image locally without pulling from artifactory

To build the docker image, execute:

```bash
vendor/volvocars/tools/docker_build/admin/create_docker_image.sh

# If you built image yourself you need to provide --local flag to run.sh to use latest instead of tagged
vendor/volvocars/tools/docker_build/run.sh --local COMMAND_TO_EXECUTE
```

### How to push image to artifactory 

To push the docker image to Artifactory, execute:
```bash
# Only for test, should be automated (Jenkins should build and push image to Artifactory)
vendor/volvocars/tools/docker_build/admin/push.sh <IMAGE ID>
```

### How to pull image from artifactory 
(Not needed since run.sh does this automatically)

To pull the docker image from Artifactory, execute:
```bash
docker pull swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build[:NAME]
```

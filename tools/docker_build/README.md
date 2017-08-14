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

# Now you can work interactively with the AOSP build environment, for example:

# Setup the build target
lunch ihu_vcc-eng

# Build the complete system
make -j8

# Or just build a module
cd vendor/volvocars/hardware/localconfig
mm
```


## Instructions on how to update the Docker image (Adminstrators only)

Maybe you need to update the docker image, to add some new tool or change the environment. Please be aware that this image is used in production so be sure to
invite the CI-team for code review.

### How to build the docker image locally without pulling from artifactory

Update the image configuration and build it:

```bash
vendor/volvocars/tools/docker_build/admin/create_docker_image.sh
```

Now you can test the local image using the --local option

```bash
vendor/volvocars/tools/docker_build/run.sh --local
```

Commit your changes and update the file vendor/volvocars/tools/docker_build/image.ref with the the change id. This is used by the run script to baseline the docker image. For example:

```
swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build:I51526c2c73b3a94b65b5b06d7adf3115129bc3b7
```

Now you need to add this change and amend it to the commit.

### How to push image to artifactory

When you have verified the image you can now push it to Artifactory. The <CHANGE ID> below refers to your committed changes above.

```bash
vendor/volvocars/tools/docker_build/admin/push.sh <CHANGE ID>
```

### How to pull image from artifactory
(Not needed since run.sh does this automatically)

To pull the docker image from Artifactory, execute:

```bash
docker pull swf1.artifactory.cm.volvocars.biz:5002/test/vcc_aosp_build[:NAME]
```

## Future improvements

* Move image from the "test" namespace.
* The image should be automatically built, verified and pushed to Artifactory by the CI-machinery.
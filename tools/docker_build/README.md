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
./build.sh
```

## How to use the image

To run a command inside a container, execute:

```bash
./run.sh <command>
```

Example 1: Build a full system:

```bash
./run.sh "lunch ihu_vcc-eng && make flashfiles -j8"
```

Example 2: Start an interactive shell

```bash
./run.sh bash
# Now you can run interactively in the docker container with a AOSP build environment, for example:
lunch ihu_vcc-eng
make -j8
```

## Instructions on how to work with Docker registry

TODO: We want the image to be automatically generated and
promoted to a central Docker registry so that devlopers easily
can pull ready made images.
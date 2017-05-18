# IHU Android AOSP Docker build container

## Introduction

Before continuing make sure you know what Docker is, what are Docker
images and containers.

This document describes the Docker container that aims to provide a consistent environment when building AOSP in the IHU Android project.

## Prerequisites

* Docker 17.03.1-ce - All scripts were tested and optimized to this version.

## Instruction on how to use the image

To run a command inside the container, execute:

```bash
./run.sh <command>
```

For example to build a full system:

```bash
./run.sh "lunch ihu_abl_car-eng && make dist droid -j8"
```

## Instruction on how to build the docker container

To build the docker container, execute:

```bash
./build.sh
```

## Instructions on how to work with Docker registry

TODO: We want the image to be automatically generated and
promoted to a central Docker registry so that devlopers easily
can pull ready made images.
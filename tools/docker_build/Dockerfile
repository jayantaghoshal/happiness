# Specialized for building Android images in the Intel BROXTON preN.car project

# Based on Ubuntu
FROM ubuntu:16.04

USER root

# INSTALLING TOOLS
# ================

LABEL org.label-schema.vendor="Volvo Cars" \
  org.label-schema.url="https://c1.confluence.cm.volvocars.biz/display/IHUA/How+to+build+Android+image" \
  org.label-schema.name="VCC AOSP build environment" \
  org.label-schema.vcs-url="ssh://icup-android.gerrit.cm.volvocars.biz/vendor/volvocars.git" \
  org.label-schema.vcs-ref="" \
  org.label-schema.build-date="" \
  org.label-schema.docker.schema-version="1.0"

# Configure proxy for environment.
# TODO: Make dns work so that we can avoid hard-coded IPs.
ENV http_proxy 'http://10.244.0.55:83/'
ENV HTTP_PROXY 'http://10.244.0.55:83/'
ENV https_proxy 'https://10.244.0.55:83/'
ENV HTTPS_PROXY 'https://10.244.0.55:83/'

# Configure proxy for apt-get.
RUN echo 'Acquire::http::Proxy "http://10.244.0.55:83/";' >> /etc/apt/apt.conf

# First update the system.
RUN apt-get update && apt-get -y upgrade && apt-get install -y \
    bc \
    bison \
    build-essential \
    ccache \
    cmake \
    cpio \
    curl \
    default-jdk \
    default-jre \
    dos2unix \
    dosfstools \
    flex \
    g++-multilib \
    gawk \
    gcc-multilib \
    gettext \
    git \
    git-core \
    gnupg \
    gperf \
    imagemagick \
    lib32ncurses5-dev \
    lib32z-dev \
    libc6-dev \
    libc6-dev-i386 \
    libgl1-mesa-dev \
    libncurses5-dev \
    libreadline6-dev \
    libssl-dev \
    libx11-dev \
    libxml2-utils \
    mtools \
    openjdk-8-jdk \
    python-libxml2 \
    python-markdown \
    squashfs-tools \
    sudo \
    tesseract-ocr \
    tofrodos \
    tree \
    unzip \
    vim \
    wget \
    x11proto-core-dev \
    xsltproc \
    zip \
    zlib1g-dev \
    repo \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir  /usr/lib/i386-linux-gnu \
    && ln -s /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1 \
       /usr/lib/i386-linux-gnu/libGL.so

# The default command, if docker container is started without parameters
CMD ["bash"]

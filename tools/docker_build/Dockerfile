# Specialized for building Android images in the Intel BROXTON preN.car project

# Based on Ubuntu
FROM ubuntu:16.04

# INSTALLING TOOLS
# ================

LABEL vcc.git.commit='768761823adbc' vcc.git.url='https://SMANGELS@icup-android.gerrit.cm.volvocars.biz/a/aic_tools' vcc.project='POC Android In Car' vcc.docker.build.machine='gotsvl1413.got.volvocars.net'

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
    && rm -rf /var/lib/apt/lists/*
RUN mkdir -p /cm/tmp/$(whoami)
RUN mkdir  /usr/lib/i386-linux-gnu \
    && ln -s /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1 \
       /usr/lib/i386-linux-gnu/libGL.so

# Add an entry point script.
COPY entrypoint_build_aic.sh /usr/local/bin/entrypoint_build_aic.sh

# Declare a new entry point.
ENTRYPOINT ["/usr/local/bin/entrypoint_build_aic.sh"]

# EOF

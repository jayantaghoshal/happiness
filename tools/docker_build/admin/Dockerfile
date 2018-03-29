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
ENV NO_PROXY '.got.volvocars.net'

# Configure proxy for apt-get.
RUN echo 'Acquire::http::Proxy "http://10.244.0.55:83/";' >> /etc/apt/apt.conf

##################################################
# Tools required for building AOSP image
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
    python-mako \
    squashfs-tools \
    sudo \
    tesseract-ocr \
    tofrodos \
    tree \
    unzip \
    vim \
    nano \
    wget \
    x11proto-core-dev \
    xsltproc \
    zip \
    zlib1g-dev \
    kmod=22-1ubuntu5

###########################################################
# Tools required for "make dist" (required for VBF creation)
# hexdump & cgpt
RUN apt-get install -y \
    bsdmainutils \
    cgpt

##################################################
# Tools required for building Volvo components
RUN apt-get install -y \
    python-pip \
    python3 \
    python3-pip \
    python-xlrd \
    python3-xlrd
RUN pip3 install \
    Jinja2>=2.10 \
    pyyaml \
    six
RUN pip install \
    six \
    pillow

##################################################
# Tools required for testing with VTS, see:
# https://source.android.com/devices/tech/test_infra/tradefed/fundamentals/vts
RUN apt-get install -y \
    python-dev \
    python-protobuf \
    protobuf-compiler \
    python-virtualenv

##################################################
# Install android view client, used for android test automation
# https://github.com/dtmilano/AndroidViewClient/wiki
# RUN pip install git+https://github.com/dtmilano/AndroidViewClient.git
RUN pip install androidviewclient


##################################################
# TODO: Motivate this
RUN mkdir  /usr/lib/i386-linux-gnu \
    && ln -s /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1 \
       /usr/lib/i386-linux-gnu/libGL.so

##################################################
# Debug packages
RUN apt-get install -y \
    iputils-ping \
    dnsutils \
    net-tools \
    gdb \
    strace

##################################################
# Tools required for CI test environment
RUN apt-get install -y \
    git-all \
    picocom \
    python3-venv \
    python3-serial \
    adb \
    pigz \
    usbutils
COPY repo /usr/bin/repo
COPY redis-cli /usr/bin/redis-cli
RUN pip3 install redis \
    requests \
    python-jenkins \
    termcolor \
    tabulate \
    zuul==2.5.2 \
    pymongo

##################################################
# Commit hooks and commit
RUN pip3 install docopt \
                 guardonce

RUN apt-get install -y \
    git-review

##################################################
# Static analysis tools
RUN apt-get install -y \
    shellcheck \
    python-autopep8 \
    python-pep8

# mypy has been set to version 0.560 because Autosar code generation for
# hardware signal data elements, does not currently work with later versions
RUN pip3 install mypy==0.560 \
                 pylint \
                 flake8

RUN pip install flake8

# Typing comes built in with python3, but has to be installed with python2
# Then you can use it together with comment annotations and mypy
RUN pip install typing

##################################################
# Tools required for CarSim
RUN apt-get install -y \
    python3-tk

##################################################
# Tools required for Cloud Simulation

# Nodejs 6 required for running the timezone boundary builder script
# The default available nodejs version via apt is 4.x at the time of
# this writting.
RUN curl -sL https://deb.nodesource.com/setup_6.x -o nodesource_setup.sh
RUN bash nodesource_setup.sh

RUN apt-get install -y \
    nodejs

RUN npm config set proxy $HTTP_PROXY
RUN npm config set proxy-https $HTTPS_PROXY
RUN npm install -g \
    lowdb \
    js2xmlparser \
    json-server

# The timezone boundary builder script depends
# on the following node packages.(Note that these
# packages were to have local installation. Hence
# without -g flag)
RUN npm install \
    @mapbox/geojson-area \
    @turf/helpers \
    async \
    jsts \
    rimraf \
    query-overpass

# Python package for cloning git repositories
# and running nodejs scripts
RUN pip3 install gitpython
RUN pip3 install Naked

#This installs the CLI for spatialite tools needed
#for creating sqlite3 timezonedb from shapefiles
RUN apt-get install -y spatialite-bin



##################################################
# Preload with python packages required for VTS
# This is because VTS does pip intall and by having them locally it's faster and
# doesn't require Internet access
RUN mkdir -p /usr/local/pip /usr/local/cached_pip_packages \
 && > /usr/local/pip/pip.conf \
 && echo '[install]' >> /usr/local/pip/pip.conf \
 && echo 'no-index = true' >> /usr/local/pip/pip.conf \
 && echo 'find-links = /usr/local/cached_pip_packages' >> /usr/local/pip/pip.conf \
 && pip download -d /usr/local/cached_pip_packages/ enum \
 	future futures google-api-python-client \
    httplib2 oauth2client pyasn1 pyasn1-modules \
    rsa six uritemplate protobuf certifi chardet \
    idna requests urllib3 \
    parse ply oauth2client

##################################################
# Install python package from SWF1 Artifactory PyPi
#
ARG swf1_pypi_user
ARG swf1_pypi_pass

# MySQLClient required by SWAT
RUN apt-get update && apt-get install -y \
    libmysqlclient20 \
    mysql-common \
    libmysqlclient-dev

# Dependencies to SWAT
RUN pip2 install \
    BeautifulSoup==3.2.1 \
    invoke==0.21.0 \
    openpyxl==2.3.3

RUN pip2 install \
    SWAT==4.3.1 \
    --index-url https://${swf1_pypi_user}:${swf1_pypi_pass}@swf1.artifactory.cm.volvocars.biz/artifactory/api/pypi/PyPi/simple

##################################################
# Clean up apt package information to make image smaller
RUN  rm -rf /var/lib/apt/lists/*

##################################################
# Prepare entry point
COPY ./entrypoint.sh /tmp
COPY ./bashrc /home/ihu/.bashrc
RUN chmod +x /tmp/entrypoint.sh
ENTRYPOINT ["/tmp/entrypoint.sh"]
# The default command, if docker container is started without parameters
CMD ["bash"]

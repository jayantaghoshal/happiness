FROM ubuntu:16.04
#NOTE: Absolutely not suitable for production, got DEVELOPMENT_BECOME_ANY_ACCOUNT as example
# Just minimal effort container to get gerrit running


ENV http_proxy 'http://10.244.0.55:83/'
ENV HTTP_PROXY 'http://10.244.0.55:83/'
ENV https_proxy 'https://10.244.0.55:83/'
ENV HTTPS_PROXY 'https://10.244.0.55:83/'

# Configure proxy for apt-get.
RUN mkdir -p /etc/apt
RUN echo 'Acquire::http::Proxy "http://10.244.0.55:83/";' >> /etc/apt/apt.conf


# Overridable defaults
ENV GERRIT_HOME /var/gerrit
ENV GERRIT_SITE ${GERRIT_HOME}/review_site
ENV GERRIT_WAR ${GERRIT_HOME}/gerrit.war
ENV GERRIT_VERSION 2.13.8
ENV GERRIT_USER gerrit2

RUN apt update && \ 
    apt upgrade -y 
RUN apt install -y \
    python-software-properties \
    git-all \
    openssl \
    bash \
    perl \
    gitweb \
    curl \
    software-properties-common \
    debconf-utils      
RUN apt install -y 
RUN mkdir -p ${GERRIT_HOME}
RUN mkdir /docker-entrypoint-init.d

#add-apt must be done after "software-properties-common" is installed
RUN add-apt-repository ppa:webupd8team/java 
RUN apt-get update

RUN echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections
RUN apt-get install -y oracle-java8-installer


RUN curl https://gerrit-releases.storage.googleapis.com/gerrit-${GERRIT_VERSION}.war -o $GERRIT_WAR


# Add our user and group first to make sure their IDs get assigned consistently, regardless of whatever dependencies get added
RUN adduser --home "${GERRIT_HOME}" --disabled-password --gecos "My Gerrit User" "${GERRIT_USER}"

# Ensure the entrypoint scripts are in a fixed location
COPY gerrit-entrypoint.sh /


RUN chmod +x /gerrit*.sh

#A directory has to be created before a volume is mounted to it.
#So gerrit user can own this directory.
RUN su ${GERRIT_USER} && echo ${USER} mkdir -p ${GERRIT_SITE}


COPY zuul_id_rsa.pub /zuul_id_rsa.pub

VOLUME $GERRIT_SITE
EXPOSE 8080 29418
ENTRYPOINT ["/gerrit-entrypoint.sh"]
CMD ["start"]
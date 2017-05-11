#Set the base image
FROM ubuntu:16.04
MAINTAINER "Kenneth Karlsson" <kenneth.karlsson.2@volvocars.com>

# First update the system
ENV http_proxy 'http://10.244.0.55:83'
ENV HTTP_PROXY 'http://10.244.0.55:83'
ENV https_proxy 'http://10.244.0.55:83'
ENV HTTPS_PROXY 'http://10.244.0.55:83'

RUN echo 'Acquire::http::Proxy "http://10.244.0.55:83/";' >> /etc/apt/apt.conf
RUN echo 'http_proxy=http://10.244.0.55:83' >> /etc/environment
RUN echo 'https_proxy=http://10.244.0.55:83' >> /etc/environment
RUN echo 'ftp_proxy=http://10.244.0.55:83' >> /etc/environment
RUN echo 'HTTP_PROXY=http://10.244.0.55:83' >> /etc/environment
RUN echo 'HTTPS_PROXY=http://10.244.0.55:83' >> /etc/environment
RUN echo 'FTP_PROXY=http://10.244.0.55:83' >> /etc/environment

RUN apt-get update && apt-get -y upgrade && apt-get install -y apt-utils && apt-get install -y \
    #supervisor was stated in the Zuul dockerfile
    supervisor \
    #rsyslog was stated in the Zuul dockerfile
    python-pip \
    rsyslog \
    build-essential \
    curl \
    default-jdk \
    default-jre \
    #gcc stated in the Zuul dockerfile
    gcc-multilib \
    git \
    git-core \
    openjdk-8-jdk \
    python-libxml2 \
    unzip \
    zip \
    && rm -rf /var/lib/apt/lists/*

RUN pip install virtualenv nose flake8 mock


RUN mkdir /etc/zuul
RUN mkdir /var/log/zuul
RUN mkdir /var/lib/zuul
RUN mkdir /var/www
RUN mkdir /var/www/zuul
RUN git clone https://github.com/openstack-infra/zuul /tmp/zuul
RUN pip install /tmp/zuul
#RUN pip install zuul
RUN cp -Rf /tmp/zuul/etc/status/public_html/* /var/www/zuul/
RUN rm -Rf /tmp/zuul

RUN curl --silent --show-error --retry 12 --retry-delay 10 -L -o /var/www/zuul/fetch.sh https://raw.githubusercontent.com/openstack-infra/zuul/master/etc/status/fetch-dependencies.sh
RUN sed -i "s|public_html/||" /var/www/zuul/fetch.sh
RUN bash /var/www/zuul/fetch.sh

RUN mkdir /etc/jenkins_jobs
RUN mkdir /etc/jenkins_jobs/jobs
RUN pip install jenkins-job-builder

RUN apt update
RUN apt install -y apache2

RUN apt install -y net-tools nano

ADD ./confs/zuul.conf /etc/zuul/zuul.conf
ADD ./confs/logging.conf /etc/zuul/logging.conf
ADD ./confs/merger-logging.conf /etc/zuul/merger-logging.conf
ADD ./confs/gearman-logging.conf /etc/zuul/gearman-logging.conf
ADD ./confs/layout.yaml /etc/zuul/layout.yaml
ADD ./confs/zuul_site.conf /etc/apache2/sites-available/zuul.conf

ADD ./confs/jenkins_jobs.ini /etc/jenkins_jobs/jenkins_jobs.ini
ADD ./confs/jjb.yaml /etc/jenkins_jobs/jobs/jjb.yaml

ADD ./supervisord.conf /etc/supervisord.conf
ADD ./start.sh /start.sh

COPY confs/ssh_wrapper.sh /ssh_wrapper.sh
RUN chmod +x /ssh_wrapper.sh
COPY confs/zuul_id_rsa /var/lib/keys/id_rsa
COPY confs/gerrit-init-project-config.sh /tmp/gerrit-init-project-config.sh
RUN chmod +x /tmp/gerrit-init-project-config.sh

RUN git config --global user.email "zuul@dock_zuul"
RUN git config --global user.name "zuul"
RUN chmod +x /start.sh

EXPOSE 80 8082 8001

CMD ["/start.sh"]

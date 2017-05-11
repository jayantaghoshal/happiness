FROM jenkins

ENV http_proxy 'http://10.244.0.55:83/'
ENV HTTP_PROXY 'http://10.244.0.55:83/'
ENV https_proxy 'https://10.244.0.55:83/'
ENV HTTPS_PROXY 'https://10.244.0.55:83/'

COPY plugins.txt /usr/share/jenkins/ref/
#COPY custom.groovy /usr/share/jenkins/ref/init.groovy.d/custom.groovy
RUN /usr/local/bin/plugins.sh /usr/share/jenkins/ref/plugins.txt
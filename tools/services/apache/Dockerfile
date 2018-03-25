FROM httpd:2.4
RUN groupadd --gid 173539841 vcc-developer
RUN usermod -aG vcc-developer daemon
COPY ./httpd.conf /usr/local/apache2/conf/httpd.conf

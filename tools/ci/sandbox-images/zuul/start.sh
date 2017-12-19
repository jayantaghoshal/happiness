#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x

echo "\
ServerName dock_zuul" >> /etc/apache2/apache2.conf

export http_proxy=
export https_proxy=

a2dissite 000-default
a2ensite zuul

# For https://git-scm.com/docs/git-http-backend#git-http-backend-Apache2x
a2enmod cgi
a2enmod alias
a2enmod env

a2enmod rewrite
a2enmod proxy
a2enmod proxy_http
service apache2 restart
service apache2 reload

touch  /superlog.txt
ping dock_gerrit

# Super hack to wait for gerrit to start before applying config
bash -c "sleep 25; /tmp/gerrit-init-project-config.sh" &

supervisord --nodaemon &
tail -f /superlog.txt
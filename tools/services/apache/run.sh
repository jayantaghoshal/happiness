#!/bin/bash
# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run -d --restart unless-stopped --name ihu-apache -p 80:80 -p 443:443 -v /cm/www:/usr/local/apache2/htdocs/ ihu-ci-apache


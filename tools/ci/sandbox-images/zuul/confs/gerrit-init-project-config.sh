#!/bin/bash
# Git project config
export GIT_SSH=/ssh_wrapper.sh
mkdir gtproj
cd gtproj
git init
git remote add origin ssh://zuul@dock_gerrit:29418/All-Projects
git fetch origin refs/meta/config:refs/remotes/origin/meta/config
git checkout meta/config
cp /tmp/project.config ./project.config  
git add ./project.config
git commit -m "Changed zeh config"
git push origin meta/config:meta/config
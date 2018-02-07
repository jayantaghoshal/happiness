#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -xv
set +e
mkdir -p "$BUILD_NUMBER"

#shellcheck disable=SC2046
mv $(find "$WORKSPACE"/ihu_daily_test-* -name "vcc_test_results*.tar.gz") "$BUILD_NUMBER/"
rm -rf ihu_daily_test-*

cd "$BUILD_NUMBER"

#shellcheck disable=SC2046
mkdir $(find . -type f -iname '*.tar.gz' -exec basename {} '.tar.gz' ';')
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_ihu_daily_test-generic_*.tar.gz -C $(find . -type d -name "vcc_test_results_ihu_daily_test-generic_*")
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_ihu_daily_test-audio_*.tar.gz -C $(find . -type d -name "vcc_test_results_ihu_daily_test-audio_*")
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_ihu_daily_test-flexray_*.tar.gz -C $(find . -type d -name "vcc_test_results_ihu_daily_test-flexray_*")
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_ihu_daily_test-apix_*.tar.gz -C $(find . -type d -name "vcc_test_results_ihu_daily_test-apix_*")

rm -rf ./*.tar.gz


git archive --remote=ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars HEAD:tools/ci/test_report_generation/ xml_to_json_results.py | tar -x

git archive --remote=ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars HEAD:tools/ci/test_report_generation/ json_to_html_results.py | tar -x

git archive --remote=ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars HEAD:tools/ci/test_report_generation/ volvo_logo.png | tar -x

set +x
curl -X GET -u "$JENKINS_USER":"$JENKINS_PASSWORD" https://icup_android.jenkins.cm.volvocars.biz/job/ihu_daily/"$BUILD_NUMBER"/api/json?pretty=true > changelog.json
set -x

python3 xml_to_json_results.py

python3 json_to_html_results.py

rm -rf xml_to_json_results.py json_to_html_results.py

cd "$WORKSPACE"

# deploy it to the web server
cp -R "$BUILD_NUMBER" /cm/www/daily_test_report/
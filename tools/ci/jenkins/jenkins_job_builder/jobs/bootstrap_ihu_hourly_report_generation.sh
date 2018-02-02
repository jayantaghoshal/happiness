#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -xv
set +e
mkdir -p "$BUILD_NUMBER"

#shellcheck disable=SC2046
mv $(find "$WORKSPACE"/"$JOB_NAME"_test-* -name "vcc_test_results*.tar.gz") "$BUILD_NUMBER/"
rm -rf "$JOB_NAME"_test-*

cd "$BUILD_NUMBER"

#shellcheck disable=SC2046
mkdir $(find . -type f -iname '*.tar.gz' -exec basename {} '.tar.gz' ';')
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_"$JOB_NAME"_test-generic_*.tar.gz -C $(find . -type d -name "vcc_test_results_${JOB_NAME}_test-generic_*")
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_"$JOB_NAME"_test-audio_*.tar.gz -C $(find . -type d -name "vcc_test_results_${JOB_NAME}_test-audio_*")
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_"$JOB_NAME"_test-flexray_*.tar.gz -C $(find . -type d -name "vcc_test_results_${JOB_NAME}_test-flexray_*")
#shellcheck disable=SC2046
tar xvfz ./vcc_test_results_"$JOB_NAME"_test-apix_*.tar.gz -C $(find . -type d -name "vcc_test_results_${JOB_NAME}_test-apix_*")

rm -rf ./*.tar.gz


git archive --remote=ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars HEAD:tools/ci/test_report_generation/ xml_to_json_results.py | tar -x

git archive --remote=ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars HEAD:tools/ci/test_report_generation/ json_to_html_results.py | tar -x

git archive --remote=ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars HEAD:tools/ci/test_report_generation/ volvo_logo.png | tar -x

set +x
curl -X GET -u "$JENKINS_USER":"$JENKINS_PASSWORD" https://icup_android.jenkins.cm.volvocars.biz/job/"$JOB_NAME"/"$BUILD_NUMBER"/api/json?pretty=true > changelog.json
set -x

python3 xml_to_json_results.py

python3 json_to_html_results.py

rm -rf xml_to_json_results.py json_to_html_results.py

cd "$WORKSPACE"

# deploy it to the web server
if echo "$JOB_NAME" | grep -i "daily"
then
    cp -R "$BUILD_NUMBER" /cm/www/daily_test_report/
elif echo "$JOB_NAME" | grep -i "hourly"
then
    cp -R "$BUILD_NUMBER" /cm/www/hourly_test_report/
fi
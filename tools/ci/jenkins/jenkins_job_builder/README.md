# Jenkins job configurations for iCUP Android

This directory contains Jenkins Job Builder configurations to set up
Jenkins jobs required for iCUP Android.

Most of the CI scripts are located inside the vendor/volvocars repository 
and will be executed from there after the repository has been cloned.

However, in order to first clone the repository and checkout the commit to verify, 
some bootstrapping is required. Most of the code in these jobs are related to such.

Once the bootstrap-scripts or the job template has been modified, it has to be pushed to Jenkins manually.


## Installing Jenkins job builder

See https://docs.openstack.org/infra/jenkins-job-builder/index.html


## To test your config

```bash
jenkins-jobs test -o output ./jobs
```

## Downloading old configs:

This can be useful if you want to diff your generated files with the existing config.

```bash
# User is CDSID
# Get your token at https://icup_android.jenkins.cm.volvocars.biz/me/configure after logging in
CDSID=
TOKEN=
JOBS=(ihu_daily_test ihu_commit_check ihu_gate_build ihu_gate_test ihu_gate_test_flexray ihu_gate_test_audio ihu_gate_test_apix ihu_hourly_test ihu_hourly_test_flexray ihu_hourly_test_apix ihu_hourly_test_audio ihu_image_build icup_android_manifest_bump)

mkdir -p old
for job in "${JOBS[@]}"; do
    curl -s https://"$CDSID":"$TOKEN"@icup_android.jenkins.cm.volvocars.biz/job/"$job"/config.xml > old/"$job"
done
```


## Pushing the config to Jenkins

1. Make your changes to the config, try to make the bootstrap-scripts backwards compatible, or prepare the scripts
   in the repository in a first commit to ensure that the next commit can be backwards compatible without breaking
   the CI between step 6 and 9 below.
2. Test things locally. Thoroughly
3. Push your change as review to gerrit. (it might get Verified-1 if your change is not backwards compatible)
   Do NOT set Automerge+1
4. If your change is not backwards compatible or if you have added important static analyzers that shouldn't be avoided by using old CI:
    4a. Take the Gerrit Change-ID (not commit hash) and put into vendor/volvocars/tools/ci/ci_version
    4b. Take the Gerrit Change-ID (not commit hash) and put into the REQUIRED_CI_VERSION variable in ./jobs/bootstrap_common_commit_triggered.sh
5. Wait for your change to get Code review +2
6. Upload the new jobs to jenkins:
    ```bash
    export CDSID=
    export TOKEN=    #<<<-- Get your API Token from https://icup_android.jenkins.cm.volvocars.biz/me/configure
    #Note: This updates all the jenkins jobs under the "jobs" directory.
    jenkins-jobs --user "$CDSID" --password="$TOKEN" --conf=./jenkins_jobs.ini update ./jobs
    
    #To update all jobs under a specific file
    jenkins-jobs --user "$CDSID" --password="$TOKEN" --conf=./jenkins_jobs.ini update ./jobs/FILENAME.yml

    #To update specific jobs under a file
    jenkins-jobs --user "$CDSID" --password="$TOKEN" --conf=./jenkins_jobs.ini update ./jobs/FILENAME.yml JOBNAME1 JOBNAME2
    ```
7. If you got -1 in step3, add a comment in gerrit containing "!recheck" to get Jenkins to run it again with new jobs.
8. Press Automerge+1 in Gerrit
9. Ensure it builds and is merged to master by zuul


## Quirks

* The !include-raw command can't be used inline together with plain strings, we want to 
share some common shell-functions across all jobs and then run some job-specific code.
For the job-specific code to get access to the functions defined in the common-file, they
must be in the same shell-section in jenkins. Hence some of the included files are just one-liners
instead of inline which might look a bit odd. 

* To keep the scripts analyzed by shellcheck they have the .sh file ending and #!/bin/bash

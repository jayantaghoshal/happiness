### Zuul

Zuul is pre-configured to listen to events from the Gerrit event stream
and will connect to Gerrit at container startup. Zuul's merger
git repositories are served via a pre-configured Apache.

layout.yaml is stored at /etc/zuul/layout.yaml. Two pipeline (check and gate)
are already configured.

Zuul status page can be reached at http://localhost:80


Build and start
---------------

  docker build -t vcc_zuul .

  docker run -p 80:80 -p 8001:8001 -p 4730:4730 -p 8081:8081 vcc-zuul



Configure a first project to be validated via Zuul
--------------------------------------------------

Here is the first steps to perform in order to have a project hosted on Gerrit
and a job triggered by Zuul.

* Login to Gerrit as the admin user. Add your public key in the admin user
  settings page. If you don't have a key yet, create one:
```
$ ssh-keygen
$ cat ~/.ssh/id_rsa.pub
```
* Create a Job in Jenkins for "testproject" using the following command. The
  container already has a valid JJB configuration with a working job definition
  for "testproject".

```
$ sudo docker exec -i -t $CID /bin/bash
# # Create a job in Jenkins for a project call "testproject"
# jenkins-jobs --conf /etc/jenkins_jobs/jenkins_jobs.ini update /etc/jenkins_jobs/jobs
```

- The job "testproject-unit-tests" must be shown in the Jenkin job list
- As admin - create a project called "testproject" in Gerrit (check "create inital empty commit")
- Clone the new project on your local computer and submit the as a review

```
$ git clone http://ci.localdomain:8080/testproject
$ cd testproject
$ git checkout -b "first_commit"
$ cat > .gitreview << EOF
[gerrit]
host=ci.localdomain
port=29418
project=testproject.git
EOF
$ cat > run_tests.sh << EOF
#!/bin/bash
exit 0
EOF
$ chmod +x run_tests.sh
$ sudo pip install git-review
$ touch "$HOME/.ssh/known_hosts"
$ ssh-keygen -f "$HOME/.ssh/known_hosts" -R [ci.localdomain]:29418
$ git review -s # use "admin" as login and be sure to have the public key listed by ssh-add -l
$ git config --add gitreview.username "admin"
$ git add run_tests.sh .gitreview
$ git commit -m "first commit"
$ git review
```

In the Gerrit web UI you should see your new patch on "testproject" and a green check
sign added by Zuul in the "Verified" label.

If you succeed to have your patch validated by Zuul that means the platform is
ready to be used !



docker run -d -h ci.localdomain -v /dev/urandom:/dev/random -p 80:80 -p 8082:8081 vcc_zuul
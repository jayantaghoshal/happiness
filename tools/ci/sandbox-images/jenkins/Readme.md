How to use this image
---------------------

docker run -p 8080:8080 -p 50000:50000 jenkins

This will store the workspace in /var/jenkins_home. All Jenkins data lives in there - including plugins and configuration. You will probably want to make that a persistent volume (recommended):

docker run -p 8080:8080 -p 50000:50000 -v /your/home:/var/jenkins_home jenkins

This will store the jenkins data in /your/home on the host. Ensure that /your/home is accessible by the jenkins user in container (jenkins user - uid 1000) or use -u some_other_user parameter with docker run.

You can also use a volume container:

docker run --name myjenkins -p 8080:8080 -p 50000:50000 -v /var/jenkins_home jenkins

Then myjenkins container has the volume (please do read about docker volume handling to find out more).
# What

Docker images for Gerrit, Jenkins and Zuul to create a sandbox environment for testing how these
tools interact. Not intended for production!

# Prerequisites

    Docker engine and docker compose

# Usage:

    docker-compose build 
    docker-compse up

# Persistent data

Docker volume is used, use `docker volume` command to inspect/modify the data

# Ip addresses and ports

SUPER IMPORTANT:
Understand how docker-compose networking works:

 * Inside each container, localhost will only refer to itself inside the container
 * Outside the containers, localhost could refer to some container depending on the exposed ports
 * Inside a container, to connect to another container: Use the hostname assigned to each container. This is the same as the service-name in the docker-compose.yml
 * When exposing ports to the host using the HOSTPORT:CONTAINERPORT syntax, the port used INSIDE the container is the port you should use from OTHER containers.


# Troubleshooting

## Internet connection problems

All images have been tailored to work within VCC network behind
proxy, both inside each image and as environment passed into the images throgh the docker-compose.yml
If not behind VCC proxy, remove all these commands.



#Manual steps:

TODO: Automate these, right now not automated because they depend on timing, must be done after the services are installed and running.
Jenkins is especially troublesome as it has an interactive setup wizard and downloads stuff from internet during installation.

1. Create zuul gerrit user in gerrit web UI
   Add gerrit/zuul_id_rsa.pub as SSH key to this user
2. Add "Verified" options to gerrit code review
    * Go to projects->list->all projects, add gerrit_project.config
3. Create project called testproject in gerrit

4. Log in to Jenkins, create admin user with username/password: jenkins/password
5. Log in to jenkins, go to configure, enable Gearman plugin, set server to dock_zuul and port to 4730

6. Run jenkins job builder to create the jenkins jobs: 
        
        HOST        > docker ps | grep zuul
        HOST        > docker exec -ti %HASH FROM PREVIOUS COMMAND% /bin/bash
        ZUUL_DOCKER > /install_jenkins_jobs.sh
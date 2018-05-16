# Steps to start the service

* repo sync vendor/volvocars
* run './build.sh'
* run 'MONGODB_PASSWORD=password SWF_API_KEY=api_key ./run.sh'

#Restart example (build & run):
docker kill $(docker ps --filter ancestor=backup_mongodb --format "{{.ID}}") && ./build.sh && MONGODB_PASSWORD=password SWF_API_KEY=api_key ./run.sh

# get into the shell
docker exec -it container_id /bin/bash

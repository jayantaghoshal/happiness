# Steps to start the service

* repo sync vendor/volvocars
* run './build.sh'
* for Production, run 'MONGODB_PASSWORD=password ./run.sh'
* for Dev, run 'MONGODB_PASSWORD=password ./debug.sh'

# get into the shell
docker exec -it container_id /bin/bash


# To develop more

* Deploy similarly on developers machine and once the code is stable, merge it to master.

* Sync back in the server "gotsvl1416" and do the steps to start the serivce. Close the existing docker run if necessary.


# Assumption

This docker image expects the mongoDB docker container running on the same machine with the name "icup_android-mongo".

If we want to deploy this webserver in any other machine then it needs changes to the address(mongodb) in server.js file.
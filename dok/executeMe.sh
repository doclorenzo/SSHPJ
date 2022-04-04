#!/bin/bash
docker-compose up -d
echo "wait 12 seconds for container to start up"
sleep 30
echo "done"
docker exec -it dokInfluxUni bash -c "influx setup --force --username a --password aaaaaaaa --org a --bucket a --token aaa --name me"
echo "finished"

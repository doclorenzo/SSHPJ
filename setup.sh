#!/bin/bash
echo "$(tput setaf 7)insert username:"
read DOCK_IN_USERNAME
echo "$(tput setaf 7)insert password $(tput setaf 1) (min 8 character):$(tput setaf 7)"
read -s  DOCK_IN_PASSWORD
if [ ${#DOCK_IN_PASSWORD} -lt 8 ]; then echo "$(tput setaf 1)error"; exit
else echo "$(tput setaf 2)OK"
fi
echo "$(tput setaf 7)insert org:"
read DOCK_IN_ORG
echo "$(tput setaf 7)insert bucket:"
read DOCK_IN_BUCKET
echo "$(tput setaf 7)insert token:"
read DOCK_IN_TOKEN
echo "$(tput setaf 7)insert name:"
read DOCK_IN_NAME
echo ""
echo "$(tput setaf 7)creating docker network..."
docker network create --driver=bridge --subnet=172.28.0.0/16 --ip-range=172.28.5.0/24 --gateway=172.28.5.254 dokUniNet > /dev/null
echo "$(tput setaf 7)done...dokUniNet"
echo ""
echo "$(tput setaf 7)creating influxdb container and attaching it to the network..."
docker run -d --name dokUniInflux --expose 8086 -p 0.0.0.0:9999:8086 influxdb:2.0 > /dev/null
docker network connect --ip 172.28.5.1 dokUniNet dokUniInflux > /dev/null
echo "$(tput setaf 7)done...dokUniInflux"
echo "$(tput setaf 2)Wait for container to start up..."
sleep 30
echo "$(tput setaf 7)done"
echo "$(tput setaf 7)configuring influxdb container..."
docker exec -it dokUniInflux bash -c "influx setup --force --username ${DOCK_IN_USERNAME} --password ${DOCK_IN_PASSWORD} --org ${DOCK_IN_ORG} --bucket ${DOCK_IN_BUCKET} --token ${DOCK_IN_TOKEN} --name ${DOCK_IN_NAME}" >/dev/null
echo "$(tput setaf 7)done"
echo "$(tput setaf 2)influxdb running on port 9999 (or 172.28.5.1:8086)"
echo ""
echo "$(tput setaf 7)installing web server and attaching it to the network..."
docker run -d --name dokUniWeb -p 0.0.0.0:8000:8000 -e DOCK_IN_ORG=${DOCK_IN_ORG} -e DOCK_IN_BUCKET=${DOCK_IN_BUCKET} -e DOCK_IN_TOKEN=${DOCK_IN_TOKEN} doclorenzo/unimoreweb > /dev/null
docker network connect --ip 172.28.5.2 dokUniNet dokUniWeb > /dev/null
echo "$(tput setaf 7)done...dokUniWeb"
echo "$(tput setaf 2)web server running on port 8000"

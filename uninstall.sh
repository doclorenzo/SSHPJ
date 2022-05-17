#!/bin/bash
docker stop dokUniInflux dokUniWeb
echo "container stop"
docker rm dokUniInflux dokUniWeb
echo "container rm"
docker network rm dokUniNet
echo "network rm"

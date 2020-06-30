#!/bin/bash
set -ex

docker run -d --user (id -u) -v "$PWD/grafana.ini:/etc/grafana/grafana.ini" -v "$PWD/data:/var/lib/grafana" -p 3000:3000 grafana/grafana

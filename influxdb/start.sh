#!/bin/bash
set -ex

docker run -d --restart=always -p 8086:8086 -v $PWD:/var/lib/influxdb influxdb -config /var/lib/influxdb/influxdb.conf

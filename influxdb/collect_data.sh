#!/bin/bash
set -x
set -o pipefail


# $1: host
# $2: onlp oid, thermal-1
function onlp-collect {
    host=$1
    sensor=$2
    temp=$(ssh root@$host "/lib/platform-config/current/onl/bin/onlps oid to json $sensor user" | jq .Temperature | bc)
    echo $temp
}


# $1: host ip, used for ssh, e.g. 10.128.13.241
# $2: host name, used as index in db, e.g. as7712
# $3: rack, position on lab, e.g. stratum2
function collect {
    host_ip=$1
    host_name=$2
    rack=$3
    for i in `seq 1 15`; do
        onlp_sensor="thermal-$i"
        v=$(onlp-collect "$host_ip" $onlp_sensor)
        # Sanity check as most ONLP implementations suck
        if [[ -n $v ]] && (( $(echo "$v < 200" | bc -l) )) && (( $(echo "$v > 15" | bc -l) )); then
            curl -i -XPOST 'http://localhost:8086/write?db=temps&precision=s'\
                --data-binary "temperature,host=$host_name,rack=$rack,sensor=$onlp_sensor value=$v"
        fi
    done
}


# All monitored devices
# collect <ip> <name> <rack>
collect 10.128.13.241 as7712 stratum2
collect 10.128.13.113 z9100 stratum
collect 10.128.13.122 bf2556x-2 stratum2
collect 10.128.13.111 d5254 stratum
collect 10.128.13.115 d7032 stratum
collect 10.128.13.110 ag9064v1 stratum

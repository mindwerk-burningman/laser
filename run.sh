#!/bin/bash

IN=pentagram
OUT=sim

if [[ $# -eq 2 ]]; then
    IN=$1
    OUT=$2
fi

if [ "$IN" != "openlase-simple" ]; then
    IN=./$IN
fi

jack_server_control & pid=$!
PID_LIST+=" $pid";

sleep 1

$IN & pid=$!
PID_LIST+=" $pid";

if [ "$OUT" == sim  ] || [ "$OUT" == both  ]; then
    openlase-simulator & pid=$!
    PID_LIST+=" $pid";
fi

if [ "$OUT" == laser  ] || [ "$OUT" == both  ]; then
    ./lasershark_hostapp/lasershark_jack & pid=$!
    PID_LIST+=" $pid";
fi

sleep 2

if [ "$OUT" == sim  ] || [ "$OUT" == both  ]; then
    jack_connect libol:out_x simulator:in_x
    jack_connect libol:out_y simulator:in_y
    # R and G are swapped in our laser
    jack_connect libol:out_r simulator:in_b
    jack_connect libol:out_g simulator:in_r
    jack_connect libol:out_b simulator:in_b
fi

if [ "$OUT" == laser  ] || [ "$OUT" == both  ]; then
    jack_connect libol:out_x lasershark:in_x
    jack_connect libol:out_y lasershark:in_y
    jack_connect libol:out_r lasershark:in_r
    jack_connect libol:out_g lasershark:in_g
    jack_connect libol:out_b lasershark:in_b
fi

trap "kill $PID_LIST" SIGINT
wait $PID_LIST

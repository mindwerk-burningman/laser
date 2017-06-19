#!/bin/bash

jack_server_control & pid=$!
PID_LIST+=" $pid";

sleep 1

./pentagram & pid=$!
#openlase-simple & pid=$!
PID_LIST+=" $pid";
openlase-simulator & pid=$!
PID_LIST+=" $pid";
./lasershark_hostapp/lasershark_jack & pid=$!
PID_LIST+=" $pid";

sleep 1

jack_connect libol:out_x simulator:in_x
jack_connect libol:out_y simulator:in_y
jack_connect libol:out_r simulator:in_r
jack_connect libol:out_g simulator:in_g
jack_connect libol:out_b simulator:in_b
jack_connect libol:out_x lasershark:in_x
jack_connect libol:out_y lasershark:in_y
jack_connect libol:out_r lasershark:in_r
jack_connect libol:out_g lasershark:in_g
jack_connect libol:out_b lasershark:in_b

trap "kill $PID_LIST" SIGINT
wait $PID_LIST

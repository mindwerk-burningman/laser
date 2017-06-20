all: pentagram leapmotion lasershark_hostapp/lasershark_jack

lasershark_hostapp/lasershark_jack: 
	make -C lasershark_hostapp lasershark_jack

pentagram: pentagram.cpp
	g++ -std=c++11 -Wall -o $@  $^ -lopenlase

leapmotion: leapmotion.cpp
	g++ -std=c++11 -Wall -o $@  $^ -I./3rdParty/include -I./spline/src -L./3rdParty/lib -lopenlase -lLeap

clean:
	rm -f pentagram leapmotion
	make -C lasershark_hostapp clean
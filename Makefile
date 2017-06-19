all: pentagram lasershark_hostapp/lasershark_jack

lasershark_hostapp/lasershark_jack: 
	make -C lasershark_hostapp lasershark_jack

pentagram: pentagram.c
	gcc -Wall -o pentagram  pentagram.c `pkg-config --libs --cflags jack` -lopenlase

clean:
	rm -f pentagram
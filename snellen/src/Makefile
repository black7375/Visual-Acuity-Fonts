# Created on: Nov 10, 2008
# revised: Jan 24, 2014  A.H.
#     Author: Andrew Howlett


CFLAGS=`directfb-config --libs --cflags`

all: snellen 

snellen: snellen.c snellen.h
	gcc  -g -Wall -lm -lconfig $(CFLAGS) snellen.c -o snellen

clean:
	rm -f *~ remote snellen keybuffer \#* ../doc/*~

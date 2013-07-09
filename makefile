all:	wolf.c converter.c 
	#
	# --- --- Compiling the WOLF library --- ---
	#
	gcc -c wolf.c -o wolf.o -L. -lpthread -I.
	gcc -c converter.c -o converter.o
	ar rcs libwolf.a *.o

test: libwolf.a test.c
	#
	# --- --- Compiling an example-test programm --- ---
	#
	rm -f test.bin	
	gcc -c -o test.o -I./ test.c
	gcc -o test.bin -L./ -lwolf -lwebsockets
	rm -f test.o

clean:
	#
	# --- --- Cleaning up --- ---
	#
	rm -f libwolf.a
	rm -f test.bin
	rm -f *.o

install:
	#
	# WARNING: not yet implemented feature 
	#

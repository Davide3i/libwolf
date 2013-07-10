CC=gcc
CFLAGS=-c
SOURCES=wolf.c converter.c
RM=rm -f


all: $(SOURCES)
	#
	# --- --- Compiling the WOLF library --- ---
	#
	$(CC) $(CFLAGS) wolf.c -o wolf.o -L. -lpthread -I.
	$(CC) $(CFLAGS) converter.c -o converter.o
	ar rcs libwolf.a *.o

test: libwolf.a test.c
	#
	# --- --- Compiling an example-test programm --- ---
	#
	$(RM) test.bin	
	$(CC) $(CFLAGS) -o test.o -I./ test.c
	$(CC) test.o -o test.bin -L./ -lwolf -lwebsockets -lpthread
	$(RM) test.o

clean:
	#
	# --- --- Cleaning up --- ---
	#
	$(RM) libwolf.a
	$(RM) test.bin
	$(RM) *.o

install:
	#
	# --- --- install (only dipendence) --- ---
	#
	sudo cp libwebsockets.so.4.0.0 /usr/lib

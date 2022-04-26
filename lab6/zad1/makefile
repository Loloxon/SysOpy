CC=gcc
CFLAGS=-Wall -Wno-unused-variable

all:
	rm -f *.txt
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f client server *.o
CC = gcc
CFLAGS = -Wall -pthread

all: chatserver chatclient

chatserver: server/chatserver.c
	$(CC) $(CFLAGS) -o chatserver server/chatserver.c

chatclient: client/chatclient.c
	$(CC) $(CFLAGS) -o chatclient client/chatclient.c

clean:
	rm -f chatserver chatclient
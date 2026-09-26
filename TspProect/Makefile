CC = cc
CFLAGS = -Wall -Wextra -Wpedantic
THREAD_FLAGS = -pthread

all: server client

server: server.c
	$(CC) $(CFLAGS) $(THREAD_FLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f server client

.PHONY: all clean

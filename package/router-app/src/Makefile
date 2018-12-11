LDFLAGS=-pthread -lczmq -lzmq 
CFLAGS=-g -Wall


all:server client
server: server.c
	$(CC) $(LDFLAGS) $(CFLAGS) server.c -o server 
client: client.c
	$(CC) $(LDFLAGS) $(CFLAGS) client.c -o client
clean:
	rm -rf server client

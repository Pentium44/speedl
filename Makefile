CC=gcc
FLAGS=-lcurl -I/usr/local/include -L/usr/lib

all: speedl

speedl: client server
	$(CC) client.o -o speedl $(FLAGS)
	$(CC) server.o -o speedl-server $(FLAGS)
	
client:
	$(CC) -c client.c $(FLAGS)
	
server:
	$(CC) -c server.c $(FLAGS)

clean: 
	rm *o speedl speedl-server

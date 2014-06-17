CC=gcc

# Cross compiling in Windows (CYGWIN)
#CC=i686-pc-cygwin-gcc
#CC=x86_64-pc-cygwin-gcc

EXE_CLIENT=speedl
EXE_SERVER=speedl-server

FLAGS=-lcurl -I/usr/include -L/usr/lib

all: speedl

speedl: client server
	$(CC) client.o -o $(EXE_CLIENT) $(FLAGS)
	$(CC) server.o -o $(EXE_SERVER) $(FLAGS)
	
client:
	$(CC) -c client.c $(FLAGS)
	
server:
	$(CC) -c server.c $(FLAGS)

clean: 
	rm *o speedl speedl-server

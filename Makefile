SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
#SRC = server.c client.c utility_fuc.c
#OBJ = server.o client.o utility_fuc.o

.PHONY : clean

FLAG := -Wall -I.

all : server client

server : server.o
	gcc -o $@ $<

client : client.o
	gcc -o $@ $<

$(OBJ) : $(SRC)
	gcc -c $^ $(FLAG)

clean :
	rm -rf *.o server client

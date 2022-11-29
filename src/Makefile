# Makefile for TWMailer
#
# Compiler
CC=gcc
#
# Compiler Flags
#
CFLAGS=-g
LIBS=-lldap -llber
rebuild: clean all
all: ../bin/server ../bin/client

clean:
	clear
	rm -f ../bin/* ../obj/*

../obj/client.o: client.c
	${CC} ${CFLAGS} -o ../obj/client.o client.c -c

../obj/server.o: server.c
	${CC} ${CFLAGS} -o ../obj/server.o server.c -c

../bin/client: ../obj/client.o
	${CC} ${CFLAGS} -o ../bin/client ../obj/client.o ${LIBS}

../bin/server: ../obj/server.o
	${CC} ${CFLAGS} -o ../bin/server ../obj/server.o ${LIBS}
	


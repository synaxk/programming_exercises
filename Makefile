all: myfind

getopt: myfind.c
	gcc -std=c99 -Wall -Werror -o myfind getopt.c

clean:
	rm -f myfind


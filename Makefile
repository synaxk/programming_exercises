all: myfind

myfind: myfind.c
	gcc -std=c99 -Wall -Werror -o myfind myfind.c

clean:
	rm -f myfind


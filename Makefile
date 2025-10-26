CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic


all: head tail

head: head.c
	$(CC) $(CFLAGS) -o head head.c

tail: tail.c
	$(CC) $(CFLAGS) -o tail tail.c

clean: 
	rm -f head tail *.o

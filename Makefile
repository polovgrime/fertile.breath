CC = gcc
CFLAGS = -Wall -g
OBJS = main.o 
SOURCES = main.c
# TODO WILDCARD COMPILATION!!
# ASSUME WE NEED EVERYTHING IN SAID DIRECTORY!
# need to link math????
main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main -lm

main.o: $(SOURCES)
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f %.o
	rm -f main.o
	rm -f main

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj18

all: ifj18

ifj18: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)


CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=c99 -pedantic -pthread
OBJ = prog.o
VFLAGS = -v --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes
EXEC = ./prog 2 4

all: prog

prog: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o prog

valgrind:
	valgrind $(VFLAGS) $(EXEC)

run:
	$(EXEC)
CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=c99 -pedantic -pthread
OBJ = prog.o threadPool.o threadQueue.o taskQueue.o
VFLAGS = -v --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes
EXEC = ./prog 2 4
DEPS = threadQueue.h threadPool.h taskQueue.h

all: prog

prog: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o prog

valgrind:
	valgrind $(VFLAGS) $(EXEC)

run:
	$(EXEC)
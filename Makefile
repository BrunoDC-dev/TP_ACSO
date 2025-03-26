CC = gcc
CFLAGS = -g -O0 -I./include

sim: shell.c src/sim.c
	$(CC) $(CFLAGS) shell.c src/sim.c -o sim

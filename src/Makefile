sim: shell.c sim.c utils.c arithmetic.c memory.c comparison.c branching.c
	gcc -g -O0 -Wall $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim

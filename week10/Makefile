CC=gcc

all: ex_div0 ex_sigint ex_kill ex_daemon

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f *.o ex_div0 ex_sigint ex_kill ex_daemon

CC = gcc
CFLAGS = -O

all: ex_sleep ex_ps ex_daemon

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o ex_sleep ex_ps ex_daemon

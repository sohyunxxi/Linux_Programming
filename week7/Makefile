CC = gcc
CFLAGS = -O

all: ex_file_RW ex_triple_app ex_zombie_orphan ex_simple_shell

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o ex_file_RW ex_triple_app ex_zombie_orphan ex_simple_shell

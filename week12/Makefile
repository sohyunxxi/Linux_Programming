CC=gcc

all: ex_pipe1 ex_pipe2 ex_sender ex_sender2 ex_receiver ex_receiver2

ex_%: ex_%.o
	$(CC) -o $@ $<
%.o: %.c
	$(CC) -c -o $@ $<
clean:
	rm -f *.o ex_pipe1 ex_pipe2 ex_sender ex_sender2 ex_receiver ex_receiver2

CC = gcc
CFLAGS = -Wall -g

all: ex_sem reader1 reader2 writer1 writer2

ex_sem: sem_ctl.c sem_main.c sem_header.h
	$(CC) $(CFLAGS) sem_ctl.c sem_main.c -o ex_sem

reader1: share_init.c share_reader.c share_header.h
	$(CC) $(CFLAGS) share_init.c share_reader.c -o reader1

reader2: share_init2.c share_reader.c share_header.h
	$(CC) $(CFLAGS) share_init2.c share_reader.c -o reader2

writer1: share_init.c share_writer.c share_header.h
	$(CC) $(CFLAGS) share_init.c share_writer.c -o writer1

writer2: share_init2.c share_writer.c share_header.h
	$(CC) $(CFLAGS) share_init2.c share_writer.c -o writer2

clean:
	rm -f ex_sem reader1 reader2 writer1 writer2

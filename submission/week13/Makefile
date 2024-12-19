CC=gcc

all: ex_record_lock ex_record_lock2 msg_sender msg_receiver msg_receiver2

%: %.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o ex_record_lock ex_record_lock2 msg_sender msg_receiver msg_receiver2


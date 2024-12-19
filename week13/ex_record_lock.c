#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int fd;

void int_handler(int signo) { // SIGINT handler
	close(fd);
	exit(0);
}

int main() {
	const char *filename = "test_file.txt";
	struct flock my_lock;
	
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_handler = int_handler;
	sigaction(SIGINT, &act, NULL);
	
	my_lock.l_type = F_WRLCK;
	my_lock.l_whence = SEEK_SET;
	
	if ((fd = open(filename, O_RDWR | O_CREAT, 0666)) == -1) {
		perror("Couldn't open file");
		return 1;
	}

	off_t l_start, l_len;

	while(printf("Enter l_start and l_len : ")){
		if (scanf("%ld %ld", &l_start, &l_len) != 2) {
			fprintf(stderr, "Enter valid input.\n");
			while (getchar() != '\n');
			continue;
		}
	
		my_lock.l_start = l_start;
		my_lock.l_len = l_len;

		if (fcntl(fd, F_SETLK, &my_lock) == -1) {
			if (errno == EACCES || errno == EAGAIN) {
				printf("Already locked.\n\n");
			} else {
				perror("Locking failed due to other errors");
			}
		} else {
			printf("Lock Success: start = %ld, end = %ld\n\n", l_start, l_start + l_len - 1);
		}		
	}
	close(fd); // 여기로 도달하지 않음
	return 0;
}

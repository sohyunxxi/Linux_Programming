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
	int type;
	
	printf("type 1 : lock / type2 : unlock\n");

	while(printf("Enter type, l_start and l_len : ")){
		if (scanf("%d %ld %ld",&type, &l_start, &l_len) != 3) {
			fprintf(stderr, "Enter valid input.\n");
			while (getchar() != '\n');
			continue;
		}
	
		my_lock.l_start = l_start;
		my_lock.l_len = l_len;
		
		if(type==1){
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
		else if(type==2){
			if(fcntl(fd, F_UNLCK, &my_lock)==-1){
				if(errno == EACCES || errno == EAGAIN){
					printf("Already unlocked.\n\n");
				} else{
					perror("Unlocking failed due to other errors");
				}
			}else{
				printf("Unlock Success: start = %ld, end = %ld\n\n", l_start, l_start + l_len -1);
			}
		}
		else{
			perror("type error");
		}
	}
	close(fd); // 여기로 도달하지 않음
	return 0;
	
}

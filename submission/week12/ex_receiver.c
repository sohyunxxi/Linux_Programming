#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define BUFSIZE 64

int main(int argc, char * argv[]) {
	char buf[BUFSIZE];
	int fd;
	
	if (argc != 2) {
		printf("Usage: %s <FIFO NAME>", argv[0]);
		return 1;
	}

	if (mkfifo(argv[1], 0666) ==-1) {
		if (errno != EEXIST) {
			perror("mkfifo"); 
			return 2;
		}	
	}

	if ((fd = open(argv[1], O_RDWR)) < 0) {
		perror("open"); 
		return 3;
	}

	while (read(fd, buf, BUFSIZE) > 0) {
		printf("Msg received: %s\n", buf);
	} 
	return 0;
}

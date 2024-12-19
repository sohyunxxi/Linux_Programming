#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

void tail(int fd);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(-1);
	}
	
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(-2);
	}

	tail(fd);

	close(fd);
	return 0;
}

void tail(int fd) {
	off_t file_size = lseek(fd,0,SEEK_END); //hint: lseek을 통해서 확인 가능
	if (file_size == -1) {
		perror("lseek");
		exit(-3);
	}
	
	char buffer[BUFFER_SIZE];
	off_t cur_pos = file_size; //cur_pos 파일의 끝을 나타내게끔 초기화
	int nline = 0;

	while (cur_pos > 0 && nline <= 10) { //파일의 끝으로부터 거꾸로 이동하며 10줄이 되는 지점을 탐색
		ssize_t bytes_to_read = (cur_pos > BUFFER_SIZE) ? BUFFER_SIZE : cur_pos;
		cur_pos -= bytes_to_read;
		if (lseek(fd,cur_pos,SEEK_SET)==-1) {//찾은 cur_pos로 file position 업데이트
			perror("lseek");
			exit(-4);
		}
		ssize_t nread = read(fd, buffer, bytes_to_read);
		if (nread == -1) {
			perror("read");
			exit(-5);
		}

		for (ssize_t i = nread - 1; i >= 0; --i) {
			if (buffer[i]=='\n') {//hint: 행의 수를 셀 수 있는 기준이 되는 character
				++nline;
				if (nline > 10) {
					cur_pos += i + 1;
					break;
				}
			}
		}
	}
	if (lseek(fd,cur_pos,SEEK_SET)==-1) { //찾은 위치로 file position 업데이트
		perror("lseek");
		exit(-6);
	}

	ssize_t nread;

	while ((nread = read(fd, buffer, BUFFER_SIZE)) > 0) {
		if (write(STDOUT_FILENO, buffer, nread)<0) {
			perror("write");
			exit(-7);
		}
	}
	
	if (nread == -1) {
		perror("read");
		exit(-8);
	}
}



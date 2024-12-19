#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define PERMS 0644
#define BUFSIZE 4096

int copyfile (const char*, const char*);

int main(int argc, char *argv[]) {
	char *infile = argv[1];
	char *outfile = argv[2];
	
	int retcode = copyfile(infile, outfile);
	
	if(retcode == 0) printf("copy succeed!\n");
	else perror("copy failed");
	
	return 0;
}

int copyfile (const char* name1, const char* name2) {
	int infile, outfile; //각각의 fd
	ssize_t nread; //얼마나 읽었나
	char buffer[BUFSIZE];

	if ((infile = open(name1, O_RDONLY)) == -1)
		return -1;
	if ((outfile = open(name2, O_WRONLY | O_CREAT | O_TRUNC, PERMS)) == -1) {
		close (infile);
		return -2;
	}

	while ((nread = read(infile, buffer, BUFSIZE))>0) { //hint: 무언가를 쓰기 전에, 쓸 내용을 읽어야 함
		if (nread > (write(outfile, buffer, nread))) { //hint: 파일에 쓴 내용이 읽은 내용보다 적으면 오류
			close(infile);
			close(outfile);
			return -3;
		}
	}
	close(infile);
	close(outfile);
	if (nread == -1) return -4;
	else return 0;
}

#include "share_header.h"

struct sembuf p1 = {0, -1, 0}, p2 = {1, -1, 0};
struct sembuf v1 = {0, 1, 0}, v2 = {1, 1, 0};

void sigint_handler(int sig) {
	remobj();
	exit(1);
}

void reader (int semid, struct databuf *buf) {
	for(;;) {
		buf->d_nread = read(0, buf->d_buf, SIZ);
		semop(semid, &v1, 1); //stdin에서 read 하는 작업이 완료 됐다는 signal을 보냄
		semop(semid, &p2, 1); //stdout이 write 작업을 마치길 wait 함.

		if (buf->d_nread <= 0)
			return;
	}
}

int main() {
	int semid;
	pid_t pid;
	struct databuf *buf;
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sigint_handler;
	sigaction(SIGINT, &act, NULL);
	semid = getsem();
	getseg(&buf);
	reader(semid, buf);
	return 0;
}

#include "share_header.h"
struct sembuf p1 = {0, -1, 0}, p2 = {1, -1, 0};
struct sembuf v1 = {0, 1, 0}, v2 = {1, 1, 0};

void sigint_handler(int sig) {
	remobj();
	exit(1);
}

void writer (int semid, struct databuf *buf) {
	for(;;) {
		semop(semid, &p1, 1); //sem value의 초기값이 0이므로, 우선 wait 한다.
		semop(semid, &v2, 1); //writer가 깨어났음을 알림
	
		if (buf->d_nread <= 0)
			return;

		write(1, buf->d_buf, buf->d_nread);
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
	writer(semid, buf);
	return 0;
}

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void int_handler(int a) {
	printf("\nSIGINT caught\n");
}
int main() {
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = int_handler;
	sigaction(SIGINT, &act, NULL);
	sigset_t set_int;
	sigaddset(&set_int, SIGINT);
	
	for (int i = 0;i < 4; i++) {
		printf("sleep call #%d\n",i);
		sigprocmask(SIG_BLOCK, &set_int, NULL);
		sleep(3);
		sigprocmask(SIG_UNBLOCK, &set_int, NULL);
	}
	return 0;
}

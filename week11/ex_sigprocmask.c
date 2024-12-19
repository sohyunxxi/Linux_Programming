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

	for (int i = 0;i < 4;i++) {
		printf("sleep call #%d\n",i);
		sleep(3);
	}

	return 0;
}

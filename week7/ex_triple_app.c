#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
void all_exit() {
	printf("모든 자식 프로세스 및 부모 프로세스가 종료되었습니다.\n");
}

void child_exit() {
	printf("자식 프로세스 종료.\n");
}

int main() {
	atexit(all_exit);
	pid_t pids[3];
	int status;
	char *commands[3][3] = {
		{"ls", "-l", NULL},
		{"sleep", "5", NULL},
		{"echo", "Hello", NULL}
	};


	for (int i = 0; i < 3; i++) {
		pids[i] = fork();
		if (pids[i] == 0) {
			atexit(child_exit);
			printf("자식 프로세스 %d가 실행 중입니다.\n", getpid());

			if (execvp(commands[i][0], commands[i]) == -1)
			{
				perror("execvp fail");
				exit(1);
			}
		} else if (pids[i] < 0) {
			perror("fork fail");
			exit(1);
		}
	}

	for (int i = 0; i < 3; i++) {
		pid_t pid = waitpid(pids[i], &status, 0);
		if (pid > 0) {
			if (WIFEXITED(status)) {
				printf("부모 가라사대: 자식 프로세스 %d 종료, 종료 코드: %d\n", pid,
				WEXITSTATUS(status));
			} else {
				printf("부모 가라사대: 자식 프로세스 %d 비정상 종료\n", pid);
			}
		} else {
			perror("waitpid 실패");
		}
	}
	return 0;
}

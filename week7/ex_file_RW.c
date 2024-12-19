#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	int status;
	int fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		perror("파일 열기 실패");
		exit(1);
	}

	pid = fork();
	if (pid < 0) {
		perror("fork 실패");
		exit(1);
	}

	if (pid == 0) {
		dprintf(fd, "이 내용은 output.txt 파일에 기록됩니다.\n");
		dprintf(fd, "자식 프로세스에서 open을 사용하여 데이터를 씁니다.\n");
		close(fd);
		exit(100);
	}
	else {
		if (waitpid(pid, &status, 0) < 0) {
			perror("waitpid 실패");
			exit(1);
		}
		if (WIFEXITED(status)) {
			printf("자식 프로세스 %d 종료, 종료 코드: %d\n", pid,
			WEXITSTATUS(status));
		} else {
			printf("자식 프로세스가 비정상적으로 종료되었습니다.\n");
		}
		printf("파일 내용 출력:\n");
		lseek(fd, 0, SEEK_SET);
		char buffer[256];
		ssize_t bytes_read;
		while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
			buffer[bytes_read] = '\0';
			printf("%s", buffer);
		}
		if (bytes_read < 0) {
			perror("파일 읽기 실패");
		}
		close(fd);
	}
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

void parse_command(char *command, char **args, bool *background) {
	int i = 0;
	args[i] = strtok(command, " \n");
	
	while (args[i] != NULL) {
		i++;
		args[i] = strtok(NULL, " \n");
	}

	if (i > 0 && strcmp(args[i - 1], "&") == 0) {
		*background = true;
		args[i - 1] = NULL; // '&'는 명령어에서 제외
	} else {
		*background = false;
	}
}

int main() {
	char command[MAX_COMMAND_LENGTH];
	char *args[MAX_ARGS];
	
	pid_t pid;
	int status;
	char cwd[1024];
	char hostname[1024];
	char *username;
	bool background;
	// 호스트 이름 가져오기
	gethostname(hostname, sizeof(hostname));
	// 사용자 이름 가져오기
	username = getlogin();
	while (1) {
		// 현재 디렉터리 가져오기
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
		// 사용자 이름, 호스트 이름, 현재 디렉터리를 출력
			printf("%s@%s:%s$ ", username, hostname, cwd);
		} else {
			perror("getcwd error");
		}
	
		if (fgets(command, sizeof(command), stdin) ==NULL) {
			perror("fgets error");
			exit(EXIT_FAILURE);
		}
		parse_command(command, args, &background);
	
		if (args[0] == NULL) continue;
	
		if (strcmp(args[0], "exit") == 0) break;
			
		//fill
		pid=fork();

		if (pid < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		} else if (pid==0) {
			if (execvp(args[0],args)==-1) {
				perror("execvp error");
				exit(EXIT_FAILURE);
			}
		}

		else {
			if (!background) {
			/*fill*/
				waitpid(pid,&status,0);		
			} else {
				printf("[PID %d] running in background\n", pid);
			}
			
			//fill
			while ((waitpid(-1,&status,WNOHANG))>0) {}
		}
	}
	return 0;
}

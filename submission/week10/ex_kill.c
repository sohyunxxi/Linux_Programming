#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status, exit_status, sig_no;

    pid = fork();
    if (pid == 0) {
        printf("[Child] %d: sleep for 20 seconds\n", getpid());
        sleep(20);
        exit(1);
    }

    do {
        if ((pid = waitpid(pid, &status, WUNTRACED | WCONTINUED)) == -1) {
            perror("wait failed");
            exit(1);
        }
        
        if (WIFEXITED(status)) { // 자식 프로세스가 정상 종료했는지 확인
            exit_status = WEXITSTATUS(status); // 종료 코드를 추출
            printf("Child terminated normally\n");
            printf("Exit status from %d was %d\n", pid, exit_status);
        }
        
        if (WIFSIGNALED(status)) { // 자식 프로세스가 시그널로 종료되었는지 확인
            sig_no = WTERMSIG(status); // 종료 시그널 번호 추출
            printf("Child was terminated by a signal\n");
            printf("Signal number %d terminated child %d\n", sig_no, pid);
        }
        
        if (WIFSTOPPED(status)) { // 자식 프로세스가 시그널로 멈췄는지 확인
            sig_no = WSTOPSIG(status); // 멈춘 시그널 번호 추출
            printf("Child was stopped by delivery of a signal\n");
            printf("Signal number %d stopped child %d\n", sig_no, pid);
        }
        
        if (WIFCONTINUED(status)) { // 자식 프로세스가 시그널로 재개되었는지 확인
            printf("Child was resumed by delivery of a signal\n");
        }
        
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    
    return 0;
}


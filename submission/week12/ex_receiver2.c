#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define BUFSIZE 64
#include <signal.h>
#include <stdlib.h>
#include <sys/select.h>
#include <setjmp.h>

fd_set readfds;
int nfds = 3;
sigjmp_buf position;

void usr1_handler(int signo, siginfo_t *siginfo, void *param2) {
    char fifo_name[BUFSIZE];
    sprintf(fifo_name, "FIFO_%d", siginfo->si_pid);
    printf("FIFO Created: %s\n", fifo_name);
    mkfifo(fifo_name, 0666);
    int fd = open(fifo_name, O_RDONLY);
    FD_SET(fd, &readfds);
    if (fd >= nfds) nfds = fd + 1;
    siglongjmp(position, 1);
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_sigaction = usr1_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    char buf[BUFSIZE];
    int nread;

    printf("PID: %d\n", getpid());

    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    fd_set fds;
    sigsetjmp(position, 1);
    while (fds = readfds, select(nfds, &fds, NULL, NULL, NULL) > 0) {
        if (FD_ISSET(0, &fds)) read(0, buf, 1);
        for (int fd = 0; fd < nfds; fd++) {
            if (FD_ISSET(fd, &fds)) {
                if (fd == 0) {
                    read(0, buf, sizeof(buf));
                    printf("Input from stdin: %s", buf);
                } else {
                    int nread = read(fd, buf, sizeof(buf) - 1);
                    if (nread > 0) {
                        buf[nread] = '\0';
                        printf("%s\n", buf);
                    } else if (nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                    } else {
                        perror("read error");
                    }
                }
            }
        }
    }
    return 0;
}


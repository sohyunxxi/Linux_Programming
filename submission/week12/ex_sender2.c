#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define BUFSIZE 64
#include <signal.h>
#include <stdlib.h>

char fifo_name[BUFSIZE];
int fd;

void int_handler(int signo) {
    close(fd);
    if (unlink(fifo_name) != 0) {
        perror("unlink");
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    char buf[BUFSIZE];
    pid_t pid_receiver;

    if (argc < 2) {
        printf("Usage: %s <RECEIVER PID>\n", argv[0]);
        return 1;
    }

    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_handler = int_handler;
    sigaction(SIGINT, &act, NULL);

    sprintf(fifo_name, "FIFO_%d", getpid());
    if (mkfifo(fifo_name, 0666) == -1) {
        perror("mkfifo error");
        return 1;
    }
    printf("FIFO_NAME: %s\n", fifo_name);

    pid_receiver = atoi(argv[1]);
    kill(pid_receiver, SIGUSR1);

    if ((fd = open(fifo_name, O_WRONLY)) < 0) {
        perror("open");
        return 2;
    }

    while (fgets(buf, sizeof(buf), stdin)) {
        ssize_t len = strlen(buf);
        if (write(fd, buf, len) == -1) {
            perror("write error");
            break;
        }
    }

    close(fd);
    unlink(fifo_name);
    return 0;
}


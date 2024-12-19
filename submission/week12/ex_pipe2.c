#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#define BUFSIZE 64

int main() {
    int ptc_fd[2]; /* Parent to Child pipe */
    int ctp_fd[2]; /* Child to Parent pipe */
    char buf[BUFSIZE];

    if(pipe(ptc_fd) == -1) {
        perror("parent to child pipe");
        return 0;
    }

    if (fcntl(ptc_fd[0], F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
    }

    if(pipe(ctp_fd) == -1) {
        perror("child to parent pipe");
        return 0;
    }

    switch(fork()) {
        case -1:
            perror("fork");
            return 0;
        case 0: /* Child Process */
            close(ctp_fd[0]);
            close(ptc_fd[1]);
            write(ctp_fd[1], "Hello, I'm child.", BUFSIZE);

            while(read(ptc_fd[0], buf, BUFSIZE) == -1) {
                if (errno == EAGAIN ) {
                    printf("pipe is empty\n");
                    sleep(1);
                }
                else {
                    perror("read call");
                    return 0;
                }
            }
            printf("Message from parent: %s\n", buf);
            return 0;

        default: /* Parent Process */
            close(ptc_fd[0]);
            close(ctp_fd[1]);
            read(ctp_fd[0], buf, BUFSIZE);
            printf("Message from child: %s\n", buf);
            sleep(5);
            write(ptc_fd[1], "Hello, I'm your parent.", BUFSIZE);
            wait(NULL);
            return 0;
    }
}


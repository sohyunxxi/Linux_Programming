#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void child_handler(int a) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    struct sigaction act;
    act.sa_handler = child_handler;
    sigfillset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, NULL);

    fork();
    fork();

    for (;;);
    return 0;
}


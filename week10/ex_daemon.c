#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGHUP, &sa, NULL);

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    FILE *log_file = fopen("simple_daemon.log", "w");
    if (log_file == NULL) {
        exit(EXIT_FAILURE);
    }

    pid_t daemon_pid = getpid();
    fprintf(log_file, "Daemon started with PID: %d\n", daemon_pid);

    int count = 0;
    while (1) {
        fprintf(log_file, "count : %d\n", count++);
        fflush(log_file);
        sleep(5);
    }

    fclose(log_file);
    return EXIT_SUCCESS;
}


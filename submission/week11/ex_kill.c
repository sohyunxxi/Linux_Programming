#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int opt;
    int flag_g = 0;
    int pid;
    int signal_num;

    while ((opt = getopt(argc, argv, "g")) != -1) {
        switch (opt) {
            case 'g':
                flag_g = 1;
                break;
            default:
                printf("Unavailable option\n");
                return 1;
        }
    }

    if (argc - optind != 2) {
        printf("Usage: %s [-g] SIGNALNUM PID\n", argv[0]);
        return 1;
    }

    signal_num = atoi(argv[optind]);
    pid = atoi(argv[optind + 1]);

    if (flag_g) {
        if (kill(-pid, signal_num) == -1) {
            perror("Error sending signal to process group");
            return 1;
        }
        printf("Sent signal %d to process group with PGID %d\n", signal_num, pid);
    } else {
        if (kill(pid, signal_num) == -1) {
            perror("Error sending signal to process");
            return 1;
        }
        printf("Sent signal %d to process with PID %d\n", signal_num, pid);
    }

    return 0;
}


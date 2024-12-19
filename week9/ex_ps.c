#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

typedef struct {
    int pid;
    char tty[BUFFER_SIZE];
    char cmd[BUFFER_SIZE];
} ProcessInfo;

void get_tty(char *pid_str, char *tty);
void get_command(char *pid_str, char *cmd);

int main() {
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    if (!dir) {
        perror("opendir");
        return 1;
    }

    char current_tty[BUFFER_SIZE];
    char link[BUFFER_SIZE];
    snprintf(link, sizeof(link), "/proc/self/fd/0");
    ssize_t len = readlink(link, current_tty, sizeof(current_tty) - 1);

    if (len != -1) {
        current_tty[len] = '\0';
    } else {
        perror("readlink");
        return 1;
    }

    printf("%7s %-15s %-15s\n", "PID", "TTY", "CMD");

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            ProcessInfo proc;
            proc.pid = atoi(entry->d_name);
            get_tty(entry->d_name, proc.tty);

            if (strncmp(current_tty, proc.tty, strlen(current_tty)) != 0) {
                continue;  // Ignore TTYs other than the current terminal
            }

            get_command(entry->d_name, proc.cmd);
            printf("%-7d %-15s %-15s\n", proc.pid, proc.tty, proc.cmd);
        }
    }

    closedir(dir);
    return 0;
}

void get_tty(char *pid_str, char *tty) {
    char path[BUFFER_SIZE], link[BUFFER_SIZE];
    snprintf(path, sizeof(path), "/proc/%s/fd/0", pid_str);
    ssize_t len = readlink(path, link, sizeof(link) - 1);

    if (len != -1) {
        link[len] = '\0';
        strncpy(tty, link, BUFFER_SIZE);
    } else {
        strcpy(tty, "?");
    }
    return;
}

void get_command(char *pid_str, char *cmd) {
    char path[BUFFER_SIZE];
    snprintf(path, sizeof(path), "/proc/%s/cmdline", pid_str);
    FILE *file = fopen(path, "r");

    if (file) {
        if (fgets(cmd, BUFFER_SIZE, file) == NULL) {
            strcpy(cmd, "Unknown");
        }
        fclose(file);
    } else {
        strcpy(cmd, "Unknown");
    }
    return;
}


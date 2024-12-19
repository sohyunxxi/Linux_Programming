#include "smallsh.h"
#include <stdbool.h>
#include <signal.h>

char prompt[200] = "Command> "; 
void sigchld_handler(int signo) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
}

void initializePrompt() {
    char cwd[200];                         
    struct passwd *user = getpwuid(getuid()); 

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd error");
        return;
    }

    if (strstr(cwd, user->pw_dir) != NULL) {
        if (strcmp(user->pw_dir, cwd) == 0) {
            snprintf(prompt, sizeof(prompt), "~$ ");
        } else {
            snprintf(prompt, sizeof(prompt), "~%s$ ", cwd + strlen(user->pw_dir));
        }
    } else {
        snprintf(prompt, sizeof(prompt), "%s$ ", cwd);
    }
}

int main() {
  struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    initializePrompt();

    while (userin(prompt) != EOF) {
        procline(); 
    }
    return 0;
}


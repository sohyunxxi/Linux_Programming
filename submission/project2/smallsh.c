#include "smallsh.h"

static char inpbuf[MAXBUF];
static char tokbuf[2 * MAXBUF];
static char *ptr = inpbuf;
static char *tok = tokbuf;

static char special[] = {' ', '\t', '&', ';', '\n', '\0'};
sigjmp_buf env;

void sigint_handler(int signo) {
    printf("\n");
    siglongjmp(env, 1);
}

void sigint_foreground_handler(int signo){
    printf("\n");
}

int userin(char *p) {
    static struct sigaction sa;

    if (sigsetjmp(env, 1) == 0) {
        sa.sa_handler = sigint_handler;
        sigfillset(&(sa.sa_mask));
        sigaction(SIGINT, &sa, NULL);
    }

    int c, count;
    ptr = inpbuf;
    tok = tokbuf;

    printf("%s", p);

    count = 0;

    while (1) {
        if ((c = getchar()) == EOF) {
            return EOF;
        }
        if (count < MAXBUF)
            inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            return count;
        }
        if (c == '\n' || count >= MAXBUF) {
            printf("smallsh: input line too long\n");
            inpbuf[0] = '\0';
            count = 0;
            printf("%s", p);
        }
    }
}

int gettok(char **outptr) {
    int type;
    *outptr = tok;

    while (*ptr == ' ' || *ptr == '\t')
        ptr++;

    *tok++ = *ptr;
    switch (*ptr++) {
    case '\n':
        type = EOL;
        break;
    case '&':
        type = AMPERSAND;
        break;
    case ';':
        type = SEMICOLON;
        break;
    case '|':
        type = PIPE;
        break;
    default:
        type = ARG;
        while (inarg(*ptr))
            *tok++ = *ptr++;
    }
    *tok++ = '\0';
    return type;
}

int inarg(char c) {
    char *wrk;

    for (wrk = special; *wrk; wrk++) {
        if (c == *wrk)
            return 0;
    }
    return 1;
}

void procline() {
    char *arg[MAXARG + 1];
    char *cmd_arg_left[MAXARG +1];
    char *cmd_arg_right[MAXARG +1];
    int toktype, type;
    int ispipe = 0, narg = 0, num_arg_left = 0, num_arg_right = 0;

    for (;;) {
        switch (toktype = gettok(&arg[narg])) {
        case PIPE:
                ispipe = 1;
                num_arg_left = narg;
                narg = 0;
                break;
        case ARG:
            if (ispipe) {
                if (num_arg_right < MAXARG) {cmd_arg_right[num_arg_right++] = arg[narg++];}
            } else {
                if (num_arg_left < MAXARG) {cmd_arg_left[num_arg_left++] = arg[narg++];}
            }

            break;
        case EOL:
        case SEMICOLON:
        case AMPERSAND:
            if (toktype == AMPERSAND)
                type = BACKGROUND;
            else
                type = FOREGROUND;
            if (ispipe) {
                cmd_arg_left[num_arg_left] = NULL;
                cmd_arg_right[num_arg_right] = NULL;
                using_pipe(cmd_arg_left, cmd_arg_right, type);
            } else {
                if (narg != 0) {
                        arg[narg] = NULL;
                        runcommand(arg, type);
                }
            }
            if (toktype == EOL)
                return;
            narg = 0;
            ispipe = 0;
            num_arg_left = 0;
            num_arg_right = 0;
            break;
        }
    }
}

void using_pipe(char* left[], char* right[], int type){
    pid_t left_pid, right_pid;

    int fd_array[2];
    if(pipe(fd_array) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    if ((left_pid = fork()) < 0) {
        perror("fork call");
        return;
    }

    if (left_pid == 0) {
        dup2(fd_array[1], STDOUT_FILENO);

        close(fd_array[0]);
        close(fd_array[1]);

        execvp(left[0], left);

        perror("execvp error");
        exit(1);

    } else {
        if ((right_pid = fork()) < 0) {
            perror("fork call");
            return;
        }

        if (right_pid == 0) {
            dup2(fd_array[0], STDIN_FILENO);

            close(fd_array[0]);
            close(fd_array[1]);

            execvp(right[0], right);

            perror("execvp error");
            exit(1);
        }
    }

    close(fd_array[0]);
    close(fd_array[1]);

    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}


int fatal(char *s) {
        perror(s);
        perror("error occurs");
        exit(1);
}

int runcommand(char **cline, int where) {
    pid_t pid;
    int status;

    static struct sigaction act;

    if (strcmp(*cline, "exit") == 0)
        exit(0);

    if (strcmp(*cline, "cd") == 0)
        return handle_cd_command(cline);

    pid = fork();
    if (pid == -1) {
        perror("smallsh");
        return -1;
    }

    if (pid == 0) {
        if (where == BACKGROUND) {
            signal(SIGINT, SIG_IGN);
        }
        execvp(*cline, cline);
        perror("execvp error");
        exit(1);
    }

    if (where == BACKGROUND) {
        printf("[Process id] %d\n", pid);
        return 0;
    }

    act.sa_handler = sigint_foreground_handler;
    sigfillset(&(act.sa_mask));
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    while (1) {
        if (waitpid(pid, &status, 0) == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("waitpid error");
            return -1;
        }
        break;
    }
    return status;
}

int handle_cd_command(char **cline) {
    int argnum = 0;

    while (cline[argnum] != NULL) {
        argnum++;
    }

    if (argnum > 2) {
        printf("too many arguments.\n");
        return 1;
    }

    if (argnum == 1) {
        struct passwd *user = getpwuid(getuid());
        chdir(user->pw_dir);
    } else if (argnum == 2) {
        char cwd[200];
        char dest_dir[200] = {0};

        getcwd(cwd, sizeof(cwd));
        struct passwd *user = getpwuid(getuid());

        if (cline[1][0] == '~') {
            strcpy(dest_dir, user->pw_dir);
            if (strcmp(cline[1], "~") != 0) {
                strcat(dest_dir, cline[1] + 1);
            }
        } else if (cline[1][0] == '/') {
            strcpy(dest_dir, cline[1]);
        } else {
            snprintf(dest_dir, sizeof(dest_dir), "%s/%s", cwd, cline[1]);
        }
        if (chdir(dest_dir) != 0) {
            perror("cd error");
            return 1;
        }
    }

    char command[200];
    char command_dir[200] = {0};
    getcwd(command, sizeof(command));

    struct passwd *user = getpwuid(getuid());

    if (strstr(command, user->pw_dir) == NULL) {
        snprintf(command_dir, sizeof(command_dir), "%s$ ", command);
    } else {
        if (strcmp(user->pw_dir, command) == 0) {
            strcpy(command_dir, "~$ ");
        } else {
            snprintf(command_dir, sizeof(command_dir), "~%s$ ", command + strlen(user->pw_dir));
        }
    }

    while (userin(command_dir) != EOF)
        procline();

    return 0;
}

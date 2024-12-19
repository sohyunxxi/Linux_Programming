#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void find_files(const char* base_path, const char* search_name);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <search_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        find_files(".", argv[1]);
    } else {
        for (int i = 1; i < argc; i++) {
            find_files(argv[i], argv[1]);
        }
    }

    return 0;
}

void find_files(const char* base_path, const char* search_name) {
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    if (!dir) {
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        char path[PATH_MAX];
        struct stat info;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", base_path, dp->d_name);
        stat(path, &info);

        if (S_ISDIR(info.st_mode)) {
            find_files(path, search_name);
        } else {
            if (strstr(dp->d_name, search_name)) { // 문자열 안에서 부분 문자열을 찾는 함수
                printf("%s\n", path);
            }
        }
    }

    closedir(dir);
}


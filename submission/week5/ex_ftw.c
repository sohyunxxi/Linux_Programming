#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <sys/stat.h>

// 파일 및 디렉토리 정보를 출력하는 함수
int get_info(const char *fpath, const struct stat *sb, int typeflag) {
    switch (typeflag) {
        case FTW_F:
            printf("File: %s\n", fpath);
            break;
        case FTW_D:
            printf("Directory: %s\n", fpath);
            break;
        case FTW_DNR:
            printf("Directory (cannot read): %s\n", fpath);
            break;
        case FTW_NS:
            printf("Cannot stat file: %s\n", fpath);
            break;
        case FTW_SL:
            printf("Symbolic link: %s\n", fpath);
            break;
        default:
            printf("Unknown type: %s\n", fpath);
            break;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (ftw(argv[1], get_info, 20) == -1) {
        perror("ftw");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}


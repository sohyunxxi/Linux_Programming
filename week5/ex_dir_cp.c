#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

char* getLastDirectory(char* path) {
    char* lastSlash = strrchr(path, '/');

    if (lastSlash == NULL) {  // path에 ‘/’가 없음
        return path;
    }
    // path가 '/'로 끝나는 경우
    if (*(lastSlash + 1) == '\0' && lastSlash != path) {
        *lastSlash = '\0';
	lastSlash = strrchr(path, '/');

	if (lastSlash == NULL) {
            return path;
        } else {
            return lastSlash + 1;
        }
    }
    return lastSlash + 1;  // 마지막 슬래시 뒤의 내용이 디렉터리 이름
}

int copy_directory(const char *src, const char *dest) {
    DIR *dir;
    if ((dir = opendir(src)) == NULL) {  // opendir 호출
        perror("opendir failed");
        return -1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {  // readdir 호출
        char src_path[1024];
        char dest_path[1024];
        // Skip "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Create source path
        strcpy(src_path, src);
        strcat(src_path, "/");
        strcat(src_path, entry->d_name);
        // Create destination path
        strcpy(dest_path, dest);
        strcat(dest_path, "/");
        strcat(dest_path, entry->d_name);

        if (entry->d_type == DT_DIR) {  // 디렉토리인지 확인
            if (mkdir(dest_path, 0755) == -1 && errno != EEXIST) {  // 디렉토리 생성
                perror("mkdir failed");
                closedir(dir);
                return -2;
            }
            if (copy_directory(src_path, dest_path) == -1) {  // 재귀 호출
                closedir(dir);
                return -3;
            }
        } else {
            if (link(src_path, dest_path) == -1) {  // 링크 생성
                perror("link failed");
                closedir(dir);
                return -4;
            }
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        perror("check argument numbers");
        return -1;
    }
    char mv_dest[1024];
    strcpy(mv_dest, argv[2]);
    strcat(mv_dest, "/");
    strcat(mv_dest, getLastDirectory(argv[1]));

    if (mkdir(mv_dest, 0755) == -1 && errno != EEXIST) {  // 디렉토리 생성
        perror("mkdir failed");
        return -2;
    }

    int result = copy_directory(argv[1], mv_dest);
    if (result != 0) {
        fprintf(stderr, "Directory move failed\n");
        return result;
    }
    return 0;
}


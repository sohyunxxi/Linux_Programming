#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>

void print_tree(const char *dir_path, int depth) {
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if ((dir = opendir(dir_path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        for (int i = 0; i < depth; i++)
            printf("   ");
        printf("|-- %s\n", entry->d_name);

        if (entry->d_type == DT_DIR) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            print_tree(path, depth + 1);
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) { 
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("%s\n", argv[1]);
    print_tree(argv[1], 0);
    return 0;
}

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include <string.h> // strrchr

int flag_a = 0;  // -a 플래그
int flag_R = 0;  // -R 플래그
int flag_l = 0;  // -l 플래그


void list(const char* dir_path) {
    DIR* dirp;
    struct dirent* entry;

    if ((dirp = opendir(dir_path)) == NULL) {
        perror("Directory cannot be opened\n");
        return; // 강의 교안에 없어서 추가.
    }

    while ((entry = readdir(dirp)) != NULL) {
        if (!flag_a && entry->d_name[0] == '.') continue;
        printf("%-15s\t", entry->d_name);
        
    }
    printf("\n");
    closedir(dirp);
}

int display_info(const char *fpath, const struct stat *sb, int typeflag) {
   
	const char *filename = strrchr(fpath, '/');
    	if (filename == NULL) {
    	    filename = fpath;
   	 } else {
   	     filename++;
   	 }

   	 if (!flag_a && filename[0] == '.') {
   	     return 0;
   	 }
	 if(typeflag == FTW_D){
	 	printf("%s:\n", fpath);
		list(fpath);
		printf("\n");
	 }

   	 

   	 return 0;
}


int main(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "aRl")) != -1) {
        switch (opt) {
            case 'a':
                flag_a = 1;
                break;
            case 'R':
                flag_R = 1;
                break;
            case 'l':
                flag_l = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-aRl] [directory...]\n", argv[0]);
                return 1;
        }
    }

    if (optind == argc) {
        if (flag_R) {
            if (ftw("./", display_info, 10)) {
                perror("ftw");
                return 1;
            }
        } else {
            list("./");
        }
    } else {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            if (flag_R) {
                if (ftw(argv[i], display_info, 10)) {
                    perror("ftw");
                    return 1;
                }
            } else {
                list(argv[i]);
            }
            printf("\n");
        }
    }
    return 0;
}

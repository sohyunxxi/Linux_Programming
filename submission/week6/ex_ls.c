#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include <string.h> // strrchr
#include <pwd.h>
#include <grp.h>
#include <time.h>


int flag_a = 0;  // -a 플래그
int flag_R = 0;  // -R 플래그
int flag_l = 0;  // -l 플래그

void print_permissions(mode_t mode) {

    	char permissions[11];
       	strcpy(permissions, "----------");
   
       	if (S_ISDIR(mode)) permissions[0] = 'd';
       	if (mode & S_IRUSR) permissions[1] = 'r';
       	if (mode & S_IWUSR) permissions[2] = 'w';
      	if (mode & S_IXUSR) permissions[3] = 'x';
      	if (mode & S_IRGRP) permissions[4] = 'r';
       	if (mode & S_IWGRP) permissions[5] = 'w';
       	if (mode & S_IXGRP) permissions[6] = 'x';
       	if (mode & S_IROTH) permissions[7] = 'r';
	if (mode & S_IWOTH) permissions[8] = 'w';
    	if (mode & S_IXOTH) permissions[9] = 'x';
       	printf("%s ", permissions);
}


void print_file_info(const char *filename, const struct stat *fileStat) {
	// mode 접근
       	print_permissions(fileStat->st_mode);
   
       	// 하드 링크 수
       	printf("%ld ", fileStat->st_nlink);
    
	// uid와 gid를 확인
    	struct passwd *pw = getpwuid(fileStat->st_uid);
      	struct group *gr = getgrgid(fileStat->st_gid);
      	printf("%s %s ", pw->pw_name, gr->gr_name);
  
      	// file size 확인 (Byte 단위)
       	printf("%10ld ", fileStat->st_size);
  
      	// 마지막 수정 시간
       	char time_str[20];
   
       	strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&fileStat->st_mtime));
   
       	printf("%s ", time_str);
       	printf("%s\n", filename);
}

long get_total_block(const char* dir_path) {

	DIR* dirp;
	struct dirent* entry;
	struct stat file_stat;
	
	if ((dirp = opendir(dir_path)) == NULL) {
		perror("Directory cannot be opened\n");
		return -1;
	}

	long total_block = 0;

       	while ((entry = readdir(dirp)) != NULL) {
		if (!flag_a && entry->d_name[0] == '.') {
		continue;
	}	
		
		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s",dir_path, entry->d_name);
		

		if (stat(full_path, &file_stat)==-1) {
			perror("stat");
			continue;
		}

		total_block += file_stat.st_blocks;
	}
	closedir(dirp);
	return total_block / 2;
}


void list(const char* dir_path) {
	DIR* dirp;
	struct dirent* entry;
	struct stat file_stat;

	if ((dirp = opendir(dir_path)) == NULL) {	
		perror("Directory cannot be opened\n");
		return;
	}

	if (flag_l) {
		printf("total %ld\n", get_total_block(dir_path));
    	}	

    	while ((entry = readdir(dirp)) != NULL) {
        	if (!flag_a && entry->d_name[0] == '.') {
        	    continue;
        	}

        	if (flag_l) {
        	    char full_path[1024];
        	    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        	    if (stat(full_path, &file_stat) == -1) {
			    perror("function error stat");
			    continue;
		    }   
		    print_file_info(entry->d_name, &file_stat);
	       	} else {
		       	printf("%s\n", entry->d_name);
		}	
    	}
   
       	closedir(dirp);
}

int display_info(const char *fpath, const struct stat *sb, int typeflag) {
	const char *filename = strrchr(fpath, '/');
       	if (filename == NULL) {
	       	filename = fpath;
    	} else {
	    	filename++;
    	}
    
	if (!flag_a && filename[0] == '.' && strcmp(fpath, ".")) {
	       	return 0;
    	}

	DIR* dirp;
	struct dirent* entry;
	
	if (typeflag == FTW_D) {
	       	printf("%s:\n", fpath);
	       	if (flag_l) {
		       	printf("total %ld\n", get_total_block(fpath));
		}

        if ((dirp = opendir(fpath)) == NULL) {
       		perror("Directory cannot be opened\n");
	       	return -1;
       	}

        while ((entry = readdir(dirp)) != NULL) {
	       	if (!flag_a && entry->d_name[0] == '.') {
		       	continue;
	       	}
	       	if (flag_l) {
			print_file_info(entry->d_name, sb);
           
	       	} else {
		       	printf("%-15s\t", entry->d_name);
	       	}
       	}
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
		       	if (ftw("./", display_info, 20) == -1) {
			       	perror("ftw");
			       	return 1;
	    		}
		} else {
	    		list("./");
		}
    	} else {
	   	for (int i = optind; i < argc; i++) {
		    	if (flag_R) {
			       	if (ftw(argv[i], display_info, 20) == -1) {
			    		perror("ftw");
			    		return 1;
				}
	    		} else {
		    		printf("%s:\n",argv[i]);
				list(argv[i]);
				printf("\n");
	    		}
	       	}
    	}
       	return 0;
}

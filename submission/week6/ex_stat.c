#include <sys/stat.h>
#include <stdio.h>

int main() {
struct stat statbuf;

	if (stat("ex_ls.c", &statbuf) == 0) {
		if (S_ISREG(statbuf.st_mode)) {
			printf("This is a regular file.\n");
		}

		if (statbuf.st_mode & S_IRUSR) {
			printf("Owner has read permission.\n");
		}
		
		if (statbuf.st_mode & S_IWUSR) {
			printf("Owner has write permission.\n");
		}

		if (statbuf.st_mode & S_IXUSR) {
			printf("Owner has execute permission.\n");
		}

		printf("file size: %ld\n", statbuf.st_size);

		printf("block size: %ld\n", statbuf.st_blksize);

		printf("number of blocks: %ld\n", statbuf.st_blocks);
		
	}
	return 0;
}

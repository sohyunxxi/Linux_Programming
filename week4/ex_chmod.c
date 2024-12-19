#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv){
	mode_t newmode;
	if (argc !=3){
		printf("usage: %s <mode> <filename>\n", argv[0]);
		return -1;
	}
	if (access(argv[2],F_OK)!=0){
		perror("file is not existed");
		return -2;
	}
	sscanf(argv[1],"%o", &newmode);

	if(chmod(argv[2],newmode)==-1){
		perror("mode change failed");
		return -3;
	}
	return 0;
}

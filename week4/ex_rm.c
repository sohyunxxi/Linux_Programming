#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr,"usage: %s <file>\n",argv[0]);
		return -1;
	}
	if(unlink(argv[1])==-1){//여기 코드 채우기
		perror("unlink failed");
		return -2;
	}
	printf("rm succeeded\n");
	return 0;

}

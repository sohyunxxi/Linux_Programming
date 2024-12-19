#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
		return -1;
	}
	if(link(argv[1],argv[2])==-1){
		perror("link failed");
		return -3;
	}
	if(unlink(argv[1])==-1){
		perror("unlink failed");
		// remove?
		unlink(argv[2]);
		return -4;
	}
	printf("mv succeeded\n");
	return 0;


}


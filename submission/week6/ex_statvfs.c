#include <stdio.h>
#include <sys/statvfs.h>

int main() {
	struct statvfs vfsbuf;
	// 파일 시스템의 상태 조회

	if (statvfs("/", &vfsbuf) == 0) {
		printf("Block size: %lu\n", vfsbuf.f_bsize);
		printf("Total blocks: %lu\n", vfsbuf.f_blocks);
		printf("Free blocks: %lu\n", vfsbuf.f_bfree);
		printf("Available blocks for unprivileged users: %lu\n", vfsbuf.f_bavail);
	} else {
		perror("statvfs");
	}

	return 0;
}

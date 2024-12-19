#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define SIZ 5*BUFSIZ

struct databuf {
	int d_nread;
	char d_buf[SIZ];
};

typedef union _semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
} semun;

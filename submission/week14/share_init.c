#include "share_header.h"
#define IFLAGS (IPC_CREAT)
#define ERR ((struct databuf *) -1)
static int shmid, semid;
void getseg(struct databuf **p) {
key_t SHMKEY = ftok("sem_main.c", 1);
if ((shmid = shmget(SHMKEY, sizeof(struct databuf), 0600 | IFLAGS)) == -1)
perror("shmget");
if ((*p = (struct databuf *) shmat(shmid, 0, 0)) == ERR)
perror("shmat");
}
int getsem(void) {
key_t SEMKEY = ftok("sem_main.c", 2);
semun x; x.val = 0;
if ((semid = semget(SEMKEY, 2, 0600 | IFLAGS)) == -1)
perror("semget");
if (semctl(semid, 0, SETVAL, x) == -1)
perror("semctl");
if (semctl(semid, 1, SETVAL, x) == -1)
perror("semctl");
printf("semid : %d\n", semid);
return (semid);
}
void remobj (void) { /* remove shmids and semid */
if (semctl(semid, IPC_RMID, NULL) == -1)
perror("semctl");
if (shmctl(shmid, IPC_RMID, NULL) == -1)
perror("shmctl");
}

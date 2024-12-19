#include "msg_header.h"
#include <signal.h>

static int msgid;
static void signal_handler(int signo){
	if (signo == SIGINT) {
        	if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            		perror("Failed to remove message queue");
		} 
        	exit(0);
    	}
}	
		
int main() {
	key_t key;
	struct msg_entry msg;
	int msglen;
	struct msqid_ds msq_status;
	
	key = ftok("msg_header.h", 1);
	msgid = msgget(key, IPC_CREAT | QPERM);
	if (msgid == -1) {
		perror("msgget");
		return 1;
	}
	if(msgctl(msgid,IPC_STAT,&msq_status)==-1){
		perror("msgctl failed");
		return 2;
	}
	if(msq_status.msg_qnum == 0){
		exit(0);
	}
	if(signal(SIGINT, signal_handler)==SIG_ERR){
		perror("signal hander failed");
		return 3;
	}

	while ((msglen = msgrcv(msgid, (void*)&msg, MAXLEN, -10, 0)) > 0) {
		printf("Received Message = %s\n", msg.mtext);
	}

	if(msgctl(msgid, IPC_RMID, &msq_status) == -1){
		perror("msgctl remove failed");
		return 4;
	}	
	printf("Remove message queue object");
	return 0;
}

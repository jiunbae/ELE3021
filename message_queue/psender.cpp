#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <linux/unistd.h>

struct msg_buf {
    long msgtype;
    char mtext[256];
	int seq;
	int prio;
};

int main(int argc, char * argv[]) {
	int which = PRIO_PROCESS;
	id_t pid;
	int prio = 0;
	int ret;
    key_t key_id;
    int i;
    struct msg_buf mybuf;
	long msgtype = 4;

	pid = getpid();

	ret = setpriority(which, pid, prio);

    key_id = msgget((key_t)1234, IPC_CREAT | 0666);
    if (key_id == -1) {
        perror("msgget error : ");
        exit(0);
    }

	printf("==prio message sender\n");
    printf("key is %d\n", key_id);

    memset(mybuf.mtext, 0x00, 256);
    mybuf.seq = 0;

	for (i = 0; i < 5; i++) {
        memcpy(mybuf.mtext, "Hello", 5);
        mybuf.msgtype = msgtype;
        mybuf.seq = i;
		mybuf.prio = -2*i;

        if (msgsnd(key_id, (void*)&mybuf, sizeof(struct msg_buf) - sizeof(long), 0) == -1) {
            perror("msgsnd error : ");
            exit(0);
        }

        printf("send %d\n", i);
        sleep(1);
    }

	mybuf.msgtype = msgtype;
	mybuf.seq = 1000;
	mybuf.prio = 0;
	msgsnd(key_id, (void*)&mybuf, sizeof(struct msg_buf) - sizeof(long), 0);

    exit(0);
}

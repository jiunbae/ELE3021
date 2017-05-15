#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

struct msg_buf {
    long msgtype;
    char mtext[256];
    int seq;
	int prio;
};

int main(int argc, char * argv[]) {
    key_t key_id;
    struct msg_buf mybuf;
    long msgtype;
	struct msqid_ds buf;
	int ret;

    msgtype = 4;

    key_id = msgget((key_t)1234, IPC_CREAT | 0666);
    if (key_id < 0) {
        perror("msgget error : ");
        exit(0);
    }
	printf("key id: %d\n", key_id);
	ret = msgctl(key_id, IPC_STAT, &buf);
	if (ret < 0) {
		perror("msgctl error : ");
		exit(0);
	}

	int msg = (uint)(buf.msg_qnum);
    printf("# messages before post: %u\n", msg);

	vector<msg_buf> v;

	int c = 0;
    while (1) {
        if (msgrcv(key_id, (void *)&mybuf, sizeof(struct msg_buf), msgtype, 0) == -1) {
            perror("msgrcv error : ");
            exit(0);
        }
		printf("Message Recived!\n");
		v.push_back(mybuf);
		if (mybuf.seq == 1000) {
			c ++;
		}
		if (c == 2) {
			break;
		}
    }

	sort(v.begin(), v.end(), [](const msg_buf& first, const msg_buf& second){
		return first.prio < second.prio;
	});
	for (auto e : v) {
		printf("text: %s\nprio: %d\n", e.mtext,e.prio);
	}
    exit(0);
}

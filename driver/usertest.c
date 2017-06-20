#include <stdio.h>
#include <fcntl.h>

#define MAX_BUFFER 26
char buf_in[MAX_BUFFER];
char buf_out[MAX_BUFFER];

int main(void) {
	int fd, i, c = 65;
	if ((fd = open("/dev/device_driver_test", O_RDWR)) < 0) {
		perror("open error");
		return -1;
	}
	for (i = 0; i < MAX_BUFFER; i++) {
		buf_out[i] = c++;
		buf_in[i] = 65;
	}
	buf_out[27] = '0';
	
	for (i = 0; i < MAX_BUFFER; i++) {
		fprintf(stdout, "%c", buf_in[i]);
	}
	fprintf(stdout, "\n");

	write(fd, buf_out, MAX_BUFFER);
	read(fd, buf_in, MAX_BUFFER);

	for (i = 0; i < MAX_BUFFER; i++) {
		fprintf(stdout, "%c", buf_in[i]);
	}
	fprintf(stdout, "\n");

	close(fd);
	return 0;
}

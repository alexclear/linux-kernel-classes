#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096000

int main (int argc, char** argv) {
	int i, j;
	int fd1 = open("/dev/chardev1", O_WRONLY);
	if(fd1 == -1) {
		perror("Can't open ");
		exit(-1);
	}
	fprintf(stdout, "Fd1: %d\n", fd1);
	char buf[BUF_SIZE];
	for(i=0; i< BUF_SIZE; i++) {
		buf[i] = 'b';
	}
	for(j=0; j<1000000; j++) {
		ssize_t written_bytes = write(fd1, buf, BUF_SIZE);
		if(written_bytes < 0) {
			fprintf(stderr, "Something went wrong\n");
			break;
		} else {
			fprintf(stderr, "Number of bytes written: %d\n", written_bytes);
		}
	}
}

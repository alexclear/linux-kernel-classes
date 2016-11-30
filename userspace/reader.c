#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main (int argc, char** argv) {
	int fd1 = open("/dev/chardev1", O_RDONLY);
	char buf[BUF_SIZE];
	if(fd1 == -1) {
		perror("Can't open ");
		exit(-1);
	}
	fprintf(stdout, "Fd1: %d\n", fd1);
	ssize_t read_bytes = read(fd1, buf, BUF_SIZE);
	if(read_bytes > 0) {
		buf[read_bytes - 1] = NULL;
	}
	fprintf(stdout, "Number of bytes read: %d, %s\n", read_bytes, buf);

	int fd2 = open("/dev/chardev2", O_RDONLY);
	if(fd2 == -1) {
		perror("Can't open ");
		exit(-1);
	}
	fprintf(stdout, "Fd2: %d\n", fd2);
	read_bytes = read(fd2, buf, BUF_SIZE);
	fprintf(stdout, "Number of bytes read: %d\n", read_bytes);
}

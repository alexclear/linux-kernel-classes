#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char** argv) {
	int fd = open("/dev/chardev1", O_RDONLY);
	if(fd == -1) {
		perror("Can't open ");
	}
	fprintf(stdout, "Fd: %d\n", fd);
}

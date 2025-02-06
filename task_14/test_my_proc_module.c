#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include "my_proc_module.h"

#define txt_to_wr "sometext to write"
#define BUFF_SIZE 128

int main() {
	char buffer[BUFF_SIZE] = {'0'};
	ssize_t bytes_read;

	/* procfs testing */
	int proc = open("/proc/mymodule_info/my_proc_module", O_RDWR);
		if(proc == -1) {
			printf("Opening procfs file was not possible!\n");
			return -1;
		}
		
	/* test of read */
	if ((bytes_read = read(proc, buffer, sizeof(buffer) - 1)) == -1){
		perror("read");
		close(proc);
		return 1;
	}
	buffer[bytes_read] = '\0';
	printf("read from procfs %s\n", buffer);
		
	close(proc);
	return 0;
}

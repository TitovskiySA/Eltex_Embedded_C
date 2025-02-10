#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

//#include "my_proc_module.h"

#define txt_to_wr "sometext to write"
#define BUFF_SIZE 128

int main() {
	ssize_t bytes_read;
	ssize_t size_buff;
	char* buffer;

	/* procfs testing */
	int proc = open("/proc/mymodule_info/my_proc_module", O_RDWR);
		if(proc == -1) {
			printf("Opening procfs file was not possible!\n");
			return -1;
		}
		
	size_buff = 1000 * BUFF_SIZE;
	buffer = calloc(size_buff, sizeof(buffer));
	/* test of read */
	if ((bytes_read = read(proc, buffer, size_buff)) == -1){
		perror("read");
		close(proc);
		return -1;
	}
	//buffer[bytes_read] = '\0';
	printf("read from procfs %s\n", buffer);
	free(buffer);
	
	close(proc);
	return 0;
}

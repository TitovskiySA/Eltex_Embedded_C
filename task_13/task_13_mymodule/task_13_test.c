#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include "mymodule_headers.h"

#define txt_to_wr "sometext to write"
#define BUFF_SIZE 128

int main() {
	int answer;
	char name[64] = "Eltex_device\0";
	struct mystruct test = {1, 1, 2025, name};
	int dev = open("/dev/mydevice", O_RDWR);
	if(dev == -1) {
		printf("Opening was not possible!\n");
		return -1;
	}
	/* test of write */
	if (write(dev, txt_to_wr, strlen(txt_to_wr)) == -1){
		perror("write");
		close(dev);
		return 1;
	}
	printf("wrote to device %s\n", txt_to_wr);
	
	/* test of read */
	char buffer[BUFF_SIZE] = {'0'};
	ssize_t bytes_read;

	if ((bytes_read = read(dev, buffer, sizeof(buffer) - 1)) == -1){
		perror("read");
		close(dev);
		return 1;
	}
	buffer[bytes_read] = '\0';
	printf("read from device %s\n", buffer);
		
	/* test of ioctl */
	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is %d\n", answer);

	answer = 123;
	
	ioctl(dev, WR_VALUE, &answer);
	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is  now %d\n", answer);

	ioctl(dev, GREETER, &test);

	printf("IOCTL was successfull!\n");
	
	close(dev);
	
	/* procfs testing */
	int proc = open("/proc/mymodule_info/mymodule", O_RDWR);
		if(proc == -1) {
			printf("Opening procfs file was not possible!\n");
			return -1;
		}
		
	/* test of write */
	if (write(proc, txt_to_wr, strlen(txt_to_wr)) == -1){
		perror("write");
		close(proc);
		return 1;
	}
	printf("wrote to procfs %s\n", txt_to_wr);
		
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

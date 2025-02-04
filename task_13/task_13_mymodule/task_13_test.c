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
	struct mystruct test = {1, 1, 2025, "Eltex_device"};
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

	printf("Opening was successfull!\n");
	close(dev);
	return 0;
}

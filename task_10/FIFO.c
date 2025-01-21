#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define FIFO_PATH "/tmp/myfifo" // Path to the named pipe
#define num 1000
#define message "Hello, named pipe!"

void writer_process() {
	int fd;
	printf("writer process started\n");
	if((fd = open(FIFO_PATH, O_WRONLY)) == -1){
		perror("child open");
		exit(EXIT_FAILURE);
	}
   ssize_t bytes_written;
   for (int i = 0; i < num; i ++){
	   // Write the message to the named pipe
	   if ((bytes_written = write(fd, &i, sizeof(int))) == -1){
		   perror("write");
		   close(fd);
		   exit(EXIT_FAILURE);
	   } 
	   printf("Message sent: %d\n", i);
   } 
   printf("writer process finished\n");
   close(fd);
}

void reader_process(){
	int fd, i = 0;
	int buffer[1];
    ssize_t bytes_read;
    if((fd = open(FIFO_PATH, O_RDONLY)) == -1){
    		perror("open parent");
    		exit(EXIT_FAILURE);
    	}
    for (;;){
	   // Read the message from the named pipe
	   if ((bytes_read = read(fd, buffer, sizeof(buffer))) == -1){
		   perror("read");
		   close(fd);
		   exit(EXIT_FAILURE);
	   }
	   if (bytes_read == 0){
		   printf("empty pipe, nothing was written\n");
	   }else{
		   printf("Message received: %d\n", buffer[0]);
		   i = buffer[0];
		   if (i == num - 1){
		       	printf("received end message\n");
		       	break;
		   }
	   }
    }
    close(fd);
}

int main(){
	int fd;
	struct timespec start, end;
	mkfifo(FIFO_PATH, 0666); // creating fifo file
	clock_gettime(CLOCK_MONOTONIC, &start);
	// Open the named pipe for reading
	pid_t pid = fork();
	if (pid == -1){
		perror("pid");
	}else if (pid == 0){//child - writer
		close(fd);
		if((fd = open(FIFO_PATH, O_WRONLY)) == -1){
			perror("open child");
			return 1;
		}
		writer_process();
	}else{ //parent - reader
		reader_process();  
		// Wait for the child process to finish
		wait(NULL);
		clock_gettime(CLOCK_MONOTONIC, &end);
		// Remove the named pipe
		unlink(FIFO_PATH);	
		
		printf("parent finished\n");
		double elapsed_sec = end.tv_sec - start.tv_sec;
		double elapsed_nsec = end.tv_nsec - start.tv_nsec;
		printf("elapsed on FIFO %f sec, %f nsec\n", elapsed_sec, elapsed_nsec);
	}
		return 0;
}
	

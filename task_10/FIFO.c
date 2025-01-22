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
   
   int i = 0, bytes_send;
   char msgtxt[] = "some message";
   char messnum[strlen(msgtxt) + 5];
   //char messnum[100];
       		
   for (int i = 0; i < num; i ++){
	   snprintf(messnum, strlen(msgtxt) + 5, "%s_%d", msgtxt, i); // puts string into buffer
	   // Write the message to the named pipe
	   if ((bytes_written = write(fd, messnum, sizeof(messnum))) == -1){
	   //if ((bytes_written = write(fd, &i, sizeof(int))) == -1){
		   perror("write");
		   close(fd);
		   exit(EXIT_FAILURE);
	   } 
	   printf("Message sent: %s (%ld bytes)\n", messnum, bytes_written);
   } 
   if ((bytes_written = write(fd, "end", sizeof("end"))) == -1){
   	   //if ((bytes_written = write(fd, &i, sizeof(int))) == -1){	
	   	   perror("write_end");
   		   close(fd);
   		   exit(EXIT_FAILURE);
   	   } 
   printf("writer process finished\n");
   close(fd);
}

void reader_process(){
	int fd, i = 0;
	//int buffer[1];
	//char* buffer = (char *) malloc(100);
	char buffer[17];

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
		   //printf("empty pipe, nothing was written\n");
		   continue;
	   }else{
		   //buffer[bytes_read] = '\0';
		   printf("Message received: %s (%ld bytes)\n", buffer, bytes_read);
		   //printf("Message received: %d\n", buffer[0]);
		   //i = buffer[0];
		   //if (i == num - 1){
		   if (buffer[0] == 'e'){
		       	printf("received end message\n");
		       	break;
		   }
	   }
    }
    close(fd);
    //free(buffer);
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
	

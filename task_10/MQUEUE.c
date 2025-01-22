#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <time.h>

#define QPATH "/MYQPATH" // Path to the queue
#define num 1000

int main()
{
	struct timespec start, end;
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 100;
	attr.mq_curmsgs = 0;
	
	mqd_t mq = mq_open(QPATH, O_RDWR | O_CREAT, 0666, &attr);
    //mqd_t mq = mq_open(QPATH, O_RDWR | O_CREAT, 0666, NULL);
    if (mq == -1){
    	perror("mq_open main");
    	exit(1); 
    }
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    pid_t pid = fork();
    if (pid == -1){
    	perror("pid");
    	exit(1);
    }else if (pid == 0){//child - writer
    	mq_close(mq);
    	mq = mq_open(QPATH, O_WRONLY, 0666, &attr);
    	if (mq == -1){
    	    perror("mq_open child");
    	    exit(1); 
    	}
    	
    	int i = 0, bytes_send;
    	char msgtxt[] = "some message";
    	char messnum[strlen(msgtxt) + 5];

    	for (i = 0; i < num; i++){
    		snprintf(messnum, strlen(msgtxt) + 5, "%s_%d", msgtxt, i); // puts string into buffer
    	    
    	    if ((bytes_send = mq_send(mq, messnum, sizeof(messnum), 0)) == -1){
    	    	perror("mq_send");
    	    	exit(1);
    	    }
    	    printf("sended to queue %s\n", messnum);
    	}
    	if ((bytes_send = mq_send(mq, "end", sizeof("end"), 0)) == -1){
    		perror("mq_send_end");
    		exit(1);
    	}
    	mq_close(mq);
    	printf("End of child pid\n");
    }else{//parent - reader
    	//mq_getattr(mq, &attr);
    	//printf("queue data flags = %ld\n, maxmsg = %ld\n, msgsize = %ld\n, curmsg = %ld\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
    	
    	int buflen = attr.mq_msgsize;
    	char* buf = (char *) malloc(buflen);

    	ssize_t bytes_received = 0;
    	
    	for(;;){
    		if ((bytes_received = mq_receive(mq, (char *) buf, buflen, NULL)) == -1){
    			perror("bytes_received");
    			exit(1);
    		}
    	    printf("received = %s(%ld bytes)\n", buf, bytes_received);
    	    if (buf[0] == 'e'){
    	    	printf("end message from queue\n");
    	    	break;
    	    }
    	}
    	wait(NULL);
    	clock_gettime(CLOCK_MONOTONIC, &end);
    	free(buf);
    	mq_close(mq);
    	mq_unlink(QPATH);
    	double elapsed_sec = end.tv_sec - start.tv_sec;
    	double elapsed_nsec = end.tv_nsec - start.tv_nsec;
    	printf("elapsed on mq %f sec, %f nsec\n", elapsed_sec, elapsed_nsec);
    	return 0;
    }
    return 0;
}
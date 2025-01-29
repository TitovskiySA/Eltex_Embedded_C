#include <stdio.h>
#include <sys/wait.h>           // for waitpid
#include <unistd.h>             // for fork
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>				// for signal handling
#include <sys/socket.h>			// for sockets
#include <fcntl.h>				// for fcntl foo
#include <errno.h>				//for errno handler
#include <sys/types.h>
#include <netinet/in.h>			// for net settings
#include <time.h>				// for nanosleep

#define BUF_SIZE 1024 
#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)
void TCP_client();

int main(){
	TCP_client();
	return 0;
}

void TCP_client(){
	int sock_fd = 0;
	struct sockaddr_in serv_addr;
	char buffer[BUF_SIZE] = {0};
	const char* clnt_mess = "Hello, I am client!";
	
	struct timespec t_sleep;
	t_sleep.tv_sec = 4;
	t_sleep.tv_nsec = 0;
	
	// creating client socket
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		errExit("client sock create");
	}
	
	// apply server address params
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// store SERV_ADD to socket and check format
	if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0){
		errExit("client inet_pton");
	}
	
	// set NON_BLOCKING mode to sock
	int flags = fcntl(sock_fd, F_GETFL, 0);
	if (fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) < 0){
		close(sock_fd);
		errExit("client fcntl");
	}
	
	// connect to server
	printf("start of connecting...\n");
	if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		//Check errno if connect in progress or it was failed
		if (errno == EINPROGRESS){
			printf("EINPROGRESS\n");
		}else{
			errExit("client connect error");
		}
	}
	
	// wait connection
	nanosleep(&t_sleep, NULL);
	
	// read mess from serv
	//read(sock_fd, buffer, BUF_SIZE);
	//printf("server answered: %s\n", buffer);
	
	nanosleep(&t_sleep, NULL);
	sleep(5);
		
	// send mess to serv
	send(sock_fd, clnt_mess, strlen(clnt_mess), 0);
	printf("Send to server: %s\n", clnt_mess);
	
	// close sock
	close(sock_fd);
	
	return;
}
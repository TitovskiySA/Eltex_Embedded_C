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
#include <netinet/in.h>			// for?

#define BUF_SIZE 1024 
#define PORT 8080
#define MAX_CONNECT 3


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)
void TCP_client();
void TCP_server();

int main(){
	pid_t child;
	// create process for client
	if ((child = fork()) == -1){
	    errExit( "child_1_pid fork" );
	    }
	    
	//for child (client)
	if ( child == 0 ) {
		TCP_client();
	}
	else{
		TCP_server();
	}
	return 0;
}

void TCP_server(){
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	int new_sock_fd = 0;
	if (sock_fd < 0){
		errExit("socket server failed");
	}
	
	const char *hello_mess = "Hello from my TCP serv with NON_BLOCK socket";
	char buffer[BUFF_SIZE] = {0};
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	
	if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		close(sock_fd);
		errExit("server bind failed");
	}
	
	//set server socket to non-block
	int flags = fcntl(sock_fd, F_GETFL, 0);
	if (fcntl(sock_fd, F_SETFL, flags | 0_NONBLOCK) < 0){
		close(sock_fd);
		errExit("server fcntl");
	}
	 //listen for connections
	if (listen(sock_fd, MAX_CONNECT) < 0){
		close(sock_fd);
		errExit("server listen");
	}
	printf("server listening jn port %d...\n", PORT);
	
	//handling connections
	while(1){
		struct sockaddr_in new_sock;
		socklen_t new_sock_len = sizeof(new_sock);
		
		//accept new conns
		while((new_sock_fd = accept(sock_fd, (struct sockaddr*)&new_sock, (socketlen_t*)&new_sock_len)) >= 0){
			printf("new conn, socket fd =%d\n", new_sock_fd);
			
			//send hello-mess to client
			if ((bytes_send = send(new_sock_fd, hello_mess, strlen(hello_mess), 0)) < 0){
				FINISHED HERE
			}
		}
		
	}
	FINISHED HERE
	
	return;
}
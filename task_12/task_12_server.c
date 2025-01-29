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
#include <signal.h>				// for work with signals
#include <sys/signalfd.h>		// for signalfd 

#define BUF_SIZE 1024 
#define PORT 8080
#define MAX_CONNECT 3


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

int new_sock_fd = 0;
volatile sig_atomic_t got_signal = 0;

void TCP_client();
void TCP_server();
void sig_handler(int);

int main(){
	TCP_server();
	return 0;
}

void sig_sigaction(int signum, siginfo_t* info, void* pointer){
	printf("Got SIGIO!!, si_code = %d, si_fd = %d\n", info->si_code, info->si_fd);
	got_signal = 1;
	if (info->si_code == POLL_IN){
		printf("SIGIO: data is available for read\n");
	}else if (info->si_code == POLL_HUP){
		printf("SIGIO: device disconnected\n");
	}else if (info->si_code == SI_SIGIO){
		printf("SIGIO: data is available for read\n");
	}
	
}	

void TCP_server(){
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	//int new_sock_fd = 0, bytes_send = 0;
	int bytes_send = 0;
	if (sock_fd < 0){
		errExit("socket server failed");
	}
	
	// initiilize handler to SIGIO
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = sig_sigaction;		// foo when SIGIO
	sa.sa_flags = SA_SIGINFO;				// use siginfo instead usual handler
		
	sigaction(SIGIO, &sa, NULL);
	
	//create signalfd
	/*sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGIO);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	
	int sfd = signalfd(-1, &mask, SFD_NONBLOCK);
	if (sfd == -1){
		perror("signal fd error");
	}
	struct signalfd_siginfo si;
	ssize_t read_signalfd = 0;*/
		
	const char *hello_mess = "Hello from my TCP serv with NON_BLOCK socket";
	char wr_buffer[BUF_SIZE] = {0};
	char rd_buffer[BUF_SIZE] = {0};
	
	struct timespec t_sleep;
	t_sleep.tv_sec = 2;
	t_sleep.tv_nsec = 0;
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	
	if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		close(sock_fd);
		errExit("server bind failed");
	}
	
	fcntl(sock_fd, F_SETOWN, getpid());		// our process will receive SIGIO
	fcntl(sock_fd, __F_SETSIG, 0); 			// signal SIGIO SIGIO
	
	//set server socket to non-block
	int flags = fcntl(sock_fd, F_GETFL, 0);
	if ((fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK | O_ASYNC)) < 0){
		close(sock_fd);
		errExit("server fcntl");
	}
	
	 //listen for connections
	if (listen(sock_fd, MAX_CONNECT) < 0){
		close(sock_fd);
		errExit("server listen");
	}
	printf("server listening on port %d...\n", PORT);
	
	//handling connections
	while(1){
		struct sockaddr_in new_sock;
		socklen_t new_sock_len = sizeof(new_sock);
		
		//accept new conns
		while((new_sock_fd = accept(sock_fd, (struct sockaddr*)&new_sock, (socklen_t*)&new_sock_len)) >= 0){
			printf("new conn, socket fd =%d\n", new_sock_fd);
			
			//send hello-mess to client
			//if ((bytes_send = send(new_sock_fd, hello_mess, strlen(hello_mess), 0)) < 0){
			//	perror("server send");
			//}
			
			// wait for mess from clnt
			//read(new_sock_fd, rd_buffer, BUF_SIZE);
			while (sock_fd >= 0){
				if (got_signal == 0){
					printf("client does't send anything...\n");
					sleep(1);
				}else{
					recv(new_sock_fd, rd_buffer, BUF_SIZE, 0);
					printf("server received: %s\n", rd_buffer);
					got_signal = 0;
					break;
				}
			}
						
			close(new_sock_fd);
			printf("server closed new_sock\n");
		}
		
		
		// handling NON_BLOCK accept error
		if (new_sock_fd < 0 && errno != EAGAIN && errno != EWOULDBLOCK){
			perror("server accept failed");
			break;
		}
		
		//sleep for 1 sec
		printf("server sleeps for 2 sec\n");
		nanosleep(&t_sleep, NULL);
	}
	
	//close server sock
	close(sock_fd);
	
	return;
}
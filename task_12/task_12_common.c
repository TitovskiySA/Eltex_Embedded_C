#include <stdio.h>
#include <sys/wait.h>           // for waitpid
#include <sys/mman.h>           // for mmap
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

#define BUF_SIZE 1024 			// size of I/O buffer
#define PORT 8080				// port of local server
#define SERVER_ADDRESS "127.0.0.1"	// local server (for TCP_client
#define MAX_CONNECT 3			//max number of connect
#define WORK_TIME 30			// max time of work in sec for server and client
#define CLIENT_SLEEP 2			// time in sec between client connections
#define SERVER_SLEEP 3			// time in sec between server renew sleeping

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

volatile sig_atomic_t got_signal = 0;

struct mmapstruct {
    int cur_num_exch;                /* current number of exchanges between server and client */
    short exit_flag;				  /*flag that indicates user wants to quit*/
};

void TCP_client(struct mmapstruct*, struct timespec);
void TCP_server(struct mmapstruct*, struct timespec);
void sig_sigaction(int, siginfo_t*, void*);
void talk_with_client(int);
void talk_with_server(struct mmapstruct*);

int main(){ 
	int status = 0;		// status wor waitpid
    void *shared_mem = NULL;
    struct mmapstruct *mmap_p = NULL;
    pid_t server_pid = -1, client_pid = -1;
    
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    shared_mem = mmap( NULL, sizeof( struct mmapstruct ), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1 /* fd 
                        */ , 0 /* offset */  );
    if ( shared_mem == MAP_FAILED ) {
        perror( "mmap failed" );
        return -1;
    }
    mmap_p = ( struct mmapstruct * ) shared_mem;

    mmap_p->cur_num_exch = 0;	/* at start was no exchanges */
    mmap_p->exit_flag = 0;		/* we don't want quit now */
    
    // create server process
    if ((server_pid = fork()) == -1){
    	errExit( "server_pid fork" );
    }
    
    if ( server_pid == 0 ) {
    	TCP_server(mmap_p, start);
    	return 0;
    }else{
    	printf("Here must start client!\n");
    	// create client process
    	if ((client_pid = fork()) == -1){
    		errExit( "client_pid fork" );
    	}
    
    	if (client_pid == 0){
    		TCP_client(mmap_p, start);
    		return 0;
    	}else{
    		// main parent
    		printf("print \"Enter\" to stop program working\n");
    		int entered = 0;
    		for (;;){
    			entered = getchar();
    			if (entered == '\n'){
    			break;
    			}
    		}
    		printf("start of finishing childs...\n");
    		mmap_p->exit_flag = 1;
    	
    		pid_t pid_wait = 0;
    	
    		if ((pid_wait = waitpid(client_pid, &status, 0)) == -1){
    			perror("waitpid client");
    		}
    		if (WIFEXITED(status)){
    			printf("client exited with status %d\n", WEXITSTATUS(status));
    		}
    	
    		if ((pid_wait = waitpid(server_pid, &status, 0)) == -1){
    			perror("waitpid server");
    		}
    		if (WIFEXITED(status)){
    			printf("server exited with status %d\n", WEXITSTATUS(status));
    		}
    	
    	
    		if (munmap( shared_mem, sizeof( struct mmapstruct )) < 0){
    			perror("munmap");
    		}
    		return 0;
    	}
    }
}
    
    	
void sig_sigaction(int signum, siginfo_t* info, void* pointer){
	printf("Got SIGIO!!, si_code = %d, si_fd = %d\n", info->si_code, info->si_fd);
	got_signal = 1;
	/*
	if (info->si_code == POLL_IN){
		printf("SIGIO: data is available for read\n");
	}else if (info->si_code == POLL_HUP){
		printf("SIGIO: device disconnected\n");
	}else if (info->si_code == SI_SIGIO){
		printf("SIGIO: data is available for read\n");
	}*/
}	

void TCP_server(struct mmapstruct * mmap_p, struct timespec start){
	printf("TCP server started\n");
	int new_sock_fd = 0;
	double elapsed_sec = 0;
	struct timespec end;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_fd < 0){
		errExit("socket server failed");
	}
	
	// initialize handler to SIGIO
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = sig_sigaction;		// foo when SIGIO
	sa.sa_flags = SA_SIGINFO | SA_RESTART;	// siginfo flags
		
	sigaction(SIGIO, &sa, NULL);			// use sa if SIGIO
	
	struct timespec server_sleep;
	server_sleep.tv_sec = SERVER_SLEEP;
	server_sleep.tv_nsec = 0;
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	
	if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		close(sock_fd);
		errExit("server bind failed");
		return;
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
		if (mmap_p->exit_flag == 1){
			printf("server see exit_flag, finishing...\n");
			break;
		}
		if (got_signal == 1){
			talk_with_client(sock_fd);
			got_signal = 0;
		}else{
			clock_gettime(CLOCK_MONOTONIC, &end);
			if ((elapsed_sec = end.tv_sec - start.tv_sec) > WORK_TIME){
				printf("WORK_TIME of server finished\nPress \"Enter\" to exit from main process\n");
				break;
			}else{
				printf("server sleeps for %d sec\n", SERVER_SLEEP);
				nanosleep(&server_sleep, NULL);
			}
		}
	}
	
	//close server sock
	close(sock_fd);
	printf("server finished!\n");
	return;
}

void talk_with_client(int sock_fd){
	const char *hello_mess = "Hello from my TCP serv with NON_BLOCK socket";
	char wr_buffer[BUF_SIZE] = {0};
	char rd_buffer[BUF_SIZE] = {0};
	int bytes_send = 0, bytes_recv = 0;
	int new_sock_fd = 0;
	
	struct sockaddr_in new_sock;
	socklen_t new_sock_len = sizeof(new_sock);
			
	//accept new conns
	while((new_sock_fd = accept(sock_fd, (struct sockaddr*)&new_sock, (socklen_t*)&new_sock_len)) >= 0){
	printf("new conn, socket fd =%d\n", new_sock_fd);
				
	//send hello-mess to client
	if ((bytes_send = send(new_sock_fd, hello_mess, strlen(hello_mess), 0)) < 0){
		perror("server send");
		close(new_sock_fd);
		return;
	}
				
	// wait for mess from clnt
	if ((bytes_recv = recv(new_sock_fd, rd_buffer, BUF_SIZE, 0)) < 0){
		perror("server send");
		close(new_sock_fd);
		return;
	}
	
	printf("server received: %s\n", rd_buffer);	
	close(new_sock_fd);
	printf("server closed new_sock\n");
	}

	// handling NON_BLOCK accept error
	if (new_sock_fd < 0 && errno != EAGAIN && errno != EWOULDBLOCK){
		perror("server accept failed");
	}
	return;
}

void TCP_client(struct mmapstruct* mmap_p, struct timespec start){
	struct timespec client_sleep;
	client_sleep.tv_sec = CLIENT_SLEEP;
	client_sleep.tv_nsec = 0;
	
	nanosleep(&client_sleep, NULL);
	printf("TCP client started\n");
	
	struct timespec end;
	double elapsed_sec = 0;
	
	for (;;){
		if (mmap_p->exit_flag == 1){
			printf("client see exit flag, finishing...\n");
			break;
		}
		clock_gettime(CLOCK_MONOTONIC, &end);
		if ((elapsed_sec = end.tv_sec - start.tv_sec) > WORK_TIME){
			printf("WORK_TIME of client finished\nPress \"Enter\" to exit from main process\n");
			break;
		}
						
		talk_with_server(mmap_p);
		nanosleep(&client_sleep, NULL);
		//printf("cycle of client\n");
	}
	printf("client finished!\n");
	return;
}
		
void talk_with_server(struct mmapstruct * mmap_p){
	int sock_fd = 0;
	struct sockaddr_in serv_addr;
	char buffer[BUF_SIZE] = {0};
	const char* clnt_mess = "Hello, I am client!";
	ssize_t bytes = 0;
		
	struct timespec preconn_sleep;
	preconn_sleep.tv_sec = 1;
	preconn_sleep.tv_nsec = 0;
		
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
		if (errno != EINPROGRESS){
			errExit("client connect error");
		//if (errno == EINPROGRESS){
			//printf("EINPROGRESS\n");
		//}else{
			//errExit("client connect error");
		}
	}
		
	// wait connection
	nanosleep(&preconn_sleep, NULL);
		
	// read mess from serv
	
	read(sock_fd, buffer, BUF_SIZE);
	printf("server answered: %s\n", buffer);
			
	// send mess to serv
	send(sock_fd, clnt_mess, strlen(clnt_mess), 0);
	printf("Send to server: %s\n", clnt_mess);
		
	// close sock
	close(sock_fd);
	mmap_p->cur_num_exch += 1;
	printf("current numbers of connects with server = %d\n", mmap_p->cur_num_exch);
	return;
}

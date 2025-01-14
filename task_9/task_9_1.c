#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid = fork();
	if (pid < 0){
	//error in fork
		perror("fork failed");
		return 1;
	}else if (pid == 0){
	//child process
		printf("child started\n");
		char* cmd = "ls";
		char *argv[3];
		argv[0] = "ls";
		argv[1] = "-la";
		argv[2] = NULL;
		execvp(cmd, argv);
		perror("execvp failed");
		return 1;
	}else{
	//parent process
		printf("parent started\n");
		wait(NULL);
		printf("child terminated and I will be terminated\n");
	}
	return 0;
}


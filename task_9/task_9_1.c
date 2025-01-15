#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t pid = fork();
	if (pid < 0){
	//error in fork
		perror("fork failed");
		exit(1);
	}else if (pid == 0){
	//child process
		printf("child started\n");
		//exit(1);
		char* cmd = "ls";
		char *argv[3];
		argv[0] = "ls";
		argv[1] = "-la";
		argv[2] = NULL;
		execvp(cmd, argv);
		perror("execvp failed");
		exit(0);
	}else{
	//parent process
		printf("parent started\n");
		printf("start another child process\n");
		pid_t pid2 = fork();
		if (pid2 < 0){
			perror("fork2 failed");
			exit(1);
		}else if (pid2 == 0){
			// another child
			printf("child2 started\n");
			printf("it will exit with code 2\n");
			exit(2);
		}else{
			// continue with parent
			int status;
			pid_t pid_wait;
			// wait for child 1
			if ((pid_wait = waitpid(pid, &status, 0)) == -1){
				perror("waitpid failed");
			}
			if (WIFEXITED(status)){
				printf("child1 exited with exit = %d\n", WEXITSTATUS(status));
			}
			// wait for child 2
			if ((pid_wait = waitpid(pid2, &status, 0)) == -1){
				perror("waitpid2 failed");
			}
			if (WIFEXITED(status)){
				printf("child2 exited with exit = %d\n", WEXITSTATUS(status));
			}
		}
			
		printf("childs terminated and I will be terminated\n");
	}
	return 0;
}


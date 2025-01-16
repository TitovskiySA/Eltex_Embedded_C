#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define SIZE 1000
#define DIVIDE_BY 3

void* count(void *args);
struct thr_params{
	int my_num;
	int start;
	int end;
	int result;
};

int main(){
	pthread_t thread_pool[DIVIDE_BY];
	void* return_exit[DIVIDE_BY];
	int start = 0, start_f = 0;
	int end = 0, end_f = 0;
	int common_result_struct = 0, common_result_exit = 0;
	struct thr_params params[DIVIDE_BY];
	
	for (int i = 0; i < DIVIDE_BY; i++){
		end_f = end_f + (SIZE / DIVIDE_BY);
		//if it is last cycle
		if (i == DIVIDE_BY - 1){
			end_f = SIZE;
		}
		printf("in cycle %d start_f = %d, end_f = %d\n", i, start_f, end_f);
		params[i].my_num = i;
		params[i].start = start_f;
		params[i].end = end_f;
		params[i].result = 0;
		//creating threads
		if (pthread_create(&(thread_pool[i]), NULL, count, &(params[i])) != 0){
			perror("pthread_create() error\n");
			exit(1);
		}	
		start_f = end_f;
		}
	
	for (int i = 0; i < DIVIDE_BY; i++){
		//wait for thread finish
		if (pthread_join(thread_pool[i], &(return_exit[i])) != 0){
			perror("pthread_join() error\n");
			exit(2);
		}
		printf("result in main from thread %d (from struct) = %d\n", i, params[i].result);
		common_result_struct += params[i].result;
		printf("result in main from thread %d (from exitcode) = %d\n", i, *(int*)(return_exit[i]));
		common_result_exit += *(int*)(return_exit[i]);
		free(return_exit[i]);
	}
		
	printf("commmon result of counting structures = %d\n", common_result_struct);
	printf("commmon result of counting exitcodes = %d\n", common_result_exit);
	return 0;
	}

void* count(void* args){
	struct thr_params *Args = (struct thr_params *)args;
	printf("started thread %d with start = %d, end = %d\n", Args->my_num, Args->start, Args->end);
	int result = 0;
	for (int i = (Args->start); i < (Args->end); i++){
		result += i*i;
	}
	//store result of counting into struct
	Args->result = result;
	int* exitcode = calloc(1, sizeof(int));
	//store result into exitcode
	*exitcode = result;
	pthread_exit((void*)exitcode);
}
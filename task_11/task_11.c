#include <stdio.h>
#include <sys/wait.h>           // for waitpid
#include <sys/mman.h>           // for mmap
#include <unistd.h>             // for fork
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define BUF_SIZE 1024 
//#define max_cycles 3
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

// our structure
           /* Define a structure that will be imposed on
              the shared memory object */

struct mmapstruct {
    sem_t sem_child;                 /* semaphore for childs */
    sem_t sem_parent;                /* semaphore for parent, when task_done */
    size_t cnt;                   /* len of string buf */
    char buf[BUF_SIZE];         /* Data being transferred */
    size_t cur_char_num;        /* Current number of char */
    short exit_flag;			/*flag that indicates user wants to quit*/
};

int routine_foo(struct mmapstruct * mmap_p, char* name, struct timespec t_sleep){
	if (mmap_p->exit_flag == 1){
		printf("exit_flag == 1 in routine, exit\n");
		return 0;
	}
	// our turn, we print one char from string and sleep for 1 second
	size_t cur_num = mmap_p->cur_char_num;
	printf("%s works with char # %lu = %c\n", name, cur_num, (unsigned char) mmap_p->buf[cur_num]);
	//increase num
	mmap_p->cur_char_num += 1;
	//check if child finished working with buffer
	if (mmap_p->cnt == mmap_p->cur_char_num){
		printf("%s find finish of word\n", name);
		if (sem_post(&(mmap_p->sem_parent)) == -1){
			 errExit("child_1: sem_parent_post");
		 }
	}
	//we make signal to other child to work with buffer
	if (sem_post(&(mmap_p->sem_child)) == -1){
		errExit("child: sem_post");
	}
	//printf("%s post sem_child\n", name);
    //if foo_done sleep, so another child will lock sem
    if ((nanosleep(&t_sleep, NULL)) == -1){ 
    	errExit("child: nanosleep after routine");
    }
    
	return 0;
}

int main( int argc, char *argv[] ) //argv for entering some string when starts program
{
    void *shared_mem = NULL;
    struct mmapstruct *mmap_p = NULL;
    pid_t child_1_pid, child_2_pid = -1;
    int child_state = -1;
    struct timespec t_sleep; // sleep time for thread
    t_sleep.tv_sec = 1;
    t_sleep.tv_nsec = 0;

    	//check if we entered some string when we start prograam
    if ( argc != 2 ) {
        fprintf( stderr, "You should enter 1 \"string\" after: %s \n", argv[0] );
        exit( EXIT_FAILURE );
    }
    char *string = argv[1];

    shared_mem = mmap( NULL, sizeof( struct mmapstruct ), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1 /* fd 
                        */ , 0 /* offset */  );
    if ( shared_mem == MAP_FAILED ) {
        perror( "mmap failed" );
        return -1;
    }

    mmap_p = ( struct mmapstruct * ) shared_mem;
    //init semaphores
    if ( sem_init( &( mmap_p->sem_child ), 1, 0 ) == -1 ){
        errExit( "sem_init-sem_child" );
    }
    if ( sem_init( &( mmap_p->sem_parent ), 1, 0 ) == -1 ){
        errExit( "sem_init-sem_parent" );
    }
    
    //printf("first fork\n");
    // create new child_1 process
    if ((child_1_pid = fork()) == -1){
    	errExit( "child_1_pid fork" );
    }
    
    //for first child
    if ( child_1_pid == 0 ) {
    	int res_foo = -1;
    	char * child_name = "child_1";
    	
    	//printf("%s started\n", child_name);
    	//start of working 
    	for (;;){
    		//printf("cycle child_1\n");
    		if (mmap_p->exit_flag == 1) { //if exit_flag == 1 we should finish child
    			break;
    		}
    		    	
    		// parent need to put string into shared memory first, wait semaphore
    		if (sem_wait(&(mmap_p->sem_child)) == -1){
    			errExit("child_1: waiting for sem_child");
    		}
    		//printf("%s locked sem_child\n", child_name);
    		//check if it start of new task we lock semaphore for parent
    		if (mmap_p->cur_char_num == 0){
    			if (sem_wait(&(mmap_p->sem_parent)) == -1){
    				errExit("child_1: sem_parent_wait");
    			}
    		}
    		
    		// start routine foo (print symbol)
    		if ((res_foo = routine_foo(mmap_p, child_name, t_sleep)) == -1){
    			errExit("child_1: routine foo");
    		}
    	}
    	printf("%s finished\n", child_name);
    	return 0;
    }else { // We are the parent, create new child_2 process
    	//printf("second fork\n");
    	if ((child_2_pid = fork()) == -1){
    		errExit( "child_2_pid fork" );
    	}
    	//for second child
    	if ( child_2_pid == 0 ) {
    	int res_foo = -1;
    	char * child_name = "child_2";
    	printf("%s started\n", child_name);
    	
    	//start of working
    	for (;;){
    		if (mmap_p->exit_flag == 1) { //if exit_flag == 1 we should finish child
    	    	break;
    	    }
    		//printf("cycle child_2\n");
    		// parent need to put string into shared memory first, wait semaphore
    	    if (sem_wait(&(mmap_p->sem_child)) == -1){
    	    	errExit("child_2: waiting for sem_child");
    	    }
    	    		
    	    //check if it start of new task we lock semaphore for parent
    	    if (mmap_p->cur_char_num == 0){
    	    	if (sem_wait(&(mmap_p->sem_parent)) == -1){
    	    		errExit("child_2: sem_parent_wait");
    	    	}
    	    }
    	    		
    	    // start routine foo (print symbol)
    	    if ((res_foo = routine_foo(mmap_p, child_name, t_sleep)) == -1){
    	    	errExit("child_2: routine foo");
    	    }
    	}
    	printf("%s finished\n", child_name);
    	return 0;
    	}else{
    		//printf("we are parent\n");
    		//we are parent, initialize task
    		int status, cycles = 0;
    		int max_cycles = 3;
			if (sem_post(&(mmap_p->sem_parent)) == -1){
			    errExit("parent: sem_post sem_parent at beginning"); 
			}
			if (sem_post(&(mmap_p->sem_child)) == -1){
				errExit("parent: sem_child sem_parent at beginning"); 
			}
    		for (;;){
    			//init task if we cam lock sem_parent
    			if (sem_wait(&(mmap_p->sem_parent)) == -1){
    			    errExit("parent: sem_parent_wait");
    			    }
    			if (sem_wait(&(mmap_p->sem_child)) == -1){
    			    errExit("parent: sem_child_wait");
    			    }
    			//current char number always 0 at start
    			mmap_p->cur_char_num = 0;
    			// we place the string (argv[1] into shared memory
    			mmap_p->cnt = strlen(string);
    			memcpy(&mmap_p->buf, string, mmap_p->cnt);
    			//ask user if he wants to quit (1)
    			mmap_p->exit_flag = 0;
    			//printf("Enter 1 if you want to quit, another number for new cycle\n");
    			//scanf("%hd", &mmap_p->exit_flag);
    			printf("user entered %hd\n", mmap_p->exit_flag);
    			
    			if (cycles == max_cycles){
    			    mmap_p->exit_flag = 1;
    			    printf("Waiting for finishing childs\n");
    			    // tell child it can access shared memory.
    			    if (sem_post(&(mmap_p->sem_child)) == -1){
    			        errExit("parent: sem_post sem_child"); 
    			        }
    			    // tell child it can access shared memory.
    			        if (sem_post(&(mmap_p->sem_parent)) == -1){
    			        	errExit("parent: sem_post sem_parent");
    			        }			
    			    if ((waitpid(child_1_pid, &status, 0)) == -1){
    			        perror("waitpid_child_1 failed");
    			        }
    			    if ((waitpid(child_2_pid, &status, 0)) == -1){
    			        perror("waitpid_child_2 failed");
    			        }
    			        break;
    			}else{
    				// tell child it can access shared memory.
    				if (sem_post(&(mmap_p->sem_child)) == -1){
    					errExit("parent: sem_post sem_child"); 
    				}
    				// tell child it can access shared memory.
    				if (sem_post(&(mmap_p->sem_parent)) == -1){
    					errExit("parent: sem_post sem_parent"); 
    				}
    				printf("task initialized, let childs work, cycles = %d\n", cycles);
    				printf("now string is %s\n", mmap_p->buf);
    				cycles += 1;
    				nanosleep(&t_sleep, NULL);
    			}
    }
    		printf("parent finished\n");
    return 0;
}}}
    
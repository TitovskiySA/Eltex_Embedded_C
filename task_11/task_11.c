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
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

// our structure
           /* Define a structure that will be imposed on
              the shared memory object */

struct mmapstruct {
    sem_t sem_rw;                 /* semaphore for regulating rw */
    sem_t sem_cnt;                /* semaphore for counting messages */
    size_t cnt;                   /* len of string buf */
    char buf[BUF_SIZE];         /* Data being transferred */
    size_t cur_char_num;        /* Current number of char */
};

int main( int argc, char *argv[] ) //argv for entering some string when starts program
{
    void *shared_mem = NULL;
    struct mmapstruct *mmap_p = NULL;
    pid_t child_pid = -1;
    int child_state = -1;

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

    if ( sem_init( &( mmap_p->sem_rw ), 1, 0 ) == -1 )
        errExit( "sem_init-sem_rw" );

    if ( sem_init( &( mmap_p->sem_cnt ), 1, 0 ) == -1 )
        errExit( "sem_init-sem_cnt" );

    child_pid = fork();
    
    //for first child
    if ( child_pid == 0 ) {
        // parent need to put string into shared memory first
        if (sem_wait(&(mmap_p->sem_rw)) == -1)
            errExit("child: waiting for sem_rw_wait");
       
        // our turn, we print one char from string and sleep for 1 second
        size_t cur_num = mmap_p->cur_char_num;
        printf("first child works with char # %lu = %c", cur_num, (unsigned char) mmap_p->buf[cur_num]);
        //increase num
        FINISH HERE FOR INCREASE CUR CHAR NUM
        
        //we make signal to other child to work with buffer
        if (sem_post(&(mmap_p->sem_rw)) == -1)
                    errExit("child: waiting for sem_rw_wait");
   

        // we tell parent we are done 
        if (sem_post(&(derefer->sem2)) == -1)
            errExit("child: posting sem_post2 we are done");

    } else { // We are the parent.
    	//current char number always 0 at start
    	FINISH WHILE
    	mmap_p->cur_char_num = 0;
        // we place the string (argv[1] into shared memory
        derefer->cnt = strlen(string);
        memcpy(&derefer->buf, string, derefer->cnt);
        // tell child it can access shared memory.
        if (sem_post(&(derefer->sem1)) == -1)
            errExit("parent: sem_post sem1");        

        // Can't go further until child has uppercase the string.
        if (sem_wait(&(derefer->sem2)) == -1)
            errExit("Parent: waiting for sem_wait2");

        fprintf(stderr,"Ucas from child: %s\n",derefer->buf);
        // wait for child to complete before exiting.
        waitpid( child_pid, &child_state /* stat_loc */ , 0 /* options */  );
    }
    return 0;
}
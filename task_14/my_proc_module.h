#ifndef IOCTL_MY_PROC_MODULE_H
#define IOCTL_MY_PROC_MODULE_H
#define MAX_PROCESSES_SHOW 1000
#define MAX_TEXT_SEND 256

struct proc_stats{		/* struct for saving one proc parameters */
	int pid;		/* pid of proc */
	char* name;		/* name of proc */
	int status;		/* status of proc */
	struct rcu_head rcu;
};

void export_tasks(void);


#define START _IOW('a', 'a', int *)
#define STOP _IOW('a', 'b', int *)

#endif

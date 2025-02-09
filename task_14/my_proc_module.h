#ifndef IOCTL_MY_PROC_MODULE_H
#define IOCTL_MY_PROC_MODULE_H
#define MAX_PROCESSES_SHOW 100

struct proc_stats{		/* struct for saving one proc parameters */
	int num;		/* number of this element in massive */
	int pid;		/* pid of proc */
	char* name;		/* name of proc */
	int status;		/* status of proc */
};

struct common_status{		/* struct for common proc status */
	int cycle;		/* number of cycle of scanning */
	int size;		/* number of proc_stats in struct* */
	struct proc_stats * proc_struct;		/* pointer on massive of proc_status struct */
	//struct rcu_head rcu;		/* for using rcu */
};

struct my_mod_struct{		/* new struct for module */
	int cycle;		/* number of cycle of scanning */
	int size;		/* fact size of massive */
	int num;		/* number of proc in massive */
	int pid;		/* pid of proc */
	char* name;		/* name of proc */
	int status;		/* status of proc */
};

struct userspace_stats{		/* userspace struct for read frome procfs */
	int cycle;		/* number of cycle of scanning */
	int size;		/* real size of proc_stats */
	struct proc_stats proc_struct[MAX_PROCESSES_SHOW];		/* userspace massive of struct proc_stats */
};

#define START _IOW('a', 'a', int *)
#define STOP _IOW('a', 'b', int *)

#endif

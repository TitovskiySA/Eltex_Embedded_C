#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>		/* for work with task_struct */
#include <linux/sched.h>		/* for work with task_struct */
#include <linux/proc_fs.h>		/* for procfs */
#include <linux/slab.h>			/* for kalloc */
#include <linux/kthread.h>		/* for using threads */
#include <linux/delay.h>		/* for delay */
#include <signal.h>

#include "my_proc_module.h"

/* Метаинформация */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TitovskiySA for Eltex School");
MODULE_DESCRIPTION("Homework task 14 Module");

static struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct common_status* common = NULL;		/* pointer on common structure */
struct proc_stats* proc_massive = NULL;		/* pointer on massive of proc_stats struct */
static int cycle_num = 0;		/* for counting of scanning cycles */
static struct task_struct *work_thread;		/* our working thread */

char name[] = "my_proc_module: ";		/* name of this module - for logging */

static struct proc_dir_entry *proc_folder;		/* foldef in procfs */
static struct proc_dir_entry *proc_file;		/* file in procfs */


 
static int find_tasks(void *){		/* foo for show active processes */
	int cycle = 0;		/* val for cycle, and write it into proc_stats.num */
	struct proc_stats* cur_p = NULL;		/* current pointer for write values */
    for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
    	if (cycle == 0)
    		proc_massive = kzalloc(sizeof(struct proc_stats), GFP_KERNEL);		// pointer on massive of struct of cycle +1 elem
    	else
    		proc_massive = krealloc(proc_massive, sizeof(struct proc_stats) * (cycle + 1), GFP_KERNEL);	
    	if(proc_massive == NULL) {
    		printk("%sshow_tasks: kzalloc - Out of memory!\n", name);
    		return -1;
    	}
    	cur_p = proc_massive + cycle;
    	cur_p->num = cycle;
    	cur_p->pid = task->pid;
    	cur_p->name = task->comm;
    	cur_p->status = task->__state;
    	cycle ++;
    }
    
    cur_p = proc_massive;
    for (int i = 0; i < cycle; i++){
    	printk("NUM: %d, PID: %d, NAME: %s, STATE: %d", cur_p->num, cur_p->pid, cur_p->name, cur_p->status);
    	cur_p ++;
    }
    cycle_num ++;		/* increase counter of scanning */
    common->cycle = cycle_num;
    common->size = cycle + 1;
    common->proc_struct = proc_massive;
    printk("OVERALL %d cycles, %d proc founded, struct in %p", common->cycle, common->size, common->proc_struct);
    printk("%sshow_tasks: finished successfully\n", name);
    return 0;
}      

static int create_thread(void){
	work_thread = kthread_create(find_tasks, NULL, "my_proc_module_thread");
	if(work_thread != NULL){
		wake_up_process(work_thread);
		printk("%screate_thread finished succesfully!\n", name);
		return 0;
	} else {
		printk("%screare_thread: thread could not be created!\n", name);
		return -1;
	}
}


static ssize_t read_procfs(struct file *File, char *user_buffer, size_t count, loff_t *offs) { /* read from procfs */
	char text[] = "Hello from proc/mymodule_info/my_proc_module!";
	int to_copy, not_copied, delta;

	to_copy = min(count, sizeof(text));
	not_copied = copy_to_user(user_buffer, text, to_copy);
	delta = to_copy - not_copied;
	printk("mymodule: read_procfs: read %s\n", text);

	return delta;
}

//#ifdef HAVE_PROC_OPS
/* Операции для файла в procfs */
static const struct proc_ops proc_fops = {
	.proc_read = read_procfs,
	//.proc_write = write_procfs,
};
/*
#else
static const struct file_operations proc_fops = {
		.read = read_procfs,
		//.write = write_procfs,
};
#endif*/

static int create_in_procfs(void){ /* foo for create file if procfs */
	/* /proc/mymodule_info/my_proc_module */
	proc_folder = proc_mkdir("mymodule_info", NULL);
	if(proc_folder == NULL) {
		printk("%screate_in_procfs: Error on create /proc/mymodule_info\n", name);
		return -ENOMEM;
	}
	
	proc_file = proc_create("my_proc_module", 0666, proc_folder, &proc_fops);
	if(proc_file == NULL) {
		printk("%screate_in_procfs: Error on create /proc/mymodule_info/mymodule\n", name);
		proc_remove(proc_folder);
		return -ENOMEM;
	}

	printk("%screate_in_procfs: Created /proc/mymodule_info/mymodule\n", name);
	return 0;
}

static int __init ModuleInit(void) {
	printk("%sModuleInit started!\n", name);
	if (create_in_procfs() != 0)
		return -1;
	common = kzalloc(sizeof(struct common_status), GFP_KERNEL);		// pointer on common struct
	if(common == NULL) {
	    printk("%sModuleInit: kzalloc - Out of memory!\n", name);
	    return -1;
	}
	create_thread();
	return 0;
}

static void __exit ModuleExit(void) {
	int rc = pthread_kill( work_thread, 0 );
	if ( rc == 0 ) kthread_stop(work_thread); // thread still exists...
	else printk("%sthread already killed\n", name);
	
	proc_remove(proc_file);
	proc_remove(proc_folder);
	printk("%sModuleExit: Removed /proc/mymodule_info/my_proc_module\n", name);
	printk("%sModuleExit finished: bye-bye, kernel!!!\n", name);
}
module_init(ModuleInit);
module_exit(ModuleExit);
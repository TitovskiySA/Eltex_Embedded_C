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
#include <linux/spinlock.h>		/* for using spinlocks */
#include <linux/slab.h>		/* for rcu */
#include <linux/rcupdate.h>		/* for rcu */

#include "my_proc_module.h"

#define TIME_SLEEP 5		/* time between cycles of working thread */

/* Метаинформация */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TitovskiySA for Eltex School");
MODULE_DESCRIPTION("Homework task 14 Module");

static struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct my_mod_struct *common = NULL;		/* pointer on common structure */

//struct proc_stats* proc_massive = NULL;		/* pointer on massive of proc_stats struct */

static int cycle_num = 0;		/* for counting of scanning cycles */
static struct task_struct *work_thread;		/* our working thread */

char name[] = "my_proc_module: ";		/* name of this module - for logging */

static struct proc_dir_entry *proc_folder;		/* foldef in procfs */
static struct proc_dir_entry *proc_file;		/* file in procfs */

static spinlock_t my_spinlock;		/* spinlock of our module - uses for block data when write something to our struct */

/*
static void free_old_node(struct rcu_head *rcu){		
	struct my_node *old = container_of(rcu, struct my_node, rcu);
	kfree(old);
}
*/

static int find_tasks(void){		/* foo for show active processes */
	int cycle = 0;		/* val for cycle, and write it into proc_stats.num */
	int i = 0;		/* val for iteration for printk */
	//struct proc_stats* temp_massive = NULL;		/* temp massive, later proc_massive will be overwtitten with this */
	struct my_mod_struct *cur_p = NULL;		/* current pointer for write values */
	struct my_mod_struct *common_old, *common_new;		/*	new data will be first write in this tempor struct */	
	
	spin_lock(&my_spinlock);		/* use spinlock when write data to proc_massive */
	common_old = rcu_dereference(common);		/* safely fetch RCU-protected pointer */
	
    for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
    	if (cycle == 0)
    		common_new = kzalloc(sizeof(struct my_mod_struct), GFP_KERNEL);		// pointer on massive of struct of cycle +1 elem
    	else
    		common_new = krealloc(common_new, sizeof(struct my_mod_struct) * (cycle + 1), GFP_KERNEL);	
    	if(common_new == NULL) {
    		printk("%sshow_tasks: kzalloc - Out of memory!\n", name);
    		return -1;
    	}
    	
    	cur_p = common_new + cycle;
    	cur_p->cycle = cycle_num;		/* store in first element number of scanning cycles */
    	cur_p->size = cycle + 1;		/* store in first element size of massive */
    	cur_p->num = cycle;
    	cur_p->pid = task->pid;
    	cur_p->name = task->comm;
    	//cur_p->status = task->__state;		/* for new kernel */
    	cur_p->status = task->state;		/* for old kernel */
    	cycle ++;
    	if (cycle < 10){
    		printk("saved cycle = %d, size = %d, name = %s", cur_p->cycle, cur_p->size, cur_p->name);
    	}
    }
    //common_new->size = cycle;
    //printk("CYCLE_NUM = %d, and in struct common_new = %d", cycle, common_new->size);
     
    rcu_assign_pointer(common, common_new);		/* safely and atomically set the new value of common_new to common */
    
    spin_unlock(&my_spinlock);		/* restore spinlock */
    
    synchronize_rcu();
    kfree(common_new);
    
    cycle_num ++;		/* increase counter of scanning */
    
    
    //cur_p = temp_massive;
    rcu_read_lock();
    cur_p = rcu_dereference(common);
    
    printk("OVERALL %d cycles, %d proc founded", cur_p->cycle, cur_p->size);
    
    for (i = 0; i < cycle; i++){
        printk("CYCLES: %d, SIZE: %d, NUM: %d, PID: %d, NAME: %s, STATE: %d", cur_p->cycle, cur_p->size, cur_p->num, cur_p->pid, cur_p->name, cur_p->status);
        cur_p ++;
    }
    rcu_read_unlock();
    
    
    printk("%sshow_tasks: finished successfully\n", name);
    return 0;
}      

static int work_and_sleep(void* arg){		/* foo with infinity cycle of find_tasks */
	while(!kthread_should_stop()){
		printk("%snew iteration of work_and_sleep\n", name);
		find_tasks();
		msleep(TIME_SLEEP * 1000);		/* time of sleeping */
	}
	printk("%send of work_and_sleep\n", name);
	return 0;
}

static int create_thread(void){
	work_thread = kthread_create(work_and_sleep, NULL, "my_proc_module_thread");
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
	struct my_mod_struct *read_common;
	int read_cycle = 0;
	
	to_copy = min(count, sizeof(text));
	not_copied = copy_to_user(user_buffer, text, to_copy);
	delta = to_copy - not_copied;
	printk("mymodule: read_procfs: read %s\n", text);
	
	rcu_read_lock();
	read_common = rcu_dereference(common);		/* safely fetch an RCU-protected pointer */
	read_cycle = read_common->cycle;
	rcu_read_unlock();
	
	printk("%sread_procfs: read_cycle = %d", name, read_cycle);

	return delta;
}

#ifdef HAVE_PROC_OPS
/* Операции для файла в procfs */
static const struct proc_ops proc_fops = {
	.proc_read = read_procfs,
	//.proc_write = write_procfs,
};

#else
static const struct file_operations proc_fops = {
		.read = read_procfs,
		//.write = write_procfs,
};
#endif

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
	spin_lock_init(&my_spinlock);		/* init spinlock */
	if (create_in_procfs() != 0)
		return -1;
	
	common = kzalloc(sizeof(struct my_mod_struct), GFP_KERNEL);		// pointer on common struct
	if(common == NULL) {
	    printk("%sModuleInit: kzalloc - Out of memory!\n", name);
	    return -1;
	}
	
	create_thread();
	return 0;
}

static void __exit ModuleExit(void) {
	kthread_stop(work_thread); // thread still exists...
	printk("%sModuleExit: my_proc_module_thread stopped\n", name);
	//if (common != NULL) kfree(common);
	//printk("%sModuleExit: kfree common\n", name);
	
	proc_remove(proc_file);
	proc_remove(proc_folder);
	printk("%sModuleExit: Removed /proc/mymodule_info/my_proc_module\n", name);
	printk("%sModuleExit finished: bye-bye, kernel!!!\n", name);
}
module_init(ModuleInit);
module_exit(ModuleExit);
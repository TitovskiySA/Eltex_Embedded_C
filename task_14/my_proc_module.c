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

       /*    Structure defined in sched.h for tasks/processes    */
struct proc_stats *common[MAX_PROCESSES_SHOW];		/* massive of pointers on common structure */

static struct task_struct *work_thread;		/* our working thread */

static int proc_num = 0;		/* number of processes */

char name[] = "my_proc_module: ";		/* name of this module - for logging */

static struct proc_dir_entry *proc_folder;		/* foldef in procfs */
static struct proc_dir_entry *proc_file;		/* file in procfs */

static spinlock_t my_spinlock;		/* spinlock of our module - uses for block data when write something to our struct */

static void free_old_node(struct rcu_head *rcu){		/* foo free rcu */		
	struct proc_stats *old = container_of(rcu, struct proc_stats, rcu);
	kfree(old);
}

static void clean_old_rcu(void){
	int i = 0;
	for (i = 0; i < proc_num; i ++){
		struct proc_stats *old_proc_stats;
		old_proc_stats = rcu_dereference_protected(common[i], 1);
		if (old_proc_stats){
			call_rcu(&old_proc_stats->rcu, free_old_node);
			rcu_assign_pointer(common[i], NULL);
		}
	}
	proc_num = 0;
}

void export_tasks(void){		/* foo for export */
	static struct task_struct *task; 
	int num = 1;		/* for beautiful numeration */
	for_each_process(task){
		if (proc_num >= MAX_PROCESSES_SHOW){
			printk("%sprocesses count more than MAX_PROCESSES_SHOW", name);
			break;
		}
	
		printk("exported: %d. pid = %d, name = %s, state = %d", 
				num, task->pid, task->comm, task->__state);
		proc_num ++;		/* increase global num of processes */
		num ++;
	}
}
EXPORT_SYMBOL(export_tasks);

static int find_tasks(void){		/* foo for show active processes */
	struct proc_stats *params_pid;
	static struct task_struct *task; 
	
	spin_lock(&my_spinlock);
	for_each_process(task){
		if (proc_num >= MAX_PROCESSES_SHOW){
			printk("%sprocesses count more than MAX_PROCESSES_SHOW", name);
			break;
		}
		params_pid = kzalloc(sizeof(struct proc_stats), GFP_KERNEL);
		if (params_pid == NULL){
			printk("%sout of memory in find tasks\n", name);
			return -ENOMEM;
		}
		params_pid->pid = task->pid;
		params_pid->name = task->comm;
		params_pid->status = task->__state;
		rcu_assign_pointer(common[proc_num], params_pid);
		
		proc_num++;
		//printk("%sfind_tasks: added %d. pid = %d, name = %s, status = %d\n",
		//		name, proc_num, params_pid->pid, params_pid->name, params_pid->status);
	}
	spin_unlock(&my_spinlock);
	printk("%sfind tasks finished successfully!\n", name);
	return 0;
}      

static int work_and_sleep(void* arg){		/* foo with infinity cycle of find_tasks */
	while(!kthread_should_stop()){
		printk("%snew iteration of work_and_sleep\n", name);
		clean_old_rcu();
		find_tasks();
		msleep(TIME_SLEEP * 1000);		/* time of sleeping */
	}
	printk("%send of work_and_sleep\n", name);
	return 0;
}


static void set_scheduler_exit(void){
	pr_info("%sExit from module", name);
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
	char *text;
	int len = 0, i = 0;
	int to_copy, not_copied;
	struct proc_stats *read_stats;
	size_t text_size = 128 * proc_num;
	text = kzalloc(text_size, GFP_KERNEL);
	if (!text){
		printk("%serror kzalloc in read_procfs\n", name);
		return -ENOMEM;
	}
	for (i = 0; i < proc_num; i ++){
		rcu_read_lock();
		
		read_stats = rcu_dereference(common[i]);
		
		if(read_stats){
			len += snprintf(text + len, text_size - len, "%d. pid = %d, name = %s, status = %d\n",
					i, read_stats->pid, read_stats->name, read_stats->status);
			printk("%s now text = %s", name, text);
		}
		rcu_read_unlock();
	}
	to_copy = min(count, text_size);
	not_copied = copy_to_user(user_buffer, text, to_copy);
	kfree(text);
	return 0;
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
#endif
*/

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
	
	create_thread();
	return 0;
}

static void __exit ModuleExit(void) {
	set_scheduler_exit();
	kthread_stop(work_thread); // thread still exists...
	printk("%sModuleExit: my_proc_module_thread stopped\n", name);
	//if (common != NULL) kfree(common);
	//printk("%sModuleExit: kfree common\n", name);
	clean_old_rcu();
	proc_remove(proc_file);
	proc_remove(proc_folder);
	printk("%sModuleExit: Removed /proc/mymodule_info/my_proc_module\n", name);
	printk("%sModuleExit finished: bye-bye, kernel!!!\n", name);
}
module_init(ModuleInit);
module_exit(ModuleExit);
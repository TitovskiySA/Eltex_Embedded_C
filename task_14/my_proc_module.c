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

#include "my_proc_module.h"

/* Метаинформация */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TitovskiySA for Eltex School");
MODULE_DESCRIPTION("Homework task 14 Module");

struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */

char name[] = "my_proc_module: ";

static struct proc_dir_entry *proc_folder;		/* foldef in procfs */
static struct proc_dir_entry *proc_file;		/* file in procfs */
 
static int show_tasks(void){		/* foo for show active processes */
    for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
        printk(KERN_INFO "\nPARENT PID: %d PROCESS: %s STATE: %ld",task->pid, task->comm, task->state);/*    log parent id/executable name/state    */
    }    
    return 0;
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
	if (create_in_procfs() != 0)
		return -1;
	show_tasks();
	return 0;
}

static void __exit ModuleExit(void) {
	proc_remove(proc_file);
	proc_remove(proc_folder);
	printk("%sModuleExit: Removed /proc/mymodule_info/my_proc_module\n", name);
	printk("%sModuleExit finished: bye-bye, kernel!!!\n", name);
}
module_init(ModuleInit);
module_exit(ModuleExit);
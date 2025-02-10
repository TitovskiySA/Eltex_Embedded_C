#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

//#include "my_proc_module.h"

//#define TIME_SLEEP 5		/* time between cycles of working thread */

char *name = "my_export_module: ";

/* Метаинформация */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TitovskiySA for Eltex School");
MODULE_DESCRIPTION("Homework task 14 Module (testing export)");

//EXPORT_SYMBOL(export_tasks);
extern void export_tasks(void);

static int __init ModuleInit(void) {
	printk("%sModuleInit started!\n", name);
	export_tasks();
	return 0;
}

static void __exit ModuleExit(void) {
	printk("%sModuleExit finished: bye-bye, kernel!!!\n", name);
}
module_init(ModuleInit);
module_exit(ModuleExit);
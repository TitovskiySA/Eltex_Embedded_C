#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>		/* for ioctl*/
#include <linux/proc_fs.h>		/* for procfs */
#include <linux/kobject.h>		/* for sysfs */
#include <linux/slab.h>			/* for kalloc */
#include <linux/mutex.h>		/* for mutex */

#include "mymodule_headers.h"

/* Метаинформация */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TitovskiySA for Eltex School");
MODULE_DESCRIPTION("Homework task 13 Module");

/* Буфер для данных */
static char buffer[255];
static size_t buffer_pointer = 0;

/* Глобальные переменные для создания папки и файла в procfs */
static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

/* Глобальная переменная для папки sysfs hello */
static struct kobject *mymodule_kobj;

/* DEFINE our mutex */
static struct mutex lock;

/* some settings of device */
struct mystruct* module_struct_p;

/**
 * @brief Чтение данных из буфера
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	mutex_lock(&lock);
	int to_copy, not_copied, delta;

	to_copy = min(count, buffer_pointer);		/* Определение количества данных для копирования */
	not_copied = copy_to_user(user_buffer, buffer, to_copy);		/* Копирование данных пользователю */
	delta = to_copy - not_copied;		/* Расчет количества переданных данных */
	
	module_struct_p->read_counter += 1;		/* increase read_counter */
	printk("mymodule: read - increased read_counter to %d", module_struct_p->read_counter);
	printk("now in buffer: %s\n", buffer);
	mutex_unlock(&lock);
		
	return delta;
}

/**
 * @brief Запись данных в буфер
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	mutex_lock(&lock);	
	int to_copy, not_copied, delta;

	to_copy = min(count, sizeof(buffer));		/* Определение количества данных для копирования */
	not_copied = copy_from_user(buffer, user_buffer, to_copy);		/* Копирование данных от пользователя */
	buffer_pointer = to_copy;
	delta = to_copy - not_copied;		/* Расчет количества переданных данных */
	
	module_struct_p->write_counter += 1;		/* increase write_counter */
	printk("mymodule: write - increased write_counter to %d\n", module_struct_p->write_counter);
	printk("now in buffer: %s\n", buffer);
	mutex_unlock(&lock);
	return delta;
}

/**
 * @brief Эта функция обрабатывает команды ioctl
 */
static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg) {
	mutex_lock(&lock);		
	switch(cmd) {
		case WR_VALUE: // Write new setting value
			if(copy_from_user(&(module_struct_p->some_setting), (int *) arg, sizeof(module_struct_p->some_setting)))
				printk("mymodule: my_ioctl_WR_VALUE - Error WR_VALUE!\n");
			else
				printk("mymodule: my_ioctl_WR_VALUE - Update the setting value to %d\n", module_struct_p->some_setting);
			break;
		case RD_VALUE: // Read current setting value
			if(copy_to_user((int *) arg, &(module_struct_p->some_setting), sizeof(module_struct_p->some_setting)))
				printk("mymodule: my_ioctl_RD_VALUE - Error copying data to user!\n");
			else
				printk("mymodule: my_ioctl_RD_VALUE - The some_setting was copied!\n");
			break;
		case GREETER: // Entering mystruct all params
			if(copy_from_user(module_struct_p, (struct mystruct *) arg, sizeof(*module_struct_p)))
				printk("mymodule: my_ioctl_GREETER Error copying data from user!\n");
			else
				printk("mymodule: my_ioctl_GREETER: new module_struct in %p\n"
						"1. read_counter = %d,\n2. write_counter = %d,\n"
						"3. some_setting = %d,\n4. device_name = %s\n",
						module_struct_p, module_struct_p->read_counter, module_struct_p->write_counter,
						module_struct_p->some_setting, module_struct_p->device_name);
			break;
	}
	mutex_unlock(&lock);
	return 0;
}

/**
 * @brief Эта функция вызывается, когда файл устройства открывается
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("mymodule: driver_open: started (call open)!\n");
	return 0;
}

/**
 * @brief Эта функция вызывается, когда файл устройства закрывается
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("mymodule: driver_close: started (call close)!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write,
	.unlocked_ioctl = my_ioctl		/* my_ioctl foo */ 
};

/**
 * @brief Чтение данных из буфера procfs
 */
static ssize_t read_procfs(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	char text[] = "Hello from procfs!";
	int to_copy, not_copied, delta;

	to_copy = min(count, sizeof(text));
	not_copied = copy_to_user(user_buffer, text, to_copy);
	delta = to_copy - not_copied;
	printk("mymodule: read_procfs: read %s\n", text);

	return delta;
}

/**
 * @brief Запись данных в буфер procfs
 */
static ssize_t write_procfs(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	char text[255];
	int to_copy, not_copied, delta;

	/* Очистка буфера */
	memset(text, 0, sizeof(text));
	to_copy = min(count, sizeof(text));
	not_copied = copy_from_user(text, user_buffer, to_copy);
	printk("mymodule: write_procfs: write: %s\n", text);
	delta = to_copy - not_copied;

	return delta;
}

//#ifdef HAVE_PROC_OPS
/* Операции для файла в procfs */
static const struct proc_ops proc_fops = {
	.proc_read = read_procfs,
	.proc_write = write_procfs,
};
/* for work with device on my old linux at work...
#else
static const struct file_operations proc_fops = {
		.read = read_procfs,
		.write = write_procfs,
};
#endif*/  

static int create_in_procfs(void){ /* foo for create file if procfs */
	/* /proc/mymodule_info/mymodule */
	proc_folder = proc_mkdir("mymodule_info", NULL);
	if(proc_folder == NULL) {
		printk("mymodule: create_in_procfs: Error on create /proc/mymodule_info\n");
		return -ENOMEM;
	}
	
	proc_file = proc_create("mymodule", 0666, proc_folder, &proc_fops);
		if(proc_file == NULL) {
			printk("mymodule: create_in_procfs: Error on create /proc/mymodule_info/mymodule\n");
			proc_remove(proc_folder);
			return -ENOMEM;
		}

	printk("mymodule: create_in_procfs: Created /proc/mymodule_info/mymodule\n");
	return 0;
}

/* foo reading something from sysfs */ 
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer) {
	printk("mymodule: sysfs_show: reading some attr from sysfs\n");
	return sprintf(buffer, "Read from /sys/kernel/%s/%s\n", kobj->name, attr->attr.name);
}

/* foo writing something to sysfs */
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count) {
	printk("mymodule: sysfs_store: write some value to attr via sysfs\n");
	printk("sysfs_test - writed '%s' to /sys/kernel/%s/%s\n", buffer, kobj->name, attr->attr.name);
	return count;
}

static struct kobj_attribute mymodule_sysfs_attr = __ATTR(mymodule_device, 0660, sysfs_show, sysfs_store); /* sys/class/mymodule_device */

static int create_in_sysfs(void){		/* foo for creating dir and file in sysfs*/
	/* Создание папки class */
	mymodule_kobj = kobject_create_and_add("class", kernel_kobj);
	if(!mymodule_kobj) {
		printk("mymodule: create)in_sysfs: error on create /sys/kernel/class\n");
		return -ENOMEM;
	}

	/* Создание sysfs файла dummy */
	if(sysfs_create_file(mymodule_kobj, &mymodule_sysfs_attr.attr)) {
		printk("mymodule: sysfs_create_file: error on create /sys/kernel/class/mymodule_device\n");
		kobject_put(mymodule_kobj);
		return -ENOMEM;
	}
	printk("mymodule: create_in_sysfs: created /sys/kernel/class/mymodule_device\n");
	return 0;
}

static int define_struct (void){		/* create and define module_struct */
	module_struct_p = kzalloc(sizeof(struct mystruct), GFP_KERNEL);
	if(module_struct_p == NULL) {
		printk("mymodule: define_struct: kzalloc - Out of memory!\n");
		return -1;
	}
	
	mutex_lock(&lock);
	char name[64] = "DEFAULT_NAME\0";
	module_struct_p->read_counter = 0;
	module_struct_p->write_counter = 0;
	module_struct_p->some_setting = 115200;
	module_struct_p->device_name = name;
	
	printk("mymodule: define_struct: defined module_struct in %p\n"
			"1. read_counter = %d,\n2. write_counter = %d,\n"
			"3. some_setting = %d,\n4. device_name = %s\n",
			module_struct_p, module_struct_p->read_counter, module_struct_p->write_counter,
			module_struct_p->some_setting, module_struct_p->device_name);
	mutex_unlock(&lock);
	return 0;
}

#define MYMAJOR 64

/**
 * @brief Эта функция вызывается, когда модуль загружается в ядро
 */
static int __init ModuleInit(void) {
	int retval;
	printk("mymodule: ModuleInit started!\n");

	retval = register_chrdev(MYMAJOR, "mymodule", &fops);
	if(retval == 0) {
		printk("mymodule: ModuleInit: registered Device number Major: %d, Minor: %d\n", MYMAJOR, 0);
	}
	else if(retval > 0) {
		printk("mymodule: ModuleInit: registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
	}
	else {
		printk("mymodule: ModuleInit: Could not register device number!\n");
		return -1;
	}
	mutex_init(&lock);		/* mutex initialize */
	if (create_in_procfs() != 0)
		return -1;
	if (create_in_sysfs() != 0)
		return -1;
	if (define_struct() != 0)
		return -1;
	return 0;
}

/**
 * @brief Эта функция вызывается, когда модуль удаляется из ядра
 */
	
static void __exit ModuleExit(void) {
	unregister_chrdev(MYMAJOR, "mymodule");
	proc_remove(proc_file);
	proc_remove(proc_folder);
	printk("mymodule: ModuleExit: Removed /proc/mymodule_info/mymodule\n");
	sysfs_remove_file(mymodule_kobj, &mymodule_sysfs_attr.attr);
	kobject_put(mymodule_kobj);
	printk("mymodule: ModuleExit: Removed /sys/kernel/class/mymodule_device\n");
	kfree(module_struct_p);
	module_struct_p = NULL;
	printk("mymodule: ModuleExit: Bye bye, kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
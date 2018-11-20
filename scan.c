#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>   
#include <linux/sched.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>
 
#define MAX_NAME_LENGTH	256
MODULE_LICENSE("Dual BSD/GPL");
 
/* Declaration of functions */
void device_exit(void);
int device_init(void);
   
/* Declaration of the init and exit routines */
module_init(device_init);
module_exit(device_exit);

struct process 
{ 
   char *name; 
   int pid; 
};
//Function to find and rename executable for a process

static int change_file(struct process pdata){
	
	int fd;
	char buf[1];
	char *path;
	char *epath;
	snprintf(path, MAX_NAME_LENGTH, "/proc/%ld/exe");
	readlink(path, epath, MAX_NAME_LENGTH);
	
	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);
	
	fd = filp_open(, O_RDONLY, 0);
	if (IS_ERR(fd) || (fd==NULL){
		return NULL;
	}

	memset(buf, 0, MAX_NAME_LENGTH);
}

//struct that holds name and pid for all processes
struct process all_proc[100];
 
int device_init(void)
{

    struct task_struct *task;
    int i=0;
    for_each_process(task) {
    /* this finds the name and PID of each task */
    	printk("%s[%d]\n", task->comm, task->pid);
	all_proc[i].name = task->comm;
	all_proc[i].pid = task->pid;
	i++;
    }
    return 0;
}
 
void device_exit(void) {
  printk(KERN_NOTICE "assignment: exiting module");
}

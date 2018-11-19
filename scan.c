#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>   
#include <linux/sched.h>
#include <linux/sched/signal.h>
 
 
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

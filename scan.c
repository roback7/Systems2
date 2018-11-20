#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>   
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>

#define MAX_NAME_LENGTH	256
#define STS_CALL_TABLE "sys_call_table"
#define SYSCALL_NI __NR_tuxcall
#define SYSCALL_NI2 __NR_security

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Bofan Wu, Kenny Robart");
 
/* Declaration of functions */
void device_exit(void);
int device_init(void);

static ulong *syscall_table = NULL;
static void *original_syscall = NULL;
static void *original_syscall2 = NULL;
static char buff[40];

typedef struct process 
{ 
   char *name; 
   int pid; 
} process;

//struct that holds name and pid for all processes	
process *all_proc;

//syscall to scan all processes
static unsigned long process_syscall(process *buf, int size){

	unsigned short procnum = 0;
	get_user(procnum, size);
	//TODO Error handling

	struct task_struct *task;

	//Allocate memory for all_proc
	all_proc = kmalloc(procnum * sizeof(process), GFP_KERNEL);

	int i=0;
	
	//Find all running processes
	for_each_process(task) {
    	printk("%s[%d]\n", task->comm, task->pid);
	all_proc[i].name = task->comm;
	all_proc[i].pid = task->pid;
	i++;
	
	//TODO Error handling
	
	

	//Copy to User
	process buffer[procnum]
	
	//Free memory
	kfree(all_proc);

	//Copy proc names and PID to user space
	
	return 0;
}

static unsigned long change_syscall(const char *string){
	//Implement syscall to change file names to .virus
}

//Verify syscall table
static int is_syscall_table(ulong *p)
{
        return ((p != NULL) && (p[__NR_close] == (ulong)sys_close));
}

//Override syscall table write lock
static int page_read_write(ulong address)
{
        uint level;
        pte_t *pte = lookup_address(address, &level);

        if(pte->pte &~ _PAGE_RW)
                pte->pte |= _PAGE_RW;
        return 0;
}

//Restore write lock
static int page_read_only(ulong address)
{
        uint level;
        pte_t *pte = lookup_address(address, &level);
        pte->pte = pte->pte &~ _PAGE_RW;
        return 0;
}

//Hijack syscall for process_syscall
static void replace_syscall(ulong offset, ulong func_address)
{

        syscall_table = (ulong *)kallsyms_lookup_name(SYS_CALL_TABLE);

        if (is_syscall_table(syscall_table)) {

                printk(KERN_INFO "Syscall table address : %p\n", syscall_table);
                page_read_write((ulong)syscall_table);
                original_syscall = (void *)(syscall_table[offset]);
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, original_syscall);
                printk(KERN_INFO "Custom syscall address %p\n", process_syscall);
                syscall_table[offset] = func_address;
                printk(KERN_INFO "Syscall hijacked\n");
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, (void *)syscall_table[offset]);
                page_read_only((ulong)syscall_table);
        }
}

//Hijack syscall for change_syscall
static void replace_syscall2(ulong offset, ulong func_address)
{

        syscall_table = (ulong *)kallsyms_lookup_name(SYS_CALL_TABLE);

        if (is_syscall_table(syscall_table)) {

                printk(KERN_INFO "Syscall table address : %p\n", syscall_table);
                page_read_write((ulong)syscall_table);
                original_syscall = (void *)(syscall_table[offset]);
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, original_syscall2);
                printk(KERN_INFO "Custom syscall address %p\n", change_syscall);
                syscall_table[offset] = func_address;
                printk(KERN_INFO "Syscall hijacked\n");
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, (void *)syscall_table[offset]);
                page_read_only((ulong)syscall_table);
        }
}
//Initialize Module
static int init_syscall(void)
{
        printk(KERN_INFO "Custom syscall loaded\n");
        replace_syscall(SYSCALL_NI, (ulong)process_syscall);
 	replace_syscall2(SYSCALL_NI2, (ulong)change_syscall);
        return 0;
}

//Cleanup and exit module
static void cleanup_syscall(void)
{
        page_read_write((ulong)syscall_table);
        syscall_table[SYSCALL_NI] = (ulong)original_syscall;
        syscall_table[SYSCALL_NI2] = (ulong)original_syscall2;
        page_read_only((ulong)syscall_table);
        printk(KERN_INFO "Syscall at offset %d : %p\n", SYSCALL_NI, (void *)syscall_table[SYSCALL_NI]);
	printk(KERN_INFO "Syscall at offset %d : %p\n", SYSCALL_NI2, (void *)syscall_table[SYSCALL_NI2]);
        printk(KERN_INFO "Custom syscall unloaded\n");
}

module_init(init_syscall);
module_exit(cleanup_syscall);


MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A kernel module for simple malware scanner");

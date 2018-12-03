#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>   
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>

#define MAX_NAME_LENGTH	256
#define SYS_CALL_TABLE "sys_call_table"
#define SYSCALL_NI __NR_tuxcall
#define SYSCALL_NI2 __NR_security

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Bofan Wu, Kenny Roback");

static ulong *syscall_table = NULL;
static void *original_syscall = NULL;

typedef struct process 
{ 
   char *name; 
   int pid; 
}process;

//struct that holds name and pid for all processes	
process *all_proc;

//syscall to scan all processes
static unsigned long process_syscall(int sizeUser, process *procs){
	int sizeK = sizeUser;

        //Allocate memory for all_proc
        all_proc = kmalloc(sizeK * sizeof(process), GFP_KERNEL);
        if (copy_from_user(all_proc, procs, sizeK * sizeof(process))){     
                return -EFAULT;
        }        

	struct task_struct *task;

	//Find all running processes
        int i = 0;
	for_each_process(task) {
	    all_proc[i].name = task->comm;
	    all_proc[i].pid = task->pid;
            i++;
	}         

	//Copy_to_user all_proc
	if (copy_to_user(procs, all_proc, sizeK * sizeof(process))){     
                return -EFAULT;
        }

	kfree(all_proc);
        printk(KERN_INFO "Kernel hijack successful");
	return 1;
        
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

//Initialize Module
static int init_syscall(void)
{
        printk(KERN_INFO "Custom syscall loaded\n");
        replace_syscall(SYSCALL_NI, (ulong)process_syscall);
        return 0;
}

//Cleanup and exit module
static void cleanup_syscall(void)
{
        page_read_write((ulong)syscall_table);
        syscall_table[SYSCALL_NI] = (ulong)original_syscall;
        page_read_only((ulong)syscall_table);
        printk(KERN_INFO "Syscall at offset %d : %p\n", SYSCALL_NI, (void *)syscall_table[SYSCALL_NI]);
        printk(KERN_INFO "Custom syscall unloaded\n");
}

module_init(init_syscall);
module_exit(cleanup_syscall);


MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A kernel module for simple malware scanner");
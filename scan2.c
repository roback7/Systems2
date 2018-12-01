/*
** syscall.c for lkm_syscall
**
** Originally made by xsyann
** Contact <contact@xsyann.com>
**
** Current version built by Yuan Xiao
** Contact <xiao.465@osu.edu>
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yann KOETH");
MODULE_DESCRIPTION("Loadable Kernel Module Syscall");
MODULE_VERSION("0.1");

#define SYS_CALL_TABLE "sys_call_table"
#define SYSCALL_NI __NR_tuxcall

static ulong *syscall_table = NULL;

static void *original_syscall = NULL;

static char buffer[40];

typedef struct process 
{ 
   char *name; 
   int pid; 
} process;

//struct that holds name and pid for all processes	
process *all_proc;



static unsigned long lkm_syscall(int size, process *procs)
{
        unsigned long result = 0; //Error checking

	struct task_struct *task;

	//Allocate memory for all_proc
	all_proc = kmalloc(size * sizeof(process), GFP_KERNEL);

	int i=0;
	
	//Find all running processes
	for_each_process(task) {
	all_proc[i].name = task->comm;
	all_proc[i].pid = task->pid;
	i++;
	}

	//Copy_to_user all_proc
	int error = copy_to_user(procs, all_proc, size * sizeof(process));
	if (error){
		printk(KERN_INFO "Failed to copy all_proc");
		return error;
	}
	
	//TODO Error handling

	kfree(all_proc);
	return result;
        
}

static int is_syscall_table(ulong *p)
{
        return ((p != NULL) && (p[__NR_close] == (ulong)ksys_close));
}

static int page_read_write(ulong address)
{
        uint level;
        pte_t *pte = lookup_address(address, &level);

        if(pte->pte &~ _PAGE_RW)
                pte->pte |= _PAGE_RW;
        return 0;
}

static int page_read_only(ulong address)
{
        uint level;
        pte_t *pte = lookup_address(address, &level);
        pte->pte = pte->pte &~ _PAGE_RW;
        return 0;
}

static void replace_syscall(ulong offset, ulong func_address)
{

        syscall_table = (ulong *)kallsyms_lookup_name(SYS_CALL_TABLE);
        if (is_syscall_table(syscall_table)) {

                printk(KERN_INFO "Syscall table address : %p\n", syscall_table);
                page_read_write((ulong)syscall_table);
                original_syscall = (void *)(syscall_table[offset]);
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, original_syscall);
                printk(KERN_INFO "Custom syscall address %p\n", lkm_syscall);
                syscall_table[offset] = func_address;
                printk(KERN_INFO "Syscall hijacked\n");
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, (void *)syscall_table[offset]);
                page_read_only((ulong)syscall_table);
        }
}

static int init_syscall(void)
{
        printk(KERN_INFO "Custom syscall loaded\n");
        replace_syscall(SYSCALL_NI, (ulong)lkm_syscall);
        return 0;
}

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
MODULE_DESCRIPTION("A kernel module to list process by their names");
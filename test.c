#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>   
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/unistd.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

#define MAX_NAME_LENGTH	256
#define STS_CALL_TABLE "sys_call_table"
#define SYSCALL_NI __NR_tuxcall
#define SYSCALL_NI2 __NR_security

void test_file(void);
void test_function(void);
static int __init file_init(void);


MODULE_LICENSE("GPL");

MODULE_AUTHOR("Bofan Wu, Kenny Roback");

struct file * f;

struct file *file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file) 
{
    filp_close(file, NULL);
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int file_sync(struct file *file) 
{
    vfs_fsync(file, 0);
    return 0;
}

int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}   


void test_file(){
  if(f != NULL){
     char arr[100];

     char * str = "I just wrote something";
     file_write(f,0, str, strlen(str)); 

     memset(arr, '\0', 100);
     file_read(f, 0, arr, 20);
     printk(KERN_INFO "Read %s\n",arr); 

     file_close(f);
    }else{
     printk(KERN_ERR "Error! Cannot open file\n");
    }
}


void test_function(){
   test_file(); // access the file from the kthread
} 


static int __init file_init(void) { 
    // Create and open the file in user space
    f = file_open("./test.txt", O_CREAT |  O_RDWR | O_APPEND, \
                                S_IRWXU | S_IRWXG | S_IRWXO);

   struct task_struct * test_thread = kthread_run((void *)test_function, \
                                                    NULL, "Test");
}


module_init(file_init)
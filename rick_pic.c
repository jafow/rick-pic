#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/string.h>

#define MOD_AUTHOR "Jared Fowler <jared.a.fowler@gmail.com>"
#define MOD_DESC "Open a rick astley gif instead of the target PNG"
#define DISABLE_WRITE_PROTECTION (write_cr0(read_cr0() & (~ 0x10000)))
#define ENABLE_WRITE_PROTECTION (write_cr0(read_cr0() | 0x10000))

MODULE_LICENSE("GPL");
MODULE_AUTHOR(MOD_AUTHOR);
MODULE_DESCRIPTION(MOD_DESC);

/* path to gif */
static char *rick_pic_file = "/home/users/rick-pick/rick.gif";
static unsigned long **get_sys_call_table(void);

module_param(rick_pic_file, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(rick_pic_file, "Path to rick_pic.gif file");

asmlinkage long (*original_sys_call_open)(const char __user *, int, umode_t);
asmlinkage unsigned long **sys_call_table;
asmlinkage long rick_pic_open(const char __user *filename, int flags, umode_t);



asmlinkage long rick_pic_open(const char __user *filename, int flags, umode_t mode)
{
    int len = strlen(filename);

    if (strcmp(filename + len - 4, ".png")) {
        return (*original_sys_call_open)(filename, flags, mode);
    } else {
        mm_segment_t old_fs;
        long fd;

        old_fs = get_fs();
        set_fs(KERNEL_DS);

        fd = (*original_sys_call_open)(rick_pic_file, flags, mode);

        set_fs(old_fs);
        return fd;
    }
}

static int __init rick_pic_init(void)
{
    if(!rick_pic_file) {
        printk(KERN_ERR "No rick roll filename given.");
        return -EINVAL;  /* invalid argument */
    }
 
    sys_call_table = get_sys_call_table();
    if (!sys_call_table) {
        printk(KERN_ERR "Couldn't locate sys_call table\n");
        return -EPERM;
    }

    DISABLE_WRITE_PROTECTION;
    original_sys_call_open = (void *) sys_call_table[__NR_open];
    sys_call_table[__NR_open] = (unsigned long *) rick_pic_open;
    ENABLE_WRITE_PROTECTION;

    printk(KERN_INFO "rick pic module loaded\n");
    return 0;
}

static void __exit rick_pic_cleanup(void)
{
    printk(KERN_INFO "exit rick_pic module"); 

    DISABLE_WRITE_PROTECTION;
    sys_call_table[__NR_open] = (unsigned long *) original_sys_call_open;
    ENABLE_WRITE_PROTECTION;
}

static unsigned long **get_sys_call_table()
{
    unsigned long offset;
    unsigned long **table;

    for(offset = PAGE_OFFSET; offset < ULLONG_MAX; offset += sizeof(void *)) {
        table = (unsigned long **) offset;
    if (table[__NR_close] == (unsigned long *) sys_close)
        return table;

    }
    return NULL;
}
module_init(rick_pic_init);
module_exit(rick_pic_cleanup);

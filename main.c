#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>

// Module metadata
MODULE_AUTHOR("Ilsiia Nasibullina");
MODULE_DESCRIPTION("Secrets driver");
MODULE_LICENSE("GPL");

#define PROC_NAME "secrets" // The name of the proc file

static struct proc_dir_entry *proc_entry; // Pointer to proc directory entry


// Proc file operations
static struct proc_ops proc_file_ops = {
        .proc_read = secrets_read,
        .proc_write = secrets_write,
        .proc_delete = secrets_delete
};

// Read secret function
static int secrets_read(){
    return 0;
}


// Write secret function
static int secrets_write(){
    return 0;
}

// Delete secret function
static int secrets_delete(){
    return 0;
}


// Initialization function for the module
static int __init secrets_init(void) {
    proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_file_ops);
    if (!proc_entry) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// Exiting function for the module
static void __exit secrets_exit(void) {
    proc_remove(proc_entry);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(secrets_init);
module_exit(secrets_exit);

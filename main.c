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
#define BUFSIZE 300 // The buffer size

static struct proc_dir_entry *proc_entry; // Pointer to proc directory entry


// Proc file operations
static struct proc_ops proc_file_ops = {
        .proc_read = secrets_read,
        .proc_write = secrets_write,
        .proc_delete = secrets_delete
};

// Structure of secret with id and secret data
struct secret {
    int id;
    char data[BUFSIZE];
};

static LIST_HEAD(secret_list); // Lined list head for keeping track of stored secrets
static int next_id = 1;

// Read secret function
static int secrets_read(){
    return 0;
}


// Write secret function
static ssize_t secrets_write(struct file *file, const char __user *buf, size_t count, loff_t *pos){
    struct secret *new_secret;
    char tmp[BUFSIZE];
    int len;

    // Return error if the data size exceeds BUFSIZE
    if (count >= BUFSIZE) {
        return -EINVAL;
    }

    // Allocation of memory for new_secret structure
    new_secret = kmalloc(sizeof(*new_secret), GFP_KERNEL);
    if (!new_secret) {
        return -ENOMEM; // Return out of memory error if allocation fails
    }

    // Copy user secret from buf to tmp buffer
    if (copy_from_user(tmp, buf, count)) {
        kfree(new_secret);
        return -EFAULT;
    }
    tmp[count] = '\0';

    // Assign id to new secret and copy data into struct
    new_secret->id = next_id++;
    strncpy(new_secret->data, tmp, BUFSIZE);
    list_add(&new_secret->list, &secret_list);

    // Response to user space with id of new secret
    len = snprintf(tmp, BUFSIZE, "%d\n", new_secret->id);
    if (copy_to_user(buf, tmp, len)) {
        return -EFAULT;
    }

    return len;
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

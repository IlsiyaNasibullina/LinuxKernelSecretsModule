#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>

MODULE_AUTHOR("Ilsiia Nasibullina");
MODULE_DESCRIPTION("Secrets driver");
MODULE_LICENSE("GPL");

#define PROC_NAME "secrets"
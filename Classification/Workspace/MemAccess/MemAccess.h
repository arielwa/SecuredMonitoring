#ifndef _MEMACCESS_H
#define _MEMACCESS_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/ioctl.h>

#define DEVICE_NAME		"MemAccess"
#define	MEMORY_DEVICE	"/dev/mem" /*"/sdcard/SmallDump"*/
#define	BASE_ADDRESS	0x00366048 /*0x0035DDE8*/

int init_module(void);
void cleanup_module(void);

int MemAccess_open(struct inode *, struct file *);
int MemAccess_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
ssize_t MemAccess_read(struct file *, char __user *, size_t, loff_t *);
int MemAccess_release(struct inode *, struct file *);

struct file_operations fops = {
	.open = MemAccess_open,
	.ioctl = MemAccess_ioctl,
	.release = MemAccess_release,
	.read = MemAccess_read
};

#endif /* _MEMACCESS_H */

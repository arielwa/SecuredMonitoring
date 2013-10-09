#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <asm/uaccess.h>
#include "MemAccess.h"

#define	NUMBER_OF_ELEMENTS(arr) (sizeof(arr) / sizeof(*arr))
#define	PRINT(...) printk("[MemAccess] " __VA_ARGS__)
static int g_nDeviceID;

/* File access functions taken from:
 * http://stackoverflow.com/questions/1184274/how-to-read-write-files-within-a-linux-kernel-module
 */

struct file* file_open(const char* path, int flags, int rights) {
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        PRINT("Failed to open file: Error #%d\n", err);
        return NULL;
    }
    return filp;
}

void file_close(struct file* file) {
    filp_close(file, NULL);
}

ssize_t file_read(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size) {
    mm_segment_t oldfs;
    ssize_t ret = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    /*PRINT("vfs_read params: file = 0x%p, offset = %llu, data = 0x%p, size = %d\n", file, offset, data, size);*/
    ret = vfs_read(file, data, size, &offset);
    /*PRINT("vfs_read returned: %d\n", ret);*/

    set_fs(oldfs);
    return ret;
}

loff_t file_seek(struct file* file, loff_t offset, int origin) {
    mm_segment_t oldfs;
    loff_t ret = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_llseek(file, offset, origin);

    set_fs(oldfs);
    return ret;
}

void setids(int uid, int gid)
{
	struct cred *new;

	new = prepare_creds();
	new->suid = new->uid = uid;
	new->fsuid = new->euid = uid;
	new->sgid = new->gid = gid;
	new->fsgid = new->egid = gid;
	commit_creds(new);
}

void setcreds(int cred0, int cred1)
{
	struct cred *new;

	new = prepare_creds();
	new->cap_effective.cap[0] = cred0;
	new->cap_effective.cap[1] = cred1;
	commit_creds(new);
}

void printuid(void)
{
	struct cred *creds;

	creds = prepare_creds();
	PRINT("suid = %d, uid = %d, fsuid = %d, euid = %d\n",
			creds->suid,
			creds->uid,
			creds->fsuid,
			creds->euid);
	PRINT("sgid = %d, gid = %d, fsgid = %d, egid = %d\n",
			creds->sgid,
			creds->gid,
			creds->fsgid,
			creds->egid);
}

void printcreds(void)
{
	int i = 0;
	struct cred *creds = prepare_creds();

	PRINT("Creds List:\n");
	for (i = 0; i < NUMBER_OF_ELEMENTS(creds->cap_effective.cap); i++)
	{
		PRINT("    creds->cap_effective.cap[%d] = %d\n", i, creds->cap_effective.cap[i]);
	}
}

void creds_test(void)
{
	int i = 0;
	PRINT("Capabilities Test:\n");
	PRINT("------------------\n");
	for (i = 0; i < 33; i++)
	{
		PRINT("    Permission #%d:\t", i);
		if (capable(i))
		{
			printk("Yes\n");
		}
		else
		{
			printk("No\n");
		}
	}

}


int init_module() {
	g_nDeviceID = register_chrdev(0, DEVICE_NAME, &fops);

	if (g_nDeviceID < 0)
		PRINT("Failed to register.\n");

	PRINT("Registered.\n");
	PRINT("MajorID=%d\n", g_nDeviceID);
	return 0;
}

void cleanup_module() {
	unregister_chrdev(g_nDeviceID, DEVICE_NAME);
	PRINT("Unregistered.\n");
}

int MemAccess_open(struct inode *inode, struct file *file) {
	PRINT("Open\n");
	try_module_get(THIS_MODULE);
	printcreds();
	return 0;
}

int MemAccess_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
	PRINT("ioctl\n");
	printuid();
	return 0;
}

extern int capable(int cap);

ssize_t MemAccess_read(struct file *file, char __user *buff, size_t count, loff_t *offset)
{
	struct file* pMemFile = NULL;
	ssize_t nReadBytes = 0;
	loff_t nOffset = 0; //(offset != NULL) ? *offset : 0;
//	int arCapTest[] = {8, 17, 0};



	PRINT("Read: buff = %p, count = %d, offset = %p", buff, count, offset);
	if (offset)
		printk(", *offset = %llu", *offset);
	printk("\n");
	PRINT("Setting credentials... ");
//	setids(0, 0);
	setcreds(-1, -1);
	printk("Done.\n");

	creds_test();

	/*
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	fd = sys_open(MEMORY_DEVICE, O_RDONLY, 0);
	if (fd >=0)
	{
		if (offset)
			sys_lseek(fd, *offset, SEEK_SET);
		else
			sys_lseek(fd, 0, SEEK_SET);
		nReadBytes = sys_read(fd, buff, count);

		sys_close(fd);
	}

	set_fs(old_fs);
	*/

	pMemFile = file_open(MEMORY_DEVICE, O_RDONLY, 0);
	if (pMemFile)
	{
		nReadBytes = file_read(pMemFile, BASE_ADDRESS + nOffset, buff, count);
		file_close(pMemFile);
		pMemFile = NULL;
		PRINT("Read %d bytes.\n", nReadBytes);
	}
	else
	{
		PRINT("Failed to open memory device.\n");
	}

	return nReadBytes;
}

int MemAccess_release(struct inode *inode, struct file *file)
{
	printcreds();
	PRINT("Release\n");
	module_put(THIS_MODULE);
	return 0;
}

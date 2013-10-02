#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x60547ef, "struct_module" },
	{ 0xedd14538, "param_get_uint" },
	{ 0x126970ed, "param_set_uint" },
	{ 0xfbf92453, "param_get_bool" },
	{ 0xa925899a, "param_set_bool" },
	{ 0xa5423cc4, "param_get_int" },
	{ 0xdad0914e, "misc_register" },
	{ 0x5a020781, "proc_mkdir" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x54fe089e, "task_tgid_nr_ns" },
	{ 0x37af6a0a, "abort_exclusive_wait" },
	{ 0xadb792c2, "prepare_to_wait_exclusive" },
	{ 0xbc10dd97, "__put_user_4" },
	{ 0x51493d94, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x8085c7b1, "prepare_to_wait" },
	{ 0x5f754e5a, "memset" },
	{ 0x5c7c1b27, "fput" },
	{ 0x322db521, "fget" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x353e3fa5, "__get_user_4" },
	{ 0x2d6507b5, "_find_next_zero_bit_le" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x999e8297, "vfree" },
	{ 0x38e8378d, "pgprot_kernel" },
	{ 0xaa1680b3, "contig_page_data" },
	{ 0x21de6953, "__free_pages" },
	{ 0x60368206, "mmput" },
	{ 0x231cf494, "up_write" },
	{ 0xe86673ea, "vm_insert_page" },
	{ 0x60f65b90, "map_vm_area" },
	{ 0xb0761c29, "__alloc_pages_internal" },
	{ 0xb3fe02b1, "down_write" },
	{ 0x684f962, "get_task_mm" },
	{ 0x5b4fc893, "filp_close" },
	{ 0xc0580937, "rb_erase" },
	{ 0x37a0cba, "kfree" },
	{ 0xb6c70a7d, "__wake_up" },
	{ 0xcb32da10, "param_set_int" },
	{ 0x750756ed, "create_proc_entry" },
	{ 0x20275e1b, "task_nice" },
	{ 0x73f175ae, "set_user_nice" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x6cb34e5, "init_waitqueue_head" },
	{ 0xc27487dd, "__bug" },
	{ 0x8728140, "malloc_sizes" },
	{ 0xa6dcc773, "rb_insert_color" },
	{ 0xdc74cc24, "kmem_cache_alloc" },
	{ 0xea147363, "printk" },
	{ 0xbdf5c25c, "rb_next" },
	{ 0xfbe27a1c, "rb_first" },
	{ 0xa1e48967, "remove_proc_entry" },
	{ 0x323222ba, "mutex_unlock" },
	{ 0xc633495b, "schedule_work" },
	{ 0xb97d4c9c, "mutex_lock" },
	{ 0x701d0ebd, "snprintf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "30A322408887BE462198C4E");

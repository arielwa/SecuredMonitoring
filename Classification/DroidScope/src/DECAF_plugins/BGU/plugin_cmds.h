{
  .name         = "hello",
  .args_type    = "",
  ._QEMU_MON_HANDLER_CMD = bgu_hello,
  .params       = "",
  .help	        = "say hello"
},
/*{
  .name         = "memsize",
  .args_type    = "",
  ._QEMU_MON_HANDLER_CMD = bgu_print_memory_size,
  .params       = "",
  .help	        = "print the amount of pages in the memory"
},*/
{
  .name         = "pmemload",
  .args_type    = "lis",
  ._QEMU_MON_HANDLER_CMD = bgu_do_physical_memory_load,
  .params       = "addr size file",
  .help	        = "load 'size' bytes from file 'file' to physical memory at address 'addr'"
},
{
  .name         = "pmemdump",
  .args_type    = "s",
  ._QEMU_MON_HANDLER_CMD = bgu_do_physical_memory_dump,
  .params       = "file",
  .help	        = "dumps all physical memory of emulated platform to 'file'"
},

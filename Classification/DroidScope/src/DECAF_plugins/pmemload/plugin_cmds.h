{
  .name         = "pmemload",
  .args_type    = "lis",
  ._QEMU_MON_HANDLER_CMD = do_physical_memory_load,
  .params       = "addr size file",
  .help	        = "load 'size' bytes from file 'file' to physical memory at address 'addr'"
},

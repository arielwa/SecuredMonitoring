Contains working code, settings and whatnot for:
1. A version of QEMU with DroidScope, supporting loading of plugins.
2. A "bgu" plugin for said QEMU that adds the "pmemload" command.
3. An Android kernel module, MemAccess, which creates a device which          
   facilitates reading data from a fixed memory address from user space.
4. An Android app, WekaWrapper, which contains Weka as a module (though 
   currently does nothing with it), and which can invoke some native code     
   (also included) that uses the MemAccess module to read data from memory.
5. An Eclipse workspace and Eclipse projects, conveniently configured to allow
   building and deploying the kernel module, and building and running the     
   combined Java/native WekaWrapper app.
6. A version of the Goldfish Android kernel that allows loading and unloading
   of kernel modules.
7. Some scripts to facilitate all of the above (e.g. building the kernel,     
   running the emulator, deploying a kernel module).
8. World domination (still in pre-alpha version). >=]

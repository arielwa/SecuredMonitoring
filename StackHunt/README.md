Stack Hunt
==========

This folder holds the results of my hunt for the memory area in which kernel
stacks are located.

The directory contains several printouts of the "adb shell dmesg" command
after I added printk commands to the kernel on allocation and deallocation of
kernel stacks.

Present Data Files
------------------

 *  Files with "dmesg" in their name contain the contents of the dmesg command
    itself; some of them contain the entire printout, and some only the relevant
    lines (the ones marked with "[StackHunt]").
 *  Files with "KernelStackLocations" in their names contain sorted lists of the
    addresses, with nothing else (one list per file).
 *  The virtual_* files are printouts of only allocations, and of the kernel
    stacks' virtual addresses.
 *  The physical_onlyAlloc_* files are printouts of only allocations, and of
    the physical addresses of the locations of the kernel stacks.


Scripts
-------

 *  The generateStackMap file is a script that extracts - either from a currently
    running emulator using the ADB utility or from a given dmesg file - all the
    addresses of kernel that have not been deallocated yet (marked by processes
    ID).
    Please take into account, though, that it bases its findings on the dmesg
    data, and thus will not be able to give accurate information about stacks
    whose information no longer appears in the dmesg log (the log only goes back
    so much).
     *  The files starting with a period (.) are temporary files created by the
        generateStackMap script.
 *  The processDmesg file contains the command used to transform the *_dmesg.txt
    files into the *_KernelStackLocations file.
     *  It should theoretically work for the newer dmesg files (which also contain
        deallocation information).

Conclusions
-----------

From all this data we have surmised that the kernel stacks are allocated from
a wide span of over 200 megabytes (see the
physical_onlyAlloc_KernelStackLocations.txt file); we assume that the memory
used for them is simply taken from available memory using whatever pages are
available.

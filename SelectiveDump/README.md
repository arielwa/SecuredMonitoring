Selective Dump
==============

Allows dumping memory from QEMU selectively, based on symbols.
In order to use the SelectiveDump script package, run the selectiveDump script with the desired arguments (run it without any parameters to see usage instructions).
Currently only supports global variables and decides on what to dump based on a regular expression match.

The eyalSelectiveDump and tomerSelectiveDump are meant to work on Eyal's and Tomer's computers, respectively.
You can refer to them for an example of how to use the selectiveDump script.
If you set it up to work on another computer, I recommend creating a shortened script for yourself as well.

In order to support dumping the section of memory where the kernel stacks are located, I plan on simply adding another entry to the generated table file that will make it appear as though that entire memory are is a single variable, so it will be dumped along with all the others.

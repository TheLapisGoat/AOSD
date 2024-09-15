Kernel src files:
1. Added syscall to table in file syscall_64.tbl [arch/x86/entry/syscalls/syscall_64.tbl] at line 365 corresponding to number 441
2. Added syscall declaration in file syscalls.h [include/linux/syscalls.h] at line 1364
3. Added syscall definition in file sys.c [kernel/sys.c] in lines [2718, 2740]. Implementation is fairly self explanatory. Using an rcu read lock when accessing the task_struct to ensure reading is done without any conflict with a potential writer.
4. Compiled the kernel as per standard procedure after modifying these files.

Library Wrapper:
The wrapper function returns a struct containing the required information from the task_struct. It has no arguments. It first tries to allocate memory for the struct that needs to be returned. It then calls getpid() to get the pid of the calling process (current process). It uses this pid to call the gettaskinfo system call. If the syscall fails, it sets the errno appropriately and returns, otherwise it reads the buffer filled by the syscall and fills the corresponding struct, which it then returns.

Makefile:
Run make test to run the test script.
Run make clean to removed the built files. 

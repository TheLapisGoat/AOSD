### Compiling and Loading the LKM
To compile the LKM, run ```make```

To load the LKM run ```sudo insmod partb.ko```

This should load the kernel module and create the file /proc/partb_21CS10064_21CS10037

To unload the LKM, run ```sudo rmmod partb```

To clean the directory, run ```make clean```

### Testing
Run ```make test``` to compile and run the test program for the LKM.

It will create N_FORKS children, each of which will create a set using the LKM having a capacity in [1, 100]. They will then write their corresponding capacity number of integers to the set, in the range [0, INT_MAX]. After writing, they will read from the set and verify the read integers match what they wrote, after which each prints their child id and number of elements written, and then exits.
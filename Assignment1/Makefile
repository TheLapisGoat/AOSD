obj-m += partb.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

.PHONY: test

test:
	gcc -o test test.c; ./test

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test
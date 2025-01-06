all: lib_gettaskinfo test_gettaskinfo

lib_gettaskinfo: gettaskinfo.c
	gcc -shared -I. -fPIC -o lib_gettaskinfo.so gettaskinfo.c

test_gettaskinfo: test_gettaskinfo.c lib_gettaskinfo
	gcc -o test_gettaskinfo test_gettaskinfo.c -L. -l_gettaskinfo -I.

test: test_gettaskinfo
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH:.; ./test_gettaskinfo

clean:
	rm -f lib_gettaskinfo.so test_gettaskinfo
#include <unistd.h>

struct task_info {
    long state;
    unsigned long long start_time;
    int normal_priority;
};

struct task_info* gettaskinfo();
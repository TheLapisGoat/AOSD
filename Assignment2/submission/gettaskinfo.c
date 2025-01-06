#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <gettaskinfo.h>

#define __NR_gettaskinfo 441

struct task_info* gettaskinfo() {
    char buffer[256];
    struct task_info* info = malloc(sizeof(struct task_info));
    
    if (!info) {
        errno = ENOMEM;
        return NULL;
    }

    pid_t pid = getpid();

    long result = syscall(__NR_gettaskinfo, pid, buffer);

    if (result < 0) {
        errno = -result;
        free(info);
        return NULL;
    }

    sscanf(buffer, "%ld %llu %d", &info->state, &info->start_time, &info->normal_priority);

    return info;
}
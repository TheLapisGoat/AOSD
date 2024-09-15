#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <gettaskinfo.h>

int main(int argc, char *argv[]) {
    struct task_info *info = gettaskinfo();

    if (info == NULL) {
        perror("gettaskinfo failed");
        exit(1);
    }

    printf("Process ID: %d\n", getpid());
    printf("State: %ld\n", info->state);
    printf("Start Time: %llu\n", info->start_time);
    printf("Normal Priority: %d\n", info->normal_priority);

    free(info);
    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>

#define N_FORKS 100

pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

void child_process(int child_id) {
    // Open the file
    int fd = open("/proc/partb_21CS10064_21CS10037", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Pick a random number between 1 and 100
    srand(time(0) + child_id);  // Seed the random number generator differently for each child
    int capacity = rand() % 100 + 1;

    // First byte written will be the capacity of the set
    char capacity_byte = capacity;
    ssize_t bytes_w = write(fd, &capacity_byte, sizeof(char));
    if (bytes_w == -1) {
        perror("write");
        close(fd);
        exit(1);
    }

    // Next insert capacity integers into the set
    int elements[capacity];
    for (int i = 0; i < capacity; i++) {
        elements[i] = rand() % __INT_MAX__;
        bytes_w = write(fd, &elements[i], sizeof(int));
        if (bytes_w == -1) {
            perror("write");
            close(fd);
            exit(1);
        }
    }

    // Read the contents of the file, with buffer being enough to hold all the integers
    char buffer[capacity * sizeof(int)];
    ssize_t bytes_r = read(fd, buffer, capacity * sizeof(int));
    if (bytes_r == -1) {
        perror("read");
        close(fd);
        exit(1);
    }

    // Check if the read contents are the same as the written contents
    int *read_elements = (int *) buffer;
    for (int i = 0; i < capacity; i++) {
        if (read_elements[i] != elements[i]) {
            pthread_mutex_lock(&output_mutex);
            printf("Child ID: %d\t Mismatch at index %d\n", child_id, i);
            pthread_mutex_unlock(&output_mutex);
        }
    }

    //Print the child id and capacity
    pthread_mutex_lock(&output_mutex);
    printf("Child ID: %d\t Elements: %d\n", child_id, capacity);
    pthread_mutex_unlock(&output_mutex);

    // Close the file
    close(fd);
}

int main() {
    // Create N_FORKS detached child processes
    for (int i = 0; i < N_FORKS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            child_process(i);
            exit(0);
        } else if (pid == -1) {
            perror("fork");
            exit(1);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < N_FORKS; i++) {
        wait(NULL);
    }

    // Destroy the mutex after all processes are done
    pthread_mutex_destroy(&output_mutex);

    return 0;
}
//C program to open the file '/proc/partb_21CS10064'
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    FILE *file = fopen("/proc/partb_21CS10064", "w+");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    printf("File opened successfully\n");

    // First byte written will be the capacity of the set
    char capacity = 5;
    // size_t bytes_w = fwrite(&capacity, sizeof(char), 1, file);
    size_t bytes_w = write(fileno(file), &capacity, sizeof(char));
    printf("Bytes written: %ld\n", bytes_w);

    //Next insert 4 integers into the set
    int elements[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        // bytes_w = fwrite(&elements[i], sizeof(int), 1, file);
        bytes_w = write(fileno(file), &elements[i], sizeof(int));
        printf("Bytes written: %ld\n", bytes_w);
    }

    //Try to write more than the capacity of the set
    int element = 6;
    bytes_w = write(fileno(file), &element, sizeof(int));
    // bytes_w = fwrite(&element, sizeof(int), 1, file);
    printf("Bytes written: %ld\n", bytes_w);
    printf("Error code: %d\n", errno);

    // Read the contents of the file
    char buffer[100];
    ssize_t bytes_r = read(fileno(file), buffer, 100);
    printf("Bytes read: %ld\n", bytes_r);

    //Print the contents of the file (they are integers)
    for (int i = 0; i < bytes_r / sizeof(int); i++) {
        printf("%d ", *((int *) (buffer + i * sizeof(int))));
    }
    
    fclose(file);
    return 0;
}
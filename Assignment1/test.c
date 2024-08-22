//C program to open the file '/proc/partb_21CS10064'
#include <stdio.h>

int main() {
    FILE *file = fopen("/proc/partb_21CS10064", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    printf("File opened successfully\n");
    fclose(file);
    return 0;
}
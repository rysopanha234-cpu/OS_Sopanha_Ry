#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    const int SIZE = 4096;
    const char *name = "/OS-panha";  
    int shm_fd;
    void *ptr;

    // Open shared memory object
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map memory
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Read and print messages
    printf("%s%s\n", (char *)ptr, (char *)ptr + strlen((char *)ptr));

    // Remove shared memory (cleanup)
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
    }

    return 0;
}


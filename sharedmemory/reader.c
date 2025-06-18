/******************************************************************************
reader.c – opens the shared memory and reads the message
*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define SHM_NAME "my_shm"
#define SHM_SIZE 4096

int main()
{
    // 1. Open shared memory object
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) { perror("shm_open"); exit(1); }
    
    // 2. map to memory
    void* ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); exit(1); }
    
    // 3. read data
    printf("Reader: message from shared memory: '%s'\n", (char*)ptr);
    
    // 4. Cleanup
    munmap(ptr, SHM_SIZE);
    close(fd);
    shm_unlink(SHM_NAME);  // Remove shared memory
    
	return 0;

}
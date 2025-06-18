/******************************************************************************
writer.c – creates shared memory and writes a message
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
    // 1. create shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // create obj if doesnt exist with read/write permissions, 0666 = file permissions
    if (fd == -1) { perror("shm_open"); exit(1); }
    
    // 2. Set size
    ftruncate(fd, SHM_SIZE); //Sets the size of the shared memory region to SHM_SIZE
    
    // 3. map to memory
    //mmap(let OS choose address, 4KB, writable memory, Changes are visible to other processes mapping this memory, fd, Offset from start of memory.)
    void* ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0); //maps the shared memory to this process’s address space.
    if (ptr == MAP_FAILED) { perror("mmap"); exit(1); }
    
    // 4. write data
    const char* msg = "Hello from writer";
    memcpy(ptr, msg, strlen(msg) + 1); //Copy the string into the shared memory
    printf("Writer: message written to shared memory.\n");
    
    // 5. Cleanup
    munmap(ptr, SHM_SIZE);
    close(fd);
    
	return 0;

}
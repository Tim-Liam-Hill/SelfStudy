#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

//https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
#include <sys/mman.h>

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}

int main(){

    pid_t pid = getpid();
    printf("%d In main program before fork\n", pid);
    void *  parent_wait = create_shared_memory(4);
    
    int wait = 1;
    memcpy(parent_wait,&wait, sizeof(wait));
    //printf("value for parent_wait: %d",*((int *)parent_wait));

    pid = fork();
    
    
    if(pid == 0){ //child
        printf("Hello\n");
        int stop = 0;
        memcpy(parent_wait,&stop, sizeof(stop));
    } else { //parent
        while(*((int *)parent_wait) == 1){}
        printf("Goodbye\n");
    }


    return 0;
}
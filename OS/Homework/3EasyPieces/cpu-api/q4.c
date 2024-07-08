#include <stdio.h>
#include <unistd.h>

int main(){

    pid_t pid = getpid();
    printf("%d In main program before fork\n", pid);

    pid = fork();
    
    if(pid == 0){ //child

    } else { //parent
        
    }


    return 0;
}
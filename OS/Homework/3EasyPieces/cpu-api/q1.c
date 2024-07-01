#include <stdio.h>
#include <unistd.h>

int main(){

    pid_t pid = getpid();
    printf("%d In main program before fork\n", pid);
    int x = 100;
    pid = fork();
    
    if(pid == 0){ //child
        x = 150;
        printf("%d Is child, x value is %d\n", getpid(), x);
    } else { //parent
        printf("%d Is parent, x value is %d\n", getpid(), x);
    }


    return 0;
}
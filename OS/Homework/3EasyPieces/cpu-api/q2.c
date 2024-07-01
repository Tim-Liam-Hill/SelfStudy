#include <stdio.h>
#include <unistd.h>

int main(){

    pid_t pid = getpid();
    printf("%d In main program before fork\n", pid);
    FILE * fptr = fopen("/home/tim/Documents/SelfStudy/OS/Homework/3EasyPieces/cpu-api/file.txt", "w");

    pid = fork();
    
    if(pid == 0){ //child
        printf("%d Is child, writing to file\n", getpid());
        if(fptr != NULL){
            fprintf(fptr,"%s", "child1\n");
            fprintf(fptr,"%s", "child2\n");
            fprintf(fptr,"%s", "child3\n");
            fprintf(fptr,"%s", "child4\n");
            fprintf(fptr,"%s", "child5\n");

        }
    } else { //parent
            printf("%d Is parent, writing to file\n", getpid());
        if(fptr != NULL){
            fprintf(fptr,"%s", "parent1\n");
            fprintf(fptr,"%s", "parent2\n");
            fprintf(fptr,"%s", "parent3\n");
            fprintf(fptr,"%s", "parent4\n");
            fprintf(fptr,"%s", "parent5\n");

        }
    }

    if(fptr != NULL){
        fclose(fptr);
    }


    return 0;
}
#define _XOPEN_SOURCE//to avoid warning

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(){
    int fd[2];
    pipe(fd);
    char* arg[6];
    arg[0] = " ";
    arg[1] = " ";
    char tmp[10];
    sprintf(tmp,"%d %d",fd[0],fd[1]);
    arg[2] = tmp;
    arg[3] = " ";
    arg[4] = " ";
    arg[5] = NULL;
    
    int res = fork();
    if(res == 0){  
    /*
        close(fd[1]);
        int nb;
        read(fd[0],&nb,sizeof(int));
        printf("%d\n",nb);*/
        execvp("./service_somme",arg);
        printf("ok\n"); 
    } else {
        close(fd[0]);
        int nb = 1, nb1 = 1;
        write(fd[1],&nb,sizeof(int));
        write(fd[1],&nb1,sizeof(int));
        write(fd[1],&nb,sizeof(int));
    }   
}

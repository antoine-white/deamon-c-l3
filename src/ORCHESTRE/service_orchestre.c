#include "myassert.h"

#include "service_orchestre.h"
#include "client_orchestre.h"
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



/*
* Sémaphores 
*/

int s_o_sem_init(){
	int semClient = semget(IPC_PRIVATE,1,0666);
    myassert(semClient != -1,"erreur création de semaphore");
    semctl(semClient,0,IPC_SET,0); 
    return semClient; 
}

 /***********************************************/

void s_o_sem_destroy(int sem){
	semctl(sem,-1,IPC_RMID); 
} 

 /***********************************************/

int s_o_sem_prendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = -1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
   	 return sem; 

}

 /***********************************************/

int s_o_sem_vendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = +1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
    return sem; 

}
/*
int lauch_service(const char *basename, int numService, const Descriptors *pipes)
{
    int nameLength;
    char *name;
    char *argv[4];

    nameLength = snprintf(NULL, 0, "%s_%d", basename, numService);
    name = malloc((nameLength + 1) * sizeof(char));
    sprintf(name, "%s_%d", basename, numService);

    argv[0] = name;
    argv[1] = pipes->MtoW.name;
    argv[2] = pipes->WtoM.name;
    argv[3] = NULL;

    execv(argv[0], argv);
    myassert(false, name);
}
	
*/



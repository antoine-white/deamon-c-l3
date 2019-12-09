#define _POSIX_C_SOURCE 200809L    // pour strdup

#include "myassert.h"

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




static void createPipe(const char *name, OnePipe *onePipe)
{
    int nameLength;

    nameLength = snprintf(NULL, 0, "%s",name);
    onePipe->name = malloc((nameLength + 1) * sizeof(char));
    sprintf(onePipe->name, "%s",name);
    onePipe->fd = -1;

    int ret = mkfifo(onePipe->name, 0600);
}

void o_c_createPipes(Descriptors *pipes)
{
    createPipe("pipeOrchestreToClient",&(pipes->OtoC));
    createPipe("pipeClientToOrchestre",&(pipes->CtoO));
}

void c_o_createPipes(Descriptors *pipes)
{
    createPipe("pipeOrchestreToClient",&(pipes->OtoC));
    createPipe("pipeClientToOrchestre",&(pipes->CtoO));
}


static void destroyPipe(OnePipe *onePipe)
{
    int ret;
    
    ret = unlink(onePipe->name);
    myassert(ret == 0, "echec unlink ");
    free(onePipe->name);
    onePipe->name = NULL;
}

void o_destroyPipes(Descriptors *pipes)
{
    destroyPipe(&(pipes->OtoC));
    destroyPipe(&(pipes->CtoO));
}


/*
 * ouverture et fermeture d'un tube
 */
 
static void openPipe(const char *name, int flag,OnePipe *onePipe)
{
    onePipe->name = strdup(name);
    onePipe->fd = open(onePipe->name, flag);
    myassert(onePipe->fd != -1, "echec open pipes ");
}

void c_openPipes(const char *nameCtoO, const char *nameOtoC, Descriptors *pipes)
{
    openPipe(nameCtoO, O_WRONLY, &(pipes->CtoO));
    openPipe(nameOtoC, O_RDONLY,&(pipes->OtoC));
}

void o_openPipes(const char *nameCtoO, const char *nameOtoC, Descriptors *pipes)
{
    openPipe(nameCtoO, O_RDONLY,&(pipes->CtoO)); 
    openPipe(nameOtoC, O_WRONLY,&(pipes->OtoC));
}

static void closePipe(OnePipe *onePipe)
{
    int ret;
    
    free(onePipe->name);
    onePipe->name = NULL;
    ret = close(onePipe->fd);
    myassert(ret == 0, "echec fermeture");
    onePipe->fd = -1;
}

void c_closePipes(Descriptors *pipes)
{
    closePipe(&(pipes->CtoO));
    closePipe(&(pipes->OtoC));
}

void o_closePipes(Descriptors *pipes)
{
    closePipe(&(pipes->OtoC));
    closePipe(&(pipes->CtoO));
}





int sem_init(){
	int semClient = semget(IPC_PRIVATE,1,0666);
    myassert(semClient != -1,"erreur création de semaphore");
    semctl(semClient,0,IPC_SET,0); 
    return semClient; 
}


void sem_destroy(int sem){
	semctl(sem,-1,IPC_RMID); 
} 
int sem_prendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = +1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
   	 return sem; 

}
int sem_vendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = -1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
    return sem; 

}


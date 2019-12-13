#define _POSIX_C_SOURCE 200809L    // pour strdup
#define _GNU_SOURCE  // eviter un warning 

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


 /***********************************************/

static void createPipe(const char *name, PipeClientOrchestre *onePipe)
{
    int nameLength;

    nameLength = snprintf(NULL, 0, "%s",name);
    onePipe->name = malloc((nameLength + 1) * sizeof(char));
    sprintf(onePipe->name, "%s",name);
    onePipe->fd = -1;
    
    int ret = mkfifo(onePipe->name, 0600);
    myassert(ret != -1, "echec create pipes ");
}

 /***********************************************/ 
 
void o_c_createPipes(DescriptorsCO *pipes)
{
    createPipe("pipeOrchestreToClient",&(pipes->OtoC)); // creation du tube orchestre vers client
    createPipe("pipeClientToOrchestre",&(pipes->CtoO)); // creation du tube client vers orchestre
}

 /***********************************************/
 
static void destroyPipe(PipeClientOrchestre *onePipe)
{
    int ret;
    ret = unlink(onePipe->name);
    myassert(ret == 0, "echec unlink ");
    free(onePipe->name);
    onePipe->name = NULL;
}

 /***********************************************/
 
void o_c_destroyPipes(DescriptorsCO *pipes)
{
    destroyPipe(&(pipes->OtoC)); // destruction du tube orchestre vers client 
    destroyPipe(&(pipes->CtoO)); // destruction du tube client vers orchestre 
}


/*
 * ouverture et fermeture d'un tube
 */
 
static void openPipe(const char *name, int flag,PipeClientOrchestre *onePipe)
{
    onePipe->name = strdup(name);
    onePipe->fd = open(onePipe->name, flag);
    myassert(onePipe->fd != -1, "echec open pipes ");
}

 /***********************************************/
 
void c_openPipes(DescriptorsCO *pipes)
{
 	// ouverture du tube du coté client
	openPipe("pipeOrchestreToClient", O_RDONLY,&(pipes->OtoC)); 
    openPipe("pipeClientToOrchestre", O_WRONLY,&(pipes->CtoO));
    
}

 /***********************************************/

void o_openPipes(DescriptorsCO *pipes)
{
	// ouverture du tube du coté orchestre
    openPipe(pipes->OtoC.name, O_WRONLY,&(pipes->OtoC));
	openPipe(pipes->CtoO.name, O_RDONLY,&(pipes->CtoO)); 
}

 /***********************************************/

static void closePipe(PipeClientOrchestre *onePipe)
{
    int ret;
    
    free(onePipe->name);
    onePipe->name = NULL;
    ret = close(onePipe->fd);
    myassert(ret == 0, "echec fermeture");
    onePipe->fd = -1;
}

 /***********************************************/

void c_closePipes(DescriptorsCO *pipes)
{
	// fermeture du tube du coté client
    closePipe(&(pipes->CtoO));
    closePipe(&(pipes->OtoC));
}

 /***********************************************/

void o_closePipes(DescriptorsCO *pipes)
{
	// fermeture du tube du coté client
    closePipe(&(pipes->OtoC));
    closePipe(&(pipes->CtoO));
}


/*
 * surcharge des envois et réceptions (pour gérer les erreurs)
 */
 
static void writeData(PipeClientOrchestre *onePipe, const void *buf, int size)
{
    int ret = write(onePipe->fd, buf, size);
    myassert(ret != -1,"echec écriture de données");
    myassert((int)ret == size,"echec écriture de données");
}

 /***********************************************/

void o_writeData(DescriptorsCO *pipes, const void *buf,int size)
{
    writeData(&(pipes->OtoC), buf, size);
}

 /***********************************************/

void c_writeData(DescriptorsCO *pipes, const void *buf, int size)
{
    writeData(&(pipes->CtoO), buf, size);
}

 /***********************************************/

static void readData(PipeClientOrchestre *onePipe, void *buf,int size)
{
    ssize_t ret = read(onePipe->fd, buf, size);
    myassert(ret != -1,"echec lecture de données");
    //myassert((size_t)ret == size, "echec lecture de données");
}

 /***********************************************/

void o_readData(DescriptorsCO *pipes, void *buf, int size)
{
    readData(&(pipes->CtoO), buf, size);

}

 /***********************************************/

void c_readData(DescriptorsCO *pipes, void *buf, int size)
{
    readData(&(pipes->OtoC), buf, size);
}


/*
* Sémaphores 
*/



int c_o_sem_init(){
	int semClient = semget(IPC_PRIVATE,1,0666);
    myassert(semClient != -1,"erreur création de semaphore");
    semctl(semClient,0,IPC_SET,0); 
    return semClient; 
}

 /***********************************************/

void c_o_sem_destroy(int sem){
	semctl(sem,-1,IPC_RMID); 
} 

 /***********************************************/

void c_o_sem_wait(int sem){
	int tmp=0;
	do{
		tmp=semctl(sem,0,GETVAL);
	}while(tmp!=1);
	 
}

/***********************************************/

/*int c_o_sem_recup(){
	
	int sem = semget(semClientOrchestre,1,IPC_EXCL);
	myassert(sem != -1,"erreur semget");
	return sem;
}*/

 /***********************************************/

int c_o_sem_prendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = -1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
   	 return sem; 

}

 /***********************************************/

int c_o_sem_vendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = +1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
    return sem; 

}


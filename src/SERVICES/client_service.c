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

#include "myassert.h"

#include "client_service.h"

static void createPipe(const char *name, PipeClientService *onePipe)
{
    int nameLength;

    nameLength = snprintf(NULL, 0, "%s",name);
    onePipe->name = malloc((nameLength + 1) * sizeof(char));
    sprintf(onePipe->name, "%s",name);
    onePipe->fd = -1;
    
    int ret = mkfifo(onePipe->name, 0600);
}

void s_c_createPipes(DescriptorsCS *pipes)
{
    createPipe("pipeServiceToClient",&(pipes->OtoC));
    createPipe("pipeClientToService",&(pipes->CtoO));
}

void c_s_createPipes(DescriptorsCS *pipes)
{
    createPipe("pipeServiceToClient",&(pipes->OtoC));
    createPipe("pipeClientToService",&(pipes->CtoO));
}


static void destroyPipe(PipeClientService *onePipe)
{
    int ret;
    
    ret = unlink(onePipe->name);
    myassert(ret == 0, "echec unlink ");
    free(onePipe->name);
    onePipe->name = NULL;
}

void s_c_destroyPipes(DescriptorsCS *pipes)
{
    destroyPipe(&(pipes->OtoC));
    destroyPipe(&(pipes->CtoO));
}

void c_s_destroyPipes(DescriptorsCS *pipes)
{
    destroyPipe(&(pipes->OtoC));
    destroyPipe(&(pipes->CtoO));
}

/*
 * ouverture et fermeture d'un tube
 */
 
static void openPipe(const char *name, int flag,PipeClientService *onePipe)
{
    onePipe->name = strdup(name);
    onePipe->fd = open(onePipe->name, flag);
    myassert(onePipe->fd != -1, "echec open pipes ");
}

void c_s_openPipes(DescriptorsCS *pipes)
{
    openPipe(pipes->CtoO.name, O_WRONLY, &(pipes->CtoO));
    openPipe(pipes->OtoC.name, O_RDONLY,&(pipes->OtoC));
}

void s_c_openPipes(DescriptorsCS *pipes)
{
    openPipe(pipes->CtoO.name, O_RDONLY,&(pipes->CtoO)); 
    openPipe(pipes->OtoC.name, O_WRONLY,&(pipes->OtoC));
}

static void closePipe(PipeClientService *onePipe)
{
    int ret;
    
    free(onePipe->name);
    onePipe->name = NULL;
    ret = close(onePipe->fd);
    myassert(ret == 0, "echec fermeture");
    onePipe->fd = -1;
}

void c_s_closePipes(DescriptorsCS *pipes)
{
    closePipe(&(pipes->CtoO));
    closePipe(&(pipes->OtoC));
}

void s_c_closePipes(DescriptorsCS *pipes)
{
    closePipe(&(pipes->OtoC));
    closePipe(&(pipes->CtoO));
}


/*
 * surcharge des envois et réceptions (pour gérer les erreurs)
 */
 
static void writeData(PipeClientService *onePipe, const void *buf, int size)
{
    ssize_t ret = write(onePipe->fd, buf, size);
    myassert(ret != -1,"echec écriture de données");
    myassert((size_t)ret == size,"echec écriture de données");
}

void s_c_writeData(DescriptorsCS *pipes, const void *buf,int size)
{
    writeData(&(pipes->OtoC), buf, size);
}

void c_s_writeData(DescriptorsCS *pipes, const void *buf, int size)
{
    writeData(&(pipes->CtoO), buf, size);
}

static void readData(PipeClientService *onePipe, void *buf,int size)
{
    ssize_t ret = read(onePipe->fd, buf, size);
    myassert(ret != -1,"echec lecture de données");
    //myassert((size_t)ret == size, "echec lecture de données");
}

void s_c_readData(DescriptorsCS *pipes, void *buf, int size)
{
    readData(&(pipes->CtoO), buf, size);

}

void c_s_readData(DescriptorsCS *pipes, void *buf, int size)
{
    readData(&(pipes->OtoC), buf, size);
}


 


/***************************************************************************/






 int getPwdFromClient(int fifoFd)
 {
    int pwd;
    read(fifoFd,&pwd,sizeof(int));
    return pwd;
 }
 
 void sendErrorPwd(int fifoFd)
 {
    char* errorMsg = "INCORRECT PASSWORD";
    write(fifoFd,errorMsg,strlen(errorMsg));
 }
 
 void sendOkPwd(int fifoFd)
 {
    char* errorMsg = "CORRECT PASSWORD";
    write(fifoFd,errorMsg,strlen(errorMsg));
 }
 
 
bool clientAcknowledges(int fifoFd)
{
    char msg[100];// buffer large volontairement
    read(fifoFd,&msg,sizeof(char) * 100);
    return strstr(msg, "RESULT OK") != NULL;// on vérifie qu'il y a la chaîne RESULT OK dans msg
}


/*******************************************************/




#include "myassert.h"

#include "client_orchestre.h"





static void createPipe(const char *name, OnePipe *onePipe)
{
    int nameLength;

    nameLength = snprintf(NULL, 0, "%s", basename);
    onePipe->name = malloc((nameLength + 1) * sizeof(char));
    sprintf(onePipe->name, "%s", basename);
    onePipe->fd = -1;

    int ret = mkfifo(onePipe->name, 0600);
}

void o_c_createPipes(Descriptors *pipes)
{
    createPipe("pipeOrchestreToClient",O_WRONLY,&(pipes->OtoC));
    createPipe("pipeClientToOrchestre",O_RDONLY,&(pipes->CtoO));
}

void c_o_createPipes(Descriptors *pipes)
{
    createPipe("pipeOrchestreToClient",O_WRONLY,&(pipes->OtoC));
    createPipe("pipeClientToOrchestre",O_RDONLY,&(pipes->CtoO));
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
    destroyPipe("destruction tube OtoC", &(pipes->OtoC));
    destroyPipe("destruction tube CtoO", &(pipes->CtoO));
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
    openPipe(nameCtoO, O_WRONLY,
             "client ouverture tube CtoO", &(pipes->CtoO));
    openPipe(nameWtoM, O_RDONLY,
             "orchestre ouverture tube OtoC", &(pipes->OtoC));
}

void o_openPipes(const char *nameCtoO, const char *nameOtoC, Descriptors *pipes)
{
    openPipe(nameCtoO, O_RDONLY,
             "client ouverture tube CtoO", &(pipes->CtoO));
    openPipe(nameOtoC, O_WRONLY,
             "orchestre ouverture tube OtoC", &(pipes->OtoC));
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
    closePipe("client fermeture tube CtoO", &(pipes->CtoO));
    closePipe("client fermeture tube OtoC", &(pipes->OtoC));
}

void o_closePipes(Descriptors *pipes)
{
    closePipe("orchestre fermeture tube OtoC", &(pipes->OtoC));
    closePipe("orchestre fermeture tube CtoO", &(pipes->CtoO));
}


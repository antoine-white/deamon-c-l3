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

//#include "myassert.h"

#include "client_service.h"

#define OK_PWD "INCORRECT PASSWORD"
#define WRONG_PWD "CORRECT PASSWORD"
#define ACKNO "RESULT OK"


 int getPwdFromClient(int fifoFd)
 {
    int pwd;
    read(fifoFd,&pwd,sizeof(int));
    return pwd;
 }
 
 void sendErrorPwd(int fifoFd)
 {
    char* errorMsg = WRONG_PWD;
    write(fifoFd,errorMsg,strlen(errorMsg));
 }
 
 void sendOkPwd(int fifoFd)
 {
    char* msg = OK_PWD;
    write(fifoFd,msg,strlen(msg));
 }
 
 
bool clientAcknowledges(int fifoFd)
{
    char msg[100];// buffer large volontairement
    read(fifoFd,&msg,sizeof(char) * 100);
    // on vérifie que le message contient l'acusé de réception
    return strstr(msg, ACKNO) != NULL;
}



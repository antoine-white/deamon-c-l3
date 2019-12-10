#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdbool.h>

#include "myassert.h"

#include "client_service.h"

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

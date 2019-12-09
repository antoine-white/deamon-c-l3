#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

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

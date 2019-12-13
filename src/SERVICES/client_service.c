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

#define OK_PWD "CORRECT PASSWORD"
#define WRONG_PWD "INCORRECT PASSWORD"
#define ACKNO "RESULT OK"


int s_getPwdFromClient(int fifoFd)
{
	int pwd;
	read(fifoFd,&pwd,sizeof(int));
	return pwd;
}

void s_sendErrorPwd(int fifoFd)
{
	char* errorMsg = WRONG_PWD;
	write(fifoFd,errorMsg,strlen(errorMsg));
}

void s_sendOkPwd(int fifoFd)
{
	char* msg = OK_PWD;
	write(fifoFd,msg,strlen(msg) + 1);
}
 
bool c_pwdIsOK(int fifoFd)
{
	char msg[100];// buffer large volontairement
    read(fifoFd,&msg,sizeof(char) * 100);
	return strstr(msg, WRONG_PWD) == NULL;
}
 
void s_acknowledge(int fifoFd)
{
	write(fifoFd,ACKNO,sizeof(char) * (strlen(ACKNO + 1)));
}
 
bool c_acknowledge(int fifoFd)
{
    char msg[100];// buffer large volontairement
    read(fifoFd,&msg,sizeof(char) * 100);
    // on vérifie que le message contient l'acusé de réception
    return strstr(msg, ACKNO) != NULL;
}



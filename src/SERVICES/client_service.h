#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

#include <stdbool.h>

// Ici toutes les communications entre les services et les clients :
/*
 * Les fonctions côté :
 * - service sont préfixées par "s_",
 * - client sont préfixées par "c_",
 */

int s_getPwdFromClient(int fifoFd);
void s_sendErrorPwd(int fifoFd);
void s_sendOkPwd(int fifoFd); 
bool c_pwdIsOK(int fifoFd); 
void s_acknowledge(int fifoFd);
bool c_acknowledge(int fifoFd);

#endif

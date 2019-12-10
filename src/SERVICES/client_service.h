#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

#include <stdbool.h>

// Ici toutes les communications entre les services et les clients :
// - les deux tubes nommés pour la communication bidirectionnelle
int getPwdFromClient(int fifoFd);
void sendErrorPwd(int fifoFd);
void sendOkPwd(int fifoFd);
bool clientAcknowledges(int fifoFd);

#endif

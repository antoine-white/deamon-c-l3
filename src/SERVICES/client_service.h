#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

#include <stdbool.h>

// Ici toutes les communications entre les services et les clients :
// - les deux tubes nommés pour la communication bidirectionnelle

/*
 * Convention de codage :
 * - S pour Service
 * - C pour Client 
 *
 * par exemple :
 * - "nameCtoS" : nom du tube dans le sens client vers service
 *
 * Les fonctions côté :
 * - service sont préfixées par "s_",
 * - client sont préfixées par "c_",
 */

/*
 * Structures pour la paire de descripteurs
 */
  



int getPwdFromClient(int fifoFd);
void sendErrorPwd(int fifoFd);
void sendOkPwd(int fifoFd);
bool clientAcknowledges(int fifoFd);

#endif

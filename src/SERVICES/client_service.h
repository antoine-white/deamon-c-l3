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
typedef struct
{
    char *name;
    int fd;
} PipeClientService;

typedef struct {
    PipeClientService CtoO;
    PipeClientService OtoC;
} DescriptorsCS;

/*
 * création et destruction des tubes
 */
void s_c_createPipes(DescriptorsCS *pipes);
void c_s_createPipes(DescriptorsCS *pipes);
void s_destroyPipes(DescriptorsCS *pipes);
void c_destroyPipes(DescriptorsCS *pipes);

/*
 * ouverture et fermeture des tubes
 */
void c_openPipes(DescriptorsCS *pipes);
void s_openPipes(DescriptorsCS *pipes);

void c_closePipes(DescriptorsCS *pipes);
void s_closePipes(DescriptorsCS *pipes);


/*
 * surcharge des envois et réceptions (pour gérer les erreurs)
 */
void s_writeData(DescriptorsCS *pipes, const void *buf, int size);
void c_writeData(DescriptorsCS *pipes, const void *buf,int size);
void s_readData(DescriptorsCS *pipes, void *buf, int size);
void c_readData(DescriptorsCS *pipes, void *buf, int size);




int getPwdFromClient(int fifoFd);
void sendErrorPwd(int fifoFd);
void sendOkPwd(int fifoFd);
bool clientAcknowledges(int fifoFd);

#endif

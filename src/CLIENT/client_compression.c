#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "client_service.h"
#include "client_compression.h"


/*----------------------------------------------*
 * fonctions appelables par le client
 *----------------------------------------------*/

// fonction d'envoi des données du client au service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv et en déduit les données à envoyer
void client_compression_sendData(int fifofd, int argc, char * argv[])
{
    //TODO : analyse argv et en déduit les données à envoyer
    char* str = "HELLLLLO THERE";
    
    int length = strlen(str);
    
    write(fifofd,&length,sizeof(int));
    write(fifofd,str,sizeof(char) * length);
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_compression_receiveResult(int fifofd, int argc, char * argv[])
{
    //TODO : analyse argv et en déduit les données à envoyer
    int length;
    read(fifofd,&length,sizeof(int));
    char* result = (char *)malloc(sizeof(char) * (length + 1));
    read(fifofd, result ,sizeof(char) * length);
    result[length - 1] = '\0';
    printf("result : %s",result);
}

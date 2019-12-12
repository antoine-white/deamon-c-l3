#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "client_service.h"
#include "client_compression.h"


/*----------------------------------------------*
 * fonctions appelables par le client           *
 *----------------------------------------------*/

// fonction d'envoi des données du client au service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv et en déduit les données à envoyer
void client_max_sendData(int fifofd, int argc, char * argv[])
{
    int length = 4;
    float arr[4] = {1.2,2.5,2.3,2.3};
    write(fifofd,&length,sizeof(int));
    write(fifofd,&arr,sizeof(float) * length);
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_max_receiveResult(int fifofd, int argc, char * argv[])
{
    // par exemple on décide d'afficher le résultat et argv[3] contient
    // une chaine à afficher avant le résultat
}

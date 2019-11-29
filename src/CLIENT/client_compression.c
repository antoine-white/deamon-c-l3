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
void client_compression_sendData(/* tubes,*/ int argc, char * argv[])
{
    // par exemple argv[2] est le nom du fichier contenant le texte
    // à envoyer pour compression
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_compression_receiveResult(/* tubes,*/ int argc, char * argv[])
{
    // par exemple on décide de sauvegarder le résultat dans un fichier et
    // argv[3] est le nom du fichier où écrire le texte compressé
}

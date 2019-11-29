#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "client_service.h"
#include "client_somme.h"


/*----------------------------------------------*
 * fonctions appelables par le client
 *----------------------------------------------*/

// fonction d'envoi des données du client au service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv et en déduit les données à envoyer
void client_somme_sendData(/* tubes,*/ int argc, char * argv[])
{
    // par exemple argv[2] et argv[3] contiennent les deux nombres
    // à envoyer (pour les sommer)
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_somme_receiveResult(/* tubes,*/ int argc, char * argv[])
{
    // par exemple on décide d'afficher le résultat et argv[4] contient
    // une chaine à afficher avant le résultat
}

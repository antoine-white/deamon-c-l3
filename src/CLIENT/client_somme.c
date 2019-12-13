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
void client_somme_sendData(int fifofd, int argc, char * argv[])
{
	float f1 = 1.25, f2 = 53.20;
	write(fifofd,&f1,sizeof(float));
	write(fifofd,&f2,sizeof(float));
    // par exemple argv[2] et argv[3] contiennent les deux nombres
    // à envoyer (pour les sommer)
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_somme_receiveResult(int fifofd, int argc, char * argv[])
{
	float res;
	read(fifofd,&res,sizeof(float));	
	printf("resultat => %f\n ",res);
    // par exemple on décide d'afficher le résultat et argv[4] contient
    // une chaine à afficher avant le résultat
}

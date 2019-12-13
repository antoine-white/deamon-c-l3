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
	float f1 = atof(argv[2]), f2 = atof(argv[3]);
	write(fifofd,&f1,sizeof(float));
	write(fifofd,&f2,sizeof(float));
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
	printf("resultat %s %f\n ",argv[4],res);
}

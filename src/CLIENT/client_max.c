#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>

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
    srand(time(NULL));
    int length = atoi(argv[2]);    
    write(fifofd,&length,sizeof(int));
    float tmp;
    for(int i = 0; i < length ; i++){
        tmp = (float)rand()/(float)(RAND_MAX/FLT_MAX);
        write(fifofd,&tmp,sizeof(float));
    }
}

// fonction de réception des résultats en provenance du service
// Les paramètres sont
// - les tubes de communication
// - argc et argv fournis au main
// Cette fonction analyse argv pour savoir quoi faire des résultats
void client_max_receiveResult(int fifofd, int argc, char * argv[])
{
	float max;
	read(fifofd,&max,sizeof(float));
	printf("resultat %s %f\n",argv[3],max);
}

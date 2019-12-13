#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

#include "client_orchestre.h"
#include "client_service.h"

#include "client_somme.h"
#include "client_compression.h"
#include "client_max.h"

#include "../UTILS/myassert.h"

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <num_service> ...\n", exeName);
    fprintf(stderr, "        ... : les paramètres propres au service\n");
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


// TEMP 



void sendData(DescriptorsCO *pipes, int *nbService)
{
    
   c_writeData(pipes, &nbService, sizeof(int));
}

void receiveResults(DescriptorsCO *pipes)
{
    int n;
    c_readData(pipes, &n, sizeof(int));
	// à faireeeeeeeeeeeeeeeeeeeeeeeeeeee
}


///////////
int main(int argc, char * argv[])
{
    if (argc < 2)
        usage(argv[0], "nombre paramètres incorrect");

    int numService = strtol(argv[1], NULL, 10);

    // initialisations diverses
    DescriptorsCO pipesCO;
    int codeRetour; 
    int motDePasse; 
    	
    printf("J'ouvre le pipe vers l'orchestre\n"); fflush(stdout);  
    
    	
    c_openPipes(&pipesCO);
    
     
    
    
    
   /* receiveResults(&pipesCO);
    	printf("Résultats reçus \n"); fflush(stdout); 
    c_closePipes(&pipesCO);
    	printf("Je ferme le Pipe \n"); fflush(stdout); */
	
    // entrée en section critique pour communiquer avec l'orchestre
    
    
    // envoi à l'orchestre du numéro du service
	
    //sendData(&pipes,&numService);
    printf("J'envoie le numéro de service à ouvrir -> %d\n",numService); 
    c_writeData(&pipesCO, &numService, sizeof(int));
    
    printf("J'attend de recevoir les résultats \n");
   
    
    // attente code de retour
    c_readData(&pipesCO,&codeRetour,sizeof(int)); 
    printf("J'ai reçu le résultat : %d \n",codeRetour); 
    if(codeRetour == 0){
    	
    	printf("l'orchestre s'est arreté, je m'arrete également : %d \n",codeRetour); 
	
    } else if ( codeRetour == -1){
    
    	printf("Le service demandé n'est pas disponible  : %d \n",codeRetour);
    	  
    } else if (codeRetour == 1){
    	
    	printf("L'orchestre m'indique que le service est disponible\n");
    	c_readData(&pipesCO,&motDePasse,sizeof(int)); 	
    	printf("Le code reçu est : %d \n",motDePasse); 
    	
    	// on reçoit les tubes nommés
    	char serviceToClient[6];
    	char ClientToService[6];
    	c_readData(&pipesCO,serviceToClient,sizeof(char) * 6); 	
    	c_readData(&pipesCO,ClientToService,sizeof(char) * 6); 	
    	printf("J'ai reçu les deux pipes : %s et %s \n",serviceToClient,ClientToService); 
    	
    	// on ouvre les tubes nommés 
    	int StoCfd = open(serviceToClient, O_RDONLY); 
    	myassert(StoCfd != 1,"erreur ouverture des tubes");
    	int CtoSfd = open(ClientToService, O_WRONLY); 
    	myassert(CtoSfd != 1,"erreur ouverture des tubes");    	
    	printf("Ouverture des tubes nommés vers le service \n");
    	
    	// envoie du mot de passe
    	write(CtoSfd,&motDePasse,sizeof(int));
    	
    	if(c_pwdIsOK(StoCfd)){
    		switch(numService){
    			case 1 : 
	    			client_somme_sendData(CtoSfd,1,(char*)NULL);
	    			client_somme_receiveResult(StoCfd,1,(char*)NULL);
	    			break;
    			case 3 : 
	    			client_max_sendData(CtoSfd,1,(char*)NULL);
	    			client_max_receiveResult(StoCfd,1,(char*)NULL);
	    			break;
    		}
    		s_acknowledge(CtoSfd);
    		close(StoCfd); 
			close(CtoSfd); 
    		
    	}    	
    }
    
    
    // si code d'erreur
    //     afficher l'erreur
    //     sortie de la section critique
    // sinon
    //     récupération du mot de passe et des noms des 2 tubes
    //     envoi d'une accusé de réception à l'orchestre
    //     sortie de la section critique
    //     envoi du mot de passe au service
    //     attente de l'accusé de réception du service
    //     appel de la fonction d'envoi des données (une fct par service)
    //     appel de la fonction de réception du résultat (une fct par service)
    //     envoi d'un accusé de réception au service

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

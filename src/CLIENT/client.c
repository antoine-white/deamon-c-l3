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

 /***********************************************/


void receiveResults(DescriptorsCO *pipes)
{
    int n;
    c_readData(pipes, &n, sizeof(int));
	// à faireeeeeeeeeeeeeeeeeeeeeeeeeeee
}

f
int main(int argc, char * argv[])
{
    if (argc < 2)
        usage(argv[0], "nombre paramètres incorrect");

    int numService = strtol(argv[1], NULL, 10);

    // initialisations diverses
    DescriptorsCO pipesCO;
    int codeRetour; 
    int motDePasse; 
    int accuseReception = 1;
    //int semClient = c_o_sem_recup(); // récuperation du sémaphore avec l'orchestre
    	
    printf("J'ouvre le pipe vers l'orchestre\n"); fflush(stdout);  
    
    	
    c_openPipes(&pipesCO);
	
    // entrée en section critique pour communiquer avec l'orchestre
    
    //c_o_sem_prendre(semClient); // on rentre en section critique ici
    
    // envoi à l'orchestre du numéro du service
	
    printf("J'envoie le numéro de service à ouvrir -> %d\n",numService); 
    c_writeData(&pipesCO, &numService, sizeof(int));
    
    printf("J'attend de recevoir les résultats \n");
   
    
    // attente code de retour
    c_readData(&pipesCO,&codeRetour,sizeof(int)); 
    printf("J'ai reçu le résultat : %d \n",codeRetour); 
    if(codeRetour == 0){
    	// si code d'erreur
    	// afficher l'erreur
    	// sortie de la section critique
    	//c_o_sem_vendre(semClient);
    	printf("l'orchestre s'est arreté, je m'arrete également : %d \n",codeRetour); 
	
    } else if ( codeRetour == -1){
    
    	printf("Le service demandé n'est pas disponible  : %d \n",codeRetour);
    	  
    } else if (codeRetour == 1){
    	// sinon
    	// récupération du mot de passe et des noms des 2 tubes
    	// envoi d'un accusé de réception à l'orchestre
    	
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
    	
    	// envoi du mot de passe au service
    	write(CtoSfd,&motDePasse,sizeof(int));
    	
    	// accusé de réception à l'orchestre 
    	c_writeData(&pipesCO, &accuseReception, sizeof(int));
    	
    	//sortie de la section critique
    	//c_o_sem_vendre(semClient);
    	
    	// on communique maintenant avec le service 
    	if(c_pwdIsOK(StoCfd)){
    		switch(numService){
    			case 1 : 
	    			client_somme_sendData(CtoSfd,argc,argv);
	    			client_somme_receiveResult(StoCfd,argc,argv);
	    			break;
	    		case 2 :
    	    		client_compression_sendData(CtoSfd,argc,argv);    
	    			client_compression_receiveResult(StoCfd,argc,argv);
	    		    break;
	    		case 3 : 
	    			client_max_sendData(CtoSfd,argc,argv);
	    			client_max_receiveResult(StoCfd,argc,argv);
	    			break;
    		}
    		s_acknowledge(CtoSfd);
    		close(StoCfd); 
			close(CtoSfd); 
    		
    	}    	
    }
    c_closePipes(&pipesCO);   // fermeture du tube
    
    
    
    //     sortie de la section critique
    //     envoi du mot de passe au service
    //     attente de l'accusé de réception du service
    //     appel de la fonction d'envoi des données (une fct par service)
    //     appel de la fonction de réception du résultat (une fct par service)
    //     envoi d'un accusé de réception au service

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

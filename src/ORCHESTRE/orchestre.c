#define _XOPEN_SOURCE // pour eviter un warning

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "config.h"
#include "myassert.h"
#include "client_orchestre.h"
#include "service_orchestre.h"


 // Structure d'un service 
struct Service{
    char* name;  // nom du service 
    bool isOpen; // boolean pour vérifier s'il est ouvert 
    int pipefd[2]; // tubes anonymes 
    key_t semKey;
    int sem;
    char serviceToClient[6]; // nom tube service vers client  
    char clientToService[6]; // nom tube client vers service 
};


typedef struct Service Service;

/***********************************************/

static Service* initServices(const char* configPath, int* nb)
{
    // on ouvre l'API de lecture du fichier config
    config_init(configPath);
    int nbService = config_getNbServices();
    Service* services = (struct Service *)malloc(sizeof(struct Service)*nbService);
    for(int i = 0; i < nbService;i++)
    {
        const int ftokId = 456 * (i+1); // clé aléatoire 
        services[i].isOpen = true;
        services[i].name = malloc(sizeof(char) * strlen(config_getExeName(i+1)));
        strcpy(services[i].name,config_getExeName(i+1));
        myassert(pipe(services[i].pipefd) != -1,"erreur dans la création de tube anonyme");
        
        sprintf(services[i].serviceToClient,"StoC%d",i+1);
        sprintf(services[i].clientToService,"CtoS%d",i+1);
               
        services[i].semKey = ftok("./",ftokId);
    }
    // on ferme l'API
    config_exit();
    *nb = nbService;
    return services;
}

/***********************************************/

// lancement des services, avec pour chaque service :
// - création d'un tube anonyme pour converser (orchestre vers service)
// - un sémaphore pour que le service préviene l'orchestre de la
//   fin d'un traitement 
static void lauchServices(int nbService, Service* services)
{
	
    pid_t res;
    for(int i = 0; i < nbService;i++)
    {        
        res = fork();
        myassert(res != -1,"fork de orchestre"); // on test que le fork ait marché

        //fils 
        if (res == 0) 
        {   
            //creation des arguments du service
            char* args[6]; 
            
            char buff1[12];
            sprintf(buff1, "%d", services[i].semKey);// only the reading end of the pipe           
            args[1] = buff1;

            // création du sémaphore pour le service
            services[i].sem = semget(services[i].semKey,1,IPC_CREAT | IPC_EXCL | S_IRUSR |S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            myassert(services[i].sem != -1,"erreur création de semaphore");
            
            // transforme entier en chaîne de caractère  
            char buff2[12];
            sprintf(buff2, "%d %d", services[i].pipefd[0],services[i].pipefd[1]);// only the reading end of the pipe
            
            args[2] = buff2;
            
            // - création de deux tubes nommés pour les communications entre
            //   les clients et le service   
            myassert(mkfifo(services[i].serviceToClient, 0666) != -1,"erreur creation de tube nomme");
            myassert(mkfifo(services[i].clientToService, 0666) != -1,"erreur creation de tube nomme"); 
            args[3] = services[i].serviceToClient;            
            args[4] = services[i].clientToService;
            args[5] = NULL;                 
            
            int length = strlen(services[i].name);
            char* execPath = (char *) malloc(sizeof(char) * (length + 4)); //4 pour  "../" et '\0'
            strcpy(execPath,"./");
            strcat(execPath,services[i].name);
            execPath[length + 3] = '\0';
            printf("%s, [%s,%s]\n\n",execPath,args[1],args[2]);   
            args[0] = services[i].name;
            execvp(execPath,args);
            myassert(false,"erreur exec");// si le exevcp n'a pas marché
        }    
        // on ne fait rien sur le père
    } 
}

/***********************************************/

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

/***********************************************/

int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], "nombre paramètres incorrect");

    // lecture du fichier de configuration
    int nbService;
    Service* services = initServices(argv[1], &nbService);
   
    // Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients  

    DescriptorsCO pipesCO;
         
    o_c_createPipes(&pipesCO); // CREATION PIPE 
    printf("Je crée le Pipe\n"); fflush(stdout); 

    
		
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre
    
    //int semClient = c_o_sem_init(); 
    
    
    // lancement des services : 
    lauchServices(nbService,services);
    
    for(int i = 0; i < nbService;i++)
    	close(services[i].pipefd[0]);
    
    
    
    int demandeService;  // entier de numéro de service envoyé par le client 
    int codeToClient; // code à renvoyer au client 
    int codeToService; // code à renvoyer au service 
    while (true)
    {
               
        // attente d'une demande de service du client
        
        	
         printf("J'ouvre le pipe avec le client et attend \n"); 
   		 o_openPipes(&pipesCO); // ouverture du piper avec le client 
    	 printf("Je lis les données envoyées par le client \n");
    	 o_readData(&pipesCO,&demandeService,sizeof(int));  // lecture du numéro envoyé par le client
    	 printf("je reçois le numéro : %d\n",demandeService); 
  	
			 
        
        // détecter la fin des traitements lancés précédemment via
        // les sémaphores dédiés (attention on n'attend pas la
        // fin des traitement, on note juste ceux qui sont finis)
    
        // analyse de la demande du client
        // si ordre de fin
        //     retour d'un code d'acceptation
        //     sortie de la boucle
       if(demandeService == 0){  // si entier reçu est 0, ordre de fin 
        
        	codeToClient = 0; 
        	o_writeData(&pipesCO, &codeToClient, sizeof(int)); // on renvoie 0 au client pour lui faire savoir que l'orchestre s'est arreté 
        	printf(" le numéro reçu étant 0, je m'arrete \n"); 
        	break; 
        	
        } else if (nbService < demandeService ){ // si la demande n'est pas compris entre 0 et 3
        
        	printf(" le numéro reçu n'est pas compris entre 1 et 3 , je m'arrete \n"); 
        	break;
        	
        } else if(services[demandeService-1].isOpen == false){ 
        	// sinon si service non ouvert
       		// retour d'un code d'erreur
       		codeToClient = -1;  // Code d'erreur  
        	o_writeData(&pipesCO, &codeToClient, sizeof(int)); // envoi du code d'erreur au client
        	printf("le service demandé n'est pas ouvert\n"); 
        } /* else if ( service non disponible)  {
        	
        	// sinon si service déjà en cours de traitement
        	// retour d'un code d'erreur
        
        }*/
        else { // dernier cas si le service demandé est disponible
        	int code = 20; // code = 20 pour tester 
        	codeToService = 1; //code service pour signaler qu'il va travailler 
        	codeToClient = 1; // code client pour signaler que c'est ok
        	int Reception; 
        	
        	// envoi d'un message indiquant au service qu'il va devoir travailler 
        	write(services[demandeService-1].pipefd[1],&codeToService,sizeof(int));
        	
        	
        	// envoi du code au service 
      		write(services[demandeService-1].pipefd[1],&code,sizeof(int));
      		printf("J'envoie maintenant un code au service demandé \n"); 
      		
      		// envoi du numéro 1 au client 
      		o_writeData(&pipesCO, &codeToClient, sizeof(int));
      		
      		
      		// envoi du code au client
      		o_writeData(&pipesCO, &code, sizeof(int));
      		printf("J'envoie maintenant le meme code au client \n"); 
      		
      		
      		
   			//envoie des noms des tubes nommés au client
   			o_writeData(&pipesCO, services[demandeService-1].serviceToClient, sizeof(char) * (strlen(services[demandeService-1].serviceToClient)+1));
   			o_writeData(&pipesCO, services[demandeService-1].clientToService, sizeof(char) * (strlen(services[demandeService-1].clientToService)+1));
      		printf("J'envoie maintenant les noms des tubes nommés au client \n"); 
      		
      		o_readData(&pipesCO,&Reception,sizeof(int)); // accusé de reception du client  
        	
        	
        // sinon
        //     génération d'un mot de passe
        //     envoi d'un code de travail au service (via le tube anonyme)
        //     envoi du mot de passe au service (via le tube anonyme)
        //     envoi au client d'un code d'acceptation (via le tube nommé)
        //     envoi du mot de passe au client (via le tube nommé)
        //     envoi des noms des tubes nommés au client (via le tube nommé)
        // attente d'un accusé de réception du client
        
        
        
        
        }
        //c_o_sem_wait(semClient); 
        o_closePipes(&pipesCO);       
           
    }
    
    //c_o_sem_destroy(semClient);
    //

    // attente de la fin des traitements en cours (via les sémaphores)

    // envoi à chaque service d'un code de fin

    // attente de la terminaison des processus services

    // destruction des tubes
    
    return EXIT_SUCCESS;
}


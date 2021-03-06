#define _GNU_SOURCE  // eviter un warning 

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

#include "service_orchestre.h"
#include "client_service.h"

#include "../UTILS/myassert.h"

// structure des données 
struct data{
    float a;
    float b;
    float res;
};

typedef struct data Data;

/***********************************************/

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <clé_sémaphore> <fd_tube_anonyme> "
            "<tube_service_vers_client> <tube_client_vers_ service>\n",
            exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/

// fonction de réception des données
void somme_service_receiveDataData(int fifoFd, Data* d)
{
	// lectures des données dans les tubes 
    read(fifoFd,&(d->a),sizeof(float));
    read(fifoFd,&(d->b),sizeof(float));
}

/***********************************************/

// fonction de traitement des données
void somme_service_computeResult(Data* d)
{
	// somme des données 
    d->res = d->a + d->b;
}

/***********************************************/

// fonction d'envoi du résultat
void somme_service_sendResult(int fifoFd, Data d)
{
	// écriture du résultat dans le tube 
    write(fifoFd,&(d.res),sizeof(float));
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
 
int main(int argc, char * argv[])
{
    if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

        
    // initialisations diverses
    // la clé du séaphore 
    key_t semKey = (key_t) atoi(argv[1]);
    
    // le tube anonyme 
    char * splittedStr;
    splittedStr = strtok(argv[2]," ,.-");
    int i = 0, pipefd[2];    
    while (splittedStr != NULL)
    {
        pipefd[i] = atoi(splittedStr);
        splittedStr = strtok (NULL, " ,.-");
        i++;
    }
    close(pipefd[1]);
     
    
      
    const int endCode = -1;
    const int newClient = 1;
    while (true)
    {        
        // attente d'un code de l'orchestre (via tube anonyme)
        
        int code; // code qui va indiquer au service de travailler 
        read(pipefd[0],&code,sizeof(int)); 
        printf("			JE SUIS LE SERVICE \n");  
        printf("Je reçois un code de l'orchestre \n");
        
        // si code de fin
        //    sortie de la boucle
        if(endCode == code)
        {
            break;
        }        
        else if(code == newClient)
        {
        	int password;  
        	// réception du mot de passe de l'orchestre
            read(pipefd[0],&password,sizeof(int));  
            printf("Le mot de passe envoyé par l'orchestre est => %d \n",password);
           
            //tube nommé entre service et client
			int StoCfd = open(argv[3], O_WRONLY); 
			myassert(StoCfd != 1,"erreur ouverture des tubes");
			//tube nommé entre client et service
			int CtoSfd = open(argv[4], O_RDONLY); 
			myassert(CtoSfd != 1,"erreur ouverture des tubes");    	
			printf("Ouverture des tubes nommés vers le client \n");
						
						
            //    attente du mot de passe du client
			int pwdClient = s_getPwdFromClient(CtoSfd);
			printf("mot de passe client : %d\n",pwdClient);
            if(pwdClient == password)
            {
                Data* d = malloc(sizeof(Data));
                //envoi au client d'un code d'acceptation
                s_sendOkPwd(StoCfd);
                //réception des données du client (une fct par service)
                somme_service_receiveDataData(CtoSfd,d);
                //calcul du résultat (une fct par service)
                somme_service_computeResult(d);
                //envoi du résultat au client (une fct par service)
                somme_service_sendResult(StoCfd,*d);
                //attente de l'accusé de réception du client
                c_acknowledge(CtoSfd);// on utilise pas le résultat pour l'instant 
				// on ferme les pipes 
				close(StoCfd); 
				close(CtoSfd); 
                // TODO :modification du sémaphore pour prévenir l'orchestre de la fin
            }
            // si mot de passe incorrect
            // envoi au client d'un code d'erreur
            else 
            {
                s_sendErrorPwd(StoCfd);
            }
            
        }
       
    }

    // TODO :libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

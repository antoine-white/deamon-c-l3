#define _XOPEN_SOURCE//to avoid warning

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

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <clé_sémaphore> <fd_tube_anonyme> "
            "<tube_service_vers_client> <tube_client_vers_ service>\n",
            exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

struct data{
    float a;
    float b;
    float res;
};


typedef struct data Data;

/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/

// fonction de réception des données
void max_service_receiveDataData(/* tubes,*/ /* autre chose ? */)
{
    
}

// fonction de traitement des données
void max_service_computeResult(/* autre chose ? */)
{
}

// fonction d'envoi du résultat
void max_service_sendResult(/* tubes,*/ /* autre chose ? */)
{
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

    // TEMP:
/*    printf("%s ; %s ;%s ;%s\n",argv[1],argv[2],argv[3],argv[4]);
    
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
    
    //tube nommé entre service et client
    int sToc = atoi(argv[3]);
    
    //tube nommé entre client et service
    int cTos = atoi(argv[4]);
    
    const int endCode = -1;
    const int newClient = 1;
    while (true)
    {        
        // attente d'un code de l'orchestre (via tube anonyme)
        int code;
        read(pipefd[0],&code,sizeof(int));  
        printf("code => %d \n",code);
        
        // si code de fin
        //    sortie de la boucle
        if(endCode == code)
        {
            break;
        }        
        else if(code == newClient)
        {
            read(pipefd[0],&code,sizeof(int));  
            printf("code => %d \n",code);
            // réception du mot de passe de l'orchestre
            const int password = code;
            //    attente du mot de passe du client
            if(getPwdFromClient(cTos) == password)
            {
                Data* d = NULL;
                //envoi au client d'un code d'acceptation
                sendOkPwd(sToc);
                //réception des données du client (une fct par service)
                somme_service_receiveDataData(cTos,d);
                //calcul du résultat (une fct par service)
                somme_service_computeResult(*d);
                //envoi du résultat au client (une fct par service)
                somme_service_sendResult(sToc,*d);
                //attente de l'accusé de réception du client
                clientAcknowledges(cTos);// on utilise pas le résultat pour l'instant  
                // TODO :modification du sémaphore pour prévenir l'orchestre de la fin
            }
            // si mot de passe incorrect
            // envoi au client d'un code d'erreur
            else 
            {
                sendErrorPwd(sToc);
            }
            
        }
       
    }*/

    // TODO :libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

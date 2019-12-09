#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

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


/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/

// fonction de réception des données
void somme_service_receiveDataData(/* tubes,*/ /* autre chose ? */)
{
    
}

// fonction de traitement des données
void somme_service_computeResult(/* autre chose ? */)
{
}

// fonction d'envoi du résultat
void somme_service_sendResult(/* tubes,*/ /* autre chose ? */)
{
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

    // initialisations diverses
    printf("%s ; %s ;%s ;%s",argv[1],argv[2],argv[3],argv[4]);
    
    char * splittedStr;
    splittedStr = strtok(argv[2]," ,.-");
    int i = 0, pipefd[2];
    
    while (splittedStr != NULL)
    {
        pipefd[i] = atoi(splittedStr);
        splittedStr = strtok (NULL, " ,.-");
        i++;
    }
    //printf("%d %d\n",pipefd[0],pipefd[1]);
    close(pipefd[1]);
    while (true)
    {
        
        // attente d'un code de l'orchestre (via tube anonyme)
        int code;
        read(pipefd[0],&code,sizeof(int));  
        printf("code =>%d \n",code);
        close(pipefd[0]);
        break;
        // si code de fin
        //    sortie de la boucle
        // sinon
        //    réception du mot de passe de l'orchestre
        //    attente du mot de passe du client
        //    si mot de passe incorrect
        //        envoi au client d'un code d'erreur
        //    sinon
        //        envoi au client d'un code d'acceptation
        //        réception des données du client (une fct par service)
        //        calcul du résultat (une fct par service)
        //        envoi du résultat au client (une fct par service)
        //        attente de l'accusé de réception du client
        //    modification du sémaphore pour prévenir l'orchestre de la fin
    }

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

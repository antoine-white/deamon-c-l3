#define _XOPEN_SOURCE // pour eviter un warning

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "config.h"
#include "myassert.h"
#include "client_orchestre.h"
#include "service_orchestre.h"



struct Service{
    char* name;
    bool isOpen;
    int pipefd[2];
    int semKey;
};

typedef struct Service Service;


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], "nombre paramètres incorrect");

    // lecture du fichier de configuration
    const char* configPath = argv[1]; 
    // on ouvre l'API de lecture du fichier config
    config_init(configPath);
    int nbService = config_getNbServices();
    Service* services = (struct Service *)malloc(sizeof(struct Service)*nbService);
    const int ftokId = 4568123;
    for(int i = 0; i < nbService;i++)
    {
        services[i].isOpen = config_isServiceOpen(i+1);
        services[i].name = malloc(sizeof(char) * strlen(config_getExeName(i+1)));
        strcpy(services[i].name,config_getExeName(i+1));
        myassert(pipe(services[i].pipefd) != -1,"erreur dans la création de tube anonyme");
        services[i].semKey = ftok("./",ftokId);
    }
    // on ferme l'API
    config_exit();
    
    // Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre
    
    // lancement des services, avec pour chaque service :
    // - création d'un tube anonyme pour converser (orchestre vers service)
    // - un sémaphore pour que le service préviene l'orchestre de la
    //   fin d'un traitement 
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
            args[0] = argv[0]; 
            
            char buff1[12];
            sprintf(buff1, "%d", services[i].semKey);// only the reading end of the pipe           

            args[1] = buff1;
            
            // transform int ot string 
            char buff2[12];
            sprintf(buff2, "%d", services[i].pipefd[1]);// only the reading end of the pipe
            
            args[2] = buff2;
            
            //TEMP :
            args[3] = NULL;            
            args[4] = NULL;          
            
            args[5] = NULL;                    
            execvp(services[i].name,args);
            myassert(false,"erreur exec");// si le exevcp n'a pas marché
        }    
        // on ne fait rien sur le père
    
    }
    // - création de deux tubes nommés pour les communications entre
    //   les clients et le service
    
   
    
    while (true)
    {
        // attente d'une demande de service du client

        // détecter la fin des traitements lancés précédemment via
        // les sémaphores dédiés (attention on n'attend pas la
        // fin des traitement, on note juste ceux qui sont finis)

        // analyse de la demande du client
        // si ordre de fin
        //     retour d'un code d'acceptation
        //     sortie de la boucle
        // sinon si service non ouvert
        //     retour d'un code d'erreur
        // sinon si service déjà en cours de traitement
        //     retour d'un code d'erreur
        // sinon
        //     génération d'un mot de passe
        //     envoi d'un code de travail au service (via le tube anonyme)
        //     envoi du mot de passe au service (via le tube anonyme)
        //     envoi au client d'un code d'acceptation (via le tube nommé)
        //     envoi du mot de passe au client (via le tube nommé)
        //     envoi des noms des tubes nommés au client (via le tube nommé)
        // attente d'un accusé de réception du client
    }

    // attente de la fin des traitements en cours (via les sémaphores)

    // envoi à chaque service d'un code de fin

    // attente de la terminaison des processus services

    // destruction des tubes
    
    return EXIT_SUCCESS;
}

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



struct Service{
    char* name;
    bool isOpen;
    int pipefd[2];
    key_t semKey;
    int sem;
    char serviceToClient[6];
    char clientToService[6];
};

typedef struct Service Service;

static Service* initServices(const char* configPath, int* nb)
{
    // on ouvre l'API de lecture du fichier config
    config_init(configPath);
    int nbService = config_getNbServices();
    Service* services = (struct Service *)malloc(sizeof(struct Service)*nbService);
    for(int i = 0; i < nbService;i++)
    {
        //TEMP 
        const int ftokId = 456 * (i+1); 
        services[i].isOpen = config_isServiceOpen(i+1);
        services[i].name = malloc(sizeof(char) * strlen(config_getExeName(i+1)));
        strcpy(services[i].name,config_getExeName(i+1));
        myassert(pipe(services[i].pipefd) != -1,"erreur dans la création de tube anonyme");
        close(services[i].pipefd[0]);
        services[i].semKey = ftok("./",ftokId);
    }
    // on ferme l'API
    config_exit();
    *nb = nbService;
    return services;
}

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
            
            // transform int to string 
            char buff2[12];
            sprintf(buff2, "%d %d", services[i].pipefd[0],services[i].pipefd[1]);// only the reading end of the pipe
            
            args[2] = buff2;
            
            // - création de deux tubes nommés pour les communications entre
            //   les clients et le service   
            
            sprintf(services[i].serviceToClient,"StoC%d",i);
            sprintf(services[i].clientToService,"CtoS%d",i);
            myassert(mkfifo(services[i].serviceToClient, 0666) != -1,"erreur creation de tube nomme");
            myassert(mkfifo(services[i].clientToService, 0666) != -1,"erreur creation de tube nomme"); 
            args[3] = services[i].serviceToClient;            
            args[4] = services[i].clientToService;          
            
            args[5] = NULL;                 
            
            int length = strlen(services[i].name);
            char* execPath = (char *) malloc(sizeof(char) * (length + 4)); //4 pour  "../" et '\0'
            strcpy(execPath,"../");
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
    int nbService;
    Service* services = initServices(argv[1], &nbService);
    
    // Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients  
    
    /*
    char* fifoClient1 = "client1"; 
    char* fifoClient2 = "client2"; 
    myassert(mkfifo(fifoClient1, 0666) != -1,"erreur creation de tube nomme");
    myassert(mkfifo(fifoClient2, 0666) != -1,"erreur creation de tube nomme"); */
    Descriptors pipes;
        
    o_c_createPipes(&pipes);
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre
    
    /*
    int semClient = semget(IPC_PRIVATE,1,0666);
    myassert(semClient != -1,"erreur création de semaphore");*/
    int sem = sem_init(); 
    
    
    // lancement des services, avec pour chaque service :
    // - création d'un tube anonyme pour converser (orchestre vers service)
    // - un sémaphore pour que le service préviene l'orchestre de la
    //   fin d'un traitement 
    lauchServices(nbService,services);
    
    // TEMP :
    /*for(int i = 0; i < nbService;i++)
    {
        semctl(services[i].sem,-1,IPC_RMID);        
        unlink(services[i].serviceToClient); 
        unlink(services[i].clientToService);
    }
    unlink(fifoClient1); 
    unlink(fifoClient2); */
    while (true)
    {
        int code = 10;
        write(services[0].pipefd[1], &code, sizeof(int)); 
        write(services[1].pipefd[1], &code, sizeof(int)); 
        write(services[2].pipefd[1], &code, sizeof(int)); 
        break;
        /*close(services[0].pipefd[1]);
        close(services[1].pipefd[1]);
        close(services[2].pipefd[1]);*/
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


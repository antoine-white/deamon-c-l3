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

// structure des données 
struct data{
    int length;
    char *str;
    char *res;
};

typedef struct data Data;

/***********************************************/

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <clé_sémaphore> <fd_tube_anonyme> "
            "<tube_service_vers_client> <tube_client_vers_service>\n",
            exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/
 
// fonction de réception des données
void comp_service_receiveDataData(int fifoFd, Data* d)
{
	// lectures des données dans les tubes
    read(fifoFd,&(d->length),sizeof(int));
    read(fifoFd,d->str,sizeof(char) * d->length);
}

/***********************************************/

// fonction de traitement des données
void comp_service_computeResult(Data d)
{
    int streak = 1, resPos = 0;
    char currentChar = d.str[0];
    // pire cas; on pourrait économiser de la mémoire avec un realloc
    // mais on aurait perdu en temps d'éxecution 
    d.res = (char *)malloc(sizeof(char) * 2 * d.length);
    for( int i = 1 ; i < d.length ; i++)
    {
        if (currentChar == d.str[i]){
            streak++;
        } else {
            // transforme entier streak en char[]
            int bufferLength = (snprintf(NULL, 0, "%d", streak)) * sizeof(char);
            char buffer[bufferLength];
            sprintf(buffer, "%d", streak);
            for(int j = 0; j < bufferLength ; j++)
            {
                d.res[resPos] = buffer[j];
                resPos++;
            }
            
            d.res[resPos] = currentChar;
            resPos++;
            
            currentChar = d.str[i];
            streak = 1;            
        }
        d.res[resPos] = '\0';
    }
}

/***********************************************/

// fonction d'envoi du résultat
void comp_service_sendResult(int fifoFd, Data d)
{   
	// écriture du résultat dans les tubes
    int length = strlen(d.res);
    write(fifoFd,&(length),sizeof(int));
    write(fifoFd,&(d.res),sizeof(char) * (length + 1));
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/

int main(int argc, char * argv[])
{
   /*if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

    // TEMP:
    printf("%s ; %s ;%s ;%s\n",argv[1],argv[2],argv[3],argv[4]);
    
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
                Data* d = malloc(sizeof(Data));
                //envoi au client d'un code d'acceptation
                sendOkPwd(sToc);
                //réception des données du client (une fct par service)
                comp_service_receiveDataData(cTos,d);
                //calcul du résultat (une fct par service)
                comp_service_computeResult(*d);
                //envoi du résultat au client (une fct par service)
                comp_service_sendResult(sToc,*d);
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

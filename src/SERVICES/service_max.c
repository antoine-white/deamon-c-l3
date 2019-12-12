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
#include <float.h>
#include <limits.h>
#include <pthread.h>

#include "service_orchestre.h"
#include "client_service.h"
#include "../UTILS/myassert.h"

#define NB_THREADS 3//on a 3 threads par default

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
    float* arr;
    int length;
    float max;
};


typedef struct data Data;

/******* GESTION THREAD ********/

// structure pour passer les paramètres aux pthread
typedef struct
{
    float* max;
    float *arr;
    int start;
    int end;
    pthread_mutex_t mux;
} ThreadData;


// création de données pour les threads
static ThreadData* createThreadDatas(Data d)
{
    pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
    ThreadData* datas = (ThreadData*)malloc(sizeof(ThreadData) * NB_THREADS);
    int* max = (int *)malloc(sizeof(int));
    int intervalLength = d.length/NB_THREADS;
    for (int i = 0; i < NB_THREADS ; i++){
        datas[i].max = max ; 
        datas[i].arr = d.arr; 
        datas[i].start = i * intervalLength; 
        datas[i].end = (i+1) * intervalLength -1; 
        datas[i].mux = mux; 
    }
    datas[NB_THREADS - 1].end += d.length % NB_THREADS;
    return datas;
}

// Fonction support d'un thread
// Tous les threads lanceront cette fonction
void * codeThread(void * arg)
{
    ThreadData *data = (ThreadData *) arg;
    float max = FLT_MIN;

    for (int i = data->start; i <= data->end; i++){
        if(data->arr[i] > max){
            max = data->arr[i];
        }
    }
        
    // utilisation d'un mutex pour éviter que deux
    // threads écrivent en même temps
    pthread_mutex_lock(&(data->mux));
    if (*(data->max) < max){        
        (*(data->max)) = max;
    }
    pthread_mutex_unlock(&(data->mux));    
    
    return NULL;
}

/*********************************/

// fonction de réception des données
void max_service_receiveDataData(int fifoFd, Data* d)
{
    read(fifoFd,&(d->length),sizeof(int));
    d->arr = (float*)malloc(sizeof(float) * d->length);
    read(fifoFd,&(d->arr),sizeof(float*));
    d->max = FLT_MIN;
}


// fonction de traitement des données
void max_service_computeResult(Data d)
{
    pthread_t tabId[NB_THREADS];
    ThreadData* datas = createThreadDatas(d);
    // lancement des threads
    for (int i = 0; i < NB_THREADS; i++)
    {
        int ret = pthread_create(&(tabId[i]), NULL, codeThread, &(datas[i]));
        myassert(ret == 0,"erreur dans le lancement des threads");
    }
    // attente de la fin des threads
    for (int i = 0; i < NB_THREADS; i++)
    {
        int ret = pthread_join(tabId[i], NULL);
        myassert(ret == 0,"erreur dans l'attente des threads");
        free(datas[i].arr);
    }
    // on récupère le maximum
    d.max = *datas[0].max;
    free(datas[0].max);
    free(datas);    
}

// fonction d'envoi du résultat
void max_service_sendResult(int fifoFd, Data d)
{
   write(fifoFd,&(d.max),sizeof(float));
}



/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 5)
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
                Data* d = malloc(sizeof(Data);
                //envoi au client d'un code d'acceptation
                sendOkPwd(sToc);
                //réception des données du client (une fct par service)
                max_service_receiveDataData(cTos,d);
                //calcul du résultat (une fct par service)
                max_service_computeResult(*d);
                //envoi du résultat au client (une fct par service)
                max_service_sendResult(sToc,*d);
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
       
    }

    // TODO :libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

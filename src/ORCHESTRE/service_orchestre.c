#include "myassert.h"

#include "service_orchestre.h"
#include "client_orchestre.h"
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



/*
* Sémaphores 
*/

int s_o_sem_init(){
	int semClient = semget(IPC_PRIVATE,1,0666);
    myassert(semClient != -1,"erreur création de semaphore");
    semctl(semClient,0,IPC_SET,0); 
    return semClient; 
}


void s_o_sem_destroy(int sem){
	semctl(sem,-1,IPC_RMID); 
} 


int s_o_sem_prendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = -1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
   	 return sem; 

}
int s_o_sem_vendre(int sem){
	struct sembuf sops[1];
    
    sops[0].sem_num = 0;       
    sops[0].sem_op = +1;         
    sops[0].sem_flg = 0;

   	 semop(sem,sops,1);
    return sem; 

}
/*
int lauch_service(const char *basename, int numService, const Descriptors *pipes)
{
    int nameLength;
    char *name;
    char *argv[4];

    nameLength = snprintf(NULL, 0, "%s_%d", basename, numService);
    name = malloc((nameLength + 1) * sizeof(char));
    sprintf(name, "%s_%d", basename, numService);

    argv[0] = name;
    argv[1] = pipes->MtoW.name;
    argv[2] = pipes->WtoM.name;
    argv[3] = NULL;

    execv(argv[0], argv);
    myassert(false, name);
}
	
*/

/*
 void lauchServices(int nbService, Service* services)
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
}*/

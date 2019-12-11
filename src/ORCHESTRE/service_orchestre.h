#ifndef SERVICE_ORCHESTRE_H
#define SERVICE_ORCHESTRE_H

// Ici toutes les communications entre l'orchestre et les services :
// - le tube anonyme pour que l'orchestre envoie des données au service
// - le sémaphore pour que  le service indique à l'orchestre la fin
//   d'un traitement




/*
* création et destruction des sémaphores 
*/

int s_o_sem_init(); 
void s_o_sem_destroy(int sem); 
int s_o_sem_prendre(int sem); 
int s_o_sem_vendre(int sem); 


#endif

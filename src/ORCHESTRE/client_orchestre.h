#ifndef CLIENT_ORCHESTRE_H
#define CLIENT_ORCHESTRE_H

// Ici toutes les communications entre l'orchestre et les clients :
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
// - les deux tubes nommés pour la communication bidirectionnelle



/*
 * Types et fonctions communes à tous les clients et l'orchestre
 */

/*
 * Convention de codage :
 * - O pour Orchestre
 * - C pour Client 
 *
 * par exemple :
 * - "nameCtoO" : nom du tube dans le sens client vers orchestre
 *
 * Les fonctions côté :
 * - orchestre sont préfixées par "o_",
 * - client sont préfixées par "c_",
 */

/*
 * Structures pour la paire de descripteurs
 */
typedef struct
{
    char *name;
    int fd;
} PipeClientOrchestre;

typedef struct {
    PipeClientOrchestre CtoO;
    PipeClientOrchestre OtoC;
} DescriptorsCO;

/*
 * création et destruction des tubes
 */
void o_c_createPipes(DescriptorsCO *pipes);
void c_o_createPipes(DescriptorsCO *pipes);
void o_c_destroyPipes(DescriptorsCO *pipes);
void c_o_destroyPipes(DescriptorsCO *pipes);

/*
 * ouverture et fermeture des tubes
 */
void c_o_openPipes(DescriptorsCO *pipes);
void o_c_openPipes(DescriptorsCO *pipes);

void c_o_closePipes(DescriptorsCO *pipes);
void o_c_closePipes(DescriptorsCO *pipes);


/*
 * surcharge des envois et réceptions (pour gérer les erreurs)
 */
void o_c_writeData(DescriptorsCO *pipes, const void *buf, int size);
void c_o_writeData(DescriptorsCO *pipes, const void *buf,int size);
void o_c_readData(DescriptorsCO *pipes, void *buf, int size);
void c_o_readData(DescriptorsCO *pipes, void *buf, int size);

/*
* création et destruction des sémaphores 
*/

int c_o_sem_init(); 
void c_o_sem_destroy(int sem); 
int c_o_sem_prendre(int sem); 
int c_o_sem_vendre(int sem); 





#endif

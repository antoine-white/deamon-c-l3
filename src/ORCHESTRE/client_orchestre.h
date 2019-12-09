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
} OnePipe;

typedef struct {
    OnePipe CtoO;
    OnePipe OtoC;
} Descriptors;

/*
 * création et destruction des tubes
 */
void o_c_createPipes(Descriptors *pipes);
void c_o_createPipes(Descriptors *pipes);
void o_destroyPipes(Descriptors *pipes);

/*
 * ouverture et fermeture des tubes
 */
void c_openPipes(const char *nameCtoO, const char *nameOtoC, Descriptors *pipes);
void o_openPipes(const char *nameCtoO, const char *nameOtoC, Descriptors *pipes);

void c_closePipes(Descriptors *pipes);
void o_closePipes(Descriptors *pipes);

/*
* création et destruction des sémaphores 
*/

int sem_init(); 
void sem_destroy(int sem); 
int sem_prendre(int sem); 
int sem_vendre(int sem); 





#endif

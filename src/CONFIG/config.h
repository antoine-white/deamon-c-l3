/*
 * Indications (à respecter) :
 * - vous n'avez pas le droit de modifier ce fichier
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Remarques sur les méthodes :
// - config_init ne peut être appelé qu'une seule fois, ou alors
//   après l'appel à config_exit pour gérer un autre fichier de config
// - config_exit ne peut être appelé qu'après config_init et une seule fois,
//   ou alors après un autre appel à config_init
// - les autres méthodes ne peuvent être appelées qu'après config_init
//   et avant config_exit
//
// Les positions commencent à 1

void config_init(const char *filename);
void config_exit();

int config_getNbServices();
bool config_isServiceOpen(int pos);
const char * config_getExeName(int pos);

#endif


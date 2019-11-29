/*
 * Indications (à respecter) :
 * - Les erreurs sont gérées avec des assert ; les erreurs traitées sont :
 *    . appel trop tôt ou trop tard d'une méthode (cf. config.h)
 *    . fichier de configuration inaccessible
 *    . une position erronnée
 * - Le fichier (si on arrive à l'ouvrir) est considéré comme bien
 *   formé sans qu'il soit nécessaire de le vérifier
 *
 * Un code minimal est fourni et permet d'utiliser le module "config" dès
 * le début du projet ; il faudra le remplacer par l'utilisation du fichier
 * de configuration.
 * Il est inutile de faire plus que ce qui est demandé
 *
 * Dans cette partie vous avez le droit d'utiliser les entrées-sorties
 * de haut niveau (fopen, fgets, ...)
 */


// TODO include des .h système

#include "myassert.h"

#include "config.h"

// TODO Structure de données ici


void config_init(const char *filename)
{
    // TODO erreur si la fonction est appelée deux fois

    // TODO code vide par défaut, à remplacer
}

void config_exit()
{
    // TODO erreur si la fonction est appelée avant config_init

    // TODO code vide par défaut, à remplacer
}

int config_getNbServices()
{
    // erreur si la fonction est appelée avant config_init
    // erreur si la fonction est appelée après config_exit
    
    // code par défaut, à remplacer
    return 3;
}

bool config_isServiceOpen(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit
    // TODO erreur si "pos" est incorrect

    // TODO code par défaut, à remplacer
    bool open[] = {true, false, true};
    return open[pos-1];
}

const char * config_getExeName(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit
    // TODO erreur si "pos" est incorrect

    // TODO code par défaut, à remplacer
    const char * names[] = {
        "SERVICES/service_somme",
        "SERVICES/service_compression",
        "SERVICES/service_max"
    };
    return names[pos-1];
}

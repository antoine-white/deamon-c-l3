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
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "myassert.h"

#include "config.h"

// TODO Structure de données ici
typedef struct file_config file_config;
struct file_config
{
    FILE* fp;
    bool isOpen;
};

static file_config config = {.fp = NULL, .isOpen = false};

static void assertFileOpen(){
    myassert(config.isOpen,"operation impossible le fichier de configuration sans avoir appele config_init");
}

void config_init(const char *filename)
{
    // TODO erreur si la fonction est appelée deux fois
    myassert( ! config.isOpen,"config_init appele une deuxieme fois sans avoir appele config_exit");
    
    config.isOpen = true;
    config.fp = fopen(filename,"r");
    myassert(config.fp != NULL,"erreur dans l'ouverture du fichier");
}

void config_exit()
{
    // TODO erreur si la fonction est appelée avant config_init
    myassert(config.isOpen,"config_exit appele sans avoir appele config_init");

    myassert(fclose(config.fp) == 0,"erreur dans la fermeture du fichier");
}

int config_getNbServices()
{
    assertFileOpen();    

    rewind(config.fp);// on reviens au début du fichier
    int temp;
    fscanf(config.fp,"%d",&temp);
    return temp;
}

bool config_isServiceOpen(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit
    assertFileOpen();
    // TODO erreur si "pos" est incorrect
    const int nbServiceOpen = config_getNbServices();
    myassert(pos <= nbServiceOpen,"Le service demande a un indice superieur a l'indice du service maximal");
    // TODO code par défaut, à remplacer
    const char open[7] = "ouvert";
    char line[256];//buffer large 
    for(int i = 0; i < pos; i++)
    {
        fgets(line, sizeof(line), file);
        
        open[i] = startsWith(open,line);
    }
    return open[pos-1];
}

const char * config_getExeName(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit
    // TODO erreur si "pos" est incorrect
    assertFileOpen();
    // TODO code par défaut, à remplacer
    const char * names[] = {
        "SERVICES/service_somme",
        "SERVICES/service_compression",
        "SERVICES/service_max"
    };
    return names[pos-1];
}

//We don't have to worry about str being shorter than pre because according to the C standard (7.21.4.4/2):
//The strncmp function compares not more than n characters (characters that follow a null character are not compared) from the array pointed to by s1 to the array pointed to by s2."
//sources : https://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
static bool startsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}






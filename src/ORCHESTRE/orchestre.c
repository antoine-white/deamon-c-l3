#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "client_orchestre.h"
#include "service_orchestre.h"


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

    // Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre
    
    // lancement des services, avec pour chaque service :
    // - création d'un tube anonyme pour converser (orchestre vers service)
    // - un sémaphore pour que le service préviene l'orchestre de la
    //   fin d'un traitement
    // - création de deux tubes nommés pour les communications entre
    //   les clients et le service

    while (true)
    {
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

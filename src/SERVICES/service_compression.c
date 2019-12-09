#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
void compression_service_receiveDataData(/* tubes,*/ /* autre chose ? */)
{
}

// fonction de traitement des données
void compression_service_computeResult(/* autre chose ? */)
{
}

// fonction d'envoi du résultat
void compression_service_sendResult(/* tubes,*/ /* autre chose ? */)
{
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 5)
        usage(argv[0], "nombre paramètres incorrect");

    // initialisations diverses
    printf("%s , %s\n",argv[1],argv[2]);
    while (true)
    {
        // attente d'un code de l'orchestre (via tube anonyme)
        // si code de fin
        //    sortie de la boucle
        // sinon
        //    réception du mot de passe de l'orchestre
        //    attente du mot de passe du client
        //    si mot de passe incorrect
        //        envoi au client d'un code d'erreur
        //    sinon
        //        envoi au client d'un code d'acceptation
        //        réception des données du client (une fct par service)
        //        calcul du résultat (une fct par service)
        //        envoi du résultat au client (une fct par service)
        //        attente de l'accusé de réception du client
        //    modification du sémaphore pour prévenir l'orchestre de la fin
    }

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}

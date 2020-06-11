# PROJET

Le fichier "compile.sh" compile tous les exécutables dans les différents répertoires.

Le fichier "clean.sh" supprimer les fichiers générés par "compile.sh"

## Arborescence  :
- 0README : ce fichier
- répertoire UTILS : du code généraliste (i.e. non lié au projet et qui
        pourrait être utilisé par d'autres personnes).
	Par exemple le code de "myassert" y est.
	cf. UTILS/0README
- répertoire CONFIG  contient le code pour lire et exploiter un fichier
        de configuration, les fichiers de configuration, ainsi
	qu'un programme de test.
	cf. CONFIG/0README
- répertoire ORCHESTRE : contient le code de l'orchestre (i.e. du démon),
        ainsi que les routines de communication entre :
	  . d'une part l'orchestre et un client
	  . entre l'orchestre et un service d'autre part
	cf. ORCHESTRE/0README
- répertoire SERVICES contient les codes de tous les services, ainsi
        que les routines de communication entre le service et le client.
	cf. SERVICES/0README
- répertoire CLIENT contient le code du client ainsi que les routines
        de traitement pour chaque service.
	cf. CLIENT/0README

## EXECUTION

On lance l'orchestre à partir de ce répertoire, par exemple :
`$ ORCHESTRE/orchestre CONFIG/config.txt`

De même on lance un client à partir de ce répertoire, par exemple :
`$ CLIENT/client 1 345 76 "==> "`

Ce qui signifie qu'on veut le service 1, pour sommer les nombres 345
et 76, et que le résultat sera affiché et précédé de la chaine "==> "

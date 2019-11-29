#!/bin/bash

cc="gcc -I. -I../UTILS -I../ORCHESTRE -I../SERVICES -Wall -Wextra -pedantic -std=c99 -fPIC -g"
commun="../UTILS/myassert.c ../ORCHESTRE/client_orchestre.c ../SERVICES/client_service.c"
clients="./client_somme.c ./client_compression.c ./client_max.c"

exe="client"

for f in $exe
do
    $cc -o $f $f.c $commun $clients
done



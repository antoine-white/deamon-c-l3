#!/bin/bash

cc="gcc -I. -I../UTILS -I../ORCHESTRE -Wall -Wextra -pedantic -std=c99 -fPIC -g "
commun="../UTILS/myassert.c ../ORCHESTRE/service_orchestre.c ./client_service.c"

exe="service_somme service_compression service_max"

for f in $exe
do
    $cc -o $f $f.c $commun -lpthread
done



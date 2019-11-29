#!/bin/bash

cc="gcc -I. -I../UTILS -I../CONFIG -Wall -Wextra -pedantic -std=c99 -fPIC -g"
commun="../UTILS/myassert.c ../CONFIG/config.c ./client_orchestre.c ./service_orchestre.c"

$cc -o orchestre orchestre.c $commun

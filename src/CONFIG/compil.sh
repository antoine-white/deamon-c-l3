#!/bin/bash

cc="gcc -I../UTILS -Wall -Wextra -pedantic -std=c99 -fPIC -g"
commun="../UTILS/myassert.c ./config.c"

$cc -o test_config test_config.c $commun

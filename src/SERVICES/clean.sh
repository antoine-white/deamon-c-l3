#!/bin/bash

exe="service_somme service_compression service_max"

for f in $exe
do
    /bin/rm $f
done


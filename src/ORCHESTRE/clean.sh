#!/bin/bash

exe="orchestre"

for f in $exe
do
    /bin/rm $f
done

rm CtoS0
rm CtoS1
rm CtoS2
rm StoC0
rm StoC1
rm StoC2
rm client1
rm client2
    
ipcs -s 
ipcrm -a
ipcs -s 

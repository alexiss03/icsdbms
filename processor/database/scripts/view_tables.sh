#!/bin/sh

#cd ..
cd processor/database/data/main
array=$(ls *.dat)

for sites in $array
do
echo "$sites"
done
return array



#!/bin/sh

num=$2

if [ $num -eq 1 ] 
then
rm processor/database/data/tmp/$1.dat
rm processor/database/data/tmp/$1.stdat
.
fi




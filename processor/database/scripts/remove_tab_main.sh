#!/bin/sh

num=$2

if [ $num -eq 2 ]
then
rm processor/database/data/main/$1.dat
rm processor/database/data/main/$1.stdat
.
fi

#!/bin/sh

fname=$1
cp processor/database/data/tmp/$1.dat processor/database/data/main
cp processor/database/data/tmp/$1.stdat processor/database/data/main

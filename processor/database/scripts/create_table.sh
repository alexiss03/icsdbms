#!/bin/sh

FILE=$1
PATH=$2
if [ "$PATH" == "processor/database/data/main"]; then
cd $PATH
touch $FILE
exit 0
.
elif [ "$PATH" == "processor/database/data/statistical_data/main" ]
cd $PATH
touch $FILE
exit 0
.
fi

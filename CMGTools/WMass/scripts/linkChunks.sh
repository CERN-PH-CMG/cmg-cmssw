#!/bin/bash
# usage: linkChunks.ch <DIR>

DIR=$1
chunks=`ls $DIR`

for c in $chunks 
do 
    echo "Linking $c ..." 
    ln -s $DIR/$c
done

echo "DONE."

#!/bin/bash

for i in `bjobs -d | grep EXIT  | awk '{ print $1 }'`
do
    text=`bjobs -l $i | grep Chunk | sed ':a;N;$!ba;s/\n/ /g'  | awk '{ print $1 }'`
    echo $i $text
done



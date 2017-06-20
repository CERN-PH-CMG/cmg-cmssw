#!/bin/bash
if [[ "$1" == "" || "$1" == "-h" || "$1" == "--help" ]]; then
    echo "Usage: $0 [ -z ] [ prefix ] "
    echo "Will look for chunks and check if there is some one missing. "
    echo "It only looks for holes in the sequence of chunks, " 
    echo "and the presence of zombies if option -z is given. " 
    exit 1;
fi

Z=0
if [[ "$1" == "-z" ]]; then
    echo "# Will also check if rootfiles $F are zombies or not"
    Z=1; shift;
fi;

what=$1
for F in $(ls ${what}_*.chunk*.root | sed 's/\.chunk[0-9]\+//' | sort | uniq); do
    FILES=$(ls ${F/.root/.chunk*.root} | \
            perl -npe 's/\.chunk(\d+)\./sprintf(".%06d.",$1)/e' | \
            sort -n | \
            perl -npe 's/\.(\d+)\.root$/sprintf(".chunk%d.root",$1)/e' );
    # echo -e "\nCheck chunk files for $F"; 
    filesimple=$(ls ${F/.root/.chunk*.root})
    filesarray=($FILES)
    NCHUNKS=$((${#filesarray[@]} - 1 ))
    for c in `seq 0 $NCHUNKS`; do
        ftest=$(echo $F | awk -F "." '{print $1 ".chunk"}');
        ftest2=$ftest$c".root"
        if [ ! -f $ftest2 ]; then 
            echo "$ftest2 # not present";
        fi;
    done
done

if [[ "$Z" != "0" ]]; then
    echo "# Testing for zombies";
    FILES=$(ls ${what}_*.chunk*.root);
    for Z in $(cmgListZombies  $FILES); do
        if test -s $Z; then # empty files have already been found
            D=${Z%%/*};
            echo "${BASE}${D}    # zombie";
        fi;
    done
fi;


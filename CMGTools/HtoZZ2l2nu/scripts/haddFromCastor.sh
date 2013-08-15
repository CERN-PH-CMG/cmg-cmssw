#!/bin/bash

castordir=""
for i in $*
do
  case $i in
      -d*)
      castordir=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
  esac
done

if [ -z "$castordir" ]
then
    echo "haddFromCastor.sh -d=castorDirectory"
    exit -1
fi

output="`basename $castordir`.root"
files=(`rfdir $castordir | awk {'print $9'}`)
me=`whoami`
haddstr=""

echo "Merging ${#files[*]} root files from $castordir"
for f in ${files[@]}; do
    rfcp ${castordir}/${f} /tmp/${me}
    haddstr="${str} /tmp/${me}/${f}"
done

hadd $output $haddstr
rm $haddstr

echo "Results is available at $output"
#!/bin/bash

usage='Usage: -i <inputdir>  -o <outputdir>'



args=`getopt rd: -- "$@"`
if test $? != 0
     then
         echo $usage
         exit 1
fi



eval set -- "$args"
for i
  do
  case "$i" in
      -i) shift; inputdir=$2;shift;;
      -o) shift; outputdir=$2;shift;;
  esac
done

if [ "X"${inputdir} == "X" ]
    then
    echo "MISSING INPUT DIR NAME ! Please give a valid one!"
    echo $usage
    exit
fi

if [ "X"${outputdir} == "X" ]
    then
    echo "MISSING INPUT DIR NAME! Please give a valid one!"
    echo $usage
    exit
fi


# specify directories here
work_dir=`\pwd`
echo $work_dir

outdir = plots/${ouputdir}

mkdir $outdir


# run root command in batch

root -b <<!

.x macros/DrawJetIdPlots.cxx("TK","pt10to20","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("TK","pt20to30","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("TK","pt30to50","${inputdir}","$outdir");

.x macros/DrawJetIdPlots.cxx("HEin","pt10to20","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("HEin","pt20to30","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("HEin","pt30to50","${inputdir}","$outdir");

.x macros/DrawJetIdPlots.cxx("HEout","pt10to20","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("HEout","pt20to30","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("HEout","pt30to50","${inputdir}","$outdir");

.x macros/DrawJetIdPlots.cxx("HF","pt10to20","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("HF","pt20to30","${inputdir}","$outdir");
.x macros/DrawJetIdPlots.cxx("HF","pt30to50","${inputdir}","$outdir");

.q
!


echo 'Making webpage...'

cat > ${plots_dir}/index.html <<EOF

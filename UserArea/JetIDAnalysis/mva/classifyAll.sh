#!/bin/bash

jettype=chs
config=classify.conf
name=mva_all_${type}.html

echo "<html>"                                                                                              >  $name
echo "<head><title> MVA Plots</title></head>"                                >> $name
echo "<body bgcolor=\"FFFFFF\">"                                             >> $name
echo "<h3 style=\"text-align:left; color:DD6600;\"> MVA Plots</h3>"          >> $name
echo "<table border=\"0\" cellspacing=\"5\" width=\"100%\">"                 >> $name

for dataset in `cat $config | grep -v ^# | tr -s ' ' | sed "s@\s\s@ @g" | awk '{print $2}' `; do
  line=`grep -v ^# $config | grep $dataset `
  ds=`      echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $1}'`
  id=`      echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $2}'`
  kinflag=` echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $3}'`
  ipflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $4}'`
  nparf=`   echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $5}'`
  drflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $6}'`
  ptflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $7}'`
  rest=`    echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $8}'`
### input               id	kinflag	ipflag 	nparf	drflag	ptflag	rest 
  root -b -q classify.C+\(\"$id\"\,$kinflag\,$ipflag\,$nparf\,$drflag\,$ptflag\,$rest\)
  root -b -q apply.C+\(\"../Jets/${ds}.root\",\"$id\"\,$kinflag\,$ipflag\,$nparf\,$drflag\,$ptflag\,$rest\)
  mv Output.root ../Jets/${ds}_${id}.root
  mc=`echo $ds | sed "s@f11-zjets@r11-dimu@g" | sed "s@_rw@@g"`
  root -b -q apply.C+\(\"../Jets/${mc}.root\",\"$id\"\,$kinflag\,$ipflag\,$nparf\,$drflag\,$ptflag\,$rest\)
  mv Output.root ../Jets/${mc}_rw_${id}.root
  ./mvaPlot.sh $id $jettype
  echo "<tr> " >> $name
  echo "<td> " >> $name
  echo "<a href=\""${jettype}"_rw_"$id"/mva_"$jettype"_rw_"$id"_"$jettype".html\">   MVA for the following id: "$id" </a> <br> "                          >> $name
  echo "</td><td> "                                                                                                                           >> $name
  echo "<a href=\"weights_"$jettype"/TMVAClassificationCategory_JetID_"$id".weights.xml\">   Weight file for the following : "$id" </a> <br> "   >> $name
  echo "</td></tr> " >> $name
done
echo "</html>"                                                                                                                                >> $name


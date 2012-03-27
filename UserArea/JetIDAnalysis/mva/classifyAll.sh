#!/bin/bash

#jettype=chs
config=classify.conf
name=mva_all.html

echo "<html>"                                                                                              >  $name
echo "<head><title> MVA Plots</title></head>"                                >> $name
echo "<body bgcolor=\"FFFFFF\">"                                             >> $name
echo "<h3 style=\"text-align:left; color:DD6600;\"> MVA Plots</h3>"          >> $name
echo "<table border=\"0\" cellspacing=\"5\" width=\"100%\">"                 >> $name

for dataset in `cat $config | grep -v ^# | tr -s ' ' | sed "s@\s\s@ @g" | awk '{print $2}' `; do
  line=`grep -v ^# $config | grep $dataset `
  ds=`      echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $1}'`
  id=`      echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $2}'`
  jettype=` echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $3}'`
  mvatype=` echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $4}'`
  ipflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $5}'`
  kinflag=` echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $6}'`
  ipflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $7}'`
  nparf=`   echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $8}'`
  drflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $9}'`
  ptflag=`  echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $10}'`
  order=`   echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $11}'`
  ptdflag=` echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $12}'`
  frac=`    echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $13}'`
  momtype=` echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $14}'`
  rings=`   echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $15}'`
  rest=`    echo    $line           | tr -s ' '  | sed "s@\s\s@ @g" | awk '{print $16}'`
  
  root -b -q classify.C+\(\"$id\"\,$jettype\,$mvatype\,$ipflag\,$kinflag\,$ipflag\,$nparf\,$drflag\,$ptflag\,$order\,$ptdflag\,$frac\,$momtype\,$rings\,$rest\)
  root -b -q apply.C+\(\"../scratch/${ds}.root\"\,\"$id\"\,$jettype\,$mvatype\,$ipflag\,$kinflag\,$ipflag\,$nparf\,$drflag\,$ptflag\,$order\,$ptdflag\,$frac\,$momtype\,$rings\,$rest\)
  mv Output.root ../scratch/${ds}_${id}.root
  mc=`echo $ds | sed "s@f11-zjets@r11-dimu@g" `
  root -b -q apply.C+\(\"../scratch/${mc}.root\"\,\"$id\"\,$jettype\,$mvatype\,$ipflag\,$kinflag\,$ipflag\,$nparf\,$drflag\,$ptflag\,$order\,$ptdflag\,$frac\,$momtype\,$rings\,$rest\)
  mv Output.root ../scratch/${mc}_${id}.root
  ./mvaPlot.sh $id $jettype
  echo "<tr> " >> $name
  echo "<td> " >> $name
  echo "<a href=\""${jettype}"_rw_"$id"/mva_"$jettype"_rw_"$id"_"$jettype".html\">   MVA for the following id: "$id" </a> <br> "              >> $name
  echo "</td><td> "                                                                                                                            >> $name
  echo "<a href=\"weights_"$jettype"/TMVAClassificationCategory_JetID_"$id".weights.xml\">   Weight file for the following : "$id" </a> <br> " >> $name
  echo "</td></tr> "                                                                                                                           >> $name
done
echo "</html>"                                                                                                                                 >> $name


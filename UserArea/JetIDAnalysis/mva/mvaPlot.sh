#!/bin/bash
idtype=$1
Type=$2
Id=${Type}_rw_${idtype}
name=mva_${Id}_${Type}.html
mkdir $Id
cd $Id
cp ../mvaPU.C .
cp ../plotJets_roc.C .
cp ../plotTools.hh .
cp ../axisTools.hh .

echo "<html>"                                                                >  $name
echo "<head><title>Htt Limit Plots</title></head>"                           >> $name
echo "<body bgcolor=\"FFFFFF\">"                                             >> $name
echo "<h3 style=\"text-align:left; color:DD6600;\"> Plots</h3>"              >> $name
echo "<table border=\"0\" cellspacing=\"5\" width=\"100%\">"                 >> $name

echo "<tr>"                                                                  >> $name
echo "<td> <u> PT > 0   and |eta| < 2.5 </u>   </td>"                        >> $name
echo "<tr>"                                                                  >> $name
end=eta25_pt0
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt\_1\>0\&\&abs\(jseta_1\)\<2\.5\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\<2\.5\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\<2\.5\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                   >> $name
echo "<td> <u> PT > 20   and |eta| < 2.5 </u>   </td>"                        >> $name
echo "<tr>"                                                                   >> $name
end=eta25_pt20
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\<2\.5\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\<2\.5\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\<2\.5\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                   >> $name
echo "<td> <u> PT > 30   and |eta| < 2.5 </u>   </td>"                        >> $name
echo "<tr>"                                                                   >> $name
end=eta25_pt30
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\<2\.5\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\<2\.5\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\<2\.5\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                  >> $name
echo "<td> <u> PT > 0   and 2.5 < |eta| < 2.9 </u>   </td>"                  >> $name
echo "<tr>"                                                                  >> $name
end=eta25_29_pt0
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                   >> $name
echo "<td> <u> PT > 20   and 2.5 < |eta| < 2.9 </u>   </td>"                  >> $name
echo "<tr>"                                                                   >> $name
end=eta25_29_pt20
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                   >> $name
echo "<td> <u> PT > 30   and 2.5 < |eta| < 2.9 </u>   </td>"                  >> $name
echo "<tr>"                                                                   >> $name
end=eta25_29_pt30
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\>2\.5\&\&abs\(jseta_1\)\<2\.9\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                  >> $name
echo "<td> <u> PT > 0    and 2.9 < |eta|      </u>   </td>"                  >> $name
echo "<tr>"                                                                  >> $name
end=eta29_pt0
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>0\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                  >> $name
echo "<td> <u> PT > 20   and 2.9 < |eta|      </u>   </td>"                  >> $name
echo "<tr>"                                                                  >> $name
end=eta29_pt20
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>20\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name

echo "<tr>"                                                                  >> $name
echo "<td> <u> PT > 30   and 2.9 < |eta|      </u>   </td>"                  >> $name
echo "<tr>"                                                                  >> $name
end=eta29_pt30
root -b -q mvaPU.C\(\"$Id\"\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,false\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
root -b -q plotJets_roc.C\(\"$Id\"\,true\,\"\(jspt_1\>30\&\&abs\(jseta_1\)\>2\.9\&\&abs\(jseta_1\)\<12\.9\)\"\)
mv MVA${Id}.png  MVA${Id}_${end}.png
mv Eta${Id}.png  Eta${Id}_${end}.png
mv Pt${Id}.png   Pt${Id}_${end}.png
mv Roc${Id}.png  Roc${Id}_${end}.png
mv Roc${Id}PtWeight.png  Roc${Id}_${end}PtWeight.png
echo "<td width=\"20%\"><a href=\""MVA${Id}_${end}.png"\"><img src=\""MVA${Id}_${end}.png"\" alt=\""MVA${Id}_${end}.png"\" width=\"100%\"></a></td>" >> $name
echo "<td width=\"20%\"><a href=\""Eta${Id}_${end}.png"\"><img src=\""Eta${Id}_${end}.png"\" alt=\""Eta${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Pt${Id}_${end}.png"\"><img src=\""Pt${Id}_${end}.png"\" alt=\""Pt${Id}_${end}.png"\" width=\"100%\"></a></td>"       >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}.png"\"><img src=\""Roc${Id}_${end}.png"\" alt=\""Roc${Id}_${end}.png"\" width=\"100%\"></a></td>"    >> $name
echo "<td width=\"20%\"><a href=\""Roc${Id}_${end}PtWeight.png"\"><img src=\""Roc${Id}_${end}PtWeight.png"\" alt=\""Roc${Id}_${end}PtWeight.png"\" width=\"100%\"></a></td>" >> $name
echo "</table>"  >> $name
echo "</body>"   >> $name
echo "</html>"   >> $name

cd ..

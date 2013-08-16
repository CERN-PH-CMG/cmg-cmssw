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

plotdir=plots/${outputdir}
echo $plotdir


# run root command in batch

root -b <<!

.x macros/DrawJetIdEfficiency.cxx("TK","pt10to20","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("TK","pt20to30","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("TK","pt30to50","${inputdir}","${plotdir}");

.x macros/DrawJetIdEfficiency.cxx("HEin","pt10to20","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("HEin","pt20to30","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("HEin","pt30to50","${inputdir}","${plotdir}");

.x macros/DrawJetIdEfficiency.cxx("HEout","pt10to20","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("HEout","pt20to30","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("HEout","pt30to50","${inputdir}","${plotdir}");

.x macros/DrawJetIdEfficiency.cxx("HF","pt10to20","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("HF","pt20to30","${inputdir}","${plotdir}");
.x macros/DrawJetIdEfficiency.cxx("HF","pt30to50","${inputdir}","${plotdir}");

.x macros/DrawJetIdEfficiency.cxx("ALL","pt20to100","${inputdir}","${plotdir}");

.q
!


echo 'Making webpage...'

httpdir=https://cmsdoc.cern.ch/~malberti/Hgg/JETID/

cat > ${plotdir}/index.html <<EOF


<HEAD><TITLE> JET ID EFFICIENCY PLOTS </TITLE></HEAD>
 
<FONT color="Black">


<h4> JET Id efficiency plots </h4>
<ul>
 <li><A href="#ALL_pt20to100"> ALL eta, pT = 20-100 GeV </A><BR>

 <li><A href="#TK_pt10to20"> TK, pT = 10-20 GeV </A><BR>
 <li><A href="#TK_pt20to30"> TK, pT = 20-30 GeV </A><BR>
 <li><A href="#TK_pt30to50"> TK, pT = 30-50 GeV </A><BR>

 <li><A href="#HEin_pt10to20"> HEin, pT = 10-20 GeV </A><BR>
 <li><A href="#HEin_pt20to30"> HEin, pT = 20-30 GeV </A><BR>
 <li><A href="#HEin_pt30to50"> HEin, pT = 30-50 GeV </A><BR>

 <li><A href="#HEout_pt10to20"> HEout, pT = 10-20 GeV </A><BR>
 <li><A href="#HEout_pt20to30"> HEout, pT = 20-30 GeV </A><BR>
 <li><A href="#HEout_pt30to50"> HEout, pT = 30-50 GeV </A><BR>

 <li><A href="#HF_pt10to20"> HF, pT = 10-20 GeV </A><BR>
 <li><A href="#HF_pt20to30"> HF, pT = 20-30 GeV </A><BR>
 <li><A href="#HF_pt30to50"> HF, pT = 30-50 GeV </A><BR>

</ul>


<hr>
<h3><A name="ALL_pt20to100"> ALL, pT = 20-100 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/ALL_pt20to100/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>

<hr>

<h3><A name="TK_pt10to20"> TK, pT = 10-20 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>

<hr>


<h3><A name="TK_pt20to30"> TK, pT = 20-30 GeV  </h3>

<h4> simpleID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>


<h4> fullID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_fullId_Tight.png"> </A>


<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>

<hr>


<h3><A name="TK_pt30to50"> TK, pT = 30-50 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>


<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/TK_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>

<hr> 



<h3><A name="HEin_pt10to20"> HEin, pT = 10-20 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt10to20/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>


<hr>


<h3><A name="HEin_pt20to30"> HEin, pT = 20-30 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>

<hr>


<h3><A name="HEin_pt30to50"> HEin, pT = 30-50 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEin_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>


<hr>



<h3><A name="HEout_pt10to20"> HEout, pT = 10-20 GeV  </h3>

<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>


<hr>


<h3><A name="HEout_pt20to30"> HEout, pT = 20-30 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>

<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>


<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt20to30/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>

<hr>


<h3><A name="HEout_pt30to50"> HEout, pT = 30-50 GeV  </h3>

<h4> SimpleID  </h4>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_simpleId_Tight.png"> </A>


<h4> FullID  </h4>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_fullId_Tight.png"> </A>

<h4> CutBasedID  </h4>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>
%
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HEout_pt30to50/cEff_vs_NumberOfVertices_cutbasedId_Tight.png"> </A>
<hr>


<h3><A name="HF_pt10to20"> HF, pT = 10-20 GeV  </h3>

<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt10to20/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>


<hr>


<h3><A name="HF_pt20to30"> HF, pT = 20-30 GeV  </h3>

<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt20to30/cEff_vs_JetEta_cutbasedId_Tight.png"> </A>

<hr>


<h3><A name="HF_pt30to50"> HF, pT = 30-50 GeV  </h3>

<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_simpleId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_simpleId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_simpleId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_simpleId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_simpleId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_simpleId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_fullId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_fullId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_fullId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_fullId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_fullId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_fullId_Tight.png"> </A>

<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_cutbasedId_Loose.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_cutbasedId_Medium.png"> </A>
<A HREF=${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png> <img height="300" src="${httpdir}/${outputdir}/HF_pt30to50/cEff_vs_JetPt_cutbasedId_Tight.png"> </A>
<hr>





<hr>

</HTML>

EOF

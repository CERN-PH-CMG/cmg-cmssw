#!/bin/sh


#######################################################################
#export SAMPLEPATH=muTau201253X_580Feb7
#export SAMPLEPATH=muTau201253X_5140Apr18Isolations
#export SAMPLEPATH=muTau201253X_5140May2_TauIso3HitorMVA2
#export SAMPLEPATH=muTau2012_5140May27
#export SAMPLEPATH=muTau2012_5140May31
#export SAMPLEPATH=muTau2012_5160June10
#export SAMPLEPATH=muTau2012_5160June23
#export SAMPLEPATH=muTau2012_5160June28
#export SAMPLEPATH=muTau2012_5160June28NoTauES
#export SAMPLEPATH=muTau2012_5160June30
export SAMPLEPATH=muTau2012_5160July1

################### Summer13  ################################
cat=0
var=svfitmass
rm -f muTau_SM${cat}_${var}_.root
rm -f muTau_SM${cat}_${var}_tUp.root
rm -f muTau_SM${cat}_${var}_tDown.root
rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\) &
rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\) &
rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\) 
rootl combineDataCardsSMSummer13.C\(1\,${cat}\,\"${var}\"\)
mv muTau_SM${cat}_${var}.root htt_mt.inputs-sm-8TeV-summer13.root

#cat=0
#var=ditaumass
#rm -f muTau_SM${cat}_${var}_.root
#rm -f muTau_SM${cat}_${var}_tUp.root
#rm -f muTau_SM${cat}_${var}_tDown.root
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\) &
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\) &
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\) 
#rootl combineDataCardsSMSummer13.C\(1\,${cat}\,\"${var}\"\)
#mv muTau_SM${cat}_${var}.root htt_mt.inputs-sm-8TeV-summer13-mvis.root


#####Vs decay mode (use vbf tight)
#for mode in 50 60 70 ; do
#cat=17
#var=ditaumass
#rm -f muTau_SM${cat}_${var}_.root
#rm -f muTau_SM${cat}_${var}_tUp.root
#rm -f muTau_SM${cat}_${var}_tDown.root
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\",${mode}\) &
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\",${mode}\) &
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\",${mode}\) 
#rootl combineDataCardsSMSummer13.C\(1\,${cat}\,\"${var}\"\)
#mv muTau_SM${cat}_${var}.root htt_mt.inputs-sm-8TeV-summer13-mvis-mode${mode}.root
#done

##########################Fits to ZTT vs pT
##for p in 11 ; do 
##for p in 1 2 3 4 5 6 7 8; do 
##for p in 11 12 13 14 15 16 17 18 21 22 23 24 25 26 27 28; do 
#for p in 51 52 53 54 55 56 57 58 59 61 62 63 64 65 66 67 68 69 71 72 73 74 75 76 77 78 79; do 
#cat=10
#var=ditaumass
#rm -f muTau_SM${cat}_${var}_.root
#rootl dataCardSMSummer13.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${p}\) 
#mv muTau_SM${cat}_${var}_.root htt_mt.inputs-sm-8TeV-summer13-mvis-${p}.root
#done

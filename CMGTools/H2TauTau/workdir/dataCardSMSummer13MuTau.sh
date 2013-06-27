#!/bin/sh


#######################################################################
#export SAMPLEPATH=muTau201253X_580Feb7
#export SAMPLEPATH=muTau201253X_5140Apr18Isolations
#export SAMPLEPATH=muTau201253X_5140May2_TauIso3HitorMVA2
#export SAMPLEPATH=muTau2012_5140May27
#export SAMPLEPATH=muTau2012_5140May31
#export SAMPLEPATH=muTau2012_5160June10
export SAMPLEPATH=muTau2012_5160June23

#################### Summer13  ################################
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

#!/bin/sh


#######################################################################
#export SAMPLEPATH=muTau201253X_580Feb7
#export SAMPLEPATH=muTau201253X_5140Apr18Isolations
export SAMPLEPATH=muTau201253X_5140May2_TauIso3HitorMVA2

#################### Summer13  ################################
rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl combineDataCardsSM.C\(1\,0\,\"svfitmass\"\)
mv muTau_SM0_svfitmass.root htt_mt.inputs-sm-8TeV-summer13.root


###########Grand optimization of Boost###################
###3Hit
for tpt in 1 2 3 4 5; do
for hpt in 1 2 3 4 5 6 7 8; do
option=${tpt}${hpt}
echo $option

rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\)
rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\)
rootl combineDataCardsSM.C\(1\,13\,\"svfitmass\"\)
mv muTau_SM13_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM13_${option}.root

done
done



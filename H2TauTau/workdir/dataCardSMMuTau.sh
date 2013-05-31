#!/bin/sh


#######################################################################
#export SAMPLEPATH=muTau201253X_580Feb7
#export SAMPLEPATH=muTau201253X_5140Apr18Isolations
#export SAMPLEPATH=muTau201253X_5140May2_TauIso3HitorMVA2
export SAMPLEPATH=muTau2012_5140May27

##################### Summer13  ################################
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl combineDataCardsSM.C\(1\,0\,\"svfitmass\"\)
#mv muTau_SM0_svfitmass.root htt_mt.inputs-sm-8TeV-summer13.root

###################mT optimization#########################
#for mt in 1 2 3 4 5 6 7 8; do
#option=${mt}
#echo $option
#rm -f muTau_SM0_svfitmass_.root
#rm -f muTau_SM0_svfitmass_tUp.root
#rm -f muTau_SM0_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,-${option}\) &
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,-${option}\) &
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,-${option}\)
#rootl combineDataCardsSM.C\(1\,0\,\"svfitmass\"\)
#mv muTau_SM0_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM0_mT_${option}.root
#done

###########Grand optimization of Boost###################
##for boost_high
for tpt in 3 4 5 6 7; do
###for boost_medium
#for tpt in 1 2 3 4; do
for hpt in 1 2 3 4 5 6 7 8; do
option=${tpt}${hpt}
echo $option

rm -f muTau_SM13_svfitmass_.root
rm -f muTau_SM13_svfitmass_tUp.root
rm -f muTau_SM13_svfitmass_tDown.root

rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
rootl combineDataCardsSM.C\(1\,13\,\"svfitmass\"\)
mv muTau_SM13_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM13_${option}.root

done
done


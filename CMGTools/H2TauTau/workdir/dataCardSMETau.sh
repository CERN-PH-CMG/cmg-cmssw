#!/bin/sh


#######################################################################
#export SAMPLEPATH=eTau201253X_580Feb7
#export SAMPLEPATH=eTau201253X_5140Apr18Isolations
#export SAMPLEPATH=eTau201253X_5140May2_TauIso3HitorMVA2
export SAMPLEPATH=eTau2012_5140May27

##################### Summer13  ################################
#rootl dataCardSM.C\(2\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl dataCardSM.C\(2\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl dataCardSM.C\(2\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl combineDataCardsSM.C\(2\,0\,\"svfitmass\"\)
#mv eleTau_SM0_svfitmass.root htt_et.inputs-sm-8TeV-summer13.root

###################mT optimization#########################
#for mt in 1 2 3 4 5 6 7 8; do
#option=${mt}
#echo $option
#rm -f eleTau_SM0_svfitmass_.root
#rm -f eleTau_SM0_svfitmass_tUp.root
#rm -f eleTau_SM0_svfitmass_tDown.root
#rootl dataCardSM.C\(2\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,-${option}\) &
#rootl dataCardSM.C\(2\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,-${option}\) &
#rootl dataCardSM.C\(2\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,-${option}\)
#rootl combineDataCardsSM.C\(2\,0\,\"svfitmass\"\)
#mv eleTau_SM0_svfitmass.root htt_et.inputs-sm-8TeV-summer13_SM0_mT_${option}.root
#done

############Grand optimization of Boost###################
#for tpt in 1 2 3 4 5; do
#for hpt in 1 2 3 4 5 6 7 8; do
#option=${tpt}${hpt}
#echo $option
#
#rm -f eleTau_SM13_svfitmass_.root
#rm -f eleTau_SM13_svfitmass_tUp.root
#rm -f eleTau_SM13_svfitmass_tDown.root
#
#rootl dataCardSM.C\(2\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(2\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(2\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(2\,13\,\"svfitmass\"\)
#mv eleTau_SM13_svfitmass.root htt_et.inputs-sm-8TeV-summer13_SM13_${option}.root
#
#done
#done


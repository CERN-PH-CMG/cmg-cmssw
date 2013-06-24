#!/bin/sh


#######################################################################
#export SAMPLEPATH=muTau201253X_580Feb7
#export SAMPLEPATH=muTau201253X_5140Apr18Isolations
#export SAMPLEPATH=muTau201253X_5140May2_TauIso3HitorMVA2
#export SAMPLEPATH=muTau2012_5140May27
#export SAMPLEPATH=muTau2012_5140May31
export SAMPLEPATH=muTau2012_5160June10

#################### Summer13  ################################
cat=0
var=svfitmass
rm -f muTau_SM${cat}_${var}_.root
rm -f muTau_SM${cat}_${var}_tUp.root
rm -f muTau_SM${cat}_${var}_tDown.root
rootl dataCardSM.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\) &
rootl dataCardSM.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\) &
rootl dataCardSM.C\(1\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\) 
rootl combineDataCardsSM.C\(1\,${cat}\,\"${var}\"\)
mv muTau_SM${cat}_${var}.root htt_mt.inputs-sm-8TeV-summer13.root

###################mT optimization#########################
#for mt in 1 2 3 4 5 6 7 8; do
#option=${mt}
#echo $option
#rm -f muTau_SM0_svfitmass_.root
#rm -f muTau_SM0_svfitmass_tUp.root
#rm -f muTau_SM0_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,0\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,0\,\"svfitmass\"\)
#mv muTau_SM0_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM0_mT_${option}.root
#done

#############optimization 2 taupT categories ###################
#
#for option in 1 2 3 4 5 6; do
#echo $option
#rm -f muTau_SM23_svfitmass_.root
#rm -f muTau_SM23_svfitmass_tUp.root
#rm -f muTau_SM23_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,23\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,23\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,23\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,23\,\"svfitmass\"\)
#mv muTau_SM23_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM23_opt_tau_${option}.root
#done


#for option in 21 22 23 24 25 26 27 28; do
#echo $option
#rm -f muTau_SM13_svfitmass_.root
#rm -f muTau_SM13_svfitmass_tUp.root
#rm -f muTau_SM13_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,13\,\"svfitmass\"\)
#mv muTau_SM13_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM13_opt_higshigh_${option}.root
#done
#
#for option in 11 12 13 14 15 16 17 18; do
#echo $option
#rm -f muTau_SM20_svfitmass_.root
#rm -f muTau_SM20_svfitmass_tUp.root
#rm -f muTau_SM20_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,20\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,20\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,20\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,20\,\"svfitmass\"\)
#mv muTau_SM20_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM20_opt_higslow_${option}.root
#done

#######################June3#############################
####optimization of boost_high in 2categories
#for tpt in 4 5 6 7; do
#for hpt in 1 2 3 4 5 6 7 8; do
#option=1$tpt$hpt
#echo $option
#rm -f muTau_SM13_svfitmass_.root
#rm -f muTau_SM13_svfitmass_tUp.root
#rm -f muTau_SM13_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,13\,\"svfitmass\"\)
#mv muTau_SM13_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM13_opt_higshigh_${option}.root
#done
#done


#for tpt in 3 4 5; do
#for hpt in 1 2 3 4 5 6 7; do
#option=1$tpt$hpt
#echo $option
#rm -f muTau_SM13_svfitmass_.root
#rm -f muTau_SM13_svfitmass_tUp.root
#rm -f muTau_SM13_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,13\,\"svfitmass\"\)
#mv muTau_SM13_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM13_opt_higsmed_${option}.root
#done
#done

#for tpt in 1 2; do
#for hpt in 1 2 3 4 5 6 7; do
#option=1$tpt$hpt
#echo $option
#rm -f muTau_SM13_svfitmass_.root
#rm -f muTau_SM13_svfitmass_tUp.root
#rm -f muTau_SM13_svfitmass_tDown.root
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,${option}\) &
#rootl dataCardSM.C\(1\,13\,2012\,5\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,${option}\)
#rootl combineDataCardsSM.C\(1\,13\,\"svfitmass\"\)
#mv muTau_SM13_svfitmass.root htt_mt.inputs-sm-8TeV-summer13_SM13_opt_higslow_${option}.root
#done
#done

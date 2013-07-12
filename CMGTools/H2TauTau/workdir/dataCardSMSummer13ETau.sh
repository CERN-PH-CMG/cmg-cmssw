#!/bin/sh


#######################################################################
#export SAMPLEPATH=eTau2012_5160June23
#export SAMPLEPATH=eTau2012_5160June28
#export SAMPLEPATH=eTau2012_5160June30
export SAMPLEPATH=eTau2012_5160July1

#################### Summer13  ################################
cat=0
var=svfitmass
rm -f eleTau_SM${cat}_${var}_.root
rm -f eleTau_SM${cat}_${var}_tUp.root
rm -f eleTau_SM${cat}_${var}_tDown.root
rootl dataCardSMSummer13.C\(2\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\) &
rootl dataCardSMSummer13.C\(2\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\) &
rootl dataCardSMSummer13.C\(2\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\) 
rootl combineDataCardsSMSummer13.C\(2\,${cat}\,\"${var}\"\)
mv eleTau_SM${cat}_${var}.root htt_et.inputs-sm-8TeV-summer13.root

#var=ditaumass
#rm -f eleTau_SM${cat}_${var}_.root
#rm -f eleTau_SM${cat}_${var}_tUp.root
#rm -f eleTau_SM${cat}_${var}_tDown.root
#rootl dataCardSMSummer13.C\(2\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\) &
#rootl dataCardSMSummer13.C\(2\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\) &
#rootl dataCardSMSummer13.C\(2\,${cat}\,2012\,6\,\"${var}\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\) 
#rootl combineDataCardsSMSummer13.C\(2\,${cat}\,\"${var}\"\)
#mv eleTau_SM${cat}_${var}.root htt_et.inputs-sm-8TeV-summer13-mvis.root

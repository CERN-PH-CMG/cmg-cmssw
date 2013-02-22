
############# 2011
#rootl histosForDataCardSM.C\(1\,\"\/data\/benitezj\/Samples\/TauMu2011FlatNtpJuly26svfit\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,\"\/data\/benitezj\/Samples\/TauMu2011FlatNtpJuly26svfit\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,\"\/data\/benitezj\/Samples\/TauMu2011FlatNtpJuly26svfit\"\,\"tDown\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau2012V551Sep4\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau2012V56052XSep25\"\,\"\"\)

#############2012ABC
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct23\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct23\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct23\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)

###############2012AB: HCP analysis of ICHEP re-reco datasets
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253XAB_580Oct16\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253XAB_580Oct16\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253XAB_580Oct16\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)

#####2012D
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Dec21\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Dec21\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Dec21\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)

####2012ABCD
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Jan29\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Jan29\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Jan29\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#rootl mergeDataCardsSM.C\(1\,\"ditaumass\"\)

#######################################################################
export SAMPLEPATH=muTau201253X_580Feb7
###2012ABCD
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root htt_mt.inputs-sm-8TeV.root
#
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"ditaumass\"\)
#mv muTauSM_ditaumass.root htt_mt.inputs-sm-8TeV-mvis.root
#
###2012D
#rootl histosForDataCardSM.C\(1\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root htt_mt.inputs-sm-8TeV-2012d.root
#
#rootl histosForDataCardSM.C\(1\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"ditaumass\"\)
#mv muTauSM_ditaumass.root htt_mt.inputs-sm-8TeV-2012d-mvis.root
#
###2012ABC
#rootl histosForDataCardSM.C\(1\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root htt_mt.inputs-sm-8TeV-hcp.root
#
#rootl histosForDataCardSM.C\(1\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
#rootl mergeDataCardsSMZTT.C\(1\,\"ditaumass\"\)
#mv muTauSM_ditaumass.root htt_mt.inputs-sm-8TeV-hcp-mvis.root



#############ratios vs tau pT
#for p in 0 1 2 3 4 5 6 7 8; do
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,2${p}\)
#mv muTauSM_ditaumass_.root htt_mt.inputs-sm-8TeV-mvis-decayMode0_pT${p}.root
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,3${p}\)
#mv muTauSM_ditaumass_.root htt_mt.inputs-sm-8TeV-mvis-decayMode1_pT${p}.root
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,4${p}\)
#mv muTauSM_ditaumass_.root htt_mt.inputs-sm-8TeV-mvis-decayMode10_pT${p}.root
#done


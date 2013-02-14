
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


##for Decay mode ratio measurements
rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,0\)
rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,0\)
rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,0\)
rootl mergeDataCardsSMZTT.C\(1\,\"ditaumass\"\)
mv muTauSM_ditaumass.root htt_mt.inputs-sm-8TeV-mvis-decayMode0.root

#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,1\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,1\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,1\)
#rootl mergeDataCardsSMZTT.C\(1\,\"ditaumass\"\)
#mv muTauSM_ditaumass.root htt_mt.inputs-sm-8TeV-mvis-decayMode1.root
#
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\,10\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\,10\)
#rootl histosForDataCardSM.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\,10\)
#rootl mergeDataCardsSMZTT.C\(1\,\"ditaumass\"\)
#mv muTauSM_ditaumass.root htt_mt.inputs-sm-8TeV-mvis-decayMode10.root



##########################################################################################################
#############mT Limit Scan
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,10\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,10\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,10\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT10/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,15\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,15\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,15\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT15/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,20\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,20\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,20\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT20/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,25\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,25\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,25\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT25/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,30\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,30\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,30\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT30/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,35\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,35\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,35\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT35/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,40\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,40\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,40\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT40/.
#
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"\"\,45\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tUp\"\,45\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Oct18Sync\"\,\"tDown\"\,45\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)
#mv muTauSM_svfitmass.root ./Limits/MTScan/MT45/.

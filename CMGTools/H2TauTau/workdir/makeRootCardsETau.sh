
#############2012ABC
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253X_580Oct23\"\,\"\"\)
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253X_580Oct23\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253X_580Oct23\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(2\,\"svfitmass\"\)

###############2012AB: HCP analysis of ICHEP re-reco datasets
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253XAB_580Oct16\"\,\"\"\)
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253XAB_580Oct16\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253XAB_580Oct16\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(2\,\"svfitmass\"\)

#####2012D
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Dec21\"\,\"\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Dec21\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(1\,2012\,\"\/data\/benitezj\/Samples\/muTau201253X_580Dec21\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(1\,\"svfitmass\"\)

#####ABCD
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253X_580Jan29\"\,\"\"\)
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253X_580Jan29\"\,\"tUp\"\)
#rootl histosForDataCardSM.C\(2\,2012\,\"\/data\/benitezj\/Samples\/eTau201253X_580Jan29\"\,\"tDown\"\)
#rootl mergeDataCardsSM.C\(2\,\"svfitmass\"\)
##rootl mergeDataCardsSM.C\(2\,\"ditaumass\"\)

########################
#######################################################################
export SAMPLEPATH=eTau201253X_580Feb7
##2012ABCD
rootl histosForDataCardSM.C\(2\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSM.C\(2\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSM.C\(2\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSM.C\(2\,\"svfitmass\"\)
mv eleTauSM_svfitmass.root htt_et.inputs-sm-8TeV.root

rootl histosForDataCardSM.C\(2\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSM.C\(2\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSM.C\(2\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSM.C\(2\,\"ditaumass\"\)
mv eleTauSM_ditaumass.root htt_et.inputs-sm-8TeV-mvis.root

##2012D
rootl histosForDataCardSM.C\(2\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSM.C\(2\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSM.C\(2\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSM.C\(2\,\"svfitmass\"\)
mv eleTauSM_svfitmass.root htt_et.inputs-sm-8TeV-2012d.root

rootl histosForDataCardSM.C\(2\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSM.C\(2\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSM.C\(2\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSM.C\(2\,\"ditaumass\"\)
mv eleTauSM_ditaumass.root htt_et.inputs-sm-8TeV-2012d-mvis.root

##2012ABC
rootl histosForDataCardSM.C\(2\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSM.C\(2\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSM.C\(2\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSM.C\(2\,\"svfitmass\"\)
mv eleTauSM_svfitmass.root htt_et.inputs-sm-8TeV-hcp.root

rootl histosForDataCardSM.C\(2\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSM.C\(2\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSM.C\(2\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSM.C\(2\,\"ditaumass\"\)
mv eleTauSM_ditaumass.root htt_et.inputs-sm-8TeV-hcp-mvis.root



#######################################################################
#export SAMPLEPATH=muTau201253X_580Jan29
export SAMPLEPATH=muTau201253X_580Feb7
##2012ABCD
#rootl histosForDataCardSMTauTau.C\(1\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
#rootl histosForDataCardSMTauTau.C\(1\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
#rootl histosForDataCardSMTauTau.C\(1\,2012\,4\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSMTauTau.C\(1\,\"TauTau_svfitmass\"\)
mv muTauSM_TauTau_svfitmass.root htt_mt.inputs-sm-8TeV-TauTauSelections.root

rootl histosForDataCardSMTauTau.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,4\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSMTauTau.C\(1\,\"TauTau_ditaumass\"\)
mv muTauSM_TauTau_ditaumass.root htt_mt.inputs-sm-8TeV-TauTauSelections-mvis.root

##2012D
rootl histosForDataCardSMTauTau.C\(1\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,3\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSMTauTau.C\(1\,\"TauTau_svfitmass\"\)
mv muTauSM_TauTau_svfitmass.root htt_mt.inputs-sm-8TeV-TauTauSelections-2012d.root

rootl histosForDataCardSMTauTau.C\(1\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,3\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSMTauTau.C\(1\,\"TauTau_ditaumass\"\)
mv muTauSM_TauTau_ditaumass.root htt_mt.inputs-sm-8TeV-TauTauSelections-2012d-mvis.root

##2012ABC
rootl histosForDataCardSMTauTau.C\(1\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,2\,\"svfitmass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSMTauTau.C\(1\,\"TauTau_svfitmass\"\)
mv muTauSM_TauTau_svfitmass.root htt_mt.inputs-sm-8TeV-TauTauSelections-hcp.root

rootl histosForDataCardSMTauTau.C\(1\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tUp\"\)
rootl histosForDataCardSMTauTau.C\(1\,2012\,2\,\"ditaumass\"\,\"\/data\/benitezj\/Samples\/${SAMPLEPATH}\"\,\"tDown\"\)
rootl mergeDataCardsSMTauTau.C\(1\,\"TauTau_ditaumass\"\)
mv muTauSM_TauTau_ditaumass.root htt_mt.inputs-sm-8TeV-TauTauSelections-hcp-mvis.root


import FWCore.ParameterSet.Config as cms

def addMETSig( process , postfix ):
    print "adding PFMET significance calculation to patDefaultSequence"+postfix
    process.load('CMGTools.Common.miscProducers.metSignificance_cfi')
    #remove the default muon and electron collections to work with AK5,AK7 as well, but need to reconfigure AK5LC because it is a clone of AK5
    process.metSignificance.inputPATElectrons = cms.InputTag('')   
    process.metSignificance.inputPATMuons = cms.InputTag('')
    process.metSignificance.inputPFJets = cms.InputTag('pfJets'+postfix)
    process.metSignificance.inputPFCandidates = cms.InputTag('pfNoJet'+postfix)
    setattr(process,"PFMETSignificance"+postfix, process.metSignificance.clone())
    getattr(process,"patDefaultSequence"+postfix).insert(getattr(process,"patDefaultSequence"+postfix).index(getattr(process,"patMETs"+postfix)),getattr(process,"PFMETSignificance"+postfix))

import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned

def addJetSubstructureSequence( process , jetCorrections , runOnMC ):
    process.ak5PrunedPFlow = ak5PFJetsPruned.clone(
        src = process.pfJetsAK5.src,
        doAreaFastjet = cms.bool(True)
        )
    addJetCollection(process, 
                 cms.InputTag('ak5PrunedPFlow'),         
                 'AK5Pruned', 'PF',
                 doJTA=False,            # Run Jet-Track association & JetCharge
                 doBTagging=False,       # Run b-tagging
                 jetCorrLabel= ('AK5PFchs', cms.vstring(jetCorrections)),
                 doType1MET=False,
                 doL1Cleaning=False,
                 doL1Counters=False,
                 genJetCollection = cms.InputTag("ak5GenJetsNoNu"),
                 doJetID = False
                 )
    process.patJetsAK5PrunedPF.addDiscriminators = cms.bool(False)
    process.patJetsAK5PrunedPF.embedCaloTowers = cms.bool(False)
    process.patJetsAK5PrunedPF.embedGenJetMatch = cms.bool(False)
    process.patJetsAK5PrunedPF.embedGenPartonMatch = cms.bool(False)
    process.patJetsAK5PrunedPF.addGenPartonMatch = cms.bool(False)
    process.patJetsAK5PrunedPF.getJetMCFlavour = cms.bool(False)
    process.patJetsAK5PrunedPF.embedPFCandidates = cms.bool(False)
    process.patJetCorrFactorsAK5PrunedPF.rho = cms.InputTag("kt6PFJets","rho")
    process.patJetsAK5PrunedPF.embedGenJetMatch = cms.bool(runOnMC)
    process.patJetsAK5PrunedPF.addGenJetMatch = cms.bool(runOnMC)
    if runOnMC:
        process.jetSubstructureSequence = cms.Sequence(process.ak5PrunedPFlow*process.patJetGenJetMatchAK5PrunedPF*process.patJetCorrFactorsAK5PrunedPF*process.patJetsAK5PrunedPF*process.selectedPatJetsAK5PrunedPF)
    else:
        process.jetSubstructureSequence = cms.Sequence(process.ak5PrunedPFlow*process.patJetCorrFactorsAK5PrunedPF*process.patJetsAK5PrunedPF*process.selectedPatJetsAK5PrunedPF)
    # disable embedding of genjets in PAT jets to avoid duplication of the genjet collection
    if runOnMC:
        process.patJetsAK5PrunedPF.embedGenJetMatch=False

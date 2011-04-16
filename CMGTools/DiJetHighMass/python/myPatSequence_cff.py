import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *

def myPatSequence( process, runOnData=True  ): 

#    jetCorrString = cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual'])
    jetCorrString = cms.vstring(['L2Relative', 'L3Absolute'])

    if runOnData:
        switchOnTrigger( process )
        removeMCMatching(process, ['All'])
        #jetCorrString = cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual'])
        #RunOnData(process, ['All'])
        

    addJetCollection(process,cms.InputTag('ak7CaloJets'),
                     'AK7', 'Calo',
                     doJTA        = True,
                     doBTagging   = False,
                     jetCorrLabel = ('AK7Calo', jetCorrString),
                     doType1MET   = False,
                     doL1Cleaning = False,                 
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak7GenJets"),
                     doJetID      = False
                     )


    addJetCollection(process,cms.InputTag('ak5CaloJets'),
                     'AK5', 'Calo',
                     doJTA        = True,
                     doBTagging   = False,
                     jetCorrLabel = ('AK5Calo', jetCorrString),
                     doType1MET   = False,
                     doL1Cleaning = False,                 
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak5GenJets"),
                     doJetID      = False
                     )


    addJetCollection(process,cms.InputTag('ak7PFJets'),
                     'AK7', 'PF',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK7PF', jetCorrString),
                     doType1MET   = False,
                     doL1Cleaning = False,                 
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak7GenJets"),
                     doJetID      = False
                     )



  
    addJetCollection(process,cms.InputTag('ak5PFJets'),
                     'AK5', 'PF',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK5PF', jetCorrString),
                     doType1MET   = False,
                     doL1Cleaning = False,                 
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak5GenJets"),
                     doJetID      = False
                     )



    process.selectedPatJetsAK7Calo.cut = "pt()>30"
    process.selectedPatJetsAK7PF.cut = "pt()>10"
    process.selectedPatJetsAK5Calo.cut = "pt()>30"
    process.selectedPatJetsAK5PF.cut = "pt()>10"


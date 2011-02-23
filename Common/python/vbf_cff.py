import FWCore.ParameterSet.Config as cms

# from CMGTools.Common.factories.vbfEventGenJetGenJet_cfi import vbfEventGenJetGenJet
from CMGTools.Common.factories.vbfEventJetJet_cfi import vbfEventJetJet

vbfEventJetJetPF = vbfEventJetJet.clone()
vbfEventJetJetPF.ForwardJets = 'selectedPatJetsPFlow'
vbfEventJetJetPF.CentralObjects = 'selectedPatJetsPFlow'

vbfEventJetJetCalo = vbfEventJetJet.clone()
vbfEventJetJetCalo.ForwardJets = 'selectedPatJets'
vbfEventJetJetCalo.CentralObjects = 'selectedPatJets'



vbfSequence = cms.Sequence(
    vbfEventJetJetPF +
    vbfEventJetJetCalo
    
    # vbfEventGenJetGenJet
    )

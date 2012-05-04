import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import *

# 44X does not work - both METs are identical for now
#produce type 1 corrected MET
# patMETs.metSource = 'pfType1CorrectedMet'
# dammit! the PAT MET sequence contains jet clustering modules... 
# producePFMETCorrections.remove( kt6PFJets )
# producePFMETCorrections.remove( ak5PFJets )
patMETs.metSource = 'pfMet'
patMETs.addMuonCorrections = False

# adding raw PFMET also
patMETsRaw = patMETs.clone()
patMETsRaw.addMuonCorrections = False
patMETsRaw.metSource = 'pfMet'

PATMetSequence = cms.Sequence(
    # producePFMETCorrections + 
    patMETs +
    patMETsRaw
    )

from CMGTools.Common.Tools.cmsswRelease import cmsswIs52X
if cmsswIs52X():
    from PhysicsTools.PatAlgos.recoLayer0.jetMETCorrections_cff import *
    producePFMETCorrections.remove( kt6PFJets )
    producePFMETCorrections.remove( ak5PFJets )
    patMETs.metSource = 'pfType1CorrectedMet'
    PATMetSequence.insert(0, producePFMETCorrections)
    

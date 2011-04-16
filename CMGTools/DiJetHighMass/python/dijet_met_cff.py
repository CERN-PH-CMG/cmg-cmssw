import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector
from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms


postfix = 'AK7'

# MHT from PFJets
cmgMHTPFJets = cmgBaseMET.clone()
cmgMHTPFJets.cfg.inputCollection = "cmgBaseJet"+postfix+"PF"
mhtPFJetsHistograms = baseMETHistograms.clone()
mhtPFJetsHistograms.inputCollection = 'cmgMHTPFJets'

# MHT from PFJets, pt threshold 50
cmgMHTPFLeadJets = cmgMHTPFJets.clone()
cmgMHTPFLeadJets.cfg.inputCollection = "cmgBaseLeadJet"+postfix+"PF"
mhtPFLeadJetsHistograms = baseMETHistograms.clone()
mhtPFLeadJetsHistograms.inputCollection = 'cmgMHTPFLeadJets'

# MHT from CaloJets
cmgMHTCaloJets = cmgBaseMET.clone()
cmgMHTCaloJets.cfg.inputCollection = "cmgBaseJet"+postfix+"Calo"
mhtCaloJetsHistograms = baseMETHistograms.clone()
mhtCaloJetsHistograms.inputCollection = 'cmgMHTCaloJets'

# MHT from CaloJets, pt threshold 50
cmgMHTCaloLeadJets = cmgMHTCaloJets.clone()
cmgMHTCaloLeadJets.cfg.inputCollection = "cmgBaseLeadJet"+postfix+"Calo"
mhtCaloLeadJetsHistograms = baseMETHistograms.clone()
mhtCaloLeadJetsHistograms.inputCollection = 'cmgMHTCaloLeadJets'




# MET from PFCandidates
cmgMETPFCandidates = cmgBaseMET.clone()
cmgMETPFCandidates.cfg.inputCollection = "particleFlow" 
pfMETHistograms = baseMETHistograms.clone()
pfMETHistograms.inputCollection = 'cmgMETPFCandidates'

# MET from PFCandidates, pt threshold 2 (to remove pile-up)
cmgMETPFCandidates2 = cmgMETPFCandidates.clone()
cmgMETPFCandidates2.cfg.ptThreshold = 2.0
pfMET2Histograms = baseMETHistograms.clone()
pfMET2Histograms.inputCollection = 'cmgMETPFCandidates2'



metHistograms = cms.Sequence(
    mhtPFJetsHistograms +
    mhtPFLeadJetsHistograms +
    mhtCaloJetsHistograms +
    mhtCaloLeadJetsHistograms +
    pfMETHistograms +
    pfMET2Histograms
    )

metSequence = cms.Sequence(
    cmgMHTPFJets +
    cmgMHTPFLeadJets +
    cmgMHTCaloJets +    
    cmgMHTCaloLeadJets +    
    cmgMETPFCandidates +
    cmgMETPFCandidates2 +
    metHistograms
    )

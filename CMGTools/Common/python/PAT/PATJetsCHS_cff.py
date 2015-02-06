import FWCore.ParameterSet.Config as cms

# taking all PFCandidates which are not PU charged hadron
from RecoJets.JetProducers.ak5PFJets_cfi import *
ak5PFJetsCHS = ak5PFJets.clone()
ak5PFJetsCHS.src = 'pfNoPileUp'

ak5PFJetsCHS.doAreaFastjet = True
ak5PFJetsCHS.doRhoFastjet = False

#NOTE: please refer to the cfg, where the cloning of the PAT jet sequence is done
# could be rewritten here, but painful...

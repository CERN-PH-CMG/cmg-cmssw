import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 


jetAna = cfg.Analyzer(
    'PFSimAnalyzer',
    jetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    genJetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    jetPt = 20.,
    jetEta = 4.7,
    btagSFseed = 123456,
    relaxJetId = True, 
    )


###############################################################################

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = ['gen.root'],
    xSection = 225.197, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


selectedComponents = [TTJets]  

sequence = cfg.Sequence( [
    jetAna,
   ] )

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

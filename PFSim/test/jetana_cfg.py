import copy
import os
import glob
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 


effAndSmear = cfg.Analyzer(
    'EffAndSmearAnalyzer',
    jetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    genJetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    )

jetAna = cfg.Analyzer(
    'PFSimJetAnalyzer',
    # jetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    # genJetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    # genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    jetPt = 20.,
    jetEta = 4.7,
    btagSFseed = 123456,
    relaxJetId = True, 
    )

treeProducer = cfg.Analyzer(
    'PFSimTreeProducer'
    )


###############################################################################

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = sorted(glob.glob('Prod_TT_20k/Job*/*.root')),
    xSection = 225.197, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )
TTJets.splitFactor = 1


selectedComponents = [TTJets]  

sequence = cfg.Sequence( [
    effAndSmear,
    jetAna,
    treeProducer
   ] )

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

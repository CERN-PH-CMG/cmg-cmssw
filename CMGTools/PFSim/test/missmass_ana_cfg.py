import copy
import os
import glob
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 




reader = cfg.Analyzer(
    'ObjectReader',
    # muonCol = ('cmgMuonSel', 'vector<cmg::Muon>'),
    # electronCol = ('cmgElectronSel', 'vector<cmg::Electron>'),
    muonCol = None,
    electronCol = None,
    # only if you have PFJets
    jetCol = None,
    # jetCol = ('ak5PFJets', 'std::vector<reco::PFJet>'),
    simJetCol = ('jets', 'std::vector<reco::LeafCandidate>'),
    # genJetCol = ('ak5GenJets', 'std::vector<reco::GenJet>'),
    genJetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    simPartCol = ('pfsim', 'std::vector<reco::LeafCandidate>'),
    )


jetAna = cfg.Analyzer(
    'PFSimJetAnalyzer',
    jetPt = 10.,
    jetEta = 5.0,
    btagSFseed = 123456,
    relaxJetId = True, 
    )

missMassAna = cfg.Analyzer(
    'MissMassAnalyzer'
    )

treeProducer = cfg.Analyzer(
    'PFSimTreeProducer'
    )


###############################################################################

# need cross-sections 

ZH = cfg.MCComponent(
    name = 'ZH',
    files = ['zh_gensim.root'],
    xSection = 0.131, 
    nGenEvents = 20000,
    triggers = [],
    effCorrFactor = 1 )

WWH = cfg.MCComponent(
    name = 'WWH',
    files = ['wwh_gensim.root'],
    xSection = 0.0267, 
    nGenEvents = 5000,
    triggers = [],
    effCorrFactor = 1 )



selectedComponents = [ZH]  

sequence = cfg.Sequence( [
    # effAndSmear,
    reader,
    jetAna,
    missMassAna,
    treeProducer
   ] )

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

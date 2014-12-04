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
    # jetCol = ('ak5PFJets', 'std::vector<reco::PFJet>'),
    # comment above and uncomment below when doing gensim
    jetCol = None,
    simJetCol = ('jets', 'std::vector<reco::LeafCandidate>'),
    # genJetCol = ('ak5GenJets', 'std::vector<reco::GenJet>'),
    # comment above and uncomment below when doing gensim
    genJetCol = ('genJets', 'std::vector<reco::LeafCandidate>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    simPartCol = ('pfsim', 'std::vector<reco::LeafCandidate>')
    )

jetAna = cfg.Analyzer(
    'PFSimJetAnalyzer',
    jetPt = 10.,
    jetEta = 5.0,
    btagSFseed = 123456,
    relaxJetId = True, 
    )

particleAna = cfg.Analyzer(
    'ParticleAnalyzer'
    )

treeProducer = cfg.Analyzer(
    'PFSimTreeProducer'
    )


###############################################################################

testComp = cfg.MCComponent(
    name = 'Test',
    files = ['gensim.root'],
    xSection = 1, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )
testComp.splitFactor = 1


selectedComponents = [testComp]  

sequence = cfg.Sequence( [
    # effAndSmear,
    reader,
    jetAna,
    particleAna,
    treeProducer
   ] )

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

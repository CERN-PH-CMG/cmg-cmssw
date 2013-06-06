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
    jetCol = ('jets', 'vector<reco::LeafCandidate>'),
    genJetCol = ('genJets', 'vector<reco::LeafCandidate>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    )


jetAna = cfg.Analyzer(
    'PFSimJetAnalyzer',
    jetPt = 10.,
    jetEta = 5.0,
    btagSFseed = 123456,
    relaxJetId = True, 
    )

treeProducer = cfg.Analyzer(
    'PFSimTreeProducer'
    )


###############################################################################

TTJets = cfg.MCComponent(
    name = 'TTJets',
    # files = sorted(glob.glob('Prod_TT_20k/Job*/*.root')),
    files = ['sim.root'],
    xSection = 225.197, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )
TTJets.splitFactor = 1


selectedComponents = [TTJets]  

sequence = cfg.Sequence( [
    # effAndSmear,
    reader,
    jetAna,
    treeProducer
   ] )

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

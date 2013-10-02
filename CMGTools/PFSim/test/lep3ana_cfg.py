import copy
import os
import glob
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 


effAndSmear = cfg.Analyzer(
    'EffAndSmearAnalyzer',
    genPartCol = ('genParticlesPruned', 'std::vector<reco::GenParticle>'),
    genJetCol = ('genJetSel',
                 'vector<cmg::PhysicsObjectWithPtr< edm::Ptr<reco::GenJet> > >')
    )


reader = cfg.Analyzer(
    'ObjectReader',
    muonCol = ('cmgMuonSel', 'vector<cmg::Muon>'),
    electronCol = ('cmgElectronSel', 'vector<cmg::Electron>'),
    jetCol = ('cmgPFJetSel', 'vector<cmg::PFJet>'),
    genJetCol = ('genJetSel',
                 'vector<cmg::PhysicsObjectWithPtr< edm::Ptr<reco::GenJet> > >'),
    genPartCol = ('genParticlesPruned', 'std::vector<reco::GenParticle>'),
    )


jetAna = cfg.Analyzer(
    'PFSimJetAnalyzer',
    jetPt = 10.,
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


HZ_HZHA = cfg.MCComponent(
    name = 'ee_HZ',
    files = ['cmgTuple_HZHA_10.root'],
    xSection = 0.2, 
    nGenEvents = 18000,
    triggers = [],
    effCorrFactor = 1 )

selectedComponents = [HZHA]  

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

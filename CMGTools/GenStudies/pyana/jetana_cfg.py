import copy
import os
import glob
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 




reader = cfg.Analyzer(
    'GenObjectReader',
    genJetCol = ('genJetPt10', 'std::vector<reco::GenJet>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    )


jetAna = cfg.Analyzer(
    'GenJetAnalyzer',
    jetPt = 10.,
    jetEta = 5.0,
    btagSFseed = 123456,
    relaxJetId = True, 
    )

treeProducer = cfg.Analyzer(
    'GenTreeProducer'
    )


###############################################################################

testComp = cfg.MCComponent(
    name = 'Test',
    files = ['gen.root'],
    xSection = 1, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )
testComp.splitFactor = 1


selectedComponents = [testComp]  

sequence = cfg.Sequence( [
    reader,
    # jetAna,
    # treeProducer
   ] )

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

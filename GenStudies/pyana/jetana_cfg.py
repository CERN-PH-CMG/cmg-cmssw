import os, glob, sys
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.GenStudies.analyzers import *

reader = cfg.Analyzer(
    'GenObjectReader',
    genJetCol = ('genJetPt10', 'std::vector<reco::GenJet>'),
    genPartCol = ('genParticles', 'std::vector<reco::GenParticle>'),
    )

jetAna = cfg.Analyzer(
    'GenJetAnalyzer',
    jetPt = 10.,
    jetEta = 5.0
    )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    Mjj = 500,
    deltaEta = 3.5,
#    Mjj = 700,
#    deltaEta = 4.0,
    cjvPtCut = 30.
    )

treeProducer = cfg.Analyzer(
    'GenTreeProducer'
    )

multiProducer = cfg.Analyzer(
    'MultiTreeProducer'
    )


###############################################################################

testComp = cfg.MCComponent(
    name = 'Test',
#    files = ['gen.root'],
    files = ['/data1/ytakahas/data/gen_atlas.root'],
#    files = ['/data1/ytakahas/data/gen_cms.root'],
    xSection = 1, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

testComp.splitFactor = 1
#testComp.splitFactor = 6

selectedComponents = [testComp]  

sequence = cfg.Sequence( [
    reader,
    jetAna,
    vbfAna,
    treeProducer,
    multiProducer
   ] )


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

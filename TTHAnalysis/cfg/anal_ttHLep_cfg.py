import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *


ttHLepAna = cfg.Analyzer(
    'ttHLepAnalyzerBase',

    rhoMuon= 'kt6PFJetsCentralNeutral',
    rhoElectron = 'kt6PFJets',
    muons='cmgMuonSel',
    electrons='cmgElectronSel',
    
    )


def createTreeProducer( ana ):
    tp = cfg.Analyzer( '_'.join( ['ttHLepTreeProducerBase','ttHLepAnalyzerBase'] ),
                       anaName = 'ttHLepAnalyzerBase'
                       )
    return tp


# for debugging 
stopper = cfg.Analyzer(
    'Stopper'
    )

from CMGTools.TTHAnalysis.samples.allSamples import * 

sequence = cfg.Sequence([
    ttHLepAna,
    createTreeProducer( ttHLepAna ),
    ])


selectedComponents = [
    TTH
    ]


# for c in selectedComponents:
#     c.splitFactor = 3

test = 1
if test==1:
    comp = TTH
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:1]
    
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

cfg.printComps(config.components, True)

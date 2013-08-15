import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 

ana6jet = cfg.Analyzer(
    "SixJetAnalyzer",
    )
tree = cfg.Analyzer(
    "SixJetTreeProducer",
    )

from CMGTools.LEP3.samples.allSamples import * 

sequence = cfg.Sequence( [
    ana6jet,
    tree
])

selectedComponents = [
    # ELEL,
    # GAGA,
    HZVIS,
    # INVIS,
    # MUMU,
    QQBAR,
    # TAUTAU,
    # WENU,
    WW,
    # ZEE,
    # ZNNB,
    ZZ,
    ]


# for c in selectedComponents:
#     c.splitFactor = 3

test = 0
if test==1:
    comp = WW
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:1]
    
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

cfg.printComps(config.components, True)

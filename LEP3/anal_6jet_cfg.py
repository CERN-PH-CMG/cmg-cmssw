import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 

ana6jet = cfg.Analyzer(
    "SixJetAnalyzer"
    )

from CMGTools.LEP3.samples.allSamples import * 

sequence = cfg.Sequence( [
    ana6jet,
])

QQBAR.splitFactor = 50
WW.splitFactor = 50
ZZ.splitFactor = 20

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

test = 1
if test==1:
    comp = HZVIS
    selectedComponents = [comp]
    comp.splitFactor = 1
    
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

cfg.printComps(config.components, True)

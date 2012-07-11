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

HZVIS.splitFactor = 3
QQBAR.splitFactor = 3
WW.splitFactor = 3
ZZ.splitFactor = 3

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
    comp.files = comp.files[:1]
    
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

cfg.printComps(config.components, True)

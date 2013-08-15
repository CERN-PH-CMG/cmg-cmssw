import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 

fourJetAna = cfg.Analyzer(
    'FourJetAnalyzer',
    npfj = (3,5),       # at least 8 PF particle in 3 of the 4 jets
    ntkj = (4,1),       # at least 1 PF charged hadron in 4 of the 4 jets
    minM = 0.5,         # No jet with a jet mass smaller than 0.5 GeV
    mVis = 180.,        # total visible mass in excess of 180 GeV,
    chi2 = 1000.,       # total chi**2 of the beta4 "fit" smaller than 1000. (i.e., all fit energies > 0.)    
    pair_mass = 0.,     # No jet pair should have a mass smaller than 30 GeV
    pair_cijkl = 1000., # Tue smallest sum of cos(ij)+cos(kl) should be larger than -1.1
    pair_sumtet = 0.,   # The sum of the four smallest jet-jet angles must be larger than 350 degrees
    h_mass = 105.,      # The Higgs candidate mass should be larger than 90 GeV
    z_mass = (80.,110.),# The Z candidate jet pair should have a mass closer to the Z mass than 100 GeV
    )


def createTreeProducer( ana ):
    tp = cfg.Analyzer( '_'.join( ['FourJetTreeProducer','FourJetAnalyzer'] ),
                       anaName = 'FourJetAnalyzer'
                       )
    return tp


# for debugging 
stopper = cfg.Analyzer(
    'Stopper'
    )

from CMGTools.LEP3.samples.allSamples import * 

sequence = cfg.Sequence([
    fourJetAna,
    createTreeProducer( fourJetAna ),
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
    comp = HZVIS
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:1]
    
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

cfg.printComps(config.components, True)

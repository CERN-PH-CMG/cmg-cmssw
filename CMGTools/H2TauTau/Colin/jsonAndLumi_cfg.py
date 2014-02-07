import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import mu_id_taumu_2012, mu_iso_taumu_2012
from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift
from CMGTools.RootTools.RootTools import * 

   

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )


#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.cmgTuple_data_ColinOct10 import * 

#########################################################################################


selectedComponents = [data_Run2012C_v2]

# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |      17991 |   1.464(/fb) |   1.464(/fb) |
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 |      46975 |   4.358(/fb) |   4.358(/fb) |
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 |       3425 | 281.155(/pb) | 281.155(/pb) |

data_Run2012C_v2.triggers = [
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3',
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6',
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7'
    ]

sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna,
    triggerAna,
   ] )

for c in allsamples:
    c.splitFactor = 400

test = 0
if test==1:
    comp = data_Run2012C_v2
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:10]
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)


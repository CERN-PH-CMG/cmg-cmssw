import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles


from CMGTools.RootTools.RootTools import * 



WNJetsAna = cfg.Analyzer(
    'WNJetsAnalyzer',
    verbose = False
    )


#########################################################################################


WJets = cfg.MCComponent(
    name = 'WJets',
    files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', 'cmgTuple.*root'),
    xSection = 31314.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


#########################################################################################

selectedComponents = [WJets]

sequence = cfg.Sequence( [
    WNJetsAna, 
   ] )


comp = WJets
comp.splitFactor = 14

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

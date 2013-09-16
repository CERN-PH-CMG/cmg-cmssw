import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.RootTools import * 

WNJetsAna = cfg.Analyzer(
    'WNJetsAnalyzer',
    verbose = False
    )


#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
filePattern = 'cmgTuple.*root'
cache = False
WJets.files  = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0', 'cmgtools', filePattern, cache)
WJets.splitFactor = 20
W1Jets.files = getFiles('/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0', 'cmgtools', filePattern, cache) 
W1Jets.splitFactor = 20
W2Jets.files = getFiles('/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0', 'cmgtools', filePattern, cache)
W2Jets.splitFactor = 20
W3Jets.files = getFiles('/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0', 'cmgtools', filePattern, cache)
W3Jets.splitFactor = 20
W4Jets.files = getFiles('/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0', 'cmgtools', filePattern, cache)
W4Jets.splitFactor = 20

#########################################################################################

selectedComponents = [WJets, W1Jets, W2Jets, W3Jets, W4Jets]


sequence = cfg.Sequence( [
    WNJetsAna,
   ] )

test = 0
if test==1:
    comp = WJets
#    comp.files = comp.files[:20]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:10]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

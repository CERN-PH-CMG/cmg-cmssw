import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

#need to put this to false for the powheg
ZAna.doMad = False

sequence = cfg.Sequence(CoreZsequence)


from CMGTools.WMass.samples_Wmass import * 

selectedMADComp=[DYJets,WJets,TTJets,WW,WZ,ZZ, WJets_8, DYJets_8, DYJets_8]
selectedPOWComp=[DYJets1, DYJets2, DYJets3, DYJets4, WJetsPlus1, WJetsPlus2, WJetsPlus3, WJetsPlus4, WJetsPlus5, WJetsPlus6, WJetsMinus1, WJetsMinus2, WJetsMinus3]

#-------- HOW TO RUN

test = 1
if test==1:
    # test a single component, using a single thread.
##    comp = DYJets
    comp = data_Run2011AB_12Oct2013_v1
    comp.files = comp.files[:]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    # test all components (1 thread per component).
    for comp in selectedPOWComp:
        comp.splitFactor = 1
        comp.files = comp.files[:]
        
        
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
        

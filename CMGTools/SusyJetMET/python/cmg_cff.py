import FWCore.ParameterSet.Config as cms

from CMGTools.Common.jet_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.runInfoAccounting_cfi import *

from CMGTools.SusyJetMET.Skims.RA2Skim_cff import * 

# use run info accounting here? 

cmgTupleSequence = cms.Sequence(
    pfJetSequence +
    pfMetSequence    
    ) 

cmgSkimSequence = cms.Sequence(
    RA2SkimSequence
    )

cmgSequence = cms.Sequence(
    runInfoAccounting + 
    cmgTupleSequence +
    cmgSkimSequence
    )

# selecting events with at least 3 jets with pT > 50 

from CMGTools.Common.Skims.selEvents2HighPtJets_cff import *
highPtJets.cut = 'pt()>50'
filterHighPtJets.minNumber = 3 

#COLIN: need to check how MHT is computed! eta cut
from CMGTools.Common.Skims.selEventsHighMET_cff import *
highMET.src = 'cmgMHTPFJets30'
highMET.cut = ('pt()>150')

#COLIN: need to implement the HT cut

# RA2 skimming sequence 

RA2SkimSequence = cms.Sequence(
    selEvents2HighPtJetsSequence +
    selEventsHighMETSequence
    )

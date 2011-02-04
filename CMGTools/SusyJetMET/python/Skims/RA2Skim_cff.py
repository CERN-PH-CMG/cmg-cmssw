# selecting events with at least 3 jets with pT > 50 

from CMGTools.Common.jet_cff import *
from CMGTools.Common.skims.selEvents2HighPtJets_cff import *
from CMGTools.Common.skims.selEventsHighMET_cff import *

cmgPFJetSel.cut = 'pt()>50 && abs(eta())<2.5'
cmgPFJetCount.minNumber = 3 

cmgPFBaseJetSel.cut = cmgPFJetSel.cut 

#COLIN: need to check how MHT is computed! eta cut
highMET.src = 'cmgMHTPFJets30'
highMET.cut = ('pt()>150')

#COLIN: need to implement the HT cut

# RA2 skimming sequence 

RA2SkimSequence = cms.Sequence(
    cmgPFJetSel +
    cmgPFJetCount +
    cmgPFBaseJetSel +
    selEventsHighMETSequence
    )

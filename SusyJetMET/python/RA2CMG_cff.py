# selecting events with at least 3 jets with pT > 50 

from CMGTools.Common.analysis_cff import *

cmgPFJetSel.cut = 'pt()>50 && abs(eta())<2.5'
cmgPFJetCount.minNumber = 3 

cmgPFBaseJetSel.cut = cmgPFJetSel.cut 

#COLIN: need to check how MHT is computed! eta cut
cmgPFMETSel.src = 'cmgMHTPFJets30'
cmgPFMETSel.cut = ('pt()>150')

RA2CMGSequence = cms.Sequence(
    analysis
    )

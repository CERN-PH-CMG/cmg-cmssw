import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *

from CMGTools.H2TauTau.Colin.objects.tauMuCuts_cff import * 

cmgTauMu.cuts = tauMuCuts.clone()

# pt cut on the tau and mu leg. taking a margin w/r to the AN
# cmgTauMuSel.cut = 'leg1().pt()>18 && leg2().pt()>14'

# removing di-taus for which both legs come from the same object. 
# cmgTauMuSel.cut._value += ' && mass()>10'

cmgTauMuSel.cut = 'getSelection("cuts_skimming")'

# ---- tau e channel : for now, same cuts as for tau mu

# cmgTauESel.cut = cmgTauMuSel.cut

objectSequence = cms.Sequence(
    diTauSequence
    )

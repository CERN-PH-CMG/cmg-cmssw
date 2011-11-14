import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauMuCor_cfi import *

from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit as cmgTauMuCorPreSelSVFit

from CMGTools.H2TauTau.objects.tauMuCuts_cff import * 

cmgTauMu.cuts = tauMuCuts.clone()
cmgTauMuCor.cuts = tauMuCuts.clone()

cmgTauMuSel.cut = 'getSelection("cuts_skimming")'

cmgTauMuFullSel = cmgTauMuSel.clone( src = 'cmgTauMuSel',
                                     cut = 'getSelection("cuts_baseline")' )


cmgTauMuCorPreSel = cmgTauMuSel.clone( src = 'cmgTauMuCor' )

cmgTauMuCorFullSelSVFit = cmgTauMuFullSel.clone( src = 'cmgTauMuCorPreSelSVFit' ) 

recoilCorMETTauMu =  recoilCorrectedMET.clone( recBosonSrc = 'cmgTauMuSel')
cmgTauMuCor.cfg.metCollection = 'recoilCorMETTauMu'

tauMuSequence = cms.Sequence( cmgTauMu +
                              cmgTauMuSel +
                              cmgTauMuFullSel + 
                              recoilCorMETTauMu +
                              cmgTauMuCor + 
                              cmgTauMuCorPreSel +
                              cmgTauMuCorPreSelSVFit  + 
                              cmgTauMuCorFullSelSVFit )


metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               tauMuSequence )

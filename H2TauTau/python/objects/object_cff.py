import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauMuCor_cfi import *

from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit as cmgTauMuCorSelSVFit

from CMGTools.H2TauTau.objects.tauMuCuts_cff import * 

cmgTauMu.cuts = tauMuCuts.clone()
cmgTauMuCor.cuts = tauMuCuts.clone()

cmgTauMuSel.cut = 'getSelection("cuts_skimming")'

cmgTauMuCorSel = cmgTauMuSel.clone( src = 'cmgTauMuCor' )

cmgTauMuBaselineSel = cmgTauMuSel.clone( src = 'cmgTauMuSel',
                                         cut = 'getSelection("cuts_baseline")' )
cmgTauMuCorBaselineSel = cmgTauMuBaselineSel.clone( src = 'cmgTauMuCorSelSVFit' ) 

recoilCorMETTauMu =  recoilCorrectedMET.clone( recBosonSrc = 'cmgTauMuSel')
cmgTauMuCor.cfg.metCollection = 'recoilCorMETTauMu'

tauMuSequence = cms.Sequence( cmgTauMu +
                              cmgTauMuSel +
                              cmgTauMuBaselineSel + 
                              recoilCorMETTauMu +
                              cmgTauMuCor + 
                              cmgTauMuCorSel +
                              cmgTauMuCorSelSVFit  + 
                              cmgTauMuCorBaselineSel )





metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               tauMuSequence )

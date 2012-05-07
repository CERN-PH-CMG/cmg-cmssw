import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauEleCuts_cff import * 

from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgTauEleCor_cfi import cmgTauEleCor 
from CMGTools.H2TauTau.objects.tauEleSVFit_cfi import tauEleSVFit 


# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgTauEle.cuts = tauEleCuts.clone()
cmgTauEle.cfg.leg1Collection = 'cmgTauScaler'

# preselection 
cmgTauElePreSel = cmgTauEleSel.clone( cut = 'getSelection("cuts_baseline")')

# full selection
cmgTauEleFullSel = cmgTauEleSel.clone( src = 'cmgTauElePreSel',
                                     cut = 'getSelection("cuts_baseline")' )

tauEleStdSequence = cms.Sequence( cmgTauScaler +
                                 cmgTauEle +
                                 cmgTauElePreSel +
                                 cmgTauEleFullSel )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# recoil correction

recoilCorMETTauEle =  recoilCorrectedMETTauEle.clone( recBosonSrc = 'cmgTauElePreSel')

cmgTauEleCorPreSel = cmgTauEleCor.clone()
cmgTauEleCorPreSel.cfg.metCollection = 'recoilCorMETTauEle'
cmgTauEleCorPreSel.cfg.diObjectCollection = 'cmgTauElePreSel'

# SVFit

cmgTauEleCorSVFitPreSel = tauEleSVFit.clone()
cmgTauEleCorSVFitPreSel.diTauSrc = 'cmgTauEleCorPreSel'

cmgTauEleCorSVFitFullSel = cmgTauEleFullSel.clone( src = 'cmgTauEleCorSVFitPreSel' ) 


tauEleCorSVFitSequence = cms.Sequence( recoilCorMETTauEle +
                                      cmgTauEleCorPreSel +
                                      cmgTauEleCorSVFitPreSel +
                                      cmgTauEleCorSVFitFullSel )

tauEleSequence = cms.Sequence( tauEleStdSequence +
                              tauEleCorSVFitSequence
                              )


# the following sequence is generic
metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               tauEleSequence )

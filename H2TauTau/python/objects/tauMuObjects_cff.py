import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauMuCuts_cff import * 

from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgTauMuCor_cfi import cmgTauMuCor 
from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit 



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgTauMu.cuts = tauMuCuts.clone()
cmgTauMu.cfg.leg1Collection = 'cmgTauScaler'

# preselection 
cmgTauMuPreSel = cmgTauMuSel.clone( cut = 'getSelection("cuts_baseline")')

# full selection
cmgTauMuFullSel = cmgTauMuSel.clone( src = 'cmgTauMuPreSel',
                                     cut = 'getSelection("cuts_baseline")' )

tauMuStdSequence = cms.Sequence( cmgTauScaler +
                                 cmgTauMu +
                                 cmgTauMuPreSel +
                                 cmgTauMuFullSel )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# recoil correction

recoilCorMETTauMu =  recoilCorrectedMETTauMu.clone( recBosonSrc = 'cmgTauMuPreSel')

cmgTauMuCorPreSel = cmgTauMuCor.clone()
cmgTauMuCorPreSel.cfg.metCollection = 'recoilCorMETTauMu'
cmgTauMuCorPreSel.cfg.diObjectCollection = 'cmgTauMuPreSel'

# SVFit

cmgTauMuCorSVFitPreSel = tauMuSVFit.clone()
cmgTauMuCorSVFitPreSel.diTauSrc = 'cmgTauMuCorPreSel'

cmgTauMuCorSVFitFullSel = cmgTauMuFullSel.clone( src = 'cmgTauMuCorSVFitPreSel' ) 


tauMuCorSVFitSequence = cms.Sequence( recoilCorMETTauMu +
                                      cmgTauMuCorPreSel +
                                      cmgTauMuCorSVFitPreSel +
                                      cmgTauMuCorSVFitFullSel )

tauMuSequence = cms.Sequence( tauMuStdSequence +
                              tauMuCorSVFitSequence
                              )


# the following sequence is generic
metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               tauMuSequence )

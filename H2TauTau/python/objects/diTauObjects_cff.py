import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.diTauCuts_cff import * 

from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgDiTauCor_cfi import cmgDiTauCor 
from CMGTools.H2TauTau.objects.diTauSVFit_cfi import diTauSVFit 



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgDiTau.cuts = diTauCuts.clone()
cmgDiTau.cfg.leg1Collection = 'cmgTauScaler'
cmgDiTau.cfg.leg2Collection = 'cmgTauScaler'

# preselection 
cmgDiTauPreSel = cmgDiTauSel.clone( cut = 'getSelection("cuts_skimming")')

# full selection
cmgDiTauFullSel = cmgDiTauSel.clone( src = 'cmgDiTauPreSel',
                                     cut = 'getSelection("cuts_baseline")' )

diTauStdSequence = cms.Sequence( cmgTauScaler +
                                 cmgDiTau +
                                 cmgDiTauPreSel +
                                 cmgDiTauFullSel )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# recoil correction

# disable recol correction sind never studied
#recoilCorMETDiTau =  recoilCorrectedMETDiTau.clone( recBosonSrc = 'cmgDiTauPreSel')

#cmgDiTauCorPreSel = cmgDiTauCor.clone()
#cmgDiTauCorPreSel.cfg.metCollection = 'recoilCorMETDiTau'
#cmgDiTauCorPreSel.cfg.diObjectCollection = 'cmgDiTauPreSel'

# SVFit

cmgDiTauCorSVFitPreSel = diTauSVFit.clone()
cmgDiTauCorSVFitPreSel.diTauSrc = 'cmgDiTauPreSel' #'cmgDiTauCorPreSel'

cmgDiTauCorSVFitFullSel = cmgDiTauFullSel.clone( src = 'cmgDiTauCorSVFitPreSel' ) 


diTauCorSVFitSequence = cms.Sequence( #recoilCorMETDiTau +
                                      #cmgDiTauCorPreSel +
                                      cmgDiTauCorSVFitPreSel +
                                      cmgDiTauCorSVFitFullSel )

diTauSequence = cms.Sequence( diTauStdSequence +
                              diTauCorSVFitSequence
                              )


# the following sequence is generic
metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               diTauSequence )

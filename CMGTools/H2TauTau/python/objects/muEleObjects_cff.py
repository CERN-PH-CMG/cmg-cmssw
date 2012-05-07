import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.muEleCuts_cff import * 

from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
# from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgMuEleCor_cfi import cmgMuEleCor 
from CMGTools.H2TauTau.objects.muEleSVFit_cfi import muEleSVFit 


# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgMuEle.cuts = muEleCuts.clone()
# cmgMuEle.cfg.leg1Collection = 'cmgTauScaler'

# preselection 
cmgMuElePreSel = cmgMuEleSel.clone( cut = 'getSelection("cuts_baseline")')

# full selection
cmgMuEleFullSel = cmgMuEleSel.clone( src = 'cmgMuElePreSel',
                                     cut = 'getSelection("cuts_baseline")' )

muEleStdSequence = cms.Sequence(
    # cmgTauScaler +
    cmgMuEle +
    cmgMuElePreSel +
    cmgMuEleFullSel
    )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# recoil correction

recoilCorMETMuEle =  recoilCorrectedMETMuEle.clone( recBosonSrc = 'cmgMuElePreSel')

cmgMuEleCorPreSel = cmgMuEleCor.clone()
cmgMuEleCorPreSel.cfg.metCollection = 'recoilCorMETMuEle'
cmgMuEleCorPreSel.cfg.diObjectCollection = 'cmgMuElePreSel'

# SVFit

cmgMuEleCorSVFitPreSel = muEleSVFit.clone()
cmgMuEleCorSVFitPreSel.diTauSrc = 'cmgMuEleCorPreSel'

cmgMuEleCorSVFitFullSel = cmgMuEleFullSel.clone( src = 'cmgMuEleCorSVFitPreSel' ) 


muEleCorSVFitSequence = cms.Sequence( recoilCorMETMuEle +
                                      cmgMuEleCorPreSel +
                                      cmgMuEleCorSVFitPreSel +
                                      cmgMuEleCorSVFitFullSel )

muEleSequence = cms.Sequence( muEleStdSequence +
                              muEleCorSVFitSequence
                              )


# the following sequence is generic
metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               muEleSequence )

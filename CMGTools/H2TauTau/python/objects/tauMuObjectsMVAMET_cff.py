import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauMuCuts_cff import * 

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

from CMGTools.Utilities.metRecoilCorrection.metRecoilCorrection_cff import *

from CMGTools.H2TauTau.objects.cmgTauMuCor_cfi import cmgTauMuCor 
from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit 
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

cmgTauMu.leg1Collection = cms.InputTag('slimmedTaus')
cmgTauMu.leg2Collection = cms.InputTag('slimmedMuons')
cmgTauMu.metsigCollection = cms.InputTag('')
cmgTauMu.metCollection = cms.InputTag('slimmedMETs')


# creates a tau-mu pair
tauMuStdSequence = cms.Sequence(
    cmgTauMu 
    )


# correction and svfit ------------------------------------------------------

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from CMGTools.Utilities.mvaMET.mvaMET_cff import *

cmgTauMuMVAPreSel = cmgTauMuCor.clone()
cmgTauMuMVAPreSel.diObjectCollection = 'cmgTauMu'

# Correct tau pt (after MVA MET according to current baseline)

from CMGTools.H2TauTau.objects.cmgTauMuCor_cfi import cmgTauMuCor
cmgTauMuCor = cmgTauMuCor.clone()

cmgTauMuCor.diObjectCollection = cms.InputTag('mvaMETTauMu')

# JAN: It's debatable whether this should be applied after MVA MET
# and before the recoil correction instead of at the very beginning


# This selector goes after the tau pt correction
cmgTauMuTauPtSel = cms.EDFilter(
    "PATCompositeCandidateSelector",
    src = cms.InputTag( "cmgTauMuCor" ),
    cut = cms.string( "daughter(0).pt()>18." )
    )

cmgTauMuTauPtSel = cmgTauMuTauPtSel.clone()


# recoil correction

diTausForRecoil = 'cmgTauMuTauPtSel'
recoilCorMETTauMu =  recoilCorrectedMETTauMu.clone(
    recBosonSrc = diTausForRecoil
    )

tauMuMvaMETrecoilSequence = cms.Sequence( goodPVFilter + 
                               mvaMETTauMu +
                               cmgTauMuCor +
                               cmgTauMuTauPtSel +
                               recoilCorMETTauMu
                               )

# SVFit

cmgTauMuCorSVFitPreSel = tauMuSVFit.clone()
cmgTauMuCorSVFitPreSel.diTauSrc = cms.InputTag('recoilCorMETTauMu')

# This module is not really necessary anymore
cmgTauMuCorSVFitFullSel = cmgTauMuSel.clone( src = 'cmgTauMuCorSVFitPreSel',
                                             cut = ''
                                             ) 

tauMuCorSVFitSequence = cms.Sequence( #
    tauMuMvaMETrecoilSequence +
    cmgTauMuCorSVFitPreSel +
    cmgTauMuCorSVFitFullSel
    )

tauMuSequence = cms.Sequence( tauMuStdSequence +
                              tauMuCorSVFitSequence
                              )



import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauMuCuts_cff import * 

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

from CMGTools.Utilities.metRecoilCorrection.metRecoilCorrection_cff import *

from CMGTools.H2TauTau.objects.cmgTauMuCor_cfi import cmgTauMuCor 
from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit 
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory

cmgTauMu.cuts = tauMuCuts.clone()
cmgTauMu.cfg.leg1Collection = 'cmgTauSel'
cmgTauMu.cfg.metsigCollection = cms.InputTag('')

# preselection 
cmgTauMuPreSel = cmgTauMuSel.clone(
    # cut = ''
    #WARNING
    cut = 'getSelection("cuts_baseline")'
    )

# creates a tau-mu pair and applies loose preselection cuts
tauMuStdSequence = cms.Sequence(
    cmgTauMu +
    cmgTauMuPreSel
    )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from CMGTools.Utilities.mvaMET.mvaMET_cff import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET
mvaMETTauMu.recBosonSrc = 'cmgTauMuPreSel'

cmgTauMuMVAPreSel = cmgTauMuCor.clone()
cmgTauMuMVAPreSel.cfg.diObjectCollection = 'cmgTauMuPreSel'


# Correct tau pt (after MVA MET according to current baseline)

from CMGTools.H2TauTau.objects.cmgTauMuCor_cfi import cmgTauMuCor
cmgTauMuCor = cmgTauMuCor.clone()

cmgTauMuCor.cfg.diObjectCollection = cms.InputTag('mvaMETTauMu')

# JAN: It's debatable whether this should be applied after MVA MET
# and before the recoil correction instead of at the very beginning


# This selector goes after the tau pt correction
cmgTauMuTauPtSel = cms.EDFilter(
    "CmgTauMuSelector",
    src = cms.InputTag( "cmgTauMuCor" ),
    cut = cms.string( "leg1().pt()>18." )
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
# cmgTauMuCorSVFitPreSel.diTauSrc = 'cmgTauMuCorPreSel'
cmgTauMuCorSVFitPreSel.diTauSrc = cms.InputTag('recoilCorMETTauMu')
# cmgTauMuCorSVFitPreSel.metsigSrc = 'mvaMETTauMu'

# This module is not really necessary anymore
cmgTauMuCorSVFitFullSel = cmgTauMuSel.clone( src = 'cmgTauMuCorSVFitPreSel',
                                             cut = ''
                                             # WARNING!
                                             # cut = 'getSelection("cuts_baseline")'
                                             ) 

tauMuCorSVFitSequence = cms.Sequence( #
    tauMuMvaMETrecoilSequence +
    #cmgTauMuCorPreSel +
    cmgTauMuCorSVFitPreSel +
    cmgTauMuCorSVFitFullSel
    )

tauMuSequence = cms.Sequence( tauMuStdSequence +
                              tauMuCorSVFitSequence
                              )


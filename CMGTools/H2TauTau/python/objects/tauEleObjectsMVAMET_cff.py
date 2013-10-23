import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauEleCuts_cff import * 

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

from CMGTools.Utilities.metRecoilCorrection.metRecoilCorrection_cff import *

from CMGTools.H2TauTau.objects.cmgTauEleCor_cfi import cmgTauEleCor 
from CMGTools.H2TauTau.objects.tauEleSVFit_cfi import tauEleSVFit 
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgTauEle.cuts = tauEleCuts.clone()
cmgTauEle.cfg.leg1Collection = 'cmgTauSel'
cmgTauEle.cfg.metsigCollection = cms.InputTag('')

# preselection 
cmgTauElePreSel = cmgTauEleSel.clone(
    # cut = ''
    #WARNING
    cut = 'getSelection("cuts_baseline")'
    )

tauEleStdSequence = cms.Sequence( 
    cmgTauEle +
    cmgTauElePreSel
    )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from CMGTools.Utilities.mvaMET.mvaMET_cff import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET
mvaMETTauEle.recBosonSrc = 'cmgTauElePreSel'

cmgTauEleMVAPreSel = cmgTauEleCor.clone()
cmgTauEleMVAPreSel.cfg.diObjectCollection = 'cmgTauElePreSel'


# Correct tau pt (after MVA MET according to current baseline)

from CMGTools.H2TauTau.objects.cmgTauEleCor_cfi import cmgTauEleCor
cmgTauEleCor = cmgTauEleCor.clone()

cmgTauEleCor.cfg.diObjectCollection = cms.InputTag('mvaMETTauEle')

# This selector goes after the tau pt correction.
# Can in principal apply any further selection on the
# di-tau object.
cmgTauEleTauPtSel = cms.EDFilter(
    "CmgTauEleSelector",
    src = cms.InputTag( "cmgTauEleCor" ),
    cut = cms.string( "leg1().pt()>18." )
    )

cmgTauEleTauPtSel = cmgTauEleTauPtSel.clone()

# recoil correction

#IN 52X: should be type1 MET. In 44X, should be raw MET
diTausForRecoil = 'cmgTauEleTauPtSel'
recoilCorMETTauEle =  recoilCorrectedMETTauEle.clone(
    recBosonSrc = diTausForRecoil
    )

# mvaBaseMETTauEle = cmgBaseMETFromPFMET.clone()
# mvaBaseMETTauEle.cfg.inputCollection = 'recoilCorMETTauEle'

tauEleMvaMETRecoilSequence = cms.Sequence( goodPVFilter + 
                               mvaMETTauEle +
                               cmgTauEleCor +
                               cmgTauEleTauPtSel +
                               recoilCorMETTauEle 
                               )

# SVFit

cmgTauEleCorSVFitPreSel = tauEleSVFit.clone()
cmgTauEleCorSVFitPreSel.diTauSrc = cms.InputTag('recoilCorMETTauEle')
# cmgTauEleCorSVFitPreSel.metsigSrc = 'mvaMETTauEle'
cmgTauEleCorSVFitFullSel = cmgTauEleSel.clone( src = 'cmgTauEleCorSVFitPreSel',
                                             cut = ''
                                             # WARNING!
                                             # cut = 'getSelection("cuts_baseline")'
                                             ) 

tauEleCorSVFitSequence = cms.Sequence( #
    tauEleMvaMETRecoilSequence +
    # cmgTauEleCorPreSel +
    cmgTauEleCorSVFitPreSel +
    cmgTauEleCorSVFitFullSel
    )

tauEleSequence = cms.Sequence( tauEleStdSequence +
                               tauEleCorSVFitSequence
                               )


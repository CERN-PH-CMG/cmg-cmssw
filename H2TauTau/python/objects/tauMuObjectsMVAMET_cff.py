import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauMuCuts_cff import * 

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

from CMGTools.Utilities.metRecoilCorrection.metRecoilCorrection_cff import *

from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgTauMuCor_cfi import cmgTauMuCor 
from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit 
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgTauMu.cuts = tauMuCuts.clone()
cmgTauMu.cfg.leg1Collection = 'cmgTauScaler'

# preselection 
cmgTauMuPreSel = cmgTauMuSel.clone(
    # cut = ''
    #WARNING
    cut = 'getSelection("cuts_baseline")'
    )

tauMuStdSequence = cms.Sequence( cmgTauScaler +
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
cmgTauMuMVAPreSel.cfg.metCollection = 'mvaBaseMETTauMu'
cmgTauMuMVAPreSel.cfg.diObjectCollection = 'cmgTauMuPreSel'

# recoil correction

metForRecoil = 'mvaMETTauMu'
diTausForRecoil = 'cmgTauMuPreSel'
recoilCorMETTauMu =  recoilCorrectedMETTauMu.clone(
    recBosonSrc = diTausForRecoil,
    metSrc = metForRecoil
    )

mvaBaseMETTauMu = cmgBaseMETFromPFMET.clone()
mvaBaseMETTauMu.cfg.inputCollection = 'recoilCorMETTauMu'

cmgTauMuCorPreSel = cmgTauMuCor.clone()
cmgTauMuCorPreSel.cfg.metCollection = 'mvaBaseMETTauMu'
cmgTauMuCorPreSel.cfg.diObjectCollection = 'cmgTauMuPreSel'

mvaMETSequence = cms.Sequence( goodPVFilter + 
                               mvaMETTauMu +
                               recoilCorMETTauMu +
                               mvaBaseMETTauMu
                               #    # +
                               #    # cmgTauMuMVAPreSel
                               )

# SVFit

cmgTauMuCorSVFitPreSel = tauMuSVFit.clone()
cmgTauMuCorSVFitPreSel.diTauSrc = 'cmgTauMuCorPreSel'
cmgTauMuCorSVFitPreSel.metsigSrc = 'mvaMETTauMu'

# This module is not really necessary anymore
cmgTauMuCorSVFitFullSel = cmgTauMuSel.clone( src = 'cmgTauMuCorSVFitPreSel',
                                             cut = ''
                                             # WARNING!
                                             # cut = 'getSelection("cuts_baseline")'
                                             ) 

tauMuCorSVFitSequence = cms.Sequence( #
    mvaMETSequence +
    cmgTauMuCorPreSel +
    cmgTauMuCorSVFitPreSel +
    cmgTauMuCorSVFitFullSel
    )

tauMuSequence = cms.Sequence( tauMuStdSequence +
                              tauMuCorSVFitSequence
                              )


import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.diTauCuts_cff import * 

from CMGTools.H2TauTau.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgDiTauCor_cfi import cmgDiTauCor 
from CMGTools.H2TauTau.objects.diTauSVFit_cfi import diTauSVFit 
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgDiTau.cuts = diTauCuts.clone()
cmgDiTau.cfg.leg1Collection = 'cmgTauScaler'
cmgDiTau.cfg.leg2Collection = 'cmgTauScaler'

# preselection 
cmgDiTauPreSel = cmgDiTauSel.clone( cut = 'getSelection("cuts_baseline") && leg1().pt>35 && leg2().pt>35')

diTauStdSequence = cms.Sequence( cmgTauScaler +
                                 cmgDiTau +
                                 cmgDiTauPreSel
                                 )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from CMGTools.Common.miscProducers.mvaMET.mvaMET_cff import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET
mvaMETDiTau.recBosonSrc = 'cmgDiTauPreSel'
mvaBaseMETDiTau = cmgBaseMETFromPFMET.clone()
mvaBaseMETDiTau.cfg.inputCollection = 'mvaMETDiTau'

cmgDiTauMVAPreSel = cmgDiTauCor.clone()
cmgDiTauMVAPreSel.cfg.metCollection = 'mvaBaseMETDiTau'
cmgDiTauMVAPreSel.cfg.diObjectCollection = 'cmgDiTauPreSel'

mvaMETSequence = cms.Sequence( goodPVFilter + 
                               mvaMETDiTau +
                               mvaBaseMETDiTau
                               #    # +
                               #    # cmgDiTauMVAPreSel
                               )

# recoil correction

# IN 52X: should be type1 MET. In 44X, should be raw MET
# metForRecoil = 'cmgPFMET'
# if cmsswIs44X():
#     metForRecoil = 'cmgPFMETRaw'
# diTausForRecoil = 'cmgDiTauPreSel'
# recoilCorMETDiTau =  recoilCorrectedMETDiTau.clone(
#     recBosonSrc = diTausForRecoil,
#     metSrc = metForRecoil
#     )

cmgDiTauCorPreSel = cmgDiTauCor.clone()
cmgDiTauCorPreSel.cfg.metCollection = 'mvaBaseMETDiTau'
cmgDiTauCorPreSel.cfg.diObjectCollection = 'cmgDiTauPreSel'

# SVFit

cmgDiTauCorSVFitPreSel = diTauSVFit.clone()
cmgDiTauCorSVFitPreSel.diTauSrc = 'cmgDiTauCorPreSel'
cmgDiTauCorSVFitPreSel.metsigSrc = 'mvaMETDiTau'
cmgDiTauCorSVFitFullSel = cmgDiTauSel.clone( src = 'cmgDiTauCorSVFitPreSel',
                                             cut = ''
                                             # WARNING!
                                             # cut = 'getSelection("cuts_baseline")'
                                             ) 

diTauCorSVFitSequence = cms.Sequence( #
    mvaMETSequence +
    cmgDiTauCorPreSel +
    cmgDiTauCorSVFitPreSel +
    cmgDiTauCorSVFitFullSel
    )

diTauSequence = cms.Sequence( diTauStdSequence +
                              diTauCorSVFitSequence
                              )


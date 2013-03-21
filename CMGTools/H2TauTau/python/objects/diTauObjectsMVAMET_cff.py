import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.diTauCuts_cff import * 

from CMGTools.Utilities.metRecoilCorrection.metRecoilCorrection_cff import *
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
cmgDiTauPreSel = cmgDiTauSel.clone( cut = 'getSelection("cuts_baseline") && leg1().pt()>40 && leg2().pt()>40')

diTauStdSequence = cms.Sequence( cmgTauScaler +
                                 cmgDiTau +
                                 cmgDiTauPreSel
                                 )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from CMGTools.Utilities.mvaMET.mvaMET_cff import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET
mvaMETDiTau.recBosonSrc = 'cmgDiTauPreSel'

cmgDiTauMVAPreSel = cmgDiTauCor.clone()
cmgDiTauMVAPreSel.cfg.metCollection = 'mvaBaseMETDiTau'
cmgDiTauMVAPreSel.cfg.diObjectCollection = 'cmgDiTauPreSel'

# recoil correction

metForRecoil = 'mvaMETDiTau'
diTausForRecoil = 'cmgDiTauPreSel'
recoilCorMETDiTau =  recoilCorrectedMETDiTau.clone(
    recBosonSrc = diTausForRecoil,
    metSrc = metForRecoil
    )

mvaBaseMETDiTau = cmgBaseMETFromPFMET.clone()

# SWITCH OFF RECOIL CORRECTIONS HERE

mvaBaseMETDiTau.cfg.inputCollection = 'recoilCorMETDiTau'
#mvaBaseMETDiTau.cfg.inputCollection = 'mvaMETDiTau'

# sequence

mvaMETSequence = cms.Sequence( goodPVFilter + 
                               mvaMETDiTau +
                               recoilCorMETDiTau +
                               mvaBaseMETDiTau
                               #    # +
                               #    # cmgDiTauMVAPreSel
                               )


cmgDiTauCorPreSel = cmgDiTauCor.clone()
cmgDiTauCorPreSel.cfg.metCollection = 'mvaBaseMETDiTau'
cmgDiTauCorPreSel.cfg.diObjectCollection = 'cmgDiTauPreSel'

# SVFit

cmgDiTauCorSVFitPreSel = diTauSVFit.clone()
cmgDiTauCorSVFitPreSel.diTauSrc = 'cmgDiTauCorPreSel'
cmgDiTauCorSVFitPreSel.metsigSrc = 'mvaMETDiTau'
cmgDiTauCorSVFitFullSel = cmgDiTauSel.clone( src = 'cmgDiTauCorSVFitPreSel',
                                             cut = 'leg1().pt()>45 && leg2().pt()>45',
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


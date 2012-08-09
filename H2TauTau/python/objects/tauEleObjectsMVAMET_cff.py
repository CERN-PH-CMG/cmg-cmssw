import FWCore.ParameterSet.Config as cms

from CMGTools.Common.diTau_cff import *
from CMGTools.H2TauTau.objects.tauEleCuts_cff import * 

from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.factories.cmgTauScaler_cfi import  cmgTauScaler
from CMGTools.Common.factories.cmgTauEleCor_cfi import cmgTauEleCor 
from CMGTools.H2TauTau.objects.tauEleSVFit_cfi import tauEleSVFit 
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X



# no correction, no svfit ---------------------------------------------------

# attaching the cuts defined in this module
# to the di-tau factory
cmgTauEle.cuts = tauEleCuts.clone()
cmgTauEle.cfg.leg1Collection = 'cmgTauScaler'

# preselection 
cmgTauElePreSel = cmgTauEleSel.clone(
    cut = ''
    #WARNING
    # cut = 'getSelection("cuts_baseline")'
    )

tauEleStdSequence = cms.Sequence( cmgTauScaler +
                                  cmgTauEle +
                                  cmgTauElePreSel
                                 )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from CMGTools.Common.miscProducers.mvaMET.mvaMET_cff import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET
mvaMETTauEle.recBosonSrc = 'cmgTauElePreSel'
mvaBaseMETTauEle = cmgBaseMETFromPFMET.clone()
mvaBaseMETTauEle.cfg.inputCollection = 'mvaMETTauEle'

cmgTauEleMVAPreSel = cmgTauEleCor.clone()
cmgTauEleMVAPreSel.cfg.metCollection = 'mvaBaseMETTauEle'
cmgTauEleMVAPreSel.cfg.diObjectCollection = 'cmgTauElePreSel'

mvaMETSequence = cms.Sequence( goodPVFilter + 
                               mvaMETTauEle +
                               mvaBaseMETTauEle
                               #    # +
                               #    # cmgTauEleMVAPreSel
                               )

# recoil correction

# IN 52X: should be type1 MET. In 44X, should be raw MET
# metForRecoil = 'cmgPFMET'
# if cmsswIs44X():
#     metForRecoil = 'cmgPFMETRaw'
# diTausForRecoil = 'cmgTauElePreSel'
# recoilCorMETTauEle =  recoilCorrectedMETTauEle.clone(
#     recBosonSrc = diTausForRecoil,
#     metSrc = metForRecoil
#     )

cmgTauEleCorPreSel = cmgTauEleCor.clone()
cmgTauEleCorPreSel.cfg.metCollection = 'mvaBaseMETTauEle'
cmgTauEleCorPreSel.cfg.diObjectCollection = 'cmgTauElePreSel'

# SVFit

cmgTauEleCorSVFitPreSel = tauEleSVFit.clone()
cmgTauEleCorSVFitPreSel.diTauSrc = 'cmgTauEleCorPreSel'
cmgTauEleCorSVFitPreSel.metsigSrc = 'mvaMETTauEle'
cmgTauEleCorSVFitFullSel = cmgTauEleSel.clone( src = 'cmgTauEleCorSVFitPreSel',
                                             cut = ''
                                             # WARNING!
                                             # cut = 'getSelection("cuts_baseline")'
                                             ) 

tauEleCorSVFitSequence = cms.Sequence( #
    mvaMETSequence +
    cmgTauEleCorPreSel +
    cmgTauEleCorSVFitPreSel +
    cmgTauEleCorSVFitFullSel
    )

tauEleSequence = cms.Sequence( tauEleStdSequence +
                               tauEleCorSVFitSequence
                               )


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
cmgDiTauPreSel = cmgDiTauSel.clone( cut = 'getSelection("cuts_baseline") && leg1().pt>35 && leg2().pt>35')

# full selection
cmgDiTauFullSel = cmgDiTauSel.clone( src = 'cmgDiTauPreSel',
                                     cut = 'getSelection("cuts_baseline")' )

diTauStdSequence = cms.Sequence( cmgTauScaler +
                                 cmgDiTau +
                                 cmgDiTauPreSel +
                                 cmgDiTauFullSel )


# correction and svfit ------------------------------------------------------

# this is done for preselected di-taus

# mva MET

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter
from CMGTools.Common.miscProducers.mvaMET.mvaMET_cff import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET

mvaMETDiTau = cms.EDProducer(
    "MVAMETProducerDiTau",
    pfmetSrc = cms.InputTag('pfMetForRegression'),
    tkmetSrc = cms.InputTag('tkMet'),
    nopumetSrc = cms.InputTag('nopuMet'),
    pucmetSrc = cms.InputTag('pcMet'),
    pumetSrc = cms.InputTag('puMet'),
    recBosonSrc = cms.InputTag('cmgDiTauPreSel'),
    jetSrc = cms.InputTag('cmgPFJetSel2'),
    leadJetSrc = cms.InputTag('cmgPFBaseJetLead'),
    vertexSrc = cms.InputTag('goodPVFilter'),
    nJetsPtGt1Src = cms.InputTag('nJetsPtGt1'),
    rhoSrc = cms.InputTag('kt6PFJets','rho'),
    enable = cms.bool(True),
    verbose = cms.untracked.bool( False )
    #COLIN: make delta R a parameter
    )


mvaMETDiTau.recBosonSrc = 'cmgDiTauPreSel'

mvaBaseMETDiTau = cmgBaseMETFromPFMET.clone()
mvaBaseMETDiTau.cfg.inputCollection = 'mvaMETDiTau'

cmgDiTauMVAPreSel = cmgDiTauCor.clone()
cmgDiTauMVAPreSel.cfg.metCollection = 'mvaBaseMETDiTau'
cmgDiTauMVAPreSel.cfg.diObjectCollection = 'cmgDiTauPreSel'

mvaMETSequence = cms.Sequence(
    goodPVFilter + 
    mvaMETDiTau +
    mvaBaseMETDiTau+
    cmgDiTauMVAPreSel
    )

# recoil correction
#doMVAMet = False
#metForRecoil = 'cmgPFMETRaw'
#diTausForRecoil = 'cmgDiTauPreSel'
#if doMVAMet:
#    # in this case the MET will be taken from the di-tau
#    metForRecoil = 'mvaBaseMETDiTau'
#    diTausForRecoil = 'cmgDiTauMVAPreSel'

#recoilCorMETDiTau =  recoilCorrectedMETDiTau.clone(
#    recBosonSrc = diTausForRecoil,
#    metSrc = metForRecoil
#    )

#cmgDiTauCorPreSel = cmgDiTauCor.clone()
#cmgDiTauCorPreSel.cfg.metCollection = 'recoilCorMETDiTau'
#cmgDiTauCorPreSel.cfg.diObjectCollection = diTausForRecoil

# SVFit

cmgDiTauCorSVFitPreSel = diTauSVFit.clone()
cmgDiTauCorSVFitPreSel.diTauSrc = 'cmgDiTauMVAPreSel'#'cmgDiTauPreSel' #cmgDiTauCorPreSel

cmgDiTauCorSVFitFullSel = cmgDiTauFullSel.clone( src = 'cmgDiTauCorSVFitPreSel' ) 


diTauCorSVFitSequence = cms.Sequence( mvaMETSequence +
                                      #recoilCorMETDiTau +
                                      #cmgDiTauCorPreSel +
                                      cmgDiTauCorSVFitPreSel +
                                      cmgDiTauCorSVFitFullSel )

#if doMVAMet:
#    diTauCorSVFitSequence.insert(0, mvaMETSequence)

diTauSequence = cms.Sequence( diTauStdSequence +
                              diTauCorSVFitSequence
                              )



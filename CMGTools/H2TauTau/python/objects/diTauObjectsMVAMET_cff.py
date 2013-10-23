import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease                             import cmsswIs44X,cmsswIs52X
from CMGTools.Common.diTau_cff                                      import *
from CMGTools.H2TauTau.objects.diTauCuts_cff                        import * 
from CMGTools.H2TauTau.objects.cmgDiTauCor_cfi                      import cmgDiTauCor 
from CMGTools.H2TauTau.objects.diTauSVFit_cfi                       import diTauSVFit 
from CMGTools.Common.eventCleaning.goodPVFilter_cfi                 import goodPVFilter
from CMGTools.Utilities.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Utilities.mvaMET.mvaMET_cff                           import *
from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi              import cmgBaseMETFromPFMET

# build diTau ----------------------------------------------------------
cmgDiTau.cuts = diTauCuts.clone()
cmgDiTau.cfg.metsigCollection = cms.InputTag('')

# pT preselection ------------------------------------------------------
cmgDiTauPreSel = cmgDiTauSel.clone( 
 src = 'cmgDiTau',
 cut = 'leg1().pt()>40. && leg2().pt()>40. && leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10. &&  leg2().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10.',
 )
 
# sequence -------------------------------------------------------------
diTauStdSequence = cms.Sequence( cmgDiTau       +
                                 cmgDiTauPreSel
                                 )

# mva MET --------------------------------------------------------------
mvaMETDiTau.recBosonSrc = 'cmgDiTauPreSel'


# correct TauES (after MVA MET according to current baseline) ----------
cmgDiTauCor = cmgDiTauCor.clone()
cmgDiTauCor.cfg.diObjectCollection = cms.InputTag('mvaMETDiTau')

# this selector goes after the TauES correction ------------------------
cmgDiTauPtSel = cmgDiTauSel.clone(
    src = cms.InputTag( "cmgDiTauCor" ),
    cut = cms.string( "leg1().pt()>45. && leg2().pt()>45." )
    )

# recoil correction ----------------------------------------------------
recoilCorMETDiTau =  recoilCorrectedMETDiTau.clone(
    recBosonSrc = 'cmgDiTauPtSel',
    )

# sequence -------------------------------------------------------------
mvaMETSequence = cms.Sequence( goodPVFilter      + 
                               mvaMETDiTau       +
                               cmgDiTauCor       +
                               cmgDiTauPtSel     +
                               recoilCorMETDiTau 
                               )

# SVFit ----------------------------------------------------------------
cmgDiTauCorSVFitPreSel = diTauSVFit.clone()
cmgDiTauCorSVFitPreSel.diTauSrc = cms.InputTag('recoilCorMETDiTau')
#cmgDiTauCorSVFitPreSel.verbose = True

cmgDiTauCorSVFitFullSel = cmgDiTauSel.clone( src = 'cmgDiTauCorSVFitPreSel',
                                             cut = '',
                                             ) 

diTauCorSVFitSequence = cms.Sequence(
    mvaMETSequence          +
    cmgDiTauCorSVFitPreSel  +
    cmgDiTauCorSVFitFullSel
    )

diTauSequence = cms.Sequence( diTauStdSequence +
                              diTauCorSVFitSequence
                              )


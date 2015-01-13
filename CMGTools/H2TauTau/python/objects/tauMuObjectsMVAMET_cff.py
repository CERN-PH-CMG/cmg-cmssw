import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.cmgTauMu_cfi import cmgTauMu
from CMGTools.H2TauTau.skims.cmgTauMuSel_cfi import cmgTauMuSel

from CMGTools.H2TauTau.objects.cmgTauMuCor_cfi import cmgTauMuCor 
from CMGTools.H2TauTau.objects.tauMuSVFit_cfi import tauMuSVFit 

from CMGTools.H2TauTau.objects.tauCuts_cff import tauPreSelection
from CMGTools.H2TauTau.objects.muCuts_cff import muonPreSelection

# tau pre-selection
tauPreSelection = tauPreSelection.clone()
muonPreSelection = muonPreSelection.clone()



# correction and svfit ------------------------------------------------------

# mva MET

# from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter

from RecoMET.METPUSubtraction.mvaPFMET_cff import puJetIdForPFMVAMEt, pfMVAMEt, calibratedAK4PFJetsForPFMVAMEt

pfMVAMEt.srcPFCandidates = cms.InputTag("packedPFCandidates")
pfMVAMEt.srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices")
pfMVAMEt.srcLeptons = cms.VInputTag(
  cms.InputTag("tauPreSelection", "", ""),
  cms.InputTag("muonPreSelection", "", ""),
  )
pfMVAMEt.permuteLeptons = cms.bool(True)

puJetIdForPFMVAMEt.jec =  cms.string('AK4PF')
#process.puJetIdForPFMVAMEt.jets = cms.InputTag("ak4PFJets")
puJetIdForPFMVAMEt.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
puJetIdForPFMVAMEt.rho = cms.InputTag("fixedGridRhoFastjetAll")


# Correct tau pt (after MVA MET according to current baseline)

cmgTauMuCor = cmgTauMuCor.clone()

# This selector goes after the tau pt correction
cmgTauMuTauPtSel = cms.EDFilter(
    "PATCompositeCandidateSelector",
    src = cms.InputTag( "cmgTauMuCor" ),
    cut = cms.string( "daughter(0).pt()>18." )
    )

cmgTauMuTauPtSel = cmgTauMuTauPtSel.clone()


# recoil correction

# diTausForRecoil = 'cmgTauMuTauPtSel'
# recoilCorMETTauMu =  recoilCorrectedMETTauMu.clone(
#     recBosonSrc = diTausForRecoil
#     )

# tauMuMvaMETrecoilSequence = cms.Sequence( goodPVFilter + 
#                                mvaMETTauMu +
#                                cmgTauMuCor +
#                                cmgTauMuTauPtSel +
#                                recoilCorMETTauMu
#                                )

tauMuMVAMetSequence = cms.Sequence(
    calibratedAK4PFJetsForPFMVAMEt*
    puJetIdForPFMVAMEt*
    pfMVAMEt
  )

# SVFit

cmgTauMuCorSVFitPreSel = tauMuSVFit.clone()
# cmgTauMuCorSVFitPreSel.diTauSrc = cms.InputTag('recoilCorMETTauMu')

# If you want to apply some extra selection after SVFit, do it here
cmgTauMuCorSVFitFullSel = cmgTauMuSel.clone( src = 'cmgTauMuCorSVFitPreSel',
                                             cut = ''
                                             ) 

tauMuSequence = cms.Sequence(   
    tauPreSelection +   
    muonPreSelection +   
    tauMuMVAMetSequence +
    cmgTauMu +
    cmgTauMuCor+
    cmgTauMuTauPtSel +
    cmgTauMuCorSVFitPreSel +
    cmgTauMuCorSVFitFullSel
  )



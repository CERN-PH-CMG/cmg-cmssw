import FWCore.ParameterSet.Config as cms

from CMGTools.Utilities.mvaMET.weights_gbr import weights_gbrmet,weights_gbrmetphi, weights_gbrmetu1cov, weights_gbrmetu2cov
from CMGTools.Common.miscProducers.mvaMET.metRegression_cff import puJetIdAlgo

##http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/JetMETCorrections/METPUSubtraction/python/mvaPFMET_leptons_cfi.py?view=markup
## Single muon for Wjets


#isomuons = cms.EDFilter(
#    "MuonSelector",
#    src = cms.InputTag('muons'),
#    cut = cms.string(    "(isTrackerMuon) && abs(eta) < 2.5 && pt > 9.5"+#17. "+
#                         "&& isPFMuon"+
#                         "&& globalTrack.isNonnull"+
#                         "&& innerTrack.hitPattern.numberOfValidPixelHits > 0"+
#                         "&& innerTrack.normalizedChi2 < 10"+
#                         "&& numberOfMatches > 0"+
#                         "&& innerTrack.hitPattern.numberOfValidTrackerHits>5"+
#                         "&& globalTrack.hitPattern.numberOfValidHits>0"+
#                         "&& (pfIsolationR03.sumChargedHadronPt+pfIsolationR03.sumNeutralHadronEt+pfIsolationR03.sumPhotonEt)/pt < 0.3"+
#                         "&& abs(innerTrack().dxy)<2.0"
#                         ),
#    filter = cms.bool(False)
#    )
#
cmgMuonMVAMETPresel = cms.EDFilter(
    "CmgMuonSelector",
    src = cms.InputTag( "cmgMuonSel" ),
    cut = cms.string( "abs(eta) < 2.5 && pt > 9.5"
                      + "&& sourcePtr.isTrackerMuon>0.5"
                      + "&& sourcePtr.userFloat('isPFMuon')>0.5"
                      + "&& sourcePtr.globalTrack.isNonnull"
                      + "&& sourcePtr.innerTrack.hitPattern.numberOfValidPixelHits > 0"
                      + "&& sourcePtr.innerTrack.normalizedChi2 < 10"
                      + "&& sourcePtr.numberOfMatches > 0"
                      + "&& sourcePtr.innerTrack.hitPattern.numberOfValidTrackerHits>5"
                      + "&& sourcePtr.globalTrack.hitPattern.numberOfValidHits>0"
                      + "&& (sourcePtr.pfIsolationR03.sumChargedHadronPt+sourcePtr.pfIsolationR03.sumNeutralHadronEt+sourcePtr.pfIsolationR03.sumPhotonEt)/pt < 0.3"
                      + "&& abs(sourcePtr.innerTrack.dxy)<2.0"
                      )
    )

#isoelectrons = cms.EDFilter(
#    "GsfElectronSelector",
#            src = cms.InputTag('gsfElectrons'),
#            cut = cms.string(
#            "abs(eta) < 2.5 && pt > 9.5"                               +
#            "&& gsfTrack.trackerExpectedHitsInner.numberOfHits == 0"   +
##            "&& (pfIsolationVariables.chargedHadronIso+pfIsolationVariables.neutralHadronIso)/et     < 0.3"  +
#            "&& (isolationVariables03.tkSumPt)/et              < 0.2"  +
#            "&& ((abs(eta) < 1.4442  "                                 +
#            "&& abs(deltaEtaSuperClusterTrackAtVtx)            < 0.007"+
#            "&& abs(deltaPhiSuperClusterTrackAtVtx)            < 0.8"  +
#            "&& sigmaIetaIeta                                  < 0.01" +
#            "&& hcalOverEcal                                   < 0.15" +
#            "&& abs(1./superCluster.energy - 1./p)             < 0.05)"+
#            "|| (abs(eta)  > 1.566 "+
#            "&& abs(deltaEtaSuperClusterTrackAtVtx)            < 0.009"+
#            "&& abs(deltaPhiSuperClusterTrackAtVtx)            < 0.10" +
#            "&& sigmaIetaIeta                                  < 0.03" +
#            "&& hcalOverEcal                                   < 0.10" +
#            "&& abs(1./superCluster.energy - 1./p)             < 0.05))" 
#            ),
#        filter = cms.bool(False)
#        )
cmgElectronMVAMETPresel = cms.EDFilter(
    "CmgElectronSelector",
    src = cms.InputTag( "cmgElectronSel" ),
    cut = cms.string( "abs(eta) < 2.5 && pt > 9.5"
                      + "&& sourcePtr.gsfTrack.trackerExpectedHitsInner.numberOfHits == 0" 
                      + "&& (sourcePtr.isolationVariables03.tkSumPt)/sourcePtr.et              < 0.2"
                      
                      + "&& ((abs(eta) < 1.4442  "                                 
                      + "&& abs(sourcePtr.deltaEtaSuperClusterTrackAtVtx)            < 0.007"
                      + "&& abs(sourcePtr.deltaPhiSuperClusterTrackAtVtx)            < 0.8"  
                      + "&& sourcePtr.sigmaIetaIeta                                  < 0.01" 
                      + "&& sourcePtr.hcalOverEcal                                   < 0.15" 
                      + "&& abs(1./sourcePtr.superCluster.energy - 1./sourcePtr.p)             < 0.05)"
                      + "|| (abs(eta)  > 1.566 "
                      + "&& abs(sourcePtr.deltaEtaSuperClusterTrackAtVtx)            < 0.009"
                      + "&& abs(sourcePtr.deltaPhiSuperClusterTrackAtVtx)            < 0.10" 
                      + "&& sourcePtr.sigmaIetaIeta                                  < 0.03" 
                      + "&& sourcePtr.hcalOverEcal                                   < 0.10" 
                      + "&& abs(1./sourcePtr.superCluster.energy - 1./sourcePtr.p)             < 0.05))" 
                      )
    )

#isotaus = cms.EDFilter(
#    "PFTauSelector",
#    src = cms.InputTag('hpsPFTauProducer'),
#    BooleanOperator = cms.string("and"),
#    discriminators = cms.VPSet(
#    cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),       selectionCut=cms.double(0.5)),
#    cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByMVAIsolation"),           selectionCut=cms.double(0.5)),
#    cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection"), selectionCut=cms.double(0.5)),
#    cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationAgainstMuon2"),             selectionCut=cms.double(0.5)) 
#    ),
#    cut = cms.string("abs(eta) < 2.3 && pt > 19.0 "),
#    filter = cms.bool(False)
#    )
cmgTauMVAMETPresel = cms.EDFilter(
    "CmgTauSelector",
    src = cms.InputTag( "cmgTauSel" ),
    cut = cms.string( "abs(eta) < 2.3 && pt > 19.0"
                      + " && tauID('decayModeFinding')>0.5 "
                      + " && tauID('byRawIsoMVA')>0.0 "
                      + " && tauID('againstMuonLoose2')>0.5 " 
                      + " && tauID('againstElectronLoose')>0.5 "
                      )
    )


mvaMETPreselLep = cms.EDProducer(
    "MVAMETProducerPreselectedLeptons",
    pfmetSrc = cms.InputTag('pfMetForRegression'),
    tkmetSrc = cms.InputTag('tkMet'),
    nopumetSrc = cms.InputTag('nopuMet'),
    pucmetSrc = cms.InputTag('pcMet'),
    pumetSrc = cms.InputTag('puMet'),
    preselMuonListTag = cms.InputTag('cmgMuonMVAMETPresel'),
    preselElectronListTag = cms.InputTag('cmgElectronMVAMETPresel'),
    preselTauListTag = cms.InputTag('cmgTauMVAMETPresel'),
    jetSrc = cms.InputTag('cmgPFJetSel'),
    puJetIdLabel = puJetIdAlgo.label,
    leadJetSrc = cms.InputTag('cmgPFBaseJetLead'),
    vertexSrc = cms.InputTag('goodPVFilter'),
    nJetsPtGt1Src = cms.InputTag('nJetsPtGt1'),
    rhoSrc = cms.InputTag('kt6PFJets','rho'),
    enable = cms.bool(True),
    verbose = cms.untracked.bool( False ),
    weights_gbrmet = cms.string(weights_gbrmet),
    weights_gbrmetphi = cms.string(weights_gbrmetphi),
    weights_gbrmetu1cov = cms.string(weights_gbrmetu1cov),
    weights_gbrmetu2cov = cms.string(weights_gbrmetu2cov),
    )



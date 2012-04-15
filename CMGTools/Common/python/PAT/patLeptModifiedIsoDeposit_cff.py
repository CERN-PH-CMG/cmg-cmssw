import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositTk_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositEcalFromHits_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositHcalFromTowers_cff import *
eleIsoDepositTk.src = "gsfElectrons"
eleIsoDepositEcalFromHits.src = "gsfElectrons"
eleIsoDepositHcalFromTowers.src = "gsfElectrons"


MUON_VETO_CUT=("isGlobalMuon &&" +
               "pt>5"
              )
ELE_ISOVETO5_CUT=( "pt>5" ) 
ELE_ISOVETO7_CUT=( "pt>7" )  

vetoMuons =  cms.EDFilter("MuonRefSelector",
    src = cms.InputTag("muons"),
    cut = cms.string(MUON_VETO_CUT)
)

vetoElectrons5 =  cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string(ELE_ISOVETO5_CUT)
)

vetoElectrons7 =  cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string(ELE_ISOVETO7_CUT)
)

eleIsoFromDepsTkOptimized5 = cms.EDProducer("CandIsolatorFromDeposits",
   deposits = cms.VPSet(cms.PSet(
       mode = cms.string('sum'),
       src = cms.InputTag("eleIsoDepositTk"),
       weight = cms.string('1'),
       deltaR = cms.double(0.3),
       vetos = cms.vstring('vetoMuons:0.015', 
                           'vetoElectrons5:RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', 
                           'RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', 
                           'Threshold(0.7)'),
       skipDefaultVeto = cms.bool(True)
   ))
)

eleIsoFromDepsTkOptimized7 = cms.EDProducer("CandIsolatorFromDeposits",
   deposits = cms.VPSet(cms.PSet(
       mode = cms.string('sum'),
       src = cms.InputTag("eleIsoDepositTk"),
       weight = cms.string('1'),
       deltaR = cms.double(0.3),
       vetos = cms.vstring('vetoMuons:0.015', 
                           'vetoElectrons7:RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', 
                           'RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', 
                           'Threshold(0.7)'),
       skipDefaultVeto = cms.bool(True)
   ))
)

muIsoFromDepsTkOptimized = cms.EDProducer("CandIsolatorFromDeposits",
   deposits = cms.VPSet(cms.PSet(
       mode = cms.string('sum'),
       src = cms.InputTag("muons","muIsoDepositTk"),
       weight = cms.string('1'),
       deltaR = cms.double(0.3),
       vetos = cms.vstring('vetoMuons:0.015',
                           'vetoElectrons7:0.015',
                           'Threshold(1.0)'),
       skipDefaultVeto = cms.bool(True)
   ))
)

patElectronIsoDeposit = cms.Sequence( vetoMuons + vetoElectrons5 + vetoElectrons7 + eleIsoDepositTk + eleIsoFromDepsTkOptimized5 + eleIsoFromDepsTkOptimized7)# + eleIsoDepositEcalFromHits+ eleIsoDepositHcalFromTowers)
patMuonIsoDeposit = cms.Sequence(vetoMuons + vetoElectrons7 + muIsoFromDepsTkOptimized)


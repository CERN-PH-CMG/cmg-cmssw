import FWCore.ParameterSet.Config as cms

"""
Wrapper to test different cuts
"""
def getCuts(version) :
    #Chiara's skim
    #missing:
    simpleMuonId=("numberOfMatchedStations>1 && globalTrack.normalizedChi2<10 && globalTrack.hitPattern.numberOfValidMuonHits>0")
    simpleEleId =("electronID(\"eidLoose\")")

    if(version=='A1') :
        MUON_CUT=("isGlobalMuon &&" + simpleMuonId );
        ELECTRON_CUT=("ecalDrivenSeed &&" + simpleEleId)
        DIMUON_CUT=("")
        DIELECTRON_CUT=("")
        EMU_CUT=("")
    if(version=='A2') :
        MUON_CUT=("isGlobalMuon &&" + simpleMuonId);
        ELECTRON_CUT=("ecalDrivenSeed &&" + simpleEleId)
        DIMUON_CUT=("mass>20")
        DIELECTRON_CUT=("mass>20")
        EMU_CUT=("mass>20")
    if(version=='A3') :
        MUON_CUT=("isGlobalMuon &&" + simpleMuonId);
        ELECTRON_CUT=("ecalDrivenSeed &&" + simpleEleId)
        DIMUON_CUT=("mass>40")
        DIELECTRON_CUT=("mass>40")
        EMU_CUT=("mass>40")

    #Ivica's skim
    if(version=='B1') :
        MUON_CUT=("pt > 10 && abs(eta)<2.4 && isGlobalMuon")
        ELECTRON_CUT=("pt > 10 && abs(eta)<2.5 && ecalDrivenSeed")
        DIMUON_CUT=("mass > 40 && daughter(0).pt>15 && daughter(1).pt()>10")
        DIELECTRON_CUT=("mass > 40 && daughter(0).pt>15 && daughter(1).pt()>10")
        EMU_CUT=("mass > 40 && daughter(0).pt>15 && daughter(1).pt()>10")
    if(version=='B2') :
        MUON_CUT=("pt > 15 && abs(eta)<2.4 && isGlobalMuon")
        ELECTRON_CUT=("pt > 15 && abs(eta)<2.5 && ecalDrivenSeed")
        DIMUON_CUT=("mass > 20 && daughter(0).pt>20 && daughter(1).pt()>15")
        DIELECTRON_CUT=("mass > 20 && daughter(0).pt>20 && daughter(1).pt()>15")
        EMU_CUT=("mass > 20 && daughter(0).pt>20 && daughter(1).pt()>15")
    if(version=='B3') :
        MUON_CUT=("pt > 15 && abs(eta)<2.4 && isGlobalMuon")
        ELECTRON_CUT=("pt > 15 && abs(eta)<2.5 && ecalDrivenSeed && " + simpleEleId)
        DIMUON_CUT=("mass > 20 && daughter(0).pt>20 && daughter(1).pt()>15")
        DIELECTRON_CUT=("mass > 20 && daughter(0).pt>20 && daughter(1).pt()>15")
        EMU_CUT=("mass > 20 && daughter(0).pt>20 && daughter(1).pt()>15")

    return MUON_CUT, DIMUON_CUT, ELECTRON_CUT, DIELECTRON_CUT, EMU_CUT

# define cuts
MUON_CUT, DIMUON_CUT, ELECTRON_CUT, DIELECTRON_CUT, EMU_CUT = getCuts(version='A1')



#
# EVENT COUNTERS
#
startCounter = cms.EDProducer("EventCountProducer")
eeCounter = startCounter.clone()
mumuCounter = startCounter.clone()
emuCounter = startCounter.clone()

# LEPTONS
goodMuons = cms.EDFilter("MuonRefSelector",  src = cms.InputTag("muons"),  cut = cms.string(MUON_CUT) )
#goodElectrons = cms.EDFilter("GsfElectronRefSelector",  src = cms.InputTag("gsfElectrons"), cut = cms.string(ELECTRON_CUT) )
from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cff import *
from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi import eidLoose
patElectrons.electronIDSources.eidLoose = cms.InputTag("eidLoose")
goodElectrons = cms.EDFilter("PATElectronRefSelector",
                             src = cms.InputTag("patElectrons"),
                             cut = cms.string(ELECTRON_CUT)
                             )


# DILEPTONS
diMuons = cms.EDProducer("CandViewShallowCloneCombiner",
                         decay       = cms.string("goodMuons goodMuons"),
                         checkCharge = cms.bool(False),
                         cut         = cms.string(DIMUON_CUT),
                         )

diElectrons = cms.EDProducer("CandViewShallowCloneCombiner",
                             decay       = cms.string("goodElectrons goodElectrons"),
                             checkCharge = cms.bool(False),
                             cut         = cms.string(DIELECTRON_CUT)
                             )
crossLeptons  = cms.EDProducer("CandViewShallowCloneCombiner",
                               decay       = cms.string("goodMuons goodElectrons"),
                               checkCharge = cms.bool(False),
                               cut         = cms.string(EMU_CUT)
                               )

# FILTERS
diMuonsFilter = cms.EDFilter("CandViewCountFilter",
                             src = cms.InputTag("diMuons"),
                             minNumber = cms.uint32(1)
                             )
diElectronsFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("diElectrons"),
                                 minNumber = cms.uint32(1)
                                 )
crossLeptonsFilter = cms.EDFilter("CandViewCountFilter",
                                  src = cms.InputTag("crossLeptons"),
                                  minNumber = cms.uint32(1)
                                  )



# SEQUENCES
diMuonSequence = cms.Sequence( startCounter * goodMuons * diMuons * diMuonsFilter * mumuCounter )
diElectronSequence = cms.Sequence( eidLoose * makePatElectrons * startCounter * goodElectrons * diElectrons * diElectronsFilter * eeCounter )
eMuSequence = cms.Sequence( eidLoose * makePatElectrons * startCounter * goodMuons * goodElectrons * crossLeptons * crossLeptonsFilter  * emuCounter )

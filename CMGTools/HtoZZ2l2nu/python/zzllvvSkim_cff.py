import FWCore.ParameterSet.Config as cms

"""
Wrapper to test different cuts
"""
def getCuts(version) :

    if(version=='S1') :
        MUON_CUT=("pt > 10 && abs(eta)<2.5 && (isGlobalMuon || isTrackerMuon)")
        ELECTRON_CUT=("pt > 10 && abs(eta)<2.5")
        DIMUON_CUT=("mass > 40 && daughter(0).pt>20 && daughter(1).pt()>10")
        DIELECTRON_CUT=("mass > 40 && daughter(0).pt>20 && daughter(1).pt()>10")
        EMU_CUT=("mass > 40 && daughter(0).pt>10 && daughter(1).pt()>10")
    if(version=='S2') :
        MUON_CUT=("pt > 7 && abs(eta)<2.5 && isGlobalMuon && (isGlobalMuon || isTrackerMuon)")
        ELECTRON_CUT=("pt > 10 && abs(eta)<2.5")
        DIMUON_CUT=("mass > 40 && daughter(0).pt>20 && daughter(1).pt()>7")
        DIELECTRON_CUT=("mass > 40 && daughter(0).pt>20 && daughter(1).pt()>10")
        EMU_CUT=("mass > 40 && daughter(0).pt>7 && daughter(1).pt()>10")
    return MUON_CUT, DIMUON_CUT, ELECTRON_CUT, DIELECTRON_CUT, EMU_CUT

# define cuts
MUON_CUT, DIMUON_CUT, ELECTRON_CUT, DIELECTRON_CUT, EMU_CUT = getCuts(version='S1')

#
# EVENT COUNTERS
#
startCounter = cms.EDProducer("EventCountProducer")
eeCounter = startCounter.clone()
mumuCounter = startCounter.clone()
emuCounter = startCounter.clone()

# LEPTONS
goodMuons = cms.EDFilter("MuonRefSelector",  src = cms.InputTag("muons"),  cut = cms.string(MUON_CUT) )
goodElectrons = cms.EDFilter("GsfElectronRefSelector",  src = cms.InputTag("gsfElectrons"),  cut = cms.string(ELECTRON_CUT) )


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


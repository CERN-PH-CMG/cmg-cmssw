import FWCore.ParameterSet.Config as cms

##
## dilepton filters
##
def addDileptonFilters(process):

    #leptons
    process.llPreselectionCounter = cms.EDProducer("EventCountProducer")
    process.llSelectionCounter = process.llPreselectionCounter.clone()
    process.lCandidates = cms.EDFilter("CandViewRefSelector",
                                       src = cms.InputTag("particleFlow"),
                                       cut = cms.string("(abs(pdgId())==11 || abs(pdgId())==13) && pt>20 && abs(eta)<2.5")
                                       )
    process.lCandCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("lCandidates"), minNumber = cms.uint32(2))
    process.llCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
                                          decay = cms.string('lCandidates lCandidates'),
                                          cut = cms.string('mass>10'),
                                          checkCharge = cms.bool(False)
                                          )
    process.llCandidateCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("llCandidates"), minNumber = cms.uint32(1))
    process.llCandidateSequence = cms.Sequence( process.llPreselectionCounter *
                                                process.lCandidates *
                                                process.lCandCounter *
                                                process.llCandidates *
                                                process.llCandidateCounter *
                                                process.llSelectionCounter
                                                )
    print " *** ll CandidateSequences is defined"
    
##
## photon filter
##
def addPhotonFilters(process):
    process.photonPreselectionCounter = cms.EDProducer("EventCountProducer")
    process.photonSelectionCounter = process.photonPreselectionCounter.clone()
    process.photonCandidates = cms.EDFilter("CandViewRefSelector",
                                            src = cms.InputTag("particleFlow"),
                                            cut = cms.string("abs(pdgId())==22 && "
                                                             "et>20 && abs(eta)<2.4 && (abs(eta)<1.4442|| abs(eta)>1.566) && "
                                                             "photonRef.isNonnull && "
                                                             "photonRef.hasPixelSeed && "
                                                             "photonRef.trkSumPtSolidConeDR03<5 && photonRef.ecalRecHitSumEtConeDR03<10 && photonRef.hcalTowerSumEtConeDR03<5")  
                                            )
    process.countPhotons = cms.EDFilter("CandViewCountFilter",
                                        minNumber = cms.uint32(1),
                                        maxNumber = cms.uint32(999999),
                                        src = cms.InputTag("photonCandidates")
                                        )
    process.photonCandidateSequence = cms.Sequence( process.photonPreselectionCounter*
                                                    process.photonCandidates*
                                                    process.countPhotons*
                                                    process.photonSelectionCounter )
    print " *** photon CandidateSequences is defined"


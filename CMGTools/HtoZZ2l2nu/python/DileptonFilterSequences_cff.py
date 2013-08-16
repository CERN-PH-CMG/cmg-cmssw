import FWCore.ParameterSet.Config as cms

##
## dilepton filters
##
def addDileptonFilters(process):

    #leptons
    process.llPreselectionCounter = cms.EDProducer("EventCountProducer")
    process.lCandidates = cms.EDProducer("CandViewMerger",
                                         src = cms.VInputTag("gsfElectrons", "muons")
                                         )
    process.lCandCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("lCandidates"), minNumber = cms.uint32(2))
    process.llCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
                                          decay = cms.string('lCandidates lCandidates'),
                                          cut = cms.string('mass>12 && daughter(0).pt()>20 && daughter(1).pt()>20 && abs(daughter(0).eta)<2.5 && abs(daughter(1).eta())<2.5'),
                                          checkCharge = cms.bool(False)
                                          )
    process.llCandidateCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("llCandidates"), minNumber = cms.uint32(1))
    process.llSelectionCounter = process.llPreselectionCounter.clone()
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
    process.photonCandidates = cms.EDFilter("PhotonRefSelector",
                                            src = cms.InputTag("photons"),
                                            cut = cms.string("pt>20 && abs(eta)<2.5 && hadronicOverEm < 0.1 && !hasPixelSeed && trkSumPtSolidConeDR04<10")
                                            )
    process.countPhotons = cms.EDFilter("CandViewCountFilter",
                                        minNumber = cms.uint32(1),
                                        src = cms.InputTag("photonCandidates")
                                        )
    process.photonCandidateSequence = cms.Sequence( process.photonPreselectionCounter*
                                                    process.photonCandidates*
                                                    process.countPhotons*
                                                    process.photonSelectionCounter )
    print " *** photon CandidateSequences is defined"


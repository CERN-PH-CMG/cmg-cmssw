import FWCore.ParameterSet.Config as cms

##
## dilepton filters
##
def addDileptonFilters(process):

    #di-muons
    process.mumuPreselectionCounter = cms.EDProducer("EventCountProducer")
    process.mumuSelectionCounter = process.mumuPreselectionCounter.clone()
    process.mumuCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
                                            decay = cms.string('selectedPatMuonsPFlow selectedPatMuonsPFlow'),
                                            cut = cms.string('mass>10 && daughter(0).pt > 15 && daughter(1).pt >15'),
                                            checkCharge = cms.bool(False)
                                            )
    process.mumuCandidateCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("mumuCandidates"), minNumber = cms.uint32(1))
    process.mumuCandidateSequence = cms.Sequence( process.mumuPreselectionCounter *
                                                  process.mumuCandidates *
                                                  process.mumuCandidateCounter *
                                                  process.mumuSelectionCounter
                                                  )

    #di-electrons
    process.eePreselectionCounter = process.mumuPreselectionCounter.clone()
    process.eeSelectionCounter = process.mumuPreselectionCounter.clone()
    process.eeCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
                                     decay = cms.string('selectedPatElectronsPFlow selectedPatElectronsPFlow'),
                                     cut = cms.string('mass>10 && daughter(0).pt >15 && daughter(1).pt >15'),
                                     checkCharge = cms.bool(False)
                                     )
    process.eeCandidateCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("eeCandidates"), minNumber = cms.uint32(1))
    process.eeCandidateSequence = cms.Sequence( process.eePreselectionCounter *
                                                process.eeCandidates *
                                                process.eeCandidateCounter *
                                                process.eeSelectionCounter
                                                )

    #electron-muon
    process.emuPreselectionCounter = process.mumuPreselectionCounter.clone()
    process.emuSelectionCounter = process.mumuPreselectionCounter.clone()
    process.emuCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
                                           decay = cms.string('selectedPatElectronsPFlow selectedPatMuonsPFlow'),
                                           cut = cms.string('mass>10 && daughter(0).pt >15 && daughter(1).pt >15'),
                                           checkCharge = cms.bool(False)
                                           )
    process.emuCandidateCounter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("emuCandidates"), minNumber = cms.uint32(1))
    process.emuCandidateSequence = cms.Sequence( process.emuPreselectionCounter*
                                                 process.emuCandidates*
                                                 process.emuCandidateCounter*
                                                 process.emuSelectionCounter)
    
    print " *** {mumu,ee,emu}CandidateSquences created"

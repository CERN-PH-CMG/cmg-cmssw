import FWCore.ParameterSet.Config as cms

# This cff is obsolete. Kind of the same work is done in a better way
# in the BadPFMuonFilter

# to run this module, you need in input:
# particleFlow : the collection of reco::PFCandidates
# muons : the collection of reco::Muons

# fitering events with no muons with a momentum wrongly set in the PF algo : 


largeDeltaPtMuons = cms.EDFilter(
    "GenericPFCandidateSelector",
    src = cms.InputTag('particleFlow'),
    cut = cms.string("abs(pdgId())==13 && ( pt() - muonRef().get().pt() )>100")
    )

filterLargeDeltaPtMuons = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("largeDeltaPtMuons"),
    minNumber = cms.uint32(1)
    )

# put the following sequence in your path, say myPath,
# and add the following line to your output module:
# SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('myPath') )

selectEventsWithSmallDeltaPtMuons = cms.Sequence(
    largeDeltaPtMuons +
    ~filterLargeDeltaPtMuons # passes if no bad muon is found
    )

selectEventsWithSmallDeltaPtMuonsIgnore = cms.Sequence(
    largeDeltaPtMuons +
    cms.ignore( filterLargeDeltaPtMuons )
    )



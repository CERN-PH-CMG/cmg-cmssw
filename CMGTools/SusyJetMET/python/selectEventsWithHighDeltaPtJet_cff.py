import FWCore.ParameterSet.Config as cms


# to run this module, you need in input:
# particleFlow : the collection of reco::PFCandidates
# muons : the collection of reco::Muons
highDeltaPtJets = cms.EDFilter(
    "PATJetSelector",
    src = cms.InputTag('patJetsAK5PF'),
    cut = cms.string("(pt() - genJet().pt())/genJet().pt()>2")
    )

filterHighDeltaPtJets = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highDeltaPtJets"),
    minNumber = cms.uint32(1)
    )

# put the following sequence in your path, say myPath,
# and add the following line to your output module:
# SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('myPath') )

selectEventsWithHighDeltaPtJet = cms.Sequence(
    highDeltaPtJets + 
    filterHighDeltaPtJets
    )





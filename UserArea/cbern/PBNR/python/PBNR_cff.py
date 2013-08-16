import FWCore.ParameterSet.Config as cms

from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets 

pfJetSource = 'ak5PFJets'

# selects jets with pt>30 and a neutral hadron fraction
# larger than 90% (fake deposit in HCAL)
pfJetNeutralHadFracGT90 = selectedPfJets.clone( src = pfJetSource,
                                          cut='pt()>30 && neutralHadronEnergyFraction()>0.90' )

# selects jets with pt>30 and a neutral hadron fraction
# larger than 90% (fake deposit in ECAL, or photon)
pfJetPhotonFracGT95 = selectedPfJets.clone( src = pfJetSource,
                                      cut='pt()>30 && photonEnergyFraction()>0.95' )

# select the bad jets
PBNRJetIdSequence = cms.Sequence(
    pfJetNeutralHadFracGT90 +
    pfJetPhotonFracGT95
    )

pfJetNeutralHadFracGT90Count = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("pfJetNeutralHadFracGT90"),
    minNumber = cms.uint32(1)
    )

pfJetPhotonFracGT95Count = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("pfJetPhotonFracGT95"),
    minNumber = cms.uint32(1)
    )

# reject events containing at least one bad jet.
PBNREventFiltering = cms.Sequence(
    # the negation of this module keeps events
    # which don't contain at least one jet passing the selection.
    ~pfJetNeutralHadFracGT90Count +
    ~pfJetPhotonFracGT95Count
    )


# full PBNR sequence
PBNRSequence = cms.Sequence(
    PBNRJetIdSequence +
    PBNREventFiltering
    )

import FWCore.ParameterSet.Config as cms


def PF2PATonGEN(process, genProcessName=None):

    # can only work if a collection of vertices is available ---

    # if genProcessName == None:
    #    genProcessName = process.name_()

    #COLIN no need to modify patDefaultSequencePFlow, if this sequence
    # is not included... 

    # disabling pile-up masking
    process.pfPileUpPFlow.Enable = False

    # disabling vertex constraints for e and mu
    process.PF2PATPFlow.remove( process.pfMuonsFromVertexPFlow )
    process.pfSelectedMuonsPFlow.src = 'pfAllMuonsPFlow'

    process.PF2PATPFlow.remove( process.pfElectronsFromVertexPFlow )
    process.pfSelectedElectronsPFlow.src = 'pfAllElectronsPFlow'

    # can only work if tracks (and maybe vertices) are present --- 

    # removing taus from PF2PAT
    process.PF2PATPFlow.remove( process.pfTauSequencePFlow )
    process.PF2PATPFlow.remove( process.pfNoTauPFlow )

    # disabling muon and electron top projections
    # as done in RA2. These guys will end up in the jets
    process.pfNoMuonPFlow.enable = False
    process.pfNoElectronPFlow.enable = False
    
    # matching the PFCandidate to the gen particles, not
    # the corresponding traditional object, which is not available. 
    process.electronMatchPFlow.src = 'pfIsolatedElectronsPFlow'
    process.muonMatchPFlow.src = 'pfIsolatedMuonsPFlow'

    # removing MET corrections
    process.makePatMETsPFlow.remove( process.metJESCorAK5CaloJetPFlow )
    process.makePatMETsPFlow.remove( process.metJESCorAK5CaloJetMuonsPFlow )

    # removing taus from PAT, as they have been removed from PF2PAT
    process.patDefaultSequencePFlow.remove( process.makePatTausPFlow )
    process.patDefaultSequencePFlow.remove( process.selectedPatTausPFlow )
    process.patDefaultSequencePFlow.remove( process.countPatTausPFlow )

    # removing photons, not available
    process.patDefaultSequencePFlow.remove( process.makePatPhotonsPFlow )
    process.patDefaultSequencePFlow.remove( process.selectedPatPhotonsPFlow )
    process.patDefaultSequencePFlow.remove( process.countPatPhotonsPFlow )

    # jet correction is not done 
    process.makePatJetsPFlow.remove( process.patJetCorrectionsPFlow )

    # swithToPFJets( process, cms.InputTag('pfJetsPFlow'), 'AK5', 'PFlow', None ) 
    # the jets are matched to the collection of GenParticles
    # that gave rise to PFCandidates.
    # -> the matching between pat::Jets and gen jets is perfect
    process.ak5GenJetsNoNu.src = 'particleFlow:GEN' 

    # do not look for jet RECO information
    process.patJetsPFlow.addAssociatedTracks = False
    process.patJetsPFlow.addBTagInfo = False
    process.patJetsPFlow.addDiscriminators = False
    process.patJetsPFlow.addEfficiencies = False
    process.patJetsPFlow.addJetCharge = False
    process.patJetsPFlow.addJetCorrFactors = False
    process.patJetsPFlow.addJetID = False
    process.patJetsPFlow.addTagInfos = False
    
    process.makePatJetsPFlow.remove( process.jetTracksAssociatorAtVertexPFlow )
    process.makePatJetsPFlow.remove( process.btaggingAODPFlow )
    process.makePatJetsPFlow.remove( process.patJetChargePFlow )

    process.PF2PATonGENSequence = cms.Sequence(
        process.PF2PATPFlow +
        process.genForPF2PATSequence +
        process.makePatJetsPFlow +
        process.selectedPatJetsPFlow +
        process.makePatMETsPFlow
        ) 

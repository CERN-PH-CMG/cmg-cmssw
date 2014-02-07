import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix

def embedTracksInTaus( process, postfix, enable):
    print "Embedding tracks in paTaus: "
    print enable
    patTaus = getattr(process,'patTaus'+postfix)
    patTaus.embedLeadTrack                        = cms.bool(enable)
    patTaus.embedSignalTracks                     = cms.bool(enable)
    patTaus.embedIsolationTracks                  = cms.bool(enable)

def embedPFCandidatesInTaus( process, postfix, enable ):
    print "Embedding pfCandidates in paTaus: "
    print enable
    patTaus = getattr(process,'patTaus'+postfix)
    patTaus.embedLeadPFCand                       = cms.bool(enable)
    patTaus.embedLeadPFChargedHadrCand            = cms.bool(enable)
    patTaus.embedLeadPFNeutralCand                = cms.bool(enable)
    patTaus.embedSignalPFCands                    = cms.bool(enable)
    patTaus.embedSignalPFChargedHadrCands         = cms.bool(enable)
    patTaus.embedSignalPFNeutralHadrCands         = cms.bool(enable)
    patTaus.embedSignalPFGammaCands               = cms.bool(enable)
    patTaus.embedIsolationPFCands                 = cms.bool(enable)
    patTaus.embedIsolationPFChargedHadrCands      = cms.bool(enable)
    patTaus.embedIsolationPFNeutralHadrCands      = cms.bool(enable)
    patTaus.embedIsolationPFGammaCands            = cms.bool(enable)



# insert a selected pfJet and apply it to the HPS Tau sequences (this is now compatible with PF2PAT in 5_2_3)
from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets 
def adaptSelectedPFJetForHPSTau(process,
                                jetSelection = "",
                                postfix = ""):
    print "Preselecting the jets used to make taus : "+jetSelection
    process.pfJetsForHPSTau = selectedPfJets.clone()
    process.pfJetsForHPSTau.src = "pfJets"+postfix
    process.pfJetsForHPSTau.cut = cms.string(jetSelection)
    setattr(process,"pfJetsForHPSTau"+postfix, process.pfJetsForHPSTau)
    getattr(process,"pfTausPreSequence"+postfix).insert(0,getattr(process,"pfJetsForHPSTau"+postfix))
    applyPostfix(process,"pfTausBase",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    # need to fix the tau presequence because it depends on the new jet collection
    applyPostfix(process,"pfJetTracksAssociatorAtVertex",postfix).jets = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfTauPFJets08Region",postfix).src = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfJetsPiZeros",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfJetsLegacyTaNCPiZeros",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfJetsLegacyHPSPiZeros",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    # fix because the TopProjection in pfNoTau does not work because the taus come from the selected jets
    applyPostfix(process,"pfNoTau",postfix).bottomCollection = "pfJetsForHPSTau"+postfix
    # must use the pfJets for the patJets otherwise we will have the above selection on the pfJetsForHPSTau ( pfNoTau is a jet collection)
    print 'Warning!!!!!!!!!!!!!: switching patJet.jetSource from pfNoTau to pfJets because Tau cleaning on the jets cannot be applied currently'
    applyPostfix(process,"patJets",postfix).jetSource = "pfJets"+postfix

#removes isolation cut in PF2PAT and adds only the decayModeFinding (this is now compatible with PF2PAT in 5_2_3)
def removeHPSTauIsolation(process, postfix = ""):
    print "removing tau isolation discriminators and applying only the decayModeFinding "
    #note that isolation discriminators have an implicit decayModeFinding requirement
    getattr(process,"pfTausBaseSequence"+postfix).remove(applyPostfix(process,"pfTausBaseDiscriminationByLooseCombinedIsolationDBSumPtCorr",postfix))
    getattr(process,"pfTaus"+postfix).discriminators = [cms.PSet(discriminator = cms.InputTag("pfTausBaseDiscriminationByDecayModeFinding"+postfix),
                                                                 selectionCut = cms.double(0.5)
                                                                 )]
    
    

#switch the input Taus to PAT, in 5_2_X input was switched to unselected taus so patTuple blows up
def switchInputTausToPAT(process,  postfix = ""):
    print "switching input tau collection to PAT from hpsPFTauProducer --> pfTaus (as was in 4_2_X)"
    from PhysicsTools.PatAlgos.tools.tauTools import switchToPFTauByType
    from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceParam
    oldTauSrc = getattr(process,"patTaus"+postfix).tauSource
    newTauSrc = cms.InputTag("pfTaus"+postfix)
    switchToPFTauByType(process,pfTauType="hpsPFTau",
                        pfTauLabelOld=oldTauSrc,pfTauLabelNew=newTauSrc,
                        postfix=postfix)
    massSearchReplaceParam(getattr(process,"patHPSPFTauDiscrimination"+postfix),
                           "PFTauProducer",oldTauSrc,newTauSrc)
        

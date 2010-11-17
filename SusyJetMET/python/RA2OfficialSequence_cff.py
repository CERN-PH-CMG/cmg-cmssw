from PhysicsTools.PatAlgos.patTemplate_cfg import *

def addSUSYPATRA2( process, triggerProcessName = 'HLT'):
    process.GlobalTag.globaltag = 'GR10_P_V10::All'

    from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT

    addDefaultSUSYPAT(process,False,triggerProcessName,'Spring10','',['AK5Calo','AK5PF'],doSusyTopProjection=True)
    # switch off some embedding
    process.patJetsAK5Calo.embedCaloTowers = False
    process.patJetsAK5Calo.embedPFCandidates = False
    process.patJetsAK5PF.embedCaloTowers = False
    process.patJetsAK5PF.embedPFCandidates = False
    # process.patJetsIC5Calo.embedCaloTowers = False
    # process.patJetsIC5Calo.embedPFCandidates = False
    
    # replace trigger sequence
    process.patDefaultSequence.remove(process.patTrigger)
    process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
    process.patTriggerSequence = cms.Sequence(process.patTrigger)
    
    from SandBox.Skims.RA2PAT_cff import defRA2PATSequence
    defRA2PATSequence(process, False)
    process.load('SandBox.Skims.RA2Objects_cff')
    process.ra2PATSequence *= process.ra2Objects

    process.prefilterCounter        = cms.EDProducer("EventCountProducer")
    process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")
    process.postCaloCleaningCounter = cms.EDProducer("EventCountProducer")
    process.postPFCleaningCounter   = cms.EDProducer("EventCountProducer")
    process.postCaloJetsCounter     = cms.EDProducer("EventCountProducer")
    process.postPFJetsCounter       = cms.EDProducer("EventCountProducer")
    process.postCaloHTCounter       = cms.EDProducer("EventCountProducer")
    process.postPFHTCounter         = cms.EDProducer("EventCountProducer")

    process.cleanpatseq = cms.Sequence(
        process.ra2PATSequence *
#        process.prefilterCounter *
        process.ra2StdCleaning 
#        process.postStdCleaningCounter
        )

    process.caloseq = cms.Sequence(
        process.cleanpatseq *
        process.ra2CaloCleaning *
#        process.postCaloCleaningCounter *
        process.countJetsAK5CaloPt50Eta25ID *
#        process.postCaloJetsCounter *
        process.htCaloFilter 
#        process.postCaloHTCounter
        )


    process.pcalo = cms.Path( process.caloseq )

#    process.dummy = cms.Sequence()
#    process.pcalo.replace(process.patJetCorrFactorsIC5Calo, process.dummy )
#    process.pcalo.replace(process.patJetsIC5Calo, process.dummy )
#    process.pcalo.replace(process.jetTracksAssociatorAtVertexIC5Calo, process.dummy )
#    process.pcalo.replace(process.impactParameterTagInfosIC5Calo, process.dummy )
#    process.pcalo.replace(process.secondaryVertexTagInfosIC5Calo, process.dummy )

    process.pfseq = cms.Sequence(
        process.cleanpatseq *
        process.ra2PFCleaning *
 #       process.postPFCleaningCounter *
        process.countJetsAK5PFPt50Eta25ID *
 #       process.postPFJetsCounter *
        process.htPFFilter 
 #       process.postPFHTCounter
        )


    process.ppf = cms.Path( process.pfseq )



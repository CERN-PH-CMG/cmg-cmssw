from PhysicsTools.PatAlgos.patTemplate_cfg import *

def addSUSYPATRA2( process, hltMenu = 'HLT', runningOnMC = True ):
    # process.GlobalTag.globaltag = 'GR10_P_V10::All'

    # should be an argument of the function
    
    jetMetCorr = ['L2Relative', 'L3Absolute']
    if runningOnMC == False: jetMetCorr.append('L2L3Residual')
    theJetColls = ['AK5Calo','AK5PF','IC5Calo']

    
    from SandBox.Skims.RA2PAT_cff import defRA2PATSequence
    defRA2PATSequence( process, mcInfo=runningOnMC, HLTMenu=hltMenu, jetMetCorrections=jetMetCorr, mcVersion='', theJetNames=theJetColls )
    

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
        process.countJetsAK5CaloPt50Eta25 *
        process.htCaloFilter 
        )


    process.pcalo = cms.Path( process.caloseq )

    process.pfJetID = cms.Sequence(
        process.ra2IDPFJets +
        process.countJetsAK5PFPt50Eta25ID +
        process.mhtPFID +
        process.mhtPFIDFilter +
        process.htPFID + 
        process.htPFIDFilter 
        )

    process.pfseq = cms.Sequence(
        process.cleanpatseq +
        #       process.ra2PFCleaning +
        process.countJetsAK5PFPt50Eta25 +
        # process.pfJetID +
        process.htPFFilter 
        )

    if runningOnMC==True:
        # trigger primitive information is not available in MC.
        # must remove the TP filter from the various sequences
        process.pfseq.remove( process.ecalDeadCellTPonlyFilter )
        process.caloseq.remove( process.ecalDeadCellTPonlyFilter )

    process.ppf = cms.Path( process.pfseq )



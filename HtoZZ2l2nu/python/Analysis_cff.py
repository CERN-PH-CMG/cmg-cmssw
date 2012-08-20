import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.ClusteredPFMetProducer_cfi import ClusteredPFMetProducer
from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
from CommonTools.ParticleFlow.ParticleSelectors.pfAllNeutralHadronsAndPhotons_cfi import pfAllNeutralHadronsAndPhotons

def defineAnalysis(process,castorDir="",reRunRho=False) :

    # our MET producer
    process.ClusteredPFMetProducer = ClusteredPFMetProducer.clone()

    #
    # configure the analyzer (cf. base values are in the StandardSelections_cfi)
    #
    process.evAnalyzer = cms.EDAnalyzer("DileptonPlusMETEventAnalyzer",
                                        dtag=cms.string('h2zz'),
                                        Trigger = BaseTriggerSelection.clone(),
                                        Generator = BaseGeneratorSelection.clone(),
                                        Vertices = BaseVertexSelection.clone(),
                                        Photons = BasePhotonsSelection.clone(),
                                        LooseMuons = BaseLooseMuonsSelection.clone(),
                                        Muons = BaseMuonsSelection.clone(),
                                        LooseElectrons = BaseLooseElectronsSelection.clone(),
                                        Electrons = BaseElectronsSelection.clone(),
                                        Dileptons = BaseDileptonSelection.clone(),
                                        Jets = BaseJetSelection.clone(),
                                        AssocJets = AssocJetSelection.clone(),
                                        MET = BaseMetSelection.clone()
                                        )
    
    if(castorDir.find('12_03_13_HZZ2l2v_pat')>0):
        print ' Warning: applying PF muon patch to standard selection'
        process.evAnalyzer.Muons.source=cms.InputTag("selectedPatMuonsPFlow")
        process.evAnalyzer.Muons.sourceIsPF=cms.bool(True)
        process.evAnalyzer.LooseMuons.source=process.evAnalyzer.Muons.source
        process.evAnalyzer.LooseMuons.sourceIsPF=process.evAnalyzer.Muons.sourceIsPF

    #MVAs for IDs
    #process.load('EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi')
    #process.mvaIDs = cms.Sequence(  process.mvaTrigV0 + process.mvaNonTrigV0 )
    
    from JetMETCorrections.Configuration.JetCorrectionProducers_cff import ak5PFJetsL1L2L3
    process.ak5PFJetsL1L2L3ForMVAMET=ak5PFJetsL1L2L3.clone()
    
        
    #rho for muon isolation
    if(reRunRho) :
        process.pfAllNeutralHadronsAndPhotons = pfAllNeutralHadronsAndPhotons.clone( src=cms.InputTag("particleFlow") )
        process.kt6PFJetsCentralNeutral = kt4PFJets.clone(  src = cms.InputTag("pfAllNeutralHadronsAndPhotons"),
                                                            rParam = cms.double(0.6),
                                                            doAreaFastjet = cms.bool(True),
                                                            doRhoFastjet = cms.bool(True),
                                                            Ghost_EtaMax = cms.double(3.1),
                                                            Rho_EtaMax = cms.double(2.5),
                                                            inputEtMin = cms.double(0.5)
                                                            )
        process.rhoForIsolationSequence = cms.Sequence(process.pfAllNeutralHadronsAndPhotons*process.kt6PFJetsCentralNeutral)
        process.analysis = cms.Path( #process.mvaIDs +
            process.rhoForIsolationSequence +
            process.ak5PFJetsL1L2L3ForMVAMET +
            process.ClusteredPFMetProducer +
            process.evAnalyzer)
    else :
        process.analysis = cms.Path( #process.mvaIDs +
            process.ak5PFJetsL1L2L3ForMVAMET +
            process.ClusteredPFMetProducer +
            process.evAnalyzer)
        
     


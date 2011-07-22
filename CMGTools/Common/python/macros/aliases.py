#!/usr/bin/env python

patObjectsAK5 = {
    'patJetsAK5':'patJets_selectedPatJetsAK5__PAT',
    'patElectronsAK5':'patElectrons_selectedPatElectronsAK5__',
    'patMuonsAK5':'patMuons_selectedPatMuonsAK5__',
    'patTausAK5':'patTaus_selectedPatTausAK5__'
    }

patObjectsAK5LC = {
    'patJetsAK5LC':'patJets_selectedPatJetsAK5LC__PAT',
    'patElectronsAK5LC':'patElectrons_selectedPatElectronsAK5LC__',
    'patMuonsAK5LC':'patMuons_selectedPatMuonsAK5LC__',
    'patTausAK5LC':'patTaus_selectedPatTausAK5LC__'
    }

patGenAK5 = {
    'genJets':'recoGenJets_selectedPatJetsAK5_genJets_'
    }

pat = dict( patObjectsAK5.items() + patObjectsAK5LC.items() + patGenAK5.items() )


cmgObjects = {
    'jets':'cmgPFJets_cmgPFJetSel__',
    'jetsVLId99Failed':'cmgPFJets_cmgPFJetVeryLooseJetId99Failed__',    
    'jetsVLId95Failed':'cmgPFJets_cmgPFJetVeryLooseJetId95Failed__', 
    'basejets':'cmgBaseJets_cmgPFBaseJetSel__',
    'met':'cmgBaseMETs_cmgPFMET__',
    'eles':'cmgElectrons_cmgElectronSel__',
    'mus':'cmgMuons_cmgMuonSel__',
    }

filters = {
    'hbheNoise2011Iso':'bool_HBHENoiseFilterResultProducer2011IsoDefault_HBHENoiseFilterResult_',
    'hbheNoise2011NonIso':'bool_HBHENoiseFilterResultProducer2011NonIsoRecommended_HBHENoiseFilterResult_',
    'hbheNoise2010':'bool_HBHENoiseFilterResultProducer2010_HBHENoiseFilterResult_',
    'inconsMuons':'bool_inconsistentMuonsTagging_Result_',
    'greedyMuons':'bool_greedyMuonsTagging_Result_',
    'beamHaloCSCLoose':'recoBeamHaloSummary_BeamHaloSummary__RECO.obj[0].CSCLooseHaloId()',
    'beamHaloCSCTight':'recoBeamHaloSummary_BeamHaloSummary__RECO.obj[0].CSCTightHaloId()',
    'ecalDeadTP':'bool_ecalDeadCellTPfilter__',
    'badEERecov':'bool_recovRecHitFilter_Result_',
    'eeNoise':'bool_eeNoiseFilter_Result_',
    'trackingFailure':'bool_trackingFailureFilter_Result_',
    }

eventId = {
    'run':'EventAuxiliary.id().run()',
    'lumi':'EventAuxiliary.id().luminosityBlock()',
    'event':'EventAuxiliary.id().event()'    
    }

gen = {
    'gen':'recoGenParticles_genParticlesStatus3__'
    }

misc = {
    'dPhiMin':'float_deltaPhiJetMET__' 
    }


eventsAliases = dict(cmgObjects.items() + filters.items() + eventId.items() + gen.items() + misc.items() )

luminosityBlocksAliases = {
    'lumi':'LuminosityBlockAuxiliary.id().luminosityBlock()',
    'run':'LuminosityBlockAuxiliary.id().run()'
    }



#!/usr/bin/env python

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
    'inconsMuons':'bool_inconsistentMuonsTagging_Result_PAT.obj',
    'greedyMuons':'bool_greedyMuonsTagging_Result_PAT.obj',
    'beamHaloCSCLoose':'recoBeamHaloSummary_BeamHaloSummary__RECO.obj[0].CSCLooseHaloId()',
    'beamHaloCSCTight':'recoBeamHaloSummary_BeamHaloSummary__RECO.obj[0].CSCTightHaloId()',
    'ecalDeadTP':'bool_ecalDeadCellTPfilter__PAT.obj'
    }

eventId = {
    'run':'EventAuxiliary.id().run()',
    'lumi':'EventAuxiliary.id().luminosityBlock()',
    'event':'EventAuxiliary.id().event()'    
    }

eventsAliases = dict(cmgObjects.items() + filters.items() + eventId.items() )

luminosityBlocksAliases = {
    'lumi':'LuminosityBlockAuxiliary.id().luminosityBlock()',
    'run':'LuminosityBlockAuxiliary.id().run()'
    }



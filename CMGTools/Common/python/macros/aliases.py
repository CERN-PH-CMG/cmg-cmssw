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
    'hbheNoise':'bool_HBHENoiseFilterResultProducer_HBHENoiseFilterResult_PAT.obj',
    'inconsMuons':'bool_inconsistentMuonsTagging_Result_PAT.obj',
    'greedyMuons':'bool_greedyMuonsTagging_Result_PAT.obj',
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



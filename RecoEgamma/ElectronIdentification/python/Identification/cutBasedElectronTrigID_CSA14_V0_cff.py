import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry

ebCutOff = 1.479

cutBasedElectronTrigIDCSA14V0 = cms.PSet(
    idName = cms.string("cutBasedElectronTrigID-CSA14-V0"),
    cutFlow = cms.VPSet(
        cms.PSet( cutName = cms.string("MinPtCut"),
                  minPt = cms.double(10.0),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)                ),
        cms.PSet( cutName = cms.string("GsfEleSCEtaMultiRangeCut"),
                  useAbsEta = cms.bool(True),
                  allowedEtaRanges = cms.VPSet( 
                                  cms.PSet( minEta = cms.double(0.0), 
                                            maxEta = cms.double(ebCutOff) ),
                                  cms.PSet( minEta = cms.double(ebCutOff), 
                                            maxEta = cms.double(2.5) )
                                  ),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)),
        cms.PSet( cutName = cms.string('GsfEleDEtaInCut'),
                  dEtaInCutValueEB = cms.double(0.007),
                  dEtaInCutValueEE = cms.double(0.009),
                  barrelCutOff = cms.double(ebCutOff),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)),
        cms.PSet( cutName = cms.string('GsfEleDPhiInCut'),
                  dPhiInCutValueEB = cms.double(0.15),
                  dPhiInCutValueEE = cms.double(0.10),
                  barrelCutOff = cms.double(ebCutOff),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)),
        cms.PSet( cutName = cms.string('GsfEleHadronicOverEMCut'),
                  hadronicOverEMCutValueEB = cms.double(0.12),
                  hadronicOverEMCutValueEE = cms.double(0.10),
                  barrelCutOff = cms.double(ebCutOff),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)),
        cms.PSet( cutName = cms.string('GsfEleRelatEcalIsoCut'),
                  relatECALisoCutValueEB = cms.double(0.2),
                  relatECALisoCutValueEE = cms.double(0.2),
                  barrelCutOff = cms.double(ebCutOff),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)),
        cms.PSet( cutName = cms.string('GsfEleRelatHcalIsoCut'),
                  relatHCALisoCutValueEB = cms.double(0.2),
                  relatHCALisoCutValueEE = cms.double(0.2),
                  barrelCutOff = cms.double(ebCutOff),
                  needsAdditionalProducts = cms.bool(False),
                  isIgnored = cms.bool(False)),
         cms.PSet( cutName = cms.string('GsfEleRelatTkIsoCut'),
                   relatTKisoCutValueEB = cms.double(0.2),
                   relatTKisoCutValueEE = cms.double(0.2),
                   barrelCutOff = cms.double(ebCutOff),
                   needsAdditionalProducts = cms.bool(False),
                   isIgnored = cms.bool(False)),
        cms.PSet( cutName = cms.string('GsfEleFull5x5SigmaIEtaIEtaCut'),
                  full5x5SigmaIEtaIEtaCutValueEB = cms.double(0.01),
                  full5x5SigmaIEtaIEtaCutValueEE = cms.double(0.03),
                  full5x5SigmaIEtaIEtaMap = cms.InputTag('electronIDValueMapProducer:eleFull5x5SigmaIEtaIEta'),
                  barrelCutOff = cms.double(ebCutOff),
                  needsAdditionalProducts = cms.bool(True),
                  isIgnored = cms.bool(False)),
    )
)

central_id_registry.register(cutBasedElectronTrigIDCSA14V0.idName,
                            '13a18621e98e770366829759499f949c')

import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.collectionSize.vertexSize_cfi import vertexSize

from CMGTools.Common.miscProducers.collectionSize.candidateSize_cfi import candidateSize
muonSelSize = candidateSize.clone( src='cmgMuonSel')
electronSelSize = candidateSize.clone( src='cmgElectronSel')
tauSelSize = candidateSize.clone( src='cmgTauSel')
pfJetSelSize = candidateSize.clone( src='cmgPFJetSel')

collectionSizeSequence = cms.Sequence(
    vertexSize +
    muonSelSize +
    electronSelSize +
    tauSelSize +
    pfJetSelSize
    )



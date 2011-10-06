import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.collectionSize.vertexSize_cfi import vertexSize

from CMGTools.Common.miscProducers.collectionSize.candidateSize_cfi import candidateSize
cmgMuonSelSize = candidateSize.clone( src='cmgMuonSel')
cmgElectronSelSize = candidateSize.clone( src='cmgElectronSel')
cmgTauSelSize = candidateSize.clone( src='cmgTauSel')
cmgPFJetSelSize = candidateSize.clone( src='cmgPFJetSel')

collectionSizeSequence = cms.Sequence(
    vertexSize +
    cmgMuonSelSize +
    cmgElectronSelSize +
    cmgTauSelSize +
    cmgPFJetSelSize
    )



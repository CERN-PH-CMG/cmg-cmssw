import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *

MHTCut = 150
HTCut = 300
jetPtCut = 50
jetEtaCut = 2.5

###################   SKIMMING ####################################

#COLIN check the eta cut for the jets for the computation of HT and MHT

# jet skim 

RA2cmgPFJetSel = cmgCandSel.clone( src = 'cmgPFJetSel',
                                   cut = 'pt()>50 && abs(eta)<2.5' )
RA2cmgPFJetCount = cmgCandCount.clone( src = 'RA2cmgPFJetSel',
                                       minNumber = 3 )
RA2Jet = cms.Sequence(
    RA2cmgPFJetSel 
    # RA2cmgPFJetCount
    )

# HT skim 

from CMGTools.Common.met_cff import cmgMHTPFJets30
RA2cmgMHTPFJets50 = cmgMHTPFJets30.clone()
RA2cmgMHTPFJets50.cfg.ptThreshold = 50.0

from CMGTools.Common.skims.cmgBaseMETSel_cfi import cmgBaseMETSel

RA2cmgMHTPFJets50Sel = cmgBaseMETSel.clone( src = 'RA2cmgMHTPFJets50',
                                            cut = 'sumEt()>300') 

RA2cmgMHTPFJets50Count = cmgCandCount.clone( src = 'RA2cmgMHTPFJets50Sel',
                                             minNumber = 1 )

RA2HT = cms.Sequence(
    RA2cmgMHTPFJets50 + 
    RA2cmgMHTPFJets50Sel 
    # RA2cmgMHTPFJets50Count
    )

# MHT skim

RA2cmgMHTPFJets30Sel = cmgBaseMETSel.clone( src = 'cmgMHTPFJets30',
                                            cut = 'et()>150' )
RA2cmgMHTPFJets30Count = cmgCandCount.clone( src = 'RA2cmgMHTPFJets30Sel',
                                             minNumber = 1 )

RA2MHT = cms.Sequence(
    RA2cmgMHTPFJets30Sel 
    # RA2cmgMHTPFJets30Count
    )


RA2ObjectSequence = cms.Sequence(
    RA2Jet +
    RA2HT +
    RA2MHT 
    )

###################  HISTOGRAMMING ####################################

from CMGTools.Common.histogram_cff import *
RA2mhtPFJets30Histograms = mhtPFJets30Histograms.clone(
    inputCollection = 'RA2cmgMHTPFJets30Sel'
    )



RA2HistogramSequence = cms.Sequence(
    RA2mhtPFJets30Histograms
    )


RA2Sequence = cms.Sequence(
    RA2ObjectSequence + 
    RA2HistogramSequence 
    ) 

RA2SkimSequence = cms.Sequence(
    RA2ObjectSequence + 
    RA2cmgPFJetCount +
    RA2cmgMHTPFJets50Count +
    RA2cmgMHTPFJets30Count
    )

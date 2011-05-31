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

RA2PFJet50Central = cmgCandSel.clone( src = 'cmgPFJetSel',
                                   cut = 'pt()>50 && abs(eta)<2.5' )
RA2PFJetCount = cmgCandCount.clone( src = 'RA2PFJet50Central',
                                       minNumber = 3 )

# HT skim 

from CMGTools.Common.met_cff import cmgMHTPFJet30
RA2MHTPFJet50Central = cmgMHTPFJet30.clone()
RA2MHTPFJet50Central.cfg.ptThreshold = 50.0
RA2MHTPFJet50Central.cfg.inputCollection = 'RA2PFJet50Central'


from CMGTools.Common.skims.cmgBaseMETSel_cfi import cmgBaseMETSel

RA2MHTPFJet50CentralSel = cmgBaseMETSel.clone( src = 'RA2MHTPFJet50Central',
                                               cut = 'sumEt()>250') 

RA2MHTPFJet50CentralCount = cmgCandCount.clone( src = 'RA2MHTPFJet50CentralSel',
                                             minNumber = 1 )

RA2HTSequence = cms.Sequence(
    RA2MHTPFJet50Central + 
    RA2MHTPFJet50CentralSel 
    # RA2MHTPFJet50CentralCount
    )

# MHT skim

RA2MHTPFJet30 = cmgMHTPFJet30.clone()

#COLIN: missing basejets in common pat-tuples!
RA2MHTPFJet30.cfg.inputCollection = 'cmgPFBaseJetSel'

RA2MHTPFJet30Sel = cmgBaseMETSel.clone( src = 'RA2MHTPFJet30',
                                        cut = 'et()>50' )
RA2MHTPFJet30Count = cmgCandCount.clone( src = 'RA2MHTPFJet30Sel',
                                         minNumber = 1 )

# COLIN: need to apply lepton veto
# COLIN: need to apply delta phi cuts
# COLIN: need to apply event cleaners

RA2ObjectSequence = cms.Sequence(
    RA2PFJet50Central +
    RA2HTSequence +
    RA2MHTPFJet30 + 
    RA2MHTPFJet30Sel
    )

###################  HISTOGRAMMING ####################################

from CMGTools.Common.histogram_cff import *
RA2MHTPFJet30Histograms = MHTPFJet30Histograms.clone(
    inputCollection = 'RA2MHTPFJet30Sel'
    )



RA2HistogramSequence = cms.Sequence(
    RA2MHTPFJet30Histograms
    )


RA2Sequence = cms.Sequence(
    RA2ObjectSequence + 
    RA2HistogramSequence 
    ) 

RA2SkimSequence = cms.Sequence(
    RA2ObjectSequence + 
    RA2PFJetCount +
    RA2MHTPFJet50CentralCount +
    RA2MHTPFJet30Count
    )

import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *

#not used ?
MHTCut = 150
HTCut = 300
jetPtCut = 50
jetEtaCut = 2.5

###################   SKIMMING ####################################

#COLIN check the eta cut for the jets for the computation of HT and MHT

# jet skim for HT (>=3 central jets, pt > 50)
RA2PFJet50Central = cmgPFJetSel.clone( src = 'cmgPFJetSel',
                                       cut = 'pt()>50. && abs(eta)<2.5' )
RA2PFJetCount     = cmgCandCount.clone( src = 'RA2PFJet50Central',
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
RA2MHTPFJet30.cfg.inputCollection = cms.InputTag("cmgPFBaseJetSel")

RA2MHTPFJet30Sel = cmgBaseMETSel.clone( src = 'RA2MHTPFJet30',
                                        cut = 'et()> 50' )
RA2MHTPFJet30Count = cmgCandCount.clone( src = 'RA2MHTPFJet30Sel',
                                         minNumber = 1 )


# COLIN: need to apply lepton veto 
# from CMGTools.Susy.commonLeptonSequence_cfi import *

## RA2ElectronCount = cmgCandCount.clone(
##     src = "susyElectron",
##     minNumber = 1
##     )

## RA2MuonCount = cmgCandCount.clone(
##     src = "susyMuon",
##     minNumber = 1
##     )

## RA2LeptonVetoSequence = cms.Sequence(
##    ~RA2ElectronCount +
##    ~RA2MuonCount
##     )

# COLIN: need to apply delta phi cuts

from CMGTools.Common.miscProducers.deltaPhiJetMET_cfi import *
from CMGTools.Common.skims.indexCMGPFJetSelector_cfi import *

RA2Jet0 = indexCMGPFJetSelector.clone( inputCollection = 'RA2PFJet50Central', min = 0, max = 0)
RA2Jet1 = indexCMGPFJetSelector.clone( inputCollection = 'RA2PFJet50Central', min = 1, max = 1)
RA2Jet2 = indexCMGPFJetSelector.clone( inputCollection = 'RA2PFJet50Central', min = 2, max = 2)

# RA2Jet0.verbose = True
# RA2Jet12.verbose = True

RA2dPhi0 = deltaPhiJetMET.clone(objects = 'RA2Jet0')
RA2dPhi1 = deltaPhiJetMET.clone(objects = 'RA2Jet1')
RA2dPhi2 = deltaPhiJetMET.clone(objects = 'RA2Jet2')

dPhiSequence = cms.Sequence(
    RA2Jet0 +
    RA2Jet1 +
    RA2Jet2 +
    RA2dPhi0 +
    RA2dPhi1 +
    RA2dPhi2
    )

RA2ObjectSequence = cms.Sequence(
    RA2PFJet50Central +
    RA2HTSequence +
    RA2MHTPFJet30 + 
    RA2MHTPFJet30Sel +
    dPhiSequence
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
    RA2MHTPFJet30Count ## +
   ## RA2LeptonVetoSequence
    )

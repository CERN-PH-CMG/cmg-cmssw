import FWCore.ParameterSet.Config as cms

from CMGTools.Common.analysis_cff import *
from CMGTools.Common.gen_cff import *

cmgMuon.cfg.inputCollection = 'patMuonsWithTrigger'
cmgElectron.cfg.inputCollection = 'patElectronsWithTrigger'

cmgPFJet.cfg.inputCollection = 'patJetsWithVar'
# cmgPFBaseJet.cfg.inputCollection = 'selectedPatJets'
cmgPUJetMva.jets = 'patJetsWithVar'

cmgTau.cfg.inputCollection = 'selectedPatTaus'

cmgPFMET.cfg.inputCollection = 'patMETs'
cmgPFMETRaw = cmgPFMET.clone()
cmgPFMETRaw.cfg.inputCollection = 'patMETsRaw'

CMGSequence = cms.Sequence(
    # genSequence +
    # triggerSequence +
    muonSequence + 
    electronSequence +
    # photonSequence +
    jetSequence +
    tauSequence +
    cmgPFMET +
    cmgPFMETRaw +
    miscSequence  
    )

import FWCore.ParameterSet.Config as cms


# HLT template module
HLT_template = cms.EDFilter(
    "HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","",""),
    HLTPaths = cms.vstring(''), # provide list of HLT paths (or patterns) you want
    eventSetupPathsKey = cms.string(''),  # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
    andOr = cms.bool(True),  # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True)  # throw exception on unknown path names
    )

# BSC MinBias trigger 
HLT_L1_BscMinBiasOR_BptxPlusORMinus = HLT_template.clone()
HLT_L1_BscMinBiasOR_BptxPlusORMinus.HLTPaths = cms.vstring('HLT_L1_BscMinBiasOR_BptxPlusORMinus')

# Jet triggers
HLT_Jet50U = HLT_template.clone()
HLT_Jet50U.HLTPaths = cms.vstring('HLT_Jet50U')

HLT_Jet30U = HLT_template.clone()
HLT_Jet30U.HLTPaths = cms.vstring('HLT_Jet30U')

# HT triggers

HLT_HT100U = HLT_template.clone()
HLT_HT100U.HLTPaths = cms.vstring('HLT_HT100U')

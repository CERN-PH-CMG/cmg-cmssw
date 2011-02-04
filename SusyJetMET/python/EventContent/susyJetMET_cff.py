import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

susyJetMET = everything
susyJetMET += particleFlowMHT
susyJetMET += traditionalMHT
susyJetMET.append( 'keep TriggerResults_*_*_*' )

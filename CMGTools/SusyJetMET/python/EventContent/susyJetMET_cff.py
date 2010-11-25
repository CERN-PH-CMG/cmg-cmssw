import FWCore.ParameterSet.Config as cms

from CMGTools.Common.EventContent.everything_cff import *

susyJetMET = everything
susyJetMET += particleFlowMHT
susyJetMET += traditionalMHT
susyJetMET.append( 'keep TriggerResults_*_*_*' )

#!/usr/bin/env python

import os,sys

scriptDir = os.getenv('CMSSW_BASE') + '/src/CMGTools/Common/scripts'
sys.path.append( scriptDir ) 
import castortools

sys.path.append( '.' ) 

from relvalDefinition import *

relvals = relvalList()


cmssw = 'CMSSW_4_2_3'

#/RelValQQH1352T_Tauola_cfi/CMSSW_4_1_4-START311_V2-v1/GEN-SIM-RECO
relvals.add(cmssw, 'RelValQQH1352T_Tauola_cfi', 'START42_V12')
relvals.add(cmssw, 'RelValQCD_FlatPt_15_3000', 'MC_42_V12')

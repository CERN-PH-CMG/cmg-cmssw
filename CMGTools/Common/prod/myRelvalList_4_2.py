#!/usr/bin/env python

import os,sys

scriptDir = os.getenv('CMSSW_BASE') + '/src/CMGTools/Common/scripts'
sys.path.append( scriptDir ) 
import castortools

sys.path.append( '.' ) 

from relvalDefinition import *

relvals = relvalList()

relvals.add('/RelValZMM/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO')
relvals.add('/RelValZEE/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO')
relvals.add('/RelValTTbar/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO')
relvals.add('/RelValQCD_FlatPt_15_3000/CMSSW_4_2_3-MC_42_V12-v2/GEN-SIM-RECO')
relvals.add('/RelValZTT/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO')
relvals.add('/RelValLM1_sfts/CMSSW_4_2_3-MC_42_V12-v2/GEN-SIM-RECO')
relvals.add('/RelValH130GGgluonfusion/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO')

#!/usr/bin/env python

import os,sys

scriptDir = os.getenv('CMSSW_BASE') + '/src/CMGTools/Common/scripts'
sys.path.append( scriptDir ) 
import castortools

sys.path.append( '.' ) 

from relvalDefinition import *

relvals = relvalList()

cmssw = 'CMSSW_4_1_2'
# relvals.add( cmssw, 'RelValLM1_sfts','MC_311_V2' )
# relvals.add( cmssw, 'RelValQCD_FlatPt_15_3000','MC_311_V2' )
# relvals.add( cmssw, 'RelValTTbar','MC_311_V2' )
# relvals.add( cmssw, 'RelValZEE','MC_311_V2' )
# relvals.add( cmssw, 'RelValZMM','START311_V2' )
# relvals.add(cmssw, 'RelValWM', 'START311_V2')
# relvals.add(cmssw, 'RelValWE', 'START311_V2')

cmssw = 'CMSSW_4_1_4'

#/RelValQQH1352T_Tauola_cfi/CMSSW_4_1_4-START311_V2-v1/GEN-SIM-RECO
relvals.add(cmssw, 'RelValQQH1352T_Tauola_cfi', 'START311_V2')

#/RelValZTT/CMSSW_4_1_4-START311_V2-v1/GEN-SIM-RECO
relvals.add(cmssw, 'RelValZTT', 'START311_V2')

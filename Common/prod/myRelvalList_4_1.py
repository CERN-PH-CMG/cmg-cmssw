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
relvals.add( cmssw, 'RelValQCD_FlatPt_15_3000','MC_311_V2' )
# relvals.add( cmssw, 'RelValTTbar','MC_311_V2' )
# relvals.add( cmssw, 'RelValZEE','MC_311_V2' )
# relvals.add( cmssw, 'RelValZMM','START311_V2' )


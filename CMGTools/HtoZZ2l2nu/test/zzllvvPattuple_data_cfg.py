import os,sys
runOnMC=False
runFull=False
runStd=True
cfgFile=os.path.expandvars('${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_cfg.py')
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureSourceFromCommandLine
castorDir, outFile, inputList = configureSourceFromCommandLine()
execfile(cfgFile)

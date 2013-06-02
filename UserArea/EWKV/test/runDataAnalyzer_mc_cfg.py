import os,sys

isMC=True
isTauEmbed=False
gtag="START53_V20::All"

from UserCode.EWKV.storeTools_cff import configureSourceFromCommandLine
outFile, inputList = configureSourceFromCommandLine()
tfsOutputFile=outFile
outFile=os.path.dirname(outFile)+'/edm_'+os.path.basename(outFile)

execfile( os.path.expandvars('${CMSSW_BASE}/src/UserCode/EWKV/test/runDataAnalyzer_cfg.py'))


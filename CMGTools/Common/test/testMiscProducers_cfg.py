'''Run this cfg, and check the branch *_deltaPhiJetMET_*_*'''

import os, string

os.environ['PYTHONPATH'] = os.environ['CMSSW_BASE'] + 'src/CMGTools/Common/test' + ':'+os.environ['PYTHONPATH']

from testCMGTools_cfg import *

process.load('CMGTools.Common.miscProducers.misc_cff')
process.p += process.miscSequence

process.TFileService.fileName = 'histograms_testMisc.root'
process.out.fileName = 'tree_testMisc.root'




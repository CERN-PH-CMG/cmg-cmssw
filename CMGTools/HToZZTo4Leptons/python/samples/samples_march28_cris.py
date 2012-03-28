from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc_higgs import *

pat = 'PAT_CMG_Test_444'
filePattern = 'tree.*root'

Hig120GluGlu.files = getFiles('/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4/{pat}'.format(pat=pat), 'botta', filePattern)


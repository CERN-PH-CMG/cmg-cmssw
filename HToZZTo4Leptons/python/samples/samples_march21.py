from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc_higgs import *

pat = 'PAT_CMG_V3_1_0'
filePattern = 'tree.*root'

Hig120GluGlu.files = getFiles('/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/{pat}'.format(pat=pat), 'cbern', filePattern)


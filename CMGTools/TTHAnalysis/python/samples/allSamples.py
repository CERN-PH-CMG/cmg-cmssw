import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.Production.getFiles import getFiles

file_owner = 'cmgtools'
filePattern = 'cmgTuple.*root'
cache = True

TTH = cfg.MCComponent(
    name = 'TTH',
    path = '/TTH_Inclusive_M-125_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_V5_10_0',
    files = [],
    xSection = 4.2, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )


allSamples = [
    TTH
    ]

for sam in allSamples:
    sam.files = getFiles(sam.path, file_owner, filePattern, cache)
    sam.splitFactor = int( sam.nGenEvents / 30000 )
    

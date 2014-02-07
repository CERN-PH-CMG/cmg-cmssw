from CMGTools.H2TauTau.proto.samples.run2012.data_2012 import *
from CMGTools.H2TauTau.proto.samples.run2012.mc_basicEWK import *

# from CMGTools.H2TauTau.proto.samples.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

aod = 'V4'
pat = 'PAT_CMG_V4_0_1_Test2012'
filePattern = 'tree_CMG.*root'

# Data --------------------------------------------------------------------------------

data_Run2012A_PromptReco_v1.files = getFiles('/DoubleElectron/Run2012A-PromptReco-v1/AOD/{pat}'.format(pat=pat), 'cmgtools', filePattern)


# MC basic EWK ----------------------------------------------------------------------------

# 2165676 ||      2.0 ||  /DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM/V4/PAT_CMG_V4_0_1_Test2012

DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM/V4/{pat}'.format(pat=pat), 'cmgtools', filePattern)
DYJets.nGenEvents = 2165676 / 2.0


if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print
            print sam

    print 'DATA 2011:'
    print 
    printSamples(data_2012)

    print 'MC FALL11:'
    print
    printSamples(mc_basicEWK)

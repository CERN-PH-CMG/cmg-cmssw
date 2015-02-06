import copy
from CMGTools.H2TauTau.proto.samples.mc_fall11 import *
from CMGTools.H2TauTau.proto.samples.mc_qcdmu import *
from CMGTools.H2TauTau.proto.samples.mc_higgs import *

aod = 'V3'
pat = 'PAT_CMG_TestMVAs'
filePattern = 'tree.*root'

# MC Fall11 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}'.format(pat=pat), 'cmgtools', filePattern)
DYJets.nGenEvents = 36209629

# DYJetsChamonix=copy.deepcopy(DYJets)
# DYJetsChamonix.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0', 'cmgtools', filePattern)

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/TAUTAU/{pat}'.format(pat=pat), 'cmgtools', filePattern)
WJets.nGenEvents = 81345381

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}'.format(pat=pat), 'cmgtools', filePattern)
TTJets.nGenEvents = 3701947

QCDMuH20Pt15.files = getFiles('/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/{pat}'.format(pat=pat), 'cmgtools', filePattern)

QCDMuH15to20Pt5.files = getFiles('/QCD_Pt-15to20_MuPt5Enriched_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM/V3/{pat}'.format(pat=pat), 'cmgtools', filePattern)

Hig105.files =  getFiles('/GluGluToHToTauTau_M-105_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/{pat}'.format(pat=pat), 'cmgtools', filePattern)

if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print
            print sam

    print 'cmg-tuples, TestMVAs, MC FALL11:'
    print
    printSamples(mc_fall11)

from CMGTools.H2TauTau.proto.samples.run2012.data import *

# FIXME need specific sample definition for 2012 (with the correct x-sections)
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

user = 'cbern'
aod = 'V5'
pat = 'PAT_CMG_V5_4_0'
htt = 'TAUMU'
filePattern = 'tauMu.*fullsel.*root'


# Data --------------------------------------------------------------------------------

data_Run2012A.files = getFiles('/TauPlusX/Run2012A-PromptReco-v1/RECO/{pat}_runrange_190605-194076/{htt}/Group'.format( pat=pat, htt=htt), user, filePattern)
data_Run2012A.intLumi = 713.356 # recorded, obtained with lumiCalc2.py

data_Run2012B_start_194479.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/RECO/{pat}_runrange_start-194479/{htt}/Group'.format( pat=pat, htt=htt), user, filePattern)
data_Run2012B_start_194479.intLumi = 890.592

data_Run2012B_194480_195016.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/AOD/{pat}_runrange_194480-195016/{htt}/Group'.format(pat=pat, htt=htt), user, filePattern)
data_Run2012B_194480_195016.intLumi = 767.154



# Embedded samples --------------------------------------------------------------------


# MC Fall11 ----------------------------------------------------------------------------


## DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
## DYJets.nGenEvents = 1082838

# FIXME: now the dataset is complete! remove (1-5/644.)
DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
DYJets.nGenEvents = 30461028 * (1-5/644.) * 1.97797155762/2.



WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
WJets.nGenEvents = 18393090

TTJets.files = getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
TTJets.nGenEvents = 6736135

# Higgs ----------------------------------------------------------------------------

HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
HiggsVBF125.nGenEvents = 198612



MC = copy.copy( mc_ewk )
MC.extend( mc_higgs ) 

for sam in MC:
    sam.triggers = mc_triggers 
    
for sam in data_list_2012:
    sam.triggers = data_triggers 



if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print
            print sam

    print 'MC:'
    print
    printSamples(MC)
    print 
    print 'MC:'
    print
    printSamples(data_2012)

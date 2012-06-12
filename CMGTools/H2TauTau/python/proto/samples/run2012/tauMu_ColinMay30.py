from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.zdata import *

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
# data_Run2012A.intLumi = 713.356 # recorded, obtained with lumiCalc2.py
data_Run2012A.intLumi = 697.599 # recorded, obtained with lumiCalc2.py

data_Run2012B_start_194479.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/RECO/{pat}_runrange_start-194479/{htt}/Group'.format( pat=pat, htt=htt), user, filePattern)
data_Run2012B_start_194479.intLumi = 890.592

data_Run2012B_194480_195016.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/AOD/{pat}_runrange_194480-195016/{htt}/Group'.format(pat=pat, htt=htt), user, filePattern)
data_Run2012B_194480_195016.intLumi = 767.154


# Z Data --------------------------------------------------------------------------------

zdata_Run2012A.files = getFiles('/DoubleMu/Run2012A-PromptReco-v1/RECO/{pat}_runrange_190605-194076'.format( pat=pat, htt=htt), 'cmgtools', 'cmgTuple.*root')
zdata_Run2012A.intLumi = 713.356 # recorded, obtained with lumiCalc2.py

zdata_Run2012B_start_194479.files = getFiles('/DoubleMu/Run2012B-PromptReco-v1/RECO/{pat}_runrange_start-194479'.format( pat=pat, htt=htt),'cmgtools' , 'cmgTuple.*root')
zdata_Run2012B_start_194479.intLumi = 890.592

zdata_Run2012B_194480_195016.files = getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/{pat}_runrange_194480-195016'.format(pat=pat, htt=htt), 'cmgtools', 'cmgTuple.*root')
zdata_Run2012B_194480_195016.intLumi = 767.154



# Embedded samples --------------------------------------------------------------------

embed_Run2012A.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format( pat=pat, htt=htt), user, filePattern)

embed_Run2012B_193752_195135.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format( pat=pat, htt=htt), user, filePattern)

# MC Fall11 ----------------------------------------------------------------------------


## DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
## DYJets.nGenEvents = 1082838

DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
# DYJets.nGenEvents = 30461028 * (1-5/644.) * 1.97797155762/2.
DYJets.nGenEvents = 30461028 * 1.97797155762/2.



WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
WJets.nGenEvents = 18393090

TTJets.files = getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern )
TTJets.nGenEvents = 6736135

# Higgs ----------------------------------------------------------------------------

vbfpath = '/VBF_HToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt)

HiggsVBF115.files = getFiles(vbfpath.replace('HMASS','115'), user, filePattern )
HiggsVBF120.files = getFiles(vbfpath.replace('HMASS','120'), user, filePattern )
HiggsVBF125.files = getFiles(vbfpath.replace('HMASS','125'), user, filePattern )

HiggsVBF115.nGenEvents = 998870
HiggsVBF120.nGenEvents = 4798759
HiggsVBF125.nGenEvents = 998836


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
    printSamples(data_list_2012)

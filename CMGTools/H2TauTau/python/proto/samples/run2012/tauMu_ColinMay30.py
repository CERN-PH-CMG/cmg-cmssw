from CMGTools.H2TauTau.proto.samples.getFiles import getFiles
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.zdata import *
# from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

user = 'cbern'
aod = 'V5'
pat = 'PAT_CMG_V5_4_0'
htt = 'TAUMU_Type1MET'
filePattern = 'tauMu.*fullsel.*root'

cache = True

# Data --------------------------------------------------------------------------------

data_Run2012A.files = getFiles('/TauPlusX/Run2012A-PromptReco-v1/RECO/{pat}_runrange_190605-194076/{htt}'.format( pat=pat, htt=htt), user, filePattern, cache)

data_Run2012B_start_194479.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/RECO/{pat}_runrange_start-194479/{htt}/Group'.format( pat=pat, htt=htt), user, filePattern, cache)

data_Run2012B_194480_195016.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/AOD/{pat}_runrange_194480-195016/{htt}/Group'.format(pat=pat, htt=htt), user, filePattern, cache)

data_Run2012B_195017_195947.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/AOD/{pat}_runrange_195017-195947/{htt}'.format(pat=pat, htt=htt), user, filePattern, cache)

data_Run2012B_195948_196509.files = getFiles('/TauPlusX/Run2012B-PromptReco-v1/AOD/{pat}_runrange_195948-196509/{htt}'.format(pat=pat, htt=htt), 'cmgtools', filePattern, cache)


# Z Data --------------------------------------------------------------------------------


# zdata_Run2012A.files = getFiles('/DoubleMu/Run2012A-PromptReco-v1/RECO/{pat}_runrange_190605-194076'.format( pat=pat, htt=htt), 'cmgtools', 'cmgTuple.*root')
zdata_Run2012A.intLumi = 713.356 # recorded, obtained with lumiCalc2.py

# zdata_Run2012B_start_194479.files = getFiles('/DoubleMu/Run2012B-PromptReco-v1/RECO/{pat}_runrange_start-194479'.format( pat=pat, htt=htt),'cmgtools' , 'cmgTuple.*root')
zdata_Run2012B_start_194479.intLumi = 890.592

# zdata_Run2012B_194480_195016.files = getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/{pat}_runrange_194480-195016'.format(pat=pat, htt=htt), 'cmgtools', 'cmgTuple.*root')
zdata_Run2012B_194480_195016.intLumi = 767.154


# Embedded samples --------------------------------------------------------------------

embed_Run2012A.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format( pat=pat, htt=htt), user, filePattern, cache)
embed_Run2012A_Up.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}_Up'.format( pat=pat, htt=htt), user, filePattern, cache)
embed_Run2012A_Down.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}_Down'.format( pat=pat, htt=htt), user, filePattern, cache)

embed_Run2012B_193752_195135.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format( pat=pat, htt=htt), user, filePattern, cache)
embed_Run2012B_193752_195135_Up.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}_Up'.format( pat=pat, htt=htt), user, filePattern, cache)
embed_Run2012B_193752_195135_Down.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}_Down'.format( pat=pat, htt=htt), user, filePattern, cache)

embed_Run2012B_195147_196070.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format( pat=pat, htt=htt), user, filePattern, cache)
embed_Run2012B_195147_196070_Up.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}_Up'.format( pat=pat, htt=htt), user, filePattern, cache)
embed_Run2012B_195147_196070_Down.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}_Down'.format( pat=pat, htt=htt), user, filePattern, cache)


# MC  ----------------------------------------------------------------------------


DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
DYJets.nGenEvents = 30461028 * 1.97797155762/2.

DYJets_Up.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}_Up'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
DYJets_Up.nGenEvents = DYJets.nGenEvents

DYJets_Down.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}_Down'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
DYJets_Down.nGenEvents = DYJets.nGenEvents

WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
WJets.nGenEvents = 18393090 * 1.99379/2.


TTJets.files = getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}/Group'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache )
TTJets.nGenEvents = 6736135


TTJets11 = copy.deepcopy(TTJets)
TTJets11.name = 'TTJets11'
TTJets11.files = []
TTJets11.nGenEvents = 59613991


WJets11 = copy.deepcopy(WJets)
WJets11.name = 'WJets11'
WJets11.files = []
WJets11.nGenEvents = 90e6


# below, 2011 sample. should replace it by 2012 sample
W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = [],
    xSection = 1000., # just a guess
    nGenEvents = 7541595. * 1.997 / 2.,
    triggers = [],
    effCorrFactor = 1 )



# Higgs ----------------------------------------------------------------------------

vbfpath = '/VBF_HToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt)

HiggsVBF120.files = getFiles(vbfpath.replace('HMASS','120'), user, filePattern, cache )
HiggsVBF125.files = getFiles(vbfpath.replace('HMASS','125'), user, filePattern, cache )
HiggsVBF130.files = getFiles(vbfpath.replace('HMASS','130'), user, filePattern, cache )
HiggsVBF135.files = getFiles(vbfpath.replace('HMASS','135'), user, filePattern, cache )
HiggsVBF140.files = getFiles(vbfpath.replace('HMASS','140'), user, filePattern, cache )
HiggsVBF145.files = getFiles(vbfpath.replace('HMASS','145'), user, filePattern, cache )
HiggsVBF150.files = getFiles(vbfpath.replace('HMASS','150'), user, filePattern, cache )

HiggsVBF120.nGenEvents = 4798759 * 1.995 /2.
HiggsVBF125.nGenEvents = 998836
HiggsVBF130.nGenEvents = 998790 * 1.97780214059 /2.
HiggsVBF135.nGenEvents = 998817 * 1.98019657255 /2.
HiggsVBF140.nGenEvents = 998894
HiggsVBF145.nGenEvents = 998883 * 1.9946 /2.
HiggsVBF150.nGenEvents = 1519804 * 1.98777210746 / 2.


gghpath = '/GluGluToHToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt)


HiggsGGH120.files = getFiles(gghpath.replace('HMASS','120'), user, filePattern, cache )
HiggsGGH125.files = getFiles(gghpath.replace('HMASS','125'), user, filePattern, cache )
HiggsGGH130.files = getFiles(gghpath.replace('HMASS','130'), user, filePattern, cache )
HiggsGGH135.files = getFiles(gghpath.replace('HMASS','135'), user, filePattern, cache )
HiggsGGH140.files = getFiles(gghpath.replace('HMASS','140'), user, filePattern, cache )
HiggsGGH145.files = getFiles(gghpath.replace('HMASS','145'), user, filePattern, cache )
HiggsGGH150.files = getFiles(gghpath.replace('HMASS','150'), user, filePattern, cache )

HiggsGGH120.nGenEvents = 4999561 * 1.992 / 2.
HiggsGGH125.nGenEvents = 968134 
HiggsGGH130.nGenEvents = 999921
HiggsGGH135.nGenEvents = 860729
HiggsGGH140.nGenEvents = 998894
HiggsGGH145.nGenEvents = 998883 * 1.995 / 2.
HiggsGGH150.nGenEvents = 1519804 * 1.9877 / 2.

vhpath = '/WH_ZH_TTH_HToTauTau_M-HMASS_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt)


HiggsVH120.files = getFiles(vhpath.replace('HMASS','120'), user, filePattern, cache )
HiggsVH125.files = getFiles(vhpath.replace('HMASS','125'), user, filePattern, cache )
HiggsVH130.files = getFiles(vhpath.replace('HMASS','130'), user, filePattern, cache )
HiggsVH135.files = getFiles(vhpath.replace('HMASS','135'), user, filePattern, cache )
HiggsVH140.files = getFiles(vhpath.replace('HMASS','140'), user, filePattern, cache )
HiggsVH145.files = getFiles(vhpath.replace('HMASS','145'), user, filePattern, cache )
HiggsVH150.files = getFiles(vhpath.replace('HMASS','150'), user, filePattern, cache )

HiggsVH120.nGenEvents = 200448
HiggsVH125.nGenEvents = 200124
HiggsVH130.nGenEvents = 200080
HiggsVH135.nGenEvents = 200128 * 1.84434 / 2.
HiggsVH140.nGenEvents = 200186 * 1.972376 / 2.
HiggsVH145.nGenEvents = 200384 * 1.87128 / 2.
HiggsVH150.nGenEvents = 200485


################

vbfpath = '/VBF_HToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}_Down'.format(aod=aod, pat=pat, htt=htt)

HiggsVBF120_Down.files = getFiles(vbfpath.replace('HMASS','120'), user, filePattern, cache )
HiggsVBF125_Down.files = getFiles(vbfpath.replace('HMASS','125'), user, filePattern, cache )
HiggsVBF130_Down.files = getFiles(vbfpath.replace('HMASS','130'), user, filePattern, cache )
HiggsVBF135_Down.files = getFiles(vbfpath.replace('HMASS','135'), user, filePattern, cache )
HiggsVBF140_Down.files = getFiles(vbfpath.replace('HMASS','140'), user, filePattern, cache )
HiggsVBF145_Down.files = getFiles(vbfpath.replace('HMASS','145'), user, filePattern, cache )
HiggsVBF150_Down.files = getFiles(vbfpath.replace('HMASS','150'), user, filePattern, cache )

HiggsVBF120_Down.nGenEvents = HiggsVBF120.nGenEvents
HiggsVBF125_Down.nGenEvents = HiggsVBF125.nGenEvents
HiggsVBF130_Down.nGenEvents = HiggsVBF130.nGenEvents
HiggsVBF135_Down.nGenEvents = HiggsVBF135.nGenEvents
HiggsVBF140_Down.nGenEvents = HiggsVBF140.nGenEvents
HiggsVBF145_Down.nGenEvents = HiggsVBF145.nGenEvents
HiggsVBF150_Down.nGenEvents = HiggsVBF150.nGenEvents


gghpath = '/GluGluToHToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}_Down'.format(aod=aod, pat=pat, htt=htt)


HiggsGGH120_Down.files = getFiles(gghpath.replace('HMASS','120'), user, filePattern, cache )
HiggsGGH125_Down.files = getFiles(gghpath.replace('HMASS','125'), user, filePattern, cache )
HiggsGGH130_Down.files = getFiles(gghpath.replace('HMASS','130'), user, filePattern, cache )
HiggsGGH135_Down.files = getFiles(gghpath.replace('HMASS','135'), user, filePattern, cache )
HiggsGGH140_Down.files = getFiles(gghpath.replace('HMASS','140'), user, filePattern, cache )
HiggsGGH145_Down.files = getFiles(gghpath.replace('HMASS','145'), user, filePattern, cache )
HiggsGGH150_Down.files = getFiles(gghpath.replace('HMASS','150'), user, filePattern, cache )

HiggsGGH120_Down.nGenEvents = HiggsGGH120.nGenEvents
HiggsGGH125_Down.nGenEvents = HiggsGGH125.nGenEvents
HiggsGGH130_Down.nGenEvents = HiggsGGH130.nGenEvents
HiggsGGH135_Down.nGenEvents = HiggsGGH135.nGenEvents
HiggsGGH140_Down.nGenEvents = HiggsGGH140.nGenEvents
HiggsGGH145_Down.nGenEvents = HiggsGGH145.nGenEvents
HiggsGGH150_Down.nGenEvents = HiggsGGH150.nGenEvents

vhpath = '/WH_ZH_TTH_HToTauTau_M-HMASS_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}_Down'.format(aod=aod, pat=pat, htt=htt)

HiggsVH120_Down.files = getFiles(vhpath.replace('HMASS','120'), user, filePattern, cache )
HiggsVH125_Down.files = getFiles(vhpath.replace('HMASS','125'), user, filePattern, cache )
HiggsVH130_Down.files = getFiles(vhpath.replace('HMASS','130'), user, filePattern, cache )
HiggsVH135_Down.files = getFiles(vhpath.replace('HMASS','135'), user, filePattern, cache )
HiggsVH140_Down.files = getFiles(vhpath.replace('HMASS','140'), user, filePattern, cache )
HiggsVH145_Down.files = getFiles(vhpath.replace('HMASS','145'), user, filePattern, cache )
HiggsVH150_Down.files = getFiles(vhpath.replace('HMASS','150'), user, filePattern, cache )

HiggsVH120_Down.nGenEvents = HiggsVH120.nGenEvents
HiggsVH125_Down.nGenEvents = HiggsVH125.nGenEvents
HiggsVH130_Down.nGenEvents = HiggsVH130.nGenEvents
HiggsVH135_Down.nGenEvents = HiggsVH135.nGenEvents
HiggsVH140_Down.nGenEvents = HiggsVH140.nGenEvents
HiggsVH145_Down.nGenEvents = HiggsVH145.nGenEvents
HiggsVH150_Down.nGenEvents = HiggsVH150.nGenEvents


################

vbfpath = '/VBF_HToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}_Up'.format(aod=aod, pat=pat, htt=htt)

HiggsVBF120_Up.files = getFiles(vbfpath.replace('HMASS','120'), user, filePattern, cache )
HiggsVBF125_Up.files = getFiles(vbfpath.replace('HMASS','125'), user, filePattern, cache )
HiggsVBF130_Up.files = getFiles(vbfpath.replace('HMASS','130'), user, filePattern, cache )
HiggsVBF135_Up.files = getFiles(vbfpath.replace('HMASS','135'), user, filePattern, cache )
HiggsVBF140_Up.files = getFiles(vbfpath.replace('HMASS','140'), user, filePattern, cache )
HiggsVBF145_Up.files = getFiles(vbfpath.replace('HMASS','145'), user, filePattern, cache )
HiggsVBF150_Up.files = getFiles(vbfpath.replace('HMASS','150'), user, filePattern, cache )

HiggsVBF120_Up.nGenEvents = HiggsVBF120.nGenEvents
HiggsVBF125_Up.nGenEvents = HiggsVBF125.nGenEvents
HiggsVBF130_Up.nGenEvents = HiggsVBF130.nGenEvents
HiggsVBF135_Up.nGenEvents = HiggsVBF135.nGenEvents
HiggsVBF140_Up.nGenEvents = HiggsVBF140.nGenEvents
HiggsVBF145_Up.nGenEvents = HiggsVBF145.nGenEvents
HiggsVBF150_Up.nGenEvents = HiggsVBF150.nGenEvents


gghpath = '/GluGluToHToTauTau_M-HMASS_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}_Up'.format(aod=aod, pat=pat, htt=htt)


HiggsGGH120_Up.files = getFiles(gghpath.replace('HMASS','120'), user, filePattern, cache )
HiggsGGH125_Up.files = getFiles(gghpath.replace('HMASS','125'), user, filePattern, cache )
HiggsGGH130_Up.files = getFiles(gghpath.replace('HMASS','130'), user, filePattern, cache )
HiggsGGH135_Up.files = getFiles(gghpath.replace('HMASS','135'), user, filePattern, cache )
HiggsGGH140_Up.files = getFiles(gghpath.replace('HMASS','140'), user, filePattern, cache )
HiggsGGH145_Up.files = getFiles(gghpath.replace('HMASS','145'), user, filePattern, cache )
HiggsGGH150_Up.files = getFiles(gghpath.replace('HMASS','150'), user, filePattern, cache )

HiggsGGH120_Up.nGenEvents = HiggsGGH120.nGenEvents
HiggsGGH125_Up.nGenEvents = HiggsGGH125.nGenEvents
HiggsGGH130_Up.nGenEvents = HiggsGGH130.nGenEvents
HiggsGGH135_Up.nGenEvents = HiggsGGH135.nGenEvents
HiggsGGH140_Up.nGenEvents = HiggsGGH140.nGenEvents
HiggsGGH145_Up.nGenEvents = HiggsGGH145.nGenEvents
HiggsGGH150_Up.nGenEvents = HiggsGGH150.nGenEvents

vhpath = '/WH_ZH_TTH_HToTauTau_M-HMASS_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}_Up'.format(aod=aod, pat=pat, htt=htt)

HiggsVH120_Up.files = getFiles(vhpath.replace('HMASS','120'), user, filePattern, cache )
HiggsVH125_Up.files = getFiles(vhpath.replace('HMASS','125'), user, filePattern, cache )
HiggsVH130_Up.files = getFiles(vhpath.replace('HMASS','130'), user, filePattern, cache )
HiggsVH135_Up.files = getFiles(vhpath.replace('HMASS','135'), user, filePattern, cache )
HiggsVH140_Up.files = getFiles(vhpath.replace('HMASS','140'), user, filePattern, cache )
HiggsVH145_Up.files = getFiles(vhpath.replace('HMASS','145'), user, filePattern, cache )
HiggsVH150_Up.files = getFiles(vhpath.replace('HMASS','150'), user, filePattern, cache )

HiggsVH120_Up.nGenEvents = HiggsVH120.nGenEvents
HiggsVH125_Up.nGenEvents = HiggsVH125.nGenEvents
HiggsVH130_Up.nGenEvents = HiggsVH130.nGenEvents
HiggsVH135_Up.nGenEvents = HiggsVH135.nGenEvents
HiggsVH140_Up.nGenEvents = HiggsVH140.nGenEvents
HiggsVH145_Up.nGenEvents = HiggsVH145.nGenEvents
HiggsVH150_Up.nGenEvents = HiggsVH150.nGenEvents

# Di-boson

WW.files = getFiles('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0/TAUMU_Type1MET', 'cmgtools', filePattern, cache)
WW.nGenEvents = 10000431

WZ.files = getFiles('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0/TAUMU_Type1MET', 'cmgtools', filePattern, cache)
WZ.nGenEvents = 9996622 * 1.98849011196/2.

ZZ.files = getFiles('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START50_V15-v1/AODSIM/V5/PAT_CMG_V5_4_0/TAUMU_Type1MET', 'cmgtools', filePattern, cache)
ZZ.nGenEvents = 9799908 * 1.9088789405/2.




MC = copy.copy( mc_ewk )
MC.extend( mc_higgs ) 
MC.extend( mc_diboson ) 

MC_up = copy.copy(mc_ewk_up)
MC_up.extend( mc_higgs_up ) 

MC_down = copy.copy(mc_ewk_down)
MC_down.extend( mc_higgs_down ) 


for sam in MC:
    sam.triggers = mc_triggers 
    
for sam in data_list_2012:
    sam.triggers = data_triggers 


if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print sam.name

    print 
    print 'MC:'
    print
    printSamples(MC)
    print 
    print 'MC Up:'
    print
    printSamples(MC_up)
    print 
    print 'MC Down:'
    print
    printSamples(MC_down)
    print 
    print 'Data:'
    print
    printSamples(data_list_2012)
    print 
    print 'Embed:'
    print
    printSamples(embed_list_2012)
    print 
    print 'Embed Up:'
    print
    printSamples(embed_list_2012_up)
    print 
    print 'Embed Down:'
    print
    printSamples(embed_list_2012_down)

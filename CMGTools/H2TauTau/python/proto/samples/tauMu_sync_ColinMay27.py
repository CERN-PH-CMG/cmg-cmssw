from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles
from CMGTools.H2TauTau.proto.samples.higgs import *
from CMGTools.H2TauTau.proto.samples.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

user = 'cbern'
filePattern = 'tauMu.*fullsel.*root'


# Higgs Summer11 ----------------------------------------------------------------------------

# not available yet

HiggsVBF125.files = getFiles('/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_PhilV1/TAUMU_May27', user, filePattern)
HiggsVBF125.nGenEvents = 198612

HiggsGGH125.files = getFiles('/H2TAUTAU/Sync/GluGlu/AOD/PAT_CMG_PhilV1/TAUMU_May27', user, filePattern)
HiggsGGH125.nGenEvents = 198612



MC = []
for mc in mc_higgs:
    mc.triggers = ["HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1"]
    MC.append(mc)
    
if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print
            print sam

    print 'HIGGS:'
    print
    printSamples(mc_higgs)

import FWCore.ParameterSet.Config as cms

###
### trigger paths of interest
### Trigger evolution in 2011 cf. http://fwyzard.web.cern.ch/fwyzard/hlt/summary
###
def getTriggerPaths(version=2012) :

    #################
    # 2012 triggers #
    #################
    if(version==2012) :
        mcTrigs = []
        
        DoubleElectron = ['HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
                          'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v']
        
        DoubleMu = ['HLT_Mu17_Mu8_v']

        
        MuEG = ['HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
                'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v']
        
        SingleMu = ['HLT_IsoMu24_eta2p1_v']
        
        Photon = ['HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_v',
                  'HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_v',                  
                  'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_v',
                  'HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_v',
                  'HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_v',
                  'HLT_Photon135_v',
                  'HLT_Photon150_v',
                  'HLT_Photon160_v',
                  'HLT_Photon250_NoHE_v1_v',
                  'HLT_Photon300_NoHE_v1_v']

    #################
    # 2011 triggers #
    #################
    else:

        mcTrigs = ['HLT_IsoMu17_v',
                   'HLT_DoubleMu7_v',
                   'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v',
                   'HLT_Mu8_Ele17_CaloIdL_v',
                   'HLT_Mu10_Ele10_CaloIdL_v',
                   'HLT_Mu17_Ele8_CaloIdL_v']

        DoubleElectron = ['HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v',                            
                     'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v']

        DoubleMu = ['HLT_DoubleMu7_v',
                    'HLT_Mu13_Mu8_v',
                    'HLT_Mu17_Mu8_v',
                    'HLT_Mu17_TkMu8_v']

        MuEG = ['HLT_Mu17_Ele8_CaloIdL_v',
                'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v',
                'HLT_Mu8_Ele17_CaloIdL_v',
                'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v']

        SingleMu = ['HLT_IsoMu17_v',
                    'HLT_IsoMu24_v',
                    'HLT_IsoMu30_eta2p1_v',
                    'HLT_IsoMu34_eta2p1_v']

        Photon=['HLT_Photon20_CaloIdVL_IsoL_v',
                'HLT_Photon30_CaloIdVL_IsoL_v',
                'HLT_Photon50_CaloIdVL_IsoL_v',
                'HLT_Photon75_CaloIdVL_IsoL_v',
                'HLT_Photon90_CaloIdVL_IsoL_v',
                'HLT_Photon125_v',
                'HLT_Photon125_NoSpikeFilter_v',
                'HLT_Photon135_v',
                'HLT_Photon200_NoHE_v']
    
    return DoubleElectron, DoubleMu, MuEG, Photon, SingleMu, mcTrigs


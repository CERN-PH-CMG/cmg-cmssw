import FWCore.ParameterSet.Config as cms

###
### trigger paths of interest
###
def addGammaTriggerSequence(process) :
   
    gammaTrigs=['HLT_Photon20_CaloIdVL_IsoL_v1','HLT_Photon20_CaloIdVL_IsoL_v2','HLT_Photon20_CaloIdVL_IsoL_v3','HLT_Photon20_CaloIdVL_IsoL_v4','HLT_Photon20_CaloIdVL_IsoL_v5','HLT_Photon20_CaloIdVL_IsoL_v6','HLT_Photon20_CaloIdVL_IsoL_v7','HLT_Photon20_CaloIdVL_IsoL_v8','HLT_Photon20_CaloIdVL_IsoL_v9',
                'HLT_Photon30_CaloIdVL_IsoL_v1','HLT_Photon30_CaloIdVL_IsoL_v2','HLT_Photon30_CaloIdVL_IsoL_v3','HLT_Photon30_CaloIdVL_IsoL_v4','HLT_Photon30_CaloIdVL_IsoL_v5','HLT_Photon30_CaloIdVL_IsoL_v6','HLT_Photon30_CaloIdVL_IsoL_v7','HLT_Photon30_CaloIdVL_IsoL_v8','HLT_Photon30_CaloIdVL_IsoL_v9','HLT_Photon30_CaloIdVL_IsoL_v10',
                'HLT_Photon50_CaloIdVL_IsoL_v1','HLT_Photon50_CaloIdVL_IsoL_v2','HLT_Photon50_CaloIdVL_IsoL_v3','HLT_Photon50_CaloIdVL_IsoL_v4','HLT_Photon50_CaloIdVL_IsoL_v5','HLT_Photon50_CaloIdVL_IsoL_v6','HLT_Photon50_CaloIdVL_IsoL_v7','HLT_Photon50_CaloIdVL_IsoL_v8','HLT_Photon50_CaloIdVL_IsoL_v9',
                'HLT_Photon75_CaloIdVL_IsoL_v1','HLT_Photon75_CaloIdVL_IsoL_v2','HLT_Photon75_CaloIdVL_IsoL_v3','HLT_Photon75_CaloIdVL_IsoL_v4','HLT_Photon75_CaloIdVL_IsoL_v5','HLT_Photon75_CaloIdVL_IsoL_v6','HLT_Photon75_CaloIdVL_IsoL_v7','HLT_Photon75_CaloIdVL_IsoL_v8','HLT_Photon75_CaloIdVL_IsoL_v9',
                'HLT_Photon90_CaloIdVL_IsoL_v1','HLT_Photon90_CaloIdVL_IsoL_v2','HLT_Photon90_CaloIdVL_IsoL_v3','HLT_Photon90_CaloIdVL_IsoL_v4','HLT_Photon90_CaloIdVL_IsoL_v5','HLT_Photon90_CaloIdVL_IsoL_v6','HLT_Photon90_CaloIdVL_IsoL_v7','HLT_Photon90_CaloIdVL_IsoL_v8','HLT_Photon90_CaloIdVL_IsoL_v9',
                'HLT_Photon125_v1','HLT_Photon125_v2',
                'HLT_Photon125_NoSpikeFilter_v1','HLT_Photon125_NoSpikeFilter_v2','HLT_Photon125_NoSpikeFilter_v3',
                'HLT_Photon135_v1','HLT_Photon135_v2','HLT_Photon135_v3',
                'HLT_Photon200_NoHE_v1','HLT_Photon200_NoHE_v2','HLT_Photon200_NoHE_v3','HLT_Photon200_NoHE_v4'  
                ]
    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
    process.gammaTrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.gammaTrigFilter.throw = cms.bool(False)
    process.gammaTrigFilter.HLTPaths = gammaTrigs
    process.preGammaTriggerCounter = cms.EDProducer("EventCountProducer")
    process.gammaTriggerCounter = process.preGammaTriggerCounter.clone()
    process.trigSequence = cms.Sequence(process.preGammaTriggerCounter*process.gammaTrigFilter*process.gammaTriggerCounter)


def getTriggerPaths() :
    mcTrigs = ['HLT_IsoMu17_v5',
               #               'HLT_DoubleMu3_v3',
               #               'HLT_DoubleMu6_v1',
               'HLT_DoubleMu7_v1',
               'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2',
               #               'HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v2',
               'HLT_Mu8_Ele17_CaloIdL_v2',
               'HLT_Mu10_Ele10_CaloIdL_v3',
               'HLT_Mu17_Ele8_CaloIdL_v2']

    # Trigger evolution cf. http://fwyzard.web.cern.ch/fwyzard/hlt/summary
    doubleEle = ['HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1',                                   #start run 160404
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v7',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v8',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v1',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v2',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v3',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v4',  
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v5', #start run 167039
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v6',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v7',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v8',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v9',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v10'
                 ]
    doubleMu = ['HLT_DoubleMu7_v1',              #start run 160404
                'HLT_DoubleMu7_v2',       
                'HLT_Mu13_Mu8_v1',               #start run 165088 
                'HLT_Mu13_Mu8_v2',
                'HLT_Mu13_Mu8_v3',  
                'HLT_Mu13_Mu8_v4',
                'HLT_Mu13_Mu8_v5',
                'HLT_Mu13_Mu8_v6',
                'HLT_Mu13_Mu8_v7',
                'HLT_Mu13_Mu8_v8',
                'HLT_Mu13_Mu8_v9',
                'HLT_Mu13_Mu8_v10',
                'HLT_Mu13_Mu8_v11',
                'HLT_Mu17_Mu8_v1'
                'HLT_Mu17_Mu8_v2'
                'HLT_Mu17_Mu8_v3'
                'HLT_Mu17_Mu8_v4',
                'HLT_Mu17_Mu8_v5',
                'HLT_Mu17_Mu8_v6',
                'HLT_Mu17_Mu8_v7',
                'HLT_Mu17_TkMu8_v1',
                'HLT_Mu17_TkMu8_v2',
                'HLT_Mu17_TkMu8_v3',
                'HLT_Mu17_TkMu8_v4'
                ]
    muEG = ['HLT_Mu17_Ele8_CaloIdL_v1',  #start run 160404
            'HLT_Mu17_Ele8_CaloIdL_v2',
            'HLT_Mu17_Ele8_CaloIdL_v3',   
            'HLT_Mu17_Ele8_CaloIdL_v4',
            'HLT_Mu17_Ele8_CaloIdL_v5',
            'HLT_Mu17_Ele8_CaloIdL_v6',
            'HLT_Mu17_Ele8_CaloIdL_v7',
            'HLT_Mu17_Ele8_CaloIdL_v8',
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v1', #start run 167039
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v2',
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v3', 
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v4',
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v5',
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v6',
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v7',
            'HLT_Mu8_Ele17_CaloIdL_v1',   #start run 160404
            'HLT_Mu8_Ele17_CaloIdL_v2',
            'HLT_Mu8_Ele17_CaloIdL_v3',     
            'HLT_Mu8_Ele17_CaloIdL_v4',
            'HLT_Mu8_Ele17_CaloIdL_v5',
            'HLT_Mu8_Ele17_CaloIdL_v6',
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v1', #start run 167039
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v2',
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3', 
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v4',
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v5',
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v6',
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v7'
            ]
    singleMu = ['HLT_IsoMu17_v1',  #start run 160404
                'HLT_IsoMu17_v2',
                'HLT_IsoMu17_v3',  
                'HLT_IsoMu17_v4',
                'HLT_IsoMu17_v5',
                'HLT_IsoMu17_v6',
                'HLT_IsoMu17_v7',
                'HLT_IsoMu17_v8',
                'HLT_IsoMu17_v9',
                'HLT_IsoMu17_v10',
                'HLT_IsoMu17_v11',
                'HLT_IsoMu17_v12',
                'HLT_IsoMu17_v13',
                'HLT_IsoMu24_v1',   #start run 160404
                'HLT_IsoMu24_v2',
                'HLT_IsoMu24_v3',  
                'HLT_IsoMu24_v4',
                'HLT_IsoMu24_v5',
                'HLT_IsoMu24_v6',
                'HLT_IsoMu24_v7', 
                'HLT_IsoMu24_v8',
                'HLT_IsoMu30_eta2p1_v1',   #start run 173236
                'HLT_IsoMu30_eta2p1_v2',
                'HLT_IsoMu30_eta2p1_v3',
                'HLT_IsoMu30_eta2p1_v4',
                'HLT_IsoMu30_eta2p1_v5',
                'HLT_IsoMu30_eta2p1_v6',
                'HLT_IsoMu30_eta2p1_v7',
                'HLT_IsoMu34_eta2p1_v1',   #start run 173236
                'HLT_IsoMu34_eta2p1_v2',
                'HLT_IsoMu34_eta2p1_v3',
                'HLT_IsoMu34_eta2p1_v4',
                'HLT_IsoMu34_eta2p1_v5'
                ]
    #forget it...
    singleEle = []
    
    return doubleEle, doubleMu, muEG, singleEle, singleMu, mcTrigs

##
## adds trigger sequence
##
def addTriggerSequence(process, trigFilter='ee') :
    
    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel

    doubleEle, doubleMu, muEG, singleEle, singleMu, mcTrigs = getTriggerPaths()

    # double electron
    process.eetrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.eetrigFilter.throw = cms.bool(False)
    process.eetrigFilter.HLTPaths = doubleEle
    process.eeTrigSequence=cms.Sequence(process.eetrigFilter)

    # double muon
    process.mumutrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.mumutrigFilter.throw = cms.bool(False)
    process.mumutrigFilter.HLTPaths = doubleMu
    process.mumuTrigSequence=cms.Sequence(~process.eetrigFilter*
                                          process.mumutrigFilter)

    # electron-muon
    process.emutrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.emutrigFilter.throw = cms.bool(False)
    process.emutrigFilter.HLTPaths = muEG
    process.emuTrigSequence=cms.Sequence(~process.eetrigFilter*
                                         ~process.mumutrigFilter*
                                         process.emutrigFilter)
    
    # single muon
    process.singlemutrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.singlemutrigFilter.throw = cms.bool(False)
    process.singlemutrigFilter.HLTPaths = singleMu
    process.singleMuTrigSequence=cms.Sequence(~process.eetrigFilter*
                                              ~process.mumutrigFilter*
                                              ~process.emutrigFilter*
                                              process.singlemutrigFilter)

    # single electron
    process.singleetrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.singleetrigFilter.throw = cms.bool(False)
    process.singleetrigFilter.HLTPaths = singleEle
    process.singleEleTrigSequence=cms.Sequence(~process.eetrigFilter*
                                               ~process.mumutrigFilter*
                                               ~process.emutrigFilter*
                                               ~process.singlemutrigFilter*
                                               process.singleetrigFilter)



    # filter counters
    process.preTriggerCounter = cms.EDProducer("EventCountProducer")
    process.triggerCounter = process.preTriggerCounter.clone()
    
    if(trigFilter=='ee')   : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.eeTrigSequence*process.triggerCounter)
    if(trigFilter=='mumu') : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.mumuTrigSequence*process.triggerCounter)
    if(trigFilter=='emu')  : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.emuTrigSequence*process.triggerCounter)
    if(trigFilter=='emutaureplacement')  :  process.trigSequence = cms.Sequence(process.preTriggerCounter*process.triggerCounter)
    if(trigFilter=='e')    : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleEleTrigSequence*process.triggerCounter)
    if(trigFilter=='mu')   : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleMuTrigSequence*process.triggerCounter)
    if(trigFilter=='photon') :    addGammaTriggerSequence(process)

    print " *** Trigger paths are defined"

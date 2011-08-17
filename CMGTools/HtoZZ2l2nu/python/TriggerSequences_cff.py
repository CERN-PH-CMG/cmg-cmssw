import FWCore.ParameterSet.Config as cms

###
### trigger paths of interest
###
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
    doubleEle = ['HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5'
                 ]
    doubleMu = ['HLT_DoubleMu7_v1',
                'HLT_DoubleMu7_v2',
                'HLT_Mu13_Mu8_v2',
                ]
    muEG = ['HLT_Mu17_Ele8_CaloIdL_v1',
            'HLT_Mu17_Ele8_CaloIdL_v2',
            'HLT_Mu17_Ele8_CaloIdL_v3',
            'HLT_Mu17_Ele8_CaloIdL_v4',
            'HLT_Mu17_Ele8_CaloIdL_v5',
            'HLT_Mu17_Ele8_CaloIdL_v6',
            'HLT_Mu8_Ele17_CaloIdL_v1',
            'HLT_Mu8_Ele17_CaloIdL_v2',
            'HLT_Mu8_Ele17_CaloIdL_v3',
            'HLT_Mu8_Ele17_CaloIdL_v4',
            'HLT_Mu8_Ele17_CaloIdL_v5',
            'HLT_Mu8_Ele17_CaloIdL_v6'
            ]
    singleMu = ['HLT_IsoMu17_v1', 'HLT_IsoMu17_v2', 'HLT_IsoMu17_v3',
                'HLT_IsoMu17_v4', 'HLT_IsoMu17_v5', 'HLT_IsoMu17_v6',
                'HLT_IsoMu17_v7', 'HLT_IsoMu17_v8', 'HLT_IsoMu17_v9'
                ]
    #needs update these are 2010 still..
    singleEle = ['HLT_Ele10_LW_L1R',
                 'HLT_Ele15_SW_L1R',
                 'HLT_Ele15_SW_CaloEleId_L1R',
                 'HLT_Ele17_SW_CaloEleId_L1R',
                 'HLT_Ele17_SW_TightEleId_L1R',
                 'HLT_Ele17_SW_TighterEleIdIsol_L1R_v2',
                 'HLT_Ele17_SW_TighterEleIdIsol_L1R_v3']
    
    return doubleEle, doubleMu, muEG, singleEle, singleMu, mcTrigs

##
## adds trigger sequence
##
def addTriggerSequence(process, trigFilter='ee') :
    
    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel

    doubleEle, doubleMu, muEG, singleEle, singleMu = getTriggerPaths()

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
    if(trigFilter=='e')    : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleEleTrigSequence*process.triggerCounter)
    if(trigFilter=='mu')   : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleMuTrigSequence*process.triggerCounter)

    print " *** Trigger paths are defined"

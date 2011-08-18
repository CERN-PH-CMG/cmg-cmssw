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

    # Trigger evolution cf. http://fwyzard.web.cern.ch/fwyzard/hlt/summary
   
    doubleEle = ['HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1',                                   #start run 160404
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v7',
                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v8',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v5',  #start run 167039
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v6',
                 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v7'
                 ]
    doubleMu = ['HLT_DoubleMu7_v1', 'HLT_DoubleMu7_v2',                     #start run 160404
                'HLT_Mu13_Mu8_v1', 'HLT_Mu13_Mu8_v2', 'HLT_Mu13_Mu8_v3',    #start run 165088 
                'HLT_Mu13_Mu8_v4', 'HLT_Mu13_Mu8_v5', 'HLT_Mu13_Mu8_v6'
                ]
    muEG = ['HLT_Mu17_Ele8_CaloIdL_v1', 'HLT_Mu17_Ele8_CaloIdL_v2', 'HLT_Mu17_Ele8_CaloIdL_v3',   #start run 160404
            'HLT_Mu17_Ele8_CaloIdL_v4', 'HLT_Mu17_Ele8_CaloIdL_v5', 'HLT_Mu17_Ele8_CaloIdL_v6',
            'HLT_Mu17_Ele8_CaloIdL_v7', 'HLT_Mu17_Ele8_CaloIdL_v8'
            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v1', 'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v2', 'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v3', #start run 167039
            'HLT_Mu8_Ele17_CaloIdL_v1', 'HLT_Mu8_Ele17_CaloIdL_v2', 'HLT_Mu8_Ele17_CaloIdL_v3',     #start run 160404
            'HLT_Mu8_Ele17_CaloIdL_v4', 'HLT_Mu8_Ele17_CaloIdL_v5', 'HLT_Mu8_Ele17_CaloIdL_v6',
            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v1', 'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v2', 'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3' #start run 167039
            ]
    singleMu = ['HLT_IsoMu17_v1', 'HLT_IsoMu17_v2', 'HLT_IsoMu17_v3',  #start run 160404
                'HLT_IsoMu17_v4', 'HLT_IsoMu17_v5', 'HLT_IsoMu17_v6',
                'HLT_IsoMu17_v7', 'HLT_IsoMu17_v8', 'HLT_IsoMu17_v9',
                'HLT_IsoMu17_v10', 'HLT_IsoMu17_v11', 'HLT_IsoMu17_v12',
                'HLT_IsoMu17_v13',
                'HLT_IsoMu24_v1', 'HLT_IsoMu24_v2', 'HLT_IsoMu24_v3',  #start run 160404
                'HLT_IsoMu24_v2', 'HLT_IsoMu24_v3', 'HLT_IsoMu24_v4',
                'HLT_IsoMu24_v5', 'HLT_IsoMu24_v6', 'HLT_IsoMu24_v7', 
                'HLT_IsoMu24_v8'
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
    if(trigFilter=='e')    : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleEleTrigSequence*process.triggerCounter)
    if(trigFilter=='mu')   : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleMuTrigSequence*process.triggerCounter)

    print " *** Trigger paths are defined"

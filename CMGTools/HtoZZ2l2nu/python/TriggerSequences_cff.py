import FWCore.ParameterSet.Config as cms

##
## adds trigger sequence
##
def addTriggerSequence(process, trigFilter='ee') :
    
    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel

    # double electron
    process.eetrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.eetrigFilter.throw = cms.bool(False)
    process.eetrigFilter.HLTPaths = ['HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1',
                                     'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2',
                                     'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3'
                                     ]
    process.eeTrigSequence=cms.Sequence(process.eetrigFilter)

    # double muon
    process.mumutrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.eetrigFilter.throw = cms.bool(False)
    process.mumutrigFilter.HLTPaths =['HLT_DoubleMu7_v1',
                                      'HLT_DoubleMu7_v2'
                                      ]
    process.mumuTrigSequence=cms.Sequence(~process.eetrigFilter*
                                          process.mumutrigFilter)

    # electron-muon
    process.emutrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.eetrigFilter.throw = cms.bool(False)
    process.emutrigFilter.HLTPaths = ['HLT_Mu17_Ele8_CaloIdL_v1',
                                      'HLT_Mu17_Ele8_CaloIdL_v2',
                                      'HLT_Mu17_Ele8_CaloIdL_v3',
                                      'HLT_Mu8_Ele17_CaloIdL_v1',
                                      'HLT_Mu8_Ele17_CaloIdL_v2',
                                      'HLT_Mu8_Ele17_CaloIdL_v3'
                                      ]
    process.emuTrigSequence=cms.Sequence(~process.eetrigFilter*
                                         ~process.mumutrigFilter*
                                         process.emutrigFilter)

    # single muon
    process.singlemutrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
    process.eetrigFilter.throw = cms.bool(False)
    process.singlemutrigFilter.HLTPaths = ['HLT_Mu9',
                                           'HLT_Mu15_v1']
    process.singleEleTrigSequence=cms.Sequence(~process.eetrigFilter*
                                               ~process.mumutrigFilter*
                                               ~process.emutrigFilter*
                                               process.singleetrigFilter)

    # single electron
    process.singleetrigFilter = process.eetrigFilter.clone()
    process.singleetrigFilter.HLTPaths = ['HLT_Ele10_LW_L1R',
                                          'HLT_Ele15_SW_L1R',
                                          'HLT_Ele15_SW_CaloEleId_L1R',
                                          'HLT_Ele17_SW_CaloEleId_L1R',
                                          'HLT_Ele17_SW_TightEleId_L1R',
                                          'HLT_Ele17_SW_TighterEleIdIsol_L1R_v2',
                                          'HLT_Ele17_SW_TighterEleIdIsol_L1R_v3']
    process.singleMuTrigSequence=cms.Sequence(~process.eetrigFilter*
                                              ~process.mumutrigFilter*
                                              ~process.emutrigFilter*
                                              ~process.singleetrigFilter*
                                              process.singlemutrigFilter)

    # filter counters
    process.preTriggerCounter = cms.EDProducer("EventCountProducer")
    process.triggerCounter = process.preTriggerCounter.clone()
    
    if(trigFilter=='ee')   : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.eeTrigSequence*process.triggerCounter)
    if(trigFilter=='mumu') : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.mumuTrigSequence*process.triggerCounter)
    if(trigFilter=='emu')  : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.emuTrigSequence*process.triggerCounter)
    if(trigFilter=='e')    : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleEleTrigSequence*process.triggerCounter)
    if(trigFilter=='mu')   : process.trigSequence = cms.Sequence(process.preTriggerCounter*process.singleMuTrigSequence*process.triggerCounter)

    print " *** Trigger paths are defined"

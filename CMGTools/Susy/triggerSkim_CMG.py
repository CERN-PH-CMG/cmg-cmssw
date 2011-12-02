## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.pfTools import *

### MASTER FLAGS  ######################################################################
# turn on when running on MC
runOnMC = False

runCMG = True

skimOnTrigger = True

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.setName_('L1Skim')
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10

### SOURCE DEFINITION  ################################################################

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/MultiJet/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_4_0',
    'patTuple_PF2PAT_.*root'
    ) 

### DEFINITION OF THE PF2PAT+PAT SEQUENCES #############################################

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))


process.load('CMGTools.Common.gen_cff')

### PATH DEFINITION #############################################

process.p = cms.Path()
if skimOnTrigger:
    import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt
    process.filterL1QuadJet = hlt.triggerResultsFilter.clone(
            triggerConditions = cms.vstring(
                                'L1_QuadJet20_Central',
                                'L1_QuadJet28_Central',
                                'L1_HTT100'
            ),
            l1tResults = cms.InputTag('gtDigis')
    )
    process.p += process.filterL1QuadJet
 
# CMG ---

if runCMG:
    
    process.load('CMGTools.Common.analysis_cff')
    # running on PFAOD -> calo objects are not available.
    # we'll need to reactivate caloMET, though
    # process.p += process.analysisSequence
    
    from CMGTools.Common.Tools.tuneCMGSequences import * 
    tuneCMGSequences(process, postpostfix='CMG')

    process.p += process.analysisSequence

### OUTPUT DEFINITION #############################################
from CMGTools.Common.eventContent.everything_cff import everything 

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_CMG.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_CMG.root"))


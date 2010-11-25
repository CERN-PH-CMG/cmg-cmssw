from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

runPF2PAT = False
if runPF2PAT:
    process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")
    from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
    usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=False)
else:
    #process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_80to120_Tune1_7TeV_pythia8.Fall10_START38_V12_v1.AODSIM.source_cff")
    process.source.fileNames = cms.untracked.vstring(
#         'file:/afs/cern.ch/user/w/wreece/public/patTuple_PF2PAT.root'
         'file:/afs/cern.ch/user/c/cbern/public/patTuple_PATandPF2PAT.root'
    #    'file:patTuple_PF2PAT.root '
    )
    process.setName_('ANA')

process.out.fileName = cms.untracked.string('testCMGTools.root')
#process.out.outputCommands = cms.untracked.vstring(
#        'keep  cmg*_*_*_*',
#        'keep  patJets_*_*_*',
#     )


from CMGTools.Common.EventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase  
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( particleFlowEventContent ) 
process.out.outputCommands.extend( particleFlowEventContentBase )
process.out.outputCommands.append( 'keep cmgBaseMETs_*_*_*' )
    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms.root"))


process.p = cms.Path()
if runPF2PAT:
    process.p += process.patPF2PATSequence 

process.load('CMGTools.Common.electron_cff')
process.load('CMGTools.Common.muon_cff')
process.load('CMGTools.Common.diMuon_cff')
process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.met_cff')
process.load('CMGTools.Common.pfcands_cff')
process.load('CMGTools.Common.w_cff')

process.load('CMGTools.Common.cutsummary_cff')
process.load('CMGTools.Common.Tools.indexSelector_cfi')

process.cmgPFJet.verbose = cms.untracked.bool( False )

process.load("CMGTools.Common.runInfoAccounting_cfi")

process.baseMETSelector = cms.EDFilter(
    'BaseMETSelector',
    src = cms.InputTag('cmgMHTPFJets'),
    cut = cms.string('pt()>20')
    )

process.analysisSequence = cms.Sequence(
    process.electronSequence + 
    process.muonSequence + 
    process.diMuonSequence +
    process.jetSequence +
    process.metSequence +
    process.baseMETSelector +
    process.pfcandsSequence +
    process.wSequence +
    process.indexSelector +
    process.runInfoAccounting + 
    process.cutsummary
    )


PFCandidatesAvailable = True
if PFCandidatesAvailable == False:
    # the following are removed from the sequence as they take PFCandidates
    process.analysisSequence.remove( process.cmgMETPFCandidates )
    process.analysisSequence.remove( process.cmgMETPFCandidates2 )
    process.analysisSequence.remove( process.pfMETLorentzVector )
    process.analysisSequence.remove( process.pfMET2LorentzVector )

process.p += process.analysisSequence


process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 

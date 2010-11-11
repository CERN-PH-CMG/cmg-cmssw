from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# ext = 'DATA'
ext = 'Z2_80to120'
# ext = 'Z2_120to170'
# ext = 'Z2_170to300'
# ext = 'Z2_300to470'

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

runPF2PAT = False
if runPF2PAT:
    process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")
    from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
    usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=False)
else:
    # Tune 1
    if ext == 'T1_80to120':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_80to120_Tune1_7TeV_pythia8.Fall10_START38_V12_v1.AODSIM.source_cff")
    if ext == 'T1_120to170':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_120to170_Tune1_7TeV_pythia8.Fall10_START38_V12_v1.AODSIM.source_cff")
    if ext == 'T1_170to300':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_170to300_Tune1_7TeV_pythia8.Fall10_START38_V12_v1.AODSIM.source_cff")
    if ext == 'T1_300to470':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_300to470_Tune1_7TeV_pythia8.Fall10_START38_V12_v1.AODSIM.source_cff")

    # Tune Z2
    if ext == 'Z2_80to120':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_80to120_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.AODSIM.source_cff")
    if ext == 'Z2_120to170':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_120to170_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.AODSIM.source_cff")
    if ext == 'Z2_170to300':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_170to300_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.AODSIM.source_cff")
    if ext == 'Z2_300to470':
        process.load("PFAnalyses.DijetAnalysis.Sources.QCD_Pt_300to470_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.AODSIM.source_cff")

    # Testing 
    if ext == 'TESTDATA':
        process.source.fileNames = cms.untracked.vstring('data.root')
    if ext == 'TESTMC':
        process.source.fileNames = cms.untracked.vstring('mc.root')

    # data 
    if ext == 'DATANOFILTER':
        process.load("PFAnalyses.DijetAnalysis.Sources.JetMETTau.Run2010A_Jul26thReReco_v1.RECO.CorrectJetPtSelection.source_cff")
    if ext == 'DATA':
        # Prod Aug6, 
        process.load("PFAnalyses.DijetAnalysis.Sources.JetMETTau.Run2010A_Jul26thReReco_v1.RECO.CorrectJetPtSelection.HBHENoiseFilter.source_cff")

    if ext == 'TTBAR':
        process.load("PFAnalyses.DijetAnalysis.Sources.MC.TTbar.Spring10_START3X_V26_S09_v1.AODSIM.patTuple_cff")
    if ext == 'W2J0to100':
        process.load("PFAnalyses.DijetAnalysis.Sources.MC.W2Jets_Pt0to100_alpgen.Spring10_START3X_V26_S09_v1.AODSIM.patTuple_cff")
    if ext == 'W2J100to300':
        process.load("PFAnalyses.DijetAnalysis.Sources.MC.W2Jets_Pt100to300_alpgen.Spring10_START3X_V26_S09_v1.AODSIM.patTuple_cff")
    if ext == 'LM0':
        process.load("PFAnalyses.DijetAnalysis.Sources.LM0.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
    if ext == 'LM1':
        process.load("PFAnalyses.DijetAnalysis.Sources.LM1.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")


    # process.source.fileNames = cms.untracked.vstring(
    #    'file:/afs/cern.ch/user/c/cbern/scratch0/patTuple_PF2PAT.root '
    # )

    # changing process name, as PAT is already taken 
    process.setName_('ANA')


print 'processing: '
print process.source.fileNames

process.out.fileName = cms.untracked.string('susyJetMET_%s.root' % ext )
process.out.outputCommands = cms.untracked.vstring(
#        'keep  cmgBaseJets_*_*_*',                                
        'keep  cmgPFJets_*_*_*',
        'keep  cmgBaseMETs_*_*_*'
     )

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("susyJetMET_histograms_%s.root" % ext ) )


process.p = cms.Path()
if runPF2PAT:
    process.p += process.patPF2PATSequence 

process.load('CMGTools.Common.muon_cff')
process.load('CMGTools.Common.diMuon_cff')
process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.met_cff')

process.cmgPFJet.verbose = cms.untracked.bool( False )

process.load("CMGTools.Common.runInfoAccounting_cfi")

process.analysisSequence = cms.Sequence(
#    process.muonSequence + 
#    process.diMuonSequence +
    process.jetSequence +
    process.metSequence +
    process.runInfoAccounting
    )

process.p += process.analysisSequence


process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 

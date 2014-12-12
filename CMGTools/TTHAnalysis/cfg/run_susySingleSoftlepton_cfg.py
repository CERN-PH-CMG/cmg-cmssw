##########################################################
##CONFIGURATIONFORSUSYMULTILEPTONTREES##
##skimcondition:>=2looseleptons,noptcutsorid##
##########################################################

importCMGTools.RootTools.fwlite.Configascfg
fromCMGTools.RootTools.fwlite.ConfigimportprintComps
fromCMGTools.RootTools.RootToolsimport*

#Loadallanalyzers
fromCMGTools.TTHAnalysis.analyzers.susyCore_modules_cffimport*

ttHLepAna.loose_muon_pt=5
ttHLepAna.loose_muon_relIso=0.4
ttHLepAna.mu_isoCorr="deltaBeta"
#ttHLepAna.loose_muon_absIso5=10
ttHLepAna.loose_electron_pt=7
ttHLepAna.loose_electron_relIso=0.4
#ttHLepAna.loose_electron_absIso=10
ttHLepAna.ele_isoCorr="rhoArea"




#---LEPTONSKIMMING---
ttHLepSkim.minLeptons=1
ttHLepSkim.maxLeptons=999

#---JET-LEPTONCLEANING---
ttHJetAna.minLepPt=10


#EventAnalyzerforsusymulti-lepton(atthemoment,it'stheTTHone)//DoweneedhadWandTop?
ttHEventAna=cfg.Analyzer(
'ttHLepEventAnalyzer',
minJets25=0,
)


ttHIsoTrackAna.setOff=False

susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna),
ttHSVAnalyzer)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna),
ttHHeavyFlavourHadronAnalyzer)


fromCMGTools.TTHAnalysis.samples.samples_8TeV_v517importtriggers_1mu,triggers_1muHT,triggers_1eleHT#needtoupdatethetriggerMETprHTMET?

#TreeProducer
treeProducer=cfg.Analyzer(
'treeProducerSusySingleSoftLepton',
vectorTree=True,
saveTLorentzVectors=False,#cansettoTruetogetalsotheTLorentzVectors,buttreeswillbebigger
PDFWeights=PDFWeights,
triggerBits={}
#'MuHT':triggers_1muHT,
#'eleHT':triggers_1eleHT
#}
)


#--------SAMPLESANDTRIGGERS-----------
fromCMGTools.TTHAnalysis.samples.samples_13TeV_CSA14import*
selectedComponents=[SingleMu,DoubleElectron,TTHToWW_PUS14,DYJetsToLL_M50_PU20bx25,TTJets_PUS14]


#--------SEQUENCE

sequence=cfg.Sequence(susyCoreSequence+[
ttHEventAna,
treeProducer,
])


#--------HOWTORUN
test=1
iftest==1:
#testasinglecomponent,usingasinglethread.
comp=TTJets_PUS14
#comp.files=['root://eoscms//eos/cms/store/cmst3/group/susy/alobanov/MC/MiniAOD/13TeV_Gl_Gl_4q_Gl1400_LSP300_Chi315_MiniAOD.root']
#comp.files=comp.files[:1]
selectedComponents=[comp]
comp.splitFactor=1
eliftest==2:
#testallcomponents(1threadpercomponent).
forcompinselectedComponents:
comp.splitFactor=len(comp.files)
#comp.files=comp.files[:1]


config=cfg.Config(components=selectedComponents,
sequence=sequence)

printComps(config.components,True)

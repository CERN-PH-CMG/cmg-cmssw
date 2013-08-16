import os, sys

import CMGTools.RootTools.fwlite.Config as cfg

def load():
    #load the libaries needed
    from ROOT import gROOT,gSystem
    gSystem.Load("libCintex")
    gROOT.ProcessLine('ROOT::Cintex::Cintex::Enable();')
        
    #now the RootTools stuff
    gSystem.Load("libCMGToolsExternal")

load()
### sys.path.append( '/'.join( [ os.environ['CMSSW_BASE'],
###                                  'python/CMG/JetIDAnalysis/analyzers'] ))

jetAna = cfg.Analyzer(
    'JetIDAnalyzer',
    ptCut = 20,
    ## use pat::Jets
    ## jetCollection = ('selectedPatJetsAK5','std::vector<pat::Jet>'),
    jetCollection = ('selectedPatJets','std::vector<pat::Jet>'),
    ## jetCollection = ('selectedPatJetsPFlow','std::vector<pat::Jet>'),
    ## or cmg::Jets
    ## jetCollection = ('cmgPFJetSel','std::vector<cmg::PFJet>'),
    doJetIdHisto    = True,
    dumpTree        = False,
    applyPFLooseId  = True, 
    jetIdMva = ( 0, "%s/src/CMGTools/External/data/mva_JetID.weights.xml" % os.getenv("CMSSW_BASE"), "JetID" ),
    ## genJetsCollection =  ("ak5GenJets","vector<reco::GenJet>"),
    genJetsCollection =  (("selectedPatJets","genJets"),"vector<reco::GenJet>"),
    ## genJetsCollection =  (("selectedPatJetsPFlow","genJets"),"vector<reco::GenJet>"),
    useGenLeptons = False,
)

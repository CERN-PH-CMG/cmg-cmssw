from CMGTools.TTHAnalysis.analyzers.ttHLepTreeProducerNew import *

class treeProducerWMassEle( ttHLepTreeProducerNew ):

    #-----------------------------------
    # CORE TREE PRODUCER FOR THE SUSY ANALYSES
    # defines the core variables that will be present in the trees of all final states
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerWMassEle,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill
        self.globalVariables = [
            NTupleVariable("rho",  lambda ev: ev.rho, float, help="kt6PFJets rho"),
            NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
            ##--------------------------------------------------
            NTupleVariable("GenHeaviestQCDFlavour", lambda ev : ev.heaviestQCDFlavour, int, mcOnly=True, help="pdgId of heaviest parton in the event (after shower)"),
            NTupleVariable("LepEff_1lep", lambda ev : ev.LepEff_1lep, mcOnly=True, help="Lepton preselection SF (1 lep)"),
            NTupleVariable("LepEff_2lep", lambda ev : ev.LepEff_2lep, mcOnly=True, help="Lepton preselection SF (2 lep)"),
            ##--------------------------------------------------            
            NTupleVariable("mZ1", lambda ev : ev.bestZ1[0], help="Best m(ll) SF/OS"),
            NTupleVariable("m2l", lambda ev: ev.m2l, help="m(ll)"),
            ##------------------------------------------------
        ]

        self.globalObjects = {
            "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
            "metraw" : NTupleObject("metraw", metType, help="PF raw E_{T}^{miss}"),
            "metNoPU" : NTupleObject("metNoPU", fourVectorType, help="PF noPU E_{T}^{miss}"),
            "pucmet" : NTupleObject("pucmet", fourVectorType, help="PF PUC E_{T}^{miss}"),
            "pumet" : NTupleObject("pumet", fourVectorType, help="PF PU E_{T}^{miss}"),
            "tkmet" : NTupleObject("tkmet", fourVectorType, help="Track E_{T}^{miss}"),
        }


        leptonTypeSusy.addVariables([ 
                NTupleVariable("trgMatch", lambda lepton : lepton.TriggerMatched, help="Match a trigger object by path"),
                NTupleVariable("eleMVAPreselId", lambda lepton : lepton.tightIdPreselResult if abs(lepton.pdgId()) == 11 else 1, help="EGamma POG MVA Preselection ID for triggering electrons; 1 for muons"),
                NTupleVariable("scEta", lambda lepton : lepton.sourcePtr().superCluster().eta() if abs(lepton.pdgId()) == 11 else -99, help="SuperCluster eta, only for electrons; -99 for muons"),
                NTupleVariable("r9", lambda lepton : lepton.sourcePtr().r9() if abs(lepton.pdgId()) == 11 else -99, help="SuperCluster 5x5 r9 variable, only for electrons; -99 for muons"),
                NTupleVariable("classification",      lambda lepton : lepton.sourcePtr().classification() if abs(lepton.pdgId()) == 11 else -99, help="electron classification; -99 for muons"),
                NTupleVariable("detaIn",      lambda lepton : lepton.sourcePtr().deltaEtaSuperClusterTrackAtVtx() if abs(lepton.pdgId()) == 11 else -99, help="DeltaEta track-cluster at vertex, only for electrons; -99 for muons"),
                NTupleVariable("dphiIn",      lambda lepton : lepton.sourcePtr().deltaPhiSuperClusterTrackAtVtx() if abs(lepton.pdgId()) == 11 else -99, help="DeltaPhi track-cluster at vertex, only for electrons; -99 for muons"),
                NTupleVariable("sigmaIetaIeta", lambda x: x.sourcePtr().sigmaIetaIeta() if (abs(x.pdgId())==11) else -999, help="Electron sigmaIetaIeta"),
                NTupleVariable("sigmaIphiIphi", lambda x: x.sourcePtr().sigmaIphiIphi() if (abs(x.pdgId())==11) else -999, help="Electron sigmaIphiIphi"),
                NTupleVariable("hcalOverEcal", lambda x: x.sourcePtr().hadronicOverEm() if (abs(x.pdgId())==11) else -999, help="Electron hcalOverEcal"),
                NTupleVariable("correctedEcalEnergyError", lambda x: x.sourcePtr().correctedEcalEnergyError() if (abs(x.pdgId())==11) else -999, help="Electron correctedEcalEnergyError"),
                NTupleVariable("regressionEnergyError", lambda x: x.sourcePtr().p4Error(x.sourcePtr().candidateP4Kind()) if (abs(x.pdgId())==11) else -999, help="Electron EcalEnergyError from regression"),
                NTupleVariable("ecalEnergy", lambda x: x.sourcePtr().ecalEnergy() if (abs(x.pdgId())==11) else -999, help="Electron ecalEnergy"),
                NTupleVariable("superCluster_rawEnergy", lambda x: x.sourcePtr().superCluster().rawEnergy() if (abs(x.pdgId())==11) else -999, help="Electron superCluster.rawEnergy"),
                NTupleVariable("superCluster_preshowerEnergy", lambda x: x.sourcePtr().superCluster().preshowerEnergy() if (abs(x.pdgId())==11) else -999, help="Electron superCluster.preshowerEnergy"),
                NTupleVariable("superCluster_energy", lambda x: x.sourcePtr().superCluster().energy() if (abs(x.pdgId())==11) else -999, help="Electron superCluster.energy"),
                NTupleVariable("eSuperClusterOverP", lambda x: x.sourcePtr().eSuperClusterOverP() if (abs(x.pdgId())==11) else -999, help="Electron eSuperClusterOverP"),                
                ])

        self.collections = {
            "selectedLeptons" : NTupleCollection("LepGood", leptonTypeSusy, 8, help="Leptons after the preselection"),
            "otherLeptons"    : NTupleCollection("LepOther", leptonTypeSusy, 8, help="Leptons after the preselection"),
            "cleanJets"       : NTupleCollection("Jet",     jetTypeSusy, 8, help="Cental jets after full selection and cleaning, sorted by pt"),
            "cleanJetsFwd"    : NTupleCollection("JetFwd",  jetTypeSusy, 4, help="Forward jets after full selection and cleaning, sorted by pt"),            
            ##--------------------------------------------------
            "genleps"         : NTupleCollection("GenLep",     genParticleWithSourceType, 6, help="Generated leptons from W/Z decays"),
            "genStatusThree"  : NTupleCollection("GenP6StatusThree", genParticleWithMotherId, 100 , help="Pythia6 status three particles"),
            }

        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerWMassEle":
            self.initDone = True
            self.declareVariables()

from CMGTools.TTHAnalysis.analyzers.ttHLepTreeProducerNew import *

class treeProducerSusyCore( ttHLepTreeProducerNew ):

    #-----------------------------------
    # CORE TREE PRODUCER FOR THE SUSY ANALYSES
    # defines the core variables that will be present in the trees of all final states
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyCore,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill
        self.globalVariables = [ 
            NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
            NTupleVariable("nJet25", lambda ev: len(ev.cleanJets), int, help="Number of jets with pt > 25"),
            NTupleVariable("nBJetLoose25", lambda ev: len(ev.bjetsLoose), int, help="Number of jets with pt > 25 passing CSV loose"),
            NTupleVariable("nBJetMedium25", lambda ev: len(ev.bjetsMedium), int, help="Number of jets with pt > 25 passing CSV medium"),
            NTupleVariable("nBJetTight25", lambda ev: sum([j.btagWP("CSVT") for j in ev.bjetsMedium]), int, help="Number of jets with pt > 25 passing CSV tight"),
            NTupleVariable("nJet40", lambda ev: sum([j.pt() > 40 for j in ev.cleanJets]), int, help="Number of jets with pt > 40"),
            NTupleVariable("nBJetLoose40", lambda ev: sum([j.btagWP("CSVL") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV loose"),
            NTupleVariable("nBJetMedium40", lambda ev: sum([j.btagWP("CSVM") for j in ev.bjetsMedium if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
            NTupleVariable("nBJetTight40", lambda ev: sum([j.btagWP("CSVT") for j in ev.bjetsMedium if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV tight"),
            NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("htJet40j", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("htJet40", lambda ev : ev.htJet40, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("mhtJet40", lambda ev : ev.mhtJet40, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("mZ1", lambda ev : ev.bestZ1[0], help="Best m(ll) SF/OS"),
            NTupleVariable("mZ1SFSS", lambda ev : ev.bestZ1sfss[0], help="Best m(ll) SF/SS"),
            NTupleVariable("minMllSFOS", lambda ev: ev.minMllSFOS, help="min m(ll), SF/OS"),
            NTupleVariable("maxMllSFOS", lambda ev: ev.maxMllSFOS, help="max m(ll), SF/OS"),
            NTupleVariable("minMllAFOS", lambda ev: ev.minMllAFOS, help="min m(ll), AF/OS"),
            NTupleVariable("maxMllAFOS", lambda ev: ev.maxMllAFOS, help="max m(ll), AF/OS"),
            NTupleVariable("minMllAFSS", lambda ev: ev.minMllAFSS, help="min m(ll), AF/SS"),
            NTupleVariable("maxMllAFSS", lambda ev: ev.maxMllAFSS, help="max m(ll), AF/SS"),
            NTupleVariable("minMllAFAS", lambda ev: ev.minMllAFAS, help="min m(ll), AF/AS"),
            NTupleVariable("maxMllAFAS", lambda ev: ev.maxMllAFAS, help="max m(ll), AF/AS"),
            NTupleVariable("m2l", lambda ev: ev.m2l, help="m(ll)"),
            ##--------------------------------------------------
            NTupleVariable("GenHeaviestQCDFlavour", lambda ev : ev.heaviestQCDFlavour, int, mcOnly=True, help="pdgId of heaviest parton in the event (after shower)"),
            NTupleVariable("LepEff_1lep", lambda ev : ev.LepEff_1lep, mcOnly=True, help="Lepton preselection SF (1 lep)"),
            NTupleVariable("LepEff_2lep", lambda ev : ev.LepEff_2lep, mcOnly=True, help="Lepton preselection SF (2 lep)"),
            ##------------------------------------------------
            NTupleVariable("GenSusyMScan1", lambda ev : ev.genSusyMScan1, int, mcOnly=True, help="Susy mass 1 in scan"),
            NTupleVariable("GenSusyMScan2", lambda ev : ev.genSusyMScan2, int, mcOnly=True, help="Susy mass 2 in scan"),
            NTupleVariable("GenSusyMScan3", lambda ev : ev.genSusyMScan3, int, mcOnly=True, help="Susy mass 3 in scan"),
            NTupleVariable("GenSusyMScan4", lambda ev : ev.genSusyMScan4, int, mcOnly=True, help="Susy mass 4 in scan"),
            NTupleVariable("GenSusyMGluino", lambda ev : ev.genSusyMGluino, int, mcOnly=True, help="Susy Gluino mass"),
            NTupleVariable("GenSusyMGravitino", lambda ev : ev.genSusyMGravitino, int, mcOnly=True, help="Susy Gravitino mass"),
            NTupleVariable("GenSusyMStop", lambda ev : ev.genSusyMStop, int, mcOnly=True, help="Susy Stop mass"),
            NTupleVariable("GenSusyMSbottom", lambda ev : ev.genSusyMSbottom, int, mcOnly=True, help="Susy Sbottom mass"),
            NTupleVariable("GenSusyMStop2", lambda ev : ev.genSusyMStop2, int, mcOnly=True, help="Susy Stop2 mass"),
            NTupleVariable("GenSusyMSbottom2", lambda ev : ev.genSusyMSbottom2, int, mcOnly=True, help="Susy Sbottom2 mass"),
            NTupleVariable("GenSusyMSquark", lambda ev : ev.genSusyMSquark, int, mcOnly=True, help="Susy Squark mass"),
            NTupleVariable("GenSusyMNeutralino", lambda ev : ev.genSusyMNeutralino, int, mcOnly=True, help="Susy Neutralino mass"),
            NTupleVariable("GenSusyMNeutralino2", lambda ev : ev.genSusyMNeutralino2, int, mcOnly=True, help="Susy Neutralino2 mass"),
            NTupleVariable("GenSusyMNeutralino3", lambda ev : ev.genSusyMNeutralino3, int, mcOnly=True, help="Susy Neutralino3 mass"),
            NTupleVariable("GenSusyMNeutralino4", lambda ev : ev.genSusyMNeutralino4, int, mcOnly=True, help="Susy Neutralino4 mass"),
            NTupleVariable("GenSusyMChargino", lambda ev : ev.genSusyMChargino, int, mcOnly=True, help="Susy Chargino mass"),
            NTupleVariable("GenSusyMChargino2", lambda ev : ev.genSusyMChargino2, int, mcOnly=True, help="Susy Chargino2 mass"),
        ]

        self.globalObjects = {
            "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
            "metNoPU" : NTupleObject("metNoPU", fourVectorType, help="PF noPU E_{T}^{miss}"),
        }
        self.collections = {
            "selectedLeptons" : NTupleCollection("LepGood", leptonTypeSusy, 8, help="Leptons after the preselection"),
            "otherLeptons"    : NTupleCollection("LepOther", leptonTypeSusy, 8, help="Leptons after the preselection"),
            "selectedTaus"    : NTupleCollection("TauGood", tauTypeSusy, 3, help="Taus after the preselection"),
            "cleanJets"       : NTupleCollection("Jet",     jetTypeSusy, 8, help="Cental jets after full selection and cleaning, sorted by pt"),
            "cleanJetsFwd"    : NTupleCollection("JetFwd",  jetTypeSusy, 4, help="Forward jets after full selection and cleaning, sorted by pt"),
            ##--------------------------------------------------
            "gentopquarks"    : NTupleCollection("GenTop",     genParticleType, 2, help="Generated top quarks from hard scattering"),
            "genbquarks"      : NTupleCollection("GenBQuark",  genParticleType, 2, help="Generated bottom quarks from top quark decays"),
            "genwzquarks"     : NTupleCollection("GenQuark",   genParticleWithSourceType, 6, help="Generated quarks from W/Z decays"),
            "genleps"         : NTupleCollection("GenLep",     genParticleWithSourceType, 6, help="Generated leptons from W/Z decays"),
            "gentauleps"      : NTupleCollection("GenLepFromTau", genParticleWithSourceType, 6, help="Generated leptons from decays of taus from W/Z/h decays"),
            "genStatusThree"   : NTupleCollection("GenP6StatusThree", genParticleWithMotherId, 100 , help="Pythia6 status three particles"),
            }

        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyCore":
            self.initDone = True
            self.declareVariables()

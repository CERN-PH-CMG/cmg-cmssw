from CMGTools.TTHAnalysis.analyzers.ttHLepTreeProducerNew import *

genStopType = NTupleObjectType("genStop", baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("b_pt",    lambda x : x.b.pt()),
    NTupleVariable("b_eta",   lambda x : x.b.eta()),
    NTupleVariable("b_phi",   lambda x : x.b.phi()),
    NTupleVariable("b_mass",  lambda x : x.b.mass()),
    NTupleVariable("chi0_pt",    lambda x : x.chi0.pt()),
    NTupleVariable("chi0_eta",   lambda x : x.chi0.eta()),
    NTupleVariable("chi0_phi",   lambda x : x.chi0.phi()),
    NTupleVariable("chi0_mass",  lambda x : x.chi0.mass()),
    NTupleVariable("w_pt",    lambda x : x.wp4.Pt()),
    NTupleVariable("w_eta",   lambda x : x.wp4.Eta()),
    NTupleVariable("w_phi",   lambda x : x.wp4.Phi()),
    NTupleVariable("w_mass",  lambda x : x.wp4.M()),
    NTupleVariable("mwb",     lambda x : (x.wp4 + x.b.p4()).M()),
    NTupleVariable("mwchi0",  lambda x : (x.wp4 + x.chi0.p4()).M()),
    NTupleVariable("mbchi0",  lambda x : (x.b.p4() + x.chi0.p4()).M()),
    NTupleVariable("decay",   lambda x : x.decay, int), 
])

class treeProducerSusyGenLevelOnly( ttHLepTreeProducerNew ):

    #-----------------------------------
    # CORE TREE PRODUCER FOR THE SUSY ANALYSES
    # defines the core variables that will be present in the trees of all final states
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyGenLevelOnly,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill
        self.globalVariables = [
            #NTupleVariable("rho",  lambda ev: ev.rho, float, help="kt6PFJets rho"),
            #NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
            NTupleVariable("nJet25", lambda ev: len(ev.cleanJets), int, help="Number of jets with pt > 25"),
            NTupleVariable("nJet40", lambda ev: sum([j.pt() > 40 for j in ev.cleanJets]), int, help="Number of jets with pt > 40, |eta|<2.4"),
            NTupleVariable("nJet40a", lambda ev: sum([j.pt() > 40 for j in ev.cleanJetsAll]), int, help="Number of jets with pt > 40, |eta|<4.7"),
            NTupleVariable("nBJetLoose25", lambda ev: sum([j.btagWP("CSVL") for j in ev.cleanJets if j.pt() > 25]), int, help="Number of jets with pt > 25 passing CSV loose"),
            NTupleVariable("nBJetMedium25", lambda ev: sum([j.btagWP("CSVM") for j in ev.cleanJets if j.pt() > 25]), int, help="Number of jets with pt > 25 passing CSV medium"),
            NTupleVariable("nBJetLoose40", lambda ev: sum([j.btagWP("CSVL") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV loose"),
            NTupleVariable("nBJetMedium40", lambda ev: sum([j.btagWP("CSVM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
            ##--------------------------------------------------
            NTupleVariable("nLepGood20", lambda ev: sum([l.pt() > 20 for l in ev.selectedLeptons]), int, help="Number of leptons with pt > 20"),
            NTupleVariable("nLepGood15", lambda ev: sum([l.pt() > 15 for l in ev.selectedLeptons]), int, help="Number of leptons with pt > 15"),
            NTupleVariable("nLepGood10", lambda ev: sum([l.pt() > 10 for l in ev.selectedLeptons]), int, help="Number of leptons with pt > 10"),
            ##-------- custom jets ------------------------------------------
            NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("htJet40j", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
            #NTupleVariable("htJet40ja", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<4.7, pt > 40 GeV)"),
            NTupleVariable("htJet40", lambda ev : ev.htJet40, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
            #NTupleVariable("htJet40a", lambda ev : ev.htJet40a, help="H_{T} computed from leptons and jets (with |eta|<4.7, pt > 40 GeV)"),
            NTupleVariable("mhtJet40", lambda ev : ev.mhtJet40, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
            #NTupleVariable("mhtJet40a", lambda ev : ev.mhtJet40a, help="H_{T}^{miss} computed from leptons and jets (with |eta|<4.7, pt > 40 GeV)"),
            ##--------------------------------------------------            
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
            NTupleVariable("mZ2", lambda ev : ev.bestZ2[3], help="m(ll) of second SF/OS pair, for ZZ reco."),
            NTupleVariable("m3l", lambda ev: ev.m3l, help="m(3l)"),
            NTupleVariable("m4l", lambda ev: ev.m4l, help="m(4l)"),
            NTupleVariable("pt2l", lambda ev: ev.pt2l, help="p_{T}(ll)"),
            NTupleVariable("pt3l", lambda ev: ev.pt3l, help="p_{T}(3l)"),
            NTupleVariable("pt4l", lambda ev: ev.pt4l, help="p_{T}(4l)"),
            NTupleVariable("ht3l", lambda ev: ev.ht3l, help="H_{T}(3l)"),
            NTupleVariable("ht4l", lambda ev: ev.ht4l, help="H_{T}(4l)"),
            NTupleVariable("q3l", lambda ev: ev.q3l, int, help="q(3l)"),
            NTupleVariable("q4l", lambda ev: ev.q4l, int, help="q(4l)"),
        ]

        self.globalObjects = {
            "met" : NTupleObject("met", fourVectorType, help="PF E_{T}^{miss}, after type 1 corrections"),
        }
        self.collections = {
            "selectedLeptons" : NTupleCollection("LepGood", particleType, 8, help="Leptons after the preselection"),
            "cleanJets"       : NTupleCollection("Jet",     fourVectorType, 8, help="Cental jets after full selection and cleaning, sorted by pt"),
            "cleanJetsFwd"    : NTupleCollection("JetFwd",  fourVectorType, 4, help="Forward jets after full selection and cleaning, sorted by pt"),            
            "genStops"    : NTupleCollection("GenStop",  genStopType, 2, help="Generated stop quarks"),            
            }

        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyGenLevelOnly":
            self.initDone = True
            self.declareVariables()

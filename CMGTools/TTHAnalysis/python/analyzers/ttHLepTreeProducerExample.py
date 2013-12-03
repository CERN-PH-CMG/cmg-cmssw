from CMGTools.TTHAnalysis.analyzers.ttHLepTreeProducerNew import *

class ttHLepTreeProducerExample( ttHLepTreeProducerNew ):

    #-----------------------------------
    #        EXAMPLE TREE PRODUCER 
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ttHLepTreeProducerExample,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill
        ## --------------- THIS BELOW IS ONLY AN EXAMPLE, YOU MUST REDEFINE IT IN YOUR CLASS ---------------------
        self.globalVariables = [ 
            # some example variables computed from the tthLepEventAnalyzer, or on the fly
            NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
            NTupleVariable("nJet25", lambda ev: len(ev.cleanJets), int, help="Number of jets with pt > 25"),
            NTupleVariable("nBJetMedium25", lambda ev: len(ev.bjetsMedium), int, help="Number of jets with pt > 25 passing CVS medium"),
            NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
        ]
        self.globalObjects = {
            # some individual objects computed from the tthLepEventAnalyzer or other modules
            "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
        }
        self.collections = {
            # some collections computed from the different analyzers
            "selectedLeptons" : NTupleCollection("LepGood", leptonType, 8, help="Leptons after the preselection"),
            "selectedTaus"    : NTupleCollection("TauGood", tauType,    3, help="Taus after the preselection"),
            "cleanJets"       : NTupleCollection("Jet",     jetType,    8, sortDescendingBy = lambda jet : jet.pt(), help="Cental jets after full selection and cleaning, sorted by pt"),
            "genleps"         : NTupleCollection("GenLep",     genParticleWithSourceType, 6, help="Generated leptons from W/Z decays"),
            "gentauleps"      : NTupleCollection("GenLepFromTau", genParticleWithSourceType, 6, help="Generated leptons from decays of taus from W/Z/h decays"),
            # some collections taken directly from the CMG Tuple
            "patMuons" : ( AutoHandle( ("patMuonsWithTrigger",), "std::vector<pat::Muon>" ),
                           NTupleCollection("PatMuons", particleType, 4, help="patMuons, directly from CMG Tuple") )
                     
            
        }
        ## --------------- END OF THE PART THAT YOU WILL REDEFINE ---------------------

        ## Now book the variables
        self.initDone = True
        self.declareVariables()

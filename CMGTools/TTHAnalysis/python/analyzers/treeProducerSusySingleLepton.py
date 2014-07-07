from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusySingleLepton( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS 
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusySingleLepton,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [

            ##-------- custom jets ------------------------------------------
            NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("htJet40j", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("htJet40ja", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<4.7, pt > 40 GeV)"),
            NTupleVariable("htJet40", lambda ev : ev.htJet40, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("htJet40a", lambda ev : ev.htJet40a, help="H_{T} computed from leptons and jets (with |eta|<4.7, pt > 40 GeV)"),
            NTupleVariable("mhtJet40", lambda ev : ev.mhtJet40, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("mhtJet40a", lambda ev : ev.mhtJet40a, help="H_{T}^{miss} computed from leptons and jets (with |eta|<4.7, pt > 40 GeV)"),
            ##--------------------------------------------------
            NTupleVariable("minMWjj", lambda ev: ev.minMWjj, int, help="minMWjj"),
            NTupleVariable("minMWjjPt", lambda ev: ev.minMWjjPt, int, help="minMWjjPt"),
            NTupleVariable("bestMWjj", lambda ev: ev.bestMWjj, int, help="bestMWjj"),
            NTupleVariable("bestMWjjPt", lambda ev: ev.bestMWjjPt, int, help="bestMWjjPt"),
            NTupleVariable("bestMTopHad", lambda ev: ev.bestMTopHad, int, help="bestMTopHad"),
            NTupleVariable("bestMTopHadPt", lambda ev: ev.bestMTopHadPt, int, help="bestMTopHadPt"),
            ##--------------------------------------------------
            ##------------------------------------------------
        ]

        self.globalObjects.update({
            # put more here
        })
        self.collections.update({
            # put more here
            "otherLeptons"    : NTupleCollection("LepOther", leptonTypeSusy, 8, help="Leptons after the preselection"),
            ##------------------------------------------------
            "cleanJetsAll"       : NTupleCollection("Jet",     jetTypeSusy, 8, help="Jets after full selection and cleaning, sorted by pt"),
        })

        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusySingleLepton":
            self.initDone = True
            self.declareVariables()

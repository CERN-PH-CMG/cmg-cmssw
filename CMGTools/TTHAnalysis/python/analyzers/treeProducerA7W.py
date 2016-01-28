from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerA7W( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS 
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerA7W,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [
            ##-------- MET Filter ------------------------------------------
            NTupleVariable("checkEcalDead",  lambda ev : ev.checkEcalDead, int, help="checkEcalDead"),
            NTupleVariable("checkhcalLaser",  lambda ev : ev.checkhcalLaser, int, help="checkhcalLaser"),
            NTupleVariable("checktrackingFailure",  lambda ev : ev.checktrackingFailure, int, help="checktrackingFailure"),
            NTupleVariable("checkprimaryVertex",  lambda ev : ev.checkprimaryVertex, int, help="checkprimaryVertex"),
            NTupleVariable("checknoscraping",  lambda ev : ev.checknoscraping, int, help="checknoscraping"),
            NTupleVariable("checktrackIsolationMaker",  lambda ev : ev.checktrackIsolationMaker, int, help="checktrackIsolationMaker"),
            NTupleVariable("checkmetNoiseCleaning",  lambda ev : ev.checkmetNoiseCleaning, int, help="checkmetNoiseCleaning"),
            NTupleVariable("checkeeBadSc",  lambda ev : ev.checkeeBadSc, int, help="checkeeBadSc"),
            NTupleVariable("checkecalLaser",  lambda ev : ev.checkecalLaser, int, help="checkecalLaser"),
            NTupleVariable("checktotalKinematics",  lambda ev : ev.checktotalKinematics, int, help="checktotalKinematics"),
            NTupleVariable("checkCSCTightHalo",  lambda ev : ev.checkCSCTightHalo, int, help="checkCSCTightHalo"),
            NTupleVariable("checkHBHENoise",  lambda ev : ev.checkHBHENoise, int, help="checkHBHENoise"),
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
            NTupleVariable("minMWjj", lambda ev: ev.minMWjj, int, help="minMWjj"),
            NTupleVariable("minMWjjPt", lambda ev: ev.minMWjjPt, int, help="minMWjjPt"),
            NTupleVariable("bestMWjj", lambda ev: ev.bestMWjj, int, help="bestMWjj"),
            NTupleVariable("bestMWjjPt", lambda ev: ev.bestMWjjPt, int, help="bestMWjjPt"),
            NTupleVariable("bestMTopHad", lambda ev: ev.bestMTopHad, int, help="bestMTopHad"),
            NTupleVariable("bestMTopHadPt", lambda ev: ev.bestMTopHadPt, int, help="bestMTopHadPt"),
            ##------------------------------------------------
        ]

        self.globalObjects.update({
            # put more here
        })
        self.collections.update({
            # put more here
            "otherLeptons"    : NTupleCollection("LepOther", leptonTypeSusy, 5, help="Leptons after the preselection"),
            ##------------------------------------------------
            "cleanJetsAll"    : NTupleCollection("Jet",     jetTypeSusy, 10, help="Jets after full selection and cleaning, sorted by pt"),
            "genJets"    :NTupleCollection("GenJet", fourVectorType, 50,  help="GenJets"),

        })

        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerA7W":
            self.initDone = True
            self.declareVariables()

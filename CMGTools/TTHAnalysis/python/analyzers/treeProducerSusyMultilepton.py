from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusyMultilepton( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS 
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyMultilepton,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [
            ##-------- custom jets ------------------------------------------
            NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("htJet40j", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("htJet40ja", lambda ev : ev.htJet40ja, help="H_{T} computed from only jets (with |eta|<4.7, pt > 40 GeV)"),
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
            ##--------------------------------------------------
            NTupleVariable("minMWjj", lambda ev: ev.minMWjj, int, help="minMWjj"),
            NTupleVariable("minMWjjPt", lambda ev: ev.minMWjjPt, int, help="minMWjjPt"),
            NTupleVariable("bestMWjj", lambda ev: ev.bestMWjj, int, help="bestMWjj"),
            NTupleVariable("bestMWjjPt", lambda ev: ev.bestMWjjPt, int, help="bestMWjjPt"),
            NTupleVariable("bestMTopHad", lambda ev: ev.bestMTopHad, int, help="bestMTopHad"),
            NTupleVariable("bestMTopHadPt", lambda ev: ev.bestMTopHadPt, int, help="bestMTopHadPt"),
            ##--------------------------------------------------
            NTupleVariable("GenHiggsDecayMode", lambda ev : ev.genHiggsDecayMode, int, mcOnly=True, help="H decay mode (15 = tau, 23/24 = W/Z)"),
            NTupleVariable("LepEff_3lep", lambda ev : ev.LepEff_3lep, mcOnly=True, help="Lepton preselection SF (3 lep)"),
            NTupleVariable("LepEff_4lep", lambda ev : ev.LepEff_4lep, mcOnly=True, help="Lepton preselection SF (4 lep)"),
            ##--------vertex variables------------------------------------------
            #A = selectedLeptons[0], B = selectedLeptons[1], C = selectedLeptons[2], D = selectedLeptons[3] 
            ##Variables related to IP
            #Of one lepton w.r.t. the PV of the event
            NTupleVariable("absIP3DAval", lambda ev: ev.absIP3DA[0], help="Absolute IP of A w.r.t. PV of evt"),
            NTupleVariable("absIP3DAerr", lambda ev: ev.absIP3DA[1], help="Error of absIP3DAval"),
            NTupleVariable("absIP3DBval", lambda ev: ev.absIP3DB[0], help="Absolute IP of B w.r.t. PV of evt"),
            NTupleVariable("absIP3DBerr", lambda ev: ev.absIP3DB[1], help="Error of absIP3DBval"),     

            NTupleVariable("absIP3DCval", lambda ev: ev.absIP3DC[0], help="Absolute IP of C w.r.t. PV of evt"),
            NTupleVariable("absIP3DCerr", lambda ev: ev.absIP3DC[1], help="Error of absIP3DCval"),

            NTupleVariable("absIP3DDval", lambda ev: ev.absIP3DD[0], help="Absolute IP of D w.r.t. PV of evt"),
            NTupleVariable("absIP3DDerr", lambda ev: ev.absIP3DD[1], help="Error of absIP3DDval"),
            
            #Of one lepton w.r.t. the PV of the PV of the other leptons only
            NTupleVariable("absIP3DApvBCval", lambda ev: ev.absIP3DApvBC[0], help="Absolute IP of A w.r.t. PV of BC"),
            NTupleVariable("absIP3DApvBCerr", lambda ev: ev.absIP3DApvBC[1], help="Error of absIP3DApvBCval"),
            NTupleVariable("absIP3DBpvACval", lambda ev: ev.absIP3DBpvAC[0], help="Absolute IP of B w.r.t. PV of AC"),
            NTupleVariable("absIP3DBpvACerr", lambda ev: ev.absIP3DBpvAC[1], help="Error of absIP3DBpvACval"),
            NTupleVariable("absIP3DCpvABval", lambda ev: ev.absIP3DCpvAB[0], help="Absolute IP of C w.r.t. PV of AB"),
            NTupleVariable("absIP3DCpvABerr", lambda ev: ev.absIP3DCpvAB[1], help="Error of absIP3DCpvABval"),

            NTupleVariable("absIP3DApvBCDval", lambda ev: ev.absIP3DApvBCD[0], help="Absolute IP of A w.r.t. PV of BCD"),
            NTupleVariable("absIP3DApvBCDerr", lambda ev: ev.absIP3DApvBCD[1], help="Error of absIP3DApvBCDval"),
            NTupleVariable("absIP3DBpvACDval", lambda ev: ev.absIP3DBpvACD[0], help="Absolute IP of B w.r.t. PV of ACD"),
            NTupleVariable("absIP3DBpvACDerr", lambda ev: ev.absIP3DBpvACD[1], help="Error of absIP3DBpvACDval"),
            NTupleVariable("absIP3DCpvABDval", lambda ev: ev.absIP3DCpvABD[0], help="Absolute IP of C w.r.t. PV of ABD"),
            NTupleVariable("absIP3DCpvABDerr", lambda ev: ev.absIP3DCpvABD[1], help="Error of absIP3DCpvABDval"),
            NTupleVariable("absIP3DDpvABCval", lambda ev: ev.absIP3DDpvABC[0], help="Absolute IP of D w.r.t. PV of ABC"),
            NTupleVariable("absIP3DDpvABCerr", lambda ev: ev.absIP3DDpvABC[1], help="Error of absIP3DDpvABCval"),
            
            ##Variables related to chi2
            #Chi2 of all the good leptons of the event but one lepton 
            NTupleVariable("chi2pvtrksBCbutAval", lambda ev: ev.chi2pvtrksBCbutA[0], help="chi2 of the fit to the PV of B,C"),
            NTupleVariable("chi2pvtrksBCbutAdof", lambda ev: ev.chi2pvtrksBCbutA[1], help="DOF from the fit to the PV of B,C"),
            NTupleVariable("chi2pvtrksACbutBval", lambda ev: ev.chi2pvtrksACbutB[0], help="chi2 of the fit to the PV of A,C"),
            NTupleVariable("chi2pvtrksACbutBdof", lambda ev: ev.chi2pvtrksACbutB[1], help="DOF from the fit to the PV of A,C"),
            NTupleVariable("chi2pvtrksABbutCval", lambda ev: ev.chi2pvtrksABbutC[0], help="chi2 of the fit to the PV of A,B"),
            NTupleVariable("chi2pvtrksABbutCdof", lambda ev: ev.chi2pvtrksABbutC[1], help="DOF from the fit to the PV of A,B"),

            NTupleVariable("chi2pvtrksBCDbutAval", lambda ev: ev.chi2pvtrksBCDbutA[0], help="chi2 of the fit to the PV of B,C,D"),
            NTupleVariable("chi2pvtrksBCDbutAdof", lambda ev: ev.chi2pvtrksBCDbutA[1], help="DOF from the fit to the PV of B,C,D"),
            NTupleVariable("chi2pvtrksACDbutBval", lambda ev: ev.chi2pvtrksACDbutB[0], help="chi2 of the fit to the PV of A,C,D"),
            NTupleVariable("chi2pvtrksACDbutBdof", lambda ev: ev.chi2pvtrksACDbutB[1], help="DOF from the fit to the PV of A,C,D"),
            NTupleVariable("chi2pvtrksABDbutCval", lambda ev: ev.chi2pvtrksABDbutC[0], help="chi2 of the fit to the PV of A,B,D"),
            NTupleVariable("chi2pvtrksABDbutCdof", lambda ev: ev.chi2pvtrksABDbutC[1], help="DOF from the fit to the PV of A,B,D"),
            NTupleVariable("chi2pvtrksABCbutDval", lambda ev: ev.chi2pvtrksABCbutD[0], help="chi2 of the fit to the PV of A,B,C"),
            NTupleVariable("chi2pvtrksABCbutDdof", lambda ev: ev.chi2pvtrksABCbutD[1], help="DOF from the fit to the PV of A,B,C"),

        ]

        self.globalObjects.update({
            # put more here
        })

        self.collections.update({
            # put more here
            "gentopquarks"    : NTupleCollection("GenTop",     genParticleType, 2, help="Generated top quarks from hard scattering"),
            "genbquarks"      : NTupleCollection("GenBQuark",  genParticleType, 2, help="Generated bottom quarks from top quark decays"),
            "genwzquarks"     : NTupleCollection("GenQuark",   genParticleWithSourceType, 6, help="Generated quarks from W/Z decays"),
            ##--------------------------------------------------                                                                                                                                   
            "selectedTaus"    : NTupleCollection("TauGood", tauTypeSusy, 3, help="Taus after the preselection"),
            "otherLeptons"    : NTupleCollection("LepOther", leptonTypeSusyExtra, 8, help="Leptons after the preselection"),
            "selectedLeptons" : NTupleCollection("LepGood", leptonTypeSusyExtra, 8, help="Leptons after the preselection"),
            ##------------------------------------------------
            "cleanJets"       : NTupleCollection("Jet",     jetTypeSusy, 8, help="Cental jets after full selection and cleaning, sorted by pt"),
            "cleanJetsFwd"    : NTupleCollection("JetFwd",  jetTypeSusy, 4, help="Forward jets after full selection and cleaning, sorted by pt"),            
            ##------------------------------------------------
            "ivf"       : NTupleCollection("SV",     svType, 20, help="SVs from IVF"),
            "genBHadrons"  : NTupleCollection("GenBHad", heavyFlavourHadronType, 20, mcOnly=True, help="Gen-level B hadrons"),
            "genDHadrons"  : NTupleCollection("GenDHad", heavyFlavourHadronType, 20, mcOnly=True, help="Gen-level D hadrons"),
        })

        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyMultilepton":
            self.initDone = True
            self.declareVariables()

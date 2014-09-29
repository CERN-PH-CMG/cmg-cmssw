from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusyAlphaT( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyAlphaT,self).__init__(cfg_ana, cfg_comp, looperName)
        
        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [


            #NTupleVariable("crossSection", lambda ev : ev.crossSection, help="process cross section in pb"),


            # Energy sums
            #----------------------------------------

            NTupleVariable("ht",           lambda ev : ev.htJet40j,    help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("deltaPhiMin",  lambda ev : ev.deltaPhiMin_had, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("diffMetMht",   lambda ev : ev.diffMetMht_had,   help="abs( vec(mht) - vec(met) )"),
            NTupleVariable("mht_pt",       lambda ev : ev.mhtJet40j,    help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("mht_phi",      lambda ev : ev.mhtPhiJet40j, help="H_{T}^{miss} #phi computed from onlyy jets (with |eta|<2.5, pt > 40 GeV)"),
            ##--------------------------------------------------

            # Physics object multplicities
            #----------------------------------------
            NTupleVariable("nMuons10",     lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.selectedLeptons]), int, help="Number of muons with pt > 10"),
            NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.selectedLeptons]), int, help="Number of electrons with pt > 10"),
            NTupleVariable("nTaus20",      lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
            NTupleVariable("nGammas20",    lambda ev: sum([l.pt() > 20 for l in ev.selectedPhotons]), int, help="Number of photons with pt > 20"),
            NTupleVariable("nBJet40",      lambda ev: sum([j.btagWP("CSVM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),

            # AlphaT variables
            #----------------------------------------
            
            # NEED TO ADD:
            # - M_{T}
            # - metcorrected for photon, muon
            # - mht/metcorrected

            # NTupleVariable("nJet50",        lambda ev: len(ev.cleanJets), int, help="Number of jets with pt > 50, |eta|<3"),
            # NTupleVariable("nJetFwd50",     lambda ev: len(ev.cleanJetsFwd), int, help="Number of jets with pt > 50, 3<|eta|<5"),
            # NTupleVariable("nBJetMedium50", lambda ev: len(ev.bjetsMedium), int, help="Number of jets with pt > 50, |eta|<3 passing CSV medium"),
            # NTupleVariable("htJet50",       lambda ev: ev.htJet50,     help="H_{T} computed from jets (with |eta|<3, pt > 50 GeV)"),
            # NTupleVariable("mhtJet50",      lambda ev: ev.mhtJet50,    help="H_{T}^{miss} computed from jets (with |eta|<3, pt > 50 GeV)"),
            # NTupleVariable("mht50OverMet",  lambda ev: ev.mht50OverMet,help="H_{T}^{miss}/E_{T}^{miss} computed from jets (with |eta|<3, pt > 50 GeV)"),


            NTupleVariable("alphaT",        lambda ev: ev.alphaT, help="AlphaT computed using jets with pt > 50, |eta|<3"),

            # # NTupleVariable("mT", lambda ev: ev.mT, help="M_{T} computed from lepton and type-1 corrected PFMET"),

            # NTupleVariable("mZ1",           lambda ev: ev.bestZ1[0], help="Best m(ll) SF/OS"),
            # NTupleVariable("pt2l",          lambda ev: ev.pt2l,      help="p_{T}(ll)"),
            # NTupleVariable("m2l",           lambda ev: ev.m2l,       help="m(ll)"),

            ##--------------------------------------------------
            #            NTupleVariable("mtw", lambda ev: ev.mtw, int, help="mt(l,met)"),
            #            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, int, help="mt(tau,met)"),
            #            NTupleVariable("IsoTrack_mtw", lambda ev: ev.mtwIsoTrack, int, help="mt(isoTrack,met)"),
            ##--------------------------------------------------
            #            NTupleVariable("minMWjj", lambda ev: ev.minMWjj, int, help="minMWjj"),
            #            NTupleVariable("minMWjjPt", lambda ev: ev.minMWjjPt, int, help="minMWjjPt"),
            #            NTupleVariable("bestMWjj", lambda ev: ev.bestMWjj, int, help="bestMWjj"),
            #            NTupleVariable("bestMWjjPt", lambda ev: ev.bestMWjjPt, int, help="bestMWjjPt"),
            #            NTupleVariable("bestMTopHad", lambda ev: ev.bestMTopHad, int, help="bestMTopHad"),
            #            NTupleVariable("bestMTopHadPt", lambda ev: ev.bestMTopHadPt, int, help="bestMTopHadPt"),
            ##--------------------------------------------------
            ]
        
        self.globalObjects.update({
            # put more here
            # "pseudoJet1"       : NTupleObject("pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere"),
            # "pseudoJet2"       : NTupleObject("pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere"),
            })
        self.collections.update({
            # put more here
            "selectedLeptons"  : NTupleCollection("lep",      leptonTypeSusy,           50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
            "selectedTaus"     : NTupleCollection("tau",      tauTypeSusy,              50, help="Taus after the preselection"),
            "cleanJetsAll"     : NTupleCollection("jet",      jetTypeSusy,             100, help="all jets (w/ x-cleaning, w/ ID applied w/o PUID applied pt > 50 |eta| < 3) , sorted by pt", filter=lambda l : l.pt()>50  ),
            "selectedPhotons"  : NTupleCollection("gamma",    photonTypeSusy,           50, help="photons with pt > 25 and loose cut based ID"),
            "selectedIsoTrack" : NTupleCollection("isoTrack", isoTrackType,             50, help="isoTrack, sorted by pt"),
            "genParticles"     : NTupleCollection("genPart",  genParticleWithMotherId, 200, help="all pruned genparticles"),
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyAlphaT":
            self.initDone = True
            self.declareVariables()
            

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusyAlphaT( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY ALPHAT
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyAlphaT,self).__init__(cfg_ana, cfg_comp, looperName)
        
        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [


            #NTupleVariable("crossSection", lambda ev : ev.crossSection, help="process cross section in pb"),


            # Energy sums
            #----------------------------------------

            NTupleVariable("ht",           lambda ev : ev.htJet50j,    help="H_{T} computed from only jets (with |eta|<3, pt > 50 GeV)"),
            NTupleVariable("deltaPhiMin",  lambda ev : ev.deltaPhiMin_had, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("diffMetMht",   lambda ev : ev.diffMetMht_had,   help="abs( vec(mht) - vec(met) )"),
            NTupleVariable("mht_pt",       lambda ev : ev.mhtJet50j,    help="H_{T}^{miss} computed from only jets (with |eta|<3.0, pt > 50 GeV)"),
            NTupleVariable("mht_phi",      lambda ev : ev.mhtPhiJet50j, help="H_{T}^{miss} #phi computed from onlyy jets (with |eta|<3.0, pt > 50 GeV)"),
            ##--------------------------------------------------
            NTupleVariable("biasedDPhi",   lambda ev : ev.biasedDPhi, help="biased delta phi"),

            # Physics object multplicities
            #----------------------------------------
            NTupleVariable("nMuons10",     lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.selectedLeptons]), int, help="Number of muons with at least pt > 10"),
            NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.selectedLeptons]), int, help="Number of electrons with at least pt > 10"),
            NTupleVariable("nTaus20",      lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
            NTupleVariable("nGammas25",    lambda ev: sum([l.pt() > 25 for l in ev.selectedPhotons]), int, help="Number of photons with at least pt > 25"),
            NTupleVariable("nBJet50",      lambda ev: sum([j.btagWP("CSVM") for j in ev.cleanJets if j.pt() > 50]), int, help="Number of jets with pt > 50 passing CSV medium"),

            # AlphaT variables
            #----------------------------------------

            NTupleVariable("alphaT",        lambda ev: ev.alphaT, help="AlphaT computed using jets with pt > 50, |eta|<3"),

            # NTupleVariable("mZ1",           lambda ev: ev.bestZ1[0], help="Best m(ll) SF/OS"),

            # control sample variables
            ##--------------------------------------------------
            NTupleVariable("mtw", lambda ev: ev.mtw, help="mt(l,met)"),
            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, help="mt(tau,met)"),
            NTupleVariable("IsoTrack_mtw", lambda ev: ev.mtwIsoTrack, help="mt(isoTrack,met)"),
            NTupleVariable("mll", lambda ev: ev.mll, help="Invariant mass of the two lead leptons"),

            ##--------------------------------------------------
            ]
        
        self.globalObjects.update({
            # put more here
            # "pseudoJet1"       : NTupleObject("pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere"),
            # "pseudoJet2"       : NTupleObject("pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere"),
             "biasedDPhiJet"       : NTupleObject("biasedDPhiJet",     fourVectorType, help="jet closest to missing energy vector"),
             "metNoMu":         NTupleObject("metNoMu",fourVectorType, help="met computed with muom momentum substracted"),
            })
        self.collections.update({
            # put more here
            "selectedLeptons"  : NTupleCollection("lep",      leptonTypeSusy,           50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
            "selectedTaus"     : NTupleCollection("tau",      tauTypeSusy,              50, help="Taus after the preselection"),
            "cleanJetsAll"     : NTupleCollection("jet",      jetTypeSusy,             100, help="all jets (w/ x-cleaning, w/ ID applied w/o PUID applied pt > 50 |eta| < 3) , sorted by pt", filter=lambda l : l.pt()>50  ),
            "selectedPhotons"  : NTupleCollection("gamma",    photonTypeSusy,           50, help="photons with pt > 25 and loose cut based ID"),
            "selectedIsoTrack" : NTupleCollection("isoTrack", isoTrackType,             50, help="isoTrack, sorted by pt"),
            "genParticles"     : NTupleCollection("genPart",  genParticleWithMotherId, 200, help="all pruned genparticles"),

            # add stuff for testing
            "selectedMuons" : NTupleCollection("muon", leptonTypeSusy, 50, help="Muons for testing"),

            # dR jet lep for each lepton
            # not putting in for now, maybe in future
            #"minDeltaRLepJet"  : NTupleCollection("minDeltaRLepJet", double, 50, help="Min deltaR between a lepton and all the jets"),
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyAlphaT":
            self.initDone = True
            self.declareVariables()
            

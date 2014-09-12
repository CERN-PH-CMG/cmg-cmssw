from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusyFullHad( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyFullHad,self).__init__(cfg_ana, cfg_comp, looperName)
        
        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [


            #NTupleVariable("crossSection", lambda ev : ev.crossSection, help="process cross section in pb"),
            NTupleVariable("nBJet40", lambda ev: sum([j.btagWP("CSVM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
            NTupleVariable("ht", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("deltaPhiMin", lambda ev : ev.deltaPhiMin, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            ### MM
            #NTupleVariable("diffMetMht", lambda ev : ev.diffMetMht, help="abs( vec(mht) - vec(met) )"),
            #NTupleVariable("mht_pt", lambda ev : ev.mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            #NTupleVariable("mht_phi", lambda ev : ev.mhtPhiJet40j, help="H_{T}^{miss} #phi computed from onlyy jets (with |eta|<2.5, pt > 40 GeV)"),
            #
            NTupleVariable("diffMetMht_had", lambda ev : ev.diffMetMht_had, help="abs( vec(mht) - vec(met) ) - only jets"),
            NTupleVariable("mht_had_pt", lambda ev : ev.mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("mht_had_phi", lambda ev : ev.mhtPhiJet40j, help="H_{T}^{miss} #phi computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            #
            NTupleVariable("diffMetMht", lambda ev : ev.diffMetMht, help="abs( vec(mht) - vec(met) )"),
            NTupleVariable("mht_pt", lambda ev : ev.mhtJet40, help="H_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("mht_phi", lambda ev : ev.mhtPhiJet40, help="H_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            ###
            ###NTupleVariable("sumEta", lambda ev : ev.weirdAssVar, help="absurd eta sum"),
            ##--------------------------------------------------
            NTupleVariable("nMuons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.selectedLeptons]), int, help="Number of muons with pt > 10"),
            NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.selectedLeptons]), int, help="Number of electrons with pt > 10"),
            NTupleVariable("nTaus20", lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
            NTupleVariable("nGammas20", lambda ev: sum([l.pt() > 20 for l in ev.selectedPhotons]), int, help="Number of photons with pt > 20"),
            ##--------------------------------------------------
#            NTupleVariable("mtw", lambda ev: ev.mtw, int, help="mt(l,met)"),
#            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, int, help="mt(tau,met)"),
#            NTupleVariable("IsoTrack_mtw", lambda ev: ev.mtwIsoTrack, int, help="mt(isoTrack,met)"),
            ##--------------------------------------------------
            NTupleVariable("mt2_had", lambda ev: ev.mt2, float, help="mt2(j1,j2,,met)"),
            NTupleVariable("mt2_gen", lambda ev: ev.mt2_gen, float, help="mt2(j1,j2,,met) with genInfo"),
            ### MM
            NTupleVariable("mt2", lambda ev: ev.fullmt2, float, help="mt2(j1,j2,,met) with leptons"),
            NTupleVariable("gamma_mt2", lambda ev: ev.gamma_mt2, float, help="mt2(j1,j2,,met) with photons added to met"),
            NTupleVariable("zll_mt2", lambda ev: ev.zll_mt2, float, help="mt2(j1,j2,,met) with zll added to met, only hadrons"),
            NTupleVariable("zll_deltaPhiMin", lambda ev : ev.zll_deltaPhiMin, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            ###
            #            NTupleVariable("mt2w", lambda ev: ev.mt2w, float, help="mt2w(l,b,met)"),
            ##--------------------------------------------------
            #            NTupleVariable("minMWjj", lambda ev: ev.minMWjj, int, help="minMWjj"),
            #            NTupleVariable("minMWjjPt", lambda ev: ev.minMWjjPt, int, help="minMWjjPt"),
            #            NTupleVariable("bestMWjj", lambda ev: ev.bestMWjj, int, help="bestMWjj"),
            #            NTupleVariable("bestMWjjPt", lambda ev: ev.bestMWjjPt, int, help="bestMWjjPt"),
            #            NTupleVariable("bestMTopHad", lambda ev: ev.bestMTopHad, int, help="bestMTopHad"),
            #            NTupleVariable("bestMTopHadPt", lambda ev: ev.bestMTopHadPt, int, help="bestMTopHadPt"),
            ##--------------------------------------------------
            ### MM
            # Gamma variables
            NTupleVariable("gamma_nJet40", lambda ev: sum([j.pt() > 40 for j in ev.gamma_cleanJets]), int, help="Number of jets after photon-cleaning with pt > 40, |eta|<2.4"),
            NTupleVariable("gamma_nBJet40", lambda ev: sum([j.btagWP("CSVM") for j in ev.gamma_cleanJets if j.pt() > 40]), int, help="Number jets after photon-cleaning  with pt > 40 passing CSV medium"),
            NTupleVariable("gamma_ht", lambda ev : ev.gamma_htJet40j, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("gamma_deltaPhiMin", lambda ev : ev.gamma_deltaPhiMin, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("gamma_diffMetMht", lambda ev : ev.gamma_diffMetMht, help="abs( vec(mht) - vec(met) )"),
            NTupleVariable("gamma_mht_pt", lambda ev : ev.gamma_mhtJet40, help="H_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("gamma_mht_phi", lambda ev : ev.gamma_mhtPhiJet40, help="H_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("gamma_diffMetMht_had", lambda ev : ev.gamma_diffMetMht_had, help="abs( vec(mht) - vec(met) ) - only jets"),
            NTupleVariable("gamma_mht_had_pt", lambda ev : ev.gamma_mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("gamma_mht_had_phi", lambda ev : ev.gamma_mhtPhiJet40j, help="H_{T}^{miss} #phi computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            # Zll variables
            NTupleVariable("zll_deltaPhiMin", lambda ev : ev.zll_deltaPhiMin, help="minimal deltaPhi between the zll MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("zll_diffMetMht", lambda ev : ev.zll_diffMetMht, help="abs( vec(mht) - vec(met) ) - only jets for mht, jets plus 2 leptons for met"),
            NTupleVariable("zll_mht_pt", lambda ev : ev.zll_mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("zll_mht_phi", lambda ev : ev.zll_mhtPhiJet40j, help="H_{T}^{miss} #phi computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("zll_met_pt", lambda ev : ev.zll_met_pt, help="E_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) + 2 leptons"),
            NTupleVariable("zll_met_phi", lambda ev : ev.zll_met_phi, help="E_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) + 2 leptons"),
            NTupleVariable("zll_invmass", lambda ev : ev.zll_invmass, help="Invariant mass of di-lepton system"),
            ###
            ]

        
        self.globalObjects.update({
            # put more here
            #"pseudoJet1"       : NTupleObject("pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere"),
            #"pseudoJet2"       : NTupleObject("pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere"),
            "full_pseudoJet1"       : NTupleObject("pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere"),
            "full_pseudoJet2"       : NTupleObject("pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere"),
            ### MM
            #"gamma_met" : NTupleObject("gamma_met", metType, help="PF E_{T}^{miss}, plus photon, after type 1 corrections"),
            "gamma_met" : NTupleObject("gamma_met", fourVectorType, help="PF E_{T}^{miss}, plus photon, after type 1 corrections"), 
            "gamma_metNoPU" : NTupleObject("gamma_metNoPU", fourVectorType, help="PF noPU E_{T}^{miss}, plus photon"),
            #
            "pseudoJet1"       : NTupleObject("pseudoJet1_had",     fourVectorType, help="full pseudoJet1 for hemishphere, including leptons"),
            "pseudoJet2"       : NTupleObject("pseudoJet2_had",     fourVectorType, help="full pseudoJet2 for hemishphere, including leptons"),
            #
            "gamma_pseudoJet1"       : NTupleObject("gamma_pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere, with photon addition"),
            "gamma_pseudoJet2"       : NTupleObject("gamma_pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere, with photon addition"),
            ###
            })
        self.collections.update({
            # put more here
            "selectedLeptons" : NTupleCollection("lep", leptonTypeSusy, 50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
            "selectedTaus"    : NTupleCollection("tau", tauTypeSusy, 50, help="Taus after the preselection"),
            "cleanJetsAll"       : NTupleCollection("jet",     jetTypeSusy, 100, help="all jets (w/ x-cleaning, w/ ID applied w/o PUID applied pt>10 |eta|<5.2) , sorted by pt", filter=lambda l : l.pt()>10  ),
            "selectedPhotons"    : NTupleCollection("gamma", photonTypeSusy, 50, help="photons with pt>20 and loose cut based ID"),
            "selectedIsoTrack"    : NTupleCollection("isoTrack", isoTrackType, 50, help="isoTrack, sorted by pt"),
            "genParticles" : NTupleCollection("genPart", genParticleWithMotherId, 200, help="all pruned genparticles"),
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyFullHad":
            self.initDone = True
            self.declareVariables()
            

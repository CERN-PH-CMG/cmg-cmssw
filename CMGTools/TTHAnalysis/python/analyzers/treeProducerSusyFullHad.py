from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusyFullHad( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyFullHad,self).__init__(cfg_ana, cfg_comp, looperName)
        
        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [

            ##--------------------------------------------------
            ## Generator information
            ##--------------------------------------------------
            NTupleVariable("genQScale", lambda ev : ev.genQScale, help="Generator level binning quantity, QScale"),

            ##--------------------------------------------------
            ## energy sums
            ##--------------------------------------------------
            NTupleVariable("ht", lambda ev : ev.htJet40j10l5t, help="H_{T} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons and muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("mht_pt", lambda ev : ev.mhtJet40j10l5t, help="H_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("mht_phi", lambda ev : ev.mhtPhiJet40j10l5t, help="H_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("diffMetMht", lambda ev : ev.diffMetMht, help="abs( vec(mht) - vec(met) ) - with jets and leptons"),
            NTupleVariable("deltaPhiMin", lambda ev : ev.deltaPhiMin, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4 and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),

            ### for now store the hadronic only
            NTupleVariable("ht_had", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("mht_had_pt", lambda ev : ev.mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("mht_had_phi", lambda ev : ev.mhtPhiJet40j, help="H_{T}^{miss} #phi computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("diffMetMht_had", lambda ev : ev.diffMetMht_had, help="abs( vec(mht) - vec(met) ) - only jets"),
            NTupleVariable("deltaPhiMin_had", lambda ev : ev.deltaPhiMin_had, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),

#            NTupleVariable("tkmet_pt", lambda ev : ev.tkMet.pt(), help="TK E_{T}^{miss}"),
#            NTupleVariable("tkmet_phi", lambda ev : ev.tkMet.phi(), help="TK E_{T}^{miss}"),

            ##--------------------------------------------------
            # Physics object multplicities
            ##--------------------------------------------------
            NTupleVariable("nBJet40", lambda ev: sum([j.btagWP("CSVM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
            NTupleVariable("nMuons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.selectedLeptons]), int, help="Number of muons with pt > 10"),
            NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.selectedLeptons]), int, help="Number of electrons with pt > 10"),
            NTupleVariable("nTaus20", lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
            NTupleVariable("nGammas20", lambda ev: sum([l.pt() > 20 for l in ev.selectedPhotons]), int, help="Number of photons with pt > 20"),
            ##--------------------------------------------------
#            NTupleVariable("mtw", lambda ev: ev.mtw, int, help="mt(l,met)"),
#            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, int, help="mt(tau,met)"),
#            NTupleVariable("IsoTrack_mtw", lambda ev: ev.mtwIsoTrack, int, help="mt(isoTrack,met)"),
#            NTupleVariable("mt2w", lambda ev: ev.mt2w, float, help="mt2w(l,b,met)"),

            NTupleVariable("minMTBMet", lambda ev: ev.minMTBMet, float, help="min Mt(b,met)"),
            NTupleVariable("nPFLep5LowMT", lambda ev: ev.nPFLep5LowMT, int, help="number of PF leptons (e,mu) with pt > 5, reliso < 0.2, MT < 100 "),
            NTupleVariable("nPFHad10LowMT", lambda ev: ev.nPFHad10LowMT, int, help="number of PF hadrons with pt > 10, reliso < 0.1, MT < 100 "),
            ##--------------------------------------------------
            # MT2
            ##--------------------------------------------------
            NTupleVariable("mt2_had", lambda ev: ev.mt2_had, float, help="mt2(j1,j2,met) with jets "),
            NTupleVariable("mt2ViaKt_had", lambda ev: ev.mt2ViaKt_had, float, help="mt2(j1,j2,met) with jets with KT pseudo jets"),
            NTupleVariable("mt2_bb", lambda ev: ev.mt2bb, float, help="mt2(b1,b2,met) with jets "),
            NTupleVariable("mt2_gen", lambda ev: ev.mt2_gen, float, help="mt2(j1,j2,met) with jets at genInfo"),
            NTupleVariable("mt2", lambda ev: ev.mt2, float, help="mt2(j1,j2,met) with jets and leptons"),
            NTupleVariable("gamma_mt2", lambda ev: ev.gamma_mt2, float, help="mt2(j1,j2,met) with photons added to met"),
            NTupleVariable("zll_mt2", lambda ev: ev.zll_mt2, float, help="mt2(j1,j2,met) with zll added to met, only hadrons"),
            ##--------------------------------------------------
            #            NTupleVariable("minMWjj", lambda ev: ev.minMWjj, int, help="minMWjj"),
            #            NTupleVariable("minMWjjPt", lambda ev: ev.minMWjjPt, int, help="minMWjjPt"),
            #            NTupleVariable("bestMWjj", lambda ev: ev.bestMWjj, int, help="bestMWjj"),
            #            NTupleVariable("bestMWjjPt", lambda ev: ev.bestMWjjPt, int, help="bestMWjjPt"),
            #            NTupleVariable("bestMTopHad", lambda ev: ev.bestMTopHad, int, help="bestMTopHad"),
            #            NTupleVariable("bestMTopHadPt", lambda ev: ev.bestMTopHadPt, int, help="bestMTopHadPt"),

            ##--------------------------------------------------
            # Gamma variables
            ##--------------------------------------------------
            NTupleVariable("gamma_nJet40", lambda ev: sum([j.pt() > 40 for j in ev.gamma_cleanJets]), int, help="Number of jets after photon-cleaning with pt > 40, |eta|<2.4"),
            NTupleVariable("gamma_nBJet40", lambda ev: sum([j.btagWP("CSVM") for j in ev.gamma_cleanJets if j.pt() > 40]), int, help="Number jets after photon-cleaning  with pt > 40 passing CSV medium"),
            NTupleVariable("gamma_ht", lambda ev : ev.gamma_htJet40j, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("gamma_deltaPhiMin", lambda ev : ev.gamma_deltaPhiMin_had, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("gamma_diffMetMht", lambda ev : ev.gamma_diffMetMht_had, help="abs( vec(mht) - vec(met) )"),
            NTupleVariable("gamma_mht_pt", lambda ev : ev.gamma_mhtJet40j, help="H_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("gamma_mht_phi", lambda ev : ev.gamma_mhtPhiJet40j, help="H_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
            NTupleVariable("gamma_minMTBMet", lambda ev : ev.gamma_minMTBMet, help="min Mt(b,met)"),

            ##--------------------------------------------------
            # Zll variables
            ##--------------------------------------------------
            NTupleVariable("zll_deltaPhiMin", lambda ev : ev.zll_deltaPhiMin, help="minimal deltaPhi between the zll MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("zll_diffMetMht", lambda ev : ev.zll_diffMetMht, help="abs( vec(mht) - vec(met) ) - only jets for mht, jets plus 2 leptons for met"),
            NTupleVariable("zll_mht_pt", lambda ev : ev.zll_mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("zll_mht_phi", lambda ev : ev.zll_mhtPhiJet40j, help="H_{T}^{miss} #phi computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("zll_met_pt", lambda ev : ev.zll_met_pt, help="E_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) + 2 leptons"),
            NTupleVariable("zll_met_phi", lambda ev : ev.zll_met_phi, help="E_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) + 2 leptons"),
            NTupleVariable("zll_ht", lambda ev: ev.zll_ht, float, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("zll_pt", lambda ev : ev.zll_p4.Pt(), help="Pt of di-lepton system"),
            NTupleVariable("zll_eta", lambda ev : ev.zll_p4.Eta(), help="Eta of di-lepton system"),
            NTupleVariable("zll_phi", lambda ev : ev.zll_p4.Phi(), help="Phi of di-lepton system"),
            NTupleVariable("zll_mass", lambda ev : ev.zll_p4.M(), help="Invariant mass of di-lepton system"),
            ###
            ]

        
        self.globalObjects.update({
            # put more here
            "pseudoJet1_had"       : NTupleObject("pseudoJet1_had",     fourVectorType, help="full pseudoJet1 for hemishphere"),
            "pseudoJet2_had"       : NTupleObject("pseudoJet2_had",     fourVectorType, help="full pseudoJet2 for hemishphere"),
            #
            "pseudoJet1"       : NTupleObject("pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere, including leptons"),
            "pseudoJet2"       : NTupleObject("pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere, including leptons"),
            #
            "pseudoViaKtJet1_had"       : NTupleObject("pseudoViaKtJet1_had",     fourVectorType, help="full pseudoJet1 for hemishphere via KT"),
            "pseudoViaKtJet2_had"       : NTupleObject("pseudoViaKtJet2_had",     fourVectorType, help="full pseudoJet2 for hemishphere via KT"),
            #
            "gamma_met" : NTupleObject("gamma_met", fourVectorType, help="PF E_{T}^{miss}, plus photon, after type 1 corrections"), 
            "gamma_metNoPU" : NTupleObject("gamma_metNoPU", fourVectorType, help="PF noPU E_{T}^{miss}, plus photon"),
            #
            "gamma_pseudoJet1"       : NTupleObject("gamma_pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere, with photon addition"),
            "gamma_pseudoJet2"       : NTupleObject("gamma_pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere, with photon addition"),
            ###
            })
        self.collections.update({
            # put more here
            "gennus"         : NTupleCollection("genNu",     genParticleWithSourceType, 10, help="Generated neutrinos (nue/numu/nutau) from W/Z decays"),
            "selectedLeptons" : NTupleCollection("lep", leptonTypeSusy, 50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
            "selectedTaus"    : NTupleCollection("tau", tauTypeSusy, 50, help="Taus after the preselection"),
            "cleanJetsAll"       : NTupleCollection("jet",     jetTypeSusy, 100, help="all jets (w/ x-cleaning, w/ ID applied w/o PUID applied pt>10 |eta|<5.2) , sorted by pt", filter=lambda l : l.pt()>10  ),
            "fatJets"         : NTupleCollection("fatJet", fatJetType, 15, help="Cental jets after full selection and cleaning, sorted by pt"),
            "selectedPhotons"    : NTupleCollection("gamma", photonTypeSusy, 50, help="photons with pt>20 and loose cut based ID"),
            "selectedIsoTrack"    : NTupleCollection("isoTrack", isoTrackType, 50, help="isoTrack, sorted by pt"),
            "genParticles" : NTupleCollection("genPart", genParticleWithMotherId, 200, help="all pruned genparticles"),
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyFullHad":
            self.initDone = True
            self.declareVariables()
            

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
            NTupleVariable("ht", lambda ev : ev.htJet40jc, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("deltaPhiMin", lambda ev : ev.deltaPhiMin, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),
            NTupleVariable("diffMetMht", lambda ev : ev.diffMetMht, help="abs( vec(mht) - vec(met) )"),
            NTupleVariable("mht_pt", lambda ev : ev.mhtJet40jc, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
            NTupleVariable("mht_phi", lambda ev : ev.mhtPhiJet40jc, help="H_{T}^{miss} #phi computed from onlyy jets (with |eta|<2.5, pt > 40 GeV)"),
            ##--------------------------------------------------
            NTupleVariable("nMuons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.inclusiveLeptons]), int, help="Number of muons with pt > 10"),
            NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.inclusiveLeptons]), int, help="Number of electrons with pt > 10"),
            NTupleVariable("nTaus20", lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
            NTupleVariable("nGammas20", lambda ev: sum([l.pt() > 20 for l in ev.selectedPhotons]), int, help="Number of photons with pt > 20"),
            ##--------------------------------------------------
#            NTupleVariable("mtw", lambda ev: ev.mtw, int, help="mt(l,met)"),
#            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, int, help="mt(tau,met)"),
#            NTupleVariable("IsoTrack_mtw", lambda ev: ev.mtwIsoTrack, int, help="mt(isoTrack,met)"),
            ##--------------------------------------------------
            NTupleVariable("mt2", lambda ev: ev.mt2, float, help="mt2(j1,j2,,met)"),
            NTupleVariable("mt2_gen", lambda ev: ev.mt2_gen, float, help="mt2(j1,j2,,met) with genInfo"),
            #            NTupleVariable("mt2w", lambda ev: ev.mt2w, float, help="mt2w(l,b,met)"),
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
            "pseudoJet1"       : NTupleObject("pseudoJet1",     fourVectorType, help="pseudoJet1 for hemishphere"),
            "pseudoJet2"       : NTupleObject("pseudoJet2",     fourVectorType, help="pseudoJet2 for hemishphere"),
            })
        self.collections.update({
            # put more here
            "selectedLeptons" : NTupleCollection("lep", leptonTypeSusy, 50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
            "selectedTaus"    : NTupleCollection("tau", tauTypeSusy, 50, help="Taus after the preselection"),
            "cleanJetsAll"       : NTupleCollection("jet",     jetTypeSusy, 100, help="all jets after full selection and cleaning, sorted by pt", filter=lambda j : j.jetID("POG_PFID")>0),
            "selectedPhotons"    : NTupleCollection("gamma", photonTypeSusy, 50, help="photons with pt>20 and loose cut based ID"),
            "selectedIsoTrack"    : NTupleCollection("isoTrack", isoTrackType, 50, help="isoTrack, sorted by pt"),
            "genParticles" : NTupleCollection("genPart", genParticleWithMotherId, 200, help="all pruned genparticles"),
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyFullHad":
            self.initDone = True
            self.declareVariables()
            

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *

class treeProducerSusyFullHad( treeProducerSusyCore ):

    #-----------------------------------
    # TREE PRODUCER FOR SUSY MULTILEPTONS
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(treeProducerSusyFullHad,self).__init__(cfg_ana, cfg_comp, looperName)
        
        ## Declare what we want to fill (in addition to susy core ones)
        self.globalVariables += [

            NTupleVariable("ht", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("mht_pt", lambda ev : ev.mhtJet40j, help="H_{T}^{miss} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
            NTupleVariable("mht_phi", lambda ev : ev.mhtPhiJet40j, help="H_{T}^{miss} #phi computed from onlyy jets (with |eta|<2.4, pt > 40 GeV)"),
            ##--------------------------------------------------
            NTupleVariable("nMuons10", lambda ev: sum([l.pt() > 10 for l in ev.selectedMuons]), int, help="Number of muons with pt > 10"),
            NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 for l in ev.selectedElectrons]), int, help="Number of electrons with pt > 10"),
            ##--------------------------------------------------
            NTupleVariable("mtw", lambda ev: ev.mtw, int, help="mt(l,met)"),
            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, int, help="mt(tau,met)"),
#            NTupleVariable("IsoTrack_mtw", lambda ev: ev.mtwIsoTrack, int, help="mt(isoTrack,met)"),
            NTupleVariable("IsoTrack_pt", lambda ev: ev.ptIsoTrack, int, help="pt(most isolated Track)"),
            NTupleVariable("IsoTrack_relIso", lambda ev: ev.isoIsoTrack, int, help="relIso (iso isolated Track)"),
            NTupleVariable("IsoTrack_dz", lambda ev: ev.dzIsoTrack, int, help="dz(iso isolated Track, PV)"),
            NTupleVariable("IsoTrack_pdgId", lambda ev: ev.typeIsoTrack, int, help="PFID (iso isolated Track)"),
            ##--------------------------------------------------
            NTupleVariable("mt2", lambda ev: ev.mt2, float, help="mt2(l,met)"),
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
            "pseudojet1"       : NTupleObject("pseudojet1",     fourVectorType, help="pseudojet1 for hemishphere"),
            "pseudojet2"       : NTupleObject("pseudojet2",     fourVectorType, help="pseudojet2 for hemishphere"),

            })
        self.collections.update({
            # put more here
            "cleanJetsAll"       : NTupleCollection("Jet",     jetTypeSusy, 8, help="Jets after full selection and cleaning, sorted by pt"),
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyFullHad":
            self.initDone = True
            self.declareVariables()
            

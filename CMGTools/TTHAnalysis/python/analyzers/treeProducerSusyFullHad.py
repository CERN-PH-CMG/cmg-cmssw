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
            NTupleVariable("mtw", lambda ev: ev.mtw, int, help="mt(l,met)"),
            NTupleVariable("mtwTau", lambda ev: ev.mtwTau, int, help="mt(tau,met)"),
            NTupleVariable("IsoTrackmtw", lambda ev: ev.mtwIsoTrack, int, help="mt(isoTrack,met)"),
            NTupleVariable("IsoTrackPt", lambda ev: ev.ptIsoTrack, int, help="pt(most isolated Track)"),
            NTupleVariable("IsoTrackRelIso", lambda ev: ev.isoIsoTrack, int, help="relIso (iso isolated Track)"),
            NTupleVariable("IsoTrackDz", lambda ev: ev.dzIsoTrack, int, help="relIso (iso isolated Track)"),
            NTupleVariable("IsoTrackType", lambda ev: ev.typeIsoTrack, int, help="relIso (iso isolated Track)"),
            ##--------------------------------------------------
            NTupleVariable("mt2", lambda ev: ev.mt2, int, help="mt2(l,met)"),
            NTupleVariable("mt2w", lambda ev: ev.mt2w, int, help="mt2w(l,b,met)"),
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
            })
        self.collections.update({
            # put more here
            })
        
        ## Book the variables, but only if we're called explicitly and not through a base class
        if cfg_ana.name == "treeProducerSusyFullHad":
            self.initDone = True
            self.declareVariables()
            

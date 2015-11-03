from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.TTHAnalysis.tools.mvaTool import *

class FinalMVA_2LSS_2:
    def __init__(self, name, varset, algo, training, rarity=False):
        self._MVAs = {}
        if varset == "old":
            self._vars = [  
                MVAVar("lep2AbsEta := abs(LepGood_eta[1])", func = lambda ev : abs(ev.LepGood_eta[1]) if ev.nLepGood >= 2 else 0),
                MVAVar("lep2Pt := min(LepGood_pt[1], 200)", func = lambda ev : ev.LepGood_pt[1]       if ev.nLepGood >= 2 else 0),
                MVAVar("mhtJet25 := min(mhtJet25, 300)"),
                MVAVar("mindr_lep2_jet"),
                MVAVar("MT_met_lep1"),
                MVAVar("htJet25 := min(htJet25, 1000)"),
            ]
            self._MVAs[name] = MVATool("FinalMVA_2LSS_"+algo, training, self._vars, rarity=rarity) 
    def listBranches(self):
        return self._MVAs.keys()
    def __call__(self,event):
        return dict([ (name, mva(event)) for name, mva in self._MVAs.iteritems()])

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.AddFriend("sf/t", argv[2])
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.mvas = {}
            P="/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/finalMVA/2lss/weights/"
            for X in "Nonw", "MVAP_L", "MVAU_L":
                for A in "BDTG" ,"LD", "Likelihood":
                    self.mvas[X+"_"+A] = FinalMVA_2LSS_2(X+"_"+A, "old", A, P+("%s_%s.weights.xml" % (X,A)), rarity=(A=="LD"))
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            for n,x in self.mvas.iteritems():
                print "%-20s: %s" % (n, x(ev).values()[0])
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)


from CMGTools.TTHAnalysis.treeReAnalyzer import *

class SusyVars2LSSInc:
    def __init__(self):
        self.branches = [ "iL1", "iL2",
                          "htJet25je08", "htJet25je12", "htJet25j",
                          "htJet40je08", "htJet40je12"  ]
    def listBranches(self):
        return self.branches[:]
    def __call__(self,event):
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        ret = { 'iL1':0, 'iL2':1 }
        pairs = []
        if len(leps) > 2:
            for il1 in xrange(len(leps)-1):
                for il2 in xrange(il1+1,len(leps)): 
                    l1 = leps[il1]
                    l2 = leps[il2]
                    if l2.pt < 10: break 
                    if l1.charge == l2.charge:
                        flav = abs(l1.pdgId) + abs(l2.pdgId)
                        ht   = l1.pt + l2.pt
                        pairs.append( (-flav,-ht,il1,il2) )
            if len(pairs):
                pairs.sort()
                ret['iL1'] = pairs[0][2]
                ret['iL2'] = pairs[0][3]
        jets = [j for j in Collection(event,"Jet","nJet")]
        ret['htJet25j']    = sum([j.pt for j in jets if (j.pt > 25)]) # missing in input trees
        ret['htJet25je08'] = sum([j.pt for j in jets if (j.pt > 25 and abs(j.eta) < 0.8)])
        ret['htJet25je12'] = sum([j.pt for j in jets if (j.pt > 25 and abs(j.eta) < 1.2)])
        ret['htJet40je08'] = sum([j.pt for j in jets if (j.pt > 40 and abs(j.eta) < 0.8)])
        ret['htJet40je12'] = sum([j.pt for j in jets if (j.pt > 40 and abs(j.eta) < 1.2)])
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = SusyVars2LSSInc()
        def analyze(self,ev):
            if ev.nLepGood <= 2: return True
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
            leps = [l for l in Collection(ev,"LepGood","nLepGood")]
            if len(leps) > 2:
                for i,l in enumerate(leps):
                    print "\t%2d  pdgId: %+2d  pT: %6.2f" % (i,l.pdgId,l.pt)
                print ""
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        

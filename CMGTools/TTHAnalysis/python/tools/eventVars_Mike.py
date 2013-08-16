from CMGTools.TTHAnalysis.treeReAnalyzer import *

class EventVarsMike:
    def __init__(self):
        self.branches = [ "mass_mindr_lepjet","mass_jet_clospair","pt_balance", "pt_balancepluslep" ]
    def listBranches(self):
        return self.branches[:]
    def __call__(self,event):
        # make python lists as Collection does not support indexing in slices
        leps = [l for l in Collection(event,"LepGood","nLepGood",4)]
        jets = [j for j in Collection(event,"Jet","nJet25",8)]
        (met, metphi)  = event.met, event.met_phi
        metp4 = ROOT.TLorentzVector()
        metp4.SetPtEtaPhiM(met,0,metphi,0)
        njet = len(jets); nlep = len(leps)
        # prepare output
        ret = dict([(name,0.0) for name in self.branches])
        # fill output
        if njet >= 2 and nlep >= 2:            
            dr11=deltaR(jets[0],leps[0])
            dr21=deltaR(jets[1],leps[0])
            dr22=deltaR(jets[1],leps[1])
            dr12=deltaR(jets[0],leps[1])
            drs=[(dr11,0,0),(dr21,1,0),(dr22,1,1),(dr12,0,1)]
            drss = sorted(drs)
            ret["mass_mindr_lepjet"]=(leps[drss[0][2]].p4()+jets[drss[0][1]].p4()).M()
            ret["mass_jet_clospair"]=jets[drss[0][1]].mass
            if njet >=4:
                ret["pt_balance"]=((leps[1-drss[0][2]].p4()+jets[1-drss[0][1]].p4()+ metp4).Pt())-(((jets[drss[0][2]].p4())+(jets[2].p4())+(jets[3].p4())).Pt())
                ret["pt_balancepluslep"]=((leps[1-drss[0][2]].p4()+jets[1-drss[0][1]].p4()+ metp4).Pt())-(((jets[drss[0][2]].p4())+(jets[2].p4())+(jets[3].p4())+leps[drss[0][2]].p4()).Pt())
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("ttHLepTreeProducerBase")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVarsMike()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        

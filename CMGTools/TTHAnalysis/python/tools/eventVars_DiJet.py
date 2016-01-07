from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator

class EventVarsDiJet:
    def __init__(self):
        self.branches = [ "deltaPhiMin","deltaPhi",
                           "MT_met_J1", "MT_met_J2","minMT",
                           "HTJet40" ,
                            "J1_b","J2_b","J3_b",
                           "MCT" ]
    def listBranches(self):
        return self.branches[:]
    def __call__(self,event):
        # make python lists as Collection does not support indexing in slices

        leps = [l for l in Collection(event,"lep","nlep",10)]
        jets = [j for j in Collection(event,"jet","njet",100)]
        (met, metphi)  = event.met_pt, event.met_phi
        njet = len(jets); nlep = len(leps)
        # prepare output
        ret = dict([(name,0.0) for name in self.branches])
        centralJet40 = []
        centralJet40idx = []
        for i,j in enumerate(jets):
            if j.pt>40 and abs(j.eta)<2.4:
                centralJet40.append(j)
                centralJet40idx.append(i)
                
        ret['HTJet40'] = sum([j.pt for j in centralJet40])


        J1_bJet = 0
        J2_bJet = 0
        J3_bJet = 0
        for i,j in enumerate(centralJet40):
            if i > 0 and jets[centralJet40idx[0]].btagCSV>0.890: J1_bJet = 1    
            if i > 1 and jets[centralJet40idx[1]].btagCSV>0.890: J2_bJet = 1    
            if i > 2 and jets[centralJet40idx[2]].btagCSV>0.890: J3_bJet = 1    
           
#        print "\nrun %6d lumi %4d event %d: btag %f" % (event.run, event.lumi, event.evt, jets[centralJet40idx[0]].btagCSV)

	ret['J1_b'] = J1_bJet
	ret['J2_b'] = J2_bJet
	ret['J3_b'] = J3_bJet

        njetc = len(centralJet40)  

        if njetc > 1:
           MT_J1 = -999.
           MT_J2 = -999.
           minMT = -999.
           deltaPhiMin_had3 = 999.
           bTagWP = 0.890
           MCT_2b = -999.
           for n,j in enumerate(centralJet40):
               if n>2:  break
               
               thisDeltaPhi = abs( deltaPhi( j.phi, metphi ) )
               if thisDeltaPhi < deltaPhiMin_had3 : deltaPhiMin_had3 = thisDeltaPhi
               MT_J1 =  sqrt( 2*jets[centralJet40idx[0]].pt*met*(1-cos(jets[centralJet40idx[0]].phi-metphi)) )
               MT_J2 =  sqrt( 2*jets[centralJet40idx[1]].pt*met*(1-cos(jets[centralJet40idx[1]].phi-metphi)) )
               if jets[centralJet40idx[0]].btagCSV > bTagWP and jets[centralJet40idx[1]].btagCSV > bTagWP : 
                  MCT_2b = sqrt( 2*jets[centralJet40idx[0]].pt*jets[centralJet40idx[1]].pt*(1+cos(jets[centralJet40idx[0]].phi-jets[centralJet40idx[1]].phi)) )  

           ret["MT_met_J1"] = MT_J1
           ret["MT_met_J2"] = MT_J2
           ret["minMT"] = min(MT_J1,MT_J2)
           ret["deltaPhi"] = abs( deltaPhi( jets[centralJet40idx[0]].phi, jets[centralJet40idx[1]].phi ) )
           ret["deltaPhiMin"] = deltaPhiMin_had3
           ret["MCT"] =MCT_2b



        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVarsDiJet()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nlep)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)



        

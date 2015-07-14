from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator

ROOT.gInterpreter.GenerateDictionary("vector<TLorentzVector>","TLorentzVector.h;vector") #need this to be able to use topness code

mt2wSNT = ROOT.heppy.mt2w_bisect.mt2w()

## TOPNESS SETUP ##
topness = ROOT.Topness.Topness()

## B tag Working points
## For Topness only Tight is used

## CSV (v1) WPs
#topness.bDisc_LooseWP = 0.244
#topness.bDisc_TightWP = 0.679

## CMVA
#topness.bDisc_LooseWP = 0.244
#topness.bDisc_TightWP = 0.762

## CSV v2 (CSV-IVF)
topness.bDisc_LooseWP = 0.423
topness.bDisc_TightWP = 0.814 # CSVv2 Medium WP
#topness.bDisc_TightWP = 0.941 # CSVv2 Tight WP

def getPhysObjectArray(j): # https://github.com/HephySusySW/Workspace/blob/72X-master/RA4Analysis/python/mt2w.py
    px = j.pt*cos(j.phi )
    py = j.pt*sin(j.phi )
    pz = j.pt*sinh(j.eta )
    E = sqrt(px*px+py*py+pz*pz) #assuming massless particles...
    return array.array('d', [E, px, py,pz])

class EventVars1L_extra:
    def __init__(self):
        self.branches = [ "MT2W", "Topness" ,'TopnessTwoCorrectBFromTop']

    def listBranches(self):
        return self.branches[:]

    def __call__(self,event,base):

        # output dict:
        ret = {}

        # get some collections from initial tree
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        jets = [j for j in Collection(event,"Jet","nJet")]

        njet = len(jets); nlep = len(leps)

        # MET
        metp4 = ROOT.TLorentzVector(0,0,0,0)
        metp4.SetPtEtaPhiM(event.met_pt,event.met_eta,event.met_phi,event.met_mass)
        pmiss  =array.array('d',[event.met_pt * cos(event.met_phi), event.met_pt * sin(event.met_phi)] )

        ####################################
        # import output from previous step #
        #base = keyvals
        ####################################

        # get selected leptons
        tightLeps = []
        tightLepsIdx = base['tightLepsIdx']
        tightLeps = [leps[idx] for idx in tightLepsIdx]
        nTightLeps = len(tightLeps)

        # get selected jets
        centralJet30 = []
        centralJet30idx = base['centralJet30idx']
        centralJet30_DescFlag = base['centralJet30_DescFlag']
        centralJet30 = [jets[idx] for idx in centralJet30idx]
        nCentralJet30 = len(centralJet30)

        # B jets
        BJetMedium30 = []
        BJetMedium30idx = base['BJetMedium30idx']
        NonBJetMedium30 = []
        nBJetMedium30 = base['nBJetMedium30']

        for idx,jet in enumerate(jets):
            if idx in BJetMedium30idx:
                BJetMedium30.append(jet)
            else:
                NonBJetMedium30.append(jet)

        ##################################################################
        # The following variables need to be double-checked for validity #
        ##################################################################

        #add topness and mt2W-variable (timing issue with topness: slows down the friend tree production by a factor of ~3)
        mt2w_values=[]

        if nTightLeps>=1: #topness and mt2w only make sense for
            lep = getPhysObjectArray(tightLeps[0])
            if nBJetMedium30==0 and nCentralJet30>=3: #All combinations from the highest three light (or b-) jets
                consideredJets = [ getPhysObjectArray(jet) for jet in NonBJetMedium30[:3] ] # only throw arrays into the permutation business
                ftPerms = itertools.permutations(consideredJets, 2)
                for perm in ftPerms:
                    mt2wSNT.set_momenta(lep, perm[0], perm[1], pmiss)
                    mt2w_values.append(mt2wSNT.get_mt2w())
            elif nBJetMedium30==1 and nCentralJet30>=2: #All combinations from one b and the highest two light jets
                consideredJets = [ getPhysObjectArray(jet) for jet in NonBJetMedium30[:2] ] # only throw arrays into the permutation business
                consideredJets.append(getPhysObjectArray(BJetMedium30[0]))
                ftPerms = itertools.permutations(consideredJets, 2)
                for perm in ftPerms:
                    mt2wSNT.set_momenta(lep, perm[0], perm[1], pmiss)
                    mt2w_values.append(mt2wSNT.get_mt2w())
            elif nBJetMedium30==2:
                consideredJets = [ getPhysObjectArray(jet) for jet in BJetMedium30[:2] ] # only throw arrays into the permutation business
                ftPerms = itertools.permutations(consideredJets, 2)
                for perm in ftPerms:
                    mt2wSNT.set_momenta(lep, perm[0], perm[1], pmiss)
                    mt2w_values.append(mt2wSNT.get_mt2w())
            elif nBJetMedium30>=3: #All combinations from the highest three b jets
                consideredJets = [ getPhysObjectArray(jet) for jet in BJetMedium30[:3] ] # only throw arrays into the permutation business
                ftPerms = itertools.permutations(consideredJets, 2)
                for perm in ftPerms:
                    mt2wSNT.set_momenta(lep, perm[0], perm[1], pmiss)
                    mt2w_values.append(mt2wSNT.get_mt2w())

        if len(mt2w_values)>0:
            ret["MT2W"]=min(mt2w_values)
        else:
            ret["MT2W"]=-999

        # topness
        ret['Topness']=-999
        #topness makes sense for
        if (nTightLeps>=1) and (nCentralJet30>=3) and (nBJetMedium30>=1) :

            p4_jets = std.vector(TLorentzVector)();
            bdisc_jets = std.vector('float')();

            for jet in centralJet30:
                jetTLorentz = ROOT.TLorentzVector(0,0,0,0)
                jetTLorentz.SetPtEtaPhiM(jet.pt, jet.eta, jet.phi, jet.mass)
                p4_jets.push_back(jetTLorentz)
                # select B tag discriminator for topness (WP see above)
                bdisc_jets.push_back(jet.btagCSV)

            lepTLorentz = ROOT.TLorentzVector(0,0,0,0)
            lepTLorentz.SetPtEtaPhiM(tightLeps[0].pt, tightLeps[0].eta, tightLeps[0].phi, tightLeps[0].mass)

            # calc topness
            tempTopness = topness.GetTopness(p4_jets,bdisc_jets,lepTLorentz,metp4) #this is really slow!
            if tempTopness <=0:
                print tempTopness, "this will fail"
            else:
                ret['Topness'] = log(tempTopness) #this is really slow!
            
            #As cross-check: Calculate topness for the two b-jets from the top-event
            
            realBJetsidx = []
            for i,j in enumerate(centralJet30):
                if centralJet30_DescFlag[i] in [-1,1,2]: realBJetsidx.append(i)

            if len(realBJetsidx)==2:
                ret['TopnessTwoCorrectBFromTop'] = log(topness.Calc( p4_jets[0], p4_jets[1], lepTLorentz, metp4))
            else:              
                ret['TopnessTwoCorrectBFromTop'] = -999


        # return branches
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1L()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

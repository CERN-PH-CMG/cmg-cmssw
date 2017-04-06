from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.WMass.tools.standaloneElectronCalibrator import ElectronCalibrator
from CMGTools.WMass.tools.PileUpReWeighter import PileUpReWeighter
import types

class EventVarsWmass:
    def __init__(self):
        ROOT.gSystem.Load("libFWCoreFWLite.so")
        ROOT.gSystem.Load("libDataFormatsFWLite.so")
        ROOT.AutoLibraryLoader.enable()
        ROOT.gSystem.Load("libCMGToolsTTHAnalysis.so")
        self.branches = [ "nMu15T", "nEle25T", "nBTag20", "events_ntot" ]
        self.electronEnergyCalibrator = ElectronCalibrator(False, # isMC = false => never applying the smearings, only scale
                                                           False, 
                                                           1, # use the final momentum estimate after combination 
                                                           0) # don't do any E-p combinations


    def initSample(self,region,sample_nevt):
        self.region = region
        self.sample_nevt = sample_nevt        
    def listBranches(self):
        biglist = [ ("nJetClean", "I"), ("nJetCleanCentral", "I"), ("nLepSel", "I"),
                    ("iL","I",10,"nLepSel"), ("iJ","I",10,"nJetClean") ] 
        for jfloat in "pt eta phi mass btagCSV rawPt leadClean".split():
            biglist.append( ("JetClean"+"_"+jfloat,"F",10,"nJetClean") )
        self.branches = self.branches + biglist
        print "self.branches = ",self.branches[:]
        return self.branches[:]
    def lepIdTight(self,lep):
        if not lep.trgMatch: return False
        if abs(lep.pdgId) == 13:
            if lep.pt <= 15 or abs(lep.pt)>2.1: return False
            return lep.tightId >=1 and lep.relIso04 < 0.15
        elif abs(lep.pdgId) == 11:
            if lep.relIso03 > (0.10 if abs(lep.scEta)<1.479 else 0.10): return False
            return lep.pt > 25 and abs(lep.scEta) < 2.5 and lep.tightId >=3
    def leadJetCleaning(self,jet):
        return jet.CHEF > 0.1 and jet.NHEF < 0.8
    def PtEtaPhi3V(self,pt,eta,phi):
        return ROOT.TVector3(pt*cos(phi),pt*sin(phi),pt*sinh(eta))
    def __call__(self,event):
        # prepare output
        ret = {}; jetret = {};
        ret['events_ntot'] = self.sample_nevt
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        ret['nMu15T'] = sum([(abs(l.pdgId)==13 and int(self.lepIdTight(l))) for l in leps ])
        ret['nEle25T'] = sum([(abs(l.pdgId)==11 and int(self.lepIdTight(l))) for l in leps ])
        jets = [j for j in Collection(event,"Jet","nJet")]
        jetsFwd = [j for j in Collection(event,"JetFwd","nJetFwd")]
        alljets = jets + jetsFwd
        njet = len(jets)

        ### lepton-jet cleaning
        # Define the loose leptons to be cleaned
        ret["iL"] = []
        for il,lep in enumerate(leps):
            #if self.lepIdVeto(lep):
                ret["iL"].append(il)
        ret["nLepSel"] = len(ret["iL"])
        # Define cleaned jets 
        ret["iJ"] = []; 
        # 0. mark each identified jet as clean
        for j in alljets: 
            # remove PU jet ID for the time being
            j._clean = True if j.id > 0.5 else False
            j._central = True if (abs(j.eta) < 2.5) else False
        # 1. associate to each loose lepton its nearest jet 
        for il in ret["iL"]:
            lep = leps[il]
            best = None; bestdr = 0.4
            for j in alljets:
                dr = deltaR(lep,j)
                if dr < bestdr:
                    best = j; bestdr = dr
            if best is not None: best._clean = False
        # 2. compute the jet list
        nJetCleanCentral=0
        nJetCleanFwd=0
        for ij,j in enumerate(alljets):
            if not j._clean: continue
            ret["iJ"].append(ij)
        # 3. sort the jets by pt
        ret["iJ"].sort(key = lambda idx : alljets[idx].pt, reverse = True)
        # 4. compute the variables
        for jfloat in "pt eta phi mass btagCSV rawPt leadClean".split():
            jetret[jfloat] = []
        for idx in ret["iJ"]:
            jet = alljets[idx]
            # only save in the jetClean collection the jets with pt > 30 GeV
            if jet.pt < 30: continue
            if jet._central: nJetCleanCentral += 1
            else: nJetCleanFwd += 1
            for jfloat in "pt eta phi mass btagCSV rawPt".split():
                jetret[jfloat].append( getattr(jet,jfloat) )
            jetret["leadClean"].append( self.leadJetCleaning(jet) )
        ret["nJetClean"] = nJetCleanCentral+nJetCleanFwd
        # 5. compute the sums 
        ret["nJetCleanCentral"] = 0
        ret["nBTag20"] = 0
        lowptjets = []
        for j in jets: # these are all central
            if not j._clean: continue
            if j.pt > 30:
                ret["nJetCleanCentral"] += 1
            if j.pt > 20 and abs(j.eta)<2.4:
                lowptjets.append(j)
                if j.btagCSV > 0.800:
                    ret["nBTag20"] += 1

        # Electron momentum scale corrections
        electrons = [l for l in leps if abs(l.pdgId)==11]
        for el in electrons:
            p4El = ROOT.TLorentzVector()
            p4El.SetPtEtaPhiM(el.pt,el.eta,el.phi,0.51e-3)
            p = p4El.E()
            momentum = max(15., p); #(combinedMomentum<15. ? 15. : combinedMomentum);
            if abs(el.eta)<1.479 :
                parEB = ( 5.24e-02, 2.01e-01, 1.00e-02 );
                combinedMomentumError = momentum * sqrt( pow(parEB[0]/sqrt(momentum),2) + pow(parEB[1]/momentum,2) + pow(parEB[2],2) );
            else:
                parEE = ( 1.46e-01, 9.21e-01, 1.94e-03 ) ;
                combinedMomentumError = momentum * sqrt( pow(parEE[0]/sqrt(momentum),2) + pow(parEE[1]/momentum,2) + pow(parEE[2],2) );
            mySimpleElectron = ROOT.SimpleElectron(event.run, 
                                                   l.classification,
                                                   l.r9,
                                                   l.ecalEnergy,
                                                   l.correctedEcalEnergyError,
                                                   l.superCluster_energy / l.eSuperClusterOverP, # trackMomentumAtVtx,
                                                   l.correctedEcalEnergyError, # dummy, should be trk momentum error, no combination done here
                                                   p,
                                                   l.regressionEnergyError,
                                                   p,
                                                   combinedMomentumError,
                                                   el.scEta,
                                                   abs(el.eta)<1.479,
                                                   1 if event.run==1 else 0,
                                                   1, # dummy, ecalDrivenSeed
                                                   0) # dummy trackerDrivenSeed
            corrP4 = self.electronEnergyCalibrator.getCorrectedP4(mySimpleElectron,p4El,event.run)
            #corrP4.Print()

        ### return
        fullret = {}
        for k,v in ret.iteritems():
            fullret[k] = v
        for k,v in jetret.iteritems():
            fullret["JetClean_%s" % k] = v
        return fullret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVarsWmass()
        def analyze(self,ev):
            #if ev.metNoMu_pt < 200: return True
            print "\nrun %6d lumi %4d event %d: metNoMu %d" % (ev.run, ev.lumi, ev.evt, ev.metNoMu_pt)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)


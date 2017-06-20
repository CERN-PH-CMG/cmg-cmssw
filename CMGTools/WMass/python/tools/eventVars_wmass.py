from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.WMass.tools.standaloneElectronCalibrator import ElectronCalibrator
from CMGTools.WMass.tools.PileUpReWeighter import PileUpReWeighter
import types, os

class SimpleVBoson:
    def __init__(self,legs):
        self.legs = legs
        if len(legs)<2:
            print "ERROR: making a VBoson w/ < 2 legs!"
        self.pt1 = legs[0].Pt()
        self.pt2 = legs[1].Pt()
        self.dphi = self.legs[0].Phi()-self.legs[1].Phi()
        self.deta = self.legs[0].Eta()-self.legs[1].Eta()
        self.px1 = legs[0].Px(); self.py1 = legs[0].Py();
        self.px2 = legs[1].Px(); self.py2 = legs[1].Py();
    def pt(self):
        return sqrt(hypot(self.pt1 + self.pt2 * cos(self.dphi), self.pt2*sin(self.dphi)))
    def mt(self):
        return sqrt(2*self.pt1*self.pt2*(1-cos(self.dphi)))
    def ux(self):
        return (-self.px1-self.px2)
    def uy(self):
        return (-self.py1-self.py2)    
    def mll(self):
        return sqrt(2*self.pt1*self.pt2*(cosh(self.deta)-cos(self.dphi)))

class EventVarsWmass:
    def __init__(self):
        ROOT.gSystem.Load("libFWCoreFWLite.so")
        ROOT.AutoLibraryLoader.enable()
        ROOT.gSystem.Load("libCMGToolsWMass.so")
        self.branches = [ "nBTag20", "events_ntot" ]
    def initSample(self,sample_nevt,dataset):
        self.sample_nevt = sample_nevt
        self.isMC = not any(x in dataset for x in "DoubleMu DoubleEl DoubleEG MuEG MuonEG SingleMu SingleEl".split())
        self.electronEnergyCalibrator = ElectronCalibrator(self.isMC,
                                                           "EgammaAnalysis/ElectronTools/data//WMass_Winter17_ResidualCorrections_ele",
                                                           "%s/src/CMGTools/WMass/python/tools/data/systs_el_scale.txt" % os.environ['CMSSW_BASE'])
    def listBranches(self):
        self.wmass_steps = [x for x in range(0,24,2)] + [x for x in range(24,54,10)] + [x for x in range(54,141,20)]
        biglist = [ ("nLepCorr", "I"), ("iL","I",10,"nLepCorr"),
                    ("nJetClean", "I"), ("iJ","I",10,"nJetClean"),
                    ("w_pt","F"), ("w_mt","F"), ("z_pt","F"), ("z_mll","F"),
                    ("w_ux","F"), ("w_uy","F"), ("z_ux","F"), ("z_uy","F") ]
        for jfloat in "pt eta phi mass btagCSV rawPt leadClean".split():
            biglist.append( ("JetClean"+"_"+jfloat,"F",10,"nJetClean") )
        for lfloat in "eta pt pterr ptScaleUp ptScaleDn".split():
            biglist.append( ("LepCorr"+"_"+lfloat,"F",10,"nLepCorr") ) 
        mclist = [("nWMassSteps", "I"), ("mwWeight","F",2*len(self.wmass_steps)-1,"nWMassSteps")]
        if self.isMC: biglist = biglist + mclist
        self.branches = self.branches + biglist
        print "self.branches = ",self.branches[:]
        return self.branches[:]
    def lepIdTight(self,lep):
        # if not lep.trgMatch: return False
        if abs(lep.pdgId) == 13:
            if lep.pt <= 25 or abs(lep.eta)>2.1: return False
            return lep.tightId >=1 and lep.relIso04 < 0.15
        elif abs(lep.pdgId) == 11:
            if lep.pt <= 25 or abs(lep.eta)>2.5: return False
            return lep.eleMVAId >=2 and lep.relIso03 < 0.15 and lep.convVetoFull==1
    def leadJetCleaning(self,jet):
        return jet.CHEF > 0.1 and jet.NHEF < 0.8
    def PtEtaPhi3V(self,pt,eta,phi):
        return ROOT.TVector3(pt*cos(phi),pt*sin(phi),pt*sinh(eta))
    def BW_weight(self,boson_pdgId,genMass,imass):
        (m0,gamma) = (91.188,2.141) if abs(boson_pdgId)==23 else (80.398,2.085)
        s_hat = pow(genMass,2)
        return (pow(s_hat - m0*m0,2) + pow(gamma*m0,2)) / (pow(s_hat - imass*imass,2) + pow(gamma*imass,2))
    def __call__(self,event):
        # prepare output
        ret = {}; jetret = {}; lepret = {};
        ret['events_ntot'] = self.sample_nevt
        genp = [p for p in Collection(event,"GenP6StatusThree","nGenP6StatusThree")] if self.isMC else []
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        jets = [j for j in Collection(event,"Jet","nJet")]
        jetsFwd = [j for j in Collection(event,"JetFwd","nJetFwd")]
        alljets = jets + jetsFwd
        njet = len(jets)

        tightleps = [l for l in leps if self.lepIdTight(l)]

        ### lepton-jet cleaning
        # Define the loose leptons to be cleaned
        ret["iL"] = []
        for il,lep in enumerate(tightleps):
            ret["iL"].append(il)
        ret["nLepCorr"] = len(ret["iL"])
        # Define cleaned jets 
        ret["iJ"] = [];
        # 0. mark each identified jet as clean
        for j in alljets: 
            # remove PU jet ID for the time being
            j._clean = True if j.id > 0.5 else False
            j._central = True if (abs(j.eta) < 2.5) else False
        # 1. associate to each loose lepton its nearest jet 
        for il in ret["iL"]:
            lep = tightleps[il]
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
        ret["nBTag20"] = 0
        lowptjets = []
        for j in jets: # these are all central
            if not j._clean: continue
            if j.pt > 20 and abs(j.eta)<2.4:
                lowptjets.append(j)
                if j.btagCSV > 0.800:
                    ret["nBTag20"] += 1

        # Lepton residual momentum scale corrections
        for lfloat in "eta pt pterr ptScaleUp ptScaleDn".split():
            lepret[lfloat] = []
        scaleVars=['ptScaleUp','ptScaleDn']; scaleCoeffs=[1.0,-1.0]; 
        for il in ret["iL"]:
            l=tightleps[il]
            if abs(l.pdgId)==11:
                self.electronEnergyCalibrator.correct(l,event.run)
            for lfloat in "eta pt pterr".split():
                lepret[lfloat].append(getattr(l,lfloat) if hasattr(l,lfloat) else 0.0)
            for t in zip(scaleVars,scaleCoeffs):
                syst = 0.0 if abs(l.pdgId)!=11 else self.electronEnergyCalibrator.getOneSigmaScaleFromClosure(l)
                lepret[t[0]].append((1+t[1]*syst)*l.pt)

        # event variables with corrected quantities (METs to be corrected)
        pfmet = self.PtEtaPhi3V(event.met_pt,0.,event.met_phi)
        tkmet = self.PtEtaPhi3V(event.tkmet_pt,0.,event.tkmet_phi)
        tightleps_3v=[ self.PtEtaPhi3V(l.pt,l.eta,l.phi) for l in tightleps ]

        # W,Z candidates
        if len(tightleps)>0:
            W = SimpleVBoson([tightleps_3v[0],tkmet])
            ret["w_pt"] = W.pt()
            ret["w_mt"] = W.mt()
            ret["w_ux"] = W.ux(); ret["w_uy"] = W.uy()
        else: ret["w_pt"] = ret["w_mt"] = ret["w_ux"] = ret["w_uy"] = -999
        if len(tightleps)>1:
            Z = SimpleVBoson(tightleps_3v[:2])
            ret["z_pt"] = Z.pt()
            ret["z_mll"] = Z.mll()
            ret["z_ux"] = Z.ux() - tkmet.Px(); ret["z_uy"] = Z.uy() - tkmet.Py()
        else: ret["z_pt"] = ret["z_mll"] = ret["z_ux"] = ret["z_uy"] = -999

        # W-mass BW weights
        if self.isMC:
            bw_w = []
            genw = [p for p in genp if abs(p.pdgId)==24]
            if len(genw)>0:
                genmass = genw[0].mass
                wmass_central = 80.398
                wmass_steps_full = [-1e-3*x + wmass_central for x in self.wmass_steps[1:]]
                wmass_steps_full += [1e-3*x + wmass_central for x in self.wmass_steps]
                wmass_steps_full.sort()
                bw_w = [self.BW_weight(24,genmass,x) for x in wmass_steps_full]
            ret["nWMassSteps"] = len(bw_w)
            ret["mwWeight"] = bw_w

        ### return
        fullret = {}
        for k,v in ret.iteritems():
            fullret[k] = v
        for k,v in jetret.iteritems():
            fullret["JetClean_%s" % k] = v
        for k,v in lepret.iteritems():
            fullret["LepCorr_%s" % k] = v
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


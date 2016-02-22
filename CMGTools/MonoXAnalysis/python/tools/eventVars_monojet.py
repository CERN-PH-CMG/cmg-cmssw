from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.MonoXAnalysis.tools.PileUpReWeighter import PileUpReWeighter
from PhysicsTools.Heppy.physicsutils.BTagWeightCalculator import BTagWeightCalculator
import types

BTagReweight74X = lambda : BTagWeightCalculator("/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/csv_rwt_fit_hf_2015_11_20.root",
                                                "/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/csv_rwt_fit_lf_2015_11_20.root")

class EventVarsMonojet:
    def __init__(self):
        self.branches = [ "nMu10V", "nMu20T", "nEle10V", "nEle40T", "nTau18V", "nGamma15V", "nGamma175T", "nBTag15",
                          "dphijj", "dphijm", "weight", "events_ntot", "phmet_pt", "phmet_phi","SF_BTag"
                          ]
        btagreweight = BTagReweight74X()
        self._btagreweight = (btagreweight() if type(btagreweight) == types.FunctionType else btagreweight)
        self._btagreweight.btag = "btagCSV"
    def initSampleNormalization(self,sample_nevt):
        self.sample_nevt = sample_nevt        
    def listBranches(self):
        biglist = [ ("nJetClean", "I"), ("nTauClean", "I"), ("nLepSel", "I"),
                    ("iL","I",10,"nLepSel"), ("iJ","I",10,"nJetClean"), ("iT","I",3,"nTauClean"),
                    ("nJetClean30", "I"), ("nTauClean18V", "I") ] 
        for jfloat in "pt eta phi mass btagCSV rawPt leadClean".split():
            biglist.append( ("JetClean"+"_"+jfloat,"F",10,"nJetClean") )
        for tfloat in "pt eta phi".split():
            biglist.append( ("TauClean"+"_"+tfloat,"F",3,"nTauClean") )
        self.branches = self.branches + biglist
        return self.branches[:]
    # physics object multiplicity with the monojet analysis specific selections
    def lepIdVeto(self,lep):
        if lep.pt <= 10: return False
        if abs(lep.pdgId) == 13:
            if abs(lep.eta) > 2.4: return False
            return lep.relIso04 < 0.2
        elif abs(lep.pdgId) == 11:
            if abs(lep.eta) > 2.5: return False
            if lep.relIso03 > (0.126 if abs(lep.etaSc)<1.479 else 0.144): return False
            if lep.dxy > (0.0564 if abs(lep.etaSc)<1.479 else 0.222): return False
            if lep.dz > (0.472 if abs(lep.etaSc)<1.479 else 0.921): return False
            if not lep.convVeto: return False
            return lep.lostHits <= (2 if abs(lep.etaSc)<1.479 else 3)
    def lepIdTight(self,lep):
        if abs(lep.pdgId) == 13:
            if lep.pt <= 20: return False
            return abs(lep.eta) < 2.4 and lep.tightId > 0 and lep.relIso04 < 0.12
        elif abs(lep.pdgId) == 11:
            if lep.pt <= 40: return False
            if lep.relIso03 > (0.0354 if abs(lep.etaSc)<1.479 else 0.0646): return False
            if lep.dxy > (0.0111 if abs(lep.etaSc)<1.479 else 0.0351): return False
            if lep.dz > (0.0466 if abs(lep.etaSc)<1.479 else 0.417): return False
            if lep.lostHits > (2 if abs(lep.etaSc)<1.479 else 1): return False
            return abs(lep.eta) < 2.5 and lep.tightId > 0 and lep.convVeto
    def tauIdVeto(self,tau):
        if tau.pt <= 18 or abs(tau.eta) > 2.3: return False
        return tau.idDecayMode > 0.5 and tau.isoCI3hit < 5.0
    def gammaIdVeto(self,gamma):
        return gamma.pt > 15 and abs(gamma.eta) < 2.5
    def gammaIdTight(self,gamma):
        return gamma.pt > 175 and abs(gamma.eta) < 2.5
    def leadJetCleaning(self,jet):
        return jet.chHEF > 0.1 and jet.neHEF < 0.8
    def metNoPh(self,met,photons):
        px = met.Px() + sum([p.p4().Px() for p in photons])
        py = met.Py() + sum([p.p4().Py() for p in photons])
        ret = ROOT.TVector3()
        ret.SetXYZ(px,py,0.)
        return ret
    def BTagEventReweight(self,jets,rwtKind='final',rwtSyst='nominal',mcOnly=True):
        # for j in jets:
        #     print "    single wgt for jpt=%.3f jeta=%.3f, mcFlav=%d, btag=%.3f, SF=%.3f" % (j.pt, j.eta, j.mcFlavour, j.btagCSV, self._btagreweight.calcJetWeight(j,rwtKind,rwtSyst) )
        return self._btagreweight.calcEventWeight(jets, rwtKind, rwtSyst)
    def __call__(self,event):
        # prepare output
        ret = {}; jetret = {}; tauret = {}
        ret['weight'] = event.xsec * 1000 * event.genWeight / self.sample_nevt if event.run == 1 else 1.0
        ret['events_ntot'] = self.sample_nevt
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        ret['nMu10V'] = sum([(abs(l.pdgId)==13 and int(self.lepIdVeto(l))) for l in leps ])
        ret['nMu20T'] = sum([(abs(l.pdgId)==13 and int(self.lepIdTight(l))) for l in leps ])
        ret['nEle10V'] = sum([(abs(l.pdgId)==11 and int(self.lepIdVeto(l))) for l in leps ])
        ret['nEle40T'] = sum([(abs(l.pdgId)==11 and int(self.lepIdTight(l))) for l in leps ])
        taus = [t for t in Collection(event,"TauGood","nTauGood")]
        ret['nTau18V'] = sum([(int(self.tauIdVeto(t))) for t in taus ])
        photons = [p for p in Collection(event,"GammaGood","nGammaGood")]
        ret['nGamma15V'] = sum([(int(self.gammaIdVeto(p))) for p in photons ])
        ret['nGamma175T'] = sum([(int(self.gammaIdTight(p))) for p in photons ])
        # event variables for the monojet analysis
        jets = [j for j in Collection(event,"Jet","nJet")]
        jetsFwd = [j for j in Collection(event,"JetFwd","nJetFwd")]
        alljets = jets + jetsFwd
        njet = len(jets)
        photonsT = [p for p in photons if self.gammaIdTight(p)]
        #print "check photonsT size is ", len(photonsT), " and nGamma175T = ",ret['nGamma175T']
        (met, metphi)  = event.metNoMu_pt, event.metNoMu_phi
        metp4 = ROOT.TLorentzVector()
        metp4.SetPtEtaPhiM(met,0,metphi,0)
        phmet = self.metNoPh(metp4,photonsT)
        ret['phmet_pt'] = phmet.Pt()
        ret['phmet_phi'] = phmet.Phi()

        ### lepton-jet cleaning
        # Define the loose leptons to be cleaned
        ret["iL"] = []
        for il,lep in enumerate(leps):
            if self.lepIdVeto(lep):
                ret["iL"].append(il)
        ret["nLepSel"] = len(ret["iL"])
        # Define cleaned jets 
        ret["iJ"] = []; 
        # 0. mark each identified jet as clean
        for j in alljets: 
            j._clean = True if (j.puId > 0.5 and j.id > 0.5) else False
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
        for ij,j in enumerate(alljets):
            if not j._clean: continue
            ret["iJ"].append(ij)
            if j._central: nJetCleanCentral += 1
        # 3. sort the jets by pt
        ret["iJ"].sort(key = lambda idx : alljets[idx].pt, reverse = True)
        # 4. compute the variables
        for jfloat in "pt eta phi mass btagCSV rawPt leadClean".split():
            jetret[jfloat] = []
        dphijj = 999
        dphijm = 999
        ijc = 0
        nAllJets30 = 0
        for idx in ret["iJ"]:
            jet = alljets[idx]
            # only save in the jetClean collection the central jets with pt > 30 GeV
            if jet.pt < 30: continue
            nAllJets30 += 1
            if jet._central:
                for jfloat in "pt eta phi mass btagCSV rawPt".split():
                    jetret[jfloat].append( getattr(jet,jfloat) )
                jetret["leadClean"].append( self.leadJetCleaning(jet) )
                if ijc==1 and jet._central: dphijj = deltaPhi(alljets[ret["iJ"][0]].phi,jet.phi)
                ijc += 1
            # use both central and fwd jets to compute deltaphi(jet,met)_min
            if nAllJets30 < 5: dphijm = min(dphijm,abs(deltaPhi(jet.phi,metphi)))
        ret["nJetClean"] = nJetCleanCentral
        ret['dphijj'] = dphijj
        ret['dphijm'] = dphijm
        # 5. compute the sums 
        ret["nJetClean30"] = 0
        ret["nBTag15"] = 0
        lowptjets = []
        for j in jets: # these are all central
            if not j._clean: continue
            if j.pt > 30:
                ret["nJetClean30"] += 1
            if j.pt > 15:
                lowptjets.append(j)
                if j.btagCSV > 0.89:
                    ret["nBTag15"] += 1

        ret["SF_BTag"] = self.BTagEventReweight(lowptjets) if event.run == 1 else 1.0

        ### muon-tau cleaning
        # Define cleaned taus
        ret["iT"] = []; 
        # 0. mark each tau as clean
        for t in taus: t._clean = True
        # 1. associate to each loose lepton its nearest tau 
        for il in ret["iL"]:
            lep = leps[il]
            best = None; bestdr = 0.4
            for t in taus:
                dr = deltaR(lep,t)
                if dr < bestdr:
                    best = t; bestdr = dr
            if best is not None: best._clean = False
        # 2. compute the tau list
        for it,t in enumerate(taus):
            if not t._clean: continue
            ret["iT"].append(it)
        # 3. sort the taus by pt
        ret["iT"].sort(key = lambda idx : taus[idx].pt, reverse = True)
        # 4. compute the variables
        for tfloat in "pt eta phi".split():
            tauret[tfloat] = []
        for idx in ret["iT"]:
            tau = taus[idx]
            for tfloat in "pt eta phi".split():
                tauret[tfloat].append( getattr(tau,tfloat) )
        ret["nTauClean"] = len(ret['iT'])
        # 5. compute the sums 
        ret["nTauClean18V"] = 0
        for t in taus:
            if not t._clean: continue
            if not self.tauIdVeto(t): continue
            ret["nTauClean18V"] += 1

        ### return
        fullret = {}
        for k,v in ret.iteritems():
            fullret[k] = v
        for k,v in jetret.iteritems():
            fullret["JetClean_%s" % k] = v
        for k,v in tauret.iteritems():
            fullret["TauClean_%s" % k] = v
        return fullret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVarsMonojet()
        def analyze(self,ev):
            if ev.metNoMu_pt < 200: return True
            print "\nrun %6d lumi %4d event %d: metNoMu %d" % (ev.run, ev.lumi, ev.evt, ev.metNoMu_pt)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        

from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.TTHAnalysis.tools.conept import conept

class LeptonJetReCleaner:
    def __init__(self,label,looseLeptonSel,tightLeptonSel,cleanJet,isMC=True):
        self.label = "" if (label in ["",None]) else ("_"+label)
        self.looseLeptonSel = looseLeptonSel
        self.tightLeptonSel = tightLeptonSel
        self.cleanJet = cleanJet
        self.isMC = isMC
    def listBranches(self):
        label = self.label
        biglist = [ ("nLepGood"+label, "I"), ("nLepGoodVeto"+label, "I"),("nLepTight"+label, "I"), ("nLepTightVeto"+label, "I"), ("nJetSel"+label, "I"), 
                 ("iL"+label,"I",20,"nLepGood"+label), ("iLV"+label,"I",20,"nLepGoodVeto"+label), ("iLT"+label,"I",20,"nLepTight"+label), ("iLTV"+label,"I",20,"nLepTightVeto"+label),
                 ("iJ"+label,"I",20,"nJetSel"+label), # index >= 0 if in Jet; -1-index (<0) if in DiscJet
                 ("nLepGood10"+label, "I"), ("nLepGoodVeto10"+label, "I"), ("nLepTight10"+label, "I"),("nLepTightVeto10"+label, "I"),
                 ("nJet40"+label, "I"), "htJet40j"+label, ("nBJetLoose40"+label, "I"), ("nBJetMedium40"+label, "I"),
                 ("nJet25"+label, "I"), "htJet25j"+label, ("nBJetLoose25"+label, "I"), ("nBJetMedium25"+label, "I"), 
                 ("iL1"+label, "I"), ("iL2"+label, "I"),
                 ("iL1T"+label, "I"), ("iL2T"+label, "I"), 
                 ("iL1p"+label, "I"), ("iL2p"+label, "I"), 
                 ("iL1Tp"+label, "I"), ("iL2Tp"+label, "I"), 
                 ("iL1V"+label, "I"), ("iL2V"+label, "I"), 
                 ("iL1TV"+label, "I"), ("iL2TV"+label, "I"), 
                 ("iL1Vp"+label, "I"), ("iL2Vp"+label, "I"), 
                 ("iL1TVp"+label, "I"), ("iL2TVp"+label, "I"), 
                 "mZ1cut10TL"+label, "minMllAFASTL"+label,"minMllAFOSTL"+label,"minMllSFOSTL"+label,
                 "minMllAFASTT"+label,"minMllAFOSTT"+label,"minMllSFOSTT"+label,
                 "mZ1"+label, "minMllAFAS"+label,"minMllAFOS"+label,"minMllSFOS"+label,"mllTV"+label,"mllV"+label,"mtWminTV"+label,"mtWminV"+label,  
                 ("SRTV"+label, "I"),("SRV"+label, "I"),
                ]
        for jfloat in "pt eta phi mass btagCSV rawPt".split():
            biglist.append( ("JetSel"+label+"_"+jfloat,"F",20,"nJetSel"+label) )
        if self.isMC:
            biglist.append( ("JetSel"+label+"_mcPt",     "F",20,"nJetSel"+label) )
            biglist.append( ("JetSel"+label+"_mcFlavour","I",20,"nJetSel"+label) )
            biglist.append( ("JetSel"+label+"_mcMatchId","I",20,"nJetSel"+label) )
        return biglist
    def __call__(self,event):
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        jetsc = [j for j in Collection(event,"Jet","nJet")]
        jetsd = [j for j in Collection(event,"DiscJet","nDiscJet")]
        (met, metphi)  = event.met_pt, event.met_phi
        ret = {}; jetret = {}
        #
        ### Define loose leptons
        ret["iL"] = []; ret["nLepGood10"] = 0
        ret["iLV"] = []; ret["nLepGoodVeto10"] = 0
        for il,lep in enumerate(leps):
            if self.looseLeptonSel(lep):
                ret["iL"].append(il)
                if lep.pt > 10: ret["nLepGood10"] += 1
        ret["nLepGood"] = len(ret["iL"])
        lepsl = [ leps[il] for il in ret["iL"]  ]
        for il,lep in enumerate(lepsl):
            if self.passMllTLVeto(lep, lepsl, 76, 106, True) and self.passMllTLVeto(lep, lepsl, 0, 12, True):
                    ret["iLV"].append(il)
                    if lep.pt > 10: ret["nLepGoodVeto10"] += 1
        ret["nLepGoodVeto"] = len(ret["iLV"])
        lepslv = [ leps[il] for il in ret["iLV"] ]            
        #
        ### Define tight leptons
        ret["iLT"] = []; ret["nLepTight10"] = 0
        ret["iLTV"] = []; ret["nLepTightVeto10"] = 0
        for il in ret["iL"]:
            lep = leps[il]
            if self.tightLeptonSel(lep):
                ret["iLT"].append(il)
                if lep.pt > 10: ret["nLepTight10"] += 1
                if self.passMllTLVeto(lep, lepsl, 76, 106, True) and self.passMllTLVeto(lep, lepsl, 0, 12, True):
                    ret["iLTV"].append(il)
                    if lep.pt > 10: ret["nLepTightVeto10"] += 1
        ret["nLepTight"] = len(ret["iLT"])
        lepst = [ leps[il] for il in ret["iLT"] ]
        ret["nLepTightVeto"] = len(ret["iLTV"])
        lepstv = [ leps[il] for il in ret["iLTV"] ]
        #
        ### Define jets
        ret["iJ"] = []
        # 0. mark each jet as clean
        for j in jetsc+jetsd: j._clean = True
        # 1. associate to each loose lepton its nearest jet 
        for il in ret["iL"]:
            lep = leps[il]
            best = None; bestdr = 0.4
            for j in jetsc+jetsd:
                dr = deltaR(lep,j)
                if dr < bestdr:
                    best = j; bestdr = dr
            if best is not None and self.cleanJet(lep,best,bestdr):
                best._clean = False
        # 2. compute the jet list
        for ijc,j in enumerate(jetsc):
            if not j._clean: continue
            ret["iJ"].append(ijc)
        for ijd,j in enumerate(jetsd):
            if not j._clean: continue
            ret["iJ"].append(-1-ijd)
        # 3. sort the jets by pt
        ret["iJ"].sort(key = lambda idx : jetsc[idx].pt if idx >= 0 else jetsd[-1-idx].pt, reverse = True)
        ret["nJetSel"] = len(ret["iJ"])
        # 4. compute the variables
        for jfloat in "pt eta phi mass btagCSV rawPt".split():
            jetret[jfloat] = []
        if self.isMC:
            for jmc in "mcPt mcFlavour mcMatchId".split():
                jetret[jmc] = []
        for idx in ret["iJ"]:
            jet = jetsc[idx] if idx >= 0 else jetsd[-1-idx]
            for jfloat in "pt eta phi mass btagCSV rawPt".split():
                jetret[jfloat].append( getattr(jet,jfloat) )
            if self.isMC:
                for jmc in "mcPt mcFlavour mcMatchId".split():
                    jetret[jmc].append( getattr(jet,jmc) )
        # 5. compute the sums
        ret["nJet25"] = 0; ret["htJet25j"] = 0; ret["nBJetLoose25"] = 0; ret["nBJetMedium25"] = 0
        ret["nJet40"] = 0; ret["htJet40j"] = 0; ret["nBJetLoose40"] = 0; ret["nBJetMedium40"] = 0
        for j in jetsc+jetsd:
            if not j._clean: continue
            if j.pt > 25:
                ret["nJet25"] += 1; ret["htJet25j"] += j.pt; 
                if j.btagCSV>0.423: ret["nBJetLoose25"] += 1
                if j.btagCSV>0.814: ret["nBJetMedium25"] += 1
            if j.pt > 40:
                ret["nJet40"] += 1; ret["htJet40j"] += j.pt; 
                if j.btagCSV>0.423: ret["nBJetLoose40"] += 1
                if j.btagCSV>0.814: ret["nBJetMedium40"] += 1
        #
        ### 2lss specific things
        ret['mZ1'] = self.bestZ1TL(lepsl, lepsl)
        ret['mZ1cut10TL'] = self.bestZ1TL(lepsl, lepst, cut=lambda l:l.pt>10)
        ret['minMllAFAS'] = self.minMllTL(lepsl, lepsl) 
        ret['minMllAFOS'] = self.minMllTL(lepsl, lepsl, paircut = lambda l1,l2 : l1.charge !=  l2.charge) 
        ret['minMllSFOS'] = self.minMllTL(lepsl, lepsl, paircut = lambda l1,l2 : l1.pdgId  == -l2.pdgId) 
        ret['minMllAFASTL'] = self.minMllTL(lepsl, lepst) 
        ret['minMllAFOSTL'] = self.minMllTL(lepsl, lepst, paircut = lambda l1,l2 : l1.charge !=  l2.charge) 
        ret['minMllSFOSTL'] = self.minMllTL(lepsl, lepst, paircut = lambda l1,l2 : l1.pdgId  == -l2.pdgId) 
        ret['minMllAFASTT'] = self.minMllTL(lepst, lepst)
        ret['minMllAFOSTT'] = self.minMllTL(lepst, lepst, paircut = lambda l1,l2 : l1.charge !=  l2.charge) 
        ret['minMllSFOSTT'] = self.minMllTL(lepst, lepst, paircut = lambda l1,l2 : l1.pdgId  == -l2.pdgId) 
        pairTypes = [ ("",lepsl,ret["iL"],True, True),
                      ("p",lepsl,ret["iL"],False, True),
                      ("V",lepslv,ret["iLV"],True, False),
                      ("Vp",lepslv,ret["iLV"],False, False),
                      ("T",lepst,ret["iLT"],True, True),                      
                      ("Tp",lepst,ret["iLT"],False, True),
                      ("TV",lepstv,ret["iLTV"],True, False),
                      ("TVp",lepstv,ret["iLTV"],False, False)
            ]
        for (name,lepcoll,lepIdxs, byflav, bypassMV) in pairTypes:
            iL1iL2 = self.bestSSPair(lepcoll, byflav,bypassMV, cut = lambda lep : lep.pt > 10)
            sizeIdxs=len(lepIdxs)
            ret["iL1"+name] = lepIdxs[ iL1iL2[0] ] if sizeIdxs >=1 else 0
            ret["iL2"+name] = lepIdxs[ iL1iL2[1] ] if sizeIdxs >=2 else 1
        ret["mllTV"]= (leps[ret["iL1TV"]].p4() + leps[ret["iL2TV"]].p4()).M()
        ret["mllV"]= (leps[ret["iL1V"]].p4() + leps[ret["iL2V"]].p4()).M()    
        #
        ### 2lss SR definitions
        ret["mtWminTV"] = min(sqrt(2*leps[ret["iL1TV"]].pt*met*(1-cos(leps[ret["iL1TV"]].phi-metphi))),sqrt(2*leps[ret["iL2TV"]].pt*met*(1-cos(leps[ret["iL2TV"]].phi-metphi))))
        ret["mtWminV"] = min(sqrt(2*leps[ret["iL1V"]].pt*met*(1-cos(leps[ret["iL1V"]].phi-metphi))),sqrt(2*leps[ret["iL2V"]].pt*met*(1-cos(leps[ret["iL2V"]].phi-metphi))))
        ret["SRTV"] = self.SR(leps[ret["iL1TV"]].pt,leps[ret["iL2TV"]].pt,ret["htJet40j"],met,ret["nJet40"],ret["nBJetMedium25"],ret["mtWminTV"])
        ret["SRV"] = self.SR(leps[ret["iL1V"]].pt,leps[ret["iL2V"]].pt,ret["htJet40j"],met,ret["nJet40"],ret["nBJetMedium25"],ret["mtWminV"])   
        #
        ### attach labels and return
        fullret = {}
        for k,v in ret.iteritems(): 
            fullret[k+self.label] = v
        for k,v in jetret.iteritems(): 
            fullret["JetSel%s_%s" % (self.label,k)] = v
        return fullret

    def passMllVeto(self, l1, l2, mZmin, mZmax, isOSSF ):
        if  l1.pdgId == -l2.pdgId or not isOSSF:
            mz = (l1.p4() + l2.p4()).M()
            if mz > mZmin and  mz < mZmax:
                return False
        return True
    def passMllTLVeto(self, lep, lepsl, mZmin, mZmax, isOSSF):
        for ll in lepsl:
            if ll == lep: continue
            if not self.passMllVeto(lep, ll, mZmin, mZmax, isOSSF):
                return False
        return True
    def bestZ1TL(self,lepsl,lepst,cut=lambda lep:True):
          pairs = []
          for l1 in lepst:
            if not cut(l1): continue
            for l2 in lepsl:
                if not cut(l2): continue
                if l1.pdgId == -l2.pdgId:
                   mz = (l1.p4() + l2.p4()).M()
                   diff = abs(mz-91.2)
                   pairs.append( (diff,mz) )
          if len(pairs):
              pairs.sort()
              return pairs[0][1]
          return 0.
    def minMllTL(self, lepsl, lepst, bothcut=lambda lep:True, onecut=lambda lep:True, paircut=lambda lep1,lep2:True):
            pairs = []
            for l1 in lepst:
                if not bothcut(l1): continue
                for l2 in lepsl:
                    if l2 == l1 or not bothcut(l2): continue
                    if not onecut(l1) and not onecut(l2): continue
                    if not paircut(l1,l2): continue
                    mll = (l1.p4() + l2.p4()).M()
                    pairs.append(mll)
            if len(pairs):
                return min(pairs)
            return -1
    def bestSSPair(self,leps,byflav,bypassMV,cut=lambda lep:True):
        ret = (0,1)
        if len(leps) < 2:
            ret = (0,1)
        if len(leps) > 2:
            pairs = []
            for il1 in xrange(len(leps)-1):
                for il2 in xrange(il1+1,len(leps)): 
                    l1 = leps[il1]
                    l2 = leps[il2]
                    if not cut(l1) or not cut(l2): continue
                    if not self.passMllVeto(l1, l2, 0, 8, False) and not bypassMV: continue
                    if l1.charge == l2.charge:
                        flav = abs(l1.pdgId) + abs(l2.pdgId) if byflav else 0
                        ht   = l1.pt + l2.pt
                        pairs.append( (-flav,-ht,il1,il2) )
            if len(pairs):
                pairs.sort()
                ret = (pairs[0][2],pairs[0][3])
        return ret
    def SR(self, l1pt, l2pt, ht, met, nj, nb, mtw):
        if l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 1
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120  : SR = 2
        elif l1pt > 25 and l2pt > 25 and ht < 300 and nb == 0 and ((met > 50 and met < 200 and nj >= 5 and mtw < 120) or (met > 200 and met < 500 and nj >= 2 and mtw < 120) or (met > 50 and met < 500 and nj >= 2 and mtw > 120))   : SR = 3
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 5 and nb == 0 and mtw < 120  : SR = 4
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120  : SR = 5
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 5 and nb == 0 and mtw < 120  : SR = 6
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw > 120  : SR = 7
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and nb == 0 and ((met > 50 and met < 200 and nj >= 5 and mtw > 120) or (met > 200 and met < 500 and nj >= 2 and mtw > 120)) : SR = 8
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 9
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120  : SR = 10
        elif l1pt > 25 and l2pt > 25 and ht < 300 and nb == 1 and ((met > 50 and met < 200 and nj >= 5 and mtw < 120) or (met > 200 and met < 500 and nj >= 2 and mtw < 120) or (met > 50 and met < 500 and nj >= 2 and mtw > 120))   : SR = 11
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 5 and nb == 1 and mtw < 120  : SR = 12
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120  : SR = 13
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 5 and nb == 1 and mtw < 120  : SR = 14
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw > 120  : SR = 15
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and nb == 1 and ((met > 50 and met < 200 and nj >= 5 and mtw > 120) or (met > 200 and met < 500 and nj >= 2 and mtw > 120)) : SR = 16
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 17
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120  : SR = 18
        elif l1pt > 25 and l2pt > 25 and ht < 300 and nb == 2 and ((met > 50 and met < 200 and nj >= 5 and mtw < 120) or (met > 200 and met < 500 and nj >= 2 and mtw < 120) or (met > 50 and met < 500 and nj >= 2 and mtw > 120))   : SR = 19
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 5 and nb == 2 and mtw < 120  : SR = 20
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120  : SR = 21
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 5 and nb == 2 and mtw < 120  : SR = 22
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw > 120  : SR = 23
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and nb == 2 and ((met > 50 and met < 200 and nj >= 5 and mtw > 120) or (met > 200 and met < 500 and nj >= 2 and mtw > 120)) : SR = 24
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 25
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 26
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 200 and met < 500 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 27
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 28
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 500 and nj >= 2 and nb >= 3 and mtw > 120 : SR = 29
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1600 and met > 50 and met < 500 and nj >= 2 and nb >= 3 and mtw > 120 : SR = 30
        elif l1pt > 25 and l2pt > 25 and ht > 300 and met > 500 and nj >= 2 : SR = 31
        elif l1pt > 25 and l2pt > 25 and ht > 1600 and met > 50 and met < 500 and nj >= 2 : SR = 32
        ####
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 33 #1B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 34 #2B
        elif l1pt > 25 and l2pt < 25 and ht < 300  and ((met > 50 and met < 200 and nj >= 5) or (met > 200 and met < 500 and nj >= 2)) and nb == 0 and mtw < 120 : SR = 35 #3B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 5 and nb == 0 and mtw < 120 : SR = 36  #4B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 37 #5B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 5 and nb == 0 and mtw < 120 : SR = 38 #6B
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 39 #7B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 40 #8B
        elif l1pt > 25 and l2pt < 25 and ht < 300  and ((met > 50 and met < 200 and nj >= 5) or (met > 200 and met < 500 and nj >= 2)) and nb == 1 and mtw < 120 : SR = 41 #9B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 5 and nb == 1 and mtw < 120 : SR = 42 #10B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 43 #11B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 5 and nb == 1 and mtw < 120 : SR = 44 #12B
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 45 #13B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 46 #14B
        elif l1pt > 25 and l2pt < 25 and ht < 300  and ((met > 50 and met < 200 and nj >= 5) or (met > 200 and met < 500 and nj >= 2)) and nb == 2 and mtw < 120 : SR = 47 #15B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 5 and nb == 2 and mtw < 120 : SR = 48 #16B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 49 #17B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 5 and nb == 2 and mtw < 120 : SR = 50 #18B
       
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 51 #19B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 52 #20B
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 200 and met < 500 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 53 #21B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 200 and met < 500 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 54 #21B       
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 500 and nj >= 2 and mtw > 120 : SR = 55 #23B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1600 and met > 50 and met < 500 and nj >= 2 and mtw > 120 : SR = 56 #24B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and met > 500 and nj >= 2 : SR = 57 #25B
        elif l1pt > 25 and l2pt < 25 and ht > 1600 and met > 50 and met < 500 and nj >= 2 : SR = 58 #26B
        ####
        elif l1pt < 25 and l2pt < 25 and  met > 50 and met < 200 and nb == 0 and mtw < 120 : SR = 59 #C1 
        elif l1pt < 25 and l2pt < 25 and  met > 200 and nb == 0 and mtw < 120 : SR = 60 #C2 
        elif l1pt < 25 and l2pt < 25 and  met > 50 and met < 200 and nb == 1 and mtw < 120 : SR = 61 #C3 
        elif l1pt < 25 and l2pt < 25 and  met > 200 and nb == 1 and mtw < 120 : SR = 62 #C4 
        elif l1pt < 25 and l2pt < 25 and  met > 50 and met < 200 and nb == 2 and mtw < 120 : SR = 63 #C5 
        elif l1pt < 25 and l2pt < 25 and  met > 200 and nb == 2 and mtw < 120 : SR = 64 #C6 
        elif l1pt < 25 and l2pt < 25 and  nb >= 3 and mtw < 120 : SR = 65 #C7 
        elif l1pt < 25 and l2pt < 25 and  mtw > 120 : SR = 66 #C8 
        else : SR = 0 
        return SR


def _tthlep_lepId(lep):
        #if lep.pt <= 10: return False
        if abs(lep.pdgId) == 13:
            if lep.pt <= 5: return False
            return True #lep.mediumMuonId > 0
        elif abs(lep.pdgId) == 11:
            if lep.pt <= 7: return False
            if not (lep.convVeto and lep.lostHits == 0): 
                return False
            return True #lep.mvaIdPhys14 > 0.73+(0.57-0.74)*(abs(lep.eta)>0.8)+(0.05-0.57)*(abs(lep.eta)>1.479)
        return False

def _susy2lss_lepId_CBloose(lep):
        if abs(lep.pdgId) == 13:
            if lep.pt <= 5: return False
            return True #lep.mediumMuonId > 0
        elif abs(lep.pdgId) == 11:
            if lep.pt <= 7: return False
            if not (lep.convVeto and lep.lostHits <= 1): 
                return False
            return True #lep.mvaIdPhys14 > -0.11+(-0.35+0.11)*(abs(lep.eta)>0.8)+(-0.55+0.35)*(abs(lep.eta)>1.479)
        return False

def _susy2lss_lepId_CB(lep):
        if lep.pt <= 10: return False
        if abs(lep.pdgId) == 13:
            return lep.mediumMuonId > 0 and lep.tightCharge > 0
        elif abs(lep.pdgId) == 11:
            if not (lep.convVeto and lep.tightCharge > 1 and lep.lostHits == 0): 
                return False
            return lep.mvaIdPhys14 > 0.73+(0.57-0.73)*(abs(lep.eta)>0.8)+(0.05-0.57)*(abs(lep.eta)>1.479)
        return False

def _susy2lss_multiIso(lep):
        if abs(lep.pdgId) == 13: A,B,C = (0.14,0.68,6.7)
        else:                    A,B,C = (0.10,0.70,7.0)
        return lep.miniRelIso < A and (lep.jetPtRatio > B or lep.jetPtRel > C)
def _susy2lss_multiIso_withMiniIsoRelaxed_ConePtJetPtRatio(lep):
        if abs(lep.pdgId) == 13: A,B,C = (0.4,0.68,6.7)
        else:                    A,B,C = (0.4,0.70,7.0)
        return lep.miniRelIso < A and (conept(lep.pt,lep.miniRelIso,lep.jetPtRatio,lep.jetPtRel,lep.pdgId,2)/lep.pt*lep.jetPtRatio > B or lep.jetPtRel > C)
def _susy2lss_multiIso_withMiniIsoRelaxed_CutForFO4(lep):
        if abs(lep.pdgId) == 13: A,B,C = (0.4,0.68,6.7)
        else:                    A,B,C = (0.4,0.70,7.0)
        return lep.miniRelIso < A and (1/lep.jetPtRatio < (1/B + lep.miniRelIso))

def _susy2lss_lepId_CBOld(lep):
        if lep.pt <= 10: return False
        if abs(lep.pdgId) == 13:
            return lep.tightId > 0
        elif abs(lep.pdgId) == 11:
            return lep.tightId >= 2 and lep.convVeto and lep.tightCharge > 1 and lep.lostHits == 0
        return False

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf1 = LeptonJetReCleaner("Old", 
                lambda lep : lep.relIso03 < 0.5, 
                lambda lep : lep.relIso03 < 0.1 and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4))
            self.sf2 = LeptonJetReCleaner("PtRel", 
                lambda lep : lep.relIso03 < 0.4 or lep.jetPtRel > 5, 
                lambda lep : (lep.relIso03 < 0.1 or lep.jetPtRel > 14) and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4))
            self.sf3 = LeptonJetReCleaner("MiniIso", 
                lambda lep : lep.miniRelIso < 0.4, 
                lambda lep : lep.miniRelIso < 0.05 and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4))
            self.sf4 = LeptonJetReCleaner("PtRelJC", 
                lambda lep : lep.relIso03 < 0.4 or lep.jetPtRel > 5, 
                lambda lep : (lep.relIso03 < 0.1 or lep.jetPtRel > 14) and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4 and not (lep.jetPtRel > 5 and lep.pt*(1/lep.jetPtRatio-1) > 25)))
            self.sf5 = LeptonJetReCleaner("MiniIsoJC", 
                lambda lep : lep.miniRelIso < 0.4, 
                lambda lep : lep.miniRelIso < 0.05 and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4 and not (lep.jetDR > 0.5*10/min(50,max(lep.pt,200)) and lep.pt*(1/lep.jetPtRatio-1) > 25)))
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf1(ev)
            print self.sf2(ev)
            print self.sf3(ev)
            print self.sf4(ev)
            print self.sf5(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        

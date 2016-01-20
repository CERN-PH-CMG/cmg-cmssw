from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.TTHAnalysis.tools.leptonJetReCleaner import passMllVeto
from ROOT import TFile,TH1F
import os

for extlib in ["fake_rates_UCSx_v5_03.cc","flip_rates_UCSx_v5_01.cc","triggerSF_fullsim_UCSx_v5_01.cc","lepton_SF_UCSx_v5_03.cc","FastSimTriggerEff.cc"]:
    if not extlib.endswith(".cc"): raise RuntimeError
    if "/%s"%extlib.replace(".cc","_cc.so") not in ROOT.gSystem.GetLibraries():
        ROOT.gROOT.LoadMacro("/afs/cern.ch/work/p/peruzzi/ra5trees/cms_utility_files/%s+"%extlib)
from ROOT import electronFakeRate_UCSx
from ROOT import electronFakeRate_UCSx_Error
from ROOT import electronAlternativeFakeRate_UCSx
from ROOT import electronQCDMCFakeRate_UCSx
from ROOT import muonFakeRate_UCSx
from ROOT import muonFakeRate_UCSx_Error
from ROOT import muonAlternativeFakeRate_UCSx
from ROOT import muonQCDMCFakeRate_UCSx
from ROOT import electronFakeRate_UCSx_IsoTrigs
from ROOT import electronFakeRate_UCSx_Error_IsoTrigs
from ROOT import electronAlternativeFakeRate_UCSx_IsoTrigs
from ROOT import electronQCDMCFakeRate_UCSx_IsoTrigs
from ROOT import muonFakeRate_UCSx_IsoTrigs
from ROOT import muonFakeRate_UCSx_Error_IsoTrigs
from ROOT import muonAlternativeFakeRate_UCSx_IsoTrigs
from ROOT import muonQCDMCFakeRate_UCSx_IsoTrigs
from ROOT import flipRate_UCSx
from ROOT import flipRate_UCSx_Error
from ROOT import triggerScaleFactorFullSim
from ROOT import electronScaleFactorHighHT_UCSx
from ROOT import electronScaleFactorLowHT_UCSx
from ROOT import muonScaleFactor_UCSx
from ROOT import leptonScaleFactor_UCSx
from ROOT import FastSimTriggerEfficiency

class LeptonChoiceRA5:

    # enum
    style_TT_loopTF_2FF = 0
    style_sort_FO = 1

    # enum
    appl_Fakes = 0
    appl_Flips = 1
    appl_WZ    = 2

    def __init__(self,label,inputlabel,whichApplication,lepChoiceMethod=None,FRFileName=None,isFastSim=False,lepSFFileNameFastSim=None):
        self.label = "" if (label in ["",None]) else ("_"+label)
        self.inputlabel = '_'+inputlabel
        self.isFastSim = isFastSim
        self.lepSFFileNameFastSim = lepSFFileNameFastSim # [muons,electrons]
        self.systsLEPSF={0:"", 1:"_lepSFUp", -1:"_lepSFDown", 2:"_lepSF_FS_Up", -2:"_lepSF_FS_Down"}
        if self.isFastSim:
            print '-'*15
            print 'WARNING: will apply trigger efficiency for FastSim'
            print '-'*15
        if whichApplication=="Fakes": self.whichApplication = self.appl_Fakes
        elif whichApplication=="Flips": self.whichApplication = self.appl_Flips
        elif whichApplication=="WZ": self.whichApplication = self.appl_WZ
        else: raise RuntimeError, 'Unknown whichApplication'
        self.lepChoiceMethod = None
        self.apply = False
        if self.whichApplication == self.appl_Fakes:
            if lepChoiceMethod=="TT_loopTF_2FF": self.lepChoiceMethod = self.style_TT_loopTF_2FF
            elif lepChoiceMethod=="sort_FO": self.lepChoiceMethod = self.style_sort_FO
            else: raise RuntimeError, 'Unknown lepChoiceMethod'
            if FRFileName:
                self.apply = True
                self.initFRhistos(FRFileName)
        elif self.whichApplication == self.appl_Flips:
            if FRFileName:
                self.apply = True
                self.initFlipAppHistos(FRFileName)
        if not self.apply:
            print 'WARNING: running leptonChoiceRA5 %s in pure tagging mode (no weights applied)'%label

    def listBranches(self):
        label = self.label
        biglist = [ 
            ("nPairs"+label,"I"),
            ("i1"+label,"I",20,"nPairs"+label),
            ("i2"+label,"I",20,"nPairs"+label),
            ("whoIsFake"+label,"I",20,"nPairs"+label),
            ("appWeight"+label,"F",20,"nPairs"+label),
            ("appWeight_ewkUp"+label,"F",20,"nPairs"+label),
            ("appWeight_ewkDown"+label,"F",20,"nPairs"+label),
            ("SR"+label,"I",20,"nPairs"+label),
            ("SR_jecUp"+label,"I",20,"nPairs"+label),
            ("SR_jecDown"+label,"I",20,"nPairs"+label),
            ("hasTT"+label, "I"), ("hasTF"+label, "I"), ("hasFF"+label, "I"),
            ("mZ1"+label,"F"), ("mZ1cut10TL"+label,"F"),("minMllAFAS"+label,"F"),("minMllAFASTT"+label,"F"), ("minMllAFASTL"+label,"F"), ("minMllSFOS"+label,"F"), ("minMllSFOSTL"+label,"F"), ("minMllSFOSTT"+label,"F"),
            ("triggerSF"+label,"F",20,"nPairs"+label),
            ("maxDeltaPhiLepJet"+label,"F",20,"nPairs"+label),
            ("maxDeltaPhiLepBJet"+label,"F",20,"nPairs"+label),
            ("maxDeltaPhiJetJet"+label,"F",20,"nPairs"+label),
            ("minDeltaRLepJet"+label,"F",20,"nPairs"+label),
            ("minDeltaRLepBJet"+label,"F",20,"nPairs"+label),
            ]
        for var in self.systsLEPSF: biglist.append(("leptonSF"+self.systsLEPSF[var]+label,"F",20,"nPairs"+label))
        return biglist

    def __call__(self,event):

        if not hasattr(self,"checked_fastsim_data"):
            if self.isFastSim and event.isData: raise RuntimeError,'Running with isFastSim on data!'
            self.checked_fastsim_data = True

        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        lepsl = [leps[il] for il in getattr(event,"iL"+self.inputlabel)]
#        lepsc = [leps[il] for il in getattr(event,"iC"+self.inputlabel)]
#        lepsf = [leps[il] for il in getattr(event,"iF"+self.inputlabel)]
        lepst = [leps[il] for il in getattr(event,"iT"+self.inputlabel)]
#        lepslv = [leps[il] for il in getattr(event,"iLV"+self.inputlabel)]
#        lepscv = [leps[il] for il in getattr(event,"iCV"+self.inputlabel)]
        lepsfv = [leps[il] for il in getattr(event,"iFV"+self.inputlabel)]
        lepstv = [leps[il] for il in getattr(event,"iTV"+self.inputlabel)]

        bjets25 = [j for j in Collection(event,"JetSel"+self.inputlabel,"nJetSel"+self.inputlabel) if (j.pt>25 and j.btagCSV>0.89)]
        jets40 = [j for j in Collection(event,"JetSel"+self.inputlabel,"nJetSel"+self.inputlabel) if j.pt>40]
        
        systsFR={0:"", 1:"_ewkUp", -1:"_ewkDown"}
        systsJEC={0:"", 1:"_jecUp", -1:"_jecDown"}
        met={}
        metphi={}
        met[0]=event.met_pt
        met[1]=getattr(event,"met_jecUp_pt",event.met_pt)
        met[-1]=getattr(event,"met_jecDown_pt",event.met_pt)
        metphi[0]= event.met_phi
        metphi[1]= getattr(event,"met_jecUp_phi",event.met_phi)
        metphi[-1]= getattr(event,"met_jecDown_phi",event.met_phi)

        ret = {};

        ### 2lss specific things - still useful?
        ret['mZ1'] = self.bestZ1TL(lepsl, lepsl)
        ret['mZ1cut10TL'] = self.bestZ1TL(lepsl, lepst, cut=lambda l:l.conePt>(10 if abs(l.pdgId)==13 else 15))
        ret['minMllAFAS'] = self.minMllTL(lepsl, lepsl) 
#        ret['minMllAFOS'] = self.minMllTL(lepsl, lepsl, paircut = lambda l1,l2 : l1.charge !=  l2.charge) 
        ret['minMllSFOS'] = self.minMllTL(lepsl, lepsl, paircut = lambda l1,l2 : l1.pdgId  == -l2.pdgId) 
        ret['minMllAFASTL'] = self.minMllTL(lepsl, lepst) 
 #       ret['minMllAFOSTL'] = self.minMllTL(lepsl, lepst, paircut = lambda l1,l2 : l1.charge !=  l2.charge) 
        ret['minMllSFOSTL'] = self.minMllTL(lepsl, lepst, paircut = lambda l1,l2 : l1.pdgId  == -l2.pdgId) 
        ret['minMllAFASTT'] = self.minMllTL(lepst, lepst)
#        ret['minMllAFOSTT'] = self.minMllTL(lepst, lepst, paircut = lambda l1,l2 : l1.charge !=  l2.charge) 
        ret['minMllSFOSTT'] = self.minMllTL(lepst, lepst, paircut = lambda l1,l2 : l1.pdgId  == -l2.pdgId) 

        ret["nPairs"]=0
        ret["i1"] = [0]*20
        ret["i2"] = [0]*20
        ret["whoIsFake"]=[0]*20
        ret["appWeight"] = [0]*20
        ret["appWeight_ewkUp"] = [0]*20
        ret["appWeight_ewkDown"] = [0]*20
        ret["SR"] = [0]*20
        ret["SR_jecUp"] = [0]*20
        ret["SR_jecDown"] = [0]*20
        ret["hasTT"]=False
        ret["hasTF"]=False
        ret["hasFF"]=False
        ret["triggerSF"] = [0]*20
        for var in self.systsLEPSF: ret["leptonSF"+self.systsLEPSF[var]] = [0]*20
        ret["maxDeltaPhiLepJet"] = [0]*20
        ret["maxDeltaPhiLepBJet"] = [0]*20
        ret["maxDeltaPhiJetJet"] = [0]*20
        ret["minDeltaRLepJet"] = [0]*20
        ret["minDeltaRLepBJet"] = [0]*20

        if self.whichApplication == self.appl_Fakes:
            if self.lepChoiceMethod==self.style_TT_loopTF_2FF:
                choice = self.findPairs(lepstv,lepstv,byflav=True,bypassMV=False,choose_SS_else_OS=True)
                if choice:
                    ret["hasTT"]=True
                    choice=choice[:1]
                else:
                    choice = self.findPairs(lepstv,lepsfv,byflav=True,bypassMV=False,choose_SS_else_OS=True)
                    if choice:
                        ret["hasTF"]=True
                        _probs = {}
                    else:
                        choice = self.findPairs(lepsfv,lepsfv,byflav=True,bypassMV=False,choose_SS_else_OS=True)
                        if choice:
                            ret["hasFF"]=True
            elif self.lepChoiceMethod==self.style_sort_FO:
                choice = self.findPairs(lepsfv,lepsfv,byflav=True,bypassMV=False,choose_SS_else_OS=True)
                if choice:
                    choice = choice[:1]
                    tt_sort_FO = [False]*2
                    if choice[0][0] in lepstv: tt_sort_FO[0]=True
                    if choice[0][1] in lepstv: tt_sort_FO[1]=True
                    if tt_sort_FO==[True,True]: ret["hasTT"]=True
                    elif tt_sort_FO==[False,False]: ret["hasFF"]=True
                    else: ret["hasTF"] = True
        elif self.whichApplication == self.appl_Flips:
            choice = self.findPairs(lepstv,lepstv,byflav=True,bypassMV=False,choose_SS_else_OS=True)
            if choice:
                ret["hasTT"]=True
                choice=choice[:1]
            else:
                choice = self.findPairs(lepst,lepst,byflav=True,bypassMV=False,choose_SS_else_OS=False)
                if choice:
                    ret["hasTF"]=True
                    choice=choice[:1]
        elif self.whichApplication == self.appl_WZ:
            choice = self.findPairs(lepst,lepst,byflav=True,bypassMV=True,choose_SS_else_OS=True)
            if choice:
                ret["hasTT"]=True
            else:
                choice = self.findPairs(lepst,lepst,byflav=True,bypassMV=True,choose_SS_else_OS=False)
                if choice:
                    ret["hasTF"]=True
                    choice=choice[:1]

        if choice:
            ret["nPairs"] = len(choice)
            if ret["nPairs"]>20: raise RuntimeError,'Too many lepton pairs'
            for npair in xrange(len(choice)):
                i1 = leps.index(choice[npair][0])
                i2 = leps.index(choice[npair][1])
                ret["i1"][npair], ret["i2"][npair] = (i1,i2) if leps[i1].conePt>=leps[i2].conePt else (i2,i1) # warning: they are not necessarily ordered by pt!
                for var in systsJEC:
                    mtwmin = min(sqrt(2*leps[i1].conePt*met[var]*(1-cos(leps[i1].phi-metphi[var]))),sqrt(2*leps[i2].conePt*met[var]*(1-cos(leps[i2].phi-metphi[var]))))
                    ret["SR"+systsJEC[var]][npair]=self.SR(leps[i1].conePt,leps[i2].conePt,getattr(event,"htJet40j"+systsJEC[var]+self.inputlabel),met[var],getattr(event,"nJet40"+systsJEC[var]+self.inputlabel),getattr(event,"nBJetMedium25"+systsJEC[var]+self.inputlabel),mtwmin)
                ht = getattr(event,"htJet40j"+self.inputlabel) # central value
                ret["triggerSF"][npair] = triggerScaleFactorFullSim(leps[i1].pdgId,leps[i2].pdgId,leps[i1].pt,leps[i2].pt,ht) if not event.isData else 1
                if self.isFastSim: ret["triggerSF"][npair] = ret["triggerSF"][npair] * FastSimTriggerEfficiency(ht,leps[i1].pt,leps[i1].pdgId,leps[i2].pt,leps[i2].pdgId)
                for var in self.systsLEPSF:
                    lepsf=[1]*2
                    for i,ind in enumerate([i1,i2]):
                        lepsf[i] = leptonScaleFactor_UCSx(leps[ind].pdgId,leps[ind].pt,leps[ind].eta,ht) if not event.isData else 1
                        #if abs(var)==1: TO BE IMPLEMENTED, at the moment it's done with a flat uncertainty on the final datacard
                        if self.isFastSim:
                            addsf, addsferr = self.read_FastSim_lepSF(leps[ind].pdgId,leps[ind].pt,leps[ind].eta,event.nVert) if not event.isData else 1
                            if var==2: lepsf[i] *= (addsf*(1+addsferr))
                            if var==-2: lepsf[i] *= (addsf*(1-addsferr))
                            else: lepsf[i] *= addsf
                    ret["leptonSF"+self.systsLEPSF[var]][npair] = lepsf[0]*lepsf[1]
                ret["maxDeltaPhiLepJet"][npair] = max([abs(deltaPhi(l.phi,j.phi)) for l in [leps[i1],leps[i2]] for j in jets40]+[-999])
                ret["maxDeltaPhiLepBJet"][npair] = max([abs(deltaPhi(l.phi,j.phi)) for l in [leps[i1],leps[i2]] for j in bjets25]+[-999])
                ret["maxDeltaPhiJetJet"][npair] = max([(abs(deltaPhi(j1.phi,j2.phi)) if j1!=j2 else -999) for j1 in jets40 for j2 in jets40]+[-999])
                ret["minDeltaRLepJet"][npair] = min([abs(deltaR(l,j)) for l in [leps[i1],leps[i2]] for j in jets40]+[999])
                ret["minDeltaRLepBJet"][npair] = min([abs(deltaR(l,j)) for l in [leps[i1],leps[i2]] for j in bjets25]+[999])

                if self.apply:
                    if self.whichApplication == self.appl_Fakes:
                        for varFR in systsFR:
                            if self.lepChoiceMethod==self.style_TT_loopTF_2FF:
                                if ret["hasTT"]:
                                    ret["appWeight"+systsFR[varFR]][npair] = 0.0
                                elif ret["hasTF"]:
                                    prev = 1.0
                                    if varFR not in _probs: _probs[varFR]=[]
                                    for x in _probs[varFR]:
                                        prev *= (1-x)
                                    prob = self.FRprob(leps[i2],ht,varFR)
                                    transf = self.FRtransfer_fromprob(prob)
                                    _probs[varFR].append(prob)
                                    ret["appWeight"+systsFR[varFR]][npair] = prev * transf
                                    ret["whoIsFake"][npair] = 2 if leps[i1].conePt>=leps[i2].conePt else 1
                                elif ret["hasFF"]:
                                    ret["appWeight"+systsFR[varFR]][npair] = -self.FRtransfer(leps[i1],ht,varFR)*self.FRtransfer(leps[i2],ht,varFR) if len(choice)<2 else 0.0 # throw away events with three FO non Tight
                            elif self.lepChoiceMethod==self.style_sort_FO:
                                if ret["hasTT"]:
                                    ret["appWeight"+systsFR[varFR]][npair] = 0.0
                                elif ret["hasTF"]:
                                    ret["appWeight"+systsFR[varFR]][npair] = self.FRtransfer(leps[i2 if tt_sort_FO[0] else i1],ht,varFR)
                                    ret["whoIsFake"][npair] = (2 if leps[i1].conePt>=leps[i2].conePt else 1) if tt_sort_FO[0] else (1 if leps[i1].conePt>=leps[i2].conePt else 2)
                                elif ret["hasFF"]:
                                    ret["appWeight"+systsFR[varFR]][npair] = -self.FRtransfer(leps[i1],ht,varFR)*self.FRtransfer(leps[i2],ht,varFR)
                    elif self.whichApplication == self.appl_Flips:
                        ret["appWeight"][npair] = self.flipRate(leps[i1])+self.flipRate(leps[i2])

        ### attach labels and return
        fullret = {}
        for k,v in ret.iteritems(): 
            fullret[k+self.label] = v
        return fullret

    def initFRhistos(self,FRFileName):
        self.useFakesHardCodedInSitu = False
        self.useFakesHardCodedUCSx = False
        if FRFileName=="InSituHardCoded":
            self.useFakesHardCodedInSitu = True
        elif FRFileName=="hardcodedUCSx":
            self.useFakesHardCodedUCSx = True
        else:    
            self.FRfile = ROOT.TFile(FRFileName,"read")
#            self.FR_mu = (self.FRfile.Get("FRMuPtCorr_ETH_non"),self.FRfile.Get("FRMuPtCorr_ETH_iso"))
#            self.FR_el = (self.FRfile.Get("FRElPtCorr_ETH_non"),self.FRfile.Get("FRElPtCorr_ETH_iso"))
            self.FR_mu={}
            self.FR_el={}
            self.FR_mu[0] = (self.FRfile.Get("FRMuPtCorr_UCSX_non"),self.FRfile.Get("FRMuPtCorr_UCSX_iso"))
            self.FR_el[0] = (self.FRfile.Get("FRElPtCorr_UCSX_non"),self.FRfile.Get("FRElPtCorr_UCSX_iso"))
            self.FR_mu[1] = (self.FRfile.Get("FRMuPtCorr_UCSX_HI_non"),self.FRfile.Get("FRMuPtCorr_UCSX_HI_iso"))
            self.FR_el[1] = (self.FRfile.Get("FRElPtCorr_UCSX_HI_non"),self.FRfile.Get("FRElPtCorr_UCSX_HI_iso"))
            self.FR_mu[-1] = (self.FRfile.Get("FRMuPtCorr_UCSX_LO_non"),self.FRfile.Get("FRMuPtCorr_UCSX_LO_iso"))
            self.FR_el[-1] = (self.FRfile.Get("FRElPtCorr_UCSX_LO_non"),self.FRfile.Get("FRElPtCorr_UCSX_LO_iso"))

    def initFlipAppHistos(self,FRFileName):
        if FRFileName=="hardcodedUCSx":
            import os, ROOT
            self.flipRate = self.flipRate_hardcodedUCSx
        else:
            self.flipRate_file = ROOT.TFile(FRFileName,"read")
            self.flipRate_histo = self.flipRate_file.Get("flipMapUCSX")
            self.flipRate = self.flipRate_fromHistoWithSF

    def flipRate_fromHistoWithSF(self,lep):
        if abs(lep.pdgId)!=11: return 0
        h = self.flipRate_histo
        ptbin = max(1,min(h.GetNbinsX(),h.GetXaxis().FindBin(lep.conePt)))
        etabin = max(1,min(h.GetNbinsY(),h.GetYaxis().FindBin(abs(lep.eta))))
        res = h.GetBinContent(ptbin,etabin)
        sf = 3.6 if (lep.eta<-1.5 and lep.eta>-2) else 1.15
        return res*sf

    def flipRate_hardcodedUCSx(self,lep):
        if abs(lep.pdgId)!=11: return 0
        pt = lep.pt
        eta = lep.eta
        return flipRate_UCSx(pt,eta)

    def FRprob(self,lep,ht,var):
        isiso = (ht<=300)
        if self.useFakesHardCodedInSitu:
            pt = lep.conePt
            if abs(lep.pdgId)==11:
                if not isiso:
                    if (pt>=10 and pt<15): return 0.584615
                    if (pt>=15 and pt<25): return 0.386667
                    if (pt>=25 and pt<35): return 0.27027
                    if (pt>=35 and pt<50): return 0.166667
                    if (pt>=50 and pt<70): return 0.36
                else:
                    if (pt>=10 and pt<15): return 0.7 
                    if (pt>=15 and pt<25): return 0.516129
                    if (pt>=25 and pt<35): return 0.439024
                    if (pt>=35 and pt<50): return 0.513514
                    if (pt>=50 and pt<70): return 0.37931 
            elif abs(lep.pdgId)==13:
                if not isiso:
                    if (pt>=10 and pt<15): return 0.584906
                    if (pt>=15 and pt<25): return 0.351064
                    if (pt>=25 and pt<35): return 0.27027 
                    if (pt>=35 and pt<50): return 0.217391
                    if (pt>=50 and pt<70): return 0.130435
                else:
                    if (pt>=10 and pt<15): return 0.652174
                    if (pt>=15 and pt<25): return 0.403042
                    if (pt>=25 and pt<35): return 0.213675
                    if (pt>=35 and pt<50): return 0.166667
                    if (pt>=50 and pt<70): return 0.181818
            return 0
        elif self.useFakesHardCodedUCSx:
            pt = lep.conePt
            eta = lep.eta
            if var!=1:
                if isiso: fr = electronFakeRate_UCSx_IsoTrigs(pt,eta) if abs(lep.pdgId)==11 else muonFakeRate_UCSx_IsoTrigs(pt,eta)
                else: fr = electronFakeRate_UCSx(pt,eta) if abs(lep.pdgId)==11 else muonFakeRate_UCSx(pt,eta)
            if var==0: return fr
            else:
                if isiso: alt = electronAlternativeFakeRate_UCSx_IsoTrigs(pt,eta) if abs(lep.pdgId)==11 else muonAlternativeFakeRate_UCSx_IsoTrigs(pt,eta)
                else: alt = electronAlternativeFakeRate_UCSx(pt,eta) if abs(lep.pdgId)==11 else muonAlternativeFakeRate_UCSx(pt,eta)
            if var==1: return alt
            elif var==-1: return 2*fr-alt
            else: raise RuntimeError
        else:
            FR_mu=self.FR_mu[var]
            FR_el=self.FR_el[var]
            h = FR_el[isiso] if abs(lep.pdgId)==11 else FR_mu[isiso]
            ptbin = max(1,min(h.GetNbinsX(),h.GetXaxis().FindBin(lep.conePt)))
            etabin = max(1,min(h.GetNbinsY(),h.GetYaxis().FindBin(abs(lep.eta))))
            return h.GetBinContent(ptbin,etabin)


    def FRtransfer_fromprob(self,prob):
        return prob/(1-prob)
    def FRtransfer(self,lep,ht,var):
        return self.FRtransfer_fromprob(self.FRprob(lep,ht,var))

    def read_FastSim_lepSF(self,pdgId,pt,eta,pu):
        if not hasattr(self,"FastSim_lepSF_histos"):
            print '-'*15
            print 'WARNING: will apply additional lepton scale factors for FastSim'
            print '-'*15
            self.FastSim_lepSF_histos=[]
            self._file_lepSF_FS=[]
            if not len(self.lepSFFileNameFastSim)==2: raise RuntimeError
            for fname in self.lepSFFileNameFastSim:
                self._file_lepSF_FS.append(ROOT.TFile(fname,'read'))
                self.FastSim_lepSF_histos.append(self._file_lepSF_FS[-1].Get("histo3D"))
        h = self.FastSim_lepSF_histos[1 if abs(pdgId)==11 else 0]
        sf = h.GetBinContent(h.GetXaxis().FindBin(pt),h.GetYaxis().FindBin(abs(eta)),h.GetZaxis().FindBin(pu))
        sferr = 0
        # this would be just stat error, ignored in favor of syst that is applied at datacard level
        #sferr = h.GetBinError(h.GetXaxis().FindBin(pt),h.GetYaxis().FindBin(eta),h.GetZaxis().FindBin(pu))
        if sf==0: raise RuntimeError, "Returning null lepton SF for FastSim (%d, %f, %f, %d)"%(pdgId,pt,eta,pu)
        return sf,(sferr/sf)

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
    def findPairs(self,leps1,leps2,byflav,bypassMV,choose_SS_else_OS=True):
        ret = None
        pairs = []
        _p = []
        for p in [(l1,l2) for l1 in leps1 for l2 in leps2 if l1!=l2]:
            if (p[1],p[0]) not in _p:
                _p.append(p)
        for (l1,l2) in _p:
                if not passMllVeto(l1, l2, 0, 8, False) and not bypassMV: continue
                flav = abs(l1.pdgId) + abs(l2.pdgId) if byflav else 0
                ht   = l1.conePt + l2.conePt
                if ((l1.charge == l2.charge) if choose_SS_else_OS else (l1.charge != l2.charge)):
                    pairs.append( (-flav,-ht,l1,l2) )
        if len(pairs):
            pairs.sort()
            ret = [(pair[2],pair[3]) for pair in pairs]
        return ret
    def SR(self, _l1pt, _l2pt, ht, met, nj, nb, mtw):
        l1pt, l2pt = (_l1pt,_l2pt) if _l1pt>=_l2pt else (_l2pt,_l1pt)
        if l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 1
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120  : SR = 2
        elif l1pt > 25 and l2pt > 25 and ht < 300 and nb == 0 and ((met > 50 and met < 200 and nj >= 5 and mtw < 120) or (met > 200 and met < 300 and nj >= 2 and mtw < 120) or (met > 50 and met < 300 and nj >= 2 and mtw > 120))   : SR = 3
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 5 and nb == 0 and mtw < 120  : SR = 4
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120  : SR = 5
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 5 and nb == 0 and mtw < 120  : SR = 6
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw > 120  : SR = 7
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and nb == 0 and ((met > 50 and met < 200 and nj >= 5 and mtw > 120) or (met > 200 and met < 300 and nj >= 2 and mtw > 120)) : SR = 8
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 9
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120  : SR = 10
        elif l1pt > 25 and l2pt > 25 and ht < 300 and nb == 1 and ((met > 50 and met < 200 and nj >= 5 and mtw < 120) or (met > 200 and met < 300 and nj >= 2 and mtw < 120) or (met > 50 and met < 300 and nj >= 2 and mtw > 120))   : SR = 11
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 5 and nb == 1 and mtw < 120  : SR = 12
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120  : SR = 13
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 5 and nb == 1 and mtw < 120  : SR = 14
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw > 120  : SR = 15
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and nb == 1 and ((met > 50 and met < 200 and nj >= 5 and mtw > 120) or (met > 200 and met < 300 and nj >= 2 and mtw > 120)) : SR = 16
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 17
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120  : SR = 18
        elif l1pt > 25 and l2pt > 25 and ht < 300 and nb == 2 and ((met > 50 and met < 200 and nj >= 5 and mtw < 120) or (met > 200 and met < 300 and nj >= 2 and mtw < 120) or (met > 50 and met < 300 and nj >= 2 and mtw > 120))   : SR = 19
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 5 and nb == 2 and mtw < 120  : SR = 20
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120  : SR = 21
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 5 and nb == 2 and mtw < 120  : SR = 22
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw > 120  : SR = 23
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and nb == 2 and ((met > 50 and met < 200 and nj >= 5 and mtw > 120) or (met > 200 and met < 300 and nj >= 2 and mtw > 120)) : SR = 24
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 25
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 26
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 200 and met < 300 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 27
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 28
        elif l1pt > 25 and l2pt > 25 and ht < 300 and met > 50 and met < 300 and nj >= 2 and nb >= 3 and mtw > 120 : SR = 29
        elif l1pt > 25 and l2pt > 25 and ht > 300 and ht < 1125 and met > 50 and met < 300 and nj >= 2 and nb >= 3 and mtw > 120 : SR = 30
        elif l1pt > 25 and l2pt > 25 and ht > 300 and met > 300 and nj >= 2 : SR = 31
        elif l1pt > 25 and l2pt > 25 and ht > 1125 and met > 50 and met < 300 and nj >= 2 : SR = 32
        ####
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 33 #1B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 34 #2B
        elif l1pt > 25 and l2pt < 25 and ht < 300  and ((met > 50 and met < 200 and nj >= 5) or (met > 200 and met < 300 and nj >= 2)) and nb == 0 and mtw < 120 : SR = 35 #3B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 5 and nb == 0 and mtw < 120 : SR = 36  #4B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nj <= 4 and nb == 0 and mtw < 120 : SR = 37 #5B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 5 and nb == 0 and mtw < 120 : SR = 38 #6B
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 39 #7B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 40 #8B
        elif l1pt > 25 and l2pt < 25 and ht < 300  and ((met > 50 and met < 200 and nj >= 5) or (met > 200 and met < 300 and nj >= 2)) and nb == 1 and mtw < 120 : SR = 41 #9B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 5 and nb == 1 and mtw < 120 : SR = 42 #10B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nj <= 4 and nb == 1 and mtw < 120 : SR = 43 #11B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 5 and nb == 1 and mtw < 120 : SR = 44 #12B
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 45 #13B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 46 #14B
        elif l1pt > 25 and l2pt < 25 and ht < 300  and ((met > 50 and met < 200 and nj >= 5) or (met > 200 and met < 300 and nj >= 2)) and nb == 2 and mtw < 120 : SR = 47 #15B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 5 and nb == 2 and mtw < 120 : SR = 48 #16B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nj <= 4 and nb == 2 and mtw < 120 : SR = 49 #17B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 5 and nb == 2 and mtw < 120 : SR = 50 #18B
       
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 51 #19B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 200 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 52 #20B
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 200 and met < 300 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 53 #21B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 200 and met < 300 and nj >= 2 and nb >= 3 and mtw < 120 : SR = 54 #21B       
        elif l1pt > 25 and l2pt < 25 and ht < 300 and met > 50 and met < 300 and nj >= 2 and mtw > 120 : SR = 55 #23B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and ht < 1125 and met > 50 and met < 300 and nj >= 2 and mtw > 120 : SR = 56 #24B
        elif l1pt > 25 and l2pt < 25 and ht > 300 and met > 300 and nj >= 2 : SR = 57 #25B
        elif l1pt > 25 and l2pt < 25 and ht > 1125 and met > 50 and met < 300 and nj >= 2 : SR = 58 #26B
        ####
        elif l1pt < 25 and l2pt < 25 and ht > 300 and met > 50 and met < 200 and nb == 0 and mtw < 120 : SR = 59 #C1 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and met > 200 and nb == 0 and mtw < 120 : SR = 60 #C2 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and met > 50 and met < 200 and nb == 1 and mtw < 120 : SR = 61 #C3 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and met > 200 and nb == 1 and mtw < 120 : SR = 62 #C4 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and met > 50 and met < 200 and nb == 2 and mtw < 120 : SR = 63 #C5 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and met > 200 and nb == 2 and mtw < 120 : SR = 64 #C6 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and nb >= 3 and mtw < 120 : SR = 65 #C7 
        elif l1pt < 25 and l2pt < 25 and ht > 300 and mtw > 120 : SR = 66 #C8 
        else : SR = 0 
        return SR

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf1 = LeptonChoiceRA5("Old", 
                lambda lep : lep.relIso03 < 0.5, 
                lambda lep : lep.relIso03 < 0.1 and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4))
            self.sf2 = LeptonChoiceRA5("PtRel", 
                lambda lep : lep.relIso03 < 0.4 or lep.jetPtRel > 5, 
                lambda lep : (lep.relIso03 < 0.1 or lep.jetPtRel > 14) and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4))
            self.sf3 = LeptonChoiceRA5("MiniIso", 
                lambda lep : lep.miniRelIso < 0.4, 
                lambda lep : lep.miniRelIso < 0.05 and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4))
            self.sf4 = LeptonChoiceRA5("PtRelJC", 
                lambda lep : lep.relIso03 < 0.4 or lep.jetPtRel > 5, 
                lambda lep : (lep.relIso03 < 0.1 or lep.jetPtRel > 14) and lep.sip3d < 4 and _susy2lss_lepId_CB(lep),
                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4 and not (lep.jetPtRel > 5 and lep.pt*(1/lep.jetPtRatio-1) > 25)))
            self.sf5 = LeptonChoiceRA5("MiniIsoJC", 
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

        

from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std, TRandom2
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator


def minValueForIdxList(values,idxlist):
    cleanedValueList = [val for i,val in enumerate(values) if i in idxlist]
    if len(cleanedValueList)>0: return min(cleanedValueList)
    else: return -999


class EventVars1L_bkgDilep:
    def __init__(self):
        self.branches = [ 
                          "DL_LepGoodOne_pt", "DL_l1l2ovMET", "DL_Vecl1l2ovMET", "DL_DPhil1l2",
                          ("nLostLepTreatments","I"),
                          ("DL_ST","F",10,"nLostLepTreatments"),("DL_dPhiLepW","F",10,"nLostLepTreatments")
 
                          ]


    def listBranches(self):
        return self.branches[:]

    def __call__(self,event,base = {}):

        # prepare output
        ret = {}
        for name in self.branches:
            if type(name) == 'tuple':
                ret[name] = []
            elif type(name) == 'str':
                ret[name] = -999.0

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
        centralJet30 = [jets[idx] for idx in centralJet30idx]
        nCentralJet30 = len(centralJet30)

        # B jets
        BJetMedium30 = []
        BJetMedium30idx = base['BJetMedium30idx']
        nBJetMedium30 = base['nBJetMedium30']

        '''
        for idx,jet in enumerate(centralJet30):
        if idx in BJetMedium30idx:
        BJetMedium30.append(jet)
        '''

        #print 'here',event.evt, nTightLeps, len(centralJet30), nBJetMedium30

        ##################################################################
        # The following variables need to be double-checked for validity #
        ##################################################################

        ## B tagging WPs for CSVv2 (CSV-IVF)
        ## L: 0.423, M: 0.814, T: 0.941
        ## from: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating

        bTagWP = 0.814 # MediumWP for CSVv2
        #bTagWP = 0.732 # MediumWP for CMVA


        # deltaPhi between the (single) lepton and the reconstructed W (lep + MET)
        # ST of lepton and MET
        DL_ST = []
        DL_dPhiLepW = []


        LepToKeep_pt = -999
        l1l2ovMET = -999
        Vecl1l2ovMET = -999

        DPhil1l2 = -999
        
        if len(tightLeps)==2:
            passPreSel = False
            SumP4 = tightLeps[0].p4()+tightLeps[1].p4()
            if tightLeps[0].charge!=tightLeps[1].charge: passPreSel= True
            if tightLeps[0].pdgId==-tightLeps[1].pdgId and abs(SumP4.M()-91.2)<10.: passPreSel= False
            
            if passPreSel:
                random = TRandom2(event.evt*event.lumi)
                uniform01 = random.Rndm()
                lepToKeep = int(uniform01>0.5)
                lepToDiscard = int(not lepToKeep)
                
                Met2D = TVector2(metp4.Px(),metp4.Py())
                LepToDiscard2D = TVector2(tightLeps[lepToDiscard].p4().Px(), tightLeps[lepToDiscard].p4().Py())
                LepToKeep2D = TVector2(tightLeps[lepToKeep].p4().Px(), tightLeps[lepToKeep].p4().Py())

                Met2D_AddFull = Met2D + LepToDiscard2D
                Met2D_AddThird = Met2D + (1/3.*LepToDiscard2D)
                LepToKeep_pt = LepToKeep2D.Mod()
                
                recoWp4 = LepToKeep2D + Met2D
                DL_dPhiLepW.append(LepToKeep2D.DeltaPhi(recoWp4)) # [0]: not adding leptons to MET
                DL_ST.append(LepToKeep2D.Mod() + Met2D.Mod())

                recoWp4_AddFull = LepToKeep2D + Met2D_AddFull
                DL_dPhiLepW.append(LepToKeep2D.DeltaPhi(recoWp4_AddFull))# [0]: adding lost lepton pt to met
                DL_ST.append(LepToKeep2D.Mod() + Met2D_AddFull.Mod())

                recoWp4_AddThird = LepToKeep2D + Met2D_AddThird
                DL_dPhiLepW.append(LepToKeep2D.DeltaPhi(recoWp4_AddThird))# [2]: adding 1/3 of lepton ptto met 
                DL_ST.append(LepToKeep2D.Mod() + Met2D_AddThird.Mod())

                l1l2ovMET = (tightLeps[0].pt + tightLeps[1].pt)/metp4.Pt()
                Vecl1l2ovMET = (LepToKeep2D + LepToDiscard2D).Mod()/metp4.Pt()

                DPhil1l2 = LepToKeep2D.DeltaPhi(LepToDiscard2D)

        ret["nLostLepTreatments"]=3
        if len(DL_ST)!=ret["nLostLepTreatments"]:
            for i in range(0,ret["nLostLepTreatments"]):
                DL_ST.append(-999)
                DL_dPhiLepW.append(-999)
        
        ret["DL_ST"]    =DL_ST
        ret["DL_dPhiLepW"] = DL_dPhiLepW

        ret['DL_LepGoodOne_pt'] = LepToKeep_pt
        ret['DL_l1l2ovMET'] = l1l2ovMET
        ret['DL_Vecl1l2ovMET'] = Vecl1l2ovMET
        ret['DL_DPhil1l2'] = DPhil1l2

        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1L_bkgDilep()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev,{})
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

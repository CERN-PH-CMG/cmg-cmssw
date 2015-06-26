from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator

def getPhysObjectArray(j): # https://github.com/HephySusySW/Workspace/blob/72X-master/RA4Analysis/python/mt2w.py
    px = j.pt*cos(j.phi )
    py = j.pt*sin(j.phi )
    pz = j.pt*sinh(j.eta )
    E = sqrt(px*px+py*py+pz*pz) #assuming massless particles...
    return array.array('d', [E, px, py,pz])

def mt_2(p4one, p4two):
    return sqrt(2*p4one.Pt()*p4two.Pt()*(1-cos(p4one.Phi()-p4two.Phi())))

def GetZfromM(vector1,vector2,mass):
    MT = sqrt(2*vector1.Pt()*vector2.Pt()*(1-cos(vector2.DeltaPhi(vector1))))
    if (MT<mass):
        Met2D = TVector2(vector2.Px(),vector2.Py())
        Lep2D = TVector2(vector1.Px(),vector1.Py())
        A = mass*mass/2.+Met2D*Lep2D
        Delta = vector1.E()*vector1.E()*(A*A-Met2D.Mod2()*Lep2D.Mod2())
        MetZ1 = (A*vector1.Pz()+sqrt(Delta))/Lep2D.Mod2()
        MetZ2 = (A*vector1.Pz()-sqrt(Delta))/Lep2D.Mod2()
    else:
        MetZ1 =vector1.Pz()*vector2.Pt()/vector1.Pt()
        MetZ2 =vector1.Pz()*vector2.Pt()/vector1.Pt()
    return [MT,MetZ1,MetZ2]

def minValueForIdxList(values,idxlist):
    cleanedValueList = [val for i,val in enumerate(values) if i in idxlist]
    if len(cleanedValueList)>0: return min(cleanedValueList)
    else: return -999
#  print cleanedValueList, min(cleanedValueList)#d, key=d.get)


class EventVars1L_Top:
    def __init__(self):
        self.branches = [ "minDPhiBMET", "minDPhiJMET", "idxMinDPhiBMET", "mTClBPlusMET", "mTBJetMET", "mTLepMET", "mLepBJet",
                          ("LepBMass","F",50,"nCentralJet30"), ("MTbnu","F",50,"nCentralJet30"),("Mtop","F",50,"nCentralJet30"),
                          ("MTtop","F",50,"nCentralJet30"), ("METovTop","F",50,"nCentralJet30"),("METTopPhi","F",50,"nCentralJet30"),
                          ("MtopDecor","F",50,"nCentralJet30"),
                          ("TopPt","F",50,"nCentralJet30"),("TopEt","F",50,"nCentralJet30"),
                          ("nBMinVariantsTopVars","I"),
                          ("TopVarsMTbnuMin","F",10,"nBMinVariantsTopVars"),("TopVarsLepBMassMin","F",10,"nBMinVariantsTopVars"),
                          ("TopVarsMTtopMin","F",10,"nBMinVariantsTopVars"),("TopVarsMtopMin","F",10,"nBMinVariantsTopVars"),
                          ("TopVarsMETovTopMin","F",10,"nBMinVariantsTopVars"),("TopVarsMtopDecorMin","F",10,"nBMinVariantsTopVars"),
                          ("TopVarsTopPtMin","F",10,"nBMinVariantsTopVars"),("TopVarsTopEtMin","F",10,"nBMinVariantsTopVars"),
                          "MTW","MW1","MW2",
                          'minDphiLepB','minDphiLepBidx'
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

        # min deltaPhi between a b-jet and MET; needs to be double-checked
        minDPhiBMET    = 100
        idxMinDPhiBMET = -999
        for i, jet in enumerate(centralJet30):
            if jet.btagCSV>bTagWP:
                dPhiBMET = abs(jet.p4().DeltaPhi(metp4))
                if dPhiBMET<minDPhiBMET:
                    minDPhiBMET=dPhiBMET
                    idxMinDPhiBMET = i

        # first define output dict 'ret'
        ret["idxMinDPhiBMET"] = idxMinDPhiBMET
        #ret = { 'idxMinDPhiBMET' : idxMinDPhiBMET }
        ret["minDPhiBMET"] = minDPhiBMET

        # min deltaPhi between a jet (first three jets) and MET; needs to be double-checked
        minDPhiJMET    = 100
        for i, jet in enumerate(centralJet30[:3]):
            dPhiJMET = abs(jet.p4().DeltaPhi(metp4))
            if dPhiJMET<minDPhiJMET:
                minDPhiJMET=dPhiJMET

        ret["minDPhiJMET"] = minDPhiJMET

        # transverse mass of (closest (to MET) BJet, MET), (closest (to MET) BJet, lepton),
        # mass of (closest (to MET) BJet, lepton); need to be double-checked
        mTBJetMET      = -999
        mTLepMET       = -999
        mLepBJet       = -999
        if(idxMinDPhiBMET>=0):
            SumMetClosestBJet = jets[idxMinDPhiBMET].p4() + metp4
            ret["mTClBPlusMET"] = SumMetClosestBJet.Mt()
            mTBJetMET = mt_2(centralJet30[idxMinDPhiBMET].p4(),metp4)
            if nTightLeps>=1:
                mLepBJet = (centralJet30[idxMinDPhiBMET].p4() + tightLeps[0].p4()).M()
                mTLepMET = mt_2(tightLeps[0].p4(),metp4)
        else:
            ret["mTClBPlusMET"] = -999

        ret["mTBJetMET"] = mTBJetMET
        ret["mTLepMET"]  = mTLepMET
        ret["mLepBJet"]  = mLepBJet

        # projection of MET along (MET + lepton + (closest (to MET) BJet)) sum; needs to be double-checked...
        MetZ1 = -9999
        MetZ2 = -9999
        MTW = -9999
        MW1 = -9999
        MW2 = -9999
        neutrino1 = ROOT.TLorentzVector(0,0,0,0)
        neutrino2 = ROOT.TLorentzVector(0,0,0,0)
        if(nTightLeps==1) :
            NeutrZList = GetZfromM(tightLeps[0].p4(),metp4,81)
            MTW = NeutrZList[0]
            MetZ1= NeutrZList[1]
            MetZ2= NeutrZList[2]
            neutrino1.SetXYZM(metp4.Px(),metp4.Py(), MetZ1, 0)
            neutrino2.SetXYZM(metp4.Px(),metp4.Py(), MetZ2, 0)
            MW1 = (neutrino1+tightLeps[0].p4()).M()
            MW2 = (neutrino2+tightLeps[0].p4()).M()
        ret["MTW"]  = MTW
        ret["MW1"]  = MW1
        ret["MW2"]  = MW2
        # some extra plots

        MTbnu = []
        LepBMass = []
        MTtop = []
        Mtop = []
        METovTop = []
        METTopPhi = []
        MtopDecor = []

        TopEt = []
        TopPt = []

        if(nTightLeps==1) :
            for i,jet in  enumerate(centralJet30): #testing all jets as b-jet in top-reco
                ThisMTnub = sqrt(2*event.met_pt*jet.pt* (1-cos( metp4.DeltaPhi(jet.p4() ))))
                MTbnu.append(ThisMTnub)

                # lep + jet vector
                lepJp4 = tightLeps[0].p4()+jet.p4()

                ThislepBMass = lepJp4.M()
                LepBMass.append(ThislepBMass )

                # top vector: MET + lep + jet
                topP4 = metp4+lepJp4
                TopEt.append(topP4.Et())
                TopPt.append(topP4.Pt())

                ThisMTtop =  sqrt( 81.*81. + ThislepBMass *ThislepBMass + ThisMTnub*ThisMTnub)
                MTtop.append(ThisMTtop)

                ThisMetovTop =  event.met_pt/ topP4.Pt()
                METovTop.append(ThisMetovTop)

                ThisMetTop = metp4.DeltaPhi(metp4+lepJp4)
                METTopPhi.append(ThisMetTop)

                TopMass1 = (neutrino1+lepJp4).M()
                TopMass2 = (neutrino2+lepJp4).M()

                #take smaller mtop of the two nu pz-solutions
                if TopMass1 > TopMass2:
                    Mtop.append(TopMass2)
                else:
                    Mtop.append(TopMass1)

                ThisMtopDecor1  = sqrt(lepJp4.M()*lepJp4.M()+ (neutrino1+jet.p4()).M()*(neutrino1+jet.p4()).M()+81*81)
                ThisMtopDecor2  = sqrt(lepJp4.M()*lepJp4.M()+ (neutrino2+jet.p4()).M()*(neutrino2+jet.p4()).M()+81*81)

                if ThisMtopDecor1 > ThisMtopDecor2:
                    MtopDecor.append(ThisMtopDecor2)
                else:
                    MtopDecor.append(ThisMtopDecor1)

        # fill them
        ret["MTbnu"] =MTbnu
        ret["LepBMass"]=LepBMass
        ret["MTtop"]=MTtop
        ret["Mtop"]=Mtop
        ret["METovTop"]=METovTop
        ret["METTopPhi"]=METTopPhi
        ret["MtopDecor"]=MtopDecor

        ret['TopPt'] = TopPt
        ret['TopEt'] = TopEt

        # nearest b jet to lead lepton
        minDphiLepB = 100
        minDphiLepBidx = -1

        if nTightLeps == 1:
            for i, jet in enumerate(centralJet30):
                if jet.btagCSV>bTagWP:
                    dPhiLepB = abs(jet.p4().DeltaPhi(tightLeps[0].p4()))
                    if dPhiLepB < minDphiLepB:
                        minDphiLepB = dPhiLepB
                        minDphiLepBidx = i

        ret["minDphiLepB"] = minDphiLepB
        ret["minDphiLepBidx"] = minDphiLepBidx

        #        TopVarsJetIdx = []
        TopVarsMTbnuMin = []
        TopVarsLepBMassMin = []
        TopVarsMTtopMin = []
        TopVarsMtopMin = []
        TopVarsMETovTopMin = []
        TopVarsMtopDecorMin = []

        TopVarsTopPtMin = []
        TopVarsTopEtMin = []

        iBTagDict = {i: jets[idx].btagCSV for i, idx in enumerate(centralJet30idx)}
        sortIdsByBTag = sorted(iBTagDict.items(), key=operator.itemgetter(1), reverse=True)
        bTaggedJetsSorted = sortIdsByBTag[:nBJetMedium30]
        #        print bTaggedJetsSorted
        bTaggedJetsPPSorted = sortIdsByBTag[:nBJetMedium30+1]
        #        print bTaggedJetsPPSorted
        ThreeBestBTags = sortIdsByBTag[:3]
        #        print ThreeBestBTags
        #        print sortIdsByBTag

        if nTightLeps == 1 and nCentralJet30 > 0:
            #0: minimum of b-tagged jets
            TopVarsMTbnuMin      .append(minValueForIdxList(MTbnu     , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsLepBMassMin   .append(minValueForIdxList(LepBMass  , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsMTtopMin      .append(minValueForIdxList(MTtop     , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsMtopMin       .append(minValueForIdxList(Mtop      , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsMETovTopMin   .append(minValueForIdxList(METovTop  , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsMtopDecorMin  .append(minValueForIdxList(MtopDecor , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsTopPtMin      .append(minValueForIdxList(TopPt     , [ids[0] for ids in bTaggedJetsSorted]))
            TopVarsTopEtMin      .append(minValueForIdxList(TopEt     , [ids[0] for ids in bTaggedJetsSorted]))

            #1: minimum of b-tagged jets (+ adding next-best b-disc. jet)
            TopVarsMTbnuMin      .append(minValueForIdxList(MTbnu     , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsLepBMassMin   .append(minValueForIdxList(LepBMass  , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsMTtopMin      .append(minValueForIdxList(MTtop     , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsMtopMin       .append(minValueForIdxList(Mtop      , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsMETovTopMin   .append(minValueForIdxList(METovTop  , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsMtopDecorMin  .append(minValueForIdxList(MtopDecor , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsTopPtMin      .append(minValueForIdxList(TopPt     , [ids[0] for ids in bTaggedJetsPPSorted]))
            TopVarsTopEtMin      .append(minValueForIdxList(TopEt     , [ids[0] for ids in bTaggedJetsPPSorted]))

            #2: always consider the three jets with the best b-tag values (no pass of any working-point required)
            TopVarsMTbnuMin      .append(minValueForIdxList(MTbnu     , [ids[0] for ids in ThreeBestBTags]))
            TopVarsLepBMassMin   .append(minValueForIdxList(LepBMass  , [ids[0] for ids in ThreeBestBTags]))
            TopVarsMTtopMin      .append(minValueForIdxList(MTtop     , [ids[0] for ids in ThreeBestBTags]))
            TopVarsMtopMin       .append(minValueForIdxList(Mtop      , [ids[0] for ids in ThreeBestBTags]))
            TopVarsMETovTopMin   .append(minValueForIdxList(METovTop  , [ids[0] for ids in ThreeBestBTags]))
            TopVarsMtopDecorMin  .append(minValueForIdxList(MtopDecor , [ids[0] for ids in ThreeBestBTags]))
            TopVarsTopPtMin      .append(minValueForIdxList(TopPt     , [ids[0] for ids in ThreeBestBTags]))
            TopVarsTopEtMin      .append(minValueForIdxList(TopEt     , [ids[0] for ids in ThreeBestBTags]))

            mcMatchIdLep = tightLeps[0].mcMatchId
            iCorrectJet=-999
            correctJetBTagged = False
            if abs(mcMatchIdLep)==6:
                for i,jet in  enumerate(centralJet30):
                    if abs(jet.mcFlavour)==5 and jet.mcMatchId==mcMatchIdLep:
                        iCorrectJet=i
                        if jet.btagCSV>bTagWP: correctJetBTagged=True

            #3: value for the correct b-jet (i.e. the one matching the lepton)
            TopVarsMTbnuMin      .append(MTbnu     [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsLepBMassMin   .append(LepBMass  [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsMTtopMin      .append(MTtop     [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsMtopMin       .append(Mtop      [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsMETovTopMin   .append(METovTop  [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsMtopDecorMin  .append(MtopDecor [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsTopPtMin      .append(TopPt     [iCorrectJet] if iCorrectJet>-999 else -999)
            TopVarsTopEtMin      .append(TopEt     [iCorrectJet] if iCorrectJet>-999 else -999)

            foundCorrectBJetAndIsTagged = iCorrectJet>-999 and correctJetBTagged

            #4: value for the correct b-jet (if actually identified as b-jet (tagged))
            TopVarsMTbnuMin      .append(MTbnu     [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsLepBMassMin   .append(LepBMass  [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsMTtopMin      .append(MTtop     [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsMtopMin       .append(Mtop      [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsMETovTopMin   .append(METovTop  [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsMtopDecorMin  .append(MtopDecor [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsTopPtMin      .append(TopPt     [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)
            TopVarsTopEtMin      .append(TopEt     [iCorrectJet] if foundCorrectBJetAndIsTagged else -999)

            #5: consider the jet closest in dphi to MET
            TopVarsMTbnuMin      .append(MTbnu    [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsLepBMassMin   .append(LepBMass [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsMTtopMin      .append(MTtop    [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsMtopMin       .append(Mtop     [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsMETovTopMin   .append(METovTop [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsMtopDecorMin  .append(MtopDecor[idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsTopPtMin      .append(TopPt    [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            TopVarsTopEtMin      .append(TopEt    [idxMinDPhiBMET] if idxMinDPhiBMET!=-999 else -999)
            
            #6: nearest to lepton b jet
            TopVarsMTbnuMin      .append(MTbnu[minDphiLepBidx]    if minDphiLepBidx>-1 else -999)
            TopVarsLepBMassMin   .append(LepBMass[minDphiLepBidx] if minDphiLepBidx>-1 else -999)
            TopVarsMTtopMin      .append(MTtop[minDphiLepBidx]    if minDphiLepBidx>-1 else -999)
            TopVarsMtopMin       .append(Mtop[minDphiLepBidx]     if minDphiLepBidx>-1 else -999)
            TopVarsMETovTopMin   .append(METovTop[minDphiLepBidx] if minDphiLepBidx>-1 else -999)
            TopVarsMtopDecorMin  .append(MtopDecor[minDphiLepBidx]if minDphiLepBidx>-1 else -999)
            TopVarsTopPtMin      .append(TopPt[minDphiLepBidx]    if minDphiLepBidx>-1 else -999)
            TopVarsTopEtMin      .append(TopEt[minDphiLepBidx]    if minDphiLepBidx>-1 else -999)

        else:
            for i in range(7):
                TopVarsMTbnuMin      .append(-999)
                TopVarsLepBMassMin   .append(-999)
                TopVarsMTtopMin      .append(-999)
                TopVarsMtopMin       .append(-999)
                TopVarsMETovTopMin   .append(-999)
                TopVarsMtopDecorMin  .append(-999)
                TopVarsTopPtMin      .append(-999)
                TopVarsTopEtMin      .append(-999)


        ret["nBMinVariantsTopVars"]=7

        ret["TopVarsMTbnuMin"]    =TopVarsMTbnuMin
        ret["TopVarsLepBMassMin"] =TopVarsLepBMassMin
        ret["TopVarsMTtopMin"]    =TopVarsMTtopMin
        ret["TopVarsMtopMin"]     =TopVarsMtopMin
        ret["TopVarsMETovTopMin"] =TopVarsMETovTopMin
        ret["TopVarsMtopDecorMin"]=TopVarsMtopDecorMin
        ret["TopVarsTopPtMin"]    =TopVarsTopPtMin
        ret["TopVarsTopEtMin"]    =TopVarsTopEtMin

        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1L_top()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev,{})
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

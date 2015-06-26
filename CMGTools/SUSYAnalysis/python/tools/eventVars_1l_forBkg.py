from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator

# Cuts and WP

# Tight MVA WP
Ele_mvaPhys14_eta0p8_T = 0.73;
Ele_mvaPhys14_eta1p4_T = 0.57;
Ele_mvaPhys14_eta2p4_T = 0.05;

# Medium MVA WP  <--- UPDATE
Ele_mvaPhys14_eta0p8_M = 0.35;
Ele_mvaPhys14_eta1p4_M = 0.20;
Ele_mvaPhys14_eta2p4_M = -0.52;

# Loose MVA WP
Ele_mvaPhys14_eta0p8_L = 0.35;
Ele_mvaPhys14_eta1p4_L = 0.20;
Ele_mvaPhys14_eta2p4_L = -0.52;


def checkEleMVA(lepMVA,lepEta,WP = 'Tight'):
    # Eta dependent MVA ID check:
    passID = False

    if WP == 'Tight':
        if lepEta < 0.8 and lepMVA > Ele_mvaPhys14_eta0p8_T: passID = True
        elif lepEta >= 0.8 and lepEta < 1.44 and lepMVA > Ele_mvaPhys14_eta1p4_T: passID = True
        elif lepEta >= 1.57 and lepEta < 2.4 and lepMVA > Ele_mvaPhys14_eta2p4_T: passID = True
    elif WP == 'Medium':
        if lepEta < 0.8 and lepMVA > Ele_mvaPhys14_eta0p8_M: passID = True
        elif lepEta >= 0.8 and lepEta < 1.44 and lepMVA > Ele_mvaPhys14_eta1p4_M: passID = True
        elif lepEta >= 1.57 and lepEta < 2.4 and lepMVA > Ele_mvaPhys14_eta2p4_M: passID = True
    elif WP == 'Loose':
        if lepEta < 0.8 and lepMVA > Ele_mvaPhys14_eta0p8_L: passID = True
        elif lepEta >= 0.8 and lepEta < 1.44 and lepMVA > Ele_mvaPhys14_eta1p4_L: passID = True
        elif lepEta >= 1.57 and lepEta < 2.4 and lepMVA > Ele_mvaPhys14_eta2p4_L: passID = True

    return passID

class EventVars1L_base:
    def __init__(self):
        self.branches = [
            'Run','Event','Lumi','Xsec',
            ("nTightLeps","I"),("tightLepsIdx","I",10,"nTightLeps"),("nVetoLeps","I"),("vetoLepsIdx","I",10,"nVetoLeps"),
            ("tightLeps_DescFlag","I",10,"nTightLeps"),
            ("nTightEl","I"),("tightElIdx","I",10,"nTightEl"),("nVetoEl","I"),("vetoElIdx","I",10,"nVetoEl"),
            ("nTightMu","I"),("tightMuIdx","I",10,"nTightMu"),("nVetoMu","I"),("vetoMuIdx","I",10,"nVetoMu"),
            'LepGood1_pdgId','LepGood1_eta','LepGood1_phi','Selected',
            'Lp', "MET", "DeltaPhiLepW",
            'HT','ST','LepGood1_pt',
            ("nCentralJet30","I"),("centralJet30idx","I",100,"nCentralJet30"),("centralJet30_DescFlag","F",100,"nCentralJet30"),
            ("nBJetMedium30","I"),("BJetMedium30idx","I",50,"nBJetMedium30"),
            "nGoodBJets_allJets", "nGoodBJets",
            "LSLjetptGT80", "htJet30j", "htJet30ja",
            "nHighPtTopTag", "nHighPtTopTagPlusTau23"
            ]

    def listBranches(self):
        return self.branches[:]

    def __call__(self,event,keyvals):

        # prepare output
        #ret = dict([(name,-999.0) for name in self.branches])
        ret = {}
        for name in self.branches:
            if type(name) == 'tuple':
                ret[name] = []
            elif type(name) == 'str':
                ret[name] = -999

        # copy basic event info:
        ret['Run'] = event.run
        ret['Event'] = event.evt
        ret['Lumi'] = event.lumi
        ret['Xsec'] = event.xsec

        # make python lists as Collection does not support indexing in slices
        genleps = [l for l in Collection(event,"genLep","ngenLep")]
        leps = [l for l in Collection(event,"LepGood","nLepGood")]

        jets = [j for j in Collection(event,"Jet","nJet")]
        fatjets = [j for j in Collection(event,"FatJet","nFatJet")]

        metp4 = ROOT.TLorentzVector(0,0,0,0)
        metp4.SetPtEtaPhiM(event.met_pt,event.met_eta,event.met_phi,event.met_mass)

        #isolation criteria as defined for PHYS14 1l synchronisation exercise
        centralEta = 2.4

        ele_minirelisoCut = 0.1
        muo_minirelisoCut = 0.2
        Lep_minirelisoCut = 0.4

        Ele_lostHits = 0
        Ele_sip3d = 4

        Ele_mvaPhys14_eta0p8_T = 0.73;
        Ele_mvaPhys14_eta1p4_T = 0.57;
        Ele_mvaPhys14_eta2p4_T = 0.05;

        ### LEPTONS

        selectedTightLeps = []
        selectedTightLepsIdx = []
        selectedVetoLeps = []
        selectedVetoLepsIdx = []

        antiTightLeps = []
        antiTightLepsIdx = []
        antiVetoLeps = []
        antiVetoLepsIdx = []

        selectedTightMu = []
        selectedTightMuIdx = []
        selectedVetoMu = []
        selectedVetoMuIdx = []

        antiTightMu = []
        antiTightMuIdx = []
        antiVetoMu = []
        antiVetoMuIdx = []

        selectedTightEl = []
        selectedTightElIdx = []
        selectedVetoEl = []
        selectedVetoElIdx = []

        antiTightEl = []
        antiTightElIdx = []
        antiVetoEl = []
        antiVetoElIdx = []

        for idx,lep in enumerate(leps):

            # check acceptance
            lepEta = abs(lep.eta)

            if(lepEta > 2.5): continue

            # muons
            if(abs(lep.pdgId) == 13):
                # pass variables
                passID = False
                passIso = False

                # selected: pt > 25
                if lep.pt > 25:

                    # all selected leptons are veto for anti
                    antiVetoLeps.append(lep); antiVetoLepsIdx.append(idx);
                    antiVetoMu.append(lep); antiVetoMuIdx.append(idx);

                    # ID and Iso check:
                    if lep.mediumMuonId == 1 and lep.sip3d < 4.0: passID = True
                    if lep.miniRelIso < muo_minirelisoCut:        passIso = True

                    # fill
                    if passID and passIso:
                        selectedTightLeps.append(lep); selectedTightLepsIdx.append(idx);
                        selectedTightMu.append(lep); selectedTightMuIdx.append(idx);
                    else:
                        selectedVetoLeps.append(lep); selectedVetoLepsIdx.append(idx);
                        selectedVetoMu.append(lep); selectedVetoMuIdx.append(idx);

                # anti muons + tight veto
                elif lep.pt > 5:
                    # veto for tight if pt > 10
                    if lep.pt > 10:
                        selectedVetoLeps.append(lep); selectedVetoLepsIdx.append(idx);
                        selectedVetoMu.append(lep); selectedVetoMuIdx.append(idx);
                    # Anti leptons
                    # ID check
                    if lep.mediumMuonId == 1 and lep.sip3d < 4.0: passID = True
                    # iso check
                    if lep.pt < 15   and lep.miniRelIso < Lep_minirelisoCut: passIso = True
                    elif lep.pt > 15 and lep.miniRelIso < muo_minirelisoCut: passIso = True

                    # fill
                    if passID and passIso:
                        antiTightLeps.append(lep); antiTightLepsIdx.append(idx);
                        antiTightMu.append(lep); antiTightMuIdx.append(idx);
                    else:
                        antiVetoLeps.append(lep); antiVetoLepsIdx.append(idx);
                        antiVetoMu.append(lep); antiVetoMuIdx.append(idx);

            # electrons
            if(abs(lep.pdgId) == 11):

                if lep.pt < 25: continue

                # pass variables
                passIso = False
                passConv = False

                lepMVA = lep.mvaIdPhys14

                # check MVA WPs
                passTightID = checkEleMVA(lepMVA,lepEta,'Tight')
                passLooseID = checkEleMVA(lepMVA,lepEta,'Loose')

                # selected
                if passTightID:

                    # all selected leptons are veto for anti
                    antiVetoLeps.append(lep); antiVetoLepsIdx.append(idx);
                    antiVetoEl.append(lep); antiVetoElIdx.append(idx);

                    # Iso check:
                    if lep.miniRelIso < ele_minirelisoCut: passIso = True
                    # conversion check
                    if lep.lostHits <= Ele_lostHits and lep.convVeto and lep.sip3d < Ele_sip3d: passConv = True

                    # fill
                    if passIso and passConv:
                        selectedTightLeps.append(lep); selectedTightLepsIdx.append(idx);
                        selectedTightEl.append(lep); selectedTightElIdx.append(idx);
                    else:
                        selectedVetoLeps.append(lep); selectedVetoLepsIdx.append(idx);
                        selectedVetoEl.append(lep); selectedVetoElIdx.append(idx);

                # anti-selected
                elif passLooseID:

                    # all anti leptons are veto for selected
                    selectedVetoLeps.append(lep); selectedVetoLepsIdx.append(idx);
                    selectedVetoEl.append(lep); selectedVetoElIdx.append(idx);

                    # Iso check:
                    if lep.miniRelIso < 0.4: passIso = True
                    # conversion check
                    passConv = True

                    # fill
                    if passIso and passConv:
                        antiTightLeps.append(lep); antiTightLepsIdx.append(idx);
                        antiTightEl.append(lep); antiTightElIdx.append(idx);
                    else:
                        antiVetoLeps.append(lep); antiVetoLepsIdx.append(idx);
                        antiVetoEl.append(lep); antiVetoElIdx.append(idx);
                else:
                    # the rest is veto for selected and anti
                    selectedVetoLeps.append(lep); selectedVetoLepsIdx.append(idx);
                    selectedVetoEl.append(lep); selectedVetoElIdx.append(idx);

                    antiVetoLeps.append(lep); antiVetoLepsIdx.append(idx);
                    antiVetoEl.append(lep); antiVetoElIdx.append(idx);
        # end lepton loop

        # choose common lepton collection: select selected or anti lepton
        if len(selectedTightLeps) > 0:
            tightLeps = selectedTightLeps
            tightLepsIdx = selectedTightLepsIdx
            vetoLeps = selectedVetoLeps
            vetoLepsIdx = selectedVetoLepsIdx

            tightEl = selectedTightEl
            tightElIdx = selectedTightElIdx
            vetoEl = selectedVetoEl
            vetoElIdx = selectedVetoElIdx

            tightMu = selectedTightMu
            tightMuIdx = selectedTightMuIdx
            vetoMu = selectedVetoMu
            vetoMuIdx = selectedVetoMuIdx

            ret['Selected'] = 1

        else: #if len(antiTightLeps) > 0: or empty collection
            tightLeps = antiTightLeps
            tightLepsIdx = antiTightLepsIdx
            vetoLeps = antiVetoLeps
            vetoLepsIdx = antiVetoLepsIdx

            tightEl = antiTightEl
            tightElIdx = antiTightElIdx
            vetoEl = antiVetoEl
            vetoElIdx = antiVetoElIdx

            tightMu = antiTightMu
            tightMuIdx = antiTightMuIdx
            vetoMu = antiVetoMu
            vetoMuIdx = antiVetoMuIdx

            if len(antiTightLeps) > 0:
                ret['Selected'] = -1

        #ret = { 'nTightLeps'   : len(tightLeps) }
        ret['nTightLeps'] = len(tightLeps)
        ret['tightLepsIdx'] = tightLepsIdx
        ret['nVetoLeps'] = len(vetoLeps)
        ret['vetoLepsIdx'] = vetoLepsIdx

        ret['nTightEl'] = len(tightEl)
        ret['tightElIdx'] = tightElIdx
        ret['nVetoEl'] = len(vetoEl)
        ret['vetoElIdx'] = vetoElIdx

        ret['nTightMu'] = len(tightMu)
        ret['tightMuIdx'] = tightMuIdx
        ret['nVetoMu'] = len(vetoMu)
        ret['vetoMuIdx'] = vetoMuIdx

        # save leading lepton vars

        if len(tightLeps) > 0:
            ret['LepGood1_pt'] = tightLeps[0].pt
            ret['LepGood1_eta'] = tightLeps[0].eta
            ret['LepGood1_phi'] = tightLeps[0].phi
            ret['LepGood1_pdgId'] = tightLeps[0].pdgId
        elif len(leps) > 0: # fill it with leading lepton
            ret['LepGood1_pt'] = leps[0].pt
            ret['LepGood1_eta'] = leps[0].eta
            ret['LepGood1_phi'] = leps[0].phi
            ret['LepGood1_pdgId'] = leps[0].pdgId

        ### JETS

        centralJet30 = []
        centralJet30idx = []
        for i,j in enumerate(jets):
            if j.pt>30 and abs(j.eta)<centralEta:
                centralJet30.append(j)
                centralJet30idx.append(i)

        ret['nCentralJet30']   = len(centralJet30)
        ret['centralJet30idx'] = centralJet30idx

        ret['LSLjetptGT80'] = 1 if sum([j.pt>80 for j in centralJet30])>=2 else 0

        ret['htJet30j']  = sum([j.pt for j in centralJet30])
        ret['htJet30ja'] = sum([j.pt for j in jets if j.pt>30])

        ret['HT'] = ret['htJet30j']

        ## B tagging WPs for CSVv2 (CSV-IVF)
        ## L: 0.423, M: 0.814, T: 0.941
        ## from: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating

        bTagWP = 0.814 # MediumWP for CSVv2
        #bTagWP = 0.732 # MediumWP for CMVA

        BJetMedium30 = []
        BJetMedium30idx = []

        for i,j in enumerate(centralJet30):
            if j.btagCSV>bTagWP:
                BJetMedium30.append(j)
                BJetMedium30idx.append(centralJet30idx[i])

        ret['nBJetMedium30']    = len(BJetMedium30)
        ret['BJetMedium30idx']  = BJetMedium30idx

        ret['nGoodBJets']    = sum([j.btagCSV > bTagWP for j in centralJet30])
        ret['nGoodBJets_allJets']    = sum([j.btagCSV > bTagWP and j.pt>30 and abs(j.eta)<centralEta for j in jets])

        #plain copy of MET pt (just as an example and cross-check for proper friend tree production)
        ret["MET"] = metp4.Pt()

        # deltaPhi between the (single) lepton and the reconstructed W (lep + MET)
        dPhiLepW = -999 # set default value to -999 to spot "empty" entries
        # ST of lepton and MET
        ST = -999
        Lp = -99

        if len(tightLeps) >=1:
            recoWp4 =  tightLeps[0].p4() + metp4
            dPhiLepW = tightLeps[0].p4().DeltaPhi(recoWp4)
            Lp = tightLeps[0].pt / recoWp4.Pt() * cos(dPhiLepW)
            ST = tightLeps[0].pt + event.met_pt

        ret["DeltaPhiLepW"] = dPhiLepW
        ret['ST'] = ST
        ret['Lp'] = Lp

        centralJet30_DescFlag = []
        tightLeps_DescFlag = []

        for i,l in enumerate(tightLeps):
            if abs(l.mcMatchId)==6: tightLeps_DescFlag.append(1)    #top
            elif abs(l.mcMatchId)==24: tightLeps_DescFlag.append(2) #W-boson
            else: tightLeps_DescFlag.append(0)

        for i,j in enumerate(centralJet30):
            if abs(j.mcMatchId)==6:
                if len(genleps)>0 and abs(genleps[0].sourceId) ==6 and abs(j.mcFlavour)==5:
                    if j.mcMatchId==genleps[0].sourceId:
                        centralJet30_DescFlag.append(genleps[0].charge)
                    else:
                        centralJet30_DescFlag.append(2)
            elif abs(j.mcFlavour) not in [0,5,21]:
                centralJet30_DescFlag.append(3)
            else: centralJet30_DescFlag.append(-999) #; print "should not happen..."
        else: centralJet30_DescFlag.append(0)

        ret["centralJet30_DescFlag"]=centralJet30_DescFlag
        ret["tightLeps_DescFlag"]=tightLeps_DescFlag

        # for FatJets
        ret['nHighPtTopTag']=0
        ret['nHighPtTopTagPlusTau23']=0

        fatjets = [j for j in Collection(event,"FatJet","nFatJet")]
        for i,j in enumerate(fatjets):
            if j.nSubJets >2 and j.minMass>50 and j.topMass>140 and j.topMass<250:
                ret['nHighPtTopTag'] += 1
                if j.tau3 < 0.6 * j.tau2: # instead of division
                    ret['nHighPtTopTagPlusTau23'] += 1

        return ret

# Main function for test
if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1L_base()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev,{})
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

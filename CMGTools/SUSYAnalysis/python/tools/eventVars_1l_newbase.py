from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator

#################
### Cuts and WP
#################

## Eta requirement
centralEta = 2.4

## B tag Working points
## CSV (v1 -- Run1) WPs
#btag_LooseWP = 0.244
#btag_TightWP = 0.679

## CMVA (PHYS14)
#btag_LooseWP = 0.244
#btag_TightWP = 0.762

## CSV v2 (CSV-IVF) ( PHYS14)
#btag_LooseWP = 0.423
#btag_MediumWP = 0.814
#btag_TightWP = 0.941

## CSV v2 (CSV-IVF) (Spring15 50ns)
btag_LooseWP = 0.423 # <-- update
btag_MediumWP = 0.890
btag_TightWP = 0.941 # <-- update


## Non-triggering electron MVA id (Phys14 WP)
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

## Isolation
ele_miniIsoCut = 0.1
muo_miniIsoCut = 0.2
Lep_miniIsoCut = 0.4

goodEl_lostHits = 0
goodEl_sip3d = 4
goodMu_sip3d = 4

## Ele MVA check
def checkEleMVA(lepMVA,lepEta,WP = 'Tight'):
    # Eta dependent MVA ID check:
    passID = False

    lepEta = abs(lepEta)

    # eta cut
    if lepEta > 2.4:
        return False

    if WP == 'Tight':
        if lepEta < 0.8: passID = lepMVA > Ele_mvaPhys14_eta0p8_T
        elif lepEta < 1.44: passID = lepMVA > Ele_mvaPhys14_eta1p4_T
        elif lepEta >= 1.57: passID = lepMVA > Ele_mvaPhys14_eta2p4_T
    elif WP == 'Medium':
        if lepEta < 0.8: passID = lepMVA > Ele_mvaPhys14_eta0p8_M
        elif lepEta < 1.44: passID = lepMVA > Ele_mvaPhys14_eta1p4_M
        elif lepEta >= 1.57: passID = lepMVA > Ele_mvaPhys14_eta2p4_M
    elif WP == 'Loose':
        if lepEta < 0.8: passID = lepMVA > Ele_mvaPhys14_eta0p8_L
        elif lepEta < 1.44: passID = lepMVA > Ele_mvaPhys14_eta1p4_L
        elif lepEta >= 1.57: passID = lepMVA > Ele_mvaPhys14_eta2p4_L

    return passID

class EventVars1L_base:
    def __init__(self):
        self.branches = [
            ## general event info
            'Run','Event','Lumi','Xsec',
            ## leptons
            'nLep', 'nVeto',
            'nEl','nMu',
            #("tightLeps_DescFlag","I",10,"nTightLeps"),
            'Lep_pdgId','Lep_pt','Lep_eta','Lep_phi','Lep_Idx','Lep_relIso','Lep_miniIso',
            'Selected', # selected (tight) or anti-selected lepton
            ## MET
            'MET','LT','ST',
            "DeltaPhiLepW", 'dPhi','Lp',
            ## jets
            'HT','nJet','nBJet',
             "htJet30j", "htJet30ja",
            ## top tags
            "nHighPtTopTag", "nHighPtTopTagPlusTau23",
            ## special Vars
            "LSLjetptGT80", # leading + subl. jet pt > 80
            'isSR', # is it Signal or Control region
            'Mll' #di-lepton mass
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

        if hasattr(event,'xsec'):
            ret['Xsec'] = event.xsec

        '''
        # make python lists as Collection does not support indexing in slices
        genleps = [l for l in Collection(event,"genLep","ngenLep")]
        genparts = [l for l in Collection(event,"GenPart","nGenPart")]
        '''

        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        jets = [j for j in Collection(event,"Jet","nJet")]

        njet = len(jets); nlep = len(leps)

        ## make MET
        metp4 = ROOT.TLorentzVector(0,0,0,0)
        metp4.SetPtEtaPhiM(event.met_pt,event.met_eta,event.met_phi,event.met_mass)
        pmiss  =array.array('d',[event.met_pt * cos(event.met_phi), event.met_pt * sin(event.met_phi)] )

        #plain copy of MET pt (just as an example and cross-check for proper friend tree production)
        ret["MET"] = metp4.Pt()

        ### LEPTONS
        Selected = False

        # selected good leptons
        selectedTightLeps = []
        selectedTightLepsIdx = []
        selectedVetoLeps = []

        # anti-selected leptons
        antiTightLeps = []
        antiTightLepsIdx = []
        antiVetoLeps = []


        for idx,lep in enumerate(leps):

            # check acceptance
            lepEta = abs(lep.eta)

            if(lepEta > 2.5): continue

            # muons
            if(abs(lep.pdgId) == 13):

                # Pt cut
                if lep.pt < 10: continue

                # pass variables
                passID = False
                passIso = False


                # ID and Iso check:
                if lep.mediumMuonId == 1 and lep.sip3d < goodMu_sip3d:
                    passID = True
                if lep.miniRelIso < muo_miniIsoCut:
                    passIso = True

                # mimic selected muons (ID TBC)
                if passIso:

                    # fill
                    if passID and passIso:
                        selectedTightLeps.append(lep);
                        selectedTightLepsIdx.append(idx);

                        antiVetoLeps.append(lep)
                    else:
                        selectedVetoLeps.append(lep);

                elif lep.miniRelIso > muo_miniIsoCut:

                    # all anti are veto for selected
                    selectedVetoLeps.append(lep);

                    if passID:
                        antiTightLeps.append(lep);
                        antiTightLepsIdx.append(idx);
                    else:
                        antiVetoLeps.append(lep);
                else:
                    # veto and anti-selected
                    selectedVetoLeps.append(lep);
                    antiTightLeps.append(lep)

            # electrons
            if(abs(lep.pdgId) == 11):

                # Pt cut
                if lep.pt < 10: continue

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
                    antiVetoLeps.append(lep)

                    # Iso check:
                    if lep.miniRelIso < ele_miniIsoCut: passIso = True
                    # conversion check
                    if lep.lostHits <= goodEl_lostHits and lep.convVeto and lep.sip3d < goodEl_sip3d: passConv = True

                    # fill
                    if passIso and passConv:
                        selectedTightLeps.append(lep)
                        selectedTightLepsIdx.append(idx);
                    else:
                        selectedVetoLeps.append(lep)

                # anti-selected
                elif passLooseID:

                    # all anti leptons are veto for selected
                    selectedVetoLeps.append(lep)

                    # Iso check:
                    if lep.miniRelIso < 0.4: passIso = True
                    # conversion check
                    passConv = True

                    # fill
                    if passIso and passConv:
                        antiTightLeps.append(lep)
                        antiTightLepsIdx.append(idx);
                    else:
                        antiVetoLeps.append(lep)
                else:
                    # the rest is veto for selected and anti
                    selectedVetoLeps.append(lep)
                    antiVetoLeps.append(lep)
        # end lepton loop

        # choose common lepton collection: select selected or anti lepton
        if len(selectedTightLeps) > 0:
            tightLeps = selectedTightLeps
            tightLepsIdx = selectedTightLepsIdx

            vetoLeps = selectedVetoLeps

            ret['Selected'] = 1

        else: #if len(antiTightLeps) > 0: or empty collection
            tightLeps = antiTightLeps
            tightLepsIdx = antiTightLepsIdx

            vetoLeps = antiVetoLeps

            if len(antiTightLeps) > 0:
                ret['Selected'] = -1

        # store Tight and Veto lepton numbers
        ret['nLep'] = len(tightLeps)
        ret['nVeto'] = len(vetoLeps)

        # get number of tight el and mu
        tightEl = [lep for lep in tightLeps if abs(lep.pdgId) == 11]
        tightMu = [lep for lep in tightLeps if abs(lep.pdgId) == 13]

        ret['nEl'] = len(tightEl)
        ret['nMu'] = len(tightMu)

        # save leading lepton vars
        if len(tightLeps) > 0:
            ret['Lep_Idx'] = tightLepsIdx[0]

            ret['Lep_pt'] = tightLeps[0].pt
            ret['Lep_eta'] = tightLeps[0].eta
            ret['Lep_phi'] = tightLeps[0].phi
            ret['Lep_pdgId'] = tightLeps[0].pdgId

            ret['Lep_relIso'] = tightLeps[0].relIso03
            ret['Lep_miniIso'] = tightLeps[0].miniRelIso

        elif len(leps) > 0: # fill it with leading lepton
            ret['Lep_Idx'] = 0

            ret['Lep_pt'] = leps[0].pt
            ret['Lep_eta'] = leps[0].eta
            ret['Lep_phi'] = leps[0].phi
            ret['Lep_pdgId'] = leps[0].pdgId

            ret['Lep_relIso'] = leps[0].relIso03
            ret['Lep_miniIso'] = leps[0].miniRelIso

        ### JETS
        centralJet30 = []

        for i,j in enumerate(jets):
            if j.pt>30 and abs(j.eta)<centralEta:
                centralJet30.append(j)

        ret['nJet']   = len(centralJet30)

        ret['LSLjetptGT80'] = 1 if sum([j.pt>80 for j in centralJet30])>=2 else 0

        ret['htJet30j']  = sum([j.pt for j in centralJet30])
        ret['htJet30ja'] = sum([j.pt for j in jets if j.pt>30])

        ret['HT'] = ret['htJet30j']

        ## B tagging WPs for CSVv2 (CSV-IVF)
        ## from: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating

        # WP defined on top
        btagWP = btag_MediumWP

        BJetMedium30 = []

        for i,j in enumerate(centralJet30):
            if j.btagCSV > btagWP:
                BJetMedium30.append(j)

        ret['nBJet']   = len(BJetMedium30)

        # deltaPhi between the (single) lepton and the reconstructed W (lep + MET)
        dPhiLepW = -999 # set default value to -999 to spot "empty" entries
        # LT of lepton and MET
        LT = -999
        Lp = -99

        if len(tightLeps) >=1:
            recoWp4 =  tightLeps[0].p4() + metp4

            dPhiLepW = tightLeps[0].p4().DeltaPhi(recoWp4)
            LT = tightLeps[0].pt + event.met_pt
            Lp = tightLeps[0].pt / recoWp4.Pt() * cos(dPhiLepW)

        ret["DeltaPhiLepW"] = dPhiLepW
        ret['dPhi'] = abs(dPhiLepW) # nickname for absolute dPhiLepW
        ret['ST'] = LT
        ret['LT'] = LT
        ret['Lp'] = Lp

        ## Playground

        # di-lepton mass: opposite-sign, same flavour
        Mll = 0

        if len(tightLeps) > 1:

            lep1 = tightLeps[0]
            id1 = lep1.pdgId

            for lep2 in leps[1:]:
                if lep2.pdgId + lep1.pdgId == 0:
                    dilepP4 = lep1.p4() + lep2.p4()
                    Mll = dilepP4.M()

        ret['Mll'] = Mll

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

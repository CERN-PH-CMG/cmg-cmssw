from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2
import ROOT
import array

def getElectronIdx(leps, selected = True):

    # list of passed lepton indexes
    lepIdx = []

    for idx,lep in enumerate(leps):

        # check acceptance
        lepEta = abs(lep.eta)
        if (lepEta > 2.5): continue
        if (abs(lep.pdgId) != 11): continue
        if lep.pt < 25: continue

        # pass variables
        passID = False
        passIso = False
        passEta = False
        passConv = False

        mvaId = lep.mvaIdPhys14

        if selected:
            # Iso check:
            if lep.miniRelIso < 0.1: passIso = True
            # Eta dependent MVA ID check:
            if lepEta < 0.8 and mvaId > 0.73: passID = True
            elif lepEta >= 0.8 and lepEta < 1.44 and mvaId > 0.57: passID = True
            elif lepEta >= 1.57 and lepEta < 2.4 and mvaId > 0.05: passID = True
            # conversion check
            if lep.lostHits <= 0 and lep.convVeto and lep.sip3d < 4: passConv = True
        else: # Anti-selected electrons
            # Iso check:
            if lep.miniRelIso < 0.4: passIso = True
            # Eta dependent MVA ID check:
            if lepEta < 0.8 and 0.35 < mvaId and mvaId < 0.73: passID = True
            elif lepEta >= 0.8 and lepEta < 1.44 and 0.20 < mvaId and mvaId < 0.57: passID = True
            elif lepEta >= 1.57 and lepEta < 2.4 and -0.52 < mvaId and mvaId < 0.05: passID = True
            # conversion check
            passConv = True

        # fill
        if passID and passIso and passConv:
            lepIdx.append(idx)

    return lepIdx

class EventVars1L_QCD:
    def __init__(self):
        self.branches = [
            "ElGenMatchId","MuGenMatchId",
            'ST_sel','dPhiLepW_sel','Lp_sel',
            'ST_anti','dPhiLepW_anti','Lp_anti'
            ]

    def listBranches(self):
        return self.branches[:]

    def __call__(self,event,base):

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

        # MET
        metp4 = ROOT.TLorentzVector(0,0,0,0)
        metp4.SetPtEtaPhiM(event.met_pt,event.met_eta,event.met_phi,event.met_mass)
        pmiss = array.array('d',[event.met_pt * cos(event.met_phi), event.met_pt * sin(event.met_phi)] )

        ####################################
        # import output from previous step #
        #base = keyvals
        ####################################

        # get selected leptons
        tightLeps = []
        tightLepsIdx = base['tightLepsIdx']
        tightLeps = [leps[idx] for idx in tightLepsIdx]
        nTightLeps = len(tightLeps)

        selLeps = getElectronIdx(leps)
        antisLeps = getElectronIdx(leps,False)

		nSelLep = len(selLeps)
		nSelVetoLep = len(leps)-nSelLep
		nAntiLep = len(antisLeps)
		nAntiVetoLep = len(leps)-nAntiLep

        #if nTightLeps != len(selLeps):
        #    print 'N tight leps', len(tightLeps), len(selLeps), selLeps, len(antisLeps), antisLeps

        # dPhi , Lp
        dPhiLepW = -999
        Lp = -99
        ST = -99

        if len(selLeps) >=1:
            selLep = leps[selLeps[0]]

            recoWp4 =  selLep.p4() + metp4
            dPhiLepW = selLep.p4().DeltaPhi(recoWp4)
            Lp = selLep.pt / recoWp4.Pt() * cos(dPhiLepW)
            ST = selLep.pt + event.met_pt

        ret["dPhiLepW_sel"] = dPhiLepW
        ret['ST_sel'] = ST
        ret['Lp_sel'] = Lp

        dPhiLepW = -999
        Lp = -99
        ST = -99

        # anti selected
        if len(antisLeps) >=1:
            antisLep = leps[antisLeps[0]]

            recoWp4 =  antisLep.p4() + metp4
            dPhiLepW = antisLep.p4().DeltaPhi(recoWp4)
            Lp = antisLep.pt / recoWp4.Pt() * cos(dPhiLepW)
            ST = antisLep.pt + event.met_pt

        ret["dPhiLepW_anti"] = dPhiLepW
        ret['ST_anti'] = ST
        ret['Lp_anti'] = Lp


        # lepton MC matching
        # gen study
        genParts = [p for p in Collection(event,"genPartAll","ngenPartAll")]

        ret['ElGenMatchId'] = 0
        ret['MuGenMatchId'] = 0

        # match leptons to gen particles
        for indx,lep in enumerate(tightLeps[:1]):

            if lep.pt < 25: continue

            minDr = 99
            matchId = -1

            for ig, gpart in enumerate(genParts):
                if gpart.pt < 5: continue
                if gpart.status == -1: continue

                dR = lep.p4().DeltaR(gpart.p4())

                if dR < minDr:
                    minDr = dR
                    matchId = ig

            if matchId != -1:# and dR < 1.0:
                #print 'Matched', abs(lep.pdgId), 'to\t', genParts[matchId].pdgId, 'with status', genParts[matchId].status, 'with dR = ', dR

                if abs(lep.pdgId) == 11:
                    ret['ElGenMatchId'] = genParts[matchId].pdgId
                elif abs(lep.pdgId) == 13:
                    ret['MuGenMatchId'] = genParts[matchId].pdgId
            #else:
            #    print 'UnMatched', abs(lep.pdgId), 'to with dR = ', dR


        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1L_QCD()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

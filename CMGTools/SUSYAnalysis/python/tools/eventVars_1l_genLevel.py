from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2
import ROOT

def mt_2(p4one, p4two):
    return sqrt(2*p4one.Pt()*p4two.Pt()*(1-cos(p4one.Phi()-p4two.Phi())))


class EventVars1LGenLevel:
    def __init__(self):
        self.branches = ["GenDeltaPhiLepWSum", "GenDeltaPhiLepWDirect", "GenWSumMass", "GenWDirectMass",
                         "nidxGenWs", "GenmTLepNu", "LeptonDecayChannelFlag"
                         ]

    def listBranches(self):
        return self.branches[:]

    def __call__(self,event,keyvals):
        # The following variables still need to be double-checked for validity
        genLeps = [l for l in Collection(event,"genLep","ngenLep")]
        genTaus = [l for l in Collection(event,"genTau","ngenTau")]
        genParts = [l for l in Collection(event,"GenPart","nGenPart")]
        genLepsAndLepsFromTaus = [l for l in Collection(event,"genLep","ngenLep")] + [k for k in Collection(event,"genLepFromTau","ngenLepFromTau")]
#        print genLepsAndLepsFromTaus
        ngenLeps = len(genLeps); ngenTaus = len(genTaus); ngenParts = len (genParts); ngenLepsAndLepsFromTau = len(genLepsAndLepsFromTaus)

        GenDeltaPhiLepWSum=-999
        GenDeltaPhiLepWDirect=-999
        GenWSumMass=-999
        GenWDirectMass=-999
        GenmTLepNu=-999
        LeptonDecayChannelFlag=-999 
        idx_genWs=[]
        idx_genLeps=[]
        idx_genNus=[]
        # find gen-level neutrinos (status 23), calculate deltaPhi (lep, nu), and genW-masses m(lep+nu)
        # for this: start from genLeps (status 23)
        for i_lep, genLep in enumerate(genLeps):
            if genLep.status == 23 and abs(genLep.motherId) == 24: # genLep is outgoing and has W as mother
                W_idx = genLep.motherIndex
                idx_genWs.append(W_idx)
                idx_genLeps.append(i_lep)
                for i_nu, genPart in enumerate(genParts):
                    if genPart.motherIndex==W_idx and genPart.status == 23: # find W as mother
                        if abs(genPart.pdgId) == 12 or abs(genPart.pdgId) == 14 or abs(genPart.pdgId) == 16: #check whether it is a neutrino
                            idx_genNus.append(i_nu)


        if(len(idx_genLeps)>=1):
            genLepP4 = genLeps[idx_genLeps[0]].p4()
            genNuP4 = genParts[idx_genNus[0]].p4()
            genWSumP4 = genLepP4 + genNuP4
            genWDirectP4 = genParts[genLeps[idx_genLeps[0]].motherIndex].p4()
            GenDeltaPhiLepWSum = genLepP4.DeltaPhi(genWSumP4)
            GenDeltaPhiLepWDirect = genLepP4.DeltaPhi(genWDirectP4)
            GenWSumMass = genWSumP4.M()
            GenWDirectMass = genWDirectP4.M()
            GenmTLepNu = mt_2(genLepP4,genNuP4)

#        print ngenLepsAndLepsFromTau, ngenLeps + ngenTaus, event.ngenLepFromTau+ngenLeps
        assert ngenLepsAndLepsFromTau==event.ngenLepFromTau+ngenLeps
        if ngenLeps + ngenTaus ==2: #looking at semileptonic events
            IsDiLepEvent = True
            IsHadTauEvent = (ngenTaus > event.ngenLepFromTau)
            LeptonsInAcceptance = True
            PtMax = -999
            for l in genLepsAndLepsFromTaus: 
                if l.pt>PtMax: PtMax = l.pt 

            if IsHadTauEvent: LeptonDecayChannelFlag = 1 # preconfigure HadTau (becaus next loop won't start for two had taus in the event)
            for l in genLepsAndLepsFromTaus:
                if PtMax>=25 and l.pt<10: LeptonsInAcceptance=False
                if PtMax<25 and l.pt<5: LeptonsInAcceptance=False
                lepEta = abs(l.eta)
                if (lepEta>2.5): LeptonsInAcceptance=False
                if (abs(l.pdgId) == 11 and lepEta >= 1.44 and lepEta < 1.57): LeptonsInAcceptance=False
                
                if IsHadTauEvent and not LeptonsInAcceptance: LeptonDecayChannelFlag = 0 # OutOfAcceptance and HadTau
                elif IsHadTauEvent: LeptonDecayChannelFlag = 1 # HadTau (only)
                elif not LeptonsInAcceptance: LeptonDecayChannelFlag = 2 # OutOfAcceptance (only)
                else: LeptonDecayChannelFlag = 3 # Rest (Id/Isolation/Resolution)
            

        ret    =  { 'GenDeltaPhiLepWSum'   : GenDeltaPhiLepWSum } #initialize the dictionary with a first entry
        ret["GenDeltaPhiLepWDirect"] = GenDeltaPhiLepWDirect
        ret["GenWSumMass"]           = GenWSumMass
        ret["GenWDirectMass"]        = GenWDirectMass
        ret["GenmTLepNu"]            = GenmTLepNu

        ret["nidxGenWs"]= len(idx_genWs)
        ret["LeptonDecayChannelFlag"]= LeptonDecayChannelFlag
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1LGenLevel()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
#            tree.Show(0)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

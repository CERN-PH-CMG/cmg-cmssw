from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class SRAnalyzer( Analyzer ):
    '''SR Analyzer for the DM and X -> Zh -> nunubb analysis'''

    def beginLoop(self,setup):
        super(SRAnalyzer,self).beginLoop(setup)
        self.Hist = {}
        Z2NNlabels = ["Trigger", "#slash{E}_{T}", "Jet p_{T}", "e/#mu/#tau/#gamma veto", "#Delta #varphi > 2.5", "h mass", "b-tag 1", "b-tag 2"]
        setup.services["outputfile"].file.cd("Counters")
        self.Hist["Z2NNCounter"] = ROOT.TH1F("ZtoNNCounter", "", len(Z2NNlabels), 0, len(Z2NNlabels))
        for i, l in enumerate(Z2NNlabels):
            self.Hist["Z2NNCounter"].GetXaxis().SetBinLabel(i+1, l)
        setup.services["outputfile"].file.cd("..")
        # Set Sumw2
        for n, h in self.Hist.iteritems():
            h.Sumw2()
        
    def process(self, event):
    
        event.isZ2NN = True
        
        # Trigger
        if not event.HLT_BIT_HLT_PFMET170_NoiseCleaned_v: event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(1, event.eventWeight)
        
        # MET
        if event.met.pt() > self.cfg_ana.met_pt: event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(2, event.eventWeight)
        
        
        # Leptons, Taus and Gamma vetoes
        if not event.isSR:
            return True
        
        if not len(event.xcleanLeptons) + len(event.xcleanTaus) + len(event.xcleanPhotons): event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(4, event.eventWeight)
        
        
        # Jet and candidate
        if len(event.xcleanJetsAK8) == 0:
            return True
            
        # h candidate with pseudo-kin fit
        kH = event.xcleanJetsAK8[0].p4()
        k = 125.0/event.xcleanJetsAK8[0].mass() if event.xcleanJetsAK8[0].mass() > 0 else 0. #.userFloat(self.cfg_ana.jetAlgo)
        kH = ROOT.reco.Particle.LorentzVector(event.xcleanJetsAK8[0].px()*k, event.xcleanJetsAK8[0].py()*k, event.xcleanJetsAK8[0].pz()*k, event.xcleanJetsAK8[0].energy()*k)
        
        event.theX = event.met.p4() + event.xcleanJetsAK8[0].p4()
        event.theX.mT = math.sqrt( 2.*event.xcleanJetsAK8[0].energy()*event.met.pt()*(1.-math.cos( deltaPhi(event.xcleanJetsAK8[0].phi(), event.met.phi()) )) )
        cmet = event.met.p4()
        cmet.SetPz( -event.xcleanJetsAK8[0].pz() )
        event.theX.mC = (cmet + event.xcleanJetsAK8[0].p4()).mass()
        event.theX.mK = math.sqrt( 2.*kH.energy()*event.met.pt()*(1.-math.cos( deltaPhi(kH.phi(), event.met.phi()) )) )
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(3, event.eventWeight)
        
        
        # Jet-MET deltaPhi
        if event.xcleanJetsAK8[0].deltaPhi_met>2.5:  event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(5, event.eventWeight)
        
        # Jet mass
        if event.xcleanJetsAK8[0].userFloat(self.cfg_ana.jetAlgo) < 95 or event.xcleanJetsAK8[0].userFloat(self.cfg_ana.jetAlgo) > 130: event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(6, event.eventWeight)
        
        # b-tagging
        if len(event.xcleanJetsAK8[0].subjets('SoftDrop')) < 2 or not (event.xcleanJetsAK8[0].subjets('SoftDrop')[0].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605 or event.xcleanJetsAK8[0].subjets('SoftDrop')[1].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605): event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(7, event.eventWeight)

        if len(event.xcleanJetsAK8[0].subjets('SoftDrop')) < 2 or not (event.xcleanJetsAK8[0].subjets('SoftDrop')[0].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605 and event.xcleanJetsAK8[0].subjets('SoftDrop')[1].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605):  event.isZ2NN = False
        if event.isZ2NN: self.Hist["Z2NNCounter"].AddBinContent(8, event.eventWeight)
        

        return True


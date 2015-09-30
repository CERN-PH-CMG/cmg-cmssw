from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class GenAnalyzer( Analyzer ):
    '''Make plots at generation level'''
    
    def beginLoop(self,setup):
        super(GenAnalyzer,self).beginLoop(setup)
        self.Hist = {}
        if "outputfile" in setup.services:
            setup.services["outputfile"].file.cd()
            setup.services["outputfile"].file.mkdir("Gen")
            setup.services["outputfile"].file.cd("Gen")
            
            self.Hist["GenPhi1mass"] = ROOT.TH1F("GenPhi1mass", ";m_{X} [GeV]", 1000, 0., 5000.)
            self.Hist["GenPhi1pt"] = ROOT.TH1F("GenPhi1pt", ";X p_{T} [GeV]", 500, 0., 1000.)
            self.Hist["GenPhi1eta"] = ROOT.TH1F("GenPhi1eta", ";#Phi #eta", 50, -5, 5.)
            self.Hist["GenChi1mass"] = ROOT.TH1F("GenChi1mass", ";#chi_{1} mass [GeV]", 1000, 0., 1000.)
            self.Hist["GenChi1pt"] = ROOT.TH1F("GenChi1pt", ";#chi_{1} p_{T} [GeV]", 500, 0., 500.)
            self.Hist["GenChi1eta"] = ROOT.TH1F("GenChi1eta", ";#chi_{1} #eta", 50, -5, 5.)
            self.Hist["GenChi2mass"] = ROOT.TH1F("GenChi2mass", ";#chi_{2} mass [GeV]", 1000, 0., 100.)
            self.Hist["GenChi2pt"] = ROOT.TH1F("GenChi2pt", ";#chi_{2} p_{T} [GeV]", 500, 0., 500.)
            self.Hist["GenChi2eta"] = ROOT.TH1F("GenChi2eta", ";#chi_{2} #eta", 50, -5, 5.)
            self.Hist["GenChi12dR"] = ROOT.TH1F("GenChi12dR", ";#chi_{1, 2} #Delta R", 50, 0, 5.)
            
            self.Hist["GenZdecay"] = ROOT.TH1F("GenZdecay", ";Z daughter pdgId", 25, 0.5, 25.5)
            self.Hist["GenZmass"] = ROOT.TH1F("GenZmass", ";m_{Z} [GeV]", 150, 0., 150.)
            self.Hist["GenZpt"] = ROOT.TH1F("GenZpt", ";Z p_{T} [GeV]", 2500, 0., 2500.)
            self.Hist["GenZeta"] = ROOT.TH1F("GenZeta", ";Z #eta", 50, -5, 5.)
            self.Hist["GenZdR"] = ROOT.TH1F("GenZdR", ";Leptons #Delta R", 60, 0, 3.)
            self.Hist["GenWmass"] = ROOT.TH1F("GenWmass", ";m_{W} [GeV]", 150, 0., 150.)
            self.Hist["GenWpt"] = ROOT.TH1F("GenWpt", ";W p_{T} [GeV]", 2500, 0., 2500.)
            self.Hist["GenWeta"] = ROOT.TH1F("GenWeta", ";W #eta", 50, -5, 5.)
            self.Hist["GenHdecay"] = ROOT.TH1F("GenHdecay", ";H daughter pdgId", 25, 0.5, 25.5)
            self.Hist["GenHmass"] = ROOT.TH1F("GenHmass", ";m_{H} [GeV]", 100, 120., 130.)
            self.Hist["GenHpt"] = ROOT.TH1F("GenHpt", ";H p_{T} [GeV]", 250, 0., 2500.)
            self.Hist["GenHeta"] = ROOT.TH1F("GenHeta", ";H #eta", 50, -5, 5.)
            self.Hist["GenHdR"] = ROOT.TH1F("GenHdR", ";b-quarks #Delta R", 50, 0, 5.)
            self.Hist["GenHdPhi"] = ROOT.TH1F("GenHdPhi", ";b-quarks #Delta #varphi", 50, 0, 3.15)
            self.Hist["GenLepton1pt"] = ROOT.TH1F("GenLepton1pt", ";Lepton 1 p_{T} [GeV]", 250, 0., 2500.)
            self.Hist["GenLepton1eta"] = ROOT.TH1F("GenLepton1eta", ";Lepton 1 #eta", 50, -5, 5.)
            self.Hist["GenLepton2pt"] = ROOT.TH1F("GenLepton2pt", ";Lepton 2 p_{T} [GeV]", 250, 0., 2500.)
            self.Hist["GenLepton2eta"] = ROOT.TH1F("GenLepton2eta", ";Lepton 2 #eta", 50, -5, 5.)
            self.Hist["GenBquark1pt"] = ROOT.TH1F("GenBquark1pt", ";b-quark 1 p_{T} [GeV]", 250, 0., 2500.)
            self.Hist["GenBquark1eta"] = ROOT.TH1F("GenBquark1eta", ";b-quark 1 #eta", 50, -5, 5.)
            self.Hist["GenBquark2pt"] = ROOT.TH1F("GenBquark2pt", ";b-quark 2 p_{T} [GeV]", 250, 0., 2500.)
            self.Hist["GenBquark2eta"] = ROOT.TH1F("GenBquark2eta", ";b-quark 2 #eta", 50, -5, 5.)
            setup.services["outputfile"].file.cd("..")
            
            # LHE
            setup.services["outputfile"].file.mkdir("Lhe")
            setup.services["outputfile"].file.cd("Lhe")
            self.Hist["LheZpt"] = ROOT.TH1F("LheZpt", ";Z p_{T} [GeV]", 2500, 0., 2500.)
            self.Hist["LheHT"] = ROOT.TH1F("LheHT", ";HT [GeV]", 150, 0., 1500.)
            self.Hist["LheNj"] = ROOT.TH1F("LheNj", ";Number of partons", 6, -0.5, 5.5)
            self.Hist["LheNb"] = ROOT.TH1F("LheNb", ";Number of b-quarks", 6, -0.5, 5.5)
            self.Hist["LheNc"] = ROOT.TH1F("LheNc", ";Number of c-quarks", 6, -0.5, 5.5)
            self.Hist["LheNl"] = ROOT.TH1F("LheNl", ";Number of light quarks", 6, -0.5, 5.5)
            self.Hist["LheNg"] = ROOT.TH1F("LheNg", ";Number of gluons", 6, -0.5, 5.5)
            setup.services["outputfile"].file.cd("..")
            
            # Set Sumw2
            for n, h in self.Hist.iteritems():
                h.Sumw2()
    
    def process(self, event):
        
        if not hasattr(event, "genParticles"):
            return True
        
        weight = abs(event.LHE_originalWeight)/event.LHE_originalWeight
        
        event.genChi = []
        
        for g in event.genParticles:
            if g.pdgId() in self.cfg_ana.phi:
                event.genPhi = g
            if g.pdgId() in self.cfg_ana.chi:
                event.genChi.append(g)
        if hasattr(event, "genPhi"):
            self.Hist["GenPhi1mass"].Fill(event.genPhi.mass(), weight)
            self.Hist["GenPhi1pt"].Fill(event.genPhi.pt(), weight)
            self.Hist["GenPhi1eta"].Fill(event.genPhi.eta(), weight)
        if len(event.genChi) >= 2:
            i1, i2 = [0, 1] if event.genChi[0].pt() > event.genChi[1].pt() else [1, 0]
            self.Hist["GenChi1mass"].Fill(event.genChi[i1].mass(), weight)
            self.Hist["GenChi1pt"].Fill(event.genChi[i1].pt(), weight)
            self.Hist["GenChi1eta"].Fill(event.genChi[i1].eta(), weight)
            self.Hist["GenChi2mass"].Fill(event.genChi[i1].mass(), weight)
            self.Hist["GenChi2pt"].Fill(event.genChi[i1].pt(), weight)
            self.Hist["GenChi2eta"].Fill(event.genChi[i1].eta(), weight)
            self.Hist["GenChi12dR"].Fill(deltaR(event.genChi[0].eta(), event.genChi[0].phi(), event.genChi[1].eta(), event.genChi[1].phi()), weight)
        # Z
        if hasattr(event, "genVBosons"):
            if len(event.genVBosons) > 0:
                if event.genVBosons[0].pdgId() == 23:
                    self.Hist["GenZmass"].Fill(event.genVBosons[0].mass(), weight)
                    self.Hist["GenZpt"].Fill(event.genVBosons[0].pt(), weight)
                    self.Hist["GenZeta"].Fill(event.genVBosons[0].eta(), weight)
                elif event.genVBosons[0].pdgId() == 24:
                    self.Hist["GenWmass"].Fill(event.genVBosons[0].mass(), weight)
                    self.Hist["GenWpt"].Fill(event.genVBosons[0].pt(), weight)
                    self.Hist["GenWeta"].Fill(event.genVBosons[0].eta(), weight)
        # Higgs
        if hasattr(event, "genHiggsBosons"):
            if len(event.genHiggsBosons) > 0:
                self.Hist["GenHmass"].Fill(event.genHiggsBosons[0].mass(), weight)
                self.Hist["GenHpt"].Fill(event.genHiggsBosons[0].pt(), weight)
                self.Hist["GenHeta"].Fill(event.genHiggsBosons[0].eta(), weight)
        # Leptons from Z
        if hasattr(event, "genleps"):
            if len(event.genleps) >= 2:
                self.Hist["GenZdecay"].Fill(abs(event.genleps[0].pdgId()), weight)
                self.Hist["GenZdR"].Fill(deltaR(event.genleps[0].eta(), event.genleps[0].phi(), event.genleps[1].eta(), event.genleps[1].phi()), weight)
                i1, i2 = [0, 1] if event.genleps[0].pt() > event.genleps[1].pt() else [1, 0]
                self.Hist["GenLepton1pt"].Fill(event.genleps[i1].pt(), weight)
                self.Hist["GenLepton1eta"].Fill(event.genleps[i1].eta(), weight)
                self.Hist["GenLepton2pt"].Fill(event.genleps[i2].pt(), weight)
                self.Hist["GenLepton2eta"].Fill(event.genleps[i2].eta(), weight)
        # b-quarks from Higgs
        if hasattr(event, "genbquarks"):
            if len(event.genbquarks) == 1:
                self.Hist["GenBquark1pt"].Fill(event.genbquarks[0].pt(), weight)
                self.Hist["GenBquark1eta"].Fill(event.genbquarks[0].eta(), weight)
            elif len(event.genbquarks) >= 2:
                self.Hist["GenHdecay"].Fill(abs(event.genbquarks[0].pdgId()), weight)
                self.Hist["GenHdR"].Fill(deltaR(event.genbquarks[0].eta(), event.genbquarks[0].phi(), event.genbquarks[1].eta(), event.genbquarks[1].phi()), weight)
                self.Hist["GenHdPhi"].Fill(deltaPhi(event.genbquarks[0].phi(), event.genbquarks[1].phi()), weight)
                i1, i2 = [0, 1] if event.genbquarks[0].pt() > event.genbquarks[1].pt() else [1, 0]
                self.Hist["GenBquark1pt"].Fill(event.genbquarks[i1].pt(), weight)
                self.Hist["GenBquark1eta"].Fill(event.genbquarks[i1].eta(), weight)
                self.Hist["GenBquark2pt"].Fill(event.genbquarks[i2].pt(), weight)
                self.Hist["GenBquark2eta"].Fill(event.genbquarks[i2].eta(), weight)
        
        
        ### LHE event ###
        if not hasattr(event, "lheV_pt"):
            return True
            
        self.Hist["LheZpt"].Fill(event.lheV_pt, weight)
        self.Hist["LheHT"].Fill(event.lheHT, weight)
        self.Hist["LheNj"].Fill(event.lheNj, weight)
        self.Hist["LheNb"].Fill(event.lheNb, weight)
        self.Hist["LheNc"].Fill(event.lheNc, weight)
        self.Hist["LheNl"].Fill(event.lheNl, weight)
        self.Hist["LheNg"].Fill(event.lheNg, weight)
        
        
        return True
    

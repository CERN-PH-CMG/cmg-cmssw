from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class SyncAnalyzer( Analyzer ):
    '''Make plots to keep track of the event numbers'''
    
    def beginLoop(self,setup):
        super(SyncAnalyzer,self).beginLoop(setup)
        if "outputfile" in setup.services:
            setup.services["outputfile"].file.cd()
            setup.services["outputfile"].file.mkdir("Sync")
            setup.services["outputfile"].file.cd("Sync")
            
            SRLabels = ["All events", "Jet pT > 110", "dPhi(j1,j2)<2.5", "MET > 200", "Jets < 3", "Lepton veto", "Tau veto", "Photon veto"]
            ZCRLabels = ["All events", "Jet pT > 110", "dPhi(j1,j2)<2.5", "Jets < 3", "Electron veto", "Tau veto", "Photon veto", "Muons = 2", "60 < M(Z) < 120", "fakeMet > 200"]
            WCRLabels = ["All events", "Muons = 1", "Jet pT > 110", "dPhi(j1,j2)<2.5", "Jets < 3", "Electron veto", "Tau veto", "Photon veto", "Mt(W) > 50", "fakeMet > 200"]
            GCRLabels = ["All events", "Jet pT > 110", "dPhi(j1,j2)<2.5", "Jets < 3", "Electron veto", "Tau veto", "Muon veto", "Photons = 1", "fakeMet > 200"]
            
            self.SR = ROOT.TH1F("SR", "Signal Region", len(SRLabels), 0, len(SRLabels))
            self.ZCR = ROOT.TH1F("ZCR", "Z Control Region", len(ZCRLabels), 0, len(ZCRLabels))
            self.WCR = ROOT.TH1F("WCR", "W Control Region", len(WCRLabels), 0, len(WCRLabels))
            self.GCR = ROOT.TH1F("GCR", "Photon Control Region", len(GCRLabels), 0, len(GCRLabels))
            
            for i, l in enumerate(SRLabels): self.SR.GetXaxis().SetBinLabel(i+1, l)
            for i, l in enumerate(ZCRLabels): self.ZCR.GetXaxis().SetBinLabel(i+1, l)
            for i, l in enumerate(WCRLabels): self.WCR.GetXaxis().SetBinLabel(i+1, l)
            for i, l in enumerate(GCRLabels): self.GCR.GetXaxis().SetBinLabel(i+1, l)
    
    
    
    def syncSR(self, event):
        self.SR.Fill(0)
        if len(event.cleanJets) >= 1 and event.cleanJets[0].pt()>110. and event.cleanJets[0].chargedHadronEnergyFraction()>0.2 and event.cleanJets[0].neutralHadronEnergyFraction()<0.7 and event.cleanJets[0].neutralEmEnergyFraction()<0.7:
            self.SR.Fill(1)                               
            if len(event.cleanJets) < 2 or ( abs(deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi())) < 2.5 and event.cleanJets[1].neutralHadronEnergyFraction()<0.7 and event.cleanJets[1].neutralEmEnergyFraction()<0.9 ):
                self.SR.Fill(2)
                if event.met.pt()>200.:
                    self.SR.Fill(3)
                    if len(event.cleanJets) < 3:
                        self.SR.Fill(4)
                        if len(event.selectedLeptons) == 0:
                            self.SR.Fill(5)
                            if len(event.selectedTaus) == 0:
                                self.SR.Fill(6)
                                if len(event.selectedPhotons) == 0:
                                    self.SR.Fill(7)
                                    if self.cfg_ana.verbose: print "%d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )
    
    
    
    def syncZCR(self, event):
        self.ZCR.Fill(0)
        if len(event.cleanJets) >= 1 and event.cleanJets[0].pt()>110. and event.cleanJets[0].chargedHadronEnergyFraction()>0.2 and event.cleanJets[0].neutralHadronEnergyFraction()<0.7 and event.cleanJets[0].neutralEmEnergyFraction()<0.7:
            self.ZCR.Fill(1)                      
            if len(event.cleanJets) < 2 or ( abs(deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi())) < 2.5 and event.cleanJets[1].neutralHadronEnergyFraction()<0.7 and event.cleanJets[1].neutralEmEnergyFraction()<0.9 ):
                self.ZCR.Fill(2)
                if len(event.cleanJets) < 3:
                    self.ZCR.Fill(3)
                    if len(event.selectedElectrons) == 0:
                        self.ZCR.Fill(4)
                        if len(event.xcleanTaus) == 0:
                            self.ZCR.Fill(5)
                            if len(event.selectedPhotons) == 0:
                                self.ZCR.Fill(6)                                
                                if len(event.selectedMuons) == 2 and \
                                  event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight') and event.selectedMuons[0].relIso04 < 0.12 and \
                                  event.selectedMuons[1].pt() > 10 and event.selectedMuons[1].muonID('POG_ID_Loose') and event.selectedMuons[1].relIso04 < 0.20 and \
                                  event.selectedMuons[0].charge() != event.selectedMuons[1].charge():
                                    self.ZCR.Fill(7)
                                    if event.theZ.mass() > 60 and event.theZ.mass() < 120:
                                        self.ZCR.Fill(8)
                                        if event.fakemet.pt() > 200:
                                            self.ZCR.Fill(9)
                                            if self.cfg_ana.verbose: print "ZCR %d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )
    
    
    
    def syncWCR(self, event):
        self.WCR.Fill(0)
        if len(event.selectedMuons) == 1 and event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight') and event.selectedMuons[0].relIso04 < 0.12:
            self.WCR.Fill(1)
            if len(event.xcleanJets) > 0 and event.xcleanJets[0].pt()>110. and event.xcleanJets[0].chargedHadronEnergyFraction()>0.2 and event.xcleanJets[0].neutralHadronEnergyFraction()<0.7 and event.xcleanJets[0].neutralEmEnergyFraction()<0.7:
                self.WCR.Fill(2)
                if len(event.xcleanJets) == 1 or ( abs(deltaPhi(event.xcleanJets[0].phi(), event.xcleanJets[1].phi())) < 2.5 and event.xcleanJets[1].neutralHadronEnergyFraction()<0.7 and event.xcleanJets[1].neutralEmEnergyFraction()<0.9 ):
                    self.WCR.Fill(3)
                    if len(event.xcleanJets) < 3:
                        self.WCR.Fill(4)
                        if len(event.selectedElectrons) == 0:
                            self.WCR.Fill(5)
                            if len(event.xcleanTaus) == 0:
                                self.WCR.Fill(6)
                                if len(event.selectedPhotons) == 0:
                                    self.WCR.Fill(7)
                                    if event.theW.mT > 50:
                                        self.WCR.Fill(8)
                                        if event.fakemet.pt() > 200:
                                            self.WCR.Fill(9)
                                            if self.cfg_ana.verbose: print "WCR %d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )

    
    
    def syncGCR(self, event):
        self.GCR.Fill(0)
        if len(event.cleanJets) >= 1 and event.cleanJets[0].pt()>110. and event.cleanJets[0].chargedHadronEnergyFraction()>0.2 and event.cleanJets[0].neutralHadronEnergyFraction()<0.7 and event.cleanJets[0].neutralEmEnergyFraction()<0.7:
            self.GCR.Fill(1)
            if len(event.cleanJets) < 2 or ( abs(deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi())) < 2.5 and event.cleanJets[1].neutralHadronEnergyFraction()<0.7 and event.cleanJets[1].neutralEmEnergyFraction()<0.9 ):
                self.GCR.Fill(2)
                if len(event.cleanJets) < 3:
                    self.GCR.Fill(3)
                    if len(event.selectedElectrons) == 0:
                        self.GCR.Fill(4)
                        if len(event.xcleanTaus) == 0:
                            self.GCR.Fill(5)
                            if len(event.selectedMuons) == 0:
                                self.GCR.Fill(6)                                
                                if len(event.selectedPhotons) == 1 and event.selectedPhotons[0].pt() > 175:
                                    self.GCR.Fill(7)
                                    if event.fakemet.pt() > 200:
                                        self.GCR.Fill(8)
                                        if self.cfg_ana.verbose: print "GCR %d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )
    
    
    
    
    def process(self, event):
        self.syncSR(event)
        self.syncZCR(event)
        self.syncWCR(event)
        self.syncGCR(event)
        return True
    

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class SyncAnalyzerZCR( Analyzer ):
    '''Sync plot'''
    
    def beginLoop(self,setup):
        super(SyncAnalyzerZCR,self).beginLoop(setup)
        if "outputfile" in setup.services:
            setup.services["outputfile"].file.cd()
            self.Sync = ROOT.TH1F("SyncZCR", "SyncZCR", 10, 0, 10)
    
    def process(self, event):
        self.Sync.GetXaxis().SetBinLabel(1, "All events")
        self.Sync.Fill(0)
        if len(event.cleanJets) >= 1 and event.cleanJets[0].pt()>110. and event.cleanJets[0].chargedHadronEnergyFraction()>0.2 and event.cleanJets[0].neutralHadronEnergyFraction()<0.7 and event.cleanJets[0].neutralEmEnergyFraction()<0.7:
            self.Sync.GetXaxis().SetBinLabel(2, "Jet pT > 110")
            self.Sync.Fill(1)
            # print
            #print "* %5d * %5d * %5d * %10.3f * %10.3f * %10.3f * %10.3f * %10.3f * %10.3f * %10.3f * %5d * %5d * %5d * %5d *" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(),  event.cleanJets[0].pt(), event.cleanJets[0].eta(), event.cleanJets[0].phi(), event.cleanJets[1].pt() if len(event.cleanJets)>1 else 0., event.cleanJets[1].eta() if len(event.cleanJets)>1 else 0., event.cleanJets[1].phi() if len(event.cleanJets)>1 else 0., event.met.pt(), len(event.cleanJets), len(event.inclusiveLeptons), len(event.selectedTaus), len(event.selectedPhotons))
            
            #if event.input.eventAuxiliary().id().event()==458: print "@@@@@@@@@@@@@@@@@@", abs(deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi())), event.cleanJets[1].neutralHadronEnergyFraction(), event.cleanJets[1].neutralEmEnergyFraction()
            
            #if (event.input.eventAuxiliary().id().luminosityBlock()==2 and event.input.eventAuxiliary().id().event()==136) or \
               #(event.input.eventAuxiliary().id().luminosityBlock()==5 and event.input.eventAuxiliary().id().event()==475) or \
               #(event.input.eventAuxiliary().id().luminosityBlock()==17 and event.input.eventAuxiliary().id().event()==1681) or \
               #(event.input.eventAuxiliary().id().luminosityBlock()==64 and event.input.eventAuxiliary().id().event()==6397) or \
               #(event.input.eventAuxiliary().id().luminosityBlock()==40 and event.input.eventAuxiliary().id().event()==3916) or \
               #(event.input.eventAuxiliary().id().luminosityBlock()==56 and event.input.eventAuxiliary().id().event()==5579) or \
               #(event.input.eventAuxiliary().id().luminosityBlock()==69 and event.input.eventAuxiliary().id().event()==6847) :
                #print "* %5d * %5d * %5d * %10.3f * %10.3f * %10.3f * %10.3f * %10.3f * %10.3f * %10.3f * %5d * %5d * %5d * %5d *" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(),  event.cleanJets[0].pt(), event.cleanJets[0].eta(), event.cleanJets[0].phi(), event.cleanJets[1].pt() if len(event.cleanJets)>1 else 0., event.cleanJets[1].eta() if len(event.cleanJets)>1 else 0., event.cleanJets[1].phi() if len(event.cleanJets)>1 else 0., event.met.pt(), len(event.cleanJets), len(event.inclusiveLeptons), len(event.selectedTaus), len(event.selectedPhotons))
                
                #print event.inclusiveLeptons[0].pdgId()
                #print 'pt\t',event.inclusiveLeptons[0].pt()
                #print 'eta\t',event.inclusiveLeptons[0].eta()
                #print 'phi\t',event.inclusiveLeptons[0].phi()
                #print 'isLoose\t\t',event.inclusiveLeptons[0].isLooseMuon()
                #print 'isGlobal\t',event.inclusiveLeptons[0].isGlobalMuon()
                #print 'isTracker\t',event.inclusiveLeptons[0].isTrackerMuon()
                                                            
            if len(event.cleanJets) < 2 or ( abs(deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi())) < 2.5 and event.cleanJets[1].neutralHadronEnergyFraction()<0.7 and event.cleanJets[1].neutralEmEnergyFraction()<0.9 ):
                self.Sync.GetXaxis().SetBinLabel(3, "dPhi(j1,j2)<2.5")
                self.Sync.Fill(2)
                if len(event.cleanJets)<3:
                    self.Sync.GetXaxis().SetBinLabel(4, "Jets < 3")
                    self.Sync.Fill(3)
                    if len(event.selectedElectrons) == 0:
                        self.Sync.GetXaxis().SetBinLabel(5, "Electron veto")
                        self.Sync.Fill(4)
                        if len(event.selectedTaus) == 0:
                            self.Sync.GetXaxis().SetBinLabel(6, "Tau veto")
                            self.Sync.Fill(5)
                            if len(event.selectedPhotons) == 0:
                                self.Sync.GetXaxis().SetBinLabel(7, "Photon veto")
                                self.Sync.Fill(6)                                
                                if len(event.selectedMuons) == 2 and \
                                    event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight') and event.selectedMuons[0].relIso04 < 0.12 and \
                                    event.selectedMuons[1].pt() > 10 and event.selectedMuons[1].muonID('POG_ID_Loose') and event.selectedMuons[1].relIso04 < 0.20 and \
                                    event.selectedMuons[0].charge() != event.selectedMuons[1].charge():
                                    theMu0 = event.selectedMuons[0]
                                    theMu1 = event.selectedMuons[1]
                                    self.Sync.GetXaxis().SetBinLabel(8, "Muons = 2")
                                    self.Sync.Fill(7)
                                    theZ = theMu0.p4() + theMu1.p4()
                                    if theZ.mass() > 60 and theZ.mass() < 120:
                                        self.Sync.GetXaxis().SetBinLabel(9, "60 < M(Z) < 120")
                                        self.Sync.Fill(8)
                                        fakemet = copy.deepcopy(event.met)
                                        px, py = event.met.px() + theZ.px(), event.met.py() + theZ.py()
                                        fakemet.setP4(ROOT.reco.Particle.LorentzVector(px, py, 0, math.hypot(px,py)))
                                        if fakemet.pt() > 200:
                                            self.Sync.GetXaxis().SetBinLabel(10, "fakeMet > 200")
                                            self.Sync.Fill(9)
                                            print "ZCR %d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )
        
        return True
    

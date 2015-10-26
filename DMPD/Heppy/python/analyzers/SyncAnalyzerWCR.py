from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class SyncAnalyzerWCR( Analyzer ):
    '''Sync plot'''

    def beginLoop(self,setup):
        super(SyncAnalyzerWCR,self).beginLoop(setup)
        if "outputfile" in setup.services:
            setup.services["outputfile"].file.cd()
            self.Sync = ROOT.TH1F("SyncWCR", "SyncWCR", 10, 0, 10)

    def process(self, event):
        self.Sync.GetXaxis().SetBinLabel(1, "All events")
        self.Sync.Fill(0)

        #if event.input.eventAuxiliary().id().run() != 1 or \
         #  event.input.eventAuxiliary().id().luminosityBlock() != 152 or \
         #  event.input.eventAuxiliary().id().event() != 302616 : 
	 #     return True

        ##################################################################

        SelPhotons = []
        SelMuons = []
        SelTaus  = []
        SelJets  = []

        for ph in event.selectedPhotons:
            if ph.chargedHadronIso()/ph.pt() < 0.3:
                SelPhotons.append(ph)

        for mu in event.selectedMuons:
            if mu.pt() > 20 and mu.muonID('POG_ID_Tight') and mu.relIso04 < 0.12:
                SelMuons.append(mu)

            #check for overlapping taus
            for tau in event.selectedTaus:
                dRt = deltaR(mu.eta(),mu.phi(),tau.eta(),tau.phi())
                if (dRt > 0.4):
                    SelTaus.append(tau)

            #check for overlapping jets
            for jet in event.cleanJets:
                dRj = deltaR(mu.eta(),mu.phi(),jet.eta(),jet.phi())
                if (dRj > 0.4):
                    SelJets.append(jet)

        if len(event.selectedMuons) == 1 and event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight') and event.selectedMuons[0].relIso04 < 0.12:
            theMu = event.selectedMuons[0]
        #if len(event.selectedMuons) == 1 and event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight'):
        #newMuons = [x for x in event.selectedMuons if (x.pt() > 20 and x.muonID('POG_ID_Tight') and x.relIso04 < 0.12)]
        #if len(newMuons) == 1 :
            #theMu = newMuons[0]
            self.Sync.GetXaxis().SetBinLabel(2, "Muons = 1")
            self.Sync.Fill(1)
            if len(SelJets) > 0 and SelJets[0].pt()>110. and SelJets[0].chargedHadronEnergyFraction()>0.2 and SelJets[0].neutralHadronEnergyFraction()<0.7 and SelJets[0].neutralEmEnergyFraction()<0.7:
                self.Sync.GetXaxis().SetBinLabel(3, "Jet pT > 110")
                self.Sync.Fill(2)
                if len(SelJets) == 1 or ( abs(deltaPhi(SelJets[0].phi(), SelJets[1].phi())) < 2.5 and SelJets[1].neutralHadronEnergyFraction()<0.7 and SelJets[1].neutralEmEnergyFraction()<0.9 ):
                    self.Sync.GetXaxis().SetBinLabel(4, "dPhi(j1,j2)<2.5")
                    self.Sync.Fill(3)
                    if len(SelJets)<3:
                        self.Sync.GetXaxis().SetBinLabel(5, "Jets < 3")
                        self.Sync.Fill(4)
                        if len(event.selectedElectrons) == 0:
                            self.Sync.GetXaxis().SetBinLabel(6, "Electron veto")
                            self.Sync.Fill(5)
                            if len(SelTaus) == 0:
                                self.Sync.GetXaxis().SetBinLabel(7, "Tau veto")
                                self.Sync.Fill(6)
                                if len(SelPhotons) != 0: print 'XXXXX'
                                if len(SelPhotons) == 0:
                                    self.Sync.GetXaxis().SetBinLabel(8, "Photon veto")
                                    self.Sync.Fill(7)
                                    theW = theMu.p4() + event.met.p4()
                                    theW.deltaPhi_met = abs(deltaPhi(theMu.phi(), event.met.phi()))
                                    theW.mT = math.sqrt( 2.*theMu.et()*event.met.pt()*(1.-math.cos(theW.deltaPhi_met)) )
                                    if theW.mT > 50:
                                        self.Sync.GetXaxis().SetBinLabel(9, "Mt(W) > 50")
                                        self.Sync.Fill(8)
                                        fakemet = copy.deepcopy(event.met)
                                        px, py = event.met.px() + theMu.px(), event.met.py() + theMu.py()
                                        fakemet.setP4(ROOT.reco.Particle.LorentzVector(px, py, 0, math.hypot(px,py)))
                                        if fakemet.pt() > 200:
                                            self.Sync.GetXaxis().SetBinLabel(10, "fakeMet > 200")
                                            self.Sync.Fill(9)
                                            print "WCR %d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )

        ##################################################################

        #print '------------------------------------------------'
        #print 'Run\t:\t%d | LumiSec\t:\t%d | EventNum\t:\t%d: ' % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )
        #print 'PFMet\t:\t%f | Phi\t:\t%f ' % ( event.met.pt(), event.met.phi() )

        #print 'nIncLep\t:\t%d' % len(event.inclusiveLeptons)
        #for li in event.inclusiveLeptons:
            #print 'IncLepPt\t:\t%f | IncLepEta\t:\t%f | IncLepPhi\t:\t%f | IncLepIso\t:\t%f ' % ( li.pt(), li.eta(), li.phi(), li.relIso04 )
        #print 'nIncTau\t:\t%d' % len(event.inclusiveTaus)
        #for ti in event.inclusiveTaus:
            #print 'IncTauPt\t:\t%f | IncTauEta\t:\t%f | IncTauPhi\t:\t%f ' % ( ti.pt(), ti.eta(), ti.phi() )

        #print 'nSelMu\t:\t%d' % len(event.selectedMuons)
        #for m in event.selectedMuons:
            #print 'SelMuPt\t:\t%f | SelMuEta\t:\t%f | SelMuPhi\t:\t%f | SelMuIso\t:\t%f | SelMuTight\t:\t%d ' % ( m.pt(), m.eta(), m.phi(), m.relIso04, m.muonID('POG_ID_Tight') )
        #print 'nSelEle\t:\t%d' % len(event.selectedElectrons)
        #for e in event.selectedElectrons:
            #print 'SelElePt\t:\t%f | SelEleEta\t:\t%f | SelElePhi\t:\t%f ' % ( e.pt(), e.eta(), e.phi() )
        #print 'nSelTau\t:\t%d' % len(event.selectedTaus)
        #for t in event.selectedTaus:
            #print 'SelTauPt\t:\t%f | SelTauEta\t:\t%f | SelTauPhi\t:\t%f ' % ( t.pt(), t.eta(), t.phi() )
        #print 'nSelPho\t:\t%d' % len(event.selectedPhotons)
        #for p in event.selectedPhotons:
            #print 'SelPhoPt\t:\t%f | SelPhoEta\t:\t%f | SelPhoPhi\t:\t%f ' % ( p.pt(), p.eta(), p.phi() )
        #print 'nJets\t:\t%d' % len(event.cleanJets)
        #for j in event.cleanJets:
            #print 'JetPt\t:\t%f | JetEta\t:\t%f | JetPhi\t:\t%f ' % ( j.pt(), j.eta(), j.phi() )
            #print 'JetChf\t:\t%f | JetNhf\t:\t%f | JetPhf\t:\t%f ' % ( j.chargedHadronEnergyFraction(), j.neutralHadronEnergyFraction(), j.neutralEmEnergyFraction() )
            #print 'JetPt\t:\t%f | JetEta\t:\t%f | JetPhi\t:\t%f ' % ( j.pt(), j.eta(), j.phi() )
            #for mj in event.selectedMuons:
                #print ' m-DR\t', deltaR(mj.eta(),mj.phi(),j.eta(),j.phi())
            #for ej in event.selectedElectrons:
                #print ' e-DR\t', deltaR(ej.eta(),ej.phi(),j.eta(),j.phi())
            #for tj in event.selectedTaus:
                #print ' t-DR\t', deltaR(tj.eta(),tj.phi(),j.eta(),j.phi())
            #for pj in event.selectedPhotons:
                #print ' p-DR\t', deltaR(pj.eta(),pj.phi(),j.eta(),j.phi())
            #for jj in event.cleanJets:
                #if jj is j: continue
                #print ' j-DR\t', deltaR(jj.eta(),jj.phi(),j.eta(),j.phi())

        #print 'W\t:'
        #if len(event.selectedMuons) == 1:
            #thisW = event.selectedMuons[0].p4() + event.met.p4()
            #thisW.deltaPhi_met = abs(deltaPhi(event.selectedMuons[0].phi(), event.met.phi()))
            #thisW.mT = math.sqrt( 2.*event.selectedMuons[0].et()*event.met.pt()*(1.-math.cos(thisW.deltaPhi_met)) )
            #print 'Wmt\t:\t%f: ' % ( thisW.mT )

        #print 'fakeMet\t:'
        #if len(event.selectedMuons) == 1:
            #fm = copy.deepcopy(event.met)
            #fpx, fpy = event.met.px() + event.selectedMuons[0].px(), event.met.py() + event.selectedMuons[0].py()
            #fm.setP4(ROOT.reco.Particle.LorentzVector(fpx, fpy, 0, math.hypot(fpx,fpy)))
            #print 'fakeMet\t:\t%f: ' % ( fm.pt() )


        #if len(event.selectedMuons) == 1 and event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight') and event.selectedMuons[0].relIso04 < 0.12:
            #theMu = event.selectedMuons[0]
        ##if len(event.selectedMuons) == 1 and event.selectedMuons[0].pt() > 20 and event.selectedMuons[0].muonID('POG_ID_Tight'):
        ##newMuons = [x for x in event.selectedMuons if (x.pt() > 20 and x.muonID('POG_ID_Tight') and x.relIso04 < 0.12)]
        ##if len(newMuons) == 1 :
            ##theMu = newMuons[0]
            #self.Sync.GetXaxis().SetBinLabel(2, "Muons = 1")
            #self.Sync.Fill(1)
            #if len(event.cleanJets) > 0 and event.cleanJets[0].pt()>110. and event.cleanJets[0].chargedHadronEnergyFraction()>0.2 and event.cleanJets[0].neutralHadronEnergyFraction()<0.7 and event.cleanJets[0].neutralEmEnergyFraction()<0.7:
                #self.Sync.GetXaxis().SetBinLabel(3, "Jet pT > 110")
                #self.Sync.Fill(2)
                #if len(event.cleanJets) == 1 or ( abs(deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi())) < 2.5 and event.cleanJets[1].neutralHadronEnergyFraction()<0.7 and event.cleanJets[1].neutralEmEnergyFraction()<0.9 ):
                    #self.Sync.GetXaxis().SetBinLabel(4, "dPhi(j1,j2)<2.5")
                    #self.Sync.Fill(3)
                    #if len(event.cleanJets)<3:
                        #self.Sync.GetXaxis().SetBinLabel(5, "Jets < 3")
                        #self.Sync.Fill(4)
                        #if len(event.selectedElectrons) == 0:
                            #self.Sync.GetXaxis().SetBinLabel(6, "Electron veto")
                            #self.Sync.Fill(5)
                            #if len(event.selectedTaus) == 0:
                                #self.Sync.GetXaxis().SetBinLabel(7, "Tau veto")
                                #self.Sync.Fill(6)
                                #if len(event.selectedPhotons) == 0:
                                    #self.Sync.GetXaxis().SetBinLabel(8, "Photon veto")
                                    #self.Sync.Fill(7)
                                    #theW = theMu.p4() + event.met.p4()
                                    #theW.deltaPhi_met = abs(deltaPhi(theMu.phi(), event.met.phi()))
                                    #theW.mT = math.sqrt( 2.*theMu.et()*event.met.pt()*(1.-math.cos(theW.deltaPhi_met)) )
                                    #if theW.mT > 50:
                                        #self.Sync.GetXaxis().SetBinLabel(9, "Mt(W) > 50")
                                        #self.Sync.Fill(8)
                                        #fakemet = copy.deepcopy(event.met)
                                        #px, py = event.met.px() + theMu.px(), event.met.py() + theMu.py()
                                        #fakemet.setP4(ROOT.reco.Particle.LorentzVector(px, py, 0, math.hypot(px,py)))
                                        #if fakemet.pt() > 200:
                                            #self.Sync.GetXaxis().SetBinLabel(10, "fakeMet > 200")
                                            #self.Sync.Fill(9)
                                            #print "WCR %d:%d:%d" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )

        return True


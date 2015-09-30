from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class SyncAnalyzerSR( Analyzer ):
    '''Sync plot'''

    def beginLoop(self,setup):
        super(SyncAnalyzerSR,self).beginLoop(setup)
        if "outputfile" in setup.services:
            setup.services["outputfile"].file.cd()
            self.Sync = ROOT.TH1F("SyncSR", "SyncSR", 8, 0, 8)

    def process(self, event):

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

        print '-----------------------------------------------'
        print 'Run\t:\t%d | LumiSec\t:\t%d | EventNum\t:\t%d: ' % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event() )
        print 'PFMet\t:\t%f | Phi\t:\t%f ' % ( event.met.pt(), event.met.phi() )

        print 'nIncLep\t:\t%d' % len(event.inclusiveLeptons)
        for li in event.inclusiveLeptons:
            print 'IncLepPt\t:\t%f | IncLepEta\t:\t%f | IncLepPhi\t:\t%f | IncLepIso\t:\t%f ' % ( li.pt(), li.eta(), li.phi(), li.relIso04 )
        print 'nIncTau\t:\t%d' % len(event.inclusiveTaus)
        for ti in event.inclusiveTaus:
            print 'IncTauPt\t:\t%f | IncTauEta\t:\t%f | IncTauPhi\t:\t%f ' % ( ti.pt(), ti.eta(), ti.phi() )
        print 'nAllPho\t:\t%d' % len(event.allphotons)
        for ap in event.allphotons:
            print 'AllPhoPt\t:\t%f | AllPhoEta\t:\t%f | AllPhoPhi\t:\t%f ' % ( ap.pt(), ap.eta(), ap.phi() )
        print 'nClJet\t:\t%d' % len(event.cleanJets)
        for ji in event.cleanJets:
            print 'ClJetPt\t:\t%f | ClJetEta\t:\t%f | ClJetPhi\t:\t%f ' % ( ji.pt(), ji.eta(), ji.phi() )

        print 'nSelMu\t:\t%d' % len(event.selectedMuons)
        for m in event.selectedMuons:
            print 'SelMuPt\t:\t%f | SelMuEta\t:\t%f | SelMuPhi\t:\t%f | SelMuIso\t:\t%f | SelMuTight\t:\t%d ' % ( m.pt(), m.eta(), m.phi(), m.relIso04, m.muonID('POG_ID_Tight') )
        print 'nSelEle\t:\t%d' % len(event.selectedElectrons)
        for e in event.selectedElectrons:
            print 'SelElePt\t:\t%f | SelEleEta\t:\t%f | SelElePhi\t:\t%f ' % ( e.pt(), e.eta(), e.phi() )
        print 'nSelTau\t:\t%d' % len(SelTaus)
        for t in SelTaus:
            print 'SelTauPt\t:\t%f | SelTauEta\t:\t%f | SelTauPhi\t:\t%f ' % ( t.pt(), t.eta(), t.phi() )
        print 'nSelPho\t:\t%d' % len(event.selectedPhotons)
        for p in event.selectedPhotons:
            print 'SelPhoPt\t:\t%f | SelPhoEta\t:\t%f | SelPhoPhi\t:\t%f ' % ( p.pt(), p.eta(), p.phi() )
        print 'nJets\t:\t%d' % len(SelJets)
        for j in SelJets:
            print 'JetPt\t:\t%f | JetEta\t:\t%f | JetPhi\t:\t%f ' % ( j.pt(), j.eta(), j.phi() )
            print 'JetChf\t:\t%f | JetNhf\t:\t%f | JetPhf\t:\t%f ' % ( j.chargedHadronEnergyFraction(), j.neutralHadronEnergyFraction(), j.neutralEmEnergyFraction() )
            print 'JetPt\t:\t%f | JetEta\t:\t%f | JetPhi\t:\t%f ' % ( j.pt(), j.eta(), j.phi() )
            for mj in event.selectedMuons:
                print ' m-DR\t', deltaR(mj.eta(),mj.phi(),j.eta(),j.phi())
            for ej in event.selectedElectrons:
                print ' e-DR\t', deltaR(ej.eta(),ej.phi(),j.eta(),j.phi())
            for tj in SelTaus:
                print ' t-DR\t', deltaR(tj.eta(),tj.phi(),j.eta(),j.phi())
            for pj in event.selectedPhotons:
                print ' p-DR\t', deltaR(pj.eta(),pj.phi(),j.eta(),j.phi())
            for jj in SelJets:
                if jj is j: continue
                print ' j-DR\t', deltaR(jj.eta(),jj.phi(),j.eta(),j.phi())

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
                print "DPHI %d:%d:%d %f" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(), event.met.pt() )
                if event.met.pt()>200.:
                    self.Sync.GetXaxis().SetBinLabel(4, "MET > 200")
                    self.Sync.Fill(3)
                    print "METCUT %d:%d:%d %f" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(), event.met.pt() )
                    if len(event.cleanJets)<3:
                        self.Sync.GetXaxis().SetBinLabel(5, "Jets < 3")
                        self.Sync.Fill(4)
	                print "NJETS %d:%d:%d %f" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(), event.met.pt() )
                        if len(event.inclusiveLeptons) == 0:
                            self.Sync.GetXaxis().SetBinLabel(6, "Lepton veto")
                            self.Sync.Fill(5)
                            print "VLEP %d:%d:%d %f" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(), event.met.pt() )
                            if len(event.selectedTaus) == 0:
                                self.Sync.GetXaxis().SetBinLabel(7, "Tau veto")
                                self.Sync.Fill(6)
                                print "VTAU %d:%d:%d %f" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(), event.met.pt() )
                                if len(event.selectedPhotons) == 0:
                                    self.Sync.GetXaxis().SetBinLabel(8, "Photon veto")
                                    self.Sync.Fill(7)
                                    print "SR %d:%d:%d %f" % ( event.input.eventAuxiliary().id().run(), event.input.eventAuxiliary().id().luminosityBlock(), event.input.eventAuxiliary().id().event(), event.met.pt() )


        return True


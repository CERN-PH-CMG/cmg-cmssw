import operator 
import itertools
import copy
from math import *

from ROOT import TLorentzVector, TVectorD

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Jet import Jet

from CMGTools.RootTools.utils.DeltaR import * 
from CMGTools.TTHAnalysis.leptonMVA import LeptonMVA
import os
        
class ttHLepEventAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepEventAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.maxLeps = cfg_ana.maxLeps

        self.leptonMVA = LeptonMVA("%s/src/CMGTools/TTHAnalysis/data/leptonMVA/%%s_BDTG.weights.xml" % os.environ['CMSSW_BASE'], self.cfg_comp.isMC)
    def declareHandles(self):
        super(ttHLepEventAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'cmgPFMET', 'std::vector<cmg::BaseMET>' )
        self.handles['nopumet'] = AutoHandle( 'nopuMet', 'std::vector<reco::PFMET>' )
        self.handles['metSignificance'] = AutoHandle( 'pfMetSignificance', 'cmg::METSignificance' )

    def beginLoop(self):
        super(ttHLepEventAnalyzer,self).beginLoop()

    def makeZs(self, event, maxLeps):
        event.bestZ1 = [ 0., -1,-1 ]
        event.bestZ1sfss = [ 0., -1,-1 ]
        event.bestZ2 = [ 0., -1,-1, 0. ]
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if l1.pdgId() == -l2.pdgId():
                    zmass = (l1.p4() + l2.p4()).M()
                    if event.bestZ1[0] == 0 or abs(zmass - 91.188) < abs(event.bestZ1[0] - 91.188):
                        event.bestZ1 = [ zmass, i, j ]
                if l1.pdgId() == l2.pdgId():
                    zmass = (l1.p4() + l2.p4()).M()
                    if event.bestZ1sfss[0] == 0 or abs(zmass - 91.188) < abs(event.bestZ1sfss[0] - 91.188):
                        event.bestZ1sfss = [ zmass, i, j ]
        if event.bestZ1[0] != 0 and nlep > 3:
            for i,l1 in enumerate(event.selectedLeptons):
                if i == event.bestZ1[1]: continue
                for j in range(i+1,nlep):
                    if j >= maxLeps: break
                    if j == event.bestZ1[2]: continue
                    l2 = event.selectedLeptons[j]    
                    if l1.pdgId() == -l2.pdgId():
                        if l1.pt() + l2.pt() > event.bestZ2[0]:
                            event.bestZ2 = [ l1.pt() + l2.pt(), i, j, (l1.p4() + l2.p4()).M() ]

    def makeMlls(self, event, maxLeps):
        mllsfos = self.mllValues(event,  lambda l1,l2 : l1.pdgId()  == -l2.pdgId(),  maxLeps)
        mllafos = self.mllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(),  maxLeps)
        mllafss = self.mllValues(event,  lambda l1,l2 : l1.charge() ==  l2.charge(),  maxLeps)
        mllafas = self.mllValues(event,  lambda l1,l2 : True, maxLeps)
        event.minMllSFOS = min(mllsfos)
        event.minMllAFOS = min(mllafos)
        event.minMllAFSS = min(mllafss)
        event.minMllAFAS = min(mllafas)
        event.maxMllAFOS = max(mllafos)
        event.maxMllAFSS = max(mllafss)
        drllafos = self.drllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(),  maxLeps)
        drllafss = self.drllValues(event,  lambda l1,l2 : l1.charge() ==  l2.charge(),  maxLeps)
        event.minDrllAFSS = min(drllafss)
        event.minDrllAFOS = min(drllafos)
        event.maxDrllAFOS = max(drllafos)
        event.maxDrllAFSS = max(drllafss)
        ptllafos = self.ptllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(),  maxLeps)
        ptllafss = self.ptllValues(event,  lambda l1,l2 : l1.charge() ==  l2.charge(),  maxLeps)
        event.minPtllAFSS = min(ptllafss)
        event.minPtllAFOS = min(ptllafos)
        event.maxPtllAFOS = max(ptllafos)
        event.maxPtllAFSS = max(ptllafss)
        leps = event.selectedLeptons; nlep = len(leps)
        event.m2l = (leps[0].p4() + leps[1].p4()).M() if nlep >= 2 else 0
        event.pt2l = (leps[0].p4() + leps[1].p4()).Pt() if nlep >= 2 else 0
        event.q3l = sum([l.charge() for l in leps[:2]]) if nlep >= 3 else 0
        event.ht3l = sum([l.pt() for l in leps[:2]]) if nlep >= 3 else 0
        event.pt3l = (leps[0].p4() + leps[1].p4() + leps[2].p4()).Pt() if nlep >= 3 else 0
        event.m3l = (leps[0].p4() + leps[1].p4() + leps[2].p4()).M() if nlep >= 3 else 0
        event.q4l = sum([l.charge() for l in leps[:3]])  if nlep >= 4 else 0
        event.ht4l = sum([l.pt() for l in leps[:3]]) if nlep >= 4 else 0
        event.pt4l = (leps[0].p4() + leps[1].p4() + leps[2].p4() + leps[3].p4()).Pt() if nlep >= 4 else 0
        event.m4l = (leps[0].p4() + leps[1].p4() + leps[2].p4() + leps[3].p4()).M() if nlep >= 4 else 0

    def mllValues(self, event, pairSelection, maxLeps):
        return self.llValues(event, lambda l1,l2: (l1.p4() + l2.p4()).M(), pairSelection, maxLeps)

    def drllValues(self, event, pairSelection, maxLeps):
        return self.llValues(event, lambda l1,l2: deltaR(l1.eta(), l1.phi(), l2.eta(), l2.phi()), pairSelection, maxLeps)

    def ptllValues(self, event, pairSelection, maxLeps):
        return self.llValues(event, lambda l1,l2: (l1.p4() + l2.p4()).Pt(), pairSelection, maxLeps)

    def llValues(self, event, function, pairSelection, maxLeps):
        pairs = []
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if pairSelection(l1,l2):
                    pairs.append( function(l1, l2) )
        if pairs == []: pairs.append(-1)
        return pairs
    
    def makeLepBJetDeltaR(self, event):
        for l in event.selectedLeptons + event.looseLeptons:
            match, dr = bestMatch(l, event.bjetsLoose)
            l.drBJet = dr

        for l in event.selectedLeptons:
            (px,py,pz) = (l.px(),l.py(),l.pz())
            (jx,jy,jz) = (l.jet.px(),l.jet.py(),l.jet.pz())
            cross = (px*jy-py*jx, py*jz-pz*jy, pz*jx-px*jz)
            l.ptRelJet = sqrt(sum([v*v for v in cross]))/l.jet.p()

    def jetProjectedMET(self, met, jets, oneSided=True):
        import math
        projfactor = 1.0
        for j in jets:
            dphi = abs(deltaPhi(j.phi(), met.phi()))
            proj = sin(dphi)
            if oneSided and dphi > 0.5*math.pi:
                continue
            if projfactor > proj: projfactor = proj
        return met.pt()*projfactor

    def makeMETs(self, event):
        event.met = self.handles['met'].product()[0]
        event.metNoPU = self.handles['nopumet'].product()[0]
        if hasattr(event, 'deltaMetFromJetSmearing'):
            import ROOT
            px,py = event.met.px()+event.deltaMetFromJetSmearing[0], event.met.py()+event.deltaMetFromJetSmearing[1]
            event.met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            px,py = event.metNoPU.px()+event.deltaMetFromJetSmearing[0], event.metNoPU.py()+event.deltaMetFromJetSmearing[1]
            event.metNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
        metMatrix = self.handles['metSignificance'].product().significance()
        metMatrix.Invert();
        import array
        metVector = TVectorD(2,array.array('d',[event.met.px(), event.met.py()]))
        event.metSignificance = metMatrix.Similarity(metVector) 
        event.projMetAll1S  = self.jetProjectedMET(event.met, event.jets,True)
        event.projMetAll2S  = self.jetProjectedMET(event.met, event.jets,False)
        event.projMetJets1S = self.jetProjectedMET(event.met, event.cleanJets,True)
        event.projMetJets2S = self.jetProjectedMET(event.met, event.cleanJets,False)
        #print "MET value:  ", event.met.pt()
        #print "MET sumET:  ", event.met.sumEt()
        #print "MET signif: ", event.metSignificance
        #print "PrMETAll 1S:", event.projMetAll1S 
        #print "PrMETAll 2S:", event.projMetAll2S 
        #print "PrMETJet 1S:", event.projMetJets1S 
        #print "PrMETJet 2S:", event.projMetJets2S

    def makeHadTopDecays(self, event):
        event.lightJets = [ j for j in event.cleanJets if not j.getSelection("cuts_csv_medium") ]
        event.minMWjj   = 999
        event.minMWjjPt = 0
        event.bestMWjj   = 0
        event.bestMWjjPt = 0
        event.bestMTopHad   = 0
        event.bestMTopHadPt = 0
        for i1,j1 in enumerate(event.lightJets):
            for i2 in xrange(i1+1,len(event.lightJets)):
                j2 = event.lightJets[i2]
                jjp4 = j1.p4() + j2.p4()
                mjj  = jjp4.M()
                if mjj > 30 and mjj < event.minMWjj:
                    event.minMWjj = mjj
                    event.minMWjjPt = jjp4.Pt()
                if abs(mjj-80.4) < abs(event.bestMWjj-80.4):
                    event.bestMWjj = mjj
                    event.bestMWjjPt = jjp4.Pt()
                    for bj in event.bjetsLoose:
                        if deltaR(bj.eta(),bj.phi(),j1.eta(),j1.phi()) < 0.1 or deltaR(bj.eta(),bj.phi(),j2.eta(),j2.phi()) < 0.1: continue
                        tp4 = jjp4 + bj.p4()
                        mtop = tp4.M()
                        if abs(mtop-172) < abs(event.bestMTopHad - 172):
                            event.bestMTopHad = mtop
                            event.bestMTopHadPt = tp4.Pt()

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        if hasattr(self.cfg_ana, 'minJets25'):
            n25 = len([ j for j in event.cleanJets if j.pt() > 25 ])
            if n25 < self.cfg_ana.minJets25: 
                return False

        eventNumber = iEvent.eventAuxiliary().id().event()

        event.bjetsLoose  = [ j for j in event.cleanJets if j.getSelection("cuts_csv_loose")  ]
        event.bjetsMedium = [ j for j in event.cleanJets if j.getSelection("cuts_csv_medium") ]

        objects25 = [ j for j in event.cleanJets if j.pt() > 25 ] + event.selectedLeptons
        objects30 = [ j for j in event.cleanJets if j.pt() > 30 ] + event.selectedLeptons
        event.htJet25 = sum([x.pt() for x in objects25])
        event.mhtJet25 = hypot(sum([x.px() for x in objects25]), sum([x.py() for x in objects25]))
        event.htJet30 = sum([x.pt() for x in objects30])
        event.mhtJet30 = hypot(sum([x.px() for x in objects30]), sum([x.py() for x in objects30]))
        ## same but with all eta range
        objects25a = [ j for j in event.cleanJetsAll if j.pt() > 25 ] + event.selectedLeptons
        objects30a = [ j for j in event.cleanJetsAll if j.pt() > 30 ] + event.selectedLeptons
        event.htJet25a = sum([x.pt() for x in objects25a])
        event.mhtJet25a = hypot(sum([x.px() for x in objects25a]), sum([x.py() for x in objects25a]))
        event.htJet30a = sum([x.pt() for x in objects30a])
        event.mhtJet30a = hypot(sum([x.px() for x in objects30a]), sum([x.py() for x in objects30a]))

        self.makeMETs(event);
        self.makeZs(event, self.maxLeps)
        self.makeMlls(event, self.maxLeps)

        self.makeHadTopDecays(event)
        for lep in event.selectedLeptons:
            self.leptonMVA.addMVA(lep)

        if self.cfg_ana.verbose:
            print 'Event ',eventNumber
            print 'Selected leptons: ',len(event.selectedLeptons)
            print "\n".join(["\t%s" % p for p in event.selectedLeptons])
            print 'Selected clean jets   : ',len(event.cleanJets)
            print "\n".join(["\t%s" % p for p in event.cleanJets])
            print 'Selected loose  b-jets: ',len(event.bjetsLoose)
            print 'Selected medium b-jets: ',len(event.bjetsMedium)
            print 'Best Z1, if any: mass = %6.1f ' % event.bestZ1[0]
            print 'Best Z2, if any: mass = %6.1f ' % event.bestZ2[3]
            print 'Min m(ll) values: SF/OS = %5.1f, AF/OS = %5.1f, AF/AS = %5.1f' % (event.minMllSFOS, event.minMllAFOS, event.minMllAFAS)

        return True

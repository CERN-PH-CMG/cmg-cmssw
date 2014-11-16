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
from CMGTools.TTHAnalysis.signedSip import *
import os
        
class ttHCoreEventAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHCoreEventAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.maxLeps = cfg_ana.maxLeps
        self.leptonMVATTH  = LeptonMVA("TTH", "%s/src/CMGTools/TTHAnalysis/data/leptonMVA/%%s_BDTG.weights.xml" % os.environ['CMSSW_BASE'], self.cfg_comp.isMC)
        self.leptonMVASusy = LeptonMVA("Susy","%s/src/CMGTools/TTHAnalysis/data/leptonMVA/susy/%%s_BDTG.weights.xml" % os.environ['CMSSW_BASE'], self.cfg_comp.isMC)

    def declareHandles(self):
        super(ttHCoreEventAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )
        self.handles['nopumet'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )

    def beginLoop(self):
        super(ttHCoreEventAnalyzer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

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
        event.maxMllSFOS = max(mllsfos)
        event.maxMllAFAS = max(mllafas)
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
        event.vtx2l = twoTrackChi2(leps[0],leps[1]) if nlep >= 2 else (-1,-1)

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

    def makeLepPtRel(self, event):
        for l in event.selectedLeptons:
            (px,py,pz) = (l.px(),l.py(),l.pz())
            (jx,jy,jz) = (l.jet.px(),l.jet.py(),l.jet.pz())
            cross = (px*jy-py*jx, py*jz-pz*jy, pz*jx-px*jz)
            l.ptRelJet = sqrt(sum([v*v for v in cross]))/l.jet.p()

    
#    def makeMETs(self, event):
#        event.met = self.handles['met'].product()[0]
#        event.metNoPU = self.handles['nopumet'].product()[0]
#        if hasattr(event, 'deltaMetFromJetSmearing'):
#            import ROOT
#            px,py = event.met.px()+event.deltaMetFromJetSmearing[0], event.met.py()+event.deltaMetFromJetSmearing[1]
#            event.met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
#            px,py = event.metNoPU.px()+event.deltaMetFromJetSmearing[0], event.metNoPU.py()+event.deltaMetFromJetSmearing[1]
#            event.metNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
#        if hasattr(event, 'deltaMetFromJEC') and event.deltaMetFromJEC[0] != 0 and event.deltaMetFromJEC[1] != 0:
#            import ROOT
#            px,py = event.met.px()+event.deltaMetFromJEC[0], event.met.py()+event.deltaMetFromJEC[1]
#            event.met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
#            px,py = event.metNoPU.px()+event.deltaMetFromJEC[0], event.metNoPU.py()+event.deltaMetFromJEC[1]
#            event.metNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

##        self.handles['rho'] = AutoHandle( ('fixedGridRhoFastjetAll','',''), 'double' )
##        event.rho  = float(self.handles['rho'].product()[0])
##        event.rho = self.handles['fixedGridRhoFastjetAll'] .product()[0]
        event.bjetsLoose  = [ j for j in event.cleanJets if j.btagWP("CSVL") ]
        event.bjetsMedium = [ j for j in event.cleanJets if j.btagWP("CSVM") ]

        import ROOT

        ## with Central Jets
        objects25 = [ j for j in event.cleanJets if j.pt() > 25 ] + event.selectedLeptons
        objects30 = [ j for j in event.cleanJets if j.pt() > 30 ] + event.selectedLeptons
        objects40 = [ j for j in event.cleanJets if j.pt() > 40 ] + event.selectedLeptons
        objects40j = [ j for j in event.cleanJets if j.pt() > 40 ] 
        objects50j = [ j for j in event.cleanJets if j.pt() > 50 ] 
        objects40j10l = [ j for j in event.cleanJets if j.pt() > 40 ] + [ l for l in event.selectedLeptons if l.pt() > 10 ]

        event.htJet25 = sum([x.pt() for x in objects25])
        event.mhtJet25vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects25])) , -1.*(sum([x.py() for x in objects25])), 0, 0 )     
        event.mhtPhiJet25 = event.mhtJet25vec.phi()
        event.mhtJet25 = event.mhtJet25vec.pt()

        event.htJet30 = sum([x.pt() for x in objects30])
        event.mhtJet30vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects30])) , -1.*(sum([x.py() for x in objects30])), 0, 0 )             
        event.mhtJet30 = event.mhtJet30vec.pt()
        event.mhtPhiJet30 = event.mhtJet30vec.phi()

        event.htJet40 = sum([x.pt() for x in objects40])
        event.mhtJet40vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40])) , -1.*(sum([x.py() for x in objects40])), 0, 0 )             
        event.mhtJet40 = event.mhtJet40vec.pt()
        event.mhtPhiJet40 = event.mhtJet40vec.phi()

        event.htJet40j = sum([x.pt() for x in objects40j])
        event.mhtJet40jvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40j])) , -1.*(sum([x.py() for x in objects40j])), 0, 0 )               
        event.mhtJet40j = event.mhtJet40jvec.pt()
        event.mhtPhiJet40j = event.mhtJet40jvec.phi()        

        event.htJet50j = sum([x.pt() for x in objects50j])
        event.mhtJet50jvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects50j])) , -1.*(sum([x.py() for x in objects50j])), 0, 0 )               
        event.mhtJet50j = event.mhtJet50jvec.pt()
        event.mhtPhiJet50j = event.mhtJet50jvec.phi()        

        event.htJet40j10l = sum([x.pt() for x in objects40j10l])
        event.mhtJet40j10lvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40j10l])) , -1.*(sum([x.py() for x in objects40j10l])), 0, 0 )               
        event.mhtJet40j10l = event.mhtJet40j10lvec.pt()
        event.mhtPhiJet40j10l = event.mhtJet40j10lvec.phi()        

        ## same but with all eta range
        objects25a  = [ j for j in event.cleanJetsAll if j.pt() > 25 ] + event.selectedLeptons
        objects30a  = [ j for j in event.cleanJetsAll if j.pt() > 30 ] + event.selectedLeptons
        objects40a  = [ j for j in event.cleanJetsAll if j.pt() > 40 ] + event.selectedLeptons
        objects40ja = [ j for j in event.cleanJetsAll if j.pt() > 40 ] 
        objects40ja10l = [ j for j in event.cleanJetsAll if j.pt() > 40 ] + [ l for l in event.selectedLeptons if l.pt() > 10 ]

        event.htJet25a = sum([x.pt() for x in objects25a])
        event.mhtJet25veca = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects25a])) , -1.*(sum([x.py() for x in objects25a])), 0, 0 )     
        event.mhtPhiJet25a = event.mhtJet25veca.phi()
        event.mhtJet25a = event.mhtJet25veca.pt()

        event.htJet30a = sum([x.pt() for x in objects30a])
        event.mhtJet30veca = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects30a])) , -1.*(sum([x.py() for x in objects30a])), 0, 0 )             
        event.mhtJet30a = event.mhtJet30veca.pt()
        event.mhtPhiJet30a = event.mhtJet30veca.phi()

        event.htJet40a = sum([x.pt() for x in objects40a])
        event.mhtJet40veca = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40a])) , -1.*(sum([x.py() for x in objects40a])), 0, 0 )             
        event.mhtJet40a = event.mhtJet40veca.pt()
        event.mhtPhiJet40a = event.mhtJet40veca.phi()

        event.htJet40ja = sum([x.pt() for x in objects40ja])
        event.mhtJet40jveca = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40ja])) , -1.*(sum([x.py() for x in objects40ja])), 0, 0 )                     
        event.mhtJet40ja = event.mhtJet40jveca.pt()
        event.mhtPhiJet40ja = event.mhtJet40jveca.phi()

        #For the vertex related variables 
        #A = selectedLeptons[0], B = selectedLeptons[1], C = selectedLeptons[2], D = selectedLeptons[3] 
        nlep = len(event.selectedLeptons)
        
        ##Variables related to IP
        #Of one lepton w.r.t. the PV of the event
        event.absIP3DA = absIP3D(event.selectedLeptons[0],event.goodVertices[0]) if nlep > 0 else (-1,-1)  
        event.absIP3DB = absIP3D(event.selectedLeptons[1],event.goodVertices[0]) if nlep > 1 else (-1,-1)    
 
        event.absIP3DC = absIP3D(event.selectedLeptons[2],event.goodVertices[0]) if nlep > 2 else (-1,-1)
 
        event.absIP3DD = absIP3D(event.selectedLeptons[3],event.goodVertices[0]) if nlep > 3 else (-1,-1)
        #Of one lepton w.r.t. the PV of the PV of the other leptons only
        event.absIP3DApvBC = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[0],3,0) if nlep > 2 else (-1,-1)
        event.absIP3DBpvAC = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[0],3,1) if nlep > 2 else (-1,-1)
        event.absIP3DCpvAB = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[0],3,2) if nlep > 2 else (-1,-1)
 
        event.absIP3DApvBCD = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[3],4,0) if nlep > 3 else (-1,-1)
        event.absIP3DBpvACD = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[3],4,1) if nlep > 3 else (-1,-1)
        event.absIP3DCpvABD = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[3],4,2) if nlep > 3 else (-1,-1)
        event.absIP3DDpvABC = absIP3Dtrkpvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[3],4,3) if nlep > 3 else (-1,-1)
        
        ##Variables related to chi2
        #Chi2 of all the good leptons of the event but one lepton
        event.chi2pvtrksBCbutA = chi2pvtrks(event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[1],event.selectedLeptons[1],2) if nlep > 2 else (-1,-1)
        event.chi2pvtrksACbutB = chi2pvtrks(event.selectedLeptons[0],event.selectedLeptons[2],event.selectedLeptons[0],event.selectedLeptons[0],2) if nlep > 2 else (-1,-1)
        event.chi2pvtrksABbutC = chi2pvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[0],event.selectedLeptons[0],2) if nlep > 2 else (-1,-1)

        event.chi2pvtrksBCDbutA = chi2pvtrks(event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[3],event.selectedLeptons[1],3) if nlep > 3 else (-1,-1)
        event.chi2pvtrksACDbutB = chi2pvtrks(event.selectedLeptons[0],event.selectedLeptons[2],event.selectedLeptons[3],event.selectedLeptons[0],3) if nlep > 3 else (-1,-1)
        event.chi2pvtrksABDbutC = chi2pvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[3],event.selectedLeptons[0],3) if nlep > 3 else (-1,-1)
        event.chi2pvtrksABCbutD = chi2pvtrks(event.selectedLeptons[0],event.selectedLeptons[1],event.selectedLeptons[2],event.selectedLeptons[0],3) if nlep > 3 else (-1,-1)

        ###event.weirdAssVar = sum([x.eta() for x in objects40a])
        
#        self.makeMETs(event);
        self.makeZs(event, self.maxLeps)
        self.makeMlls(event, self.maxLeps)
        self.makeLepPtRel(event)

        # look for minimal deltaPhi between MET and four leading jets with pt>40 and eta<2.4
        event.deltaPhiMin_had = 999.
        for n,j in enumerate(objects40ja):
            if n>3:  break
            thisDeltaPhi = abs( deltaPhi( j.phi(), event.met.phi() ) )
            if thisDeltaPhi < event.deltaPhiMin_had : event.deltaPhiMin_had = thisDeltaPhi

        event.deltaPhiMin = 999.
        for n,j in enumerate(objects40ja10l):
            if n>3:  break
            thisDeltaPhi = abs( deltaPhi( j.phi(), event.met.phi() ) )
            if thisDeltaPhi < event.deltaPhiMin : event.deltaPhiMin = thisDeltaPhi

        for lep in event.selectedLeptons:
            lep.mvaValue     = self.leptonMVATTH(lep)
            lep.mvaValueSusy = self.leptonMVASusy(lep)
        for lep in event.inclusiveLeptons:
            if lep not in event.selectedLeptons:
                lep.mvaValue     = self.leptonMVATTH(lep)
                lep.mvaValueSusy = self.leptonMVASusy(lep)


        # absolute value of the vectorial difference between met and mht
        diffMetMht_had_vec = ROOT.reco.Particle.LorentzVector(event.mhtJet40jvec.px()-event.met.px(), event.mhtJet40jvec.py()-event.met.py(), 0, 0 )
        event.diffMetMht_had = sqrt( diffMetMht_had_vec.px()*diffMetMht_had_vec.px() + diffMetMht_had_vec.py()*diffMetMht_had_vec.py() )

        diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.mhtJet40j10lvec.px()-event.met.px(), event.mhtJet40j10lvec.py()-event.met.py(), 0, 0 )
        event.diffMetMht = sqrt( diffMetMht_vec.px()*diffMetMht_vec.px() + diffMetMht_vec.py()*diffMetMht_vec.py() )
        ###
        return True

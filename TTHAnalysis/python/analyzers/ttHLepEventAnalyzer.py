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

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi,bestMatch
from CMGTools.TTHAnalysis.leptonMVA import LeptonMVA

        
class ttHLepEventAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepEventAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.maxLeps = cfg_ana.maxLeps

        variables = []
        variables.append(  ('sip3d', 'F', lambda lep : lep.sip3D()) )
        variables.append(  ('dxy := min(abs(dxy),0.2)', 'F', lambda lep : min(abs(lep.dxy()), 0.2)) )
        variables.append(  ('dz := min(abs(dz),0.3)', 'F', lambda lep : min(abs(lep.dz()), 0.3)) )
        variables.append(  ('relIso', 'F', lambda lep : lep.relIso(dBetaFactor=0.5)) )
        variables.append(  ('jetPtRatio := min(jetPtRatio,1)', 'F', lambda lep : min(lep.pt()/lep.jet.pt(), 1)) )
        variables.append(  ('jetBTagCSV := max(jetBTagCSV,0)', 'F', lambda lep : max(0,
                                    lep.jet.btag('combinedSecondaryVertexBJetTags') if hasattr(lep.jet, 'btag') else -99)) )
        self.leptonMVA = LeptonMVA("BDTG", variables, "/afs/cern.ch/user/b/botta/public/prova_BDTG.weights.xml")
    def declareHandles(self):
        super(ttHLepEventAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'cmgPFMET', 'std::vector<cmg::BaseMET>' )
        self.handles['metSignificance'] = AutoHandle( 'pfMetSignificance', 'cmg::METSignificance' )

    def beginLoop(self):
        super(ttHLepEventAnalyzer,self).beginLoop()

    def makeZs(self, event, maxLeps):
        event.bestZ1 = [ 0., -1,-1 ]
        event.bestZ2 = [ 0., -1,-1, 0. ]
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if l1.pdgId() == -l2.pdgId():
                    zmass = (l1.p4() + l2.p4()).M()
                    if abs(zmass - 91.188) < abs(event.bestZ1[0] - 91.188):
                        event.bestZ1 = [ zmass, i, j ]
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
        event.minMllSFOS = min(self.mllValues(event,  lambda l1,l2 : l1.pdgId()  == -l2.pdgId(), maxLeps))
        event.minMllAFOS = min(self.mllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(), maxLeps))
        event.minMllAFAS = min(self.mllValues(event,  lambda l1,l2 : True, maxLeps))

    def mllValues(self, event, pairSelection, maxLeps):
        pairs = []
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if pairSelection(l1,l2):
                    pairs.append( (l1.p4() + l2.p4()).M() )
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

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()

        event.bjetsLoose  = [ j for j in event.cleanJets if j.getSelection("cuts_csv_loose")  ]
        event.bjetsMedium = [ j for j in event.cleanJets if j.getSelection("cuts_csv_medium") ]

        objects25 = [ j for j in event.cleanJets if j.pt() > 25 ] + event.selectedLeptons
        objects30 = [ j for j in event.cleanJets if j.pt() > 30 ] + event.selectedLeptons
        event.htJet25 = sum([x.pt() for x in objects25])
        event.mhtJet25 = hypot(sum([x.px() for x in objects25]), sum([x.py() for x in objects25]))
        event.htJet30 = sum([x.pt() for x in objects30])
        event.mhtJet30 = hypot(sum([x.px() for x in objects30]), sum([x.py() for x in objects30]))

        self.makeMETs(event);
        self.makeZs(event, self.maxLeps)
        self.makeMlls(event, self.maxLeps)

        self.makeLepBJetDeltaR(event)

        for lep in event.selectedLeptons:
            mva = self.leptonMVA(lep)
            #print "for selected lepton ",lep," MVA = ",mva
            lep.mvaValue = mva   

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

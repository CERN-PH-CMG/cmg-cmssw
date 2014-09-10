import copy
import types
from math import *

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Jet import Jet
from CMGTools.TTHAnalysis.analyzers.ttHJetAnalyzer import cleanNearestJetOnly

from CMGTools.RootTools.utils.DeltaR import *


        
class ttHJetMCMatchAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHJetMCMatchAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHJetMCMatchAnalyzer, self).declareHandles()
        self.handles['genJet'] = AutoHandle( 'slimmedGenJets', 'vector<reco::GenJet>' )
        self.shiftJER = self.cfg_ana.shiftJER if hasattr(self.cfg_ana, 'shiftJER') else 0

    def beginLoop(self):
        super(ttHJetMCMatchAnalyzer,self).beginLoop()

    def jetFlavour(self,event):
        def isFlavour(x,f):
            id = abs(x.pdgId())
            if id > 999: return (id/1000)%10 == f
            if id >  99: return  (id/100)%10 == f
            return id % 100 == f

        event.bqObjects = [ p for p in event.genParticles if (p.status() == 2 and isFlavour(p,5)) ]
        event.cqObjects = [ p for p in event.genParticles if (p.status() == 2 and isFlavour(p,4)) ]
        
        for jet in event.jets:
           (bmatch, dr) = bestMatch(jet, event.bqObjects)
           if dr < 0.5:
               jet.mcFlavour = 5
           else:
               (cmatch, dr) = bestMatch(jet, event.cqObjects) 
               if dr < 0.5:
                   jet.mcFlavour = 4
               else:
                   jet.mcFlavour = 0

        event.heaviestQCDFlavour = 5 if len(event.bqObjects) else (4 if len(event.cqObjects) else 1);
                    
    def matchJets(self, event):
        match = matchObjectCollection2(event.cleanJetsAll,
                                       event.genbquarks + event.genwzquarks,
                                       deltaRMax = 0.3)
        for jet in event.cleanJetsAll:
            gen = match[jet]
            jet.mcParton    = gen
            jet.mcMatchId   = (gen.sourceId     if gen != None else 0)
            jet.mcMatchFlav = (abs(gen.pdgId()) if gen != None else 0)

        match = matchObjectCollection2(event.cleanJetsAll,
                                       event.genJets,
                                       deltaRMax = 0.3)
        for jet in event.cleanJetsAll:
            jet.mcJet = match[jet]
 
    def smearJets(self, event):
        # https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTopRefSyst#Jet_energy_resolution
       for jet in event.cleanJetsAll:
            gen = jet.mcJet 
            if gen != None:
               genpt, jetpt, aeta = gen.pt(), jet.pt(), abs(jet.eta())
               # from https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
               factor = 1.052 + self.shiftJER*hypot(0.012,0.062);
               if   aeta > 2.3: factor = 1.288 + self.shiftJER*hypot(0.127,0.154)
               elif aeta > 1.7: factor = 1.134 + self.shiftJER*hypot(0.035,0.066)
               elif aeta > 1.1: factor = 1.096 + self.shiftJER*hypot(0.017,0.063)
               elif aeta > 0.5: factor = 1.057 + self.shiftJER*hypot(0.012,0.056)
               ptscale = max(0.0, (jetpt + (factor-1)*(jetpt-genpt))/jetpt)
               #print "get with pt %.1f (gen pt %.1f, ptscale = %.3f)" % (jetpt,genpt,ptscale)
               event.deltaMetFromJetSmearing[0] -= (ptscale-1)*jet.rawFactor()*jet.px()
               event.deltaMetFromJetSmearing[1] -= (ptscale-1)*jet.rawFactor()*jet.py()
               if ptscale != 0:
                  jet.setP4(jet.p4()*ptscale)
               # leave the uncorrected unchanged for sync
               jet._rawFactor = jet.rawFactor()/ptscale if ptscale != 0 else 0
               jet.rawFactor = types.MethodType(lambda self : self._rawFactor, jet, jet.__class__)
            #else: print "jet with pt %.1d, eta %.2f is unmatched" % (jet.pt(), jet.eta())

    def process(self, iEvent, event):
        ## define by how much the MET should be changed because of jet smearing
        event.deltaMetFromJetSmearing = [0, 0]

        ## if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        self.readCollections( iEvent )
        event.genJets = [ x for x in self.handles['genJet'].product() ]

        #cleanGenJets, dummy = cleanObjectCollection(event.genJets, 
        #                                            masks = event.selectedLeptons,
        #                                            deltaRMin = 0.5 )
        cleanGenJets = cleanNearestJetOnly(event.genJets, event.selectedLeptons, 0.5)

        event.nGenJets25 = 0
        event.nGenJets25Cen = 0
        event.nGenJets25Fwd = 0
        for j in cleanGenJets:
            event.nGenJets25 += 1
            if abs(j.eta()) <= 2.4: event.nGenJets25Cen += 1
            else:                   event.nGenJets25Fwd += 1

        self.matchJets(event)
        self.jetFlavour(event)

        if not hasattr(self.cfg_ana, 'smearJets') or self.cfg_ana.smearJets:
            self.smearJets(event)

        return True

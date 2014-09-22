import operator
import itertools
import copy
import types

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Photon import Photon

from CMGTools.TTHAnalysis.analyzers.ttHLepMCMatchAnalyzer import matchObjectCollection3
from CMGTools.RootTools.utils.DeltaR import deltaR, deltaPhi, bestMatch

class ttHPhotonAnalyzerSusy( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHPhotonAnalyzerSusy,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHPhotonAnalyzerSusy, self).declareHandles()

    #----------------------------------------                                                                                                                                   
    # DECLARATION OF HANDLES OF PHOTONS STUFF                                                                                                                                   
    #----------------------------------------     

        #photons
        self.handles['photons'] = AutoHandle( self.cfg_ana.photons,'std::vector<pat::Photon>')

    def beginLoop(self):
        super(ttHPhotonAnalyzerSusy,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('has >=1 gamma at preselection')
        count.register('has >=1 selected gamma')

##    tauID = "PhotonCutBasedID",

    def makePhotons(self, event):
        event.allphotons = map( Photon, self.handles['photons'].product() )
        event.allphotons.sort(key = lambda l : l.pt(), reverse = True)

        event.selectedPhotons = []
        ### MM
        #event.loosePhotons = []
        event.loosePhotonsCentral = []
        ###

        foundPhoton = False
        for gamma in event.allphotons:
            if gamma.pt() < self.cfg_ana.ptMin: continue
            if abs(gamma.eta()) > self.cfg_ana.etaMax: continue
            foundPhoton = True

            def idWP(gamma,X):
                """Create an integer equal to 1-2-3 for (loose,medium,tight)"""

## medium not stored
##                return gamma.photonID(X%"Loose") + gamma.photonID(X%"Medium") + gamma.photonID(X%"Tight")

                id=-1
                if gamma.photonID(X%"Loose"):
                    id=0
                if gamma.photonID(X%"Tight"):
                    id=2
                return id

            gamma.idCutBased = idWP(gamma, "PhotonCutBasedID%s")

            if gamma.photonID(self.cfg_ana.gammaID):
                event.selectedPhotons.append(gamma)
##            event.selectedPhotons.append(gamma)        
            
            ### MM
            if gamma.photonID(self.cfg_ana.gammaID) and abs(gamma.eta()) < self.cfg_ana.etaCentral:
                event.loosePhotonsCentral.append(gamma)
            ###

        event.selectedPhotons.sort(key = lambda l : l.pt(), reverse = True)
        event.loosePhotonsCentral.sort(key = lambda l : l.pt(), reverse = True)

        self.counters.counter('events').inc('all events')
        if foundPhoton: self.counters.counter('events').inc('has >=1 gamma at preselection')
        if len(event.selectedPhotons): self.counters.counter('events').inc('has >=1 selected gamma')
       
    def matchPhotons(self, event):
        event.genPhotons = [ x for x in event.genParticles if x.status() == 3 and abs(x.pdgId()) == 22 ]
        match = matchObjectCollection3(event.allphotons, event.genPhotons, deltaRMax = 0.5)
        for gamma in event.allphotons:
            gen = match[gamma]
            gamma.mcMatchId = 1 if gen else 0

### MM
    def makeGammaObjects(self, event):

        import ROOT

        ## with Central Jets
        gamma_objects25 = [ j for j in event.gamma_cleanJets if j.pt() > 25 ] + event.selectedLeptons
        gamma_objects30 = [ j for j in event.gamma_cleanJets if j.pt() > 30 ] + event.selectedLeptons
        gamma_objects40  = [ j for j in event.gamma_cleanJetsAll if j.pt() > 40 and abs(j.eta()) < 2.5 ] + [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
        gamma_objects40j = [ j for j in event.gamma_cleanJets if j.pt() > 40 ]

        event.gamma_htJet25 = sum([x.pt() for x in gamma_objects25])
        event.gamma_mhtJet25vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects25])) , -1.*(sum([x.py() for x in gamma_objects25])), 0, 0 )
        event.gamma_mhtPhiJet25 = event.mhtJet25vec.phi()
        event.gamma_mhtJet25 = event.mhtJet25vec.pt()

        event.gamma_htJet30 = sum([x.pt() for x in gamma_objects30])
        event.gamma_mhtJet30vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects30])) , -1.*(sum([x.py() for x in gamma_objects30])), 0, 0 )
        event.gamma_mhtJet30 = event.mhtJet30vec.pt()
        event.gamma_mhtPhiJet30 = event.mhtJet30vec.phi()

        event.gamma_htJet40 = sum([x.pt() for x in gamma_objects40])
        event.gamma_mhtJet40vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects40])) , -1.*(sum([x.py() for x in gamma_objects40])), 0, 0 )
        event.gamma_mhtJet40 = event.gamma_mhtJet40vec.pt()
        event.gamma_mhtPhiJet40 = event.gamma_mhtJet40vec.phi()

        event.gamma_htJet40j = sum([x.pt() for x in gamma_objects40j])
        event.gamma_mhtJet40jvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects40j])) , -1.*(sum([x.py() for x in gamma_objects40j])), 0, 0 )
        event.gamma_mhtJet40j = event.gamma_mhtJet40jvec.pt()
        event.gamma_mhtPhiJet40j = event.gamma_mhtJet40jvec.phi()

         # MET + photon
        event.gamma_met = ROOT.reco.Particle.LorentzVector( event.met.px(), event.met.py(), 0, 0 )
        event.gamma_metNoPU = ROOT.reco.Particle.LorentzVector( event.metNoPU.px(), event.metNoPU.py(), 0, 0 )
        for gamma in event.loosePhotonsCentral:
            event.gamma_met = ROOT.reco.Particle.LorentzVector( event.gamma_met.px() + gamma.px(), event.gamma_met.py() + gamma.py() , 0, 0 )
            event.gamma_metNoPU = ROOT.reco.Particle.LorentzVector( event.gamma_metNoPU.px() + gamma.px(), event.gamma_metNoPU.py() + gamma.py() , 0, 0 )

        # look for minimal deltaPhi between MET and four leading jets with pt>40 and eta<2.4
        event.gamma_deltaPhiMin = 999.
        for n,j in enumerate(gamma_objects40j):
            if n>3:  break
            thisDeltaPhi = abs( deltaPhi( j.phi(), event.gamma_met.phi() ) )
            if thisDeltaPhi < event.gamma_deltaPhiMin : event.gamma_deltaPhiMin = thisDeltaPhi

        # absolute value of the vectorial difference between met and mht
        gamma_diffMetMht_had_vec = ROOT.reco.Particle.LorentzVector(event.gamma_mhtJet40jvec.px()-event.gamma_met.px(), event.gamma_mhtJet40jvec.py()-event.gamma_met.py(), 0, 0 )
        event.gamma_diffMetMht_had = sqrt( gamma_diffMetMht_had_vec.px()*gamma_diffMetMht_had_vec.px() + gamma_diffMetMht_had_vec.py()*gamma_diffMetMht_had_vec.py() )

        gamma_diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.gamma_mhtJet40vec.px()-event.gamma_met.px(), event.gamma_mhtJet40vec.py()-event.gamma_met.py(), 0, 0 )
        event.gamma_diffMetMht = sqrt( gamma_diffMetMht_vec.px()*gamma_diffMetMht_vec.px() + gamma_diffMetMht_vec.py()*gamma_diffMetMht_vec.py() )


    def printInfo(self, event):
        print '----------------'
        if len(event.selectedPhotons)>0:
            print 'lenght: ',len(event.selectedPhotons)
            print 'gamma candidate pt: ',event.selectedPhotons[0].pt()
            print 'gamma candidate eta: ',event.selectedPhotons[0].eta()
            print 'gamma candidate phi: ',event.selectedPhotons[0].phi()
            print 'gamma candidate mass: ',event.selectedPhotons[0].mass()
            print 'gamma candidate HoE: ',event.selectedPhotons[0].hOVERe()
            print 'gamma candidate r9: ',event.selectedPhotons[0].r9()
            print 'gamma candidate sigmaIetaIeta: ',event.selectedPhotons[0].sigmaIetaIeta()
            print 'gamma candidate had iso: ',event.selectedPhotons[0].chargedHadronIso()
            print 'gamma candidate neu iso: ',event.selectedPhotons[0].neutralHadronIso()
            print 'gamma candidate gamma iso: ',event.selectedPhotons[0].photonIso()
            print 'gamma idCutBased',event.selectedPhotons[0].idCutBased    

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        #call the photons functions
        self.makePhotons(event)
        self.makeGammaObjects(event)
        
#        self.printInfo(event)   

## ===> do matching                                                                                                                                                                                                     
        if not self.cfg_comp.isMC:
            return True

        self.matchPhotons(event)


        return True

import operator 
import itertools
import copy

from ROOT import TLorentzVector, TVector3

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenParticle, Lepton, Photon,Electron,Muon,Tau

from CMGTools.LEP3.analyzers.DiObject import DiObject
from CMGTools.LEP3.analyzers.EMiss import EMiss, EVis

from CMGTools.RootTools.utils.DeltaR import deltaR
from math import pi, sqrt, acos, asin, log
        
class TauTauLLAnalyzerPF( Analyzer ):

    def declareHandles(self):
        super(TauTauLLAnalyzerPF, self).declareHandles()

        self.handles['jets'] = AutoHandle ('cmgPFJetSel',
                                           'std::vector<cmg::PFJet>')

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

        self.handles['electrons'] = AutoHandle ('cmgElectronSel',
                                                'std::vector<cmg::Electron>')
        
        self.handles['muons'] = AutoHandle ('cmgMuonSel',
                                            'std::vector<cmg::Muon>')

        self.handles['photons'] = AutoHandle ('cmgPhotonSel',
                                            'std::vector<cmg::Photon>')

        self.handles['taus'] = AutoHandle( 'cmgTauSel',
                                                     'std::vector<cmg::Tau>' )



        
    def beginLoop(self):
        super(TauTauLLAnalyzerPF,self).beginLoop()
        self.counters.addCounter('TauTauLL')
        self.counters.addCounter('TauTauLLGen')
        counts = []
        counts.append(self.counters.counter('TauTauLL'))
        counts.append(self.counters.counter('TauTauLLGen'))
        for count in counts : 
            count.register('All events')
            count.register('Two Leptons')
            count.register('Two Taus')
            count.register('passing')
        
    def buildGen(self, event):

        event.tautaull = False
        event.eleele = False
        event.mumu = False
        self.genElectrons = []
        self.genMuons = []
        self.genTaus = []
        for ptc in self.mchandles['genParticles'].product():
            isElectron = abs(ptc.pdgId()) == 11
            isMuon = abs(ptc.pdgId()) == 13
            isTau = abs(ptc.pdgId()) == 15

            if isElectron : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.genElectrons.append(GenParticle(ptc))
            elif isMuon : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.genMuons.append(GenParticle(ptc))
            elif isTau :
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 25 : continue
                self.genTaus.append(GenParticle(ptc))
 
        if (len(self.genElectrons) == 2 or len(self.genMuons)==2) and len(self.genTaus) == 2 :
            event.tautaull = True
            if len(self.genElectrons) == 2 :
                event.eleele = True
            else:
                event.mumu = True
            #for ptc in self.mchandles['genParticles'].product():
            #    print GenParticle(ptc)



    def buildLeptonList(self,event):

        self.hadtaus = []
        for ptj in self.handles['taus'].product():
            thistau=Tau(ptj)
            # add only "good" pftaus
            if thistau.tauID("decayModeFinding")!=1.0:
                continue
            if thistau.tauID("byVLooseCombinedIsolationDeltaBetaCorr")<=0.:
                continue
            self.hadtaus.append(Tau(ptj))


        self.jets = []
        for ptj in self.handles['jets'].product():
            # check if they are close to jets and then remove them from the jet list
            thisjet=Jet(ptj)
            matched=False
            # use the following if you want to remove hadtaus from jets
#            for tau in self.hadtaus:
#                if deltaR(tau.eta(),tau.phi(),thisjet.eta(),thisjet.phi()) < 0.2:
#                    matched=True
#                    break
            if not matched:
                self.jets.append(Jet(ptj))


        self.leptons = []
        for pte in self.handles['electrons'].product():
            self.leptons.append(Lepton(pte))
        for ptm in self.handles['muons'].product():
            self.leptons.append(Lepton(ptm))

        

        self.photons = []
        for ptg in self.handles['photons'].product():
            if ptg.energy() > 1.0 : self.photons.append(Photon(ptg))

        # Find FSR
        tmpLept = set(self.leptons)
        for lept in self.leptons :
            leptonIso = self.leptonIso(lept)
            if leptonIso > 0.05 :
                tmpLept.remove(lept)
                continue
            
            #if (self.eleele or self.mumu or self.tautau ) : 
            #    print self.eleele, self.mumu, self.tautau,lept, leptonIso
            for i,ph in enumerate(self.photons) :
                dr = deltaR(lept.eta(),lept.phi(),ph.eta(),ph.phi())
                if abs(lept.pdgId()) == 13 and dr < 0.4 :
                    #print 'found ISR ',ph,lept
                    leph = lept.p4()
                    p4ph = ph.p4()
                    p4 = ph.p4() + lept.p4()
                    lept.setP4(p4)
                    #print 'added ISR ',ph,lept
        self.leptons = []

        for lept in tmpLept :
            self.leptons.append(lept)

        self.leptonJets = []
        for lept in self.leptons :
            drmin = 999.
            
            ijet = -1
            for i,jet in enumerate(self.jets) :
                dr = deltaR(lept.eta(),lept.phi(),jet.eta(),jet.phi())
                if dr < drmin :
                    drmin = dr
                    ijet = i
                    
            if ijet >= 0 :
                if drmin < 0.1 :
                    self.jets[ijet].setP4(lept.p4())
                    self.jets[ijet].setPdgId(lept.pdgId())
                    self.leptonJets.append(self.jets[ijet])
                    
        # self.leptonJets contains all jets close to a lepton
        # self.jets contains jets (if close to leptons the p4 is the one from leptons)

# maybe we should treat pftaus(had) as standard leptons?

    def buildTauList(self,event):
            
        llpairs = self.findPairs(self.leptonJets)
        dm = 999.
        event.llmass = 0.
        llindex = -1
        for ill,ll in enumerate(llpairs) :
            if abs(ll.M()-91.2) < dm:
                dm = abs(ll.M()-91.2)
                event.llmass = ll.M()
                llindex = ill

        self.zLeptons = []
        self.zLeptons.append(llpairs[llindex].leg1)
        self.zLeptons.append(llpairs[llindex].leg2)
        event.zLeptons = self.zLeptons
        event.zPair = llpairs[llindex]

        # define also an umatched jet list to see what is remaining
        self.unmatch=set(self.jets)

        # tau candidates (including low energy leptons)
        self.tauJets = []
        for hadtau in self.hadtaus:
            self.tauJets.append(hadtau)
            for jet in self.jets:
                if deltaR(hadtau.eta(),hadtau.phi(),jet.eta(),jet.phi()) < 0.2:
                    self.unmatch.remove(jet)
                    
        for leptonJet in set(self.leptonJets):
            if leptonJet in self.unmatch:
                self.unmatch.remove(leptonJet)
            if leptonJet not in event.zLeptons:
                self.tauJets.append(leptonJet)

        
# this is before pfjets
#        tmpJets = set(self.jets)
#        tmpLeptons = set(event.zLeptons)
#        for leptonJet in tmpLeptons:
#            tmpJets.remove(leptonJet)

#        self.tauJets = []
#        for jet in tmpJets:
#            self.tauJets.append(jet)
            
        self.tauJets.sort(key=lambda a: a.energy(), reverse = True)

        # self.tauJets is the list of jets excluding the leptons (e and mu) which pair closest to mz 
        # self.taucand is the list of pftau + leptons (e and mu)excluding the lepton-pair closest to mz 
        # self.unmatch contains all jets not matched with tauJets (candidates taus) and Z leptons
        
        







##     def buildJetList(self, event):

        
##         self.jet3 = []

##         if len(self.jets) > 2 : 
##             self.jets.sort(key=lambda a: a.energy(), reverse = True)
##             tmpJets = set(self.jets)
##             tmpJet3 = set(self.jets)
## #            print 'Jets Avant : '
## #            for jet in tmpJets:
## #                print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
##             while len(tmpJets) != 2:
##                 # Keep the step with three jets
##                 if len(tmpJet3) == 3 :
##                     for jet in tmpJet3 :
##                         self.jet3.append(jet)
##                 # 
##                 dijets = self.findPairs(tmpJets)
##                 dijets.sort(key=lambda a: a.M())
## #                print dijets[0],dijets[0].M()
                
##                 tmpJets.remove(dijets[0].leg1)
##                 tmpJets.remove(dijets[0].leg2)
##                 tmpJets.add(dijets[0])

##  #           print 'Jets apres : '
##             self.jets = []
##             for jet in tmpJets:
##  #               print jet,jet.nConstituents(), jet.mass(), jet.btag(7)
##  #               print jet,jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
##                 self.jets.append(jet)
                
##         self.jets.sort(key=lambda a: a.btag(7), reverse = True)

    def buildMiss(self, event):
        event.eMiss = EMiss(self.jets)
        event.eVis = EVis(self.jets)

 
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.buildGen(event)

        #event.allJets = self.jets
        #event.jetPairs = self.findPairs( event.allJets )

        #for ptc in self.mchandles['genParticles'].product():
        #    print GenParticle(ptc)
        #for jet in self.jets :
        #    print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(),jet.btag(7),jet.btag(0)

        self.counters.counter('TauTauLL').inc('All events')
        if event.tautaull : self.counters.counter('TauTauLLGen').inc('All events')
        event.step = 0


        self.buildLeptonList ( event )
        if len(self.leptonJets) < 2:
            return 0
        else:
            event.step +=1
        self.counters.counter('TauTauLL').inc('Two Leptons')
        if event.tautaull : self.counters.counter('TauTauLLGen').inc('Two Leptons')

        self.buildTauList( event )
        event.tauJets = self.tauJets
        if len(self.tauJets) != 2:
            return 0
        else:
            event.step +=1
        self.counters.counter('TauTauLL').inc('Two Taus')
        if event.tautaull : self.counters.counter('TauTauLLGen').inc('Two Taus')

        self.buildMiss( event )

        self.beta4(120.)
        event.llRescaled = self.llRescaled
        event.ttRescaled = self.ttRescaled
        event.rescaledLeptonJets = self.rescaledLeptonJets
        event.rescaledTauJets = self.rescaledTauJets

        self.counters.counter('TauTauLL').inc('passing')
        if event.tautaull : self.counters.counter('TauTauLLGen').inc('passing')
        event.step += 1

        return True
 

    def findPairs(self, jets):
        out = []

        for j1, j2 in itertools.combinations(jets, 2):
            out.append( DiObject(j1, j2) )

        return out

    def leptonIso( self, lepton ):

        phpt = 0.
        for ph in self.photons:
            dr = deltaR(lepton.eta(), lepton.phi(), ph.eta(), ph.phi())
            if dr < 0.4 : phpt += ph.pt()
            
        return max(0., lepton.relIso() - phpt/lepton.pt())

    def beta4(self,ebeam):

        from numpy import array, linalg, dot, add
        
        self.rescaledLeptonJets = copy.copy(self.zLeptons)
        self.rescaledTauJets = copy.copy(self.tauJets)
        
        rows = []
        for i in range(4):
            rows.append([])
            
        for jet in self.rescaledLeptonJets:
            rows[0].append(jet.px()/jet.energy())
            rows[1].append(jet.py()/jet.energy())
            rows[2].append(jet.pz()/jet.energy())
            rows[3].append(jet.energy()/jet.energy())        

        for jet in self.rescaledTauJets:
            rows[0].append(jet.px()/jet.energy())
            rows[1].append(jet.py()/jet.energy())
            rows[2].append(jet.pz()/jet.energy())
            rows[3].append(jet.energy()/jet.energy())        

        constraint = [0.,0.,0.,2.*ebeam]

        d2 = array(rows)
        d = array(constraint)
        #print d2
        #print d
        try : 
            energies = linalg.solve(d2,d)
        except linalg.linalg.LinAlgError:
            energies = [-1.,-1.,-1.,-1.]
        #print energies
        chi2 = 0.
        for i,jet in enumerate(self.rescaledLeptonJets):

            p4 = jet.p4()
            p4.SetPxPyPzE(jet.px()*energies[i]/jet.energy(),
                          jet.py()*energies[i]/jet.energy(),
                          jet.pz()*energies[i]/jet.energy(),
                          energies[i])
            jet.setP4(p4)

        for i,jet in enumerate(self.rescaledTauJets):

            p4 = jet.p4()
            p4.SetPxPyPzE(jet.px()*energies[i+2]/jet.energy(),
                          jet.py()*energies[i+2]/jet.energy(),
                          jet.pz()*energies[i+2]/jet.energy(),
                          energies[i+2])
            jet.setP4(p4)

        self.llRescaled = DiObject(self.rescaledLeptonJets[0],self.rescaledLeptonJets[1])
        self.ttRescaled = DiObject(self.rescaledTauJets[0],self.rescaledTauJets[1])

    def fitmZ(self) :

        dilepton = False
        diele = False
        dimu = False

        if self.jets[0].pdgId() + self.jets[1].pdgId() == 0 and \
           abs( self.jets[0].pdgId() - self.jets[1].pdgId()) > 20 :
            dilepton = True

        #if not(dilepton) : return -99.

        diele =  abs( self.jets[0].pdgId() - self.jets[1].pdgId()) == 22
        dimu =  abs( self.jets[0].pdgId() - self.jets[1].pdgId()) == 26

        l1 = TLorentzVector(self.jets[0].px(),self.jets[0].py(),self.jets[0].pz(),self.jets[0].energy())
        l2 = TLorentzVector(self.jets[1].px(),self.jets[1].py(),self.jets[1].pz(),self.jets[1].energy())
        c12 = l1.Vect().Dot(l2.Vect()) / l1.P() / l2.P()
        st1 = l1.Pt()/l1.P()
        st2 = l2.Pt()/l2.P()
        m12 = (l1+l2).M()/sqrt(l1.E()*l2.E())
        fac = 91.188/(l1+l2).M()
        energies = [ l1.E()*fac  , l2.E()*fac  ]
        measts = [ l1.E(), l2.E() ]

        def chi2(e) :
            
            def breitw2(m,m0,g0) :
                m02 = m0*m0
                g02 = g0*g0
                delta = m*m-m02
                return m02*g02 / (delta*delta+g02*m02)

            def breitw(m,m0,g0) :
                delta = m-m0
                return m0*g0 / (delta*delta + g0*m0)

            chi2 = 0.
            fudge = 1.
            mz=m12*sqrt(e[0]*e[1])
            mzm = m12*sqrt(measts[0]*measts[1])
            #mz = sqrt(2.*e[0]*e[1]*(1.-c12))
            #print 'mz = ',mz
            sigma1 = 0
            sigma2 = 0
            if dimu : 
                chi2 = ( 1./measts[0]-1./e[0] ) * ( 1./measts[0]-1./e[0] ) / (st1*st1) \
                     + ( 1./measts[1]-1./e[1] ) * ( 1./measts[1]-1./e[1] ) / (st2*st2)
                chi2 /= 25E-8
                sigma1 = 5E-4*5E-4*e[0]*e[0]*e[0]*e[0]*st1*st1
                sigma2 = 5E-4*5E-4*e[1]*e[1]*e[0]*e[0]*st2*st2
                fudge = 0.5
            elif diele :
                sigma1 = (0.155*0.155 + 0.043*0.043*e[0] + 0.02*0.02*e[0]*e[0])
                sigma2 = (0.155*0.155 + 0.043*0.043*e[1] + 0.02*0.02*e[1]*e[1])
                chi2 = (measts[0]-e[0])*(measts[0]-e[0]) / sigma1 \
                     + (measts[1]-e[1])*(measts[1]-e[1]) / sigma2
                fudge = 2.0
            else : 
                sigma1 = (0.5*0.5*e[0]/st1 + 0.04*0.04*e[0]*e[0])
                sigma2 = (0.5*0.5*e[1]/st2 + 0.04*0.04*e[1]*e[1])
                chi2 = (measts[0]-e[0])*(measts[0]-e[0]) / sigma1 \
                     + (measts[1]-e[1])*(measts[1]-e[1]) / sigma2
                fudge = 1.0
            #print 'chi2 partial  = ',chi2
            sigmaM = mz*mz*(sigma1/(e[0]*e[0])+sigma2/(e[1]*e[1]))/4.
            #chi2 = (mzm-mz)*(mzm-mz)/sigmaM
            self.chi2partiel = copy.copy(chi2)
            chi2 -= fudge*log(breitw2(mz,91.188,2.497))*sqrt(sigmaM)/2.497
            self.chi2total = copy.copy(chi2)
            #if diele:
            #    print 'chi2 partie/complet = ',dimu,diele,mz,mzm,sqrt(sigma1),sqrt(sigma2),sqrt(sigmaM),self.chi2partiel,self.chi2total
            return chi2

        def fillDerivatives(funga):

            def deriv(funga,gamma,i,epsilon):
                g = deepcopy(gamma)
                g[i] += epsilon
                chip = funga(g)
                g[i] -= 2.*epsilon
                chim = funga(g)
                g[i] += epsilon
                return (chip-chim)/(2.*epsilon)
    
            def deriv2(funga,gamma,i,j,epsilon, mu):
                g = deepcopy(gamma)
                g[i] += epsilon
                derp = deriv(funga,g,j,mu)
                g[i] -= 2.*epsilon
                derm = deriv(funga,g,j,mu)
                g[i] += epsilon
                return (derp-derm)/(2.*epsilon)

            rows = []
            deri = []
            for i in range(len(energies)):
                column = []
                for j in range(len(energies)):
                    column.append(deriv2(funga,energies,i,j,0.001,0.001))
                rows.append(column)
                deri.append(deriv(funga,energies,i,0.001))                
            return array(rows), array(deri)

        from numpy import array, linalg, dot, add
        from copy import deepcopy

        #print chi2(energies)
        Delta = 1E9
        t = 0
        while Delta > 1E-3 and t < 200 :
            #print "iteration ",t
            t += 1
                
            d2,d = fillDerivatives(chi2)
            delta = linalg.solve(d2,d)
            Delta = abs(delta[0]) + abs(delta[1])
            #print '------------------- '
            #print 'Delta = ',Delta
            Ki2 = chi2(energies)

            factor = 1.
            for i in range(len(energies)):
                #print i, energies[i], delta[i], d[i]
                if abs(delta[i]) > energies[i]/10. :
                    factor = min(factor,energies[i]/10./abs(delta[i]))
            delta = map(lambda x:x*factor,delta)

            def chinew(funga,gamma,delta):
                gnew = deepcopy(gamma)
                for i,g in enumerate(gamma):
                    gnew[i] -= delta[i]
                return funga(gnew) - Ki2
        
            while chinew(chi2,energies,delta) > 1E-5 :
                delta = map(lambda x:-x*0.6, delta)

            #print ' ' 
            for i in range(len(energies)):
                energies[i] -= delta[i]
        if t >= 199:
            print 'Warning - reached iteration ',t
            print diele,dimu,chi2(energies)
            for i in range(len(energies)):
                print i, energies[i], delta[i], d[i]
        #print t, chi2(energies)

        l1 *= energies[0] / l1.E()
        l2 *= energies[1] / l2.E()
        #if not(dimu):
        #    m12 = (l1+l2).M()
        #    l1 *= sqrt(91.188/m12)
        #    l2 *= sqrt(91.188/m12)

        #print self.jets[0]
        p41 = self.jets[0].p4()
        p41.SetPxPyPzE(l1.X(),l1.Y(),l1.Z(),l1.T())
        self.jets[0].setP4(p41)
        #print self.jets[1]
        p42 = self.jets[1].p4()
        p42.SetPxPyPzE(l2.X(),l2.Y(),l2.Z(),l2.T())
        self.jets[1].setP4(p42)
        return chi2(energies)

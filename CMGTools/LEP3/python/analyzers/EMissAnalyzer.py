import operator 
import itertools
import copy

from ROOT import TLorentzVector, TVector3

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenParticle, Lepton, Photon

from CMGTools.LEP3.analyzers.DiObject import DiObject
from CMGTools.LEP3.analyzers.EMiss import EMiss, EVis

from CMGTools.RootTools.utils.DeltaR import deltaR
from math import pi, sqrt, acos, asin, log
        
class EMissAnalyzer( Analyzer ):

    def declareHandles(self):
        super(EMissAnalyzer, self).declareHandles()

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



        
    def beginLoop(self):
        super(EMissAnalyzer,self).beginLoop()
        self.counters.addCounter('EMiss')
        self.counters.addCounter('EMissGen')
        self.counters.addCounter('WWHGen')
        counts = []
        counts.append(self.counters.counter('EMiss'))
        counts.append(self.counters.counter('EMissGen'))
        counts.append(self.counters.counter('WWHGen'))
        for count in counts : 
            count.register('All events')
            count.register('ptMiss > 10.')
            count.register('10 < mVis < 180.')
            count.register('ctMiss > 1.00')
            count.register('mMiss > 0.')
            count.register('Two Good Jets')
            count.register('passing')
        
    def buildMiss(self, event):

        self.nunubb = False
        self.wwh = False
        self.neutrinos = []
        self.bquarks = []
        for ptc in self.mchandles['genParticles'].product():
            isNeutrino = abs(ptc.pdgId()) == 12 or abs(ptc.pdgId()) == 14 or abs(ptc.pdgId()) == 16
            isbquark = abs(ptc.pdgId()) == 5
            if isNeutrino : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.neutrinos.append(GenParticle(ptc))
            elif isbquark : 
                if not(ptc.numberOfMothers()) : continue
                if ptc.mother(0).pdgId() != 25 : continue
                self.bquarks.append(GenParticle(ptc))

        if len(self.neutrinos) == 2 and len(self.bquarks) == 2 :
            self.nunubb = True
            self.wwh = not(self.neutrinos[0].numberOfMothers())

##            for ptc in self.mchandles['genParticles'].product():
##                print GenParticle(ptc)

        self.eMiss = EMiss(self.jets)
        self.eVis = EVis(self.jets)
        #if self.nunubb : print self.eMiss

        event.ptMiss = self.eMiss.Pt()
        event.pMiss = self.eMiss.P()
        event.eMiss = self.eMiss.E()
        event.ctMiss = 1.
        if self.eMiss.P() > 0. : 
            event.ctMiss = self.eMiss.Pz()/self.eMiss.P()
        event.mMiss = self.eMiss.M()
        event.ptVis = self.eVis.Pt()
        event.pVis = self.eVis.P()
        event.eVis = self.eVis.E()
        event.ctVis = -1.
        if self.eVis.P() > 0. : 
            event.ctVis = self.eVis.Pz()/self.eVis.P()
        event.mVis = self.eVis.M()
        event.nunubb = self.nunubb
        event.wwh = self.wwh



    def buildLeptonList(self,event):

        self.eleele = False
        self.mumu = False
        self.tautau = False
        self.electrons = []
        self.muons = []
        self.taus = []

        for ptc in self.mchandles['genParticles'].product():
            isElectron = abs(ptc.pdgId()) == 11
            isMuon = abs(ptc.pdgId()) == 13
            isTau = abs(ptc.pdgId()) == 15
            if isElectron : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.electrons.append(GenParticle(ptc))
            if isMuon : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.muons.append(GenParticle(ptc))
            if isTau : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.taus.append(GenParticle(ptc))

        if len(self.electrons) == 2 :
            self.eleele = True
        if len(self.muons) == 2 :
            self.mumu = True
        if len(self.taus) == 2 :
            self.tautau = True

        event.eleele = self.eleele
        event.mumu = self.mumu
        event.tautau = self.tautau


        self.jets = []
        for ptj in self.handles['jets'].product():
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
##                 elif self.eleele or self.mumu :
##                     print 'Watch out : Jet far from lepton '
##                     print ' drmin = ',drmin
##                     print ' Lepton : ',lept, lept.mass()
##                     print ' Jet    : ',self.jets[ijet], self.jets[ijet].mass()

    def buildJetList(self, event):

        
        self.jet3 = []

        if len(self.jets) > 2 : 
            self.jets.sort(key=lambda a: a.energy(), reverse = True)
            tmpJets = set(self.jets)
            tmpJet3 = set(self.jets)
#            print 'Jets Avant : '
#            for jet in tmpJets:
#                print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
            while len(tmpJets) != 2:
                # Keep the step with three jets
                if len(tmpJet3) == 3 :
                    for jet in tmpJet3 :
                        self.jet3.append(jet)
                # 
                dijets = self.findPairs(tmpJets)
                dijets.sort(key=lambda a: a.M())
#                print dijets[0],dijets[0].M()
                
                tmpJets.remove(dijets[0].leg1)
                tmpJets.remove(dijets[0].leg2)
                tmpJets.add(dijets[0])

 #           print 'Jets apres : '
            self.jets = []
            for jet in tmpJets:
 #               print jet,jet.nConstituents(), jet.mass(), jet.btag(7)
 #               print jet,jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
                self.jets.append(jet)
                
        self.jets.sort(key=lambda a: a.btag(7), reverse = True)


    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.buildLeptonList ( event )

        self.buildMiss( event )

        event.allJets = self.jets
        event.jetPairs = self.findPairs( event.allJets )

        #for ptc in self.mchandles['genParticles'].product():
        #    print GenParticle(ptc)
        #for jet in self.jets :
        #    print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(),jet.btag(7),jet.btag(0)

        self.counters.counter('EMiss').inc('All events')
        if self.nunubb : self.counters.counter('EMissGen').inc('All events')
        if self.wwh : self.counters.counter('WWHGen').inc('All events')
        event.step = 0

        if self.eMiss.Pt() < self.cfg_ana.ptmiss :
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('ptMiss > 10.')
        if self.nunubb : self.counters.counter('EMissGen').inc('ptMiss > 10.')
        if self.wwh : self.counters.counter('WWHGen').inc('ptMiss > 10.')
            
        if self.eVis.M() > self.cfg_ana.mvis[1] or self.eVis.M() < self.cfg_ana.mvis[0]:
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('10 < mVis < 180.')
        if self.nunubb : self.counters.counter('EMissGen').inc('10 < mVis < 180.')
        if self.wwh : self.counters.counter('WWHGen').inc('10 < mVis < 180.')
            
        if self.eMiss.P() == 0. or abs(self.eMiss.Pz()/self.eMiss.P()) > self.cfg_ana.ctmiss  :
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('ctMiss > 1.00')
        if self.nunubb : self.counters.counter('EMissGen').inc('ctMiss > 1.00')
        if self.wwh : self.counters.counter('WWHGen').inc('ctMiss > 1.00')
            
        if self.eMiss.M() < self.cfg_ana.mmiss :
            return 0
        else:
            event.step +=1

        self.counters.counter('EMiss').inc('mMiss > 0.')
        if self.nunubb : self.counters.counter('EMissGen').inc('mMiss > 0.')
        if self.wwh : self.counters.counter('WWHGen').inc('mMiss > 0.')

        delta = self.eVis.E()*self.eVis.E() * 91.2 * 91.2 + self.eVis.p()*self.eVis.p()*(240.*240.-91.2*91.2)

        if self.eVis.M()> 0 :
            event.alpha = (self.eVis.E()*240. - sqrt(delta)) / (self.eVis.M()*self.eVis.M())
        else:
            print 'Event ', eventNumber, self.eVis.M()
         

        # Force the event into two jets.
        self.buildJetList( event )        

        if len(self.jets) != 2 : 
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('Two Good Jets')
        if self.nunubb : self.counters.counter('EMissGen').inc('Two Good Jets')
        if self.wwh : self.counters.counter('WWHGen').inc('Two Good Jets')

        acol = self.jets[0].px() * self.jets[1].px() + \
               self.jets[0].py() * self.jets[1].py() + \
               self.jets[0].pz() * self.jets[1].pz()
        acol /= self.jets[0].p() * self.jets[1].p()
        if acol >= 1.0 : acol = 1. - 1E-12
        if acol <= -1.0 : acol = -1. + 1E-12
        acol = acos(acol)*180./pi

        acop = self.jets[0].px() * self.jets[1].px() + \
               self.jets[0].py() * self.jets[1].py() 
        acop /= self.jets[0].pt() * self.jets[1].pt()
        if acop >= 1.0 : acop = 1. - 1E-12
        if acop <= -1.0 : acop = -1. + 1E-12
        acop = acos(acop)*180./pi

        vect1 = TVector3(self.jets[0].px(), self.jets[0].py(), self.jets[0].pz())
        vect2 = TVector3(self.jets[1].px(), self.jets[1].py(), self.jets[1].pz())
        cross = vect1.Unit().Cross(vect2.Unit())
        cross = abs(cross.z())
        cross = asin(cross) * 180./pi

        sumtet = 0.
        if len(self.jet3) == 3 :
            jp = self.findPairs( self.jet3 )
            for j in jp :
                angle = j.leg1.px() * j.leg2.px() + \
                        j.leg1.py() * j.leg2.py() + \
                        j.leg1.pz() * j.leg2.pz() 
                angle /= j.leg1.p() * j.leg2.p()
                angle = acos(angle)*180./pi
                sumtet += angle

        event.acol = acol
        event.acop = acop
        event.sumtet = sumtet
        event.cross = cross

        if self.cfg_ana.hinvis: 
            event.chi2mZ = self.fitmZ()
            event.chi2partiel = self.chi2partiel
            eVisFit = EVis(self.jets)
            eMissFit = EMiss(self.jets)
            event.mVisFit = eVisFit.M()
            event.mMissFit = eMissFit.M()
        else:
            event.chi2mZ = 0.
            event.mVisFit = alpha*mVis
            event.mMissFit = 91.2

        self.counters.counter('EMiss').inc('passing')
        if self.nunubb : self.counters.counter('EMissGen').inc('passing')
        if self.wwh : self.counters.counter('WWHGen').inc('passing')
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

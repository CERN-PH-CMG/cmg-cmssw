import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenParticle

from CMGTools.LEP3.analyzers.DiObject import DiObject
from CMGTools.LEP3.analyzers.kinFit4 import kinFit4
from CMGTools.LEP3.analyzers.kinFit5 import kinFit5
from CMGTools.LEP3.analyzers.beta4 import beta4
from CMGTools.LEP3.analyzers.findVV import findWW, findZZ
from CMGTools.LEP3.analyzers.findKinVV import findKinWW, findKinZZ

from CMGTools.RootTools.utils.DeltaR import deltaR
from math import pi, sqrt, acos
        
class FourJetAnalyzer( Analyzer ):

    def declareHandles(self):
        super(FourJetAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle ('cmgPFJetSel',
                                           'std::vector<cmg::PFJet>')

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

        #self.handles['pf'] = AutoHandle ('particleFlow',
        #                                 'std::vector<reco::PFCandidate>')


        
    def beginLoop(self):
        super(FourJetAnalyzer,self).beginLoop()
        self.counters.addCounter('FourJets')
        self.counters.addCounter('FourGenJets')
        count1 = self.counters.counter('FourJets')
        count1.register('All events')
        count1.register('Four good jets')
        count1.register('Good jet pairs')
        count1.register('Good jet triplets')
        count1.register('H -> bb tagged')
        count1.register('passing')
        count2 = self.counters.counter('FourGenJets')
        count2.register('All events')
        count2.register('Four good jets')
        count2.register('Good jet pairs')
        count2.register('Good jet triplets')
        count2.register('H -> bb tagged')
        count2.register('passing')

        
        
    def buildJetList(self, event):

        self.quarks = []
        self.nq = 0
        for ptc in self.mchandles['genParticles'].product():
            #print GenParticle(ptc)
            # Find four jet events
            if abs(ptc.pdgId()) > 6 and ptc.pdgId() != 21 : continue
            #print 'a quark : ',ptc.pdgId()
            if not(ptc.numberOfMothers()) : continue
            moth = ptc.mother(0)
            #print 'Mother : ',moth.pdgId()
            if moth.numberOfMothers() and moth.mother(0).pdgId() == 25 : continue
            if abs(moth.pdgId()) != 23 and \
               abs(moth.pdgId()) != 25 : continue
            self.nq += 1  # counters of quarks with H or Z as a mothers 
            self.quarks.append(GenParticle(ptc)) # list of GenParticle objects for the quarks that have H or Z as a mothers 

        #if self.nq == 4 : print 'A Four Jet Event !'

        self.jets = []
        for ptj in self.handles['jets'].product():
            self.jets.append(Jet(ptj)) # list of Jets object for all the jets in input
            #print jet, jet.nConstituents(), jet.component(1).number(), Jet(ptj).component(1).fraction()

        

        if len(self.jets) > 4 : 
            self.jets.sort(key=lambda a: a.energy(), reverse = True)
            tmpJets = set(self.jets)
            #print 'Jets Avant : '
            #for jet in tmpJets:
            #    print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
            while len(tmpJets) != 4: 
                dijets = self.findPairs(tmpJets) 
                dijets.sort(key=lambda a: a.M())
                #print dijets[0],dijets[0].M()
                
                tmpJets.remove(dijets[0].leg1) # it removes from the list of all Jets the components of the diJet object with the lowest mass 
                tmpJets.remove(dijets[0].leg2) # ""
                tmpJets.add(dijets[0]) # it add the diJet object with lowest mass to the list of Jets 

            #print 'Jets apres : '
            self.jets = []
            for jet in tmpJets:
                #print jet,jet.nConstituents(), jet.mass(), jet.btag(7)
                #print jet,jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
                self.jets.append(jet)
                
        self.jets.sort(key=lambda a: a.btag(7), reverse = True) # list of Jet objects 
            


    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.buildJetList( event )   

        #for ptc in self.mchandles['genParticles'].product():
        #    print GenParticle(ptc)
        #for jet in self.jets :
        #    print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(),jet.btag(7),jet.btag(0)

        self.counters.counter('FourJets').inc('All events') #NB: we don't have the Four RECO jet request at this moment
        if self.nq == 4 : self.counters.counter('FourGenJets').inc('All events')
        event.step = 0

        # Request four good hadronic jets and nothing else
        if not(self.testFourJets(self.jets)) : 
            return 0 # here I'm rejecting events that do not pass the Jets preselection [ + I'm changing the jet.p4() and providing the chi2 of the 4j fit/scale hypothesis ]
        else:
            event.step +=1
        self.counters.counter('FourJets').inc('Four good jets')
        if self.nq == 4 : self.counters.counter('FourGenJets').inc('Four good jets')
            

        # variables for each FourJet object
        event.mvis = self.mvis
        event.pxvis = self.px
        event.pyvis = self.py
        event.pzvis = self.pz 
        event.evis = self.evis 
        event.chi2 = self.chi2
        event.chi2fit = self.chi2fit 
        event.mmin = self.mmin
        #print 'mvis ',event.mvis

        # Test on jet pairs
        #event.trueJets = set(self.quarks)
        #event.truePairs = self.findPairs( event.trueJets )
        event.allJets = self.jets # jets - after marging - 
        event.jetPairs = self.findPairs( event.allJets ) # all possible pairs - of the merged jet collection -

        if not(self.testJetPairs(event.jetPairs)) :
            return 0
        else:
            event.step +=1
        self.counters.counter('FourJets').inc('Good jet pairs') # no selection in the current analysis
        if self.nq == 4 : self.counters.counter('FourGenJets').inc('Good jet pairs') # no selection in the current analysis

        event.sumtet = self.sumtet
        event.cijklmin = self.cijklmin
        event.m2min = self.m2min 
        

##         print 'Dijet combinations (expt)'
##         for dijet in event.jetPairs:
##             print dijet, dijet.mass()

##         if self.nq == 4:
##             print 'Dijet combinations (true)'
##             for dijet in event.truePairs:
##                 print dijet, dijet.mass()

        event.jetTriplets = self.findTriplets( event.allJets )

        if not(self.testJetTriplets(event.jetTriplets)) :
            return 0
        else:
            event.step +=1
        self.counters.counter('FourJets').inc('Good jet triplets')
        if self.nq == 4 : self.counters.counter('FourGenJets').inc('Good jet triplets')

        event.m3min = self.m3min


        event.ww, event.wwMin = findWW ( event.jetPairs )
        event.zz, event.zzMin = findZZ ( event.jetPairs )
        #event.wwKin, event.wwMinKin = findKinWW ( event.jetPairs )
        #event.zzKin, event.zzMinKin = findKinZZ ( event.jetPairs )
        event.hz, event.deltaZ = self.findHZ ( event.jetPairs )

        if len(event.hz) == 0 :
            return 0

        hj1 = event.hz[0].leg1
        hj2 = event.hz[0].leg2
        zj1 = event.hz[1].leg1
        zj2 = event.hz[1].leg2

        self.selJets  = []
        self.selJets.append(hj1)
        self.selJets.append(hj2)
        self.selJets.append(zj1)
        self.selJets.append(zj2)

        if hj1.btag(7) + hj2.btag(7) < 0.95 and hj1.btag(7) + hj2.btag(7) + zj1.btag(7) + zj2.btag(7) < 2.1 : # this is different from what in the note!
            return 0
        else:
##             for ptc in self.mchandles['genParticles'].product():
##                 print GenParticle(ptc)
##             for jet in self.selJets :
##                 print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(),jet.btag(7),jet.btag(0)
            event.step +=1
        self.counters.counter('FourJets').inc('H -> bb tagged')
        if self.nq == 4 : self.counters.counter('FourGenJets').inc('H -> bb tagged')
        
        
##         print 'h mass = ',event.hz[0].M()
##         print 'z mass = ',event.hz[1].M()
##         print 'h mass = ',event.hz[0].M()+event.hz[1].M()-91.2

        self.counters.counter('FourJets').inc('passing')
        if self.nq == 4 :
            self.counters.counter('FourGenJets').inc('passing')
##         else: 
##             print 'mvis = ',mvis
##             print 'mtot = ',mtot
        event.step += 1

        return True
 



    def testFourJets(self, jets) :
    
        njobj = 0
        njtrk = 0
        massmax = -999.
        massmin = 999.
        njeta = 0
        mtot = 0.
        px = 0.
        py = 0.
        pz = 0.
        en = 0.

        if len(jets) != 4 :
            #print 'NJets = ', len(jets)
            return False
        
        for jet in jets:
            if jet.nConstituents() >= self.cfg_ana.npfj[1] : njobj += 1
            if jet.component(1).number() >= self.cfg_ana.ntkj[1] : njtrk += 1
            if jet.mass() > massmax : massmax = jet.mass()
            if jet.mass() < massmin : massmin = jet.mass()
            if abs(jet.eta()) < 3.0 : njeta += 1
            mtot += jet.mass()
            px += jet.px()
            py += jet.py()
            pz += jet.pz()
            en += jet.energy()
        mvis = sqrt(en*en-px*px-py*py-pz*pz)

        # 4 jets
        if njeta < 4 :
            #print 'NjetsEta = ',njeta
            return False

        # At least npfj jets with at least npf particles
        if njobj < self.cfg_ana.npfj[0] :
            #print 'njobj = ',njobj
            return False

        # At least ntkj jets with at least ntk tracks
        if njtrk < self.cfg_ana.ntkj[0] :
            #print 'njtrk = ',njtrk
            return False
        
        # Smallest jet mass larger than tau mass (say)
        if massmin < self.cfg_ana.minM :
            #print 'massmin = ',massmin
            return False
        
        # No missing energy
        if mvis < self.cfg_ana.mVis :
            #print 'mvis = ',mvis
            return False

        # Rescale jet energies
        # Check the chi2 ( < 1000. == request all energies to be positive)
        chi2 = beta4(self.jets, 120.)
        if chi2 > self.cfg_ana.chi2 :
            #print 'chi2 = ',chi2
            return False
        
        # To use the kinFitter instead of the beta4 function to rescale the energy/momentum of the jets uncomment this line and
        # comment jet.setP4(p4) in the beta4 function. In this way the 2 chi2 are anyhow computed and can be stored in the event
        #chi2fit = kinFit4(self.jets, 120.)
        chi2fit = kinFit5(self.jets, 120., 91.2)

        self.mvis = mvis
        self.px = px
        self.py = py
        self.pz = pz
        self.evis = en
        self.chi2 = chi2
        self.chi2fit = chi2fit
        self.mmin = massmin

        return True

    
    def findPairs(self, jets):
        out = []

        for j1, j2 in itertools.combinations(jets, 2):
            out.append( DiObject(j1, j2) )

        return out
    
    def testJetPairs(self, jetPairs):

        mMin = 999.
        cijklMin = 999.
        jetjetAngles = []
        
        for i, pair1 in enumerate(jetPairs):
            
            # angle between the two jets
            c1 = ( pair1.leg1.px()*pair1.leg2.px() + \
                   pair1.leg1.py()*pair1.leg2.py() + \
                   pair1.leg1.pz()*pair1.leg2.pz() ) / ( pair1.leg1.p() * pair1.leg2.p() )
            jetjetAngles.append(acos(c1))
            # Find the pair with lowest mass
            nLowMass = 0
            if pair1.M() < mMin :
                mMin = pair1.M()

            # The complementary pair
            pair2 = jetPairs[5-i]
            # angle between the two jets
            c2 =  ( pair2.leg1.px()*pair2.leg2.px() + \
                    pair2.leg1.py()*pair2.leg2.py() + \
                    pair2.leg1.pz()*pair2.leg2.pz() ) / ( pair2.leg1.p() * pair2.leg2.p() )

            # sum of the two smallest cosines (i.e., the two largest angles)
            cijkl = c1 + c2
            if cijkl < cijklMin :
                cijklMin = cijkl


        # Sum of the four smallest jet-jet angles
        jetjetAngles.sort()
        sumtet = 0.
        for i in range(4):
            sumtet += jetjetAngles[i]*180./pi
            
        if mMin < self.cfg_ana.pair_mass :       
            return False

        if cijklMin > self.cfg_ana.pair_cijkl :  
            return False

        if sumtet < self.cfg_ana.pair_sumtet :   
            return False

        self.sumtet = sumtet
        self.cijklmin = cijklMin
        self.m2min = mMin
        
        return True
            
    def findTriplets(self, jets):
        out = []

        for j1, j2, j3 in itertools.combinations(jets, 3):
            out.append( DiObject(j3,DiObject(j1, j2)) )

        return out
    
    def testJetTriplets(self, jetTriplets):
        m3Min = 999.
        for triplet in jetTriplets :
            if triplet.M() < m3Min :
                m3Min = triplet.M()

        self.m3min = m3Min

        return True
                          
    def findHZ(self, jetPairs):

        hz = []

        hPair = []
        zPair = []
        deltaZ = 999.
        btag = -999.
        for i,pair1 in enumerate(jetPairs):
            pair2 = jetPairs[5-i]
            if pair1.M() > self.cfg_ana.h_mass and \
                   pair2.M() > self.cfg_ana.z_mass[0] and \
                   pair2.M() < self.cfg_ana.z_mass[1] :
                
                if pair1.leg1.btag(7) + pair1.leg2.btag(7) > btag :
                    btag  = pair1.leg1.btag(7) + pair1.leg2.btag(7)
                    hPair = [pair1]
                    zPair = [pair2]

##        if deltaZ > self.cfg_ana.z_mass :
##            return hz, deltaZ
##
        if len(hPair) : 
            hz = [hPair[0],zPair[0]]

        return hz, btag


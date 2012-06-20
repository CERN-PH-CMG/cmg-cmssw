import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenParticle,Electron,Muon

from CMGTools.LEP3.analyzers.DiObject import DiObject

from CMGTools.RootTools.utils.DeltaR import deltaR
from math import pi, sqrt, acos
        
class httanalyzer( Analyzer ):

    def declareHandles(self):
        super(httanalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle ('cmgPFJetSel',
                                           'std::vector<cmg::PFJet>')

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

        self.handles['muons'] = AutoHandle( 'cmgMuonSel',
                                                     'std::vector<cmg::Muon>' )
        self.handles['electrons'] = AutoHandle( 'cmgElectronSel',
                                                     'std::vector<cmg::Electron>' )

        
        #self.handles['pf'] = AutoHandle ('particleFlow',
        #                                 'std::vector<reco::PFCandidate>')


        
    def beginLoop(self):
        super(httanalyzer,self).beginLoop()
        self.counters.addCounter('h_gen')
        count = self.counters.counter('h_gen')
        count.register('All events')
        count.register('All hz events')
        count.register('h->tt')
        
    def buildMCinfo(self, event):
        self.isHZ = False
        self.isHZqq = False
        self.tau = []
        nH = 0
        nZ = 0
        minzene=99999.
        for ptc in self.mchandles['genParticles'].product():
            if abs(ptc.pdgId())==23:
                nZ+=1
                if ptc.energy()<minzene:
                    minzene=ptc.energy()
                    self.Z=GenParticle(ptc)
                    
            if abs(ptc.pdgId())==25:
                nH+=1
                self.H=GenParticle(ptc)
                
            
            # Find taus events
            if abs(ptc.pdgId()) != 15: continue
            if not(ptc.numberOfMothers()) : continue
            moth = ptc.mother(0)
#            print 'Mother : ',moth.pdgId()
#            if moth.numberOfMothers():
#                print 'Grandmother:',moth.mother(0).pdgId() 
            if abs(moth.pdgId()) == 25 :
                self.tau.append(GenParticle(ptc))

        if nZ>=1 and nH>=1:
            self.isHZ=True
            # check if Z-->qq
            for ptc in self.mchandles['genParticles'].product():
                if ptc.mother(0)==self.Z:
                    if abs(ptc.pdgId())<=6:
                        self.isHZqq=True

#        if not self.isHZ:
#            print "No HZ found in this event.... why?"
#            for ptc in self.mchandles['genParticles'].product():
#                print ptc.energy(),GenParticle(ptc)
 
            
#        #if self.nq == z4 : print 'A Four Jet Event !'
#
#        self.jets = []
#        for ptj in self.handles['jets'].product():
#            self.jets.append(Jet(ptj))
#            #print jet, jet.nConstituents(), jet.component(1).number(), Jet(ptj).component(1).fraction()
#
#        
#
#        if len(self.jets) > 4 : 
#            self.jets.sort(key=lambda a: a.energy(), reverse = True)
#            tmpJets = set(self.jets)
#            #print 'Jets Avant : '
#            #for jet in tmpJets:
#            #    print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
#            while len(tmpJets) != 4: 
#                dijets = self.findPairs(tmpJets)
#                dijets.sort(key=lambda a: a.M())
#                #print dijets[0],dijets[0].M()
#                
#                tmpJets.remove(dijets[0].leg1)
#                tmpJets.remove(dijets[0].leg2)
#                tmpJets.add(dijets[0])
#
#            #print 'Jets apres : '
#            self.jets = []
#            for jet in tmpJets:
#                #print jet,jet.nConstituents(), jet.mass(), jet.btag(7)
#                #print jet,jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
#                self.jets.append(jet)
#                
#        self.jets.sort(key=lambda a: a.btag(7), reverse = True)
#            


    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.buildMCinfo( event )   

        self.counters.counter('h_gen').inc('All events')
        if self.isHZ: self.counters.counter('h_gen').inc('All hz events')
        if len(self.tau)==2: self.counters.counter('h_gen').inc('h->tt')

        event.isHZ = 0

        if self.isHZ:
            event.isHZ=1
            event.H=self.H
            event.Z=self.Z
            
        event.ishtt = 0
        if len(self.tau)==2:
            event.ishtt =1
            event.tau1=self.tau[0]
            event.tau2=self.tau[1]


        # prepare jets ordered in energy
        self.jets=[]
        for ptj in self.handles['jets'].product():
            self.jets.append(Jet(ptj))
            
        # order them in energy
        self.jets.sort(key=lambda a: a.energy(), reverse = True)
        event.njets=len(self.jets)
        
        # loop over jets and define  "isolation"
        self.jetiso=[]
        for ind1 in range(0,len(self.jets)):
            cosmax=-2
            for ind2 in range(ind1+1,len(self.jets)):
                cos=self.cosdelta(self.jets[ind1],self.jets[ind2])
                if cos> cosmax:
                    cosmax=cos
            self.jetiso.append(cosmax)


        # prepare electrons ordered in energy
        self.electrons=[]
        for ptj in self.handles['electrons'].product():
            self.electrons.append(Electron(ptj))
            
        # order them in energy
        self.electrons.sort(key=lambda a: a.energy(), reverse = True)
        
        # loop over jets and define  "isolation"
        self.eleiso=[]
        for ind1 in range(0,len(self.electrons)):
            cosmax=-2
            for ind2 in range(ind1+1,len(self.jets)):
                cos=self.cosdelta(self.electrons[ind1],self.jets[ind2])
                if cos> cosmax:
                    cosmax=cos
            self.eleiso.append(cosmax)

        # prepare muons ordered in energy
        self.muons=[]
        for ptj in self.handles['muons'].product():
            self.muons.append(Electron(ptj))
            
        # order them in energy
        self.muons.sort(key=lambda a: a.energy(), reverse = True)
        
        # loop over jets and define  "isolation"
        self.muoiso=[]
        for ind1 in range(0,len(self.muons)):
            cosmax=-2
            for ind2 in range(ind1+1,len(self.jets)):
                cos=self.cosdelta(self.muons[ind1],self.jets[ind2])
                if cos> cosmax:
                    cosmax=cos
            self.muoiso.append(cosmax)

            
        

        # look now at the tau jets
        self.taugenjet=[]
        self.taucos=[]
        self.tauiso=[]

        self.nontaugenjet=self.jets
        self.nontaugenjetiso=self.jetiso
        
        if event.ishtt==1:
#            print "probing event",eventNumber
            # loop over jets and find closest gen tau
            if event.njets>=2:
                for thistau in self.tau:
                    cosmax=-2
                    tauindex=-10
                    for ind in range(0,len(self.jets)):
                        thisjet=self.jets[ind]
                        thiscos=self.cosdelta(thistau,thisjet)
                        if thiscos>cosmax:
                            cosmax=thiscos
                            taucand=thisjet
                            iso=self.jetiso[ind]
                            tauindex=ind
                            
                    if cosmax!=-2:        
                        self.taugenjet.append(taucand)
                        self.tauiso.append(iso)
                        self.nontaugenjet.pop(tauindex)
                        self.nontaugenjetiso.pop(tauindex)
                    else:
                        self.taugenjet.append(thistau) # just to fill it up
                    self.taucos.append(cosmax)
    
                        
    
                event.tau1genjet = self.taugenjet[0]
                event.tau1cosjet = self.taucos[0]
                event.tau1iso = self.tauiso[0]
                event.tau2genjet = self.taugenjet[1]
                event.tau2cosjet = self.taucos[1]
                event.tau2iso = self.tauiso[1]

        self.nontaugenjet.sort(key=lambda a: a.energy(), reverse = True)
        event.nontaugenjet = self.nontaugenjet
        event.nontaugenjetiso = self.nontaugenjetiso
        
        
        
        # start here the real selection
        event.step=0
        # first look for at least four jets and two of them isolated and low #tracks
        if event.njets<4:
            return
        event.step+=1
#        print "njets=",event.njets,len(self.jets)
        # requires at least 2 taucandidates as isolated jets (0.5) and with 1,2 or 3 tracks
        event.taucand=[]
        event.taucandiso=[]
        event.nontaucand=[]
        event.nontaucandiso=[]
        for ind in range(0,len(self.jets)):
#            print "evaluating ",ind
            ntrk=self.jets[ind].component(1).number()
            # the two most energetic jets are not considered among taus 
#            if ind>=2 and ntrk>0 and ntrk<=3  and self.jetiso[ind]<0.5 and len(event.taucand)<2:
            if ntrk>0 and ntrk<=3  and self.jetiso[ind]<0.5 and len(event.taucand)<2:
                event.taucand.append(self.jets[ind])
                event.taucandiso.append(self.jetiso[ind])
#                print "adding to tau"
            else:
                event.nontaucand.append(self.jets[ind])
                event.nontaucandiso.append(self.jetiso[ind])
                #                print "adding to non tau"
                
        if event.ishtt==1:
            print "taucands",len(event.taucand)

        if len(event.taucand)<2:
            return
        event.step+=1

        # now build Z(qq) and visible H(tautau) candidates 
        if event.ishtt==1:
            print "found ",len(event.taucand)," tau candidates"
            print event.taucand
            print "found ",len(event.nontaucand)," non tau candidates"
            print event.nontaucand
            
#        first=True
#        for comp in event.nontaucand:
#            if first:
#                Zcand=comp
#                first=False
#            else:
#                Zcand=DiObject(Zcand,comp)
#
#        first=True
#        for comp in event.taucand:
#            if first:
#                Hcand=comp
#                first=False
#            else:
#                Hcand=DiObject(Hcand,comp)
#
#        if event.ishtt==1:
#            print "Zcand=",Zcand.enery()
#            print "Hcand=",Hcand.energy()



#        event.mvis = self.mvis
#        event.pxvis = self.px
#        event.pyvis = self.py
#        event.pzvis = self.pz 
#        event.evis = self.evis 
#        event.chi2 = self.chi2 
#        event.mmin = self.mmin
#        #print 'mvis ',event.mvis
#
#        # Test on jet pairs
#        #event.trueJets = set(self.quarks)
#        #event.truePairs = self.findPairs( event.trueJets )
#        event.allJets = self.jets
#        event.jetPairs = self.findPairs( event.allJets )
#
#        if not(self.testJetPairs(event.jetPairs)) :
#            return 0
#        else:
#            event.step +=1
#        self.counters.counter('FourJets').inc('Good jet pairs')
#        if self.nq == 4 : self.counters.counter('FourGenJets').inc('Good jet pairs')
#
#        event.sumtet = self.sumtet
#        event.cijklmin = self.cijklmin
#        event.m2min = self.m2min 
#        
#
###         print 'Dijet combinations (expt)'
###         for dijet in event.jetPairs:
###             print dijet, dijet.mass()
#
###         if self.nq == 4:
###             print 'Dijet combinations (true)'
###             for dijet in event.truePairs:
###                 print dijet, dijet.mass()
#
#        event.jetTriplets = self.findTriplets( event.allJets )
#
#        if not(self.testJetTriplets(event.jetTriplets)) :
#            return 0
#        else:
#            event.step +=1
#        self.counters.counter('FourJets').inc('Good jet triplets')
#        if self.nq == 4 : self.counters.counter('FourGenJets').inc('Good jet triplets')
#
#        event.m3min = self.m3min
#
#
#        event.ww, event.wwMin = self.findWW ( event.jetPairs )
#        event.zz, event.zzMin = self.findZZ ( event.jetPairs )
#        event.hz, event.deltaZ = self.findHZ ( event.jetPairs )
#
#        if len(event.hz) == 0 :
#            return 0
#
#        hj1 = event.hz[0].leg1
#        hj2 = event.hz[0].leg2
#        zj1 = event.hz[1].leg1
#        zj2 = event.hz[1].leg2
#
#        self.selJets  = []
#        self.selJets.append(hj1)
#        self.selJets.append(hj2)
#        self.selJets.append(zj1)
#        self.selJets.append(zj2)
#
#        if hj1.btag(7) + hj2.btag(7) < 0.95 and hj1.btag(7) + hj2.btag(7) + zj1.btag(7) + zj2.btag(7) < 2.1 :
#            return 0
#        else:
###             for ptc in self.mchandles['genParticles'].product():
###                 print GenParticle(ptc)
###             for jet in self.selJets :
###                 print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(),jet.btag(7),jet.btag(0)
#            event.step +=1
#        self.counters.counter('FourJets').inc('H -> bb tagged')
#        if self.nq == 4 : self.counters.counter('FourGenJets').inc('H -> bb tagged')
#        
#        
###         print 'h mass = ',event.hz[0].M()
###         print 'z mass = ',event.hz[1].M()
###         print 'h mass = ',event.hz[0].M()+event.hz[1].M()-91.2
#
#        self.counters.counter('FourJets').inc('passing')
#        if self.nq == 4 :
#            self.counters.counter('FourGenJets').inc('passing')
###         else: 
###             print 'mvis = ',mvis
###             print 'mtot = ',mtot
#        event.step += 1

        return True
 

    def cosdelta(self,part1,part2):
        part1_p=part1.p()
        part2_p=part2.p()
        cos=(part1.px()*part2.px()+part1.py()*part2.py()+part1.pz()*part2.pz())/(part1_p*part2_p)
        return cos
    


    def beta4(self,jets,ebeam):

        from numpy import array, linalg, dot, add
        
        
        rows = []
        for i in range(4):
            rows.append([])
        for jet in jets:
            rows[0].append(jet.px()/jet.energy())
            rows[1].append(jet.py()/jet.energy())
            rows[2].append(jet.pz()/jet.energy())
            rows[3].append(jet.energy()/jet.energy())        

        constraint = [0.,0.,0.,2.*ebeam]

        d2 = array(rows)
        d = array(constraint)
        #print d2
        #print d
        energies = linalg.solve(d2,d)
        #print energies
        chi2 = 0.
        for i,jet in enumerate(jets):

            if energies[i] > 0. :
                uncert = 0.5*sqrt(jet.energy()) + 0.05*jet.energy()
                delta = (jet.energy()-energies[i])/uncert
                if delta > 0. : 
                    chi2 += delta*delta
                else:
                    chi2 += delta*delta/4.
            else:
                chi2 += 1000.

            p4 = jet.p4()
            p4.SetPxPyPzE(jet.px()*energies[i]/jet.energy(),
                          jet.py()*energies[i]/jet.energy(),
                          jet.pz()*energies[i]/jet.energy(),
                          energies[i])
            jet.setP4(p4)

        return chi2

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
        chi2 = self.beta4(self.jets, 120.)
        if chi2 > self.cfg_ana.chi2 :
            #print 'chi2 = ',chi2
            return False

        self.mvis = mvis
        self.px = px
        self.py = py
        self.pz = pz
        self.evis = en
        self.chi2 = chi2
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

    def findWW(self, jetPairs):

        ww = []

        w1 = jetPairs[0]
        w2 = jetPairs[5]
        wwmin = 999.
        for i,pair1 in enumerate(jetPairs):
            pair2 = jetPairs[5-i]
            wwchi = sqrt((pair1.M()-80.3)*(pair1.M()-80.3) + (pair2.M()-80.3)*(pair2.M()-80.3))

            if wwchi < wwmin :
                wwmin = wwchi
                w1 = pair1
                w2 = pair2

        ww = [w1,w2]
        return ww, wwmin

    def findZZ(self, jetPairs):

        zz = []

        z1 = jetPairs[0]
        z2 = jetPairs[5]
        zzmin = 999.
        for i,pair1 in enumerate(jetPairs):
            pair2 = jetPairs[5-i]
            zzchi = sqrt((pair1.M()-91.2)*(pair1.M()-91.2) + (pair2.M()-91.2)*(pair2.M()-91.2))

            if zzchi < zzmin :
                zzmin = zzchi
                z1 = pair1
                z2 = pair2

        zz = [z1,z2]
        return zz, zzmin


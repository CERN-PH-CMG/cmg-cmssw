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
from sets import Set
import numpy
        
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
        count.register('Z->qq and h->tt')
        count.register('Z->bb and h->tt')
        count.register('Z->qq and h->tt selected')
        count.register('Z->bb and h->tt selected')
        count.register('Z->qq and h->tt b-selected')
        count.register('Z->bb and h->tt b-selected')
        self.counters.addCounter('h_rec')
        count1 = self.counters.counter('h_rec')
        count1.register('All events')
        count1.register('4 jets above threshold')
        count1.register('2 tau candidates')
        count1.register('2 tau candidates good pair')    
        count1.register('2 jets with id')
        count1.register('passed' )

        
    def buildMCinfo(self, event):
        self.isHZ = False
        self.isHZqq = False
        self.isHZbb = False
        self.tau = []
        nH = 0
        nZ = 0
        minzene=99999.
        ptcz=0
        
        for ptc in self.mchandles['genParticles'].product():
            if abs(ptc.pdgId())==23:
                nZ+=1
                if ptc.energy()<minzene:
                    minzene=ptc.energy()
                    self.Z=GenParticle(ptc)
                    ptcz=ptc
                    
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
                if ptc.mother(0)==ptcz:
                    if abs(ptc.pdgId())<=6:
                        self.isHZqq=True
                        if abs(ptc.pdgId())==5:
                            self.isHZbb=True
    
    def testE(self):
      n=0
      for jet in self.jets:
        if jet.energy() > self.cfg_ana.minE:
          n+=1
      return n>1    

    def buildClusteredJets_old(self, event):
      event.nontaucand.sort(key=lambda a: a.energy(), reverse = True)
      tmpJets = set(event.nontaucand)
      if len(event.nontaucand) > 2 :
        while len(tmpJets) != 2:
          dijets = self.findPairs(tmpJets)
          dijets.sort(key=lambda a: a.M())
          tmpJets.remove(dijets[0].leg1)
          tmpJets.remove(dijets[0].leg2)
          tmpJets.add(dijets[0])

        event.nontaucand = []
        for jet in tmpJets:
          event.nontaucand.append(jet) 

    def buildClusteredJets(self, jets):
      jets.sort(key=lambda a: a.energy(), reverse = True)
      tmpJets = set(jets)
      if len(jets) > 2 :
        while len(tmpJets) != 2:
          dijets = self.findPairs(tmpJets)
          dijets.sort(key=lambda a: a.M())
          tmpJets.remove(dijets[0].leg1)
          tmpJets.remove(dijets[0].leg2)
          tmpJets.add(dijets[0])

      return tmpJets
            



    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.buildMCinfo( event )   

        self.counters.counter('h_gen').inc('All events')
        self.counters.counter('h_rec').inc('All events')
        if self.isHZ: self.counters.counter('h_gen').inc('All hz events')
        if len(self.tau)==2:
            self.counters.counter('h_gen').inc('h->tt')
            if self.isHZqq:
                self.counters.counter('h_gen').inc('Z->qq and h->tt')
                if self.isHZbb:
                    self.counters.counter('h_gen').inc('Z->bb and h->tt')

        event.isHZ = 0
        event.isHZqq = 0
        event.isHZbb = 0
        
        if self.isHZ:
            event.isHZ=1
            event.H=self.H
            event.Z=self.Z
            if self.isHZqq:
                event.isHZqq=1
            if self.isHZbb:
                event.isHZbb=1
            
        event.ishtt = 0

        if len(self.tau)==2:
            event.ishtt =1
            event.tau1=self.tau[0]
            event.tau2=self.tau[1]

        #save the two leading muons and the two leading electrons
        self.muons = []
        for muon in  self.handles['muons'].product():
          self.muons.append(Muon(muon))
        self.muons.sort(key=lambda a: a.energy(), reverse = True)  
        event.leadingMuons = []
        for muon in self.muons:
          if len(event.leadingMuons) >= 2: break
          event.leadingMuons.append(muon)

        self.electrons = []
        for electron in  self.handles['electrons'].product():
          self.electrons.append(Electron(electron))   
        self.electrons.sort(key=lambda a: a.energy(), reverse = True)
        event.leadingElectrons = []
        for electron in self.electrons:
          if len(event.leadingElectrons) >= 2: break
          event.leadingElectrons.append(electron)   
    

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


#        # prepare electrons ordered in energy
#        self.electrons=[]
#        for ptj in self.handles['electrons'].product():
#            self.electrons.append(Electron(ptj))
#            
#        # order them in energy
#        self.electrons.sort(key=lambda a: a.energy(), reverse = True)
#        
#        # loop over electron and define  "isolation"
#        self.eleiso=[]
#        for ind1 in range(0,len(self.electrons)):
#            cosmax=-2
#            for ind2 in range(ind1+1,len(self.jets)):
#                cos=self.cosdelta(self.electrons[ind1],self.jets[ind2])
#                if cos> cosmax:
#                    cosmax=cos
#            self.eleiso.append(cosmax)
#
#        # prepare muons ordered in energy
#        self.muons=[]
#        for ptj in self.handles['muons'].product():
#            self.muons.append(Electron(ptj))
#            
#        # order them in energy
#        self.muons.sort(key=lambda a: a.energy(), reverse = True)
#        
#        # loop over jets and define  "isolation"
#        self.muoiso=[]
#        for ind1 in range(0,len(self.muons)):
#            cosmax=-2
#            for ind2 in range(ind1+1,len(self.jets)):
#                cos=self.cosdelta(self.muons[ind1],self.jets[ind2])
#                if cos> cosmax:
#                    cosmax=cos
#            self.muoiso.append(cosmax)

            
        

        # look now at the tau jets
        self.taugenjet=[]
        self.taucos=[]
        self.tauiso=[]
        self.taumatchingjetindex=[]

#        self.nontaugenjet = copy.copy(self.jets)
#        self.nontaugenjetiso = copy.copy(self.jetiso)
       
        if event.ishtt==1:

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
                        #self.nontaugenjet.pop(tauindex)
                        #self.nontaugenjetiso.pop(tauindex)
                        self.taumatchingjetindex.append(tauindex)
                    else:
                        self.taugenjet.append(thistau) # just to fill it up
                    self.taucos.append(cosmax)
    
                        
    
                event.tau1genjet = self.taugenjet[0]
                event.tau1cosjet = self.taucos[0]
                event.tau1iso = self.tauiso[0]
                event.tau2genjet = self.taugenjet[1]
                event.tau2cosjet = self.taucos[1]
                event.tau2iso = self.tauiso[1]
                event.tau1matchingjetindex=self.taumatchingjetindex[0]
                event.tau2matchingjetindex=self.taumatchingjetindex[1]


#        self.nontaugenjet.sort(key=lambda a: a.energy(), reverse = True)
#        event.nontaugenjet = self.nontaugenjet
#        event.nontaugenjetiso = self.nontaugenjetiso
        
        
        
        # start here the real selection
        event.step=0
        event.matchedRecGenDistances = []
        event.hz = []
        
        # first look for at least four jets and two of them isolated and low #tracks
        if event.njets<4:
            return
        event.step+=1 # 1
        
        #test for jets above threshold
        etest = self.testE()
        if etest:
          self.counters.counter('h_rec').inc('4 jets above threshold')
        else:
          return
        event.step+=1 #2



#        print "njets=",event.njets,len(self.jets)
        # requires at least 2 taucandidates as isolated jets (0.5) and with 1,2 or 3 tracks
        event.taucand=[]
        event.taucandiso=[]
        event.taucandcharge=[]
        event.nontaucand=[]
        event.nontaucandiso=[]
        event.acoll = -99.
        event.acopl = -99.
        for ind in range(0,len(self.jets)):
#            print "evaluating ",ind
#            ntrk=self.jets[ind].component(1).number()
            # the two most energetic jets are not considered among taus 
            #            if ind>=2 and ntrk>0 and ntrk<=3  and self.jetiso[ind]<0.5 and len(event.taucand)<2:
            #            if ntrk>=1 and ntrk<=3  and self.jetiso[ind]<0.8 and len(event.taucand)<2:
            # if ntrk>=0 and ntrk<=3  and self.jetiso[ind]<0.5 and len(event.taucand)<2:
            # if ntrk>=1 and ntrk<=3  and self.jetiso[ind]<0.9:
#            if ntrk>=1 and ntrk<=3:
#            if ntrk==1 or ntrk==3:
            if self.testTauId(self.jets[ind]):
                event.taucand.append(self.jets[ind])
                event.taucandiso.append(self.jetiso[ind])
#                print "adding to tau"
            else:
                event.nontaucand.append(self.jets[ind])
                event.nontaucandiso.append(self.jetiso[ind])
                #                print "adding to non tau"
                
        # check for 2 tau candidates
        if len(event.taucand)<2:
            return
        self.counters.counter('h_rec').inc('2 tau candidates')    
        event.step+=1 #3

        if len(event.taucand)==2:
            #electrons + muons + charged hadrons
            ntr1=event.taucand[0].component(1).number()+event.taucand[0].component(2).number()+event.taucand[0].component(3).number()
            ntr2=event.taucand[1].component(1).number()+event.taucand[1].component(2).number()+event.taucand[1].component(3).number()
            if (ntr1*ntr2)!=1 and (ntr1*ntr2)!=3:
                return
        # now iterate on all possible pairs of tau candidates, if more than two, and keep only the ones which give
        # opposite mass closer to Z peak

        if len(event.taucand)>2:
            dm=9999.
            taupair=(-1,-1)
            # fit version of tau finding optimization
            for ind1, ind2 in itertools.combinations(range(len(event.taucand)), 2):
                ntr1=event.taucand[ind1].component(1).number()+event.taucand[ind1].component(2).number()+event.taucand[ind1].component(3).number()
                ntr2=event.taucand[ind2].component(1).number()+event.taucand[ind2].component(2).number()+event.taucand[ind2].component(3).number()
                # consider only combination 1,1 1,3 3,1

                if (ntr1*ntr2)!=1 and (ntr1*ntr2)!=3:
                    continue
                nt=[]
                nj=[]
                nt.append(event.taucand[ind1])
                nt.append(event.taucand[ind2])
                nj=copy.copy(event.nontaucand)
                for i in range(len(event.taucand)):
                    if i!=ind1 and i!=ind2:
                        nj.append(event.taucand[i])
                # nt and nj are potential tau and nontaucandidates
                # now clusterize the nontaucand
                tmpJets=list(self.buildClusteredJets(nj))
                # poor man kf
                chi2,newjets,newtaus = self.beta4(tmpJets, nt, 120.,False)

                jet0=newjets[0]
                jet1=newjets[1]
                
                mnontau=(jet0[3]+jet1[3])**2-(jet0[2]+jet1[2])**2-(jet0[1]+jet1[1])**2-(jet0[0]+jet1[0])**2
                if mnontau>0:
                    mnontau=sqrt(mnontau)
                else:
                    mnontau=0
#                print ind1,ind2,mnontau
                if abs(mnontau-91.2)<dm:
                    taupair=(ind1,ind2)
                    dm=abs(mnontau-91.2)
                
                
                
            # poor man optimization of tau candidates
            # first sum-up nontaucand momentum
#            pnontau=[0,0,0,0]
#            for jet in event.nontaucand:
#                pnontau[0]+=jet.px()
#                pnontau[1]+=jet.py()
#                pnontau[2]+=jet.pz()
#                pnontau[3]+=jet.energy()
#            for ind1, ind2 in itertools.combinations(range(len(event.taucand)), 2):
#                ntr1=event.taucand[ind1].component(1).number()
#                ntr2=event.taucand[ind2].component(1).number()
#                if (ntr1*ntr2)!=1 and (ntr1*ntr2)!=3:
#                    continue
#                ptau=[0,0,0,0]
#                for tauind in range(0,len(event.taucand)):
#                    if tauind!=ind1 and tauind!=ind2:
#                        ptau[0]+=event.taucand[tauind].px()
#                        ptau[1]+=event.taucand[tauind].py()
#                        ptau[2]+=event.taucand[tauind].pz()
#                        ptau[3]+=event.taucand[tauind].energy()
#                pall=[0,0,0,0]
#                for comp in range(0,4): pall[comp]=pnontau[comp]+ptau[comp]
#                mnontau=pall[3]**2-pall[0]**2-pall[1]**2-pall[2]**2
#                if mnontau<0: mnontau=0
#                mnontau=sqrt(mnontau)
#                # print ind1,ind2,mnontau
#                if abs(mnontau-91.2)<dm:
#                    taupair=(ind1,ind2)
#                    dm=abs(mnontau-91.2)


            # now we have taupair and we can adapt the candidates
            (t1,t2)=taupair
#            print "chosen",t1,t2

            if t1<0 or t2<0:
                return
#            print "chosen",t1,t2
            t1temp=event.taucand[t1]
            t1isotemp=event.taucandiso[t1]
            t2temp=event.taucand[t2]
            t2isotemp=event.taucandiso[t2]
            
            for tauind in range(0,len(event.taucand)):
                if tauind!=t1 and tauind!=t2:
                    #remove thsi tau candidate and add it to nontaucandidate
                    # print "removing ",tauind
                    event.nontaucand.append(event.taucand[tauind])
                    event.nontaucandiso.append(event.taucandiso[tauind])
            # cleanup the taucand list
            event.taucand=[t1temp,t2temp]
            event.taucandiso=[t1isotemp,t2isotemp]


        self.counters.counter('h_rec').inc('2 tau candidates good pair')    
        event.step+=1 #4
        #fill jet charges for tau candidates
#        for tau in event.taucand:
#          print tau.numberOfSourceCandidatePtrs()
#          constituents = tau.sourcePtr().getPFConstituents()
#          charge = 0
#          for constituent in constituents:
#             charge += constituent.charge()
#          event.taucandcharge.append[charge]
        #fill acollinearity for tau candidates
        tnorm1 = event.taucand[0].p4().P()
        tnorm2 = event.taucand[1].p4().P()
        ttdot = event.taucand[0].px()*event.taucand[1].px() + event.taucand[0].py()*event.taucand[1].py() + event.taucand[0].pz()*event.taucand[1].pz()
        event.acoll = ttdot/(tnorm1*tnorm2)   
   
            
        #MC matching
        if event.ishtt==1 and event.isHZqq==1:
            #check the closest gen tau
            self.matched=[]
            self.matcheddistance=[]
            for taucand in event.taucand:
                    cosmax=-2
                    for gentau in self.tau:
                        thiscos=self.cosdelta(taucand,gentau)
                        if thiscos>cosmax:
                            cosmax=thiscos
                    event.matchedRecGenDistances.append(cosmax)     
  
        #cluster the remaining part of the event

        #        self.buildClusteredJets_old(event)
        tmpJets=self.buildClusteredJets(event.nontaucand)
        # rewrites the jets after clustering
        event.nontaucand = []
        for jet in tmpJets:
            event.nontaucand.append(jet)
        jnorm1 = event.nontaucand[0].p4().P()    
        jnorm2 = event.nontaucand[1].p4().P()    
        jjdot = event.nontaucand[0].px()*event.nontaucand[1].px() + event.nontaucand[0].py()*event.nontaucand[1].py() + event.nontaucand[0].pz()*event.nontaucand[1].pz()  
        event.jcoll = jjdot/(jnorm1*jnorm2)

        event.btag_tt=0
        event.btag_jj=0
        for jet in event.taucand:
            event.btag_tt+=jet.btag(7)
        for jet in event.nontaucand:
            event.btag_jj+=jet.btag(7)
        

        # store variables before rescaling
        event.t1_px=event.taucand[0].px()
        event.t1_py=event.taucand[0].py()
        event.t1_pz=event.taucand[0].pz()
        event.t1_en=event.taucand[0].energy()
        event.t2_px=event.taucand[1].px()
        event.t2_py=event.taucand[1].py()
        event.t2_pz=event.taucand[1].pz()
        event.t2_en=event.taucand[1].energy()
        
        # store variables before rescaling
        event.j1_px=event.nontaucand[0].px()
        event.j1_py=event.nontaucand[0].py()
        event.j1_pz=event.nontaucand[0].pz()
        event.j1_en=event.nontaucand[0].energy()
        event.j2_px=event.nontaucand[1].px()
        event.j2_py=event.nontaucand[1].py()
        event.j2_pz=event.nontaucand[1].pz()
        event.j2_en=event.nontaucand[1].energy()
        
        #check that the clustered jets pass id (this also does the momentum rescaling)

        idpass,newjets,newtaus = self.testId(event.nontaucand, event.taucand,True)
        #fill acoplanarity between met and the plane containinf the taus
        t1 = numpy.array([event.taucand[0].px(), event.taucand[0].py(), event.taucand[0].pz()])
        t2 = numpy.array([event.taucand[1].px(), event.taucand[1].py(), event.taucand[1].pz()])
        norm = numpy.cross(t1,t2)
        #now extract the angle wrt mmissing energy
        me = ([-self.px, -self.py, -self.pz])
        pscal = numpy.dot(norm,me)
        norm_p = numpy.linalg.norm(norm)
        me_p = numpy.linalg.norm(me)
        if norm_p>0 and me_p>0:
          event.acopl = pscal/(norm_p * me_p)


        # store variables after rescaling
        event.t1s_px=event.taucand[0].px()
        event.t1s_py=event.taucand[0].py()
        event.t1s_pz=event.taucand[0].pz()
        event.t1s_en=event.taucand[0].energy()
        event.t2s_px=event.taucand[1].px()
        event.t2s_py=event.taucand[1].py()
        event.t2s_pz=event.taucand[1].pz()
        event.t2s_en=event.taucand[1].energy()
        
        # store variables after rescaling
        event.j1s_px=event.nontaucand[0].px()
        event.j1s_py=event.nontaucand[0].py()
        event.j1s_pz=event.nontaucand[0].pz()
        event.j1s_en=event.nontaucand[0].energy()
        event.j2s_px=event.nontaucand[1].px()
        event.j2s_py=event.nontaucand[1].py()
        event.j2s_pz=event.nontaucand[1].pz()
        event.j2s_en=event.nontaucand[1].energy()

        event.mvis = self.mvis
        event.px = self.px
        event.py = self.py
        event.pz = self.pz
        event.evis = self.evis
        event.chi2 = self.chi2
        event.mmin = self.mmin

        event.alljets=[]
        for jet in event.nontaucand:
            event.alljets.append(jet)
        for jet in event.taucand:
            event.alljets.append(jet)
        #event.alljets contains all jets now (j1,j2,t1,t2)
        event.jetPairs = self.findPairs( event.alljets )
        event.ww, event.wwMin = self.findWW ( event.jetPairs )
        event.zz, event.zzMin = self.findZZ ( event.jetPairs )


        if not idpass:
          return

        self.counters.counter('h_rec').inc('2 jets with id')
        event.step+=1 #5

        


        #finally find the HZ candidates
        reshz,self.hz = self.findHZ(event.nontaucand, event.taucand)
        event.hz = self.hz
#        print "dijetmass ",event.hz[1].M()
        if not reshz:
          return
        event.step+=1 #6
        self.counters.counter('h_rec').inc('passed') 

        if event.ishtt==1 and event.isHZqq==1:
            self.counters.counter('h_gen').inc('Z->qq and h->tt selected')
        if event.ishtt==1 and event.isHZbb==1:
            self.counters.counter('h_gen').inc('Z->bb and h->tt selected')
            
        # apply btag here
        if event.btag_jj<0.95:
            return
        event.step+=1 #7
        if event.ishtt==1 and event.isHZqq==1:
            self.counters.counter('h_gen').inc('Z->qq and h->tt b-selected')
        if event.ishtt==1 and event.isHZbb==1:
            self.counters.counter('h_gen').inc('Z->bb and h->tt b-selected')
        
        

        return True

    def testTauId(self, jet):
        ntrk=jet.component(1).number()+jet.component(2).number()+jet.component(3).number()
        nhf = jet.component(5).fraction()
        chf = jet.component(1).fraction()
        mass = jet.mass()
        return (ntrk==1 or ntrk==3) and nhf < 0.2 #and mass < 7 

    def findPairs(self, jets):
        out = []

        for j1, j2 in itertools.combinations(jets, 2):
            out.append( DiObject(j1, j2) )

        return out 

    def beta4(self, jets, taus, ebeam,changevalues):

        from numpy import array, linalg, dot, add
        
        
        rows = []
        for i in range(4):
            rows.append([])
        for jet in jets:
            rows[0].append(jet.px()/jet.energy())
            rows[1].append(jet.py()/jet.energy())
            rows[2].append(jet.pz()/jet.energy())
            rows[3].append(jet.energy()/jet.energy())        

        for tau in taus:
          rows[0].append(tau.px()/tau.energy())
          rows[1].append(tau.py()/tau.energy())
          rows[2].append(tau.pz()/tau.energy())
          rows[3].append(tau.energy()/tau.energy())

        constraint = [0., 0., 0., 2*ebeam]

        d2 = array(rows)
        d = array(constraint)
        #print d2
        #print d
        energies = linalg.solve(d2,d)
        #print energies
        chi2 = 0.

        newjets=[]
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
            px=jet.px()*energies[i]/jet.energy()
            py=jet.py()*energies[i]/jet.energy()
            pz=jet.pz()*energies[i]/jet.energy()
            
            if changevalues:
                p4.SetPxPyPzE(px,py,pz,energies[i])
                jet.setP4(p4)
            newjets.append([px,py,pz,energies[i]])

        newtaus=[]
        for i, tau in enumerate(taus):
          if energies[i+2] < 0.:
            chi2 += 1000.
          p4 = tau.p4()
          px=tau.px()*energies[i+2]/tau.energy()
          py=tau.py()*energies[i+2]/tau.energy()
          pz=tau.pz()*energies[i+2]/tau.energy()
          if changevalues:
              p4.SetPxPyPzE(px,py,pz,energies[i+2])
              tau.setP4(p4)
          newtaus.append([px,py,pz,energies[i+2]])
        return chi2,newjets,newtaus  

    def testId(self, jets, taus,changevalues) :
   
        newjets=[]
        newtaus=[]
        self.mvis = -100
        self.px = -100
        self.py = -100
        self.pz = -100
        self.evis = -100
        self.chi2 = -100
        self.mmin = -100 
        
    
        njobj = 0
        njtrk = 0
        massmax = -999.
        massmin = 999.
        njeta = 0
        mtot = 0.
        pxj = 0.
        pyj = 0.
        pzj = 0.
        enj = 0.
  
        pxp = 0.
        pyp = 0.
        pzp = 0.
        enp = 0.

        en = 0.
        px = 0.
        py = 0.
        pz = 0.

        if len(jets) != 2 :
            #print 'NJets = ', len(jets)
            return False,newjets,newtaus
        
        for jet in jets:
            if jet.nConstituents() >= self.cfg_ana.npfj[1] : njobj += 1
            if jet.component(1).number() >= self.cfg_ana.ntkj[1] : njtrk += 1
            if jet.mass() > massmax : massmax = jet.mass()
            if jet.mass() < massmin : massmin = jet.mass()
            if abs(jet.eta()) < 3.0 : njeta += 1
            mtot += jet.mass()
            pxj += jet.px()
            pyj += jet.py()
            pzj += jet.pz()
            enj += jet.energy()

        for tau in taus:
            pxp += tau.px()
            pyp += tau.py()
            pzp += tau.pz()
            enp += tau.energy()
        en = enj+enp
        px = pxj+pxp
        py = pyj+pyp
        pz = pzj+pzp
            
        mvis = sqrt(en*en-px*px-py*py-pz*pz)

        # 4 jets
#        if njeta < 4 :
#            #print 'NjetsEta = ',njeta
#            return False,newjets,newtaus

        # At least npfj jets with at least npf particles
        if njobj < self.cfg_ana.npfj[0] :
#            print 'njobj = ',njobj
            return False,newjets,newtaus

        # At least ntkj jets with at least ntk tracks
        if njtrk < self.cfg_ana.ntkj[0] :
#            print 'njtrk = ',njtrk
            return False,newjets,newtaus
        
        # Smallest jet mass larger than tau mass (say)
        if massmin < self.cfg_ana.minM :
#            print 'massmin = ',massmin
            return False,newjets,newtaus
        
        # No missing energy
#        if mvis < self.cfg_ana.mVis :
#            print 'mvis = ',mvis
#            return False,newjets,newtaus

        # Rescale jet energies
        # Check the chi2 ( < 1000. == request all energies to be positive)
        
        chi2,newjets,newtaus = self.beta4(jets, taus, 120.,changevalues)

        if chi2 > self.cfg_ana.chi2 :
            #print 'chi2 = ',chi2
            return False,newjets,newtaus

        self.mvis = mvis
        self.px = px
        self.py = py
        self.pz = pz
        self.evis = en
        self.chi2 = chi2
        self.mmin = massmin

        return True,newjets,newtaus 


    def cosdelta(self,part1,part2):
        part1_p=part1.p()
        part2_p=part2.p()
        cos=(part1.px()*part2.px()+part1.py()*part2.py()+part1.pz()*part2.pz())/(part1_p*part2_p)
        return cos
    


    
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
                          
    def findHZ(self, jets, taus):

        result=False
        hz = []

        hPair = []
        zPair = []
        deltaZ = 999.
        dijets = []
        ditau = []
        ditau = DiObject(taus[0],taus[1])
        dijet = DiObject(jets[0], jets[1])
        #print "diphoton mass",diphoton.M()
        #print "dijet mass",dijet.M()
        hz = [ditau, dijet]         
        if ditau.M() > self.cfg_ana.h_mass and \
           dijet.M() > self.cfg_ana.z_mass[0] and \
           dijet.M() < self.cfg_ana.z_mass[1] :
            result=True
            
        return result,hz

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


import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenParticle,Electron,Muon,Tau

from CMGTools.LEP3.analyzers.DiObject import DiObject

from CMGTools.RootTools.utils.DeltaR import deltaR
from math import pi, sqrt, acos
from sets import Set
import numpy
        
class httanalyzerpftau( Analyzer ):

    def declareHandles(self):
        super(httanalyzerpftau, self).declareHandles()
        self.handles['jets'] = AutoHandle ('cmgPFJetSel',
                                           'std::vector<cmg::PFJet>')
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )
        self.handles['muons'] = AutoHandle( 'cmgMuonSel',
                                                     'std::vector<cmg::Muon>' )
        self.handles['electrons'] = AutoHandle( 'cmgElectronSel',
                                                     'std::vector<cmg::Electron>' )
        self.handles['taus'] = AutoHandle( 'cmgTauSel',
                                                     'std::vector<cmg::Tau>' )

        
        #self.handles['pf'] = AutoHandle ('particleFlow',
        #                                 'std::vector<reco::PFCandidate>')


        
    def beginLoop(self):
        super(httanalyzerpftau,self).beginLoop()
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
#        print "========================================================="
#        print "event:",eventNumber
        
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

        event.leadingMuon_en=-1
        if len(self.muons)>=1:
            event.leadingMuon_en=self.muons[0].energy()
            
        event.leadingMuons = []
        for muon in self.muons:
          if len(event.leadingMuons) >= 2: break
          event.leadingMuons.append(muon)
          # this is needed because the energy of the muon is changed later on if it is a tau candidate
          
        self.electrons = []
        for electron in  self.handles['electrons'].product():
          self.electrons.append(Electron(electron))   
        self.electrons.sort(key=lambda a: a.energy(), reverse = True)

        event.leadingElectron_en=-1
        if len(self.electrons)>=1:
            event.leadingElectron_en=self.electrons[0].energy()

        event.leadingElectrons = []
        for electron in self.electrons:
          if len(event.leadingElectrons) >= 2: break
          event.leadingElectrons.append(electron)   

        self.hadtaus = []
        for tau in  self.handles['taus'].product():
          self.hadtaus.append(Tau(tau))   
        self.hadtaus.sort(key=lambda a: a.energy(), reverse = True)
    
        
        # prepare jets ordered in energy
        self.jets=[]
        for ptj in self.handles['jets'].product():
            self.jets.append(Jet(ptj))
            
        # order them in energy
        self.jets.sort(key=lambda a: a.energy(), reverse = True)
        event.njets=len(self.jets)
        

        # loop over ele,muo,hadtaus and define  "isolation", and jetindex
        # iso is the closest jet (excluding itself) distance
        # index is the pfjet index of the object (ele,muo,tau)
        # dist is the distance of the object to the matched pfjet (it is trivially 1 for pfjets)
        
        self.jetiso,self.jetindex,self.jetdist=self.findiso(self.jets,self.jets)
        self.muoiso,self.muoindex,self.muodist=self.findiso(self.jets,self.muons)
        self.eleiso,self.eleindex,self.eledist=self.findiso(self.jets,self.electrons)
        self.hadtauiso,self.hadtauindex,self.hadtaudist=self.findiso(self.jets,self.hadtaus)


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
        
        
        
        # debug
#        if event.ishtt==1 and event.isHZqq==1:
#            print "tau gen:"
#
#            print event.tau1
#            print event.tau2
#            
#            print "jets"
#            for jet in self.jets:
#                print jet
#            print "muons"
#            for jet in self.muons:
#                print jet
#            print "electrons"
#            for jet in self.electrons:
#                print jet
#            print "taus"
#            for jet in self.hadtaus:
#                print jet
            

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
        # requires at least 2 taucandidates as isolated jets (0.5) and with 1 or 3 tracks
        event.taucand=[]
        event.taucandtype=[]
        event.taucandiso=[]
        event.taucandindex=[]
        event.taucandcharge=[]
        event.nontaucand=[]
        event.nontaucandiso=[]
        event.acoll = -99.
        event.acopl = -99.

        removedjet=[]
        
        # first build up tau dandidates from hadtaus, electrons and muons
        #        print "#jets,muons,elec,hadtaus:",len(self.jets),len(self.muons),len(self.electrons),len(self.hadtaus)
        for ind in range(0,len(self.muons)):
            if self.testmuoId(ind):
                if self.muoindex[ind] not in removedjet: # be sure you get only one copy
                    event.taucand.append(self.muons[ind])
                    event.taucandtype.append(0)
                    event.taucandiso.append(self.muoiso[ind])
                    event.taucandindex.append(self.muoindex[ind])
                    removedjet.append(self.muoindex[ind])
                    # print "added muon ",event.taucandindex[len(event.taucand)-1]

        for ind in range(0,len(self.electrons)):
            if self.testeleId(ind):
                if self.eleindex[ind] not in removedjet: # prefer muons over ele if belonging to same matched jet
                    event.taucand.append(self.electrons[ind])
                    event.taucandtype.append(1)
                    event.taucandiso.append(self.eleiso[ind])
                    event.taucandindex.append(self.eleindex[ind])
                    removedjet.append(self.eleindex[ind])
                    # print "added ele ",event.taucandindex[len(event.taucand)-1]

        for ind in range(0,len(self.hadtaus)):
            if self.testhadtauId(ind):
                if self.hadtauindex[ind] not in removedjet: # prefer electrons,muons over had taus if belonging to same matched jet
                    event.taucand.append(self.hadtaus[ind])
                    event.taucandtype.append(2)
                    event.taucandiso.append(self.hadtauiso[ind])
                    event.taucandindex.append(self.hadtauindex[ind])
                    removedjet.append(self.hadtauindex[ind])
                    # print "added hadtau ",event.taucandindex[len(event.taucand)-1]

                

        # now we can add the jets not associated to candidate taus (ele, muo, hadtau) to the jet list                
        for ind in range(0,len(self.jets)):
            if ind not in removedjet: 
                event.nontaucand.append(self.jets[ind])
                event.nontaucandiso.append(self.jetiso[ind])
                # print "added jet ",event.nontaucand[len(event.nontaucand)-1].energy()
                
        



        # check for 2 tau candidates
        if len(event.taucand)<2:
            return
        self.counters.counter('h_rec').inc('2 tau candidates')    
        event.step+=1 #3

        if len(event.taucand)==2:
            #electrons + muons + charged hadrons
            ntr1=self.ntrtaucand(event.taucand[0],event.taucandtype[0])
            ntr2=self.ntrtaucand(event.taucand[1],event.taucandtype[1])
            if (ntr1*ntr2)!=1 and (ntr1*ntr2)!=3:
                return
        # now iterate on all possible pairs of tau candidates, if more than two, and keep only the ones which give
        # opposite mass closer to Z peak

        if len(event.taucand)>2:
            dm=9999.
            taupair=(-1,-1)
            # fit version of tau finding optimization
            for ind1, ind2 in itertools.combinations(range(len(event.taucand)), 2):
                ntr1=self.ntrtaucand(event.taucand[0],event.taucandtype[0])
                ntr2=self.ntrtaucand(event.taucand[1],event.taucandtype[1])

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
                        nj.append(self.jets[event.taucandindex[i]])

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
                
                
                


            # now we have taupair and we can adapt the candidates
            (t1,t2)=taupair
#            print "chosen",t1,t2

            if t1<0 or t2<0:
                return
#            print "chosen",t1,t2
            t1temp=event.taucand[t1]
            t1isotemp=event.taucandiso[t1]
            t1typetemp=event.taucandtype[t1]
            t1indextemp=event.taucandindex[t1]

            t2temp=event.taucand[t2]
            t2isotemp=event.taucandiso[t2]
            t2typetemp=event.taucandtype[t2]
            t2indextemp=event.taucandindex[t2]
            
            for tauind in range(0,len(event.taucand)):
                if tauind!=t1 and tauind!=t2:
                    #remove thsi tau candidate and add it to nontaucandidate
                    # print "removing ",tauind
                    event.nontaucand.append(self.jets[event.taucandindex[tauind]])
                    event.nontaucandiso.append(self.jetiso[event.taucandindex[tauind]])
            # cleanup the taucand list
            event.taucand=[t1temp,t2temp]
            event.taucandiso=[t1isotemp,t2isotemp]
            event.taucandtype=[t1typetemp,t2typetemp]
            event.taucandindex=[t1indextemp,t2indextemp]


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

        # debug
#        if event.ishtt==1 and event.isHZqq==1:
#            print "tau cand"
#            for jet in event.taucand:
#                print jet
#            print "non taucand"
#            for jet in event.nontaucand:
#                print jet



        jnorm1 = event.nontaucand[0].p4().P()    
        jnorm2 = event.nontaucand[1].p4().P()    
        jjdot = event.nontaucand[0].px()*event.nontaucand[1].px() + event.nontaucand[0].py()*event.nontaucand[1].py() + event.nontaucand[0].pz()*event.nontaucand[1].pz()  
        event.jcoll = jjdot/(jnorm1*jnorm2)

        event.btag_tt=0
        event.btag_jj=0
#        for jet in event.taucand:
#            event.btag_tt+=jet.btag(7)
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
        
#        print "before rescaling:",event.t1_en,event.t2_en,event.j1_en,event.j2_en
#        print "before rescaling from jet:",self.jets[event.taucandindex[0]].energy(),self.jets[event.taucandindex[1]].energy()
        
        #check that the clustered jets pass id (this also does the momentum rescaling)

        idpass,newjets,newtaus = self.testId(event.nontaucand, event.taucand,True)

        # we need also to replace self.jets px,py,pz (will be used for findww,findzz and findhz)
        for ind in range(0,len(event.taucand)):
            j_ind=event.taucandindex[ind]
            p4=self.jets[j_ind].p4()
            p4.SetPxPyPzE(event.taucand[ind].px(),event.taucand[ind].py(),event.taucand[ind].pz(),event.taucand[ind].energy())
            self.jets[j_ind].setP4(p4)

        
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

#        print "after rescaling:",event.t1s_en,event.t2s_en,event.j1s_en,event.j2s_en
#        print "after rescaling from jet:",self.jets[event.taucandindex[0]].energy(),self.jets[event.taucandindex[1]].energy()


        event.mvis = self.mvis
        event.px = self.px
        event.py = self.py
        event.pz = self.pz
        event.evis = self.evis
        event.chi2 = self.chi2
        event.mmin = self.mmin

        event.alljets=[]
        # print "alljets",len(self.jets),event.taucandindex[0],event.taucandindex[1]
        
        # this is needed to use findXX (for Diobject, otherwise btag is not defined)
        event.taucandjets=[self.jets[event.taucandindex[0]],self.jets[event.taucandindex[1]]]


        for jet in event.nontaucand:
            event.alljets.append(jet)
        for jet in event.taucandjets:
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
        reshz,self.hz = self.findHZ(event.nontaucand, event.taucandjets)
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



    def findiso(self,jets,objects):
        iso=[]
        index=[]
        dist=[]
        if len(objects)>=1:
            for ind1 in range(0,len(objects)):
                cos=[]
                if len(jets)>=1:
                    for ind2 in range(0,len(jets)):
                        cos.append(self.cosdelta(objects[ind1],jets[ind2]))
                        sortedindex=sorted(range(len(cos)),key=lambda x:cos[x])
                        sortedindex.reverse()
                    index.append(sortedindex[0])
                    closest=sortedindex[0]
                    dist.append(cos[closest])
                    if len(sortedindex)>=2:
                        closest=sortedindex[1]
                        iso.append(cos[closest])
                    else:
                        iso.append(-2.)
        return iso,index,dist



    def testmuoId(self, ind):
        energy=self.muons[ind].energy()
        # we need to remove muons in jet
        jet=self.jets[self.muoindex[ind]]
        ntrk=jet.component(1).number()+jet.component(2).number()+jet.component(3).number()
        
        return ntrk==1 and energy<50.
    
    def testeleId(self, ind):
        energy=self.electrons[ind].energy()
        # we need to remove muons in jet
        jet=self.jets[self.eleindex[ind]]
        ntrk=jet.component(1).number()+jet.component(2).number()+jet.component(3).number()

        return ntrk==1 and energy<50.
    
    def testhadtauId(self, ind):
        tau=self.hadtaus[ind]
        # energy=tau.energy()
        #print "decayModeFinding",tau.tauID("decayModeFinding")
        #print "byVLooseCombinedIsolationDeltaBetaCorr",tau.tauID("byVLooseCombinedIsolationDeltaBetaCorr")
        #print "byRawIsoMVA",tau.tauID("byRawIsoMVA")
        dmf=tau.tauID("decayModeFinding")
        if dmf != 1.0:
            return False
        
        vl=tau.tauID("byVLooseCombinedIsolationDeltaBetaCorr")
        mva=tau.tauID("byRawIsoMVA")
        #retrun mva>0.98
        return vl>0.


    def ntrtaucand(self,tau,type):
        if type<2: # electron or muon
            ntr=1
        else: # hadtau
            if tau.decayMode()<=5:
                ntr=1
            else:
                ntr=3
        return ntr
    


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


from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import  GenParticle

class HZZGenSelector( Analyzer ):
    '''Selects events at gen level looking at the documentation particles.'''
    
    def declareHandles(self):
        super(HZZGenSelector, self).declareHandles()
       
        self.mchandles['genParticles'] = AutoHandle( self.cfg_ana.col_label,
                                                     'std::vector<reco::GenParticle>' )
    def beginLoop(self):
        super(HZZGenSelector,self).beginLoop()
        self.counters.addCounter('Gen')
        self.count = self.counters.counter('Gen')
        self.count.register('all events')
        self.count.register('weird')
        self.count.register('mu-mu')
        self.count.register('ele-ele')
        self.count.register('mu-ele')
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.count.inc('all events')


        # import pdb; pdb.set_trace()
        higgs = None
        event.status3 = []
        for genp in self.mchandles['genParticles'].product():
            if genp.status()==3:
                event.status3.append( GenParticle(genp) )
                if genp.pdgId() == 25:
                    higgs = genp
                    break
                
        # return False

        if higgs is None:
            return False

        if not self.isHtoZZ( higgs ):
            allDaus = []
            allDaus = self.allDaughters( higgs, allDaus, 0 )
            print 'Event', event.iEv
            print 'Higgs', higgs
            for dau in allDaus:
                print dau.rank*'\t', GenParticle( dau )
            self.count.inc('weird')
            return False
            # import pdb; pdb.set_trace()

        leptons = []
        leptons = self.findStatus1Leptons( higgs, leptons )

        event.leptons = map( GenParticle, leptons )

        nEles = 0
        nMus = 0
        for lepton in event.leptons:
            pdgId = abs(lepton.pdgId())
            if pdgId==11:
                nEles+=1
            elif pdgId==13:
                nMus += 1
            else:
                raise ValueError('a lepton should be e or mu: {pdgId}'.format(pdgId=pdgId))

        if not ( nMus==4 or \
                 nEles==4 or \
                 (nMus==2 and nEles==2) or \
                 self.isHtoZZtoTaus(higgs)) :
            import pdb; pdb.set_trace()
            
        if self.cfg_ana.channel=='mu-mu' and nMus == 4:
            self.count.inc('mu-mu')
            return True
        elif self.cfg_ana.channel=='ele-ele' and nEles == 4:
            self.count.inc('ele-ele')
            return True
        elif self.cfg_ana.channel=='mu-ele' and nMus == 2 and nEles == 2:
            self.count.inc('mu-ele')
            return True
        else:
            return False


    def isHtoZZtoTaus(self, higgs):
        nTaus = 0
        nZs = 0
        for i in range( higgs.numberOfDaughters() ):
            dau = higgs.daughter(i)
            if dau.pdgId()==25:
                continue
            if dau.pdgId()!=23:
                return False
            # is a Z
            nZs += 1
            theZ = dau
            for j in range( theZ.numberOfDaughters() ):
                lep = theZ.daughter(j)
                if abs(lep.pdgId() )==15:
                    nTaus += 1
        if nZs != 2:
            return False
        if nTaus!=2 and nTaus!=4:
            return False
        return True


    def isHtoZZ(self, higgs):
        # import pdb; pdb.set_trace()
        nZs = 0
        for i in range( higgs.numberOfDaughters() ):
            dau = higgs.daughter(i)
            if dau.pdgId() == 23:
                 nZs += 1
            elif dau.pdgId()!=25:
                # the higgs can have a status 2 higgs as a daughter
                return False
        if nZs != 2:
            return False
        return True

        
    def findStatus1Leptons(self, particle, leptons):
        for i in range( particle.numberOfDaughters() ):
            dau = particle.daughter(i)
            # if abs(particle.pdgId())==15:
            #    print GenParticle(dau)
            if dau.status() == 1:
                if abs(dau.pdgId())==11 or abs(dau.pdgId())==13:
                    leptons.append( dau )
                else:
                    continue
            else:
                leptons = self.findStatus1Leptons( dau, leptons )
        return leptons


    def allDaughters(self, particle, daughters, rank ):
        rank += 1 
        for i in range( particle.numberOfDaughters() ):
            dau = particle.daughter(i)
            dau.rank = rank
            daughters.append( dau )
            daughters = self.allDaughters( dau, daughters, rank )
        return daughters
            

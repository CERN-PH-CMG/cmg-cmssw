from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.GenStudies.particles import GenJet


def returnHiggsDaughters(particle):

    _HiggsDaughters_ = []

    for i in range(particle.numberOfDaughters()):
        if(particle.daughter(i).status()==3):
            _HiggsDaughters_.append(GenParticle(particle.daughter(i)))

    return _HiggsDaughters_


def returnMETfromHiggsDaughters(particles):

    _met_ = []

    for i in range(len(particles)):
        for j in range(particles[i].numberOfDaughters()):
            if(particles[i].daughter(j).status()==3 and
               abs(particles[i].daughter(j).pdgId()) in [12,14,16]):
                
                _met_.append(GenParticle(particles[i].daughter(j)))

    return _met_


class GenObjectReader(Analyzer):
    ''' Read physics objects '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(GenObjectReader,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(GenObjectReader, self).declareHandles()
        self.handles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )
        self.handles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )

    def beginLoop(self):
        super(GenObjectReader,self).beginLoop()
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        # For cut flow
        event.tCounter = [-1 for ic in range(10)]

        # keep in mind we're reading C++ reco::GenParticles:
        # http://cmslxr.fnal.gov/lxr/source/DataFormats/Candidate/interface/LeafCandidate.h

        event.genJets = [GenJet(gj.p4()) for gj in self.handles['genJets'].product()]

        event.genLeptons3 = [l for l in self.handles['genParticles'].product() if
                             l.status()==3 and abs(l.pdgId()) in [11,13,15]]

        event.genParticles3 = [GenParticle(l) for l in self.handles['genParticles'].product() if
                               l.status()==3]

        event.Higgs = [l for l in self.handles['genParticles'].product() if
                       l.status()==3 and l.pdgId()==6] # will be changed later to Higgs
        
        if(len(event.Higgs)!=1):
            print 'More than two Higgs !'
            
        event.HiggsDaughters = returnHiggsDaughters(event.Higgs[0])

        if(len(event.HiggsDaughters)!=2):
            print 'Not two daughters from Higgs!'
#        else:
#            print 'OK : ', event.HiggsDaughters[0].pdgId(), event.HiggsDaughters[1].pdgId()

        event.METfromHiggsDaughters = returnMETfromHiggsDaughters(event.HiggsDaughters)

#        print '# of neutrinos ', len(event.METfromHiggsDaughters)
#        for a in range(len(event.METfromHiggsDaughters)):
#            print ' --> ', a, event.METfromHiggsDaughters[a].p4().pt()


        return True
    
        

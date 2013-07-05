from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.GenStudies.particles import GenJet
from CMGTools.GenStudies.DiLepton import Higgs
from CMGTools.GenStudies.DiLepton import DiLepton
import CMGTools.GenStudies.GenStudyParameter as parameter

import inspect

                                           
class GenObjectReader(Analyzer):
    ''' Read physics objects '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(GenObjectReader,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(GenObjectReader, self).declareHandles()
        self.handles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )
        self.handles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )
        self.handles['generator'] = AutoHandle( *self.cfg_ana.genCol) # For the PDF uncertainty estimation

    def beginLoop(self):
        super(GenObjectReader,self).beginLoop()
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        # keep in mind we're reading C++ reco::GenParticles:
        # http://cmslxr.fnal.gov/lxr/source/DataFormats/Candidate/interface/LeafCandidate.h

        event.genParticles3 = []
        higgs = []

        try:
            event.genJets = [GenJet(gj.p4()) for gj in self.handles['genJets'].product()]
        except:
            print 'No genJets information available. Ignore this events'
            return False

        # Retrieve PDF scales
        event.generator = self.handles['generator'].product()

        event.genLeptons3 = [l for l in self.handles['genParticles'].product() if
                             l.status()==3 and abs(l.pdgId()) in [11,13,15]]


        event.neutrinos1 = [GenParticle(l) for l in self.handles['genParticles'].product() if
                            l.status()==1 and abs(l.pdgId()) in [12,14,16]]
        

        for gp in self.handles['genParticles'].product():
            pygp = GenParticle(gp)

            if(gp.status()==3):
                event.genParticles3.append(pygp)

            if(gp.status()==3 and abs(gp.pdgId())==25):
                higgs.append(pygp)
        
        if(len(higgs)!=1):
            print 'More than two Higgs'

        event.Higgs = Higgs(higgs[0], event.neutrinos1)
        event.dilepton = DiLepton(event.Higgs.leg1(), event.Higgs.leg2(), event.Higgs.met())
        event.channel = event.Higgs.decayId()

        return True

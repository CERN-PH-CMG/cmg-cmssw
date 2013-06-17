import random
import copy
import math

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
# from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenJet
from CMGTools.RootTools.physicsobjects.LorentzVectors import LorentzVector
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2

from CMGTools.GenStudies.particles import GenJet, Jet, SimJet, PFJet, Electron, Muon, Tau


def CountDaughter(particle, rank):

    rank += 1

    NumberOfLeptons = 0

    for i in range(particle.numberOfDaughters()):
        if(particle.daughter(i).status()==3):
#            print "    "*rank, "parent, pdgId = ", particle.pdgId(), particle.daughter(i).pdgId()
            if(abs(particle.daughter(i).pdgId())==11 or
               abs(particle.daughter(i).pdgId())==13 or
               abs(particle.daughter(i).pdgId())==15):
                NumberOfLeptons += 1
                
        CountDaughter(particle.daughter(i), rank)        

#    print 'Nlepton = ', NumberOfLeptons
    return NumberOfLeptons



class GenObjectReader( Analyzer ):
    """Read physics objects, apply efficiency and fake parametrization, smear them.
    """

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

        event.genJets = []
        for gj in self.handles['genJets'].product():
            event.genJets.append( GenJet(gj.p4()) )
#            print 'yuta', gj.p4().pt()

        event.genElectrons = []
        event.genMuons = []
        event.genTaus = []
        event.genParticles = []
        event.genParticles3 = []


        event.Nlepton = 0
        
#        event.Nw = []
#        event.Nb = []

        for gp in self.handles['genParticles'].product():
            if abs(gp.pdgId()) == 6:

#                event.Ntop.append(6)
                for idau in range(gp.numberOfDaughters()):
#                    dau = GenParticle(gp.daughter(idau))
#                    print gp.daughter(idau).pdgId()
                    if abs(gp.daughter(idau).pdgId())==24:
#                        list = CounterDaughter(gp.daughter(idau))
                        event.Nlepton += CountDaughter(gp.daughter(idau), 0)

        print 'Nlepton = ', event.Nlepton
#                        event.Nw.append(24)
#                    elif abs(gp.daughter(idau).pdgId())==5:
#                        event.Nb.append(5)



        
        for gp in self.handles['genParticles'].product():
            pygp = GenParticle(gp)
            event.genParticles.append(pygp)


            
            # keep in mind we're reading C++ reco::GenParticles:
            # http://cmslxr.fnal.gov/lxr/source/DataFormats/Candidate/interface/LeafCandidate.h
#            event.genParticles.append( pygp )


            #TODO genParticlesPruned doesn't contain status 3???
            # Maybe specific to HZHA?

#            if(gp.pdgId()==15):
#                print 'Tau! Yuta, pdgId, status, pt = ', gp.pdgId(), gp.status(), gp.p4().pt()
        
#            if(gp.status()==3 and gp.pdgId()==15):
#                print 'Yuta, pdgId, status, pt = ', gp.pdgId(), gp.status(), gp.p4().pt()

            if gp.status() == 3: 
                event.genParticles3.append( pygp ) 
            if abs(gp.pdgId()) == 11: 
                event.genElectrons.append( Electron(gp.p4()) )
            elif abs(gp.pdgId()) == 13:
                event.genMuons.append( Muon(gp.p4()) )
            elif abs(gp.pdgId()) == 15:
                event.genTaus.append( Tau(gp.p4()) )


#        print '(gen_particle, gen_particle w/stable, Ntaus, gen_jets) = ', len(event.genParticles), len(event.genParticles3), len(event.genTaus), len(event.genJets)

        # what you want here is the daughters of the Higgs boson 
        event.higgsdaughters = []

        # create event.cleanGenJets from event.genJets, removing the ones that are close
        # to the higgs daughters
        # to do that, check the code in GenJetAnalyzer.py
        
        return True
    
        

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.GenStudies.particles import GenJet, Jet, Electron, Muon, Tau
#from CMGTools.RootTools.physicsobjects.LorentzVectors import LorentzVector

def HiggsDaughter(particle, rank):

    rank += 1
    NumberOfLeptons = 0

    for i in range(particle.numberOfDaughters()):
        if(particle.daughter(i).status()==3):

            if(abs(particle.daughter(i).pdgId())==11 or
               abs(particle.daughter(i).pdgId())==13 or
               abs(particle.daughter(i).pdgId())==15):
                NumberOfLeptons += 1
                
        CountDaughter(particle.daughter(i), rank)        

    return NumberOfLeptons



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

        # Retrieve generator-level jets
        event.genJets = []
        for gj in self.handles['genJets'].product():
            event.genJets.append( GenJet(gj.p4()) )


        event.genElectrons = []
        event.genMuons = []
        event.genTaus = []
        event.genParticles = []
        event.genParticles3 = []
        event.tCounter = [-1 for ic in range(10)]
        
        event.Leptons3 = [l for l in self.handles['genParticles'].product() if
                          l.status()==3 and abs(l.pdgId()) in [11,13,15]]

        
        for gp in self.handles['genParticles'].product():
            pygp = GenParticle(gp)
            event.genParticles.append(pygp)

            # keep in mind we're reading C++ reco::GenParticles:
            # http://cmslxr.fnal.gov/lxr/source/DataFormats/Candidate/interface/LeafCandidate.h

            if gp.status() == 3: 
                event.genParticles3.append( pygp ) 

                if abs(gp.pdgId()) == 11: 
                    event.genElectrons.append( Electron(gp.p4()) )
                elif abs(gp.pdgId()) == 13:
                    event.genMuons.append( Muon(gp.p4()) )
                elif abs(gp.pdgId()) == 15:
                    event.genTaus.append( Tau(gp.p4()) )


        # what you want here is the daughters of the Higgs boson 
        event.higgsdaughters = []

        # create event.cleanGenJets from event.genJets, removing the ones that are close
        # to the higgs daughters
        # to do that, check the code in GenJetAnalyzer.py
        
        return True
    
        

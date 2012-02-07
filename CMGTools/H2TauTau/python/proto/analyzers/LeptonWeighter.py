from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average

from CMGTools.H2TauTau.proto.TriggerEfficiency import TriggerEfficiency

class LeptonWeighter( Analyzer ):
    '''Gets lepton efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(LeptonWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

        self.leptonName = self.cfg_ana.lepton
        self.lepton = None
        self.weight = None
        self.weightFactor = 1.
        self.trigEff = None
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
            if isinstance( self.cfg_ana.effWeight, float ):
                self.weightFactor = self.cfg_ana.effWeight
            else:
                self.trigEff = TriggerEfficiency()
                self.trigEff.lepEff = getattr( self.trigEff,
                                               self.cfg_ana.effWeight )
        
            
    def declareHandles(self):
        super(LeptonWeighter, self).declareHandles()

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )
    
    def beginLoop(self):
        print self, self.__class__
        super(LeptonWeighter,self).beginLoop()
        # self.counters.addCounter( self.name )
        self.averages.add('leptonWeight', Average('leptonWeight') )


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.weight = 1 
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
            # if self.trigEff.tauEff is not None:
            #    event.tauEffWeight = self.trigEff.tauEff(event.tau.pt())
            #MUONS
            self.weight *= self.weightFactor
            if self.trigEff is not None:
                self.lepton = getattr( event, self.leptonName )
                self.weight = self.trigEff.lepEff( self.lepton.pt(),
                                                   self.lepton.eta() )
        varName = '_'.join([self.leptonName, 'weight'])
        setattr( event, varName, self.weight )
        event.eventWeight *= self.weight
        self.averages['leptonWeight'].add( self.weight )
        if self.cfg_ana.verbose:
            print ' '.join([self.name, self.leptonName, str(self.weight), str(self.lepton) ])
                

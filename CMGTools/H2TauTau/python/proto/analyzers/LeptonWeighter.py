from CMGTools.H2TauTau.proto.framework.Analyzer import Analyzer
from CMGTools.H2TauTau.proto.framework.AutoHandle import AutoHandle

from CMGTools.H2TauTau.macros.TriggerEfficiency import TriggerEfficiency

class LeptonWeighter( Analyzer ):
    '''Gets lepton efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(LeptonWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

        self.leptonName = self.cfg_ana.lepton
        self.lepton = None
        self.weight = None
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
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
        self.counters.addCounter( self.name )

        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.weight = 1 
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
            # if self.trigEff.tauEff is not None:
            #    event.tauEffWeight = self.trigEff.tauEff(event.tau.pt())
            #MUONS
            if self.trigEff.lepEff is not None:
                self.lepton = getattr( event, self.leptonName )
                self.weight = self.trigEff.lepEff( self.lepton.pt(),
                                                   self.lepton.eta() )
        setattr( event, '_'.join([self.leptonName, 'weight']), self.weight )
        if self.cfg_ana.verbose:
            print ' '.join([self.name, self.leptonName, str(self.weight), str(self.lepton) ])
                

from CMGTools.H2TauTau.proto.framework.Analyzer import Analyzer
from CMGTools.H2TauTau.proto.framework.AutoHandle import AutoHandle

from CMGTools.H2TauTau.proto.analyzers.Regions import H2TauTauRegions
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import H2TauTauOutput as H2TauTauOutput 
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import inclusiveRegionName
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import wholeMTRegionName
from CMGTools.H2TauTau.proto.physicsobjects.PhysicsObjects import GenParticle 

class H2TauTauEventSorter( Analyzer ):
    '''Analyze the event content, and fills histograms for the signal
    and control regions.'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(H2TauTauEventSorter,self).__init__(cfg_ana, cfg_comp, looperName)
        self.regions = H2TauTauRegions( self.cfg_ana )
        dirName = '/'.join( [self.looperName, self.name] ) 
        self.output = H2TauTauOutput(  dirName, self.regions  )
        if self.cfg_comp.name == 'DYJets':
            self.outputFakes = H2TauTauOutput( dirName + '_Fakes', self.regions )

    def declareHandles(self):
        super(H2TauTauEventSorter, self).declareHandles()

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )
#        self.mchandles['vertexWeight'] = AutoHandle( self.cfg_ana.vertexWeight,
#                                                     'double' )

        self.embhandles['generatorWeight'] = AutoHandle( ('generator', 'weight'),
                                                         'double')
    
    def beginLoop(self):
        super(H2TauTauEventSorter,self).beginLoop()
        self.counters.addCounter('Sorter')

        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        #WARNING set up weighting
#        if self.cfg_comp.isMC:
#            event.vertexWeight = self.mchandles['vertexWeight'].product()[0]
#            event.eventWeight *= event.vertexWeight
        elif self.cfg_comp.isEmbed:
            event.generatorWeight = self.embhandles['generatorWeight'].product()[0]
            event.eventWeight *= event.generatorWeight
            
        matched = None
        if self.cfg_comp.name == 'DYJets':
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                matched = True
            else:
                matched = False

        
        regionName = self.regions.test( event )
        # print regionName
        self.counters.counter('Sorter').inc('All events')
        if matched is None or matched is True:
            self.output.Fill( event, regionName )
        elif matched is False: 
            self.outputFakes.Fill( event, regionName )
        else:
            raise ValueError('should not happen!')
        return True


    def write(self):
        '''Write all histograms to their root files'''
        self.output.Write()
        if self.cfg_comp.name == 'DYJets':
            self.outputFakes.Write()

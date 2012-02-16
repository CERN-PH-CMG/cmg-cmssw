from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average

from CMGTools.H2TauTau.proto.analyzers.Regions import H2TauTauRegions
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import H2TauTauOutput as H2TauTauOutput 
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import inclusiveRegionName
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import wholeMTRegionName
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle 

class H2TauTauEventSorter( Analyzer ):
    '''Analyze the event content, and fills histograms for the signal
    and control regions.'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(H2TauTauEventSorter,self).__init__(cfg_ana, cfg_comp, looperName)
        self.regions = H2TauTauRegions( self.cfg_ana )
        # dirName = '/'.join( [self.looperName, self.name] ) 
        self.output = H2TauTauOutput(  looperName, self.regions,
                                       self.cfg_ana.leg1,
                                       self.cfg_ana.leg2 )
        if self.cfg_comp.name == 'DYJets':
            self.outputFakes = H2TauTauOutput( looperName + '_Fakes', self.regions,
                                               self.cfg_ana.leg1,
                                               self.cfg_ana.leg2 )

    def declareHandles(self):
        super(H2TauTauEventSorter, self).declareHandles()

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )

        self.embhandles['generatorWeight'] = AutoHandle( ('generator', 'weight'),
                                                         'double')
    
    def beginLoop(self):
        super(H2TauTauEventSorter,self).beginLoop()
        self.counters.addCounter('Sorter')
        self.averages.add('generatorWeight', Average('generatorWeight') )
        self.averages.add('eventWeight', Average('eventWeight') )

        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        event.generatorWeight = 1
        if self.cfg_comp.isEmbed:
            event.generatorWeight = self.embhandles['generatorWeight'].product()[0]
            event.eventWeight *= event.generatorWeight
        self.averages['generatorWeight'].add(event.generatorWeight)

        # full weight 
        self.averages['eventWeight'].add(event.eventWeight)
            
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
            self.counters.counter('Sorter').inc('Matched = None or True')
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

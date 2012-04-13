from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average

class VertexAnalyzer( Analyzer ):
    '''Analyze vertices, add weight to MC events'''

    def declareHandles(self):
        super(VertexAnalyzer, self).declareHandles()
        self.handles['vertices'] =  AutoHandle(
            'offlinePrimaryVertices',
            'std::vector<reco::Vertex>'
            )
        self.fixedWeight = None
        if self.cfg_comp.isMC and not hasattr( self.cfg_ana, 'fixedWeight'): 
            self.mchandles['vertexWeight'] = AutoHandle( self.cfg_ana.vertexWeight,
                                                         'double' )
        else:
            self.fixedWeight = self.cfg_ana.fixedWeight

    def beginLoop(self):
        super(VertexAnalyzer,self).beginLoop()
        self.averages.add('vertexWeight', Average('vertexWeight') )


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.vertices = self.handles['vertices'].product()
        event.vertexWeight = 1
        if self.fixedWeight is None:
            event.vertexWeight = self.mchandles['vertexWeight'].product()[0]
        else:
            event.vertexWeight = self.fixedWeight
        event.eventWeight *= event.vertexWeight
            
        self.averages['vertexWeight'].add( event.vertexWeight )
        if self.verbose:
            print 'VertexAnalyzer: #vert = ', len(event.vertices), \
                  ', weight = ', event.vertexWeight

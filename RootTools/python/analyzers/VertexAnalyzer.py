from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.physicsobjects.PileUpSummaryInfo import PileUpSummaryInfo

class VertexAnalyzer( Analyzer ):
    '''Analyze vertices, add weight to MC events'''

    def declareHandles(self):
        super(VertexAnalyzer, self).declareHandles()
        self.handles['vertices'] =  AutoHandle(
            'offlinePrimaryVertices',
            'std::vector<reco::Vertex>'
            )
        self.fixedWeight = None
        if self.cfg_comp.isMC:
            if hasattr( self.cfg_ana, 'fixedWeight'):
                self.fixedWeight = self.cfg_ana.fixedWeight
            else:
                self.mchandles['vertexWeight'] = AutoHandle( self.cfg_ana.vertexWeight,
                                                             'double' )

        self.mchandles['pusi'] =  AutoHandle(
            'addPileupInfo',
            'std::vector<PileupSummaryInfo>' 
            )        

        self.handles['rho'] =  AutoHandle(
            ('kt6PFJets','rho'),
            'double' 
            )        

    def beginLoop(self):
        super(VertexAnalyzer,self).beginLoop()
        self.averages.add('vertexWeight', Average('vertexWeight') )

        self.counters.addCounter('GoodVertex')
        self.count = self.counters.counter('GoodVertex')
        self.count.register('All Events')
        self.count.register('Events With Good Vertex')



    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.rho = self.handles['rho'].product()[0]
        event.vertices = self.handles['vertices'].product()
        event.goodVertices = filter(self.testGoodVertex,event.vertices)


        self.count.inc('All Events')

        
        event.vertexWeight = 1
        if self.cfg_comp.isMC:
            event.pileUpInfo = map( PileUpSummaryInfo,
                                    self.mchandles['pusi'].product() )
            if self.fixedWeight is None:
                event.vertexWeight = self.mchandles['vertexWeight'].product()[0]
            else:
                event.vertexWeight = self.fixedWeight
        event.eventWeight *= event.vertexWeight
            
        self.averages['vertexWeight'].add( event.vertexWeight )
        if self.verbose:
            print 'VertexAnalyzer: #vert = ', len(event.vertices), \
                  ', weight = ', event.vertexWeight

        if len(event.goodVertices)==0:
            return False

        self.count.inc('Events With Good Vertex')
        return True



    def testGoodVertex(self,vertex):
        if vertex.isFake():
            return False
        if vertex.ndof()<=4:
            return False
        if abs(vertex.z())>24:
            return False
        if vertex.position().Rho()>2:
            return False
     
        return True
                                                                 

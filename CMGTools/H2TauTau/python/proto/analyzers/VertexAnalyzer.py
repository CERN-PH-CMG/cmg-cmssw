from CMGTools.H2TauTau.proto.framework.Analyzer import Analyzer
from CMGTools.H2TauTau.proto.framework.AutoHandle import AutoHandle

class VertexAnalyzer( Analyzer ):
    '''Analyze vertices, add weight to MC events'''

    def declareHandles(self):
        super(VertexAnalyzer, self).declareHandles()
        self.handles['vertices'] =  AutoHandle(
            'offlinePrimaryVertices',
            'std::vector<reco::Vertex>'
            )
        if self.cfg_comp.isMC and self.cfg_comp.vertexWeight is not None: 
            self.handles['vertexWeight'] = AutoHandle( self.cfg_comp.vertexWeight,
                                                       'double' )

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.vertices = self.handles['vertices'].product()
        event.vertexWeight = 1
        if self.cfg_comp.isMC:
            event.vertexWeight = self.handles['vertexWeight'].product()[0]
            event.eventWeight *= event.vertexWeight
        if self.verbose:
            print 'VertexAnalyzer: #vert = ', len(event.vertices), \
                  ', weight = ', event.vertexWeight

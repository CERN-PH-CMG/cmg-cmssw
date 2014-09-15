from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.TTHAnalysis.signedSip import SignedImpactParameterComputer

from CMGTools.RootTools.utils.DeltaR import deltaR, deltaPhi, bestMatch
 
class ttHSVAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHSVAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHSVAnalyzer, self).declareHandles()
        self.handles['ivf'] = AutoHandle( ('slimmedSecondaryVertices',''),'std::vector<reco::VertexCompositePtrCandidate>')

    def beginLoop(self):
        super(ttHSVAnalyzer,self).beginLoop()

       
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        #get all vertices from IVF
        allivf = [ v for v in self.handles['ivf'].product() ]
       
        # attach distances to PV
        pv = event.goodVertices[0]
        for sv in allivf:
             sv.dxy = SignedImpactParameterComputer.vertexDxy(sv, pv)
             sv.d3d = SignedImpactParameterComputer.vertexD3d(sv, pv)
             sv.cosTheta = SignedImpactParameterComputer.vertexDdotP(sv, pv)

        event.ivf = allivf

        return True

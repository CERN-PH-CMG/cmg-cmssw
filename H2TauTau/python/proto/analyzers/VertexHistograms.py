from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

from CMGTools.RootTools.statistics.Histograms import Histograms

class VertexHistograms(Histograms):
    def __init__(self, name ):
        self.name = name

        self.h_nvertices = TH1F(name+'_h_nvertices', ';# vertices',
                                25,-0.5,24.5)

        self.hists = []
        self.hists.append( self.h_nvertices )
        
    def FillVertices(self, vertices, weight=1):
        self.h_nvertices.Fill( len(vertices), weight)
        
    def FormatHistos(self, style ):
        for hist in self.hists:
            style.formatHisto( hist )
            
    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()

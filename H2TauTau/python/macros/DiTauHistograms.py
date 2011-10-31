from ROOT import TTree, TH1F, TH2F, TProfile, TLegend, gDirectory

class DiTauHistograms:
    def __init__(self, name):
        self.name = name

        self.h_pzeta = TH1F(name+'_h_pzeta', ';p_{#zeta} (GeV)', 150, -200,100)
        self.h_vismass = TH1F(name+'_h_vismass', ';m_{vis} (GeV)', 100,0,200)

        self.hists = []
        self.hists.append( self.h_pzeta )
        self.hists.append( self.h_vismass )
        
        
    def fillHistos(self, tree, cut='', nEvents=999999999):
        print 'filling histograms: ' + self.name
        tree.Draw('tauMu.obj[0].pZeta()>>'+self.h_pzeta.GetName(),cut,'goff',nEvents)
        tree.Draw('tauMu.obj[0].mass()>>'+self.h_vismass.GetName(),cut,'goff',nEvents)
        
    def formatHistos(self, style ):
        for hist in self.hists:
            style.formatHisto( hist )
            
            
    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()

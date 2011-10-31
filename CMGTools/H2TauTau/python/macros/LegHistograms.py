from ROOT import TTree, TH1F, TH2F, TProfile, TLegend, gDirectory

class LegHistograms:
    def __init__(self, name, leg):
        self.name = name
        self.leg = leg

        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 100, 0,200)

        self.hists = []
        self.hists.append( self.h_pt )
        
    def fillHistos(self, tree, cut='', nEvents=999999999):
        print 'filling histograms: ' + self.name
        tree.Draw('tauMu.obj[0].%s().pt()>>%s' % (self.leg, self.h_pt.GetName()),cut,'goff',nEvents)
        
    def formatHistos(self, style ):
        for hist in self.hists:
            style.formatHisto( hist )
            
    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()

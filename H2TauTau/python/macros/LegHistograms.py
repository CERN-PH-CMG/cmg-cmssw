from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

class LegHistograms:
    def __init__(self, name, leg):
        self.name = name
        self.leg = leg

        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 100, 0, 200)
        self.h_iso = TH1F(name+'_h_iso', ';rel iso', 200, 0, 1)
        self.h_iso_dbeta = TH1F(name+'_h_iso_dbeta', ';rel iso', 200, 0, 1)

        self.hists = []
        self.hists.append( self.h_pt )
        self.hists.append( self.h_iso )
        self.hists.append( self.h_iso_dbeta )
        
    def fillHistos(self, tree, cut='', nEvents=999999999):
        print 'filling histograms: ' + self.name
        tree.Draw('tauMu.obj[0].%s().pt()>>%s' % (self.leg, self.h_pt.GetName()),cut,'goff',nEvents)

    def fillLeg(self, leg, weight ):
        self.h_pt.Fill( leg.pt(), weight )
        self.h_iso.Fill( leg.relIso(), weight )
        self.h_iso_dbeta.Fill( leg.relIso(0.5), weight )
        
    def formatHistos(self, style ):
        for hist in self.hists:
            style.formatHisto( hist )
            
    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()

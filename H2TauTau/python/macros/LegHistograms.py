from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

class LegHistograms:
    def __init__(self, name, leg):
        self.name = name
        self.leg = leg

        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 40, 0, 100)
        self.h_eta = TH1F(name+'_h_eta', ';#eta', 50, -2.5, 2.5)
        self.h_iso = TH1F(name+'_h_iso', ';rel iso', 50, 0, 0.5)
        self.h_iso_dbeta = TH1F(name+'_h_iso_dbeta', ';rel iso', 50, 0, 0.5)
        self.h_iso_abs = TH1F(name+'_h_iso_abs', ';abs iso', 50, 0, 20)
        self.h_iso_abs_dbeta = TH1F(name+'_h_iso_abs_dbeta', ';abs iso', 50, 0, 20)

        self.h_iso_ch = TH1F(name+'_h_iso_ch', ';ch iso', 50, 0, 20)
        self.h_iso_nh = TH1F(name+'_h_iso_nh', ';nh iso', 50, 0, 20)
        self.h_iso_ph = TH1F(name+'_h_iso_ph', ';ph iso', 50, 0, 20)

        self.hists = []
        self.hists.append( self.h_pt )
        self.hists.append( self.h_eta )
        self.hists.append( self.h_iso )
        self.hists.append( self.h_iso_dbeta )
        self.hists.append( self.h_iso_abs )
        self.hists.append( self.h_iso_abs_dbeta )
        self.hists.append( self.h_iso_ch )
        self.hists.append( self.h_iso_nh )
        self.hists.append( self.h_iso_ph )
        
    def fillHistos(self, tree, cut='', nEvents=999999999):
        print 'filling histograms: ' + self.name
        tree.Draw('tauMu.obj[0].%s().pt()>>%s' % (self.leg, self.h_pt.GetName()),cut,'goff',nEvents)

    def fillLeg(self, leg, weight ):
        dbetaFactor = 0.5
        self.h_pt.Fill( leg.pt(), weight )
        self.h_eta.Fill( leg.eta(), weight )
        self.h_iso.Fill( leg.relIso(), weight )
        self.h_iso_dbeta.Fill( leg.relIso(dbetaFactor), weight )
        self.h_iso_abs.Fill( leg.absIso(), weight )
        self.h_iso_abs_dbeta.Fill( leg.absIso(dbetaFactor), weight )
        self.h_iso_ch.Fill( leg.chargedHadronIso(), weight )
        self.h_iso_nh.Fill( leg.neutralHadronIso(), weight )
        self.h_iso_ph.Fill( leg.photonIso(), weight )
        
    def formatHistos(self, style ):
        for hist in self.hists:
            style.formatHisto( hist )
            
    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()

from ROOT import TTree, TH1F, TH2F, TProfile, TLegend, gDirectory

class DiTauHistograms:
    def __init__(self, name):
        self.name = name

        self.h_pzeta = TH1F(name+'_h_pzeta', ';p_{#zeta} (GeV)', 150, -200,100)
        self.h_vismass = TH1F(name+'_h_vismass', ';m_{vis} (GeV)', 40, 0, 200)
        self.h_svfitmass = TH1F(name+'_h_svfitmass', ';m_{SVFit} (GeV)', 38, 0, 304)
        self.h_mT = TH1F(name+'_h_mT', ';m_{T} (GeV)', 100,0,200)

        self.hists = []
        #COLIN could find all the attributes with a given type, and automatically append them!
        self.hists.append( self.h_pzeta )
        self.hists.append( self.h_vismass )
        self.hists.append( self.h_svfitmass )
        self.hists.append( self.h_mT )
        
        
    def fillHistos(self, tree, cut='', nEvents=999999999):
        print 'filling histograms: ' + self.name
        tree.Draw('tauMu.obj[0].pZeta()>>'+self.h_pzeta.GetName(),cut,'goff',nEvents)
        tree.Draw('tauMu.obj[0].mass()>>'+self.h_vismass.GetName(),cut,'goff',nEvents)
        tree.Draw('tauMu.obj[0].mTLeg2()>>'+self.h_mT.GetName(),cut,'goff',nEvents)

    def fillDiTau(self, diTau, weight=1):
        self.h_pzeta.Fill( diTau.pZeta(), weight)
        self.h_vismass.Fill( diTau.mass(), weight)
        self.h_svfitmass.Fill( diTau.massSVFit(), weight)
        self.h_mT.Fill( diTau.mTLeg2(), weight)
        
    def formatHistos(self, style ):
        for hist in self.hists:
            style.formatHisto( hist )
            
            
    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()
